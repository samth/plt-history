/* DO NOT EDIT THIS FILE. */
/* This file was generated by xctocc from "wxs_rado.xc". */


#if defined(_MSC_VER)
# include "wx.h"
#endif

#include "wx_rbox.h"




#include "wxscheme.h"
#include "wxs_rado.h"
#include "wxscomon.h"


static Scheme_Object *radioboxStyle_wxVERTICAL_sym = NULL;
static Scheme_Object *radioboxStyle_wxHORIZONTAL_sym = NULL;

static void init_symset_radioboxStyle(void) {
  radioboxStyle_wxVERTICAL_sym = scheme_intern_symbol("vertical");
  radioboxStyle_wxHORIZONTAL_sym = scheme_intern_symbol("horizontal");
}

static int unbundle_symset_radioboxStyle(Scheme_Object *v, const char *where) {
  if (!radioboxStyle_wxHORIZONTAL_sym) init_symset_radioboxStyle();
  Scheme_Object *i, *l = v;
  long result = 0;
  while (SCHEME_PAIRP(l)) {
  i = SCHEME_CAR(l);
  if (0) { }
  else if (i == radioboxStyle_wxVERTICAL_sym) { result = result | wxVERTICAL; }
  else if (i == radioboxStyle_wxHORIZONTAL_sym) { result = result | wxHORIZONTAL; }
  else { break; } 
  l = SCHEME_CDR(l);
  }
  if (SCHEME_NULLP(l)) return result;
  if (where) scheme_wrong_type(where, "radioboxStyle symbol list", -1, 0, &v);
  return 0;
}

static int istype_symset_radioboxStyle(Scheme_Object *v, const char *where) {
  if (!radioboxStyle_wxHORIZONTAL_sym) init_symset_radioboxStyle();
  Scheme_Object *i, *l = v;
  long result = 1;
  while (SCHEME_PAIRP(l)) {
  i = SCHEME_CAR(l);
  if (0) { }
  else if (i == radioboxStyle_wxVERTICAL_sym) { ; }
  else if (i == radioboxStyle_wxHORIZONTAL_sym) { ; }
  else { break; } 
  l = SCHEME_CDR(l);
  }
  if (SCHEME_NULLP(l)) return result;
  if (where) scheme_wrong_type(where, "radioboxStyle symbol list", -1, 0, &v);
  return 0;
}

static Scheme_Object *bundle_symset_radioboxStyle(int v) {
  if (!radioboxStyle_wxHORIZONTAL_sym) init_symset_radioboxStyle();
  Scheme_Object *l = scheme_null;
  if (v & wxVERTICAL) l = scheme_make_pair(radioboxStyle_wxVERTICAL_sym, l);
  if (v & wxHORIZONTAL) l = scheme_make_pair(radioboxStyle_wxHORIZONTAL_sym, l);
  return l;
}




#define CB_FUNCTYPE wxFunction 


#undef CALLBACKCLASS
#undef CB_REALCLASS
#undef CB_UNBUNDLE
#undef CB_USER

#define CALLBACKCLASS os_wxRadioBox
#define CB_REALCLASS wxRadioBox
#define CB_UNBUNDLE objscheme_unbundle_wxRadioBox
#define CB_USER METHODNAME("radio-box%", "initialization")

#undef CB_TOSCHEME
#undef CB_TOC
#define CB_TOSCHEME wxRadioBoxCallbackToScheme
#define CB_TOC wxRadioBoxCallbackToC


class CALLBACKCLASS;





extern wxCommandEvent *objscheme_unbundle_wxCommandEvent(Scheme_Object *,const char *,int);
extern Scheme_Object *objscheme_bundle_wxCommandEvent(wxCommandEvent *);

static void CB_TOSCHEME(CB_REALCLASS *obj, wxCommandEvent &event);

#include "wxs_bmap.h"

#undef l_ADDRESS
#undef l_DEREF
#undef l_TEST
#undef l_POINT
#undef l_TYPE
#undef l_LIST_ITEM_BUNDLE
#undef l_LIST_ITEM_UNBUNDLE
#undef l_MAKE_LIST
#undef l_MAKE_ARRAY
#undef l_EXTRA
#undef l_TERMINATE
#undef l_COPY
#undef l_OKTEST
#undef l_INTTYPE

#define l_ADDRESS 
#define l_DEREF 
#define l_NULLOK 0
#define l_TEST 
#define l_POINT 
#define l_EXTRA 0
#define l_TERMINATE 
#define l_COPY l_COPYDEST=l_COPYSRC;
#define l_OKTEST 
#define l_INTTYPE int

#define l_TYPE string
#define l_LIST_ITEM_BUNDLE objscheme_bundle_string
#define l_LIST_ITEM_UNBUNDLE objscheme_unbundle_string
#define l_MAKE_LIST __MakestringList
#define l_MAKE_ARRAY __MakestringArray





static Scheme_Object *l_MAKE_LIST(l_TYPE l_POINT *f, l_INTTYPE c)
{
  Scheme_Object *cdr = scheme_null, *obj;

  while (c--) {
    obj = l_LIST_ITEM_BUNDLE(l_ADDRESS f[c]);
    cdr = scheme_make_pair(obj, cdr);
  }
  
  return cdr;
}

static l_TYPE l_POINT *l_MAKE_ARRAY(Scheme_Object *l, l_INTTYPE *c, char *who)
{
  Scheme_Object *orig_l = l;
  int i = 0;
  long len;

  len = scheme_proper_list_length(l);
  if (len < 0) scheme_wrong_type(who, "proper-list", -1, 0, &l);
  if (c) *c = len;

  if (!(len + l_EXTRA))
    return NULL;

  l_TYPE l_POINT *f = new l_TYPE l_POINT[len + l_EXTRA];

  while (!SCHEME_NULLP(l)) {
    if (!SCHEME_LISTP(l))
     scheme_arg_mismatch(who, "expected a proper list: ", orig_l);

#define l_COPYDEST f[i]
#define l_COPYSRC (l_DEREF l_LIST_ITEM_UNBUNDLE(SCHEME_CAR(l), who l_TEST))

    l_COPY

    l_OKTEST

    i++;

    l = SCHEME_CDR(l);
  }
  l_TERMINATE

  return f;
}


#define OKTESTWHERE METHODNAME("radio-box%", "initialization") 
#undef l_ADDRESS
#undef l_DEREF
#undef l_TEST
#undef l_POINT
#undef l_TYPE
#undef l_LIST_ITEM_BUNDLE
#undef l_LIST_ITEM_UNBUNDLE
#undef l_MAKE_LIST
#undef l_MAKE_ARRAY
#undef l_EXTRA
#undef l_TERMINATE
#undef l_COPY
#undef l_OKTEST
#undef l_INTTYPE

#define l_ADDRESS 
#define l_DEREF 
#define l_NULLOK 0
#define l_TEST , l_NULLOK
#define l_POINT *
#define l_EXTRA 0
#define l_TERMINATE 
#define l_COPY l_COPYDEST=l_COPYSRC;
#define l_OKTEST if (!((l_COPYDEST)->Ok())) scheme_arg_mismatch(OKTESTWHERE, "bad bitmap: ", SCHEME_CAR(l));
#define l_INTTYPE int

#define l_TYPE wxBitmap
#define l_LIST_ITEM_BUNDLE objscheme_bundle_wxBitmap
#define l_LIST_ITEM_UNBUNDLE objscheme_unbundle_wxBitmap
#define l_MAKE_LIST __MakewxBitmapList
#define l_MAKE_ARRAY __MakewxBitmapArray





static Scheme_Object *l_MAKE_LIST(l_TYPE l_POINT *f, l_INTTYPE c)
{
  Scheme_Object *cdr = scheme_null, *obj;

  while (c--) {
    obj = l_LIST_ITEM_BUNDLE(l_ADDRESS f[c]);
    cdr = scheme_make_pair(obj, cdr);
  }
  
  return cdr;
}

static l_TYPE l_POINT *l_MAKE_ARRAY(Scheme_Object *l, l_INTTYPE *c, char *who)
{
  Scheme_Object *orig_l = l;
  int i = 0;
  long len;

  len = scheme_proper_list_length(l);
  if (len < 0) scheme_wrong_type(who, "proper-list", -1, 0, &l);
  if (c) *c = len;

  if (!(len + l_EXTRA))
    return NULL;

  l_TYPE l_POINT *f = new l_TYPE l_POINT[len + l_EXTRA];

  while (!SCHEME_NULLP(l)) {
    if (!SCHEME_LISTP(l))
     scheme_arg_mismatch(who, "expected a proper list: ", orig_l);

#define l_COPYDEST f[i]
#define l_COPYSRC (l_DEREF l_LIST_ITEM_UNBUNDLE(SCHEME_CAR(l), who l_TEST))

    l_COPY

    l_OKTEST

    i++;

    l = SCHEME_CDR(l);
  }
  l_TERMINATE

  return f;
}











#define RANGECLASS wxRadioBox

#define THISOBJECT ((RANGECLASS *)((Scheme_Class_Object *)obj)->primdata)





class os_wxRadioBox : public wxRadioBox {
 public:
  Scheme_Object *callback_closure;

  os_wxRadioBox(Scheme_Object * obj, class wxPanel* x0, wxFunction x1, nstring x2, int x3 = -1, int x4 = -1, int x5 = -1, int x6 = -1, int x7 = 0, string* x8 = NULL, int x9 = 0, int x10 = wxVERTICAL, string x11 = "radioBox");
  os_wxRadioBox(Scheme_Object * obj, class wxPanel* x0, wxFunction x1, nstring x2, int x3, int x4, int x5, int x6, int x7, wxBitmap** x8, int x9 = 0, int x10 = wxVERTICAL, string x11 = "radioBox");
  ~os_wxRadioBox();
  void OnDropFile(pathname x0);
  Bool PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1);
  Bool PreOnChar(class wxWindow* x0, class wxKeyEvent* x1);
  void OnSize(int x0, int x1);
  void OnSetFocus();
  void OnKillFocus();
};

Scheme_Object *os_wxRadioBox_class;

os_wxRadioBox::os_wxRadioBox(Scheme_Object * o, class wxPanel* x0, wxFunction x1, nstring x2, int x3, int x4, int x5, int x6, int x7, string* x8, int x9, int x10, string x11)
: wxRadioBox(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11)
{
  __gc_external = (void *)o;
  objscheme_backpointer(&__gc_external);
  objscheme_note_creation(o);
}

os_wxRadioBox::os_wxRadioBox(Scheme_Object * o, class wxPanel* x0, wxFunction x1, nstring x2, int x3, int x4, int x5, int x6, int x7, wxBitmap** x8, int x9, int x10, string x11)
: wxRadioBox(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11)
{
  __gc_external = (void *)o;
  objscheme_backpointer(&__gc_external);
  objscheme_note_creation(o);
}

os_wxRadioBox::~os_wxRadioBox()
{
    objscheme_destroy(this, (Scheme_Object *)__gc_external);
}

void os_wxRadioBox::OnDropFile(pathname x0)
{
  Scheme_Object *p[1];
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxRadioBox_class, "on-drop-file", &mcache);
  if (method && !OBJSCHEME_PRIM_METHOD(method)) {
    COPY_JMPBUF(savebuf, scheme_error_buf);
    sj = scheme_setjmp(scheme_error_buf);
    if (sj) {
      COPY_JMPBUF(scheme_error_buf, savebuf);
      scheme_clear_escape();
    }
  } else sj = 1;
  if (sj) {
wxRadioBox::OnDropFile(x0);
  } else {
  
  p[0] = objscheme_bundle_pathname((char *)x0);
  

  v = scheme_apply(method, 1, p);
  
  
  COPY_JMPBUF(scheme_error_buf, savebuf);

  }
}

Bool os_wxRadioBox::PreOnEvent(class wxWindow* x0, class wxMouseEvent* x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxRadioBox_class, "pre-on-event", &mcache);
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

  return objscheme_unbundle_bool(v, "pre-on-event in radio-box%"", extracting return value");
  }
}

Bool os_wxRadioBox::PreOnChar(class wxWindow* x0, class wxKeyEvent* x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxRadioBox_class, "pre-on-char", &mcache);
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

  return objscheme_unbundle_bool(v, "pre-on-char in radio-box%"", extracting return value");
  }
}

void os_wxRadioBox::OnSize(int x0, int x1)
{
  Scheme_Object *p[2];
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxRadioBox_class, "on-size", &mcache);
  if (method && !OBJSCHEME_PRIM_METHOD(method)) {
    COPY_JMPBUF(savebuf, scheme_error_buf);
    sj = scheme_setjmp(scheme_error_buf);
    if (sj) {
      COPY_JMPBUF(scheme_error_buf, savebuf);
      scheme_clear_escape();
    }
  } else sj = 1;
  if (sj) {
wxRadioBox::OnSize(x0, x1);
  } else {
  
  p[0] = scheme_make_integer(x0);
  p[1] = scheme_make_integer(x1);
  

  v = scheme_apply(method, 2, p);
  
  
  COPY_JMPBUF(scheme_error_buf, savebuf);

  }
}

void os_wxRadioBox::OnSetFocus()
{
  Scheme_Object **p = NULL;
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxRadioBox_class, "on-set-focus", &mcache);
  if (method && !OBJSCHEME_PRIM_METHOD(method)) {
    COPY_JMPBUF(savebuf, scheme_error_buf);
    sj = scheme_setjmp(scheme_error_buf);
    if (sj) {
      COPY_JMPBUF(scheme_error_buf, savebuf);
      scheme_clear_escape();
    }
  } else sj = 1;
  if (sj) {
wxRadioBox::OnSetFocus();
  } else {
  
  

  v = scheme_apply(method, 0, p);
  
  
  COPY_JMPBUF(scheme_error_buf, savebuf);

  }
}

void os_wxRadioBox::OnKillFocus()
{
  Scheme_Object **p = NULL;
  Scheme_Object *v;
  mz_jmp_buf savebuf;
  Scheme_Object *method;
  int sj;
  static void *mcache = 0;

  method = objscheme_find_method((Scheme_Object *)__gc_external, os_wxRadioBox_class, "on-kill-focus", &mcache);
  if (method && !OBJSCHEME_PRIM_METHOD(method)) {
    COPY_JMPBUF(savebuf, scheme_error_buf);
    sj = scheme_setjmp(scheme_error_buf);
    if (sj) {
      COPY_JMPBUF(scheme_error_buf, savebuf);
      scheme_clear_escape();
    }
  } else sj = 1;
  if (sj) {
wxRadioBox::OnKillFocus();
  } else {
  
  

  v = scheme_apply(method, 0, p);
  
  
  COPY_JMPBUF(scheme_error_buf, savebuf);

  }
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxButtonFocus(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "button-focus in radio-box%");

  
  r = ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->ButtonFocus(x0);

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxEnable(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  if ((n >= 1) && objscheme_istype_number(p[0], NULL)) {
    int x0;
    Bool x1;

    
    if (n != 2) 
      scheme_wrong_count("enable in radio-box% (single-button case)", 2, 2, n, p);
    x0 = objscheme_unbundle_integer(p[0], "enable in radio-box% (single-button case)");
    x1 = objscheme_unbundle_bool(p[1], "enable in radio-box% (single-button case)");

    if ((x0 < 0) || (x0 >= THISOBJECT->Number())) return scheme_void;
    ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->Enable(x0, x1);

    
    
  } else  {
    Bool x0;

    
    if (n != 1) 
      scheme_wrong_count("enable in radio-box% (all-buttons case)", 1, 1, n, p);
    x0 = objscheme_unbundle_bool(p[0], "enable in radio-box% (all-buttons case)");

    
    ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->Enable(x0);

    
    
  }

  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxGetString(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  nstring r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "get-string in radio-box%");

  if ((x0 < 0) || (x0 >= THISOBJECT->Number())) return XC_SCHEME_NULL;
  r = ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->GetString(x0);

  
  
  return objscheme_bundle_string((char *)r);
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxSetSelection(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "set-selection in radio-box%");

  if ((x0 < 0) || (x0 >= THISOBJECT->Number())) return scheme_void;
  ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->SetSelection(x0);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxSetStringSelection(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  string x0;

  
  x0 = (string)objscheme_unbundle_string(p[0], "set-string-selection in radio-box%");

  
  ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->SetStringSelection(x0);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxNumber(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);

  

  
  r = ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->Number();

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxGetStringSelection(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  nstring r;
  objscheme_check_valid(obj);

  

  
  r = ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->GetStringSelection();

  
  
  return objscheme_bundle_string((char *)r);
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxGetSelection(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);

  

  
  r = ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->GetSelection();

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxFindString(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);
  string x0;

  
  x0 = (string)objscheme_unbundle_string(p[0], "find-string in radio-box%");

  
  r = ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->FindString(x0);

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxOnDropFile(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  pathname x0;

  
  x0 = (pathname)objscheme_unbundle_pathname(p[0], "on-drop-file in radio-box%");

  
  if (((Scheme_Class_Object *)obj)->primflag)
    ((os_wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->wxRadioBox::OnDropFile(x0);
  else
    ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->OnDropFile(x0);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxPreOnEvent(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  Bool r;
  objscheme_check_valid(obj);
  class wxWindow* x0;
  class wxMouseEvent* x1;

  
  x0 = objscheme_unbundle_wxWindow(p[0], "pre-on-event in radio-box%", 0);
  x1 = objscheme_unbundle_wxMouseEvent(p[1], "pre-on-event in radio-box%", 0);

  
  if (((Scheme_Class_Object *)obj)->primflag)
    r = ((os_wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)-> wxWindow::PreOnEvent(x0, x1);
  else
    r = ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->PreOnEvent(x0, x1);

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxPreOnChar(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  Bool r;
  objscheme_check_valid(obj);
  class wxWindow* x0;
  class wxKeyEvent* x1;

  
  x0 = objscheme_unbundle_wxWindow(p[0], "pre-on-char in radio-box%", 0);
  x1 = objscheme_unbundle_wxKeyEvent(p[1], "pre-on-char in radio-box%", 0);

  
  if (((Scheme_Class_Object *)obj)->primflag)
    r = ((os_wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)-> wxWindow::PreOnChar(x0, x1);
  else
    r = ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->PreOnChar(x0, x1);

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxOnSize(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  int x1;

  
  x0 = objscheme_unbundle_integer(p[0], "on-size in radio-box%");
  x1 = objscheme_unbundle_integer(p[1], "on-size in radio-box%");

  
  if (((Scheme_Class_Object *)obj)->primflag)
    ((os_wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->wxRadioBox::OnSize(x0, x1);
  else
    ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->OnSize(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxOnSetFocus(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);

  

  
  if (((Scheme_Class_Object *)obj)->primflag)
    ((os_wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->wxRadioBox::OnSetFocus();
  else
    ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->OnSetFocus();

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxRadioBoxOnKillFocus(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);

  

  
  if (((Scheme_Class_Object *)obj)->primflag)
    ((os_wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->wxRadioBox::OnKillFocus();
  else
    ((wxRadioBox *)((Scheme_Class_Object *)obj)->primdata)->OnKillFocus();

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxRadioBox_ConstructScheme(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  os_wxRadioBox *realobj;
  if ((n >= 8) && objscheme_istype_wxPanel(p[0], NULL, 0) && (SCHEME_NULLP(p[1]) || objscheme_istype_proc2(p[1], NULL)) && (XC_SCHEME_NULLP(p[2]) || objscheme_istype_string(p[2], NULL)) && objscheme_istype_number(p[3], NULL) && objscheme_istype_number(p[4], NULL) && objscheme_istype_number(p[5], NULL) && objscheme_istype_number(p[6], NULL) && (SCHEME_LISTP(p[7]) && (XC_SCHEME_NULLP(p[7]) || objscheme_istype_wxBitmap((SCHEME_CAR(p[7])), NULL, 0)))) {
    class wxPanel* x0;
    wxFunction x1;
    nstring x2;
    int x3;
    int x4;
    int x5;
    int x6;
    int x7;
    wxBitmap** x8;
    int x9;
    int x10;
    string x11;

    Scheme_Object *tmp_callback = NULL;
    if ((n < 8) ||(n > 11)) 
      scheme_wrong_count("initialization in radio-box% (bitmap list case)", 8, 11, n, p);
    x0 = objscheme_unbundle_wxPanel(p[0], "initialization in radio-box% (bitmap list case)", 0);
    x1 = (SCHEME_NULLP(p[1]) ? NULL : (WXGC_IGNORE(tmp_callback), objscheme_istype_proc2(p[1], CB_USER), tmp_callback = p[1], (CB_FUNCTYPE)CB_TOSCHEME));
    x2 = (nstring)objscheme_unbundle_nullable_string(p[2], "initialization in radio-box% (bitmap list case)");
    x3 = objscheme_unbundle_integer(p[3], "initialization in radio-box% (bitmap list case)");
    x4 = objscheme_unbundle_integer(p[4], "initialization in radio-box% (bitmap list case)");
    x5 = objscheme_unbundle_integer(p[5], "initialization in radio-box% (bitmap list case)");
    x6 = objscheme_unbundle_integer(p[6], "initialization in radio-box% (bitmap list case)");
    x8 = NULL;
    if (n > 8) {
      x9 = objscheme_unbundle_integer(p[8], "initialization in radio-box% (bitmap list case)");
    } else
      x9 = 0;
    if (n > 9) {
      x10 = unbundle_symset_radioboxStyle(p[9], "initialization in radio-box% (bitmap list case)");
    } else
      x10 = wxVERTICAL;
    if (n > 10) {
      x11 = (string)objscheme_unbundle_string(p[10], "initialization in radio-box% (bitmap list case)");
    } else
      x11 = "radioBox";

    if (!x5) x5 = -1;if (!x6) x6 = -1;x8 = __MakewxBitmapArray((7 < n) ? p[7] : scheme_null, &x7, METHODNAME("radio-box%","initialization"));
    realobj = new os_wxRadioBox(obj, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11);
    delete[] x8;
    realobj->callback_closure = tmp_callback; objscheme_backpointer(&realobj->callback_closure);
  } else  {
    class wxPanel* x0;
    wxFunction x1;
    nstring x2;
    int x3;
    int x4;
    int x5;
    int x6;
    int x7;
    string* x8;
    int x9;
    int x10;
    string x11;

    Scheme_Object *tmp_callback = NULL;
    if ((n < 3) ||(n > 11)) 
      scheme_wrong_count("initialization in radio-box% (string list case)", 3, 11, n, p);
    x0 = objscheme_unbundle_wxPanel(p[0], "initialization in radio-box% (string list case)", 0);
    x1 = (SCHEME_NULLP(p[1]) ? NULL : (WXGC_IGNORE(tmp_callback), objscheme_istype_proc2(p[1], CB_USER), tmp_callback = p[1], (CB_FUNCTYPE)CB_TOSCHEME));
    x2 = (nstring)objscheme_unbundle_nullable_string(p[2], "initialization in radio-box% (string list case)");
    if (n > 3) {
      x3 = objscheme_unbundle_integer(p[3], "initialization in radio-box% (string list case)");
    } else
      x3 = -1;
    if (n > 4) {
      x4 = objscheme_unbundle_integer(p[4], "initialization in radio-box% (string list case)");
    } else
      x4 = -1;
    if (n > 5) {
      x5 = objscheme_unbundle_integer(p[5], "initialization in radio-box% (string list case)");
    } else
      x5 = -1;
    if (n > 6) {
      x6 = objscheme_unbundle_integer(p[6], "initialization in radio-box% (string list case)");
    } else
      x6 = -1;
    if (n > 7) {
      x8 = NULL;
    } else
      x8 = NULL;
    if (n > 8) {
      x9 = objscheme_unbundle_integer(p[8], "initialization in radio-box% (string list case)");
    } else
      x9 = 0;
    if (n > 9) {
      x10 = unbundle_symset_radioboxStyle(p[9], "initialization in radio-box% (string list case)");
    } else
      x10 = wxVERTICAL;
    if (n > 10) {
      x11 = (string)objscheme_unbundle_string(p[10], "initialization in radio-box% (string list case)");
    } else
      x11 = "radioBox";

    if (!x5) x5 = -1;if (!x6) x6 = -1;x8 = __MakestringArray((7 < n) ? p[7] : scheme_null, &x7, METHODNAME("radio-box%","initialization"));
    realobj = new os_wxRadioBox(obj, x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11);
    delete[] x8;
    realobj->callback_closure = tmp_callback; objscheme_backpointer(&realobj->callback_closure);
  }

  ((Scheme_Class_Object *)obj)->primdata = realobj;
  objscheme_register_primpointer(&((Scheme_Class_Object *)obj)->primdata);
  ((Scheme_Class_Object *)obj)->primflag = 1;
  return obj;
}

void objscheme_setup_wxRadioBox(void *env)
{
if (os_wxRadioBox_class) {
    objscheme_add_global_class(os_wxRadioBox_class, "radio-box%", env);
} else {
  os_wxRadioBox_class = objscheme_def_prim_class(env, "radio-box%", "item%", os_wxRadioBox_ConstructScheme, 15);

 scheme_add_method_w_arity(os_wxRadioBox_class, "button-focus", os_wxRadioBoxButtonFocus, 1, 1);
 scheme_add_method_w_arity(os_wxRadioBox_class, "enable", os_wxRadioBoxEnable, 1, 2);
 scheme_add_method_w_arity(os_wxRadioBox_class, "get-string", os_wxRadioBoxGetString, 1, 1);
 scheme_add_method_w_arity(os_wxRadioBox_class, "set-selection", os_wxRadioBoxSetSelection, 1, 1);
 scheme_add_method_w_arity(os_wxRadioBox_class, "set-string-selection", os_wxRadioBoxSetStringSelection, 1, 1);
 scheme_add_method_w_arity(os_wxRadioBox_class, "number", os_wxRadioBoxNumber, 0, 0);
 scheme_add_method_w_arity(os_wxRadioBox_class, "get-string-selection", os_wxRadioBoxGetStringSelection, 0, 0);
 scheme_add_method_w_arity(os_wxRadioBox_class, "get-selection", os_wxRadioBoxGetSelection, 0, 0);
 scheme_add_method_w_arity(os_wxRadioBox_class, "find-string", os_wxRadioBoxFindString, 1, 1);
 scheme_add_method_w_arity(os_wxRadioBox_class, "on-drop-file", os_wxRadioBoxOnDropFile, 1, 1);
 scheme_add_method_w_arity(os_wxRadioBox_class, "pre-on-event", os_wxRadioBoxPreOnEvent, 2, 2);
 scheme_add_method_w_arity(os_wxRadioBox_class, "pre-on-char", os_wxRadioBoxPreOnChar, 2, 2);
 scheme_add_method_w_arity(os_wxRadioBox_class, "on-size", os_wxRadioBoxOnSize, 2, 2);
 scheme_add_method_w_arity(os_wxRadioBox_class, "on-set-focus", os_wxRadioBoxOnSetFocus, 0, 0);
 scheme_add_method_w_arity(os_wxRadioBox_class, "on-kill-focus", os_wxRadioBoxOnKillFocus, 0, 0);


  scheme_made_class(os_wxRadioBox_class);


}
}

int objscheme_istype_wxRadioBox(Scheme_Object *obj, const char *stop, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return 1;
  if (SAME_TYPE(SCHEME_TYPE(obj), scheme_object_type)
      && scheme_is_subclass(((Scheme_Class_Object *)obj)->sclass,          os_wxRadioBox_class))
    return 1;
  else {
    if (!stop)
       return 0;
    scheme_wrong_type(stop, nullOK ? "radio-box% object or " XC_NULL_STR: "radio-box% object", -1, 0, &obj);
    return 0;
  }
}

Scheme_Object *objscheme_bundle_wxRadioBox(class wxRadioBox *realobj)
{
  Scheme_Class_Object *obj;
  Scheme_Object *sobj;

  if (!realobj) return XC_SCHEME_NULL;

  if (realobj->__gc_external)
    return (Scheme_Object *)realobj->__gc_external;
  if ((sobj = objscheme_bundle_by_type(realobj, realobj->__type)))
    return sobj;
  obj = (Scheme_Class_Object *)scheme_make_uninited_object(os_wxRadioBox_class);

  obj->primdata = realobj;
  objscheme_register_primpointer(&obj->primdata);
  obj->primflag = 0;

  realobj->__gc_external = (void *)obj;
  objscheme_backpointer(&realobj->__gc_external);
  return (Scheme_Object *)obj;
}

class wxRadioBox *objscheme_unbundle_wxRadioBox(Scheme_Object *obj, const char *where, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return NULL;

  (void)objscheme_istype_wxRadioBox(obj, where, nullOK);
  Scheme_Class_Object *o = (Scheme_Class_Object *)obj;
  objscheme_check_valid(obj);
  if (o->primflag)
    return (os_wxRadioBox *)o->primdata;
  else
    return (wxRadioBox *)o->primdata;
}



static void CB_TOSCHEME(CB_REALCLASS *realobj, wxCommandEvent &event)
{
  Scheme_Object *p[2];
  Scheme_Class_Object *obj;
  jmp_buf savebuf;

  obj = (Scheme_Class_Object *)realobj->__gc_external;

  if (!obj) {
    // scheme_signal_error("bad callback object");
    return;
  }

  p[0] = (Scheme_Object *)obj;
  p[1] = objscheme_bundle_wxCommandEvent(&event);

  COPY_JMPBUF(savebuf, scheme_error_buf);

  if (!scheme_setjmp(scheme_error_buf)) {
    scheme_apply_multi(((CALLBACKCLASS *)obj->primdata)->callback_closure, 2, p);
  }

  COPY_JMPBUF(scheme_error_buf, savebuf);
}
