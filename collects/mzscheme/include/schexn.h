/* This file was generated by makeexn */
#ifndef _MZEXN_DEFINES
#define _MZEXN_DEFINES

enum {
  MZEXN,
  MZEXN_USER,
  MZEXN_VARIABLE,
  MZEXN_VARIABLE_KEYWORD,
  MZEXN_APPLICATION,
  MZEXN_APPLICATION_ARITY,
  MZEXN_APPLICATION_TYPE,
  MZEXN_APPLICATION_MISMATCH,
  MZEXN_APPLICATION_DIVIDE_BY_ZERO,
  MZEXN_APPLICATION_CONTINUATION,
  MZEXN_ELSE,
  MZEXN_STRUCT,
  MZEXN_OBJECT,
  MZEXN_UNIT,
  MZEXN_SYNTAX,
  MZEXN_READ,
  MZEXN_READ_EOF,
  MZEXN_I_O,
  MZEXN_I_O_PORT,
  MZEXN_I_O_PORT_READ,
  MZEXN_I_O_PORT_WRITE,
  MZEXN_I_O_PORT_CLOSED,
  MZEXN_I_O_PORT_USER,
  MZEXN_I_O_FILESYSTEM,
  MZEXN_I_O_TCP,
  MZEXN_MISC,
  MZEXN_MISC_UNSUPPORTED,
  MZEXN_MISC_USER_BREAK,
  MZEXN_MISC_OUT_OF_MEMORY,
  MZEXN_OTHER
};

#endif

#ifdef _MZEXN_TABLE

#define MZEXN_MAXARGS 4

#ifdef GLOBAL_EXN_ARRAY
static exn_rec exn_table[] = {
  { 2, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 4, NULL, NULL, 0 },
  { 4, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 3, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 },
  { 2, NULL, NULL, 0 }
};
#else
static exn_rec *exn_table;
#endif

#endif

#ifdef _MZEXN_PRESETUP

#ifndef GLOBAL_EXN_ARRAY
  exn_table = (exn_rec *)scheme_malloc(sizeof(exn_rec) * MZEXN_OTHER);
  exn_table[MZEXN].args = 2;
  exn_table[MZEXN_USER].args = 2;
  exn_table[MZEXN_VARIABLE].args = 3;
  exn_table[MZEXN_VARIABLE_KEYWORD].args = 3;
  exn_table[MZEXN_APPLICATION].args = 3;
  exn_table[MZEXN_APPLICATION_ARITY].args = 4;
  exn_table[MZEXN_APPLICATION_TYPE].args = 4;
  exn_table[MZEXN_APPLICATION_MISMATCH].args = 3;
  exn_table[MZEXN_APPLICATION_DIVIDE_BY_ZERO].args = 3;
  exn_table[MZEXN_APPLICATION_CONTINUATION].args = 3;
  exn_table[MZEXN_ELSE].args = 2;
  exn_table[MZEXN_STRUCT].args = 2;
  exn_table[MZEXN_OBJECT].args = 2;
  exn_table[MZEXN_UNIT].args = 2;
  exn_table[MZEXN_SYNTAX].args = 3;
  exn_table[MZEXN_READ].args = 3;
  exn_table[MZEXN_READ_EOF].args = 3;
  exn_table[MZEXN_I_O].args = 2;
  exn_table[MZEXN_I_O_PORT].args = 3;
  exn_table[MZEXN_I_O_PORT_READ].args = 3;
  exn_table[MZEXN_I_O_PORT_WRITE].args = 3;
  exn_table[MZEXN_I_O_PORT_CLOSED].args = 3;
  exn_table[MZEXN_I_O_PORT_USER].args = 3;
  exn_table[MZEXN_I_O_FILESYSTEM].args = 3;
  exn_table[MZEXN_I_O_TCP].args = 2;
  exn_table[MZEXN_MISC].args = 2;
  exn_table[MZEXN_MISC_UNSUPPORTED].args = 2;
  exn_table[MZEXN_MISC_USER_BREAK].args = 2;
  exn_table[MZEXN_MISC_OUT_OF_MEMORY].args = 2;
#endif

#endif

#ifdef _MZEXN_DECL_FIELDS

static const char *MZEXN_FIELDS[2] = { "message", "debug-info" };
static const char *MZEXN_VARIABLE_FIELDS[1] = { "id" };
static const char *MZEXN_APPLICATION_FIELDS[1] = { "value" };
static const char *MZEXN_APPLICATION_ARITY_FIELDS[1] = { "expected" };
static const char *MZEXN_APPLICATION_TYPE_FIELDS[1] = { "expected" };
static const char *MZEXN_SYNTAX_FIELDS[1] = { "expr" };
static const char *MZEXN_READ_FIELDS[1] = { "port" };
static const char *MZEXN_I_O_PORT_FIELDS[1] = { "port" };
static const char *MZEXN_I_O_FILESYSTEM_FIELDS[1] = { "pathname" };

#endif

#ifdef _MZEXN_SETUP

  SETUP_STRUCT(MZEXN, NULL, "exn", 2, MZEXN_FIELDS)
  SETUP_STRUCT(MZEXN_USER, EXN_PARENT(MZEXN), "exn:user", 0, NULL)
  SETUP_STRUCT(MZEXN_VARIABLE, EXN_PARENT(MZEXN), "exn:variable", 1, MZEXN_VARIABLE_FIELDS)
  SETUP_STRUCT(MZEXN_VARIABLE_KEYWORD, EXN_PARENT(MZEXN_VARIABLE), "exn:variable:keyword", 0, NULL)
  SETUP_STRUCT(MZEXN_APPLICATION, EXN_PARENT(MZEXN), "exn:application", 1, MZEXN_APPLICATION_FIELDS)
  SETUP_STRUCT(MZEXN_APPLICATION_ARITY, EXN_PARENT(MZEXN_APPLICATION), "exn:application:arity", 1, MZEXN_APPLICATION_ARITY_FIELDS)
  SETUP_STRUCT(MZEXN_APPLICATION_TYPE, EXN_PARENT(MZEXN_APPLICATION), "exn:application:type", 1, MZEXN_APPLICATION_TYPE_FIELDS)
  SETUP_STRUCT(MZEXN_APPLICATION_MISMATCH, EXN_PARENT(MZEXN_APPLICATION), "exn:application:mismatch", 0, NULL)
  SETUP_STRUCT(MZEXN_APPLICATION_DIVIDE_BY_ZERO, EXN_PARENT(MZEXN_APPLICATION), "exn:application:divide-by-zero", 0, NULL)
  SETUP_STRUCT(MZEXN_APPLICATION_CONTINUATION, EXN_PARENT(MZEXN_APPLICATION), "exn:application:continuation", 0, NULL)
  SETUP_STRUCT(MZEXN_ELSE, EXN_PARENT(MZEXN), "exn:else", 0, NULL)
  SETUP_STRUCT(MZEXN_STRUCT, EXN_PARENT(MZEXN), "exn:struct", 0, NULL)
  SETUP_STRUCT(MZEXN_OBJECT, EXN_PARENT(MZEXN), "exn:object", 0, NULL)
  SETUP_STRUCT(MZEXN_UNIT, EXN_PARENT(MZEXN), "exn:unit", 0, NULL)
  SETUP_STRUCT(MZEXN_SYNTAX, EXN_PARENT(MZEXN), "exn:syntax", 1, MZEXN_SYNTAX_FIELDS)
  SETUP_STRUCT(MZEXN_READ, EXN_PARENT(MZEXN), "exn:read", 1, MZEXN_READ_FIELDS)
  SETUP_STRUCT(MZEXN_READ_EOF, EXN_PARENT(MZEXN_READ), "exn:read:eof", 0, NULL)
  SETUP_STRUCT(MZEXN_I_O, EXN_PARENT(MZEXN), "exn:i/o", 0, NULL)
  SETUP_STRUCT(MZEXN_I_O_PORT, EXN_PARENT(MZEXN_I_O), "exn:i/o:port", 1, MZEXN_I_O_PORT_FIELDS)
  SETUP_STRUCT(MZEXN_I_O_PORT_READ, EXN_PARENT(MZEXN_I_O_PORT), "exn:i/o:port:read", 0, NULL)
  SETUP_STRUCT(MZEXN_I_O_PORT_WRITE, EXN_PARENT(MZEXN_I_O_PORT), "exn:i/o:port:write", 0, NULL)
  SETUP_STRUCT(MZEXN_I_O_PORT_CLOSED, EXN_PARENT(MZEXN_I_O_PORT), "exn:i/o:port:closed", 0, NULL)
  SETUP_STRUCT(MZEXN_I_O_PORT_USER, EXN_PARENT(MZEXN_I_O_PORT), "exn:i/o:port:user", 0, NULL)
  SETUP_STRUCT(MZEXN_I_O_FILESYSTEM, EXN_PARENT(MZEXN_I_O), "exn:i/o:filesystem", 1, MZEXN_I_O_FILESYSTEM_FIELDS)
  SETUP_STRUCT(MZEXN_I_O_TCP, EXN_PARENT(MZEXN_I_O), "exn:i/o:tcp", 0, NULL)
  SETUP_STRUCT(MZEXN_MISC, EXN_PARENT(MZEXN), "exn:misc", 0, NULL)
  SETUP_STRUCT(MZEXN_MISC_UNSUPPORTED, EXN_PARENT(MZEXN_MISC), "exn:misc:unsupported", 0, NULL)
  SETUP_STRUCT(MZEXN_MISC_USER_BREAK, EXN_PARENT(MZEXN_MISC), "exn:misc:user-break", 0, NULL)
  SETUP_STRUCT(MZEXN_MISC_OUT_OF_MEMORY, EXN_PARENT(MZEXN_MISC), "exn:misc:out-of-memory", 0, NULL)

#endif
