/* DO NOT EDIT THIS FILE. */
/* This file was generated by xctocc from "wxs_fram.xc". */


#if defined(_MSC_VER)
# include "wx.h"
#endif

#include "wx_frame.h"
#include "wx_gdi.h"




#ifdef wx_x
# define BM_SELECTED(map) ((map)->selectedTo)
#endif
#if defined(wx_mac) || defined(wx_msw)
# define BM_SELECTED(map) ((map)->selectedInto)
#endif
# define BM_IN_USE(map) ((map)->selectedIntoDC)





#include "wxscheme.h"
#include "wxs_fram.h"

#ifdef MZ_PRECISE_GC
START_XFORM_SKIP;
#endif

#ifdef wx_xt
#define GET_THE_MENU_BAR(f) (f)->GetMenuBar()
#else
#define GET_THE_MENU_BAR(f) (f)->wx_menu_bar
#endif

#ifdef wx_motif
#define wxALLOW_AUTO_RESIZE wxPUSH_PIN
#else
#define wxALLOW_AUTO_RESIZE 0
#endif



extern void *wxsCheckEventspace(char *);

static wxMenuBar *GetTheMenuBar(wxFrame *f)
{
  return GET_THE_MENU_BAR(f);
}

static Scheme_Object *frameStyle_wxNO_CAPTION_sym = NULL;
static Scheme_Object *frameStyle_wxMDI_PARENT_sym = NULL;
static Scheme_Object *frameStyle_wxMDI_CHILD_sym = NULL;
static Scheme_Object *frameStyle_wxNO_SYSTEM_MENU_sym = NULL;
static Scheme_Object *frameStyle_wxNO_RESIZE_BORDER_sym = NULL;

static void init_symset_frameStyle(void) {
  REMEMBER_VAR_STACK();
  wxREGGLOB(frameStyle_wxNO_CAPTION_sym);
  frameStyle_wxNO_CAPTION_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("no-caption"));
  wxREGGLOB(frameStyle_wxMDI_PARENT_sym);
  frameStyle_wxMDI_PARENT_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("mdi-parent"));
  wxREGGLOB(frameStyle_wxMDI_CHILD_sym);
  frameStyle_wxMDI_CHILD_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("mdi-child"));
  wxREGGLOB(frameStyle_wxNO_SYSTEM_MENU_sym);
  frameStyle_wxNO_SYSTEM_MENU_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("no-system-menu"));
  wxREGGLOB(frameStyle_wxNO_RESIZE_BORDER_sym);
  frameStyle_wxNO_RESIZE_BORDER_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("no-resize-border"));
}

static int unbundle_symset_frameStyle(Scheme_Object *v, const char *where) {
  SETUP_VAR_STACK(1);
  VAR_STACK_PUSH(0, v);
  if (!frameStyle_wxNO_RESIZE_BORDER_sym) WITH_VAR_STACK(init_symset_frameStyle());
  Scheme_Object *i INIT_NULLED_OUT, *l = v;
  long result = 0;
  while (SCHEME_PAIRP(l)) {
  i = SCHEME_CAR(l);
  if (0) { }
  else if (i == frameStyle_wxNO_CAPTION_sym) { result = result | wxNO_CAPTION; }
  else if (i == frameStyle_wxMDI_PARENT_sym) { result = result | wxMDI_PARENT; }
  else if (i == frameStyle_wxMDI_CHILD_sym) { result = result | wxMDI_CHILD; }
  else if (i == frameStyle_wxNO_SYSTEM_MENU_sym) { result = result | wxNO_SYSTEM_MENU; }
  else if (i == frameStyle_wxNO_RESIZE_BORDER_sym) { result = result | wxNO_RESIZE_BORDER; }
  else { break; } 
  l = SCHEME_CDR(l);
  }
  if (SCHEME_NULLP(l)) return result;
  if (where) WITH_VAR_STACK(scheme_wrong_type(where, "frameStyle symbol list", -1, 0, &v));
  return 0;
}


static Scheme_Object *iconKind_0_sym = NULL;
static Scheme_Object *iconKind_1_sym = NULL;
static Scheme_Object *iconKind_2_sym = NULL;

static void init_symset_iconKind(void) {
  REMEMBER_VAR_STACK();
  wxREGGLOB(iconKind_0_sym);
  iconKind_0_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("both"));
  wxREGGLOB(iconKind_1_sym);
  iconKind_1_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("small"));
  wxREGGLOB(iconKind_2_sym);
  iconKind_2_sym = WITH_REMEMBERED_STACK(scheme_intern_symbol("large"));
}

static int unbundle_symset_iconKind(Scheme_Object *v, const char *where) {
  SETUP_VAR_STACK(1);
  VAR_STACK_PUSH(0, v);
  if (!iconKind_2_sym) WITH_VAR_STACK(init_symset_iconKind());
  if (0) { }
  else if (v == iconKind_0_sym) { return 0; }
  else if (v == iconKind_1_sym) { return 1; }
  else if (v == iconKind_2_sym) { return 2; }
  if (where) WITH_VAR_STACK(scheme_wrong_type(where, "iconKind symbol", -1, 0, &v));
  return 0;
}


#ifdef wx_msw
# define XTMAC_UNUSED(x) x
#else
# define XTMAC_UNUSED(x) /**/
#endif

static void frameMenu(wxFrame *XTMAC_UNUSED(f))
{
#ifdef wx_msw
  f->SystemMenu();
#endif
}















class os_wxFrame : public wxFrame {
 public:

  os_wxFrame CONSTRUCTOR_ARGS((class wxFrame* x0, string x1, int x2 = -1, int x3 = -1, int x4 = -1, int x5 = -1, int x6 = 0, string x7 = "frame"));
  ~os_wxFrame();
  void OnDropFile(pathname x0);
  Bool PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1);
  Bool PreOnChar(class wxWindow* x0, class wxKeyEvent* x1);
  void OnSize(int x0, int x1);
  void OnSetFocus();
  void OnKillFocus();
  void OnMenuClick();
  void OnMenuCommand(ExactLong x0);
  Bool OnClose();
  void OnActivate(Bool x0);
#ifdef MZ_PRECISE_GC
  void gcMark();
  void gcFixup();
#endif
};

#ifdef MZ_PRECISE_GC
void os_wxFrame::gcMark() {
  wxFrame::gcMark();
}
void os_wxFrame::gcFixup() {
  wxFrame::gcFixup();
}
#endif

static Scheme_Object *os_wxFrame_class;

os_wxFrame::os_wxFrame CONSTRUCTOR_ARGS((class wxFrame* x0, string x1, int x2, int x3, int x4, int x5, int x6, string x7))
CONSTRUCTOR_INIT(: wxFrame(x0, x1, x2, x3, x4, x5, x6, x7))
{
}

os_wxFrame::~os_wxFrame()
{
    objscheme_destroy(this, (Scheme_Object *) __gc_external);
}

void os_wxFrame::OnDropFile(pathname x0)
{
  Scheme_Object *p[POFFSET+1] INIT_NULLED_ARRAY({ NULLED_OUT INA_comma NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(6);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+1);
  VAR_STACK_PUSH(5, x0);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "on-drop-file", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxFrame::OnDropFile(x0);
  } else {
  mz_jmp_buf savebuf;
  p[POFFSET+0] = WITH_VAR_STACK(objscheme_bundle_pathname((char *)x0));
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+1, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

Bool os_wxFrame::PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1)
{
  Scheme_Object *p[POFFSET+2] INIT_NULLED_ARRAY({ NULLED_OUT INA_comma NULLED_OUT INA_comma NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(7);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+2);
  VAR_STACK_PUSH(5, x0);
  VAR_STACK_PUSH(6, x1);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "pre-on-event", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    return FALSE;
  } else {
  mz_jmp_buf savebuf;
  p[POFFSET+0] = WITH_VAR_STACK(objscheme_bundle_wxWindow(x0));
  p[POFFSET+1] = WITH_VAR_STACK(objscheme_bundle_wxMouseEvent(x1));
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return 1; }
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+2, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  return WITH_VAR_STACK(objscheme_unbundle_bool(v, "pre-on-event in frame%"", extracting return value"));
  }
}

Bool os_wxFrame::PreOnChar(class wxWindow* x0, class wxKeyEvent* x1)
{
  Scheme_Object *p[POFFSET+2] INIT_NULLED_ARRAY({ NULLED_OUT INA_comma NULLED_OUT INA_comma NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(7);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+2);
  VAR_STACK_PUSH(5, x0);
  VAR_STACK_PUSH(6, x1);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "pre-on-char", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    return FALSE;
  } else {
  mz_jmp_buf savebuf;
  p[POFFSET+0] = WITH_VAR_STACK(objscheme_bundle_wxWindow(x0));
  p[POFFSET+1] = WITH_VAR_STACK(objscheme_bundle_wxKeyEvent(x1));
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return 1; }
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+2, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  return WITH_VAR_STACK(objscheme_unbundle_bool(v, "pre-on-char in frame%"", extracting return value"));
  }
}

void os_wxFrame::OnSize(int x0, int x1)
{
  Scheme_Object *p[POFFSET+2] INIT_NULLED_ARRAY({ NULLED_OUT INA_comma NULLED_OUT INA_comma NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(5);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+2);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "on-size", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxFrame::OnSize(x0, x1);
  } else {
  
  p[POFFSET+0] = scheme_make_integer(x0);
  p[POFFSET+1] = scheme_make_integer(x1);
  
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+2, p));
  
  
  }
}

void os_wxFrame::OnSetFocus()
{
  Scheme_Object *p[POFFSET+0] INIT_NULLED_ARRAY({ NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(5);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+0);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "on-set-focus", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxFrame::OnSetFocus();
  } else {
  mz_jmp_buf savebuf;
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+0, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

void os_wxFrame::OnKillFocus()
{
  Scheme_Object *p[POFFSET+0] INIT_NULLED_ARRAY({ NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(5);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+0);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "on-kill-focus", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxFrame::OnKillFocus();
  } else {
  mz_jmp_buf savebuf;
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+0, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

void os_wxFrame::OnMenuClick()
{
  Scheme_Object *p[POFFSET+0] INIT_NULLED_ARRAY({ NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(5);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+0);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "on-menu-click", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxFrame::OnMenuClick();
  } else {
  
  
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+0, p));
  
  
  }
}

void os_wxFrame::OnMenuCommand(ExactLong x0)
{
  Scheme_Object *p[POFFSET+1] INIT_NULLED_ARRAY({ NULLED_OUT INA_comma NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(5);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+1);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "on-menu-command", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxFrame::OnMenuCommand(x0);
  } else {
  
  p[POFFSET+0] = WITH_VAR_STACK(scheme_make_integer_value(x0));
  
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+1, p));
  
  
  }
}

Bool os_wxFrame::OnClose()
{
  Scheme_Object *p[POFFSET+0] INIT_NULLED_ARRAY({ NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(5);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+0);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "on-close", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    return ASSELF wxFrame::OnClose();
  } else {
  mz_jmp_buf savebuf;
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return 0; }
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+0, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  return WITH_VAR_STACK(objscheme_unbundle_bool(v, "on-close in frame%"", extracting return value"));
  }
}

void os_wxFrame::OnActivate(Bool x0)
{
  Scheme_Object *p[POFFSET+1] INIT_NULLED_ARRAY({ NULLED_OUT INA_comma NULLED_OUT });
  Scheme_Object *v;
  Scheme_Object *method INIT_NULLED_OUT;
#ifdef MZ_PRECISE_GC
  os_wxFrame *sElF = this;
#endif
  static void *mcache = 0;

  SETUP_VAR_STACK(5);
  VAR_STACK_PUSH(0, method);
  VAR_STACK_PUSH(1, sElF);
  VAR_STACK_PUSH_ARRAY(2, p, POFFSET+1);
  SET_VAR_STACK();

  method = objscheme_find_method((Scheme_Object *) ASSELF __gc_external, os_wxFrame_class, "on-activate", &mcache);
  if (!method || OBJSCHEME_PRIM_METHOD(method)) {
    SET_VAR_STACK();
    ASSELF wxFrame::OnActivate(x0);
  } else {
  mz_jmp_buf savebuf;
  p[POFFSET+0] = (x0 ? scheme_true : scheme_false);
  COPY_JMPBUF(savebuf, scheme_error_buf); if (scheme_setjmp(scheme_error_buf)) { COPY_JMPBUF(scheme_error_buf, savebuf); return; }
  p[0] = (Scheme_Object *) ASSELF __gc_external;

  v = WITH_VAR_STACK(scheme_apply(method, POFFSET+1, p));
  COPY_JMPBUF(scheme_error_buf, savebuf);
  
  }
}

static Scheme_Object *os_wxFrameOnDropFile(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "on-drop-file in frame%", n, p);
  pathname x0 INIT_NULLED_OUT;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, x0);

  
  x0 = (pathname)WITH_VAR_STACK(objscheme_unbundle_pathname(p[POFFSET+0], "on-drop-file in frame%"));

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->wxFrame::OnDropFile(x0));
  else
    WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->OnDropFile(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFramePreOnEvent(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  Bool r;
  objscheme_check_valid(os_wxFrame_class, "pre-on-event in frame%", n, p);
  class wxWindow* x0 INIT_NULLED_OUT;
  class wxMouseEvent* x1 INIT_NULLED_OUT;

  SETUP_VAR_STACK_REMEMBERED(3);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, x0);
  VAR_STACK_PUSH(2, x1);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxWindow(p[POFFSET+0], "pre-on-event in frame%", 0));
  x1 = WITH_VAR_STACK(objscheme_unbundle_wxMouseEvent(p[POFFSET+1], "pre-on-event in frame%", 0));

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    r = WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)-> wxWindow::PreOnEvent(x0, x1));
  else
    r = WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->PreOnEvent(x0, x1));

  
  
  return (r ? scheme_true : scheme_false);
}

static Scheme_Object *os_wxFramePreOnChar(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  Bool r;
  objscheme_check_valid(os_wxFrame_class, "pre-on-char in frame%", n, p);
  class wxWindow* x0 INIT_NULLED_OUT;
  class wxKeyEvent* x1 INIT_NULLED_OUT;

  SETUP_VAR_STACK_REMEMBERED(3);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, x0);
  VAR_STACK_PUSH(2, x1);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxWindow(p[POFFSET+0], "pre-on-char in frame%", 0));
  x1 = WITH_VAR_STACK(objscheme_unbundle_wxKeyEvent(p[POFFSET+1], "pre-on-char in frame%", 0));

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    r = WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)-> wxWindow::PreOnChar(x0, x1));
  else
    r = WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->PreOnChar(x0, x1));

  
  
  return (r ? scheme_true : scheme_false);
}

static Scheme_Object *os_wxFrameOnSize(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "on-size in frame%", n, p);
  int x0;
  int x1;

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_integer(p[POFFSET+0], "on-size in frame%"));
  x1 = WITH_VAR_STACK(objscheme_unbundle_integer(p[POFFSET+1], "on-size in frame%"));

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->wxFrame::OnSize(x0, x1));
  else
    WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->OnSize(x0, x1));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameOnSetFocus(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "on-set-focus in frame%", n, p);

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->wxFrame::OnSetFocus());
  else
    WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->OnSetFocus());

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameOnKillFocus(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "on-kill-focus in frame%", n, p);

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->wxFrame::OnKillFocus());
  else
    WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->OnKillFocus());

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameOnMenuClick(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "on-menu-click in frame%", n, p);

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->wxFrame::OnMenuClick());
  else
    WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->OnMenuClick());

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameOnMenuCommand(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "on-menu-command in frame%", n, p);
  ExactLong x0;

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_ExactLong(p[POFFSET+0], "on-menu-command in frame%"));

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->wxFrame::OnMenuCommand(x0));
  else
    WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->OnMenuCommand(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameOnClose(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  Bool r;
  objscheme_check_valid(os_wxFrame_class, "on-close in frame%", n, p);

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    r = WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->wxFrame::OnClose());
  else
    r = WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->OnClose());

  
  
  return (r ? scheme_true : scheme_false);
}

static Scheme_Object *os_wxFrameOnActivate(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "on-activate in frame%", n, p);
  Bool x0;

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_bool(p[POFFSET+0], "on-activate in frame%"));

  
  if (((Scheme_Class_Object *)p[0])->primflag)
    WITH_VAR_STACK(((os_wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->wxFrame::OnActivate(x0));
  else
    WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->OnActivate(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameframeMenu(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "system-menu in frame%", n, p);

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  

  
  WITH_VAR_STACK(frameMenu(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameCreateStatusLine(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "create-status-line in frame%", n, p);
  int x0;
  string x1 INIT_NULLED_OUT;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, x1);

  
  if (n > (POFFSET+0)) {
    x0 = WITH_VAR_STACK(objscheme_unbundle_integer(p[POFFSET+0], "create-status-line in frame%"));
  } else
    x0 = 1;
  if (n > (POFFSET+1)) {
    x1 = (string)WITH_VAR_STACK(objscheme_unbundle_string(p[POFFSET+1], "create-status-line in frame%"));
  } else
    x1 = "status_line";

  
  WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->CreateStatusLine(x0, x1));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameMaximize(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "maximize in frame%", n, p);
  Bool x0;

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_bool(p[POFFSET+0], "maximize in frame%"));

  
  WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->Maximize(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameStatusLineExists(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  Bool r;
  objscheme_check_valid(os_wxFrame_class, "status-line-exists? in frame%", n, p);

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  

  
  r = WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->StatusLineExists());

  
  
  return (r ? scheme_true : scheme_false);
}

static Scheme_Object *os_wxFrameIconized(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  Bool r;
  objscheme_check_valid(os_wxFrame_class, "iconized? in frame%", n, p);

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  

  
  r = WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->Iconized());

  
  
  return (r ? scheme_true : scheme_false);
}

static Scheme_Object *os_wxFrameSetStatusText(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "set-status-text in frame%", n, p);
  string x0 INIT_NULLED_OUT;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, x0);

  
  x0 = (string)WITH_VAR_STACK(objscheme_unbundle_string(p[POFFSET+0], "set-status-text in frame%"));

  
  WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->SetStatusText(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameGetTheMenuBar(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  class wxMenuBar* r;
  objscheme_check_valid(os_wxFrame_class, "get-menu-bar in frame%", n, p);

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  

  
  r = WITH_VAR_STACK(GetTheMenuBar(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)));

  
  
  return WITH_REMEMBERED_STACK(objscheme_bundle_wxMenuBar(r));
}

static Scheme_Object *os_wxFrameSetMenuBar(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "set-menu-bar in frame%", n, p);
  class wxMenuBar* x0 INIT_NULLED_OUT;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, x0);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxMenuBar(p[POFFSET+0], "set-menu-bar in frame%", 0));

  if (GET_THE_MENU_BAR(((wxFrame *)((Scheme_Class_Object *)THEOBJ)->primdata))) return scheme_void;
  WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->SetMenuBar(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameSetIcon(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "set-icon in frame%", n, p);
  class wxBitmap* x0 INIT_NULLED_OUT;
  class wxBitmap* x1 INIT_NULLED_OUT;
  int x2;

  SETUP_VAR_STACK_REMEMBERED(3);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, x0);
  VAR_STACK_PUSH(2, x1);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxBitmap(p[POFFSET+0], "set-icon in frame%", 0));
  if (n > (POFFSET+1)) {
    x1 = WITH_VAR_STACK(objscheme_unbundle_wxBitmap(p[POFFSET+1], "set-icon in frame%", 1));
  } else
    x1 = NULL;
  if (n > (POFFSET+2)) {
    x2 = WITH_VAR_STACK(unbundle_symset_iconKind(p[POFFSET+2], "set-icon in frame%"));
  } else
    x2 = 0;

  if (x0 && !x0->Ok()) scheme_arg_mismatch(METHODNAME("frame%","set-icon"), "bad bitmap: ", p[POFFSET+0]);if (x1 && !x1->Ok()) scheme_arg_mismatch(METHODNAME("frame%","set-icon"), "bad bitmap: ", p[POFFSET+1]);if (x1 && (x1->GetDepth() != 1)) scheme_arg_mismatch(METHODNAME("frame%","set-icon"), "mask bitmap is not monochrome: ", p[POFFSET+1]);
  WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->SetIcon(x0, x1, x2));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameIconize(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "iconize in frame%", n, p);
  Bool x0;

  SETUP_VAR_STACK_REMEMBERED(1);
  VAR_STACK_PUSH(0, p);

  
  x0 = WITH_VAR_STACK(objscheme_unbundle_bool(p[POFFSET+0], "iconize in frame%"));

  
  WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->Iconize(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrameSetTitle(int n,  Scheme_Object *p[])
{
  WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  REMEMBER_VAR_STACK();
  objscheme_check_valid(os_wxFrame_class, "set-title in frame%", n, p);
  string x0 INIT_NULLED_OUT;

  SETUP_VAR_STACK_REMEMBERED(2);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, x0);

  
  x0 = (string)WITH_VAR_STACK(objscheme_unbundle_string(p[POFFSET+0], "set-title in frame%"));

  
  WITH_VAR_STACK(((wxFrame *)((Scheme_Class_Object *)p[0])->primdata)->SetTitle(x0));

  
  
  return scheme_void;
}

static Scheme_Object *os_wxFrame_ConstructScheme(int n,  Scheme_Object *p[])
{
  SETUP_PRE_VAR_STACK(1);
  PRE_VAR_STACK_PUSH(0, p);
  os_wxFrame *realobj INIT_NULLED_OUT;
  REMEMBER_VAR_STACK();
  class wxFrame* x0 INIT_NULLED_OUT;
  string x1 INIT_NULLED_OUT;
  int x2;
  int x3;
  int x4;
  int x5;
  int x6;
  string x7 INIT_NULLED_OUT;

  SETUP_VAR_STACK_PRE_REMEMBERED(5);
  VAR_STACK_PUSH(0, p);
  VAR_STACK_PUSH(1, realobj);
  VAR_STACK_PUSH(2, x0);
  VAR_STACK_PUSH(3, x1);
  VAR_STACK_PUSH(4, x7);

  
  if ((n < (POFFSET+2)) || (n > (POFFSET+8))) 
    WITH_VAR_STACK(scheme_wrong_count_m("initialization in frame%", POFFSET+2, POFFSET+8, n, p, 1));
  x0 = WITH_VAR_STACK(objscheme_unbundle_wxFrame(p[POFFSET+0], "initialization in frame%", 1));
  x1 = (string)WITH_VAR_STACK(objscheme_unbundle_string(p[POFFSET+1], "initialization in frame%"));
  if (n > (POFFSET+2)) {
    x2 = WITH_VAR_STACK(objscheme_unbundle_integer(p[POFFSET+2], "initialization in frame%"));
  } else
    x2 = -1;
  if (n > (POFFSET+3)) {
    x3 = WITH_VAR_STACK(objscheme_unbundle_integer(p[POFFSET+3], "initialization in frame%"));
  } else
    x3 = -1;
  if (n > (POFFSET+4)) {
    x4 = WITH_VAR_STACK(objscheme_unbundle_integer(p[POFFSET+4], "initialization in frame%"));
  } else
    x4 = -1;
  if (n > (POFFSET+5)) {
    x5 = WITH_VAR_STACK(objscheme_unbundle_integer(p[POFFSET+5], "initialization in frame%"));
  } else
    x5 = -1;
  if (n > (POFFSET+6)) {
    x6 = WITH_VAR_STACK(unbundle_symset_frameStyle(p[POFFSET+6], "initialization in frame%"));
  } else
    x6 = 0;
  if (n > (POFFSET+7)) {
    x7 = (string)WITH_VAR_STACK(objscheme_unbundle_string(p[POFFSET+7], "initialization in frame%"));
  } else
    x7 = "frame";

  WITH_VAR_STACK(wxsCheckEventspace(METHODNAME("frame%","initialization")));if (!x4) x4 = -1;if (!x5) x5 = -1;
  realobj = WITH_VAR_STACK(new os_wxFrame CONSTRUCTOR_ARGS((x0, x1, x2, x3, x4, x5, x6, x7)));
#ifdef MZ_PRECISE_GC
  WITH_VAR_STACK(realobj->gcInit_wxFrame(x0, x1, x2, x3, x4, x5, x6, x7));
#endif
  realobj->__gc_external = (void *)p[0];
  
  
  ((Scheme_Class_Object *)p[0])->primdata = realobj;
  WITH_REMEMBERED_STACK(objscheme_register_primpointer(p[0], &((Scheme_Class_Object *)p[0])->primdata));
  ((Scheme_Class_Object *)p[0])->primflag = 1;
  return scheme_void;
}

void objscheme_setup_wxFrame(Scheme_Env *env)
{
  SETUP_VAR_STACK(1);
  VAR_STACK_PUSH(0, env);

  wxREGGLOB(os_wxFrame_class);

  os_wxFrame_class = WITH_VAR_STACK(objscheme_def_prim_class(env, "frame%", "window%", (Scheme_Method_Prim *)os_wxFrame_ConstructScheme, 21));

  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "on-drop-file" " method", (Scheme_Method_Prim *)os_wxFrameOnDropFile, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "pre-on-event" " method", (Scheme_Method_Prim *)os_wxFramePreOnEvent, 2, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "pre-on-char" " method", (Scheme_Method_Prim *)os_wxFramePreOnChar, 2, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "on-size" " method", (Scheme_Method_Prim *)os_wxFrameOnSize, 2, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "on-set-focus" " method", (Scheme_Method_Prim *)os_wxFrameOnSetFocus, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "on-kill-focus" " method", (Scheme_Method_Prim *)os_wxFrameOnKillFocus, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "on-menu-click" " method", (Scheme_Method_Prim *)os_wxFrameOnMenuClick, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "on-menu-command" " method", (Scheme_Method_Prim *)os_wxFrameOnMenuCommand, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "on-close" " method", (Scheme_Method_Prim *)os_wxFrameOnClose, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "on-activate" " method", (Scheme_Method_Prim *)os_wxFrameOnActivate, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "system-menu" " method", (Scheme_Method_Prim *)os_wxFrameframeMenu, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "create-status-line" " method", (Scheme_Method_Prim *)os_wxFrameCreateStatusLine, 0, 2));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "maximize" " method", (Scheme_Method_Prim *)os_wxFrameMaximize, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "status-line-exists?" " method", (Scheme_Method_Prim *)os_wxFrameStatusLineExists, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "iconized?" " method", (Scheme_Method_Prim *)os_wxFrameIconized, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "set-status-text" " method", (Scheme_Method_Prim *)os_wxFrameSetStatusText, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "get-menu-bar" " method", (Scheme_Method_Prim *)os_wxFrameGetTheMenuBar, 0, 0));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "set-menu-bar" " method", (Scheme_Method_Prim *)os_wxFrameSetMenuBar, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "set-icon" " method", (Scheme_Method_Prim *)os_wxFrameSetIcon, 1, 3));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "iconize" " method", (Scheme_Method_Prim *)os_wxFrameIconize, 1, 1));
  WITH_VAR_STACK(scheme_add_method_w_arity(os_wxFrame_class, "set-title" " method", (Scheme_Method_Prim *)os_wxFrameSetTitle, 1, 1));


  WITH_VAR_STACK(scheme_made_class(os_wxFrame_class));

  WITH_VAR_STACK(objscheme_install_bundler((Objscheme_Bundler)objscheme_bundle_wxFrame, wxTYPE_FRAME));

}

int objscheme_istype_wxFrame(Scheme_Object *obj, const char *stop, int nullOK)
{
  REMEMBER_VAR_STACK();
  if (nullOK && XC_SCHEME_NULLP(obj)) return 1;
  if (objscheme_is_a(obj,  os_wxFrame_class))
    return 1;
  else {
    if (!stop)
       return 0;
    WITH_REMEMBERED_STACK(scheme_wrong_type(stop, nullOK ? "frame% object or " XC_NULL_STR: "frame% object", -1, 0, &obj));
    return 0;
  }
}

Scheme_Object *objscheme_bundle_wxFrame(class wxFrame *realobj)
{
  Scheme_Class_Object *obj INIT_NULLED_OUT;
  Scheme_Object *sobj INIT_NULLED_OUT;

  if (!realobj) return XC_SCHEME_NULL;

  if (realobj->__gc_external)
    return (Scheme_Object *)realobj->__gc_external;

  SETUP_VAR_STACK(2);
  VAR_STACK_PUSH(0, obj);
  VAR_STACK_PUSH(1, realobj);

  if ((realobj->__type != wxTYPE_FRAME) && (sobj = WITH_VAR_STACK(objscheme_bundle_by_type(realobj, realobj->__type))))
    return sobj;
  obj = (Scheme_Class_Object *)WITH_VAR_STACK(scheme_make_uninited_object(os_wxFrame_class));

  obj->primdata = realobj;
  WITH_VAR_STACK(objscheme_register_primpointer(obj, &obj->primdata));
  obj->primflag = 0;

  realobj->__gc_external = (void *)obj;
  return (Scheme_Object *)obj;
}

class wxFrame *objscheme_unbundle_wxFrame(Scheme_Object *obj, const char *where, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return NULL;

  REMEMBER_VAR_STACK();

  (void)objscheme_istype_wxFrame(obj, where, nullOK);
  Scheme_Class_Object *o = (Scheme_Class_Object *)obj;
  WITH_REMEMBERED_STACK(objscheme_check_valid(NULL, NULL, 0, &obj));
  if (o->primflag)
    return (os_wxFrame *)o->primdata;
  else
    return (wxFrame *)o->primdata;
}

