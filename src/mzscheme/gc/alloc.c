/*
 * Copyright 1988, 1989 Hans-J. Boehm, Alan J. Demers
 * Copyright (c) 1991-1996 by Xerox Corporation.  All rights reserved.
 * Copyright (c) 1998 by Silicon Graphics.  All rights reserved.
 * Copyright (c) 1999 by Hewlett-Packard Company. All rights reserved.
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */


# include "private/gc_priv.h"

# include <stdio.h>
# if !defined(MACOS) && !defined(MSWINCE)
#   include <signal.h>
#   include <sys/types.h>
# endif

/*
 * Separate free lists are maintained for different sized objects
 * up to MAXOBJSZ.
 * The call GC_allocobj(i,k) ensures that the freelist for
 * kind k objects of size i points to a non-empty
 * free list. It returns a pointer to the first entry on the free list.
 * In a single-threaded world, GC_allocobj may be called to allocate
 * an object of (small) size i as follows:
 *
 *            opp = &(GC_objfreelist[i]);
 *            if (*opp == 0) GC_allocobj(i, NORMAL);
 *            ptr = *opp;
 *            *opp = obj_link(ptr);
 *
 * Note that this is very fast if the free list is non-empty; it should
 * only involve the execution of 4 or 5 simple instructions.
 * All composite objects on freelists are cleared, except for
 * their first word.
 */

/*
 *  The allocator uses GC_allochblk to allocate large chunks of objects.
 * These chunks all start on addresses which are multiples of
 * HBLKSZ.   Each allocated chunk has an associated header,
 * which can be located quickly based on the address of the chunk.
 * (See headers.c for details.) 
 * This makes it possible to check quickly whether an
 * arbitrary address corresponds to an object administered by the
 * allocator.
 */

word GC_non_gc_bytes = 0;  /* Number of bytes not intended to be collected */

word GC_gc_no = 0;

#ifndef SMALL_CONFIG
  int GC_incremental = 0;  /* By default, stop the world.	*/
#endif

int GC_parallel = FALSE;   /* By default, parallel GC is off.	*/

int GC_full_freq = 19;	   /* Every 20th collection is a full	*/
			   /* collection, whether we need it 	*/
			   /* or not.			        */

GC_bool GC_need_full_gc = FALSE;
			   /* Need full GC do to heap growth.	*/

word GC_used_heap_size_after_full = 0;

char * GC_copyright[] =
{"Copyright 1988,1989 Hans-J. Boehm and Alan J. Demers ",
"Copyright (c) 1991-1995 by Xerox Corporation.  All rights reserved. ",
"Copyright (c) 1996-1998 by Silicon Graphics.  All rights reserved. ",
"Copyright (c) 1999-2000 by Hewlett-Packard Company.  All rights reserved. ",
"THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY",
" EXPRESSED OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.",
"See source code for details." };

# include "version.h"

/* some more variables */

extern signed_word GC_mem_found;  /* Number of reclaimed longwords	*/
				  /* after garbage collection      	*/

GC_bool GC_dont_expand = 0;

word GC_free_space_divisor = 4; /* PLTSCHEME: 3 -> 4 */

extern GC_bool GC_collection_in_progress();
		/* Collection is in progress, or was abandoned.	*/

int GC_never_stop_func GC_PROTO((void)) { return(0); }

CLOCK_TYPE GC_start_time;  	/* Time at which we stopped world.	*/
				/* used only in GC_timeout_stop_func.	*/

int GC_n_attempts = 0;		/* Number of attempts at finishing	*/
				/* collection within TIME_LIMIT		*/

#ifdef SMALL_CONFIG
#   define GC_timeout_stop_func GC_never_stop_func
#else
  int GC_timeout_stop_func GC_PROTO((void))
  {
    CLOCK_TYPE current_time;
    static unsigned count = 0;
    unsigned long time_diff;
    
    if ((count++ & 3) != 0) return(0);
    GET_TIME(current_time);
    time_diff = MS_TIME_DIFF(current_time,GC_start_time);
    if (time_diff >= TIME_LIMIT) {
#   	ifdef CONDPRINT
	  if (GC_print_stats) {
	    GC_printf0("Abandoning stopped marking after ");
	    GC_printf1("%lu msecs", (unsigned long)time_diff);
	    GC_printf1("(attempt %d)\n", (unsigned long) GC_n_attempts);
	  }
#	endif
    	return(1);
    }
    return(0);
  }
#endif /* !SMALL_CONFIG */

/* Return the minimum number of words that must be allocated between	*/
/* collections to amortize the collection cost.				*/
static word min_words_allocd()
{
#   ifdef THREADS
 	/* We punt, for now. */
 	register signed_word stack_size = 10000;
#   else
        int dummy;
        register signed_word stack_size = (ptr_t)(&dummy) - GC_stackbottom;
#   endif
    word total_root_size;  	    /* includes double stack size,	*/
    				    /* since the stack is expensive	*/
    				    /* to scan.				*/
    word scan_size;		/* Estimate of memory to be scanned 	*/
				/* during normal GC.			*/
    
    if (stack_size < 0) stack_size = -stack_size;
    total_root_size = 2 * stack_size + GC_root_size;
    scan_size = BYTES_TO_WORDS(GC_heapsize - GC_large_free_bytes
			       + (GC_large_free_bytes >> 2)
				   /* use a bit more of large empty heap */
			       + total_root_size);
    if (GC_incremental) {
        return scan_size / (2 * GC_free_space_divisor);
    } else {
        return scan_size / GC_free_space_divisor;
    }
}

/* Return the number of words allocated, adjusted for explicit storage	*/
/* management, etc..  This number is used in deciding when to trigger	*/
/* collections.								*/
word GC_adj_words_allocd()
{
    register signed_word result;
    register signed_word expl_managed =
    		BYTES_TO_WORDS((long)GC_non_gc_bytes
    				- (long)GC_non_gc_bytes_at_gc);
    
    /* Don't count what was explicitly freed, or newly allocated for	*/
    /* explicit management.  Note that deallocating an explicitly	*/
    /* managed object should not alter result, assuming the client	*/
    /* is playing by the rules.						*/
    result = (signed_word)GC_words_allocd
    	     - (signed_word)GC_mem_freed - expl_managed;
    if (result > (signed_word)GC_words_allocd) {
        result = GC_words_allocd;
    	/* probably client bug or unfortunate scheduling */
    }
    result += GC_words_finalized;
    	/* We count objects enqueued for finalization as though they	*/
    	/* had been reallocated this round. Finalization is user	*/
    	/* visible progress.  And if we don't count this, we have	*/
    	/* stability problems for programs that finalize all objects.	*/
    result += GC_words_wasted;
     	/* This doesn't reflect useful work.  But if there is lots of	*/
     	/* new fragmentation, the same is probably true of the heap,	*/
     	/* and the collection will be correspondingly cheaper.		*/
    if (result < (signed_word)(GC_words_allocd >> 3)) {
    	/* Always count at least 1/8 of the allocations.  We don't want	*/
    	/* to collect too infrequently, since that would inhibit	*/
    	/* coalescing of free storage blocks.				*/
    	/* This also makes us partially robust against client bugs.	*/
        return(GC_words_allocd >> 3);
    } else {
        return(result);
    }
}


/* Clear up a few frames worth of garbage left at the top of the stack.	*/
/* This is used to prevent us from accidentally treating garbade left	*/
/* on the stack by other parts of the collector as roots.  This 	*/
/* differs from the code in misc.c, which actually tries to keep the	*/
/* stack clear of long-lived, client-generated garbage.			*/
void GC_clear_a_few_frames()
{
#   define NWORDS 64
    word frames[NWORDS];
    register int i;
    
    for (i = 0; i < NWORDS; i++) frames[i] = 0;
}

/* Have we allocated enough to amortize a collection? */
GC_bool GC_should_collect()
{
    return(GC_adj_words_allocd() >= min_words_allocd());
}


void GC_notify_full_gc()
{
    if (GC_start_call_back != (void (*) GC_PROTO((void)))0) {
	(*GC_start_call_back)();
    }
}

GC_bool GC_is_full_gc = FALSE;

/* 
 * Initiate a garbage collection if appropriate.
 * Choose judiciously
 * between partial, full, and stop-world collections.
 * Assumes lock held, signals disabled.
 */
void GC_maybe_gc()
{
    static int n_partial_gcs = 0;

    if (GC_should_collect()) {
        if (!GC_incremental) {
	    GC_notify_full_gc();
            GC_gcollect_inner();
            n_partial_gcs = 0;
            return;
        } else if (GC_need_full_gc || n_partial_gcs >= GC_full_freq) {
#   	    ifdef CONDPRINT
	      if (GC_print_stats) {
	        GC_printf2(
	          "***>Full mark for collection %lu after %ld allocd bytes\n",
     		  (unsigned long) GC_gc_no+1,
	   	  (long)WORDS_TO_BYTES(GC_words_allocd));
	      }
#           endif
	    GC_promote_black_lists();
#   	    ifdef PARALLEL_MARK
		GC_wait_for_reclaim();
#   	    endif
	    (void)GC_reclaim_all((GC_stop_func)0, TRUE);
	    GC_clear_marks();
            n_partial_gcs = 0;
	    GC_notify_full_gc();
 	    GC_is_full_gc = TRUE;
        } else {
            n_partial_gcs++;
        }
        /* We try to mark with the world stopped.	*/
        /* If we run out of time, this turns into	*/
        /* incremental marking.			*/
        GET_TIME(GC_start_time);
        if (GC_stopped_mark(GC_timeout_stop_func)) {
#           ifdef SAVE_CALL_CHAIN
                GC_save_callers(GC_last_stack);
#           endif
            GC_finish_collection();
        } else {
	    if (!GC_is_full_gc) {
		/* Count this as the first attempt */
	        GC_n_attempts++;
	    }
	}
    }
}


/* PLTSCHEME: notification callback for starting/ending a GC */
void (*GC_collect_start_callback)(void) = NULL;
void (*GC_collect_end_callback)(void) = NULL;

/*
 * Stop the world garbage collection.  Assumes lock held, signals disabled.
 * If stop_func is not GC_never_stop_func, then abort if stop_func returns TRUE.
 */
GC_bool GC_try_to_collect_inner(stop_func)
GC_stop_func stop_func;
{
    /* PLTSCHEME */
    if (GC_collect_start_callback)
      GC_collect_start_callback();

    if (GC_incremental && GC_collection_in_progress()) {
#   ifdef CONDPRINT
      if (GC_print_stats) {
	GC_printf0(
	    "GC_try_to_collect_inner: finishing collection in progress\n");
      }
#   endif /* CONDPRINT */
      /* Just finish collection already in progress.	*/
    	while(GC_collection_in_progress()) {
    	    if (stop_func()) return(FALSE);
    	    GC_collect_a_little_inner(1);
    	}
    }
#   ifdef CONDPRINT
      if (GC_print_stats) {
	GC_printf2(
	   "Initiating full world-stop collection %lu after %ld allocd bytes\n",
	   (unsigned long) GC_gc_no+1,
	   (long)WORDS_TO_BYTES(GC_words_allocd));
      }
#   endif
    GC_promote_black_lists();
    /* Make sure all blocks have been reclaimed, so sweep routines	*/
    /* don't see cleared mark bits.					*/
    /* If we're guaranteed to finish, then this is unnecessary.		*/
    /* In the find_leak case, we have to finish to guarantee that 	*/
    /* previously unmarked objects are not reported as leaks.		*/
#       ifdef PARALLEL_MARK
	    GC_wait_for_reclaim();
#       endif
 	if ((GC_find_leak || stop_func != GC_never_stop_func)
	    && !GC_reclaim_all(stop_func, FALSE)) {
	    /* Aborted.  So far everything is still consistent.	*/
	    return(FALSE);
	}
    GC_invalidate_mark_state();  /* Flush mark stack.	*/
    GC_clear_marks();
#   ifdef SAVE_CALL_CHAIN
        GC_save_callers(GC_last_stack);
#   endif
    GC_is_full_gc = TRUE;
    if (!GC_stopped_mark(stop_func)) {
      if (!GC_incremental) {
    	/* We're partially done and have no way to complete or use 	*/
    	/* current work.  Reestablish invariants as cheaply as		*/
    	/* possible.							*/
    	GC_invalidate_mark_state();
	GC_unpromote_black_lists();
      } /* else we claim the world is already still consistent.  We'll 	*/
        /* finish incrementally.					*/
      return(FALSE);
    }
    GC_finish_collection();

    /* PLTSCHEME */
    if (GC_collect_end_callback)
      GC_collect_end_callback();

    return(TRUE);
}



/*
 * Perform n units of garbage collection work.  A unit is intended to touch
 * roughly GC_RATE pages.  Every once in a while, we do more than that.
 * This needa to be a fairly large number with our current incremental
 * GC strategy, since otherwise we allocate too much during GC, and the
 * cleanup gets expensive.
 */
# define GC_RATE 10 
# define MAX_PRIOR_ATTEMPTS 1
 	/* Maximum number of prior attempts at world stop marking	*/
 	/* A value of 1 means that we finish the seconf time, no matter */
 	/* how long it takes.  Doesn't count the initial root scan	*/
 	/* for a full GC.						*/

int GC_deficit = 0;	/* The number of extra calls to GC_mark_some	*/
			/* that we have made.				*/

void GC_collect_a_little_inner(n)
int n;
{
    register int i;
    
    if (GC_incremental && GC_collection_in_progress()) {
    	for (i = GC_deficit; i < GC_RATE*n; i++) {
    	    if (GC_mark_some((ptr_t)0)) {
    	        /* Need to finish a collection */
#     		ifdef SAVE_CALL_CHAIN
        	    GC_save_callers(GC_last_stack);
#     		endif
		if (GC_n_attempts < MAX_PRIOR_ATTEMPTS) {
		  GET_TIME(GC_start_time);
		  if (!GC_stopped_mark(GC_timeout_stop_func)) {
		    GC_n_attempts++;
		    break;
		  }
		} else {
		  (void)GC_stopped_mark(GC_never_stop_func);
		}
    	        GC_finish_collection();
    	        break;
    	    }
    	}
    	if (GC_deficit > 0) GC_deficit -= GC_RATE*n;
	if (GC_deficit < 0) GC_deficit = 0;
    } else {
        GC_maybe_gc();
    }
}

int GC_collect_a_little GC_PROTO(())
{
    int result;
    DCL_LOCK_STATE;

    DISABLE_SIGNALS();
    LOCK();
    GC_collect_a_little_inner(1);
    result = (int)GC_collection_in_progress();
    UNLOCK();
    ENABLE_SIGNALS();
    return(result);
}

/*
 * Assumes lock is held, signals are disabled.
 * We stop the world.
 * If stop_func() ever returns TRUE, we may fail and return FALSE.
 * Increment GC_gc_no if we succeed.
 */
GC_bool GC_stopped_mark(stop_func)
GC_stop_func stop_func;
{
    register int i;
    int dummy;
#   ifdef PRINTTIMES
	CLOCK_TYPE start_time, current_time;
#   endif
	
    STOP_WORLD();
#   ifdef PRINTTIMES
	GET_TIME(start_time);
#   endif
#   ifdef CONDPRINT
      if (GC_print_stats) {
	GC_printf1("--> Marking for collection %lu ",
	           (unsigned long) GC_gc_no + 1);
	GC_printf2("after %lu allocd bytes + %lu wasted bytes\n",
	   	   (unsigned long) WORDS_TO_BYTES(GC_words_allocd),
	   	   (unsigned long) WORDS_TO_BYTES(GC_words_wasted));
      }
#   endif

    /* Mark from all roots.  */
        /* Minimize junk left in my registers and on the stack */
            GC_clear_a_few_frames();
            GC_noop(0,0,0,0,0,0);
	GC_initiate_gc();
	for(i = 0;;i++) {
	    if ((*stop_func)()) {
#   		    ifdef CONDPRINT
		      if (GC_print_stats) {
		    	GC_printf0("Abandoned stopped marking after ");
			GC_printf1("%lu iterations\n",
				   (unsigned long)i);
		      }
#		    endif
		    GC_deficit = i; /* Give the mutator a chance. */
	            START_WORLD();
	            return(FALSE);
	    }
	    if (GC_mark_some((ptr_t)(&dummy))) break;
	}
	
    GC_gc_no++;
#   ifdef PRINTSTATS
      GC_printf2("Collection %lu reclaimed %ld bytes",
		  (unsigned long) GC_gc_no - 1,
	   	  (long)WORDS_TO_BYTES(GC_mem_found));
#   else
#     ifdef CONDPRINT
        if (GC_print_stats) {
	  GC_printf1("Collection %lu finished", (unsigned long) GC_gc_no - 1);
	}
#     endif
#   endif /* !PRINTSTATS */
#   ifdef CONDPRINT
      if (GC_print_stats) {
        GC_printf1(" ---> heapsize = %lu bytes\n",
      	           (unsigned long) GC_heapsize);
        /* Printf arguments may be pushed in funny places.  Clear the	*/
        /* space.							*/
        GC_printf0("");
      }
#   endif  /* CONDPRINT  */

    /* Check all debugged objects for consistency */
        if (GC_debugging_started) {
            (*GC_check_heap)();
        }
    
#   ifdef PRINTTIMES
	GET_TIME(current_time);
	GC_printf1("World-stopped marking took %lu msecs\n",
	           MS_TIME_DIFF(current_time,start_time));
#   endif
    START_WORLD();
    return(TRUE);
}


/* Finish up a collection.  Assumes lock is held, signals are disabled,	*/
/* but the world is otherwise running.					*/
void GC_finish_collection()
{
#   ifdef PRINTTIMES
	CLOCK_TYPE start_time;
	CLOCK_TYPE finalize_time;
	CLOCK_TYPE done_time;
	
	GET_TIME(start_time);
	finalize_time = start_time;
#   endif

#   ifdef GATHERSTATS
        GC_mem_found = 0;
#   endif
#   if defined(LINUX) && defined(__ELF__) && !defined(SMALL_CONFIG)
	if (getenv("GC_PRINT_ADDRESS_MAP") != 0) {
	  GC_print_address_map();
	}
#   endif
    if (GC_find_leak) {
      /* Mark all objects on the free list.  All objects should be */
      /* marked when we're done.				   */
	{
	  register word size;		/* current object size		*/
	  register ptr_t p;	/* pointer to current object	*/
	  register struct hblk * h;	/* pointer to block containing *p */
	  register hdr * hhdr;
	  register int word_no;           /* "index" of *p in *q          */
	  int kind;

	  for (kind = 0; kind < GC_n_kinds; kind++) {
	    for (size = 1; size <= MAXOBJSZ; size++) {
	      for (p= GC_obj_kinds[kind].ok_freelist[size];
	           p != 0; p=obj_link(p)){
		h = HBLKPTR(p);
		hhdr = HDR(h);
		word_no = (((word *)p) - ((word *)h));
		set_mark_bit_from_hdr(hhdr, word_no);
	      }
	    }
	  }
	}
	GC_start_reclaim(TRUE);
	  /* The above just checks; it doesn't really reclaim anything. */
    }

    GC_finalize();
#   ifdef STUBBORN_ALLOC
      GC_clean_changing_list();
#   endif

#   ifdef PRINTTIMES
      GET_TIME(finalize_time);
#   endif

    /* Clear free list mark bits, in case they got accidentally marked   */
    /* Note: HBLKPTR(p) == pointer to head of block containing *p        */
    /* (or GC_find_leak is set and they were intentionally marked.)	 */
    /* Also subtract memory remaining from GC_mem_found count.           */
    /* Note that composite objects on free list are cleared.             */
    /* Thus accidentally marking a free list is not a problem;  only     */
    /* objects on the list itself will be marked, and that's fixed here. */
      {
	register word size;		/* current object size		*/
	register ptr_t p;	/* pointer to current object	*/
	register struct hblk * h;	/* pointer to block containing *p */
	register hdr * hhdr;
	register int word_no;           /* "index" of *p in *q          */
	int kind;

	for (kind = 0; kind < GC_n_kinds; kind++) {
	  for (size = 1; size <= MAXOBJSZ; size++) {
	    for (p= GC_obj_kinds[kind].ok_freelist[size];
	         p != 0; p=obj_link(p)){
		h = HBLKPTR(p);
		hhdr = HDR(h);
		word_no = (((word *)p) - ((word *)h));
		clear_mark_bit_from_hdr(hhdr, word_no);
#		ifdef GATHERSTATS
		    GC_mem_found -= size;
#		endif
	    }
	  }
	}
      }


#   ifdef PRINTSTATS
	GC_printf1("Bytes recovered before sweep - f.l. count = %ld\n",
	          (long)WORDS_TO_BYTES(GC_mem_found));
#   endif
    /* Reconstruct free lists to contain everything not marked */
        GC_start_reclaim(FALSE);
        if (GC_is_full_gc)  {
	    GC_used_heap_size_after_full = USED_HEAP_SIZE;
	    GC_need_full_gc = FALSE;
	} else {
	    GC_need_full_gc =
		 BYTES_TO_WORDS(USED_HEAP_SIZE - GC_used_heap_size_after_full)
		 > min_words_allocd();
	}

#   ifdef PRINTSTATS
	GC_printf2(
		  "Immediately reclaimed %ld bytes in heap of size %lu bytes",
	          (long)WORDS_TO_BYTES(GC_mem_found),
	          (unsigned long)GC_heapsize);
#	ifdef USE_MUNMAP
	  GC_printf1("(%lu unmapped)", GC_unmapped_bytes);
#	endif
	GC_printf2(
		"\n%lu (atomic) + %lu (composite) collectable bytes in use\n",
	        (unsigned long)WORDS_TO_BYTES(GC_atomic_in_use),
	        (unsigned long)WORDS_TO_BYTES(GC_composite_in_use));
#   endif

      GC_n_attempts = 0;
      GC_is_full_gc = FALSE;
    /* Reset or increment counters for next cycle */
      GC_words_allocd_before_gc += GC_words_allocd;
      GC_non_gc_bytes_at_gc = GC_non_gc_bytes;
      GC_words_allocd = 0;
      GC_words_wasted = 0;
      GC_mem_freed = 0;
      
#   ifdef USE_MUNMAP
      GC_unmap_old();
#   endif
#   ifdef PRINTTIMES
	GET_TIME(done_time);
	GC_printf2("Finalize + initiate sweep took %lu + %lu msecs\n",
	           MS_TIME_DIFF(finalize_time,start_time),
	           MS_TIME_DIFF(done_time,finalize_time));
#   endif
}

/* Externally callable routine to invoke full, stop-world collection */
# if defined(__STDC__) || defined(__cplusplus)
    int GC_try_to_collect(GC_stop_func stop_func)
# else
    int GC_try_to_collect(stop_func)
    GC_stop_func stop_func;
# endif
{
    int result;
    DCL_LOCK_STATE;
    
    GC_INVOKE_FINALIZERS();
    DISABLE_SIGNALS();
    LOCK();
    ENTER_GC();
    if (!GC_is_initialized) GC_init_inner();
    /* Minimize junk left in my registers */
      GC_noop(0,0,0,0,0,0);
    result = (int)GC_try_to_collect_inner(stop_func);
    EXIT_GC();
    UNLOCK();
    ENABLE_SIGNALS();
    if(result) GC_INVOKE_FINALIZERS();
    return(result);
}

void GC_gcollect GC_PROTO(())
{
    GC_notify_full_gc();
    (void)GC_try_to_collect(GC_never_stop_func);
}

word GC_n_heap_sects = 0;	/* Number of sections currently in heap. */

/*
 * Use the chunk of memory starting at p of size bytes as part of the heap.
 * Assumes p is HBLKSIZE aligned, and bytes is a multiple of HBLKSIZE.
 */
void GC_add_to_heap(p, bytes)
struct hblk *p;
word bytes;
{
    word words;
    hdr * phdr;
    
    if (GC_n_heap_sects >= MAX_HEAP_SECTS) {
    	ABORT("Too many heap sections: Increase MAXHINCR or MAX_HEAP_SECTS");
    }
    phdr = GC_install_header(p);
    if (0 == phdr) {
    	/* This is extremely unlikely. Can't add it.  This will		*/
    	/* almost certainly result in a	0 return from the allocator,	*/
    	/* which is entirely appropriate.				*/
    	return;
    }
    GC_heap_sects[GC_n_heap_sects].hs_start = (ptr_t)p;
    GC_heap_sects[GC_n_heap_sects].hs_bytes = bytes;
    GC_n_heap_sects++;
    words = BYTES_TO_WORDS(bytes);
    phdr -> hb_sz = words;
    phdr -> hb_map = (unsigned char *)1;   /* A value != GC_invalid_map	*/
    phdr -> hb_flags = 0;
    GC_freehblk(p);
    GC_heapsize += bytes;
    if ((ptr_t)p <= GC_least_plausible_heap_addr
        || GC_least_plausible_heap_addr == 0) {
        GC_least_plausible_heap_addr = (ptr_t)p - sizeof(word);
        	/* Making it a little smaller than necessary prevents	*/
        	/* us from getting a false hit from the variable	*/
        	/* itself.  There's some unintentional reflection	*/
        	/* here.						*/
    }
    if ((ptr_t)p + bytes >= GC_greatest_plausible_heap_addr) {
        GC_greatest_plausible_heap_addr = (ptr_t)p + bytes;
    }
}

# if !defined(NO_DEBUGGING)
void GC_print_heap_sects()
{
    register unsigned i;
    
    GC_printf1("Total heap size: %lu\n", (unsigned long) GC_heapsize);
    for (i = 0; i < GC_n_heap_sects; i++) {
        unsigned long start = (unsigned long) GC_heap_sects[i].hs_start;
        unsigned long len = (unsigned long) GC_heap_sects[i].hs_bytes;
        struct hblk *h;
        unsigned nbl = 0;
        
    	GC_printf3("Section %ld from 0x%lx to 0x%lx ", (unsigned long)i,
    		   start, (unsigned long)(start + len));
    	for (h = (struct hblk *)start; h < (struct hblk *)(start + len); h++) {
    	    if (GC_is_black_listed(h, HBLKSIZE)) nbl++;
    	}
    	GC_printf2("%lu/%lu blacklisted\n", (unsigned long)nbl,
    		   (unsigned long)(len/HBLKSIZE));
    }
}
# endif

ptr_t GC_least_plausible_heap_addr = (ptr_t)ONES;
ptr_t GC_greatest_plausible_heap_addr = 0;

ptr_t GC_max(x,y)
ptr_t x, y;
{
    return(x > y? x : y);
}

ptr_t GC_min(x,y)
ptr_t x, y;
{
    return(x < y? x : y);
}

# if defined(__STDC__) || defined(__cplusplus)
    void GC_set_max_heap_size(GC_word n)
# else
    void GC_set_max_heap_size(n)
    GC_word n;
# endif
{
    GC_max_heapsize = n;
}

GC_word GC_max_retries = 0;

/*
 * this explicitly increases the size of the heap.  It is used
 * internally, but may also be invoked from GC_expand_hp by the user.
 * The argument is in units of HBLKSIZE.
 * Tiny values of n are rounded up.
 * Returns FALSE on failure.
 */
GC_bool GC_expand_hp_inner(n)
word n;
{
    word bytes;
    struct hblk * space;
    word expansion_slop;	/* Number of bytes by which we expect the */
    				/* heap to expand soon.			  */

    if (n < MINHINCR) n = MINHINCR;
    bytes = n * HBLKSIZE;
    /* Make sure bytes is a multiple of GC_page_size */
      {
	word mask = GC_page_size - 1;
	bytes += mask;
	bytes &= ~mask;
      }
    
    if (GC_max_heapsize != 0 && GC_heapsize + bytes > GC_max_heapsize) {
        /* Exceeded self-imposed limit */
        return(FALSE);
    }
    space = GET_MEM(bytes);
    if( space == 0 ) {
	return(FALSE);
    }
#   ifdef CONDPRINT
      if (GC_print_stats) {
	GC_printf2("Increasing heap size by %lu after %lu allocated bytes\n",
	           (unsigned long)bytes,
	           (unsigned long)WORDS_TO_BYTES(GC_words_allocd));
# 	ifdef UNDEFINED
	  GC_printf1("Root size = %lu\n", GC_root_size);
	  GC_print_block_list(); GC_print_hblkfreelist();
	  GC_printf0("\n");
#	endif
      }
#   endif
    expansion_slop = 8 * WORDS_TO_BYTES(min_words_allocd());
    if (5 * HBLKSIZE * MAXHINCR > expansion_slop) {
        expansion_slop = 5 * HBLKSIZE * MAXHINCR;
    }
    if (GC_last_heap_addr == 0 && !((word)space & SIGNB)
        || GC_last_heap_addr != 0 && GC_last_heap_addr < (ptr_t)space) {
        /* Assume the heap is growing up */
        GC_greatest_plausible_heap_addr =
            GC_max(GC_greatest_plausible_heap_addr,
                   (ptr_t)space + bytes + expansion_slop);
    } else {
        /* Heap is growing down */
        GC_least_plausible_heap_addr =
            GC_min(GC_least_plausible_heap_addr,
                   (ptr_t)space - expansion_slop);
    }
    GC_prev_heap_addr = GC_last_heap_addr;
    GC_last_heap_addr = (ptr_t)space;
    GC_add_to_heap(space, bytes);
    return(TRUE);
}

/* PLTSCHEME: To report out-of-memory in an app- or platform-specific way. */
void (*GC_out_of_memory)(void) = NULL;

/* Really returns a bool, but it's externally visible, so that's clumsy. */
/* Arguments is in bytes.						*/
# if defined(__STDC__) || defined(__cplusplus)
  int GC_expand_hp(size_t bytes)
# else
  int GC_expand_hp(bytes)
  size_t bytes;
# endif
{
    int result;
    DCL_LOCK_STATE;
    
    DISABLE_SIGNALS();
    LOCK();
    if (!GC_is_initialized) GC_init_inner();
    result = (int)GC_expand_hp_inner(divHBLKSZ((word)bytes));
    if (result) GC_requested_heapsize += bytes;
    UNLOCK();
    ENABLE_SIGNALS();
    return(result);
}

unsigned GC_fail_count = 0;  
			/* How many consecutive GC/expansion failures?	*/
			/* Reset by GC_allochblk.			*/

GC_bool GC_collect_or_expand(needed_blocks, ignore_off_page)
word needed_blocks;
GC_bool ignore_off_page;
{
    if (!GC_incremental && !GC_dont_gc &&
	(GC_dont_expand && GC_words_allocd > 0 || GC_should_collect())) {
      GC_notify_full_gc();
      GC_gcollect_inner();
    } else {
      word blocks_to_get = GC_heapsize/(HBLKSIZE*GC_free_space_divisor)
      			   + needed_blocks;
      
      if (blocks_to_get > MAXHINCR) {
          word slop;
          
          if (ignore_off_page) {
              slop = 4;
          } else {
	      slop = 2*divHBLKSZ(BL_LIMIT);
	      if (slop > needed_blocks) slop = needed_blocks;
	  }
          if (needed_blocks + slop > MAXHINCR) {
              blocks_to_get = needed_blocks + slop;
          } else {
              blocks_to_get = MAXHINCR;
          }
      }
      if (!GC_expand_hp_inner(blocks_to_get)
        && !GC_expand_hp_inner(needed_blocks)) {
      	if (GC_fail_count++ < GC_max_retries) {
	    /* PLTSCHEME */
	    if (GC_out_of_memory)
	      GC_out_of_memory();

      	    WARN("Out of Memory!  Trying to continue ...\n", 0);
	    GC_notify_full_gc();
	    GC_gcollect_inner();
	} else {
#	    if !defined(AMIGA) || !defined(GC_AMIGA_FASTALLOC)
	      WARN("Out of Memory!  Returning NIL!\n", 0);
#	    endif
	    return(FALSE);
	}
      } else {
#	  ifdef CONDPRINT
            if (GC_fail_count && GC_print_stats) {
	      GC_printf0("Memory available again ...\n");
	    }
#	  endif
      }
    }
    return(TRUE);
}

/*
 * Make sure the object free list for sz is not empty.
 * Return a pointer to the first object on the free list.
 * The object MUST BE REMOVED FROM THE FREE LIST BY THE CALLER.
 * Assumes we hold the allocator lock and signals are disabled.
 *
 */
ptr_t GC_allocobj(sz, kind)
word sz;
int kind;
{
    register ptr_t * flh = &(GC_obj_kinds[kind].ok_freelist[sz]);
    
    if (sz == 0) return(0);

    while (*flh == 0) {
      ENTER_GC();
      /* Do our share of marking work */
        if(GC_incremental && !GC_dont_gc) GC_collect_a_little_inner(1);
      /* Sweep blocks for objects of this size */
          GC_continue_reclaim(sz, kind);
      EXIT_GC();
      if (*flh == 0) {
        GC_new_hblk(sz, kind);
      }
      if (*flh == 0) {
        ENTER_GC();
        if (!GC_collect_or_expand((word)1,FALSE)) {
	    EXIT_GC();
	    return(0);
	}
	EXIT_GC();
      }
    }
    
    return(*flh);
}
