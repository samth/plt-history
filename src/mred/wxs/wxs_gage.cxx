/* DO NOT EDIT THIS FILE. */
/* This file was generated by xctocc from "wxs_gage.xc". */


#if defined(_MSC_VER)
# include "wx.h"
#endif

#include "wx_gauge.h"





class wxsGauge : public wxGauge
{
 public:
   int range, pos;

   wxsGauge(wxPanel *panel, char *label, int rng,
	    int x = -1, int y = -1, int width = -1, int height = -1,
	    long style = wxHORIZONTAL, char *name = "gauge")
    : wxGauge(panel, label, rng, x, y, width, height,
	      style, name)
  {
    range = rng; pos = 0;
  }
  void SetRange(int r) {
    if (r > 0) {
      range = r;
      wxGauge::SetRange(r);
      if (pos > r) {
       pos = r;
       wxGauge::SetValue(r);
      }
    }
  }
  void SetValue(int v) {
    if (v >= 0 && v <= range) {
     pos = v;
     wxGauge::SetValue(v);
    }
  }
  int GetValue(void) { return pos; }
  int GetRange(void) { return range; }
};

#include "wxscheme.h"
#include "wxs_gage.h"
#include "wxscomon.h"


static Scheme_Object *gaugeStyle_wxVERTICAL_sym = NULL;
static Scheme_Object *gaugeStyle_wxHORIZONTAL_sym = NULL;

static void init_symset_gaugeStyle(void) {
  gaugeStyle_wxVERTICAL_sym = scheme_intern_symbol("vertical");
  gaugeStyle_wxHORIZONTAL_sym = scheme_intern_symbol("horizontal");
}

static int unbundle_symset_gaugeStyle(Scheme_Object *v, const char *where) {
  if (!gaugeStyle_wxHORIZONTAL_sym) init_symset_gaugeStyle();
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
  if (where) scheme_wrong_type(where, "gaugeStyle symbol list", -1, 0, &v);
  return 0;
}

static int istype_symset_gaugeStyle(Scheme_Object *v, const char *where) {
  if (!gaugeStyle_wxHORIZONTAL_sym) init_symset_gaugeStyle();
  Scheme_Object *i, *l = v;
  long result = 1;
  while (SCHEME_PAIRP(l)) {
  i = SCHEME_CAR(l);
  if (0) { }
  else if (i == gaugeStyle_wxVERTICAL_sym) { ; }
  else if (i == gaugeStyle_wxHORIZONTAL_sym) { ; }
  else { break; } 
  l = SCHEME_CDR(l);
  }
  if (SCHEME_NULLP(l)) return result;
  if (where) scheme_wrong_type(where, "gaugeStyle symbol list", -1, 0, &v);
  return 0;
}

static Scheme_Object *bundle_symset_gaugeStyle(int v) {
  if (!gaugeStyle_wxHORIZONTAL_sym) init_symset_gaugeStyle();
  Scheme_Object *l = scheme_null;
  if (v & wxVERTICAL) l = scheme_make_pair(gaugeStyle_wxVERTICAL_sym, l);
  if (v & wxHORIZONTAL) l = scheme_make_pair(gaugeStyle_wxHORIZONTAL_sym, l);
  return l;
}










class os_wxsGauge : public wxsGauge {
 public:

  os_wxsGauge(Scheme_Object * obj, class wxPanel* x0, nstring x1, int x2, int x3 = -1, int x4 = -1, int x5 = -1, int x6 = -1, int x7 = wxHORIZONTAL, string x8 = "gauge");
  ~os_wxsGauge();
  Bool PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1);
  Bool PreOnChar(class wxWindow* x0, class wxKeyEvent* x1);
  void OnSize(int x0, int x1);
  void OnSetFocus();
  void OnKillFocus();
};

Scheme_Object *os_wxsGauge_class;

os_wxsGauge::os_wxsGauge(Scheme_Object * o, class wxPanel* x0, nstring x1, int x2, int x3, int x4, int x5, int x6, int x7, string x8)
: wxsGauge(x0, x1, x2, x3, x4, x5, x6, x7, x8)
{
  __gc_external = (void *)o;
  objscheme_backpointer(&__gc_external);
  objscheme_note_creation(o);
}

os_wxsGauge::~os_wxsGauge()
{
    objscheme_destroy(this, (Scheme_Object *)__gc_external);
}

Bool os_wxsGauge::PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "pre-on-event", &mcache);
  if (method && !OBJSCHEME_PRIM_METHOD(method)) {
    COPY_JMPBUF(savebuf, scheme_error_buf);
    sj = scheme_setjmp(scheme_error_buf);
    if (sj) {
      COPY_JMPBUF(scheme_error_buf, savebuf);
      scheme_clear_escape();
    }
  } else sj = 1;
  if (sj) {
return FALSE;
  } else {
  
  p[0] = objscheme_bundle_wxWindow(x0);
  p[1] = objscheme_bundle_wxMouseEvent(x1);
  

  v = scheme_apply(method, 2, p);
  
  
  COPY_JMPBUF(scheme_error_buf, savebuf);

  return objscheme_unbundle_bool(v, "gauge%::pre-on-event"", extracting return value");
  }
}

Bool os_wxsGauge::PreOnChar(class wxWindow* x0, class wxKeyEvent* x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "pre-on-char", &mcache);
  if (method && !OBJSCHEME_PRIM_METHOD(method)) {
    COPY_JMPBUF(savebuf, scheme_error_buf);
    sj = scheme_setjmp(scheme_error_buf);
    if (sj) {
      COPY_JMPBUF(scheme_error_buf, savebuf);
      scheme_clear_escape();
    }
  } else sj = 1;
  if (sj) {
return FALSE;
  } else {
  
  p[0] = objscheme_bundle_wxWindow(x0);
  p[1] = objscheme_bundle_wxKeyEvent(x1);
  

  v = scheme_apply(method, 2, p);
  
  
  COPY_JMPBUF(scheme_error_buf, savebuf);

  return objscheme_unbundle_bool(v, "gauge%::pre-on-char"", extracting return value");
  }
}

void os_wxsGauge::OnSize(int x0, int x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "on-size", &mcache);
  if (method && !OBJSCHEME_PRIM_METHOD(method)) {
    COPY_JMPBUF(savebuf, scheme_error_buf);
    sj = scheme_setjmp(scheme_error_buf);
    if (sj) {
      COPY_JMPBUF(scheme_error_buf, savebuf);
      scheme_clear_escape();
    }
  } else sj = 1;
  if (sj) {
wxsGauge::OnSize(x0, x1);
  } else {
  
  p[0] = scheme_make_integer(x0);
  p[1] = scheme_make_integer(x1);
  

  v = scheme_apply(method, 2, p);
  
  
  COPY_JMPBUF(scheme_error_buf, savebuf);

  }
}

void os_wxsGauge::OnSetFocus()
{
  Scheme_Object **p = NULL;
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "on-set-focus", &mcache);
  if (method && !OBJSCHEME_PRIM_METHOD(method)) {
    COPY_JMPBUF(savebuf, scheme_error_buf);
    sj = scheme_setjmp(scheme_error_buf);
    if (sj) {
      COPY_JMPBUF(scheme_error_buf, savebuf);
      scheme_clear_escape();
    }
  } else sj = 1;
  if (sj) {
wxsGauge::OnSetFocus();
  } else {
  
  

  v = scheme_apply(method, 0, p);
  
  
  COPY_JMPBUF(scheme_error_buf, savebuf);

  }
}

void os_wxsGauge::OnKillFocus()
{
  Scheme_Object **p = NULL;
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxsGauge_class, "on-kill-focus", &mcache);
  if (method && !OBJSCHEME_PRIM_METHOD(method)) {
    COPY_JMPBUF(savebuf, scheme_error_buf);
    sj = scheme_setjmp(scheme_error_buf);
    if (sj) {
      COPY_JMPBUF(scheme_error_buf, savebuf);
      scheme_clear_escape();
    }
  } else sj = 1;
  if (sj) {
wxsGauge::OnKillFocus();
  } else {
  
  

  v = scheme_apply(method, 0, p);
  
  
  COPY_JMPBUF(scheme_error_buf, savebuf);

  }
}

#pragma argsused
static Scheme_Object *os_wxsGaugeGetValue(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);

  

  
  r = ((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->GetValue();

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxsGaugeSetValue(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "gauge%::set-value");

  
  ((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->SetValue(x0);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxsGaugeGetRange(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);

  

  
  r = ((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->GetRange();

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxsGaugeSetRange(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "gauge%::set-range");

  
  ((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->SetRange(x0);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxsGaugePreOnEvent(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  Bool r;
  objscheme_check_valid(obj);
  class wxWindow* x0;
  class wxMouseEvent* x1;

  
  x0 = objscheme_unbundle_wxWindow(p[0], "gauge%::pre-on-event", 0);
  x1 = objscheme_unbundle_wxMouseEvent(p[1], "gauge%::pre-on-event", 0);

  
  if (((Scheme_Class_Object *)obj)->primflag)
    r = ((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)-> wxWindow::PreOnEvent(x0, x1);
  else
    r = ((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->PreOnEvent(x0, x1);

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxsGaugePreOnChar(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  Bool r;
  objscheme_check_valid(obj);
  class wxWindow* x0;
  class wxKeyEvent* x1;

  
  x0 = objscheme_unbundle_wxWindow(p[0], "gauge%::pre-on-char", 0);
  x1 = objscheme_unbundle_wxKeyEvent(p[1], "gauge%::pre-on-char", 0);

  
  if (((Scheme_Class_Object *)obj)->primflag)
    r = ((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)-> wxWindow::PreOnChar(x0, x1);
  else
    r = ((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->PreOnChar(x0, x1);

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxsGaugeOnSize(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  int x1;

  
  x0 = objscheme_unbundle_integer(p[0], "gauge%::on-size");
  x1 = objscheme_unbundle_integer(p[1], "gauge%::on-size");

  
  if (((Scheme_Class_Object *)obj)->primflag)
    ((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->wxsGauge::OnSize(x0, x1);
  else
    ((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->OnSize(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxsGaugeOnSetFocus(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);

  

  
  if (((Scheme_Class_Object *)obj)->primflag)
    ((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->wxsGauge::OnSetFocus();
  else
    ((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->OnSetFocus();

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxsGaugeOnKillFocus(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);

  

  
  if (((Scheme_Class_Object *)obj)->primflag)
    ((os_wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->wxsGauge::OnKillFocus();
  else
    ((wxsGauge *)((Scheme_Class_Object *)obj)->primdata)->OnKillFocus();

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxsGauge_ConstructScheme(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  os_wxsGauge *realobj;
  class wxPanel* x0;
  nstring x1;
  int x2;
  int x3;
  int x4;
  int x5;
  int x6;
  int x7;
  string x8;

  
  if ((n < 3) ||(n > 9)) 
    scheme_wrong_count("gauge%::initialization", 3, 9, n, p);
  x0 = objscheme_unbundle_wxPanel(p[0], "gauge%::initialization", 0);
  x1 = (nstring)objscheme_unbundle_nullable_string(p[1], "gauge%::initialization");
  x2 = objscheme_unbundle_integer(p[2], "gauge%::initialization");
  if (n > 3) {
    x3 = objscheme_unbundle_integer(p[3], "gauge%::initialization");
  } else
    x3 = -1;
  if (n > 4) {
    x4 = objscheme_unbundle_integer(p[4], "gauge%::initialization");
  } else
    x4 = -1;
  if (n > 5) {
    x5 = objscheme_unbundle_integer(p[5], "gauge%::initialization");
  } else
    x5 = -1;
  if (n > 6) {
    x6 = objscheme_unbundle_integer(p[6], "gauge%::initialization");
  } else
    x6 = -1;
  if (n > 7) {
    x7 = unbundle_symset_gaugeStyle(p[7], "gauge%::initialization");
  } else
    x7 = wxHORIZONTAL;
  if (n > 8) {
    x8 = (string)objscheme_unbundle_string(p[8], "gauge%::initialization");
  } else
    x8 = "gauge";

  if (!x5) x5 = -1;if (!x6) x6 = -1;
  realobj = new os_wxsGauge(obj, x0, x1, x2, x3, x4, x5, x6, x7, x8);
  
  
  ((Scheme_Class_Object *)obj)->primdata = realobj;
  objscheme_register_primpointer(&((Scheme_Class_Object *)obj)->primdata);
  ((Scheme_Class_Object *)obj)->primflag = 1;
  return obj;
}

static Scheme_Object *objscheme_classname_os_wxsGauge(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(obj);
  if (n) scheme_wrong_count("gauge%" "::get-class-name", 0, 0, n, p);
  return scheme_intern_symbol("gauge%");
}

void objscheme_setup_wxsGauge(void *env)
{
if (os_wxsGauge_class) {
    objscheme_add_global_class(os_wxsGauge_class, "gauge%", env);
} else {
  os_wxsGauge_class = objscheme_def_prim_class(env, "gauge%", "item%", os_wxsGauge_ConstructScheme, 10);

  scheme_add_method_w_arity(os_wxsGauge_class,"get-class-name",objscheme_classname_os_wxsGauge, 0, 0);

 scheme_add_method_w_arity(os_wxsGauge_class, "get-value", os_wxsGaugeGetValue, 0, 0);
 scheme_add_method_w_arity(os_wxsGauge_class, "set-value", os_wxsGaugeSetValue, 1, 1);
 scheme_add_method_w_arity(os_wxsGauge_class, "get-range", os_wxsGaugeGetRange, 0, 0);
 scheme_add_method_w_arity(os_wxsGauge_class, "set-range", os_wxsGaugeSetRange, 1, 1);
 scheme_add_method_w_arity(os_wxsGauge_class, "pre-on-event", os_wxsGaugePreOnEvent, 2, 2);
 scheme_add_method_w_arity(os_wxsGauge_class, "pre-on-char", os_wxsGaugePreOnChar, 2, 2);
 scheme_add_method_w_arity(os_wxsGauge_class, "on-size", os_wxsGaugeOnSize, 2, 2);
 scheme_add_method_w_arity(os_wxsGauge_class, "on-set-focus", os_wxsGaugeOnSetFocus, 0, 0);
 scheme_add_method_w_arity(os_wxsGauge_class, "on-kill-focus", os_wxsGaugeOnKillFocus, 0, 0);


  scheme_made_class(os_wxsGauge_class);


}
}

int objscheme_istype_wxsGauge(Scheme_Object *obj, const char *stop, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return 1;
  if (SAME_TYPE(SCHEME_TYPE(obj), scheme_object_type)
      && scheme_is_subclass(((Scheme_Class_Object *)obj)->sclass,          os_wxsGauge_class))
    return 1;
  else {
    if (!stop)
       return 0;
    scheme_wrong_type(stop, nullOK ? "gauge% object or " XC_NULL_STR: "gauge% object", -1, 0, &obj);
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
  if ((sobj = objscheme_bundle_by_type(realobj, realobj->__type)))
    return sobj;
  obj = (Scheme_Class_Object *)scheme_make_uninited_object(os_wxsGauge_class);

  obj->primdata = realobj;
  objscheme_register_primpointer(&obj->primdata);
  obj->primflag = 0;

  realobj->__gc_external = (void *)obj;
  objscheme_backpointer(&realobj->__gc_external);
  return (Scheme_Object *)obj;
}

class wxsGauge *objscheme_unbundle_wxsGauge(Scheme_Object *obj, const char *where, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return NULL;

  (void)objscheme_istype_wxsGauge(obj, where, nullOK);
  Scheme_Class_Object *o = (Scheme_Class_Object *)obj;
  objscheme_check_valid(obj);
  if (o->primflag)
    return (os_wxsGauge *)o->primdata;
  else
    return (wxsGauge *)o->primdata;
}

