/* DO NOT EDIT THIS FILE. */
/* This file was generated by xctocc from "wxs_gage.xc". */


#if defined(_MSC_VER)
# include "wx.h"
#endif

#include "wx_gauge.h"




#ifdef wx_x
# define BM_SELECTED(map) ((map)->selectedTo)
#endif
#if defined(wx_mac) || defined(wx_msw)
# define BM_SELECTED(map) ((map)->selectedInto)
#endif
# define BM_IN_USE(map) ((map)->selectedIntoDC)





#include "wxscheme.h"
#include "wxs_gage.h"

#ifdef MZ_PRECISE_GC
START_XFORM_SKIP;
#endif

#ifdef MZ_PRECISE_GC
END_XFORM_SKIP;
#endif

class wxsGauge : public wxGauge
{
 public:
   int range, pos;

   wxsGauge(wxPanel *panel, char *label, int rng,
	    int x, int y, int width, int height,
	    long style, char *name);

   void SetRange(int r);

   void SetValue(int v);

   int GetValue(void) { return pos; }
   int GetRange(void) { return range; }
};

wxsGauge::wxsGauge(wxPanel *panel, char *label, int rng,
		   int x = -1, int y = -1, int width = -1, int height = -1,
		   long style = wxHORIZONTAL, char *name = "gauge")
: wxGauge(panel, label, rng, x, y, width, height,
	  style, name)
{
  range = rng; pos = 0;
}

void wxsGauge::SetRange(int r)
{
  if (r > 0) {
    range = r;
    wxGauge::SetRange(r);
    if (pos > r) {
      pos = r;
      wxGauge::SetValue(r);
    }
  }
}

void wxsGauge::SetValue(int v) {
  if (v >= 0 && v <= range) {
    pos = v;
    wxGauge::SetValue(v);
  }
}

#ifdef MZ_PRECISE_GC
START_XFORM_SKIP;
#endif

static Scheme_Object *gaugeStyle_wxVERTICAL_sym = NULL;
static Scheme_Object *gaugeStyle_wxHORIZONTAL_sym = NULL;

static void init_symset_gaugeStyle(void) {
  REMEMBER_VAR_STACK();
  wxREGGLOB(gaugeStyle_wxVERTICAL_sym);
  gaugeStyle_wxVERTICAL_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("vertical"));
  wxREGGLOB(gaugeStyle_wxHORIZONTAL_sym);
  gaugeStyle_wxHORIZONTAL_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("horizontal"));
}

static int unbundle_symset_gaugeStyle(Scheme_Object *v, const char *where) {
  SETUP_VAR_STACK(1);
  VAR_STACK_PUSH(0, v);
  if (!gaugeStyle_wxHORIZONTAL_sym) WITH_VAR_STACK(init_symset_gaugeStyle());
  Scheme_Object *i, *l = v;
  long result = 0;
  while (SCHEME_PAIRP(l)) {
  i = SCHEME_CAR(l);
  if (0) { }
  else if (i == gaugeStyle_wxVERTICAL_sym) { result = result | wxVERTICAL; }
  else if (i == gaugeStyle_wxHORIZONTAL_sym) { result = result | wxHORIZONTAL; }
  else { break; } 
  l = SCHEME_CDR(l);
  }
  if (SCHEME_NULLP(l)) return result;
  if (where) WITH_VAR_STACK(scheme_wrong_type(where, "gaugeStyle symbol list", -1, 0, &v));
  return 0;
}











class os_wxsGauge : public wxsGauge {
 public:

  os_wxsGauge CONSTRUCTOR_ARGS((class wxPanel* x0, nstring x1, int x2, int x3 = -1, int x4 = -1, int x5 = -1, int x6 = -1, int x7 = wxHORIZONTAL, string x8 = "gauge"));
  ~os_wxsGauge();
  void OnDropFile(pathname x0);
  Bool PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1);
  Bool PreOnChar(class wxWindow* x0, class wxKeyEvent* x1);
  void OnSize(int x0, int x1);
  void OnSetFocus();
  void OnKillFocus();
#ifdef MZ_PRECISE_GC
  void gcMark();
  void gcFixup();
#endif
};

#ifdef MZ_PRECISE_GC
void os_wxsGauge::gcMark() {
  wxsGauge::gcMark();
}
void os_wxsGauge::gcFixup() {
  wxsGauge::gcFixup();
}
#endif

static Scheme_Object *os_wxsGauge_class;

os_wxsGauge::os_wxsGauge CONSTRUCTOR_ARGS((class wxPanel* x0, nstring x1, int x2, int x3, int x4, int x5, int x6, int x7, string x8))
CONSTRUCTOR_INIT(: wxsGauge(x0, x1, x2, x3, x4, x5, x6, x7, x8))
{
}

os_wxsGauge::~os_wxsGauge()
{
    objscheme_destroy(this, (Scheme_Object *)__gc_external);
}

void os_wxsGauge::OnDropFile(pathname x0)
{
  Scheme_Object *p[1];
  Scheme_Object *v;
  Scheme_Object *method;
#ifdef MZ_PRECISE_GC
  os_wxsGauge *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(6);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, 1);
  VAR_STACK_PUSH(5, x0);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "on-drop-file", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxsGauge::OnDropFile(x0);
  } else {
  mz_jmp_buf savebuf;
  p[0] = WITH_VAR_STACK(objscheme_bundle_pathname((char *)x0));
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }

  v = WITH_VAR_STACK(scheme_apply(method, 1, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

Bool os_wxsGauge::PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  Scheme_Object *method;
#ifdef MZ_PRECISE_GC
  os_wxsGauge *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(7);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, 2);
  VAR_STACK_PUSH(5, x0);
  VAR_STACK_PUSH(6, x1);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "pre-on-event", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    return FALSE;
  } else {
  mz_jmp_buf savebuf;
  p[0] = WITH_VAR_STACK(objscheme_bundle_wxWindow(x0));
  p[1] = WITH_VAR_STACK(objscheme_bundle_wxMouseEvent(x1));
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return 1; }

  v = WITH_VAR_STACK(scheme_apply(method, 2, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  return WITH_VAR_STACK(objscheme_unbundle_bool(v, "pre-on-event in gauge%"", extracting return value"));
  }
}

Bool os_wxsGauge::PreOnChar(class wxWindow* x0, class wxKeyEvent* x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  Scheme_Object *method;
#ifdef MZ_PRECISE_GC
  os_wxsGauge *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(7);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, 2);
  VAR_STACK_PUSH(5, x0);
  VAR_STACK_PUSH(6, x1);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "pre-on-char", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    return FALSE;
  } else {
  mz_jmp_buf savebuf;
  p[0] = WITH_VAR_STACK(objscheme_bundle_wxWindow(x0));
  p[1] = WITH_VAR_STACK(objscheme_bundle_wxKeyEvent(x1));
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return 1; }

  v = WITH_VAR_STACK(scheme_apply(method, 2, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  return WITH_VAR_STACK(objscheme_unbundle_bool(v, "pre-on-char in gauge%"", extracting return value"));
  }
}

void os_wxsGauge::OnSize(int x0, int x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  Scheme_Object *method;
#ifdef MZ_PRECISE_GC
  os_wxsGauge *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(5);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, 2);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "on-size", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxsGauge::OnSize(x0, x1);
  } else {
  
  p[0] = scheme_make_integer(x0);
  p[1] = scheme_make_integer(x1);
  

  v = WITH_VAR_STACK(scheme_apply(method, 2, p));
  
  
  }
}

void os_wxsGauge::OnSetFocus()
{
  Scheme_Object **p = NULL;
  Scheme_Object *v;
  Scheme_Object *method;
#ifdef MZ_PRECISE_GC
  os_wxsGauge *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(2);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "on-set-focus", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxsGauge::OnSetFocus();
  } else {
  mz_jmp_buf savebuf;
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }

  v = WITH_VAR_STACK(scheme_apply(method, 0, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

void os_wxsGauge::OnKillFocus()
{
  Scheme_Object **p = NULL;
  Scheme_Object *v;
  Scheme_Object *method;
#ifdef MZ_PRECISE_GC
  os_wxsGauge *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(2);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "on-kill-focus", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxsGauge::OnKillFocus();
  } else {
  mz_jmp_buf savebuf;
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }

  v = WITH_VAR_STACK(scheme_apply(method, 0, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

static Scheme_Object *os_wxsGaugeGetValue(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  int r;
  objscheme_check_valid(obj);

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  

  
  r = WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->GetValue());

  
  
  return scheme_make_integer(r);
}

static Scheme_Object *os_wxsGaugeSetValue(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);
  int x0;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_integer(p[0], "set-value in gauge%"));

  
  WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->SetValue(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxsGaugeGetRange(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  int r;
  objscheme_check_valid(obj);

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  

  
  r = WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->GetRange());

  
  
  return scheme_make_integer(r);
}

static Scheme_Object *os_wxsGaugeSetRange(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);
  int x0;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_integer(p[0], "set-range in gauge%"));

  
  WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->SetRange(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxsGaugeOnDropFile(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);
  pathname x0;

  SETUP_VAR_STACK_REMEMBERED(3);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);
  VAR_STACK_PUSH(2, x0);

  
  x0 = (pathname)WITH_VAR_STACK(objscheme_unbundle_pathname(p[0], "on-drop-file in gauge%"));

  
  if (((Scheme_Class_Object *)obj)->primflag)
    WITH_VAR_STACK(((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->wxsGauge::OnDropFile(x0));
  else
    WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->OnDropFile(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxsGaugePreOnEvent(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  Bool r;
  objscheme_check_valid(obj);
  class wxWindow* x0;
  class wxMouseEvent* x1;

  SETUP_VAR_STACK_REMEMBERED(4);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);
  VAR_STACK_PUSH(2, x0);
  VAR_STACK_PUSH(3, x1);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxWindow(p[0], "pre-on-event in gauge%", 0));
  x1 = WITH_VAR_STACK(objscheme_unbundle_wxMouseEvent(p[1], "pre-on-event in gauge%", 0));

  
  if (((Scheme_Class_Object *)obj)->primflag)
    r = WITH_VAR_STACK(((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)-> wxWindow::PreOnEvent(x0, x1));
  else
    r = WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->PreOnEvent(x0, x1));

  
  
  return (r ? scheme_true : scheme_false);
}

static Scheme_Object *os_wxsGaugePreOnChar(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  Bool r;
  objscheme_check_valid(obj);
  class wxWindow* x0;
  class wxKeyEvent* x1;

  SETUP_VAR_STACK_REMEMBERED(4);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);
  VAR_STACK_PUSH(2, x0);
  VAR_STACK_PUSH(3, x1);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxWindow(p[0], "pre-on-char in gauge%", 0));
  x1 = WITH_VAR_STACK(objscheme_unbundle_wxKeyEvent(p[1], "pre-on-char in gauge%", 0));

  
  if (((Scheme_Class_Object *)obj)->primflag)
    r = WITH_VAR_STACK(((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)-> wxWindow::PreOnChar(x0, x1));
  else
    r = WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->PreOnChar(x0, x1));

  
  
  return (r ? scheme_true : scheme_false);
}

static Scheme_Object *os_wxsGaugeOnSize(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);
  int x0;
  int x1;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_integer(p[0], "on-size in gauge%"));
  x1 = WITH_VAR_STACK(objscheme_unbundle_integer(p[1], "on-size in gauge%"));

  
  if (((Scheme_Class_Object *)obj)->primflag)
    WITH_VAR_STACK(((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->wxsGauge::OnSize(x0, x1));
  else
    WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->OnSize(x0, x1));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxsGaugeOnSetFocus(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  

  
  if (((Scheme_Class_Object *)obj)->primflag)
    WITH_VAR_STACK(((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->wxsGauge::OnSetFocus());
  else
    WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->OnSetFocus());

  
  
  return scheme_void;
}

static Scheme_Object *os_wxsGaugeOnKillFocus(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  

  
  if (((Scheme_Class_Object *)obj)->primflag)
    WITH_VAR_STACK(((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->wxsGauge::OnKillFocus());
  else
    WITH_VAR_STACK(((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->OnKillFocus());

  
  
  return scheme_void;
}

static Scheme_Object *os_wxsGauge_ConstructScheme(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  SETUP_PRE_VAR_STACK(1);
  PRE_VAR_STACK_PUSH(0, obj);
  os_wxsGauge *realobj;
  REMEMBER_VAR_STACK();
  class wxPanel* x0;
  nstring x1;
  int x2;
  int x3;
  int x4;
  int x5;
  int x6;
  int x7;
  string x8;

  SETUP_VAR_STACK_PRE_REMEMBERED(6);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);
  VAR_STACK_PUSH(2, realobj);
  VAR_STACK_PUSH(3, x0);
  VAR_STACK_PUSH(4, x1);
  VAR_STACK_PUSH(5, x8);

  
  if ((n < 3) ||(n > 9)) 
    WITH_VAR_STACK(scheme_wrong_count("initialization in gauge%", 3, 9, n, p));
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxPanel(p[0], "initialization in gauge%", 0));
  x1 = (nstring)WITH_VAR_STACK(objscheme_unbundle_nullable_string(p[1], "initialization in gauge%"));
  x2 = WITH_VAR_STACK(objscheme_unbundle_integer(p[2], "initialization in gauge%"));
  if (n > 3) {
    x3 = WITH_VAR_STACK(objscheme_unbundle_integer(p[3], "initialization in gauge%"));
  } else
    x3 = -1;
  if (n > 4) {
    x4 = WITH_VAR_STACK(objscheme_unbundle_integer(p[4], "initialization in gauge%"));
  } else
    x4 = -1;
  if (n > 5) {
    x5 = WITH_VAR_STACK(objscheme_unbundle_integer(p[5], "initialization in gauge%"));
  } else
    x5 = -1;
  if (n > 6) {
    x6 = WITH_VAR_STACK(objscheme_unbundle_integer(p[6], "initialization in gauge%"));
  } else
    x6 = -1;
  if (n > 7) {
    x7 = WITH_VAR_STACK(unbundle_symset_gaugeStyle(p[7], "initialization in gauge%"));
  } else
    x7 = wxHORIZONTAL;
  if (n > 8) {
    x8 = (string)WITH_VAR_STACK(objscheme_unbundle_string(p[8], "initialization in gauge%"));
  } else
    x8 = "gauge";

  if (!x5) x5 = -1;if (!x6) x6 = -1;
  realobj = WITH_VAR_STACK(new os_wxsGauge CONSTRUCTOR_ARGS((x0, x1, x2, x3, x4, x5, x6, x7, x8)));
#ifdef MZ_PRECISE_GC
  WITH_VAR_STACK(realobj->gcInit_wxsGauge(x0, x1, x2, x3, x4, x5, x6, x7, x8));
#endif
  realobj->__gc_external = (void *)obj;
  objscheme_note_creation(obj);
  
  
  ((Scheme_Class_Object *)obj)->primdata = realobj;
  WITH_REMEMBERED_STACK(objscheme_register_primpointer(&((Scheme_Class_Object *)obj)->primdata));
  ((Scheme_Class_Object *)obj)->primflag = 1;
  return obj;
}

void objscheme_setup_wxsGauge(void *env)
{
  SETUP_VAR_STACK(1);
  VAR_STACK_PUSH(0, env);

  wxREGGLOB(os_wxsGauge_class);

  os_wxsGauge_class = WITH_VAR_STACK(objscheme_def_prim_class(env, "gauge%", "item%", os_wxsGauge_ConstructScheme, 10));

  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "get-value", os_wxsGaugeGetValue, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "set-value", os_wxsGaugeSetValue, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "get-range", os_wxsGaugeGetRange, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "set-range", os_wxsGaugeSetRange, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "on-drop-file", os_wxsGaugeOnDropFile, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "pre-on-event", os_wxsGaugePreOnEvent, 2, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "pre-on-char", os_wxsGaugePreOnChar, 2, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "on-size", os_wxsGaugeOnSize, 2, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "on-set-focus", os_wxsGaugeOnSetFocus, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxsGauge_class, "on-kill-focus", os_wxsGaugeOnKillFocus, 0, 0));


  WITH_VAR_STACK(scheme_made_class(os_wxsGauge_class));


}

int objscheme_istype_wxsGauge(Scheme_Object *obj, const char *stop, int nullOK)
{
  REMEMBER_VAR_STACK();
  if (nullOK && XC_SCHEME_NULLP(obj)) return 1;
  if (SAME_TYPE(SCHEME_TYPE(obj), scheme_object_type)
      && scheme_is_subclass(((Scheme_Class_Object *)obj)->sclass,          os_wxsGauge_class))
    return 1;
  else {
    if (!stop)
       return 0;
    WITH_REMEMBERED_STACK(scheme_wrong_type(stop, nullOK ? "gauge% object or " XC_NULL_STR: "gauge% object", -1, 0, &obj));
    return 0;
  }
}

Scheme_Object *objscheme_bundle_wxsGauge(class wxsGauge *realobj)
{
  Scheme_Class_Object *obj;
  Scheme_Object *sobj;

  if (!realobj) return XC_SCHEME_NULL;

  if (realobj->__gc_external)
    return (Scheme_Object *)realobj->__gc_external;

  SETUP_VAR_STACK(2);
  VAR_STACK_PUSH(0, obj);
  VAR_STACK_PUSH(1, realobj);

  if ((sobj = WITH_VAR_STACK(objscheme_bundle_by_type(realobj, realobj->__type))))
    return sobj;
  obj = (Scheme_Class_Object *)WITH_VAR_STACK(scheme_make_uninited_object(os_wxsGauge_class));

  obj->primdata = realobj;
  WITH_VAR_STACK(objscheme_register_primpointer(&obj->primdata));
  obj->primflag = 0;

  realobj->__gc_external = (void *)obj;
  return (Scheme_Object *)obj;
}

class wxsGauge *objscheme_unbundle_wxsGauge(Scheme_Object *obj, const char *where, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return NULL;

  REMEMBER_VAR_STACK();

  (void)objscheme_istype_wxsGauge(obj, where, nullOK);
  Scheme_Class_Object *o = (Scheme_Class_Object *)obj;
  WITH_REMEMBERED_STACK(objscheme_check_valid(obj));
  if (o->primflag)
    return (os_wxsGauge *)o->primdata;
  else
    return (wxsGauge *)o->primdata;
}

#ifdef MZ_PRECISE_GC
END_XFORM_SKIP;
#endif
