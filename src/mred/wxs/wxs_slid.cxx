/* DO NOT EDIT THIS FILE. */
/* This file was generated by xctocc from "wxs_slid.xc". */


#if defined(_MSC_VER)
# include "wx.h"
#endif

#include "wx_slidr.h"




#ifdef wx_x
# define BM_SELECTED(map) ((map)->selectedTo)
#endif
#if defined(wx_mac) || defined(wx_msw)
# define BM_SELECTED(map) ((map)->selectedInto)
#endif
# define BM_IN_USE(map) ((map)->selectedIntoDC)





#include "wxscheme.h"
#include "wxs_slid.h"

#ifdef MZ_PRECISE_GC
START_XFORM_SKIP;
#endif

#define wxPLAIN_SLIDER (wxHORIZONTAL << 2)

static Scheme_Object *sliderStyle_wxVERTICAL_sym = NULL;
static Scheme_Object *sliderStyle_wxHORIZONTAL_sym = NULL;
static Scheme_Object *sliderStyle_wxPLAIN_SLIDER_sym = NULL;

static void init_symset_sliderStyle(void) {
  REMEMBER_VAR_STACK();
  wxREGGLOB(sliderStyle_wxVERTICAL_sym);
  sliderStyle_wxVERTICAL_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("vertical"));
  wxREGGLOB(sliderStyle_wxHORIZONTAL_sym);
  sliderStyle_wxHORIZONTAL_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("horizontal"));
  wxREGGLOB(sliderStyle_wxPLAIN_SLIDER_sym);
  sliderStyle_wxPLAIN_SLIDER_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("plain"));
}

static int unbundle_symset_sliderStyle(Scheme_Object *v, const char *where) {
  REMEMBER_VAR_STACK();
  if (!sliderStyle_wxPLAIN_SLIDER_sym) init_symset_sliderStyle();
  Scheme_Object *i, *l = v;
  long result = 0;
  while (SCHEME_PAIRP(l)) {
  i = SCHEME_CAR(l);
  if (0) { }
  else if (i == sliderStyle_wxVERTICAL_sym) { result = result | wxVERTICAL; }
  else if (i == sliderStyle_wxHORIZONTAL_sym) { result = result | wxHORIZONTAL; }
  else if (i == sliderStyle_wxPLAIN_SLIDER_sym) { result = result | wxPLAIN_SLIDER; }
  else { break; } 
  l = SCHEME_CDR(l);
  }
  if (SCHEME_NULLP(l)) return result;
  if (where) WITH_REMEMBERED_STACK(scheme_wrong_type(where, "sliderStyle symbol list", -1, 0, &v));
  return 0;
}




#define CB_FUNCTYPE wxFunction 


#undef CALLBACKCLASS
#undef CB_REALCLASS
#undef CB_UNBUNDLE
#undef CB_USER

#define CALLBACKCLASS os_wxSlider
#define CB_REALCLASS wxSlider
#define CB_UNBUNDLE objscheme_unbundle_wxSlider
#define CB_USER METHODNAME("slider%","initialization")

#undef CB_TOSCHEME
#undef CB_TOC
#define CB_TOSCHEME wxSliderCallbackToScheme
#define CB_TOC wxSliderCallbackToC


class CALLBACKCLASS;





extern wxCommandEvent *objscheme_unbundle_wxCommandEvent(Scheme_Object *,const char *,int);
extern Scheme_Object *objscheme_bundle_wxCommandEvent(wxCommandEvent *);

static void CB_TOSCHEME(CB_REALCLASS *obj, wxCommandEvent *event);










class os_wxSlider : public wxSlider {
 public:
  Scheme_Object *callback_closure;

  os_wxSlider CONSTRUCTOR_ARGS((class wxPanel* x0, wxFunction x1, nstring x2, int x3, int x4, int x5, int x6, int x7 = -1, int x8 = -1, int x9 = wxHORIZONTAL, string x10 = "slider"));
  ~os_wxSlider();
  void OnDropFile(pathname x0);
  Bool PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1);
  Bool PreOnChar(class wxWindow* x0, class wxKeyEvent* x1);
  void OnSize(int x0, int x1);
  void OnSetFocus();
  void OnKillFocus();
#ifdef MZ_PRECISE_GC
  void gcMark(Mark_Proc mark);
#endif
};

#ifdef MZ_PRECISE_GC
void os_wxSlider::gcMark(Mark_Proc mark) {
  wxSlider::gcMark(mark);
  if (mark) {
    gcMARK_TYPED(Scheme_Object *, callback_closure);
  }
}
#endif

static Scheme_Object *os_wxSlider_class;

os_wxSlider::os_wxSlider CONSTRUCTOR_ARGS((class wxPanel* x0, wxFunction x1, nstring x2, int x3, int x4, int x5, int x6, int x7, int x8, int x9, string x10))
CONSTRUCTOR_INIT(: wxSlider(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10))
{
}

os_wxSlider::~os_wxSlider()
{
    objscheme_destroy(this, (Scheme_Object *)__gc_external);
}

void os_wxSlider::OnDropFile(pathname x0)
{
  Scheme_Object *p[1];
  Scheme_Object *v;
  Scheme_Object *method;
  static void *mcache = 0;

  SETUP_VAR_STACK(5);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH_ARRAY(1, p, 1);
  VAR_STACK_PUSH(4, x0);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxSlider_class, "on-drop-file", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    wxSlider::OnDropFile(x0);
  } else {
  mz_jmp_buf savebuf;
  p[0] = WITH_VAR_STACK(objscheme_bundle_pathname((char *)x0));
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }

  v = WITH_VAR_STACK(scheme_apply(method, 1, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

Bool os_wxSlider::PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  Scheme_Object *method;
  static void *mcache = 0;

  SETUP_VAR_STACK(6);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH_ARRAY(1, p, 2);
  VAR_STACK_PUSH(4, x0);
  VAR_STACK_PUSH(5, x1);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxSlider_class, "pre-on-event", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    return FALSE;
  } else {
  mz_jmp_buf savebuf;
  p[0] = WITH_VAR_STACK(objscheme_bundle_wxWindow(x0));
  p[1] = WITH_VAR_STACK(objscheme_bundle_wxMouseEvent(x1));
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return 0; }

  v = WITH_VAR_STACK(scheme_apply(method, 2, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  return WITH_VAR_STACK(objscheme_unbundle_bool(v, "pre-on-event in slider%"", extracting return value"));
  }
}

Bool os_wxSlider::PreOnChar(class wxWindow* x0, class wxKeyEvent* x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  Scheme_Object *method;
  static void *mcache = 0;

  SETUP_VAR_STACK(6);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH_ARRAY(1, p, 2);
  VAR_STACK_PUSH(4, x0);
  VAR_STACK_PUSH(5, x1);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxSlider_class, "pre-on-char", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    return FALSE;
  } else {
  mz_jmp_buf savebuf;
  p[0] = WITH_VAR_STACK(objscheme_bundle_wxWindow(x0));
  p[1] = WITH_VAR_STACK(objscheme_bundle_wxKeyEvent(x1));
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return 0; }

  v = WITH_VAR_STACK(scheme_apply(method, 2, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  return WITH_VAR_STACK(objscheme_unbundle_bool(v, "pre-on-char in slider%"", extracting return value"));
  }
}

void os_wxSlider::OnSize(int x0, int x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  Scheme_Object *method;
  static void *mcache = 0;

  SETUP_VAR_STACK(4);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH_ARRAY(1, p, 2);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxSlider_class, "on-size", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    wxSlider::OnSize(x0, x1);
  } else {
  
  p[0] = scheme_make_integer(x0);
  p[1] = scheme_make_integer(x1);
  

  v = WITH_VAR_STACK(scheme_apply(method, 2, p));
  
  
  }
}

void os_wxSlider::OnSetFocus()
{
  Scheme_Object **p = NULL;
  Scheme_Object *v;
  Scheme_Object *method;
  static void *mcache = 0;

  SETUP_VAR_STACK(1);
  VAR_STACK_PUSH(0, method);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxSlider_class, "on-set-focus", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    wxSlider::OnSetFocus();
  } else {
  mz_jmp_buf savebuf;
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }

  v = WITH_VAR_STACK(scheme_apply(method, 0, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

void os_wxSlider::OnKillFocus()
{
  Scheme_Object **p = NULL;
  Scheme_Object *v;
  Scheme_Object *method;
  static void *mcache = 0;

  SETUP_VAR_STACK(1);
  VAR_STACK_PUSH(0, method);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxSlider_class, "on-kill-focus", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    wxSlider::OnKillFocus();
  } else {
  mz_jmp_buf savebuf;
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }

  v = WITH_VAR_STACK(scheme_apply(method, 0, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

#pragma argsused
static Scheme_Object *os_wxSliderSetValue(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);
  int x0;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_integer(p[0], "set-value in slider%"));

  
  WITH_VAR_STACK(((wxSlider *)((Scheme_Class_Object *)obj)->primdata)->SetValue(x0));

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxSliderGetValue(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  int r;
  objscheme_check_valid(obj);

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  

  
  r = WITH_VAR_STACK(((wxSlider *)((Scheme_Class_Object *)obj)->primdata)->GetValue());

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxSliderOnDropFile(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);
  pathname x0;

  SETUP_VAR_STACK_REMEMBERED(3);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);
  VAR_STACK_PUSH(2, x0);

  
  x0 = (pathname)WITH_VAR_STACK(objscheme_unbundle_pathname(p[0], "on-drop-file in slider%"));

  
  if (((Scheme_Class_Object *)obj)->primflag)
    WITH_VAR_STACK(((os_wxSlider *)((Scheme_Class_Object *)obj)->primdata)->wxSlider::OnDropFile(x0));
  else
    WITH_VAR_STACK(((wxSlider *)((Scheme_Class_Object *)obj)->primdata)->OnDropFile(x0));

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxSliderPreOnEvent(Scheme_Object *obj, int n,  Scheme_Object *p[])
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

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxWindow(p[0], "pre-on-event in slider%", 0));
  x1 = WITH_VAR_STACK(objscheme_unbundle_wxMouseEvent(p[1], "pre-on-event in slider%", 0));

  
  if (((Scheme_Class_Object *)obj)->primflag)
    r = WITH_VAR_STACK(((os_wxSlider *)((Scheme_Class_Object *)obj)->primdata)-> wxWindow::PreOnEvent(x0, x1));
  else
    r = WITH_VAR_STACK(((wxSlider *)((Scheme_Class_Object *)obj)->primdata)->PreOnEvent(x0, x1));

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxSliderPreOnChar(Scheme_Object *obj, int n,  Scheme_Object *p[])
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

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxWindow(p[0], "pre-on-char in slider%", 0));
  x1 = WITH_VAR_STACK(objscheme_unbundle_wxKeyEvent(p[1], "pre-on-char in slider%", 0));

  
  if (((Scheme_Class_Object *)obj)->primflag)
    r = WITH_VAR_STACK(((os_wxSlider *)((Scheme_Class_Object *)obj)->primdata)-> wxWindow::PreOnChar(x0, x1));
  else
    r = WITH_VAR_STACK(((wxSlider *)((Scheme_Class_Object *)obj)->primdata)->PreOnChar(x0, x1));

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxSliderOnSize(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);
  int x0;
  int x1;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_integer(p[0], "on-size in slider%"));
  x1 = WITH_VAR_STACK(objscheme_unbundle_integer(p[1], "on-size in slider%"));

  
  if (((Scheme_Class_Object *)obj)->primflag)
    WITH_VAR_STACK(((os_wxSlider *)((Scheme_Class_Object *)obj)->primdata)->wxSlider::OnSize(x0, x1));
  else
    WITH_VAR_STACK(((wxSlider *)((Scheme_Class_Object *)obj)->primdata)->OnSize(x0, x1));

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxSliderOnSetFocus(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  

  
  if (((Scheme_Class_Object *)obj)->primflag)
    WITH_VAR_STACK(((os_wxSlider *)((Scheme_Class_Object *)obj)->primdata)->wxSlider::OnSetFocus());
  else
    WITH_VAR_STACK(((wxSlider *)((Scheme_Class_Object *)obj)->primdata)->OnSetFocus());

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxSliderOnKillFocus(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(obj);

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);

  

  
  if (((Scheme_Class_Object *)obj)->primflag)
    WITH_VAR_STACK(((os_wxSlider *)((Scheme_Class_Object *)obj)->primdata)->wxSlider::OnKillFocus());
  else
    WITH_VAR_STACK(((wxSlider *)((Scheme_Class_Object *)obj)->primdata)->OnKillFocus());

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxSlider_ConstructScheme(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  os_wxSlider *realobj;
  REMEMBER_VAR_STACK();
  class wxPanel* x0;
  wxFunction x1;
  nstring x2;
  int x3;
  int x4;
  int x5;
  int x6;
  int x7;
  int x8;
  int x9;
  string x10;

  SETUP_VAR_STACK_REMEMBERED(5);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, obj);
  VAR_STACK_PUSH(2, x0);
  VAR_STACK_PUSH(3, x2);
  VAR_STACK_PUSH(4, x10);

  Scheme_Object *tmp_callback = NULL;
  if ((n < 7) ||(n > 11)) 
    WITH_VAR_STACK(scheme_wrong_count("initialization in slider%", 7, 11, n, p));
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxPanel(p[0], "initialization in slider%", 0));
  x1 = (SCHEME_NULLP(p[1]) ? NULL : (WXGC_IGNORE(tmp_callback), WITH_REMEMBERED_STACK(objscheme_istype_proc2(p[1], CB_USER)), tmp_callback = p[1], (CB_FUNCTYPE)CB_TOSCHEME));
  x2 = (nstring)WITH_VAR_STACK(objscheme_unbundle_nullable_string(p[2], "initialization in slider%"));
  x3 = WITH_VAR_STACK(objscheme_unbundle_integer(p[3], "initialization in slider%"));
  x4 = WITH_VAR_STACK(objscheme_unbundle_integer(p[4], "initialization in slider%"));
  x5 = WITH_VAR_STACK(objscheme_unbundle_integer(p[5], "initialization in slider%"));
  x6 = WITH_VAR_STACK(objscheme_unbundle_integer(p[6], "initialization in slider%"));
  if (n > 7) {
    x7 = WITH_VAR_STACK(objscheme_unbundle_integer(p[7], "initialization in slider%"));
  } else
    x7 = -1;
  if (n > 8) {
    x8 = WITH_VAR_STACK(objscheme_unbundle_integer(p[8], "initialization in slider%"));
  } else
    x8 = -1;
  if (n > 9) {
    x9 = WITH_VAR_STACK(unbundle_symset_sliderStyle(p[9], "initialization in slider%"));
  } else
    x9 = wxHORIZONTAL;
  if (n > 10) {
    x10 = (string)WITH_VAR_STACK(objscheme_unbundle_string(p[10], "initialization in slider%"));
  } else
    x10 = "slider";

  if (x3 < x4 || x5 < x3) WITH_VAR_STACK(scheme_arg_mismatch(METHODNAME("slider%","initialization"), "minimum, value, and maximum must be increasing; given minimum: ", p[4]));if (x6 <= 0) x6 = 1;
  realobj = WITH_VAR_STACK(new os_wxSlider CONSTRUCTOR_ARGS((x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10)));
#ifdef MZ_PRECISE_GC
  WITH_VAR_STACK(realobj->gcInit_wxSlider(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10));
#endif
  realobj->__gc_external = (void *)obj;
  objscheme_note_creation(obj);
  
  realobj->callback_closure = tmp_callback;
  ((Scheme_Class_Object *)obj)->primdata = realobj;
  objscheme_register_primpointer(&((Scheme_Class_Object *)obj)->primdata);
  ((Scheme_Class_Object *)obj)->primflag = 1;
  return obj;
}

void objscheme_setup_wxSlider(void *env)
{
  SETUP_VAR_STACK(1);
  VAR_STACK_PUSH(0, env);

  wxREGGLOB(os_wxSlider_class);

  os_wxSlider_class = objscheme_def_prim_class(env, "slider%", "item%", os_wxSlider_ConstructScheme, 8);

  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxSlider_class, "set-value", os_wxSliderSetValue, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxSlider_class, "get-value", os_wxSliderGetValue, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxSlider_class, "on-drop-file", os_wxSliderOnDropFile, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxSlider_class, "pre-on-event", os_wxSliderPreOnEvent, 2, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxSlider_class, "pre-on-char", os_wxSliderPreOnChar, 2, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxSlider_class, "on-size", os_wxSliderOnSize, 2, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxSlider_class, "on-set-focus", os_wxSliderOnSetFocus, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxSlider_class, "on-kill-focus", os_wxSliderOnKillFocus, 0, 0));


  WITH_VAR_STACK(scheme_made_class(os_wxSlider_class));


}

int objscheme_istype_wxSlider(Scheme_Object *obj, const char *stop, int nullOK)
{
  REMEMBER_VAR_STACK();
  if (nullOK && XC_SCHEME_NULLP(obj)) return 1;
  if (SAME_TYPE(SCHEME_TYPE(obj), scheme_object_type)
      && scheme_is_subclass(((Scheme_Class_Object *)obj)->sclass,          os_wxSlider_class))
    return 1;
  else {
    if (!stop)
       return 0;
    WITH_REMEMBERED_STACK(scheme_wrong_type(stop, nullOK ? "slider% object or " XC_NULL_STR: "slider% object", -1, 0, &obj));
    return 0;
  }
}

Scheme_Object *objscheme_bundle_wxSlider(class wxSlider *realobj)
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
  obj = (Scheme_Class_Object *)WITH_VAR_STACK(scheme_make_uninited_object(os_wxSlider_class));

  obj->primdata = realobj;
  WITH_VAR_STACK(objscheme_register_primpointer(&obj->primdata));
  obj->primflag = 0;

  realobj->__gc_external = (void *)obj;
  return (Scheme_Object *)obj;
}

class wxSlider *objscheme_unbundle_wxSlider(Scheme_Object *obj, const char *where, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return NULL;

  REMEMBER_VAR_STACK();

  (void)objscheme_istype_wxSlider(obj, where, nullOK);
  Scheme_Class_Object *o = (Scheme_Class_Object *)obj;
  WITH_REMEMBERED_STACK(objscheme_check_valid(obj));
  if (o->primflag)
    return (os_wxSlider *)o->primdata;
  else
    return (wxSlider *)o->primdata;
}



static void CB_TOSCHEME(CB_REALCLASS *realobj, wxCommandEvent *event)
{
  Scheme_Object *p[2];
  Scheme_Class_Object *obj;
  mz_jmp_buf savebuf;
  SETUP_VAR_STACK(4);
  VAR_STACK_PUSH(0, obj);
  VAR_STACK_PUSH(1, event);
  VAR_STACK_PUSH(2, p[0]);
  VAR_STACK_PUSH(3, p[1]);

  obj = (Scheme_Class_Object *)realobj->__gc_external;

  if (!obj) {
    // scheme_signal_error("bad callback object");
    return;
  }

  p[0] = (Scheme_Object *)obj;
  p[1] = WITH_VAR_STACK(objscheme_bundle_wxCommandEvent(event));

  COPY_JMPBUF(savebuf, scheme_error_buf);

  if (!scheme_setjmp(scheme_error_buf))
    WITH_VAR_STACK(scheme_apply_multi(((CALLBACKCLASS *)obj->primdata)->callback_closure, 2, p));

  COPY_JMPBUF(scheme_error_buf, savebuf);
}
#ifdef MZ_PRECISE_GC
END_XFORM_SKIP;
#endif
