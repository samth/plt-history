/*
  MzScheme
  Copyright (c) 1995 Matthew Flatt

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

  libscheme
  Copyright (c) 1994 Brent Benson
  All rights reserved.
*/

#include "schpriv.h"
#include "schminc.h"

#if defined(UNIX_LIMIT_STACK) || defined(UNIX_LIMIT_FDSET_SIZE)
# include <signal.h>
# include <sys/time.h>
# include <sys/resource.h>
#endif

#ifdef MZ_USE_IRIX_SPROCS
# include "../gc/gc.h"
#endif

#define GLOBAL_TABLE_SIZE 500
#define REFERENCES_TABLE_SIZE GLOBAL_TABLE_SIZE

/* #define TIME_STARTUP_PROCESS */

/* globals */
int scheme_allow_set_undefined;
int scheme_escape_continuations_only = 0; 

int scheme_starting_up;

Scheme_Object *scheme_local[MAX_CONST_LOCAL_POS][2];

static Scheme_Env *initial_env;

/* locals */
static Scheme_Env *make_env(Scheme_Env *base);
static void make_init_env(void);
static Scheme_Object *list_globals(int argc, Scheme_Object *argv[]);
static Scheme_Object *defined(int argc, Scheme_Object *argv[]);
static Scheme_Object *global_defined_value(int, Scheme_Object *[]);
static Scheme_Object *local_exp_time_value(int argc, Scheme_Object *argv[]);
static Scheme_Object *local_exp_time_bound_p(int argc, Scheme_Object *argv[]);
static Scheme_Object *local_exp_time_name(int argc, Scheme_Object *argv[]);
static Scheme_Object *id_macro(int argc, Scheme_Object *argv[]);

static Scheme_Object *write_variable(Scheme_Object *obj);
static Scheme_Object *read_variable(Scheme_Object *obj);
static Scheme_Object *write_local(Scheme_Object *obj);
static Scheme_Object *read_local(Scheme_Object *obj);
static Scheme_Object *read_local_unbox(Scheme_Object *obj);

#ifdef MZ_PRECISE_GC
static void register_traversers(void);
#endif

static int set_reference_ids = 0;
static int builtin_ref_counter = 0;

typedef struct Constant_Binding {
  MZTAG_IF_REQUIRED
  Scheme_Object *name;
  Scheme_Object *val;
  Scheme_Object *rename;
  short before;
  struct Constant_Binding *next;
} Constant_Binding;

#define ARBITRARY_USE 1
#define CONSTRAINED_USE 2
#define NOT_SETTABLE 4
#define WAS_SET_BANGED 8

typedef struct Compile_Data {
  char **stat_dists; /* (pos, depth) => used? */
  int *sd_depths;
  Constant_Binding *constants;
  int *use;
} Compile_Data;

typedef struct Scheme_Full_Comp_Env {
  Scheme_Comp_Env base;
  Compile_Data data;
} Scheme_Full_Comp_Env;
static void init_compile_data(Scheme_Comp_Env *env);

/* Precise GC WARNING: this macro produces unaligned pointers: */
#define COMPILE_DATA(e) (&((Scheme_Full_Comp_Env *)e)->data)

#ifdef MZ_REAL_THREADS
void *scheme_global_lock;
int scheme_global_lock_c;
#endif

static void skip_certain_things(Scheme_Object *o, Scheme_Close_Manager_Client *f, void *data)
{
  if ((o == scheme_orig_stdin_port)
      || (o == scheme_orig_stdout_port)
      || (o == scheme_orig_stderr_port))
    return;

  /* f is NULL for threads */
  if (f)
    f(o, data);
}

Scheme_Env *scheme_basic_env()
{
  Scheme_Env *env;

  if (scheme_main_process) {
    /* Reset everything: */
    scheme_do_close_managed(NULL, skip_certain_things);
    scheme_main_process = NULL;

    scheme_reset_finalizations();
    scheme_init_stack_check();
#ifndef MZ_PRECISE_GC
    scheme_init_setjumpup();
#endif

    scheme_make_process();
    scheme_init_error_escape_proc(scheme_current_process);

    env = scheme_make_empty_env();
    scheme_copy_from_original_env(env);

    scheme_set_param(scheme_config, MZCONFIG_ENV, (Scheme_Object *)env); 
    scheme_init_port_config();
    scheme_init_port_fun_config();
    scheme_init_error_config();
#ifndef NO_SCHEME_EXNS
    scheme_init_exn_config();
#endif

    return env;
  }

#ifdef UNIX_LIMIT_STACK
  {
    struct rlimit rl;
    
    getrlimit(RLIMIT_STACK, &rl);
    if (rl.rlim_cur > UNIX_LIMIT_STACK) {
      rl.rlim_cur = UNIX_LIMIT_STACK;
      setrlimit(RLIMIT_STACK, &rl);
    }
  }
#endif
#ifdef UNIX_LIMIT_FDSET_SIZE
  {
    struct rlimit rl;
    
    getrlimit(RLIMIT_NOFILE, &rl);
    if (rl.rlim_cur > FD_SETSIZE) {
      rl.rlim_cur = FD_SETSIZE;
      setrlimit(RLIMIT_NOFILE, &rl);
    }
  }
#endif

#ifdef MZ_USE_IRIX_SPROCS
  GC_INIT();
#endif

  scheme_starting_up = 1;

#ifndef MZ_PRECISE_GC
  scheme_init_setjumpup();
#endif

#ifdef TIME_STARTUP_PROCESS
   printf("#if 0\nbasic @ %ld\n", scheme_get_process_milliseconds());
#endif

#ifdef MZ_REAL_THREADS
  REGISTER_SO(scheme_global_lock);
  scheme_global_lock = SCHEME_MAKE_MUTEX();
#else
  scheme_init_stack_check();
#endif

  {
    int i, k;

#ifndef USE_TAGGED_ALLOCATION
    Scheme_Local *all;

    all = (Scheme_Local *)scheme_malloc_eternal(sizeof(Scheme_Local) * 2 * MAX_CONST_LOCAL_POS);
# ifdef MEMORY_COUNTING_ON
    scheme_misc_count += sizeof(Scheme_Local) * 2 * MAX_CONST_LOCAL_POS;
# endif    
#endif

    for (i = 0; i < MAX_CONST_LOCAL_POS; i++) {
      for (k = 0; k < 2; k++) {
	Scheme_Object *v;
	
#ifndef USE_TAGGED_ALLOCATION
	v = (Scheme_Object *)(all++);
#else
	v = (Scheme_Object *)scheme_malloc_eternal_tagged(sizeof(Scheme_Local));
#endif
	v->type = k + scheme_local_type;
	SCHEME_LOCAL_POS(v) = i;
	
	scheme_local[i][k] = v;
      }
    }
  }

  scheme_init_true_false();

#ifdef MZ_PRECISE_GC
  scheme_register_traversers();
  register_traversers();
  scheme_init_hash_key_procs();
#endif

#ifdef TIME_STARTUP_PROCESS
  printf("pre-process @ %ld\n", scheme_get_process_milliseconds());
#endif

  scheme_make_process();

#ifdef TIME_STARTUP_PROCESS
  printf("process @ %ld\n", scheme_get_process_milliseconds());
#endif

  make_init_env();

  env = scheme_make_empty_env();
  scheme_copy_from_original_env(env);

  scheme_add_embedded_builtins(env);

  scheme_init_format_procedure(env);
  scheme_init_rep(env);

  scheme_set_param(scheme_current_process->config, MZCONFIG_ENV, 
		   (Scheme_Object *)env); 

  scheme_init_error_escape_proc(scheme_current_process);

  scheme_starting_up = 0;

  scheme_init_getenv();

#ifdef TIME_STARTUP_PROCESS
  printf("done @ %ld\n#endif\n", scheme_get_process_milliseconds());
#endif

  return env;
}

#if USE_COMPILED_MACROS
Scheme_Object *scheme_eval_compiled_sized_string(const char *str, int len, Scheme_Env *env)
{
  Scheme_Object *port, *expr, *saved;
  Scheme_Process *p = scheme_current_process;
  Scheme_Config *config = p->config;

  port = scheme_make_sized_string_input_port(str, len);

  saved = scheme_get_param(config, MZCONFIG_ENV);
  scheme_set_param(config, MZCONFIG_ENV, (Scheme_Object *)env);
  expr = scheme_internal_read(port, NULL, 1, scheme_config
#ifdef MZ_REAL_THREADS
			      , p
#endif
			      );
  scheme_set_param(config, MZCONFIG_ENV, saved);

  return _scheme_eval_compiled(expr);
}
#endif

static void make_init_env(void)
{
  Scheme_Env *env;
#ifdef TIME_STARTUP_PROCESS
  long startt;
#endif

  env = make_env(NULL);

  scheme_set_param(scheme_current_process->config, MZCONFIG_ENV, 
		   (Scheme_Object *)env);

  REGISTER_SO(initial_env);
  initial_env = env;

  scheme_defining_primitives = 1;
  set_reference_ids = 1;
  builtin_ref_counter = 0;

#ifdef TIME_STARTUP_PROCESS
   printf("init @ %ld\n", scheme_get_process_milliseconds());
# define MZTIMEIT(n, f) (MARK_START_TIME(), f, DONE_TIME(n))
# define MARK_START_TIME() startt = scheme_get_process_milliseconds()
# define DONE_TIME(n) (printf(#n ": %ld\n", (long)(scheme_get_process_milliseconds() - startt)))
#else
# define MZTIMEIT(n, f) f
# define MARK_START_TIME() /**/
# define DONE_TIME(n) /**/
#endif

  /* The ordering of the first few init calls is important.
	  Add to the end of the list, not the beginning. */
  MZTIMEIT(symbol-table, scheme_init_symbol_table());
  MZTIMEIT(type, scheme_init_type(env));
  MZTIMEIT(symbol-type, scheme_init_symbol_type(env));
  MZTIMEIT(fun, scheme_init_fun(env));
  MZTIMEIT(symbol, scheme_init_symbol(env));
  MZTIMEIT(list, scheme_init_list(env));
  MZTIMEIT(number, scheme_init_number(env));
  MZTIMEIT(numarith, scheme_init_numarith(env));
  MZTIMEIT(numcomp, scheme_init_numcomp(env));
  MZTIMEIT(numstr, scheme_init_numstr(env));
  MZTIMEIT(stx, scheme_init_stx(env));
  MZTIMEIT(module, scheme_init_module(env));
  MZTIMEIT(port, scheme_init_port(env));
  MZTIMEIT(portfun, scheme_init_port_fun(env));
#ifndef NO_TCP_SUPPORT
  MZTIMEIT(network, scheme_init_network(env));
#endif
  MZTIMEIT(string, scheme_init_string(env));
  MZTIMEIT(vector, scheme_init_vector(env));
  MZTIMEIT(char, scheme_init_char(env));
  MZTIMEIT(bool, scheme_init_bool(env));
  MZTIMEIT(syntax, scheme_init_syntax(env));
  MZTIMEIT(eval, scheme_init_eval(env));
  MZTIMEIT(error, scheme_init_error(env));
  MZTIMEIT(promise, scheme_init_promise(env));
  MZTIMEIT(struct, scheme_init_struct(env));
#ifndef NO_SCHEME_EXNS
  MZTIMEIT(exn, scheme_init_exn(env));
#endif
  MZTIMEIT(process, scheme_init_process(env));
#ifndef NO_SCHEME_THREADS
  MZTIMEIT(sema, scheme_init_sema(env));
#endif
  MZTIMEIT(read, scheme_init_read(env));
  MZTIMEIT(print, scheme_init_print(env));
  MZTIMEIT(file, scheme_init_file(env));
  MZTIMEIT(dynamic-extension, scheme_init_dynamic_extension(env));
  MZTIMEIT(image, scheme_init_image(env));
#ifndef NO_REGEXP_UTILS
  MZTIMEIT(regexp, scheme_regexp_initialize(env));
#endif

  MARK_START_TIME();

  scheme_add_global_constant("make-global-value-list",
			     scheme_make_prim_w_arity(list_globals,
						      "make-global-value-list",
						      0, 0),
			     env);
  
  scheme_add_global_constant("defined?", 
			     scheme_make_prim_w_arity(defined,
						      "defined?",
						      1, 1),
			     env);
  scheme_add_global_constant("global-defined-value", 
			     scheme_make_prim_w_arity(global_defined_value,
						      "global-defined-value",
						      1, 2),
			     env);
  scheme_add_global_constant("expansion-time-value", 
			     scheme_make_prim_w_arity(local_exp_time_value,
						      "expansion-time-value",
						      1, 1),
			     env);
  scheme_add_global_constant("expansion-time-bound?", 
			     scheme_make_prim_w_arity(local_exp_time_bound_p,
						      "expansion-time-bound?",
						      1, 1),
			     env);  
  scheme_add_global_constant("expansion-time-name", 
			     scheme_make_prim_w_arity(local_exp_time_name,
						      "expansion-time-name",
						      0, 0),
			     env);

  scheme_add_global_constant("set!-expander", 
			     scheme_make_prim_w_arity(id_macro,
						      "set!-expander",
						      1, 1),
			     env);

  DONE_TIME(env);

  scheme_install_type_writer(scheme_variable_type, write_variable);
  scheme_install_type_reader(scheme_variable_type, read_variable);
  scheme_install_type_writer(scheme_local_type, write_local);
  scheme_install_type_reader(scheme_local_type, read_local);
  scheme_install_type_writer(scheme_local_unbox_type, write_local);
  scheme_install_type_reader(scheme_local_unbox_type, read_local_unbox);

  MARK_START_TIME();

  set_reference_ids = 0;

  scheme_finish_kernel(env);

#if USE_COMPILED_MACROS
  if (builtin_ref_counter != EXPECTED_PRIM_COUNT) {
    printf("Primitive count %d doesn't match expected count %d\n"
	   "Turn off USE_COMPILED_MACROS in src/schminc.h\n",
	   builtin_ref_counter, EXPECTED_PRIM_COUNT);
    exit(1);
  }
#endif
   
  scheme_defining_primitives = 0;
}

Scheme_Env *scheme_make_empty_env(void)
{
  return make_env(NULL);
}

static Scheme_Env *make_env(Scheme_Env *base)
{
  Scheme_Hash_Table *toplevel, *syntax, *modules;
  Scheme_Env *env;

  toplevel = scheme_hash_table(GLOBAL_TABLE_SIZE, SCHEME_hash_ptr, 1, 0);
  syntax = scheme_hash_table(GLOBAL_TABLE_SIZE, SCHEME_hash_ptr, 0, 0);
  if (base)
    modules = base->modules;
  else
    modules = scheme_hash_table(GLOBAL_TABLE_SIZE, SCHEME_hash_ptr, 0, 0);

  env = MALLOC_ONE_TAGGED(Scheme_Env);
  env->type = scheme_namespace_type;

  env->modname = scheme_false;
  env->imports = scheme_null;

  env->toplevel = toplevel;
  env->syntax = syntax;
  env->modules = modules;

  env->exports = NULL;
  env->num_exports = 0;

  {
    Scheme_Comp_Env *me;
    me = (Scheme_Comp_Env *)MALLOC_ONE_RT(Scheme_Full_Comp_Env);
    env->init = me;
  }
#ifdef MZTAG_REQUIRED
  env->init->type = scheme_rt_comp_env;
#endif
  env->init->num_bindings = 0;
  env->init->next = NULL;
  env->init->genv = env;
  env->init->eenv = env;
  init_compile_data(env->init);

  return env;
}

Scheme_Env *
scheme_new_module_env(Scheme_Env *env, Scheme_Object *modname)
{
  Scheme_Env *menv;

  menv = make_env(env);
  menv->modname = modname;
  menv->init->flags |= SCHEME_MODULE_FRAME;

  return menv;
}

void
scheme_do_add_global_symbol(Scheme_Env *env, Scheme_Object *sym, 
			    Scheme_Object *obj, 
			    int valvar, int constant)
{
  Scheme_Hash_Table *toplevel;

  if (valvar)
    toplevel = env->toplevel;
  else
    toplevel = env->syntax;

  scheme_add_to_table(toplevel, (char *)sym, obj, constant);
}

void
scheme_add_global(const char *name, Scheme_Object *obj, Scheme_Env *env)
{
  scheme_do_add_global_symbol(env, scheme_intern_symbol(name), obj, 1, 0);
}

void
scheme_add_global_symbol(Scheme_Object *sym, Scheme_Object *obj, Scheme_Env *env)
{
  scheme_do_add_global_symbol(env, sym, obj, 1, 0);
}

void
scheme_add_global_constant(const char *name, Scheme_Object *obj, 
			   Scheme_Env *env)
{
  scheme_do_add_global_symbol(env, scheme_intern_symbol(name), obj, 1, 0);
}

void
scheme_add_global_constant_symbol(Scheme_Object *name, Scheme_Object *obj, 
				  Scheme_Env *env)
{
  scheme_do_add_global_symbol(env, name, obj, 1, 1);
}

void
scheme_add_global_keyword(const char *name, Scheme_Object *obj, 
			  Scheme_Env *env)
{
  scheme_do_add_global_symbol(env, scheme_intern_symbol(name), obj, 0, 0);
}

void
scheme_remove_global_symbol(Scheme_Object *sym, Scheme_Env *env)
{
  Scheme_Bucket *b;
  Scheme_Hash_Table *toplevel;

  toplevel = env->toplevel;

  b = scheme_bucket_from_table(toplevel, (char *)sym);

  if (!b)
    return;

  b->val = NULL;
}

void
scheme_remove_global(const char *name, Scheme_Env *env)
{
  Scheme_Object *sym;

  sym = scheme_intern_symbol(name);

  scheme_remove_global_symbol(sym, env);
}

void scheme_copy_from_original_env(Scheme_Env *env)
{
  Scheme_Hash_Table *ht, *ht2;
  Scheme_Bucket **bs;
  Scheme_Object *call_ec;
  int i, j;
  
  for (j = 0; j < 2; j++) {
    if (!j) {
      ht = initial_env->toplevel;
      ht2 = env->toplevel;
    } else {
      ht = initial_env->syntax;
      ht2 = env->syntax;
    }

    bs = ht->buckets;

    if (!j && scheme_escape_continuations_only)
      call_ec = scheme_lookup_global(scheme_intern_symbol("call/ec"), initial_env);
    else
      call_ec = NULL;
  
    for (i = ht->size; i--; ) {
      Scheme_Bucket *b = bs[i];
      if (b && b->val) {
	Scheme_Object *name = (Scheme_Object *)b->key;
	Scheme_Object *val = (Scheme_Object *)b->val;
	int cst = (((Scheme_Bucket_With_Const_Flag *)b)->flags) & GLOB_IS_CONST;
	int refid = (((Scheme_Bucket_With_Const_Flag *)b)->flags) & GLOB_HAS_REF_ID;
      
	if (call_ec) {
	  char *s = SCHEME_SYM_VAL(name);
	  /* WARNING: s is GC-misaligned... */
	  
	  if (s[0] == '#')
	    s += 2;
	  
	  if ((s[0] == 'c') && (!strcmp(s, "call/cc") || !strcmp(s, "call-with-current-continuation")))
	    val = call_ec;
	}
	
	if (refid)
	  ht2->has_constants = 2;

	scheme_add_to_table(ht2, (char *)name, val, 0);
	
	if (refid)
	  ht2->has_constants = 1;
	
	if (cst || refid) {
	  Scheme_Bucket *b2;
	  
	  b2 = scheme_bucket_from_table(ht2, (char *)name);
	  if (cst)
	    ((Scheme_Bucket_With_Const_Flag *)b2)->flags |= GLOB_IS_CONST;
	  if (refid) {
	    ((Scheme_Bucket_With_Const_Flag *)b2)->flags |= GLOB_HAS_REF_ID;
	    ((Scheme_Bucket_With_Ref_Id *)b2)->id = ((Scheme_Bucket_With_Ref_Id *)b)->id;
	  }
	}
      }
    }
  }
}

Scheme_Object **scheme_make_builtin_references_table(void)
{
  Scheme_Hash_Table *ht;
  Scheme_Object **t;
  Scheme_Bucket **bs;
  long i;

  t = MALLOC_N(Scheme_Object *, (builtin_ref_counter + 1));
#ifdef MEMORY_COUNTING_ON
  scheme_misc_count += sizeof(Scheme_Object *) * (builtin_ref_counter + 1);
#endif

  ht = initial_env->toplevel;

  bs = ht->buckets;

  for (i = ht->size; i--; ) {
    Scheme_Bucket *b = bs[i];
    if (b && (((Scheme_Bucket_With_Const_Flag *)b)->flags & GLOB_HAS_REF_ID))
      t[((Scheme_Bucket_With_Ref_Id *)b)->id] = (Scheme_Object *)b->val;
  }

  return t;
}

Scheme_Hash_Table *scheme_map_constants_to_globals(void)
{
  Scheme_Hash_Table *ht, *result;
  Scheme_Bucket **bs;
  long i;

  ht = initial_env->toplevel;
  bs = ht->buckets;

  result = scheme_hash_table(10, SCHEME_hash_ptr, 0, 0);

  for (i = ht->size; i--; ) {
    Scheme_Bucket *b = bs[i];
    if (b && (((Scheme_Bucket_With_Const_Flag *)b)->flags & GLOB_IS_CONST)
	&& (((Scheme_Bucket_With_Const_Flag *)b)->flags & GLOB_IS_KEYWORD))
      scheme_add_to_table(result, (const char *)b->val, b, 0);
  }

  return result;
}

void scheme_check_identifier(const char *formname, Scheme_Object *id, 
			     const char *where, Scheme_Comp_Env *env,
			     Scheme_Object *form)
{
  if (!where)
    where = "";

  if (!SCHEME_STX_SYMBOLP(id))
    scheme_wrong_syntax(formname, form ? id : NULL, 
			form ? form : id, 
			"not an identifier%s", where);
}

static void init_compile_data(Scheme_Comp_Env *env)
{
  Compile_Data *data;
  int i, c, *use;

  c = env->num_bindings;
  use = MALLOC_N_ATOMIC(int, c);

  data = COMPILE_DATA(env);

  data->stat_dists = NULL;
  data->sd_depths = NULL;
  data->use = use;
  for (i = 0; i < c; i++) {
    use[i] = 0;
  }

  data->constants = NULL;
}

Scheme_Comp_Env *scheme_new_compilation_frame(int num_bindings, int flags,
					 Scheme_Comp_Env *base)
{
  Scheme_Comp_Env *frame;
  int count;
  
  count = num_bindings;

  frame = (Scheme_Comp_Env *)MALLOC_ONE_RT(Scheme_Full_Comp_Env);
#ifdef MZTAG_REQUIRED
  frame->type = scheme_rt_comp_env;
#endif

  {
    Scheme_Object **vals;
    vals = MALLOC_N(Scheme_Object *, count);
    frame->values = vals;
  }

  frame->num_bindings = num_bindings;
  frame->flags = flags | (base->flags & SCHEME_PRIM_GLOBALS_ONLY);
  frame->next = base;
  frame->genv = base->genv;
  frame->eenv = base->eenv;

  init_compile_data(frame);

  return frame;
}

int scheme_used_app_only(Scheme_Comp_Env *env, int which)
{
  Compile_Data *data = COMPILE_DATA(env);

  if (data->use[which] & ARBITRARY_USE)
    return 0;
  else
    return 1;
}

int scheme_used_ever(Scheme_Comp_Env *env, int which)
{
  Compile_Data *data = COMPILE_DATA(env);

  return !!data->use[which];
}

int scheme_is_env_variable_boxed(Scheme_Comp_Env *env, int which)
{
  Compile_Data *data = COMPILE_DATA(env);

  return !!(data->use[which] & WAS_SET_BANGED);
}

void scheme_unsettable_variable(Scheme_Comp_Env *env, int which)
{
  Compile_Data *data = COMPILE_DATA(env);

  data->use[which] |= NOT_SETTABLE;
}

void
scheme_add_compilation_binding(int index, Scheme_Object *val, Scheme_Comp_Env *frame)
{
  if ((index >= frame->num_bindings) || (index < 0))
    scheme_signal_error("internal error: scheme_add_binding: "
			"index out of range: %d", index);
  
  frame->values[index] = val;
}

void scheme_add_local_syntax(Scheme_Object *name, 
			     Scheme_Comp_Env *env)
{
  Constant_Binding *b;
  
  b = MALLOC_ONE_RT(Constant_Binding);
#ifdef MZTAG_REQUIRED
  b->type = scheme_rt_constant_binding;
#endif

  b->next = COMPILE_DATA(env)->constants;
  b->name = name;
  b->val = NULL;
  b->before = env->num_bindings;

  COMPILE_DATA(env)->constants = b;
}

void scheme_set_local_syntax(Scheme_Object *name, Scheme_Object *val,
			    Scheme_Comp_Env *env)
{
  Constant_Binding *b;
  
  b = COMPILE_DATA(env)->constants;
  while (b) {
    if (scheme_stx_bound_eq(b->name, name)) {
      b->val = val;
      return;
    }
    b = b->next;
  }

  scheme_signal_error("internal error: scheme_set_local_syntax: not found");
}

Scheme_Comp_Env *
scheme_add_compilation_frame(Scheme_Object *vals, Scheme_Comp_Env *env, int flags)
{
  Scheme_Comp_Env *frame;
  int len, i, count;
  
  len = scheme_stx_list_length(vals);
  count = len;

  frame = scheme_new_compilation_frame(count, flags, env);

  for (i = 0; i < len ; i++) {
    if (SCHEME_STX_SYMBOLP(vals))
      frame->values[i] = vals;
    else {
      frame->values[i] = SCHEME_STX_CAR(vals);
      vals = SCHEME_STX_CDR(vals);
    }
  }
  
  init_compile_data(frame);

  return frame;
}

Scheme_Comp_Env *scheme_no_defines(Scheme_Comp_Env *env)
{
  if (scheme_is_toplevel(env))
    return scheme_new_compilation_frame(0, 0, env);
  else
    return env;
}

int scheme_is_toplevel(Scheme_Comp_Env *env)
{
  return !env->next || (env->flags & SCHEME_TOPLEVEL_FRAME);
}

int scheme_is_module_env(Scheme_Comp_Env *env)
{
  return !!(env->flags & SCHEME_MODULE_FRAME);
}

Scheme_Comp_Env *scheme_extend_as_toplevel(Scheme_Comp_Env *env)
{
  if (scheme_is_toplevel(env))
    return env;
  else
    return scheme_new_compilation_frame(0, SCHEME_TOPLEVEL_FRAME, env);
}


static int env_uid_counter;

static Scheme_Object *env_frame_uid(Scheme_Comp_Env *env)
{
  if (env->flags & SCHEME_CAPTURE_WITHOUT_RENAME)
    return NULL;

  if (!env->uid) {
    char name[20];
    env_uid_counter++;
    sprintf(name, "env%d", env_uid_counter);
    env->uid = scheme_make_symbol(name); /* uninterned! */
  }
  return env->uid;
}

Scheme_Object *scheme_add_env_renames(Scheme_Object *stx, Scheme_Comp_Env *env, 
				      Scheme_Comp_Env *upto)
{
  if (!SCHEME_STXP(stx)) {
    scheme_signal_error("internal error: not syntax");
    return NULL;
  }

  while (env != upto) {
    Scheme_Object *uid;
    Constant_Binding *c = COMPILE_DATA(env)->constants;
    int i;

    uid = env_frame_uid(env);

    while (c) {
      if (!c->rename) {
	Scheme_Object *rnm;
	rnm = scheme_make_rename(c->name, uid);
	c->rename = rnm;
      }

      stx = scheme_add_rename(stx, c->rename);
      c = c->next;
    }
    
    if (!env->renames) {
      Scheme_Object **rnms;
      rnms = MALLOC_N(Scheme_Object *, env->num_bindings);
      env->renames = rnms;
    }

    for (i = env->num_bindings; i--; ) {
      if (env->values[i]) {
	if (!env->renames[i]) {
	  Scheme_Object *rnm;
	  rnm = scheme_make_rename(env->values[i], uid);
	  env->renames[i] = rnm;
	}

	stx = scheme_add_rename(stx, env->renames[i]);
      }
    }

    env = env->next;
  }

  return stx;
}

static Scheme_Object *alloc_local(short type, int pos)
{
  Scheme_Object *v;

  v = (Scheme_Object *)scheme_malloc_atomic_tagged(sizeof(Scheme_Local));
  v->type = type;
  SCHEME_LOCAL_POS(v) = pos;

  return (Scheme_Object *)v;
}

Scheme_Object *scheme_make_local(Scheme_Type type, int pos)
{
  int k;

  k = type - scheme_local_type;

  if (pos < MAX_CONST_LOCAL_POS)
    return scheme_local[pos][k];

  return alloc_local(type, pos);
}

static Scheme_Local *get_frame_loc(Scheme_Comp_Env *frame,
				   int i, int j, int p, int flags)
{
  COMPILE_DATA(frame)->use[i] |= (((flags & (SCHEME_APP_POS | SCHEME_SETTING))
				   ? CONSTRAINED_USE
				   : ARBITRARY_USE)
				  | ((flags & (SCHEME_SETTING | SCHEME_LINKING_REF))
				     ? WAS_SET_BANGED
				     : 0));
  
  if (!COMPILE_DATA(frame)->stat_dists) {
    int k, *ia;
    char **ca;
    ca = MALLOC_N(char*, frame->num_bindings);
    COMPILE_DATA(frame)->stat_dists = ca;
    ia = MALLOC_N_ATOMIC(int, frame->num_bindings);
    COMPILE_DATA(frame)->sd_depths = ia;
    for (k = frame->num_bindings; k--; ) {
      COMPILE_DATA(frame)->sd_depths[k] = 0;
    }
  }
  
  if (COMPILE_DATA(frame)->sd_depths[i] <= j) {
    char *naya, *a;
    int k;
    
    naya = MALLOC_N_ATOMIC(char, (j + 1));
    for (k = j + 1; k--; ) {
      naya[k] = 0;
    }
    a = COMPILE_DATA(frame)->stat_dists[i];
    for (k = COMPILE_DATA(frame)->sd_depths[i]; k--; ) {
      naya[k] = a[k];
    }
    
    COMPILE_DATA(frame)->stat_dists[i] = naya;
    COMPILE_DATA(frame)->sd_depths[i] = j + 1;
  }

  COMPILE_DATA(frame)->stat_dists[i][j] = 1;

  return (Scheme_Local *)scheme_make_local(scheme_local_type, p + i);
}

Scheme_Object *
scheme_static_distance(Scheme_Object *symbol, Scheme_Comp_Env *env, int flags)
{
  Scheme_Comp_Env *frame;
  int j = 0, p = 0;
  Scheme_Bucket *b;
  Scheme_Object *val, *modname;
  Scheme_Env *genv;
  
  frame = env;
  for (frame = env; frame->next != NULL; frame = frame->next) {
    int i;
    Scheme_Object *uid;
    Constant_Binding *c = COMPILE_DATA(frame)->constants;
    
    if (frame->flags & SCHEME_LAMBDA_FRAME)
      j++;

    uid = env_frame_uid(frame);

    for (i = frame->num_bindings; i--; ) {
      while (c && (c->before > i)) {
	int issame;
	if (env->flags & SCHEME_CAPTURE_WITHOUT_RENAME)
	  issame = scheme_stx_module_eq(symbol, c->name);
	else
	  issame = scheme_stx_env_bound_eq(symbol, c->name, uid);
	
	if (issame) {
	  val = c->val;
	  goto found_const;
	}
	c = c->next;
      }

      if (frame->values[i] && scheme_stx_env_bound_eq(symbol, frame->values[i], uid)) {
	if ((flags & SCHEME_SETTING) && (COMPILE_DATA(frame)->use[i] & NOT_SETTABLE))
	  scheme_wrong_syntax("set!", NULL, symbol,
			      "imported/inherited variable cannot be mutated");

	if (flags & SCHEME_DONT_MARK_USE)
	  return scheme_make_local(scheme_local_type, 0);
	else
	  return (Scheme_Object *)get_frame_loc(frame, i, j, p, flags);
      }
    }

    while (c) {
      int issame;
      if (env->flags & SCHEME_CAPTURE_WITHOUT_RENAME)
	issame = scheme_stx_module_eq(symbol, c->name);
      else
	issame = scheme_stx_env_bound_eq(symbol, c->name, uid);

      if (issame) {
	val = c->val;
	goto found_const;
      }
      c = c->next;
    }

    p += frame->num_bindings;
  }
  
  /* Used out of context? */
  if (scheme_stx_has_binder(symbol)) {
    if (!(flags & SCHEME_OUT_OF_CONTEXT_OK))
      scheme_wrong_syntax("compile", NULL, symbol,
			  "identifier used out of context");
    return NULL;
  }

  modname = scheme_stx_module_name(&symbol);
  if (modname)
    genv = scheme_module_load(modname, env->genv);
  else
    genv = env->genv;

  if (!(flags & SCHEME_GLOB_ALWAYS_REFERENCE)) {
    /* Try syntax table: */
    val = scheme_lookup_in_table(genv->syntax, (char *)SCHEME_STX_SYM(symbol));
    if (val)
      return val;
  }

  if ((flags & SCHEME_NULL_FOR_UNBOUND) && !modname)
    return NULL;

  b = scheme_bucket_from_table(genv->toplevel, (char *)SCHEME_STX_SYM(symbol));
  if ((flags & SCHEME_ELIM_CONST) && b && b->val 
      && (((Scheme_Bucket_With_Const_Flag *)b)->flags & GLOB_IS_CONST)
      && !(flags & SCHEME_GLOB_ALWAYS_REFERENCE))
    return (Scheme_Object *)b->val;

  return (Scheme_Object *)b;

 found_const:
  if (!val) {
    scheme_wrong_syntax("identifier", NULL, symbol,
			"variable used out of context");
    return NULL;
  }
  if (!(flags & SCHEME_ENV_CONSTANTS_OK)) {
    if (SAME_TYPE(SCHEME_TYPE(val), scheme_macro_type)
	|| SAME_TYPE(SCHEME_TYPE(val), scheme_syntax_type))
      return val;
    else
      scheme_wrong_syntax("set!", NULL, symbol,
			  "local syntax identifier cannot be mutated");
    return NULL;
  }
  return val;
}

void scheme_env_make_closure_map(Scheme_Comp_Env *env, short *_size, short **_map)
{
  Compile_Data *data;
  Scheme_Comp_Env *frame;
  int i, j, pos = 0, lpos = 0;
  short *map, size;

  /* Count vars used by this closure (skip args): */
  j = 1;
  for (frame = env->next; frame; frame = frame->next) {
    data = COMPILE_DATA(frame);

    if (frame->flags & SCHEME_LAMBDA_FRAME)
      j++;

    if (data->stat_dists) {
      for (i = 0; i < frame->num_bindings; i++) {
	if (data->sd_depths[i] > j) {
	  if (data->stat_dists[i][j]) {
	    pos++;
	    if (frame->flags & SCHEME_ANCHORED_FRAME)
	      pos++;
	  }
	}
      }
    }
  }

  data = NULL; /* Clear unaligned pointer */

  size = pos;
  *_size = size;
  map = MALLOC_N_ATOMIC(short, size);
  *_map = map;

  /* Build map, unmarking locals and marking deeper in parent prame */
  j = 1; pos = 0;
  for (frame = env->next; frame; frame = frame->next) {
    data = COMPILE_DATA(frame);

    if (frame->flags & SCHEME_LAMBDA_FRAME)
      j++;

    if (data->stat_dists) {
      for (i = 0; i < frame->num_bindings; i++) {
	if (data->sd_depths[i] > j) {
	  if (data->stat_dists[i][j]) {
	    map[pos++] = lpos;
	    if (frame->flags & SCHEME_ANCHORED_FRAME)
	      map[pos++] = -(lpos + 1);
	    data->stat_dists[i][j] = 0; /* This closure's done with these vars... */
	    data->stat_dists[i][j - 1] = 1; /* ... but insure previous keeps */
	  }
	}
	lpos++;
      }
    } else
      lpos += frame->num_bindings;
  }
}

int *scheme_env_get_flags(Scheme_Comp_Env *frame, int start, int count)
{
  int *v, i;
  
  v = MALLOC_N_ATOMIC(int, count);
  memcpy(v, COMPILE_DATA(frame)->use + start, sizeof(int) * count);

  for (i = count; i--; ) {
    int old;
    old = v[i];
    v[i] = 0;
    if (old & (ARBITRARY_USE | CONSTRAINED_USE))
      v[i] |= SCHEME_WAS_USED;
    if (old & WAS_SET_BANGED)
      v[i] |= SCHEME_WAS_SET_BANGED;
  }

  return v;
}

void scheme_begin_dup_symbol_check(DupCheckRecord *r)
{
  r->count = 0;
}

void scheme_dup_symbol_check(DupCheckRecord *r, const char *where,
			     Scheme_Object *symbol, char *what, 
			     Scheme_Object *form)
{
  int i;
  Scheme_Object *l, *p;
  char *key;

  if (r->count <= 5) {
    for (i = 0; i < r->count; i++)
      if (scheme_stx_bound_eq(symbol, r->syms[i]))
	scheme_wrong_syntax(where, symbol, form,
			    "duplicate %s name", what);

    if (r->count < 5) {
      r->syms[r->count++] = symbol;
      return;
    } else {
      r->ht = scheme_hash_table(7, SCHEME_hash_ptr, 0, 0);
      for (i = 0; i < r->count; i++) {
	key = (char *)SCHEME_STX_VAL(r->syms[i]);
	l = (Scheme_Object *)scheme_lookup_in_table(r->ht, key);
	if (!l)
	  l = scheme_null;
	l = scheme_make_pair(r->syms[i], l);
	scheme_add_to_table(r->ht, key, (void *)l, 0);
      }
    }
  }

  key = (char *)SCHEME_STX_VAL(symbol);
  l = (Scheme_Object *)scheme_lookup_in_table(r->ht, key);
  if (!l)
    l = scheme_null;

  for (p = l; SCHEME_PAIRP(p); p = SCHEME_CDR(p)) {
    if (scheme_stx_bound_eq(symbol, SCHEME_CAR(p)))
      scheme_wrong_syntax(where, symbol, form,
			  "duplicate %s name", what);
  }

  l = scheme_make_pair(symbol, l);
  scheme_add_to_table(r->ht, key, (void *)l, 0);
}

Link_Info *scheme_link_info_create()
{
  Link_Info *naya;

  naya = MALLOC_ONE_RT(Link_Info);
#ifdef MZTAG_REQUIRED
  naya->type = scheme_rt_link_info;
#endif
  naya->count = 0;
  naya->next = NULL;

  return naya;
}

Link_Info *scheme_link_info_extend(Link_Info *info, int size, int oldsize, int mapc)
     /* size = number of appended items in run-time frame */
     /* oldisze = number of appended items in original compile-time frame */
     /* mapc = mappings that will be installed */
{
  Link_Info *naya;

  naya = MALLOC_ONE_RT(Link_Info);
#ifdef MZTAG_REQUIRED
  naya->type = scheme_rt_link_info;
#endif
  naya->next = info;
  naya->size = size;
  naya->oldsize = oldsize;
  naya->count = mapc;
  naya->pos = 0;
  naya->anchor_offset = 0;
  if (mapc) {
    int i, *ia;
    short *sa;

    sa = MALLOC_N_ATOMIC(short, mapc);
    naya->old_pos = sa;
    sa = MALLOC_N_ATOMIC(short, mapc);
    naya->new_pos = sa;
    ia = MALLOC_N_ATOMIC(int, mapc);
    naya->flags = ia;

    /* necessary? added when changed allocation to atomic */
    for (i = mapc; i--; ) {
      naya->old_pos[i] = 0;
      naya->new_pos[i] = 0;
      naya->flags[i] = 0;
    }
  }

  return naya;
}

void scheme_link_info_add_mapping(Link_Info *info, int oldp, int newp, int flags)
{
  if (info->pos == info->count) {
    scheme_signal_error("internal error: add_mapping: "
			"too many: %d", info->pos);
  }

  info->old_pos[info->pos] = oldp;
  info->new_pos[info->pos] = newp;
  info->flags[info->pos] = flags;
  
  info->pos++;
}

void scheme_link_info_set_anchor_offset(Link_Info *info, int offset)
{
  info->anchor_offset = offset;
}

static int link_info_lookup(Link_Info *info, int pos, int *flags)
{
  int i, offset = 0, orig = pos;
  int get_anchor;

  if (pos < 0) {
    get_anchor = pos;
    pos = -(pos + 1);
  } else
    get_anchor = 0;

  while (info) {
    for (i = info->pos; i--; ) {
      int oldp = info->old_pos[i];
      if (pos == oldp) {
	/* not yet mapped anchor */
	if (flags)
	  *flags = info->flags[i];
	return info->new_pos[i] + offset + (get_anchor ? info->anchor_offset : 0);
      }
      if (get_anchor && (get_anchor == oldp)) {
	/* re-mapped anchor */
	if (flags)
	  *flags = info->flags[i];
	return info->new_pos[i] + offset;
      }
    }

    pos -= info->oldsize;
    offset += info->size;
    info = info->next;
  }

  scheme_signal_error("internal error: scheme_link_info_lookup: "
		      "variable %d not found", orig);

  return 0;
}

int scheme_link_info_flags(Link_Info *info, int pos)
{
  int flags;

  link_info_lookup(info, pos, &flags);

  return flags;
}

int scheme_link_info_lookup(Link_Info *info, int pos, int *flags)
{
  return link_info_lookup(info, pos, flags);
}

int scheme_link_info_lookup_anchor(Link_Info *info, int pos)
{
  return link_info_lookup(info, -(pos + 1), NULL);
}


Scheme_Object *
scheme_lookup_global (Scheme_Object *symbol, Scheme_Env *env)
{
  return (Scheme_Object *)scheme_lookup_in_table(env->toplevel, (char *)symbol);
}

Scheme_Bucket *
scheme_global_bucket(Scheme_Object *symbol, Scheme_Env *env)
{
  return scheme_bucket_from_table(env->toplevel, (char *)symbol);
}

Scheme_Bucket *
scheme_global_keyword_bucket(Scheme_Object *symbol, Scheme_Env *env)
{
  return scheme_bucket_from_table(env->syntax, (char *)symbol);
}

Scheme_Object *scheme_make_envunbox(Scheme_Object *value)
{
  Scheme_Object *obj;

#ifdef MZ_PRECISE_GC
  obj = (Scheme_Object *)scheme_malloc_envunbox(sizeof(Scheme_Small_Object));
  obj->type = scheme_envunbox_type;
#else
  obj = (Scheme_Object *)scheme_malloc_envunbox(sizeof(Scheme_Object*));
#endif
  SCHEME_ENVBOX_VAL(obj) = value;

  return obj;
}

static Scheme_Object *list_globals(int argc, Scheme_Object *argv[])
{
  Scheme_Object *l = scheme_null;
  int i;
  Scheme_Hash_Table *toplevel;
  Scheme_Bucket *b;

  toplevel = scheme_get_env(scheme_config)->toplevel;

  for (i = 0; i < toplevel->size; i++) {
    b = toplevel->buckets[i];
    if (b && b->val)
      l = scheme_make_pair(scheme_make_pair((Scheme_Object *)b->key, 
					    (Scheme_Object *)b->val),
			   l);
  }

  return l;
}

static Scheme_Object *defined(int argc, Scheme_Object *argv[])
{
  if (!SCHEME_SYMBOLP(argv[0]))
    scheme_wrong_type("defined?", "symbol", 0, argc, argv);

  return scheme_lookup_global(argv[0], scheme_get_env(scheme_config))
    ? scheme_true : scheme_false;
}

static Scheme_Object *
global_defined_value(int argc, Scheme_Object *argv[])
{
  Scheme_Object *v;
  Scheme_Env *env;

  if (!SCHEME_SYMBOLP(argv[0]))
    scheme_wrong_type("global-defined-value", "symbol", 0, argc, argv);

  env = scheme_get_env(scheme_config);

  if (argc > 1) {
    Scheme_Bucket *bucket;

    bucket = scheme_global_bucket(argv[0], env);

    scheme_set_global_bucket("global-defined-value", bucket, argv[1], 1);

    return scheme_void;
  } else {
    v = scheme_lookup_global(argv[0], env);
    
    if (!v)
      scheme_raise_exn(MZEXN_VARIABLE, argv[0],
		       "global-defined-value: %S is not defined",
		       argv[0]);

    return v;
  }
}

static Scheme_Object *
local_exp_time_value(int argc, Scheme_Object *argv[])
{
  Scheme_Object *v, *sym;
  Scheme_Comp_Env *env;

  env = scheme_current_process->current_local_env;
  if (!env)
    scheme_raise_exn(MZEXN_MISC, 
		     "expansion-time-value: not currently expansion time");

  sym = argv[0];

  if (!SCHEME_SYMBOLP(sym)
      && !SCHEME_STX_SYMBOLP(sym))
    scheme_wrong_type("expansion-time-value", "syntax identifier", 0, argc, argv);

  if (SCHEME_STXP(sym))
    sym = scheme_add_remove_mark(sym, scheme_current_process->current_local_mark);

  v = scheme_static_distance(sym, env,
			     (SCHEME_NULL_FOR_UNBOUND
			      + SCHEME_APP_POS + SCHEME_ENV_CONSTANTS_OK
			      + SCHEME_OUT_OF_CONTEXT_OK + SCHEME_ELIM_CONST));

  /* Deref globals */
  if (v && SAME_TYPE(SCHEME_TYPE(v), scheme_variable_type))
    v = (Scheme_Object *)(SCHEME_VAR_BUCKET(v))->val;

  if (!v || (NOT_SAME_TYPE(SCHEME_TYPE(v), scheme_macro_type)
	     && NOT_SAME_TYPE(SCHEME_TYPE(v), scheme_syntax_type)))
    scheme_raise_exn(MZEXN_MISC,
		     "expansion-time-value: %S is not defined "
		     "as syntax",
		     argv[0]);
  
  return SCHEME_PTR_VAL(v);
}

static Scheme_Object *
local_exp_time_bound_p(int argc, Scheme_Object *argv[])
{
  Scheme_Object *v, *sym;
  Scheme_Comp_Env *env;

  env = scheme_current_process->current_local_env;
  if (!env)
    scheme_raise_exn(MZEXN_MISC,
		     "local-expansion-time-bound?: not currently expansion time");

  sym = argv[0];

  if (!SCHEME_SYMBOLP(sym)
      && !SCHEME_STX_SYMBOLP(sym))
    scheme_wrong_type("expansion-time-bound?", "syntax identifier", 0, argc, argv);

  if (SCHEME_STXP(sym))
    sym = scheme_add_remove_mark(sym, scheme_current_process->current_local_mark);

  v = scheme_static_distance(sym, env,
			     (SCHEME_NULL_FOR_UNBOUND
			      + SCHEME_APP_POS + SCHEME_ENV_CONSTANTS_OK
			      + SCHEME_OUT_OF_CONTEXT_OK + SCHEME_ELIM_CONST));

  /* Deref globals */
  if (v && SAME_TYPE(SCHEME_TYPE(v), scheme_variable_type))
    v = (Scheme_Object *)(SCHEME_VAR_BUCKET(v))->val;

  if (!v || (NOT_SAME_TYPE(SCHEME_TYPE(v), scheme_macro_type)
	     && NOT_SAME_TYPE(SCHEME_TYPE(v), scheme_syntax_type)))
    return scheme_false;
  else
    return scheme_true;
}

static Scheme_Object *
local_exp_time_name(int argc, Scheme_Object *argv[])
{
  Scheme_Object *sym;

  sym = scheme_current_process->current_local_name;
  if (!sym)
    scheme_raise_exn(MZEXN_MISC, 
		     "expansion-time-name: not currently expansion time");

  return sym;
}

static Scheme_Object *
id_macro(int argc, Scheme_Object *argv[])
{
  Scheme_Object *v;

  scheme_check_proc_arity("set!-expander", 1, 0, argc, argv);

  v = scheme_alloc_small_object();
  v->type = scheme_id_macro_type;
  SCHEME_PTR_VAL(v) = argv[0];

  return v;
}


/*********************************************************************/

static Scheme_Object *write_variable(Scheme_Object *obj)
{
  Scheme_Object *sym;

  sym = (Scheme_Object *)(SCHEME_VAR_BUCKET(obj))->key;

  return sym;
}

static Scheme_Object *read_variable(Scheme_Object *obj)
{
#define SCHEME_GLOBAL_REFERENCE (Scheme_Object *)scheme_global_bucket

  return SCHEME_GLOBAL_REFERENCE(obj, scheme_get_env(scheme_config));
}

static Scheme_Object *write_local(Scheme_Object *obj)
{
  return scheme_make_integer(SCHEME_LOCAL_POS(obj));
}

static Scheme_Object *read_local(Scheme_Object *obj)
{
  return scheme_make_local(scheme_local_type,
			   SCHEME_INT_VAL(obj));
}

static Scheme_Object *read_local_unbox(Scheme_Object *obj)
{
  return scheme_make_local(scheme_local_unbox_type,
			   SCHEME_INT_VAL(obj));
}

/**********************************************************************/

#ifdef MZ_PRECISE_GC

START_XFORM_SKIP;

#define MARKS_FOR_ENV_C
#include "mzmark.c"

static void register_traversers(void)
{
  GC_REG_TRAV(scheme_rt_comp_env, mark_comp_env);
  GC_REG_TRAV(scheme_rt_constant_binding, mark_const_binding);
  GC_REG_TRAV(scheme_rt_link_info, mark_link_info);
}

END_XFORM_SKIP;

#endif
