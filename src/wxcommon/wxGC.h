#ifndef GC_CPP_H
#define GC_CPP_H

/****************************************************************************
Based On:

C++ Interface to the Boehm Collector

    John R. Ellis and Jesse Hull 
    Last modified on Wed Jan  4 16:30:20 PST 1995 by ellis


Copyright (c) 1994 by Xerox Corporation.  All rights reserved.
 
THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 
Permission is hereby granted to use or copy this program for any
purpose, provided the above notices are retained on all copies.
Permission to modify the code and to distribute modified code is
granted, provided the above notices are retained, and a notice that
the code was modified is included with the above copyright notice.
****************************************************************************/

enum GCPlacement {UseGC, AtomicGC};

typedef void (*GCCleanUpFunc)(void* obj, void* clientData);

extern "C" {
  void gc_mark_external_invalid(void *);
  void GC_cleanup(void *obj, void *displ);
};

#include "gc.h"

#ifdef USE_SENORA_GC
extern void *GC_cpp_malloc(size_t);
#endif

/********************* The `gc' class *************************/

class gc 
{
public:
  void *__gc_external;

  inline gc();
  inline gc(int cleanup);
  inline virtual ~gc();
  inline void install_cleanup();

  inline void *operator new(size_t size);
  inline void *operator new(size_t size, GCPlacement gcp);
  inline void operator delete(void *obj);
#ifdef OPERATOR_NEW_ARRAY
  inline void *operator new[](size_t size);
  inline void *operator new[](size_t size, GCPlacement gcp);
  inline void operator delete[](void *obj);
#endif
};

/***** Constructors and Destructors: ******/

inline gc::gc(void)
{
  __gc_external = NULL;
  install_cleanup();
}

inline gc::gc(int cleanup) {
  __gc_external = NULL;
  if (cleanup)
    install_cleanup();
}

inline gc::~gc(void)
{
  GC_register_finalizer_ignore_self(this, 0, 0, 0, 0);
  if (__gc_external) 
    gc_mark_external_invalid(__gc_external);
}

inline void gc::install_cleanup(void)
{
  register void *base = GC_base((void *)this);

  if (base) {
    GC_finalization_proc old_fn;
    void *old_data;
    GC_register_finalizer_ignore_self(base, GC_cleanup,
				      (void *)((char *)this - (char *)base), 
				      &old_fn, &old_data);
    if (old_fn) {
      /* Put the old one back. We don't need to register. */
      GC_register_finalizer_ignore_self(base, old_fn, old_data, 
					&old_fn, &old_data);
    }
  }
}

/***** Allocators: ******/

inline void *gc::operator new(size_t size)
{
#ifdef USE_SENORA_GC
  return GC_cpp_malloc(size);
#else
  return GC_malloc(size);
#endif
}

inline void *gc::operator new(size_t size, GCPlacement gcp)
{
  if (gcp == AtomicGC) 
    return GC_malloc_atomic(size);
  else
    return GC_malloc(size);
}

inline void gc::operator delete(void */*obj*/) 
{
}


#ifdef OPERATOR_NEW_ARRAY
inline void *gc::operator new[](size_t size) {
  return gc::operator new(size);
}
    
inline void *gc::operator new[](size_t size, GCPlacement gcp) {
  return gc::operator new(size, gcp);
}

inline void gc::operator delete[](void *obj) {
  gc::operator delete(obj);
}
#endif


/*************** For objects not derived from `gc' ***********************/

inline void *operator new(size_t size, GCPlacement gcp)
{
  void *obj;
  
  if (gcp == AtomicGC)
    obj = GC_malloc_atomic(size);
  else
    obj = GC_malloc(size);

  return obj;
}
        

#ifdef OPERATOR_NEW_ARRAY
inline void *operator new[](size_t size, GCPlacement gcp)
{
  return ::operator new(size, gcp);
}
#endif

#endif /* GC_CPP_H */
