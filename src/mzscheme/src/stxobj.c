/*
  MzScheme
  Copyright (c) 2000 Matthew Flatt
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "schpriv.h"
#include "schmach.h"

#define STX_GRAPH_FLAG 0x1

static Scheme_Object *syntax_p(int argc, Scheme_Object **argv);
static Scheme_Object *graph_syntax_p(int argc, Scheme_Object **argv);

static Scheme_Object *syntax_to_datum(int argc, Scheme_Object **argv);
static Scheme_Object *datum_to_syntax(int argc, Scheme_Object **argv);
static Scheme_Object *syntax_to_datum_wraps(int argc, Scheme_Object **argv);

static Scheme_Object *syntax_e(int argc, Scheme_Object **argv);
static Scheme_Object *syntax_line(int argc, Scheme_Object **argv);
static Scheme_Object *syntax_col(int argc, Scheme_Object **argv);
static Scheme_Object *syntax_src(int argc, Scheme_Object **argv);

static Scheme_Object *bound_eq(int argc, Scheme_Object **argv);
static Scheme_Object *free_eq(int argc, Scheme_Object **argv);

#define HAS_SUBSTX(obj) (SCHEME_PAIRP(obj) || SCHEME_VECTORP(obj) || SCHEME_BOXP(obj))

/* Wraps:

   A pair (cons <num> v) is a mark
   A pair (cons <sym> (cons <stx> v)) is a rename

   For object with sub-syntax:

   The wraps field is
   (cons <local wraps> <lazy markswraps>), aand
   #f means (cons null null)

   The reason we keep local wraps, even for parens,
   is that the wraps for a list might have to be used
   as wraps for #%application
*/

void scheme_init_stx(Scheme_Env *env)
{
  scheme_add_global_constant("syntax?", 
			     scheme_make_folding_prim(syntax_p,
						      "syntax?",
						      1, 1, 1),
			     env);
  scheme_add_global_constant("syntax-graph?", 
			     scheme_make_folding_prim(graph_syntax_p,
						      "syntax-graph?",
						      1, 1, 1),
			     env);

  scheme_add_global_constant("syntax->datum", 
			     scheme_make_folding_prim(syntax_to_datum,
						      "syntax->datum",
						      1, 1, 1),
			     env);
  scheme_add_global_constant("datum->syntax", 
			     scheme_make_folding_prim(datum_to_syntax,
						      "datum->syntax",
						      2, 2, 1),
			     env);

  scheme_add_global_constant("syntax->datum/wraps", 
			     scheme_make_folding_prim(syntax_to_datum_wraps,
						      "syntax->datum/wraps",
						      1, 1, 1),
			     env);

  scheme_add_global_constant("syntax-e", 
			     scheme_make_folding_prim(syntax_e,
						      "syntax-e",
						      1, 1, 1),
			     env);
  scheme_add_global_constant("syntax-line", 
			     scheme_make_folding_prim(syntax_line,
						      "syntax-line",
						      1, 1, 1),
			     env);
  scheme_add_global_constant("syntax-column", 
			     scheme_make_folding_prim(syntax_col,
						      "syntax-column",
						      1, 1, 1),
			     env);
  scheme_add_global_constant("syntax-source", 
			     scheme_make_folding_prim(syntax_src,
						      "syntax-source",
						      1, 1, 1),
			     env);

  scheme_add_global_constant("bound-identifier=?", 
			     scheme_make_folding_prim(bound_eq,
						      "bound-identifier=?",
						      2, 2, 1),
			     env);
  scheme_add_global_constant("free-identifier=?", 
			     scheme_make_folding_prim(free_eq,
						      "free-identifier=?",
						      2, 2, 1),
			     env);
}

Scheme_Object *scheme_make_stx(Scheme_Object *val, 
			       long line, long col, 
			       Scheme_Object *src)
{
  Scheme_Stx *stx;

  stx = MALLOC_ONE_TAGGED(Scheme_Stx);
  stx->type = scheme_stx_type;
  stx->val = val;
  stx->line = line;
  stx->col = col;
  stx->src = src;
  if (HAS_SUBSTX(val))
    stx->wraps = scheme_false;
  else
    stx->wraps = scheme_null;

  return (Scheme_Object *)stx;
}

Scheme_Object *scheme_make_graph_stx(Scheme_Object *stx, long line, long col)
{
  ((Scheme_Stx *)stx)->hash_code |= STX_GRAPH_FLAG;

  return stx;
}

static Scheme_Object *mark_id = scheme_make_integer(0);

Scheme_Object *scheme_new_mark()
{
  mark_id = scheme_add1(1, &mark_id);
  return mark_id;
}

Scheme_Object *add_remove_mark(Scheme_Object *wraps, Scheme_Object *m)
{
  if (!SCHEME_NUMBERP(m))
    scheme_signal_error("internal error: mark is not a number");

  if (SCHEME_PAIRP(wraps) &&
      SAME_OBJ(m, SCHEME_CAR(wraps)))
    return SCHEME_CDR(wraps);
  else
    return scheme_make_pair(m, wraps);
}

Scheme_Object *scheme_add_remove_mark(Scheme_Object *o, Scheme_Object *m)
{
  Scheme_Stx *stx = (Scheme_Stx *)o;
  Scheme_Object *wraps;

  if (HAS_SUBSTX(stx->val)) {
    Scheme_Object *here_wraps, *lazy_wraps;
    wraps = stx->wraps;

    here_wraps = SCHEME_FALSEP(stx->wraps) ? scheme_null : SCHEME_CAR(stx->wraps);
    here_wraps = add_remove_mark(here_wraps, m);
    lazy_wraps = SCHEME_FALSEP(stx->wraps) ? scheme_null : SCHEME_CDR(stx->wraps);
    lazy_wraps = add_remove_mark(lazy_wraps, m);
    
    wraps = scheme_make_pair(here_wraps, lazy_wraps);
  } else {
    wraps = add_remove_mark(stx->wraps, m);
  }

  stx = (Scheme_Stx *)scheme_make_stx(stx->val, stx->line, stx->col, stx->src);
  stx->wraps = wraps;

  return (Scheme_Object *)stx;
}

Scheme_Object *scheme_add_rename(Scheme_Object *o, Scheme_Object *name, Scheme_Object *newname)
{
  Scheme_Stx *stx = (Scheme_Stx *)o;
  Scheme_Object *wraps;

  if (HAS_SUBSTX(stx->val)) {
    Scheme_Object *here_wraps, *lazy_wraps;
    wraps = stx->wraps;

    here_wraps = SCHEME_FALSEP(stx->wraps) ? scheme_null : SCHEME_CAR(stx->wraps);
    here_wraps = scheme_make_pair(newname, scheme_make_pair(name, here_wraps));
    lazy_wraps = SCHEME_FALSEP(stx->wraps) ? scheme_null : SCHEME_CDR(stx->wraps);
    lazy_wraps = scheme_make_pair(newname, scheme_make_pair(name, lazy_wraps));

    wraps = scheme_make_pair(here_wraps, lazy_wraps);
  } else {
    wraps = scheme_make_pair(newname, scheme_make_pair(name, stx->wraps));
  }
  
  stx = (Scheme_Stx *)scheme_make_stx(stx->val, stx->line, stx->col, stx->src);
  stx->wraps = wraps;

  return (Scheme_Object *)stx;
}

static Scheme_Object *propagate_wraps(Scheme_Object *o, Scheme_Object *wl)
{
  while (!SCHEME_NULLP(wl)) {
    if (SCHEME_NUMBERP(SCHEME_CAR(wl))) {
      o = scheme_add_remove_mark(o, SCHEME_CAR(wl));
      wl = SCHEME_CDR(wl);
    } else {
      o = scheme_add_rename(o, SCHEME_CADR(wl), SCHEME_CAR(wl));
      wl = SCHEME_CDR(SCHEME_CDR(wl));
    }
  }

  return o;
}

Scheme_Object *scheme_stx_content(Scheme_Object *o)
{
  Scheme_Stx *stx = (Scheme_Stx *)o;

  if (HAS_SUBSTX(stx->val) 
      && !SCHEME_FALSEP(stx->wraps)
      && !SCHEME_NULLP(SCHEME_CDR(stx->wraps))) {
    Scheme_Object *v = stx->val, *result;
    Scheme_Object *wraps, *here_wraps;
    Scheme_Object *ml = scheme_null;
    
    here_wraps = SCHEME_CAR(stx->wraps);
    wraps = SCHEME_CDR(stx->wraps);
    
    /* Reverse the list of wraps, to preserve order: */
    while (!SCHEME_NULLP(wraps)) {
      if (SCHEME_NUMBERP(SCHEME_CAR(wraps))) {
	ml = scheme_make_pair(SCHEME_CAR(wraps), ml);
	wraps = SCHEME_CDR(wraps);
      } else {
	ml = scheme_make_pair(SCHEME_CAR(wraps), 
			      scheme_make_pair(SCHEME_CADR(wraps), ml));
	wraps = SCHEME_CDDR(wraps);
      }
    }

    if (SCHEME_PAIRP(v)) {
      Scheme_Object *last = NULL;
      while (SCHEME_PAIRP(v)) {
	result = propagate_wraps(SCHEME_CAR(v), ml);
	SCHEME_CAR(v) = result;
	last = v;
	v = SCHEME_CDR(v);
      }
      if (!SCHEME_NULLP(v)) {
	result = propagate_wraps(v, ml);
	if (last)
	  SCHEME_CDR(last) = result;
	else
	  stx->val = result;
      }
    } else if (SCHEME_BOXP(v)) {
      result = propagate_wraps(SCHEME_BOX_VAL(v), ml);
      SCHEME_BOX_VAL(v) = result;
    } else if (SCHEME_VECTORP(v)) {
      int size = SCHEME_VEC_SIZE(v), i;

      for (i = 0; i < size; i++) {
	result = propagate_wraps(SCHEME_VEC_ELS(v)[i], ml);
	SCHEME_VEC_ELS(v)[i] = result;
      }
    }

    stx->wraps = scheme_make_pair(here_wraps, scheme_null);
  }

  return stx->val;
}

static int same_marks(Scheme_Object *awl, Scheme_Object *bwl)
{
  while (1) {
    /* Skip over renames: */
    while (!SCHEME_NULLP(awl)
	   && !SCHEME_NUMBERP(SCHEME_CAR(awl)))
      awl = SCHEME_CDDR(awl);
    while (!SCHEME_NULLP(bwl)
	   && !SCHEME_NUMBERP(SCHEME_CAR(bwl)))
      bwl = SCHEME_CDDR(bwl);

    /* Either at end? Then the same only if both at end. */
    if (SCHEME_NULLP(awl) || SCHEME_NULLP(bwl))
      return (SCHEME_NULLP(awl) && SCHEME_NULLP(bwl));

    /* Same first mark? */
    if (!scheme_bin_eq(SCHEME_CAR(awl), SCHEME_CAR(bwl)))
      return 0;

    awl = SCHEME_CDR(awl);
    bwl = SCHEME_CDR(bwl);
  }
}

static Scheme_Object *resolve_env(Scheme_Object *a)
{
  Scheme_Object *wraps = ((Scheme_Stx *)a)->wraps;
  Scheme_Object *rename_stack = scheme_null;

  while (1) {
    if (SCHEME_NULLP(wraps)) {
      Scheme_Object *result = scheme_false;
      /* See rename case for info on rename_stack: */
      while (!SCHEME_NULLP(rename_stack)) {
	if (SAME_OBJ(SCHEME_CAAR(rename_stack), result))
	  result = SCHEME_CDR(SCHEME_CAR(rename_stack));
	rename_stack = SCHEME_CDR(rename_stack);
      }
      return result;
    } else if (SCHEME_NUMBERP(SCHEME_CAR(wraps)))
      wraps = SCHEME_CDR(wraps);
    else {
      /* Rename: */
      Scheme_Object *envname, *renamed;

      envname = SCHEME_CAR(wraps);

      renamed = SCHEME_CADR(wraps);
      wraps = SCHEME_CDDR(wraps);

      if (SAME_OBJ(SCHEME_STX_VAL(renamed), SCHEME_STX_VAL(a))) {
	if (same_marks(((Scheme_Stx *)renamed)->wraps, wraps)) {
	  Scheme_Object *other_env = resolve_env(renamed);
	  /* If it turns out that we're going to return
	     other_env, then return envname instead. */
	  rename_stack = scheme_make_pair(scheme_make_pair(other_env, envname),
					  rename_stack);
	}
      }
    }
  }
}

int scheme_stx_free_eq(Scheme_Object *a, Scheme_Object *b)
{
  Scheme_Object *asym, *bsym;

  if (!a || !b)
    return (a == b);

  if (SCHEME_STXP(a))
    asym = SCHEME_STX_VAL(a);
  else
    asym = a;
  if (SCHEME_STXP(b))
    bsym = SCHEME_STX_VAL(b);
  else
    bsym = b;

  /* Same name? */
  if (!SAME_OBJ(asym, bsym))
    return 0;

  if ((a == asym) || (b == bsym))
    return 1;
  
  a = resolve_env(a);
  b = resolve_env(b);

  /* Same binding environment? */
  return SAME_OBJ(a, b);
}

int scheme_stx_env_bound_eq(Scheme_Object *a, Scheme_Object *b, Scheme_Object *uid)
{
  Scheme_Object *asym, *bsym;

  if (!a || !b)
    return (a == b);

  if (SCHEME_STXP(a))
    asym = SCHEME_STX_VAL(a);
  else
    asym = a;
  if (SCHEME_STXP(b))
    bsym = SCHEME_STX_VAL(b);
  else
    bsym = b;

  /* Same name? */
  if (!SAME_OBJ(asym, bsym))
    return 0;

  if ((a == asym) || (b == bsym))
    return 1;

  if (!same_marks(((Scheme_Stx *)a)->wraps, ((Scheme_Stx *)b)->wraps))
    return 0;
  
  a = resolve_env(a);
  if (uid)
    b = uid;
  else
    b = resolve_env(b);

  /* Same binding environment? */
  return SAME_OBJ(a, b);
}

int scheme_stx_bound_eq(Scheme_Object *a, Scheme_Object *b)
{
  return scheme_stx_env_bound_eq(a, b, NULL);
}

int scheme_stx_list_length(Scheme_Object *list)
{
  int len;

  len = 0;
  while (!SCHEME_NULLP(list)) {
    len++;
    if (SCHEME_STXP(list))
      list = SCHEME_STX_VAL(list);
    if (SCHEME_PAIRP(list))
      list = SCHEME_CDR(list);
    else
      list = scheme_null;
  }

  return len;
}

int scheme_stx_proper_list_length(Scheme_Object *list)
{
  int len;
  Scheme_Object *turtle;

  if (SCHEME_STXP(list))
    list = SCHEME_STX_VAL(list);

  len = 0;
  turtle = list;
  while (SCHEME_PAIRP(list)) {
    len++;

    list = SCHEME_CDR(list);
    if (SCHEME_STXP(list))
      list = SCHEME_STX_VAL(list);

    if (!SCHEME_PAIRP(list))
      break;
    len++;
    list = SCHEME_CDR(list);
    if (SCHEME_STXP(list))
      list = SCHEME_STX_VAL(list);

    if (SAME_OBJ(turtle, list))
      break;

    turtle = SCHEME_CDR(turtle);
    if (SCHEME_STXP(turtle))
      list = SCHEME_STX_VAL(turtle);

  }
  
  if (SCHEME_NULLP(list))
    return len;

  return -1;
}

/*========================================================================*/
/*                           syntax->datum                                */
/*========================================================================*/

#ifdef DO_STACK_CHECK
static Scheme_Object *syntax_to_datum_inner(Scheme_Object *o, 
					    Scheme_Hash_Table **ht,
					    int with_marks);

static Scheme_Object *syntax_to_datum_k(void)
{
  Scheme_Process *p = scheme_current_process;
  Scheme_Object *o = (Scheme_Object *)p->ku.k.p1;
  Scheme_Hash_Table **ht = (Scheme_Hash_Table **)p->ku.k.p2;

  p->ku.k.p1 = NULL;
  p->ku.k.p2 = NULL;

  return syntax_to_datum_inner(o, ht, p->ku.k.i1);
}
#endif

static Scheme_Object *syntax_to_datum_inner(Scheme_Object *o, 
					    Scheme_Hash_Table **ht,
					    int with_marks)
{
  Scheme_Stx *stx = (Scheme_Stx *)o;
  Scheme_Object *ph, *v, *result;

#ifdef DO_STACK_CHECK
  {
# include "mzstkchk.h"
    {
# ifndef MZ_REAL_THREADS
      Scheme_Process *p = scheme_current_process;
# endif
      p->ku.k.p1 = (void *)o;
      p->ku.k.p2 = (void *)ht;
      p->ku.k.i1 = with_marks;
      return scheme_handle_stack_overflow(syntax_to_datum_k);
    }
  }
#endif
  SCHEME_USE_FUEL(1);

  if (stx->hash_code & STX_GRAPH_FLAG) {
    if (!*ht)
      *ht = scheme_hash_table(7, SCHEME_hash_ptr, 0, 0);
    
    ph = (Scheme_Object *)scheme_lookup_in_table(*ht, (char *)stx);

    if (ph)
      return ph;
    else {
      ph = scheme_alloc_small_object();
      ph->type = scheme_placeholder_type;
      
      scheme_add_to_table(*ht, (char *)stx, (void *)ph, 0);
    }
  } else 
    ph = NULL;

  if (with_marks) {
    /* Propagate marks: */
    scheme_stx_content((Scheme_Object *)stx);
  }
    
  v = stx->val;
  
  if (SCHEME_PAIRP(v)) {
    Scheme_Object *first = NULL, *last = NULL, *p;
    
    while (SCHEME_PAIRP(v)) {
      Scheme_Object *a;
      
      a = syntax_to_datum_inner(SCHEME_CAR(v), ht, with_marks);
      
      p = scheme_make_pair(a, scheme_null);
      
      if (last)
	SCHEME_CDR(last) = p;
      else
	first = p;
      last = p;
      v = SCHEME_CDR(v);
    }
    if (!SCHEME_NULLP(v)) {
      v = syntax_to_datum_inner(v, ht, with_marks);
      SCHEME_CDR(last) = v;
    }
    
    result = first;
  } else if (SCHEME_BOXP(v)) {
    v = syntax_to_datum_inner(SCHEME_BOX_VAL(v), ht, with_marks);
    result = scheme_box(v);
  } else if (SCHEME_VECTORP(v)) {
    int size = SCHEME_VEC_SIZE(v), i;
    Scheme_Object *r, *a;
    
    r = scheme_make_vector(size, NULL);
    
    for (i = 0; i < size; i++) {
      a = syntax_to_datum_inner(SCHEME_VEC_ELS(v)[i], ht, with_marks);
      SCHEME_VEC_ELS(r)[i] = a;
    }
    
    result = r;
  } else if (with_marks && SCHEME_SYMBOLP(v)) {
    result = scheme_make_pair(v, stx->wraps);
  } else
    result = v;

  if (ph)
    SCHEME_PTR_VAL(ph) = result;

  return result;
}

Scheme_Object *scheme_syntax_to_datum(Scheme_Object *stx, int with_marks)
{
  Scheme_Hash_Table *ht = NULL;
  Scheme_Object *v;

  v = syntax_to_datum_inner(stx, &ht, with_marks);

  if (ht)
    v = scheme_resolve_placeholders(v, 0);

  return v;
}

/*========================================================================*/
/*                           datum->syntax                                */
/*========================================================================*/


#ifdef DO_STACK_CHECK
static Scheme_Object *datum_to_syntax_inner(Scheme_Object *o, 
					    Scheme_Stx *stx,
					    Scheme_Hash_Table *ht);

static Scheme_Object *datum_to_syntax_k(void)
{
  Scheme_Process *p = scheme_current_process;
  Scheme_Object *o = (Scheme_Object *)p->ku.k.p1;
  Scheme_Stx *stx = (Scheme_Stx *)p->ku.k.p2;
  Scheme_Hash_Table *ht = (Scheme_Hash_Table *)p->ku.k.p3;

  p->ku.k.p1 = NULL;
  p->ku.k.p2 = NULL;
  p->ku.k.p3 = NULL;

  return datum_to_syntax_inner(o, stx, ht);
}
#endif

static Scheme_Object *datum_to_syntax_inner(Scheme_Object *o, 
					    Scheme_Stx *stx,
					    Scheme_Hash_Table *ht)
{
  Scheme_Object *result, *ph = NULL;

  if (SCHEME_STXP(o))
    return o;

#ifdef DO_STACK_CHECK
  {
# include "mzstkchk.h"
    {
# ifndef MZ_REAL_THREADS
      Scheme_Process *p = scheme_current_process;
# endif
      p->ku.k.p1 = (void *)o;
      p->ku.k.p2 = (void *)stx;
      p->ku.k.p3 = (void *)ht;
      return scheme_handle_stack_overflow(datum_to_syntax_k);
    }
  }
#endif

  SCHEME_USE_FUEL(1);

  if (ht) {
    if (HAS_SUBSTX(o)) {
      Scheme_Bucket *b;
      b = scheme_bucket_from_table(ht, (const char *)o);
      
      if ((long)b->val != 1) {
	if ((long)b->val & 0x1) {
	  ph = scheme_alloc_small_object();
	  ph->type = scheme_placeholder_type;
	  b->val = (void *)ph;
	} else {
	  return (Scheme_Object *)b->val;
	}
      }
    }
  }

  if (SCHEME_PAIRP(o)) {
    Scheme_Object *first = NULL, *last = NULL, *p;
    
    while (SCHEME_PAIRP(o)) {
      Scheme_Object *a;
      
      if (ht && last) {
	if ((long)scheme_lookup_in_table(ht, (const char *)o) != 1) {
	  /* cdr is shared. Stop here. */
	  break;
	}
      }

      a = datum_to_syntax_inner(SCHEME_CAR(o), stx, ht);
      
      p = scheme_make_pair(a, scheme_null);
      
      if (last)
	SCHEME_CDR(last) = p;
      else
	first = p;
      last = p;
      o = SCHEME_CDR(o);
    }
    if (!SCHEME_NULLP(o)) {
      o = datum_to_syntax_inner(o, stx, ht);
      SCHEME_CDR(last) = o;
    }

    result = first;
  } else if (SCHEME_BOXP(o)) {
    o = datum_to_syntax_inner(SCHEME_PTR_VAL(o), stx, ht);
    result = scheme_box(o);
  } else if (SCHEME_VECTORP(o)) {
    int size = SCHEME_VEC_SIZE(o), i;
    Scheme_Object *a;

    result = scheme_make_vector(size, NULL);
    
    for (i = 0; i < size; i++) {
      a = datum_to_syntax_inner(SCHEME_VEC_ELS(o)[i], stx, ht);
      SCHEME_VEC_ELS(result)[i] = a;
    }
  } else {
    result = o;
  }

  if (SCHEME_FALSEP((Scheme_Object *)stx))
    result = scheme_make_stx(result, -1, -1, scheme_false);
  else
    result = scheme_make_stx(result, stx->line, stx->col, stx->src);
  
  if (HAS_SUBSTX(SCHEME_STX_VAL(result)))
    ((Scheme_Stx *)result)->wraps = scheme_false;
  else
    ((Scheme_Stx *)result)->wraps = scheme_null;
  
  if (ph) {
    ((Scheme_Stx *)result)->hash_code |= STX_GRAPH_FLAG;
    SCHEME_PTR_VAL(ph) = result;
  }

  return result;
}

Scheme_Object *scheme_datum_to_syntax(Scheme_Object *o, Scheme_Object *stx)
{
  Scheme_Hash_Table *ht;
  Scheme_Object *v;

  if (!SCHEME_FALSEP(stx) && !SCHEME_STXP(stx))
    return o;

  ht = scheme_setup_datum_graph(o, 0);

  v = datum_to_syntax_inner(o, (Scheme_Stx *)stx, ht);

  if (ht)
    v = scheme_resolve_placeholders(v, 1);

  return v;
}

/*========================================================================*/
/*                    Scheme functions and helpers                        */
/*========================================================================*/

static Scheme_Object *syntax_p(int argc, Scheme_Object **argv)
{
  return SCHEME_STXP(argv[0]) ? scheme_true : scheme_false;
}

static Scheme_Object *graph_syntax_p(int argc, Scheme_Object **argv)
{
  if (!SCHEME_STXP(argv[0]))
    scheme_wrong_type("syntax-graph?", "syntax", 0, argc, argv);

  return ((((Scheme_Stx *)argv[0])->hash_code & STX_GRAPH_FLAG)
	  ? scheme_true
	  : scheme_false);
}


static Scheme_Object *syntax_to_datum(int argc, Scheme_Object **argv)
{
  if (!SCHEME_STXP(argv[0]))
    scheme_wrong_type("syntax->datum", "syntax", 0, argc, argv);
    
  return scheme_syntax_to_datum(argv[0], 0);
}

static Scheme_Object *syntax_to_datum_wraps(int argc, Scheme_Object **argv)
{
  if (!SCHEME_STXP(argv[0]))
    scheme_wrong_type("syntax->datum/wraps", "syntax", 0, argc, argv);
    
  return scheme_syntax_to_datum(argv[0], 1);
}

static Scheme_Object *datum_to_syntax(int argc, Scheme_Object **argv)
{
  if (!SCHEME_FALSEP(argv[1]) && !SCHEME_STXP(argv[1]))
    scheme_wrong_type("datum->syntax", "syntax or #f", 1, argc, argv);
    
  return scheme_datum_to_syntax(argv[0], argv[1]);
}


static Scheme_Object *syntax_e(int argc, Scheme_Object **argv)
{
  if (!SCHEME_STXP(argv[0]))
    scheme_wrong_type("syntax-e", "syntax", 0, argc, argv);
    
  return scheme_stx_content(argv[0]);
}

static Scheme_Object *syntax_line(int argc, Scheme_Object **argv)
{
  Scheme_Stx *stx = (Scheme_Stx *)argv[0];

  if (!SCHEME_STXP(argv[0]))
    scheme_wrong_type("syntax-line", "syntax", 0, argc, argv);
    
  return scheme_make_integer(stx->line);
}

static Scheme_Object *syntax_col(int argc, Scheme_Object **argv)
{
  Scheme_Stx *stx = (Scheme_Stx *)argv[0];

  if (!SCHEME_STXP(argv[0]))
    scheme_wrong_type("syntax-column", "syntax", 0, argc, argv);
    
  return scheme_make_integer(stx->col);
}

static Scheme_Object *syntax_src(int argc, Scheme_Object **argv)
{
  Scheme_Stx *stx = (Scheme_Stx *)argv[0];

  if (!SCHEME_STXP(argv[0]))
    scheme_wrong_type("syntax-src", "syntax", 0, argc, argv);

  return stx->src;
}


static Scheme_Object *bound_eq(int argc, Scheme_Object **argv)
{
  if (!SCHEME_STXP(argv[0]))
    scheme_wrong_type("bound-identifier=?", "syntax", 0, argc, argv);
  if (!SCHEME_STXP(argv[1]))
    scheme_wrong_type("bound-identifier=?", "syntax", 1, argc, argv);

  return (scheme_stx_bound_eq(argv[0], argv[1])
	  ? scheme_true
	  : scheme_false);
}

static Scheme_Object *free_eq(int argc, Scheme_Object **argv)
{
  if (!SCHEME_STXP(argv[0]))
    scheme_wrong_type("free-identifier=?", "syntax", 0, argc, argv);
  if (!SCHEME_STXP(argv[1]))
    scheme_wrong_type("free-identifier=?", "syntax", 1, argc, argv);

  return (scheme_stx_free_eq(argv[0], argv[1])
	  ? scheme_true
	  : scheme_false);
}
