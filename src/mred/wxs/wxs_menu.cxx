/* DO NOT EDIT THIS FILE. */
/* This file was generated by xctocc from "wxs_menu.xc". */


#if defined(_MSC_VER)
# include "wx.h"
#endif

#include "wx_menu.h"

#include "wxscheme.h"
#include "wxs_menu.h"
#include "wxscomon.h"






// @CLASSBASE wxMenuItem "wx:menu-item" : "wx:object"
// @END

// wxMenu is really derived from wxItem
//  but that makes no sense. Enforce different hierarchy here


#define CB_FUNCTYPE wxFunction 


#undef CALLBACKCLASS
#undef CB_REALCLASS
#undef CB_UNBUNDLE
#undef CB_USER

#define CALLBACKCLASS os_wxMenu
#define CB_REALCLASS wxMenu
#define CB_UNBUNDLE objscheme_unbundle_wxMenu
#define CB_USER "wx:menu%::initialization"

#undef CB_TOSCHEME
#undef CB_TOC
#define CB_TOSCHEME wxMenuCallbackToScheme
#define CB_TOC wxMenuCallbackToC


class CALLBACKCLASS;





extern wxCommandEvent *objscheme_unbundle_wxCommandEvent(Scheme_Object *,const char *,int);
extern Scheme_Object *objscheme_bundle_wxCommandEvent(wxCommandEvent *);

static void CB_TOSCHEME(CB_REALCLASS *obj, wxCommandEvent &event);




// @ "find-item-for-id" : wxMenuItem^ FindItemForId(int);


class os_wxMenu : public wxMenu {
 public:
  Scheme_Object *callback_closure;

  os_wxMenu(Scheme_Object * obj, nstring x0 = NULL, wxFunction x1 = NULL);
  ~os_wxMenu();
};

Scheme_Object *os_wxMenu_class;

os_wxMenu::os_wxMenu(Scheme_Object * o, nstring x0, wxFunction x1)
: wxMenu(x0, x1)
{
  __gc_external = (void *)o;
  objscheme_backpointer(&__gc_external);
  objscheme_note_creation(o);
}

os_wxMenu::~os_wxMenu()
{
    objscheme_destroy(this, (Scheme_Object *)__gc_external);
}

#pragma argsused
static Scheme_Object *os_wxMenuSetTitle(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  string x0;

  
  x0 = (string)objscheme_unbundle_string(p[0], "wx:menu%::set-title");

  
  ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->SetTitle(x0);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuSetLabel(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  string x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu%::set-label");
  x1 = (string)objscheme_unbundle_string(p[1], "wx:menu%::set-label");

  if (x0 < 0) return scheme_void;
  ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->SetLabel(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuSetHelpString(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  nstring x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu%::set-help-string");
  x1 = (nstring)objscheme_unbundle_nullable_string(p[1], "wx:menu%::set-help-string");

  if (x0 < 0) return scheme_void;
  ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->SetHelpString(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuGetTitle(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  nstring r;
  objscheme_check_valid(obj);

  

  
  r = ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->GetTitle();

  
  
  return objscheme_bundle_string((char *)r);
}

#pragma argsused
static Scheme_Object *os_wxMenuGetLabel(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  nstring r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu%::get-label");

  if (x0 < 0) return XC_SCHEME_NULL;
  r = ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->GetLabel(x0);

  
  
  return objscheme_bundle_string((char *)r);
}

#pragma argsused
static Scheme_Object *os_wxMenuGetHelpString(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  nstring r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu%::get-help-string");

  if (x0 < 0) return XC_SCHEME_NULL;
  r = ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->GetHelpString(x0);

  
  
  return objscheme_bundle_string((char *)r);
}

#pragma argsused
static Scheme_Object *os_wxMenuFindItem(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);
  string x0;

  
  x0 = (string)objscheme_unbundle_string(p[0], "wx:menu%::find-item");

  
  r = ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->FindItem(x0);

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxMenuNumber(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);

  

  
  r = ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->Number();

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxMenuEnable(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  Bool x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu%::enable");
  x1 = objscheme_unbundle_bool(p[1], "wx:menu%::enable");

  if (x0 < 0) return scheme_void;
  ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->Enable(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuCheck(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  Bool x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu%::check");
  x1 = objscheme_unbundle_bool(p[1], "wx:menu%::check");

  if (x0 < 0) return scheme_void;
  ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->Check(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuChecked(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  Bool r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu%::checked?");

  if (x0 < 0) return scheme_false;
  r = ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->Checked(x0);

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxMenuAppendSeparator(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);

  

  
  ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->AppendSeparator();

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuDeleteByPosition(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  Bool r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu%::delete-by-position");

  if (x0 < 0) return scheme_false;
  r = ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->DeleteByPosition(x0);

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxMenuDelete(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  Bool r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu%::delete");

  if (x0 < 0) return scheme_false;
  r = ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->Delete(x0);

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxMenuAppend(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  if ((n >= 3) && objscheme_istype_number(p[0], NULL) && objscheme_istype_string(p[1], NULL) && objscheme_istype_wxMenu(p[2], NULL, 0)) {
    int x0;
    string x1;
    class wxMenu* x2;
    nstring x3;

    
    if ((n < 3) ||(n > 4)) 
      scheme_wrong_count("wx:menu%::append (submenu case)", 3, 4, n, p);
    x0 = objscheme_unbundle_integer(p[0], "wx:menu%::append (submenu case)");
    x1 = (string)objscheme_unbundle_string(p[1], "wx:menu%::append (submenu case)");
    x2 = objscheme_unbundle_wxMenu(p[2], "wx:menu%::append (submenu case)", 0);
    if (n > 3) {
      x3 = (nstring)objscheme_unbundle_nullable_string(p[3], "wx:menu%::append (submenu case)");
    } else
      x3 = NULL;

    if (x0 < 0) return scheme_void;
    ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->Append(x0, x1, x2, x3);

    
    
  } else  {
    int x0;
    string x1;
    nstring x2;
    Bool x3;

    
    if ((n < 2) ||(n > 4)) 
      scheme_wrong_count("wx:menu%::append (string item case)", 2, 4, n, p);
    x0 = objscheme_unbundle_integer(p[0], "wx:menu%::append (string item case)");
    x1 = (string)objscheme_unbundle_string(p[1], "wx:menu%::append (string item case)");
    if (n > 2) {
      x2 = (nstring)objscheme_unbundle_nullable_string(p[2], "wx:menu%::append (string item case)");
    } else
      x2 = NULL;
    if (n > 3) {
      x3 = objscheme_unbundle_bool(p[3], "wx:menu%::append (string item case)");
    } else
      x3 = FALSE;

    if (x0 < 0) return scheme_void;
    ((wxMenu *)((Scheme_Class_Object *)obj)->primdata)->Append(x0, x1, x2, x3);

    
    
  }

  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenu_ConstructScheme(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  os_wxMenu *realobj;
  nstring x0;
  wxFunction x1;

  Scheme_Object *tmp_callback = NULL;
  if ((n > 2)) 
    scheme_wrong_count("wx:menu%::initialization", 0, 2, n, p);
  if (n > 0) {
    x0 = (nstring)objscheme_unbundle_nullable_string(p[0], "wx:menu%::initialization");
  } else
    x0 = NULL;
  if (n > 1) {
    x1 = (SCHEME_NULLP(p[1]) ? NULL : (WXGC_IGNORE(tmp_callback), objscheme_istype_proc2(p[1], CB_USER), tmp_callback = p[1], (CB_FUNCTYPE)CB_TOSCHEME));
  } else
    x1 = NULL;

  
  realobj = new os_wxMenu(obj, x0, x1);
  
  realobj->callback_closure = tmp_callback; objscheme_backpointer(&realobj->callback_closure);
  ((Scheme_Class_Object *)obj)->primdata = realobj;
  objscheme_register_primpointer(&((Scheme_Class_Object *)obj)->primdata);
  ((Scheme_Class_Object *)obj)->primflag = 1;
  return obj;
}

static Scheme_Object *objscheme_classname_os_wxMenu(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(obj);
  if (n) scheme_wrong_count("wx:menu%" "::get-class-name", 0, 0, n, p);
  return scheme_intern_symbol("wx:menu%");
}

void objscheme_setup_wxMenu(void *env)
{
if (os_wxMenu_class) {
    objscheme_add_global_class(os_wxMenu_class,  "wx:menu%", env);
} else {
  os_wxMenu_class = objscheme_def_prim_class(env, "wx:menu%", "wx:object%", os_wxMenu_ConstructScheme, 16);

  scheme_add_method_w_arity(os_wxMenu_class,"get-class-name",objscheme_classname_os_wxMenu, 0, 0);

 scheme_add_method_w_arity(os_wxMenu_class, "set-title", os_wxMenuSetTitle, 1, 1);
 scheme_add_method_w_arity(os_wxMenu_class, "set-label", os_wxMenuSetLabel, 2, 2);
 scheme_add_method_w_arity(os_wxMenu_class, "set-help-string", os_wxMenuSetHelpString, 2, 2);
 scheme_add_method_w_arity(os_wxMenu_class, "get-title", os_wxMenuGetTitle, 0, 0);
 scheme_add_method_w_arity(os_wxMenu_class, "get-label", os_wxMenuGetLabel, 1, 1);
 scheme_add_method_w_arity(os_wxMenu_class, "get-help-string", os_wxMenuGetHelpString, 1, 1);
 scheme_add_method_w_arity(os_wxMenu_class, "find-item", os_wxMenuFindItem, 1, 1);
 scheme_add_method_w_arity(os_wxMenu_class, "number", os_wxMenuNumber, 0, 0);
 scheme_add_method_w_arity(os_wxMenu_class, "enable", os_wxMenuEnable, 2, 2);
 scheme_add_method_w_arity(os_wxMenu_class, "check", os_wxMenuCheck, 2, 2);
 scheme_add_method_w_arity(os_wxMenu_class, "checked?", os_wxMenuChecked, 1, 1);
 scheme_add_method_w_arity(os_wxMenu_class, "append-separator", os_wxMenuAppendSeparator, 0, 0);
 scheme_add_method_w_arity(os_wxMenu_class, "delete-by-position", os_wxMenuDeleteByPosition, 1, 1);
 scheme_add_method_w_arity(os_wxMenu_class, "delete", os_wxMenuDelete, 1, 1);
 scheme_add_method(os_wxMenu_class, "append", os_wxMenuAppend);


  scheme_made_class(os_wxMenu_class);


}
}

int objscheme_istype_wxMenu(Scheme_Object *obj, const char *stop, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return 1;
  if (SAME_TYPE(SCHEME_TYPE(obj), scheme_object_type)
      && scheme_is_subclass(((Scheme_Class_Object *)obj)->sclass,          os_wxMenu_class))
    return 1;
  else {
    if (!stop)
       return 0;
    scheme_wrong_type(stop, nullOK ? "wx:menu% object or " XC_NULL_STR: "wx:menu% object", -1, 0, &obj);
    return 0;
  }
}

Scheme_Object *objscheme_bundle_wxMenu(class wxMenu *realobj)
{
  Scheme_Class_Object *obj;
  Scheme_Object *sobj;

  if (!realobj) return XC_SCHEME_NULL;

  if (realobj->__gc_external)
    return (Scheme_Object *)realobj->__gc_external;
  if ((sobj = objscheme_bundle_by_type(realobj, realobj->__type)))
    return sobj;
  obj = (Scheme_Class_Object *)scheme_make_uninited_object(os_wxMenu_class);

  obj->primdata = realobj;
  objscheme_register_primpointer(&obj->primdata);
  obj->primflag = 0;

  realobj->__gc_external = (void *)obj;
  objscheme_backpointer(&realobj->__gc_external);
  return (Scheme_Object *)obj;
}

class wxMenu *objscheme_unbundle_wxMenu(Scheme_Object *obj, const char *where, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return NULL;

  (void)objscheme_istype_wxMenu(obj, where, nullOK);
  Scheme_Class_Object *o = (Scheme_Class_Object *)obj;
  objscheme_check_valid(obj);
  if (o->primflag)
    return (os_wxMenu *)o->primdata;
  else
    return (wxMenu *)o->primdata;
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

// wxMenuBar is really derived from wxItem
//  but that makes no sense. Enforce different hierarchy here

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
     scheme_signal_error("%s: expected a proper list", who);

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
#define l_OKTEST 
#define l_INTTYPE int

#define l_TYPE wxMenu
#define l_LIST_ITEM_BUNDLE objscheme_bundle_wxMenu
#define l_LIST_ITEM_UNBUNDLE objscheme_unbundle_wxMenu
#define l_MAKE_LIST __MakewxMenuList
#define l_MAKE_ARRAY __MakewxMenuArray





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
     scheme_signal_error("%s: expected a proper list", who);

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






// @ "find-item-for-id" : wxMenuItem^ FindItemForId(int);


// @ "set-title" : void SetTitle(string);

class os_wxMenuBar : public wxMenuBar {
 public:

  os_wxMenuBar(Scheme_Object * obj);
  os_wxMenuBar(Scheme_Object * obj, int x0, wxMenu** x1, string* x2);
  ~os_wxMenuBar();
};

Scheme_Object *os_wxMenuBar_class;

os_wxMenuBar::os_wxMenuBar(Scheme_Object * o)
: wxMenuBar()
{
  __gc_external = (void *)o;
  objscheme_backpointer(&__gc_external);
  objscheme_note_creation(o);
}

os_wxMenuBar::os_wxMenuBar(Scheme_Object * o, int x0, wxMenu** x1, string* x2)
: wxMenuBar(x0, x1, x2)
{
  __gc_external = (void *)o;
  objscheme_backpointer(&__gc_external);
  objscheme_note_creation(o);
}

os_wxMenuBar::~os_wxMenuBar()
{
    objscheme_destroy(this, (Scheme_Object *)__gc_external);
}

#pragma argsused
static Scheme_Object *os_wxMenuBarSetLabelTop(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  string x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::set-label-top");
  x1 = (string)objscheme_unbundle_string(p[1], "wx:menu-bar%::set-label-top");

  if (x0 < 0) return scheme_void;
  ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->SetLabelTop(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuBarSetLabel(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  string x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::set-label");
  x1 = (string)objscheme_unbundle_string(p[1], "wx:menu-bar%::set-label");

  if (x0 < 0) return scheme_void;
  ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->SetLabel(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuBarSetHelpString(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  nstring x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::set-help-string");
  x1 = (nstring)objscheme_unbundle_nullable_string(p[1], "wx:menu-bar%::set-help-string");

  if (x0 < 0) return scheme_void;
  ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->SetHelpString(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuBarGetTitle(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  nstring r;
  objscheme_check_valid(obj);

  

  
  r = ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->GetTitle();

  
  
  return objscheme_bundle_string((char *)r);
}

#pragma argsused
static Scheme_Object *os_wxMenuBarGetLabelTop(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  nstring r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::get-label-top");

  if (x0 < 0) return XC_SCHEME_NULL;
  r = ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->GetLabelTop(x0);

  
  
  return objscheme_bundle_string((char *)r);
}

#pragma argsused
static Scheme_Object *os_wxMenuBarGetLabel(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  nstring r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::get-label");

  if (x0 < 0) return XC_SCHEME_NULL;
  r = ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->GetLabel(x0);

  
  
  return objscheme_bundle_string((char *)r);
}

#pragma argsused
static Scheme_Object *os_wxMenuBarGetHelpString(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  nstring r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::get-help-string");

  if (x0 < 0) return XC_SCHEME_NULL;
  r = ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->GetHelpString(x0);

  
  
  return objscheme_bundle_string((char *)r);
}

#pragma argsused
static Scheme_Object *os_wxMenuBarFindMenuItem(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);
  string x0;
  string x1;

  
  x0 = (string)objscheme_unbundle_string(p[0], "wx:menu-bar%::find-menu-item");
  x1 = (string)objscheme_unbundle_string(p[1], "wx:menu-bar%::find-menu-item");

  
  r = ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->FindMenuItem(x0, x1);

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxMenuBarNumber(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);

  

  
  r = ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->Number();

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxMenuBarEnableTop(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  Bool x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::enable-top");
  x1 = objscheme_unbundle_bool(p[1], "wx:menu-bar%::enable-top");

  if (x0 < 0) return scheme_void;
  ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->EnableTop(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuBarEnable(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  Bool x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::enable");
  x1 = objscheme_unbundle_bool(p[1], "wx:menu-bar%::enable");

  if (x0 < 0) return scheme_void;
  ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->Enable(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuBarChecked(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  Bool r;
  objscheme_check_valid(obj);
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::checked?");

  if (x0 < 0) return scheme_false;
  r = ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->Checked(x0);

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxMenuBarCheck(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  int x0;
  Bool x1;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-bar%::check");
  x1 = objscheme_unbundle_bool(p[1], "wx:menu-bar%::check");

  if (x0 < 0) return scheme_void;
  ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->Check(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuBarDelete(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  Bool r;
  objscheme_check_valid(obj);
  class wxMenu* x0;
  int x1;

  
  x0 = objscheme_unbundle_wxMenu(p[0], "wx:menu-bar%::delete", 1);
  if (n > 1) {
    x1 = objscheme_unbundle_integer(p[1], "wx:menu-bar%::delete");
  } else
    x1 = 0;

  
  r = ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->Delete(x0, x1);

  
  
  return (r ? scheme_true : scheme_false);
}

#pragma argsused
static Scheme_Object *os_wxMenuBarAppend(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  objscheme_check_valid(obj);
  class wxMenu* x0;
  string x1;

  
  x0 = objscheme_unbundle_wxMenu(p[0], "wx:menu-bar%::append", 0);
  x1 = (string)objscheme_unbundle_string(p[1], "wx:menu-bar%::append");

  
  ((wxMenuBar *)((Scheme_Class_Object *)obj)->primdata)->Append(x0, x1);

  
  
  return scheme_void;
}

#pragma argsused
static Scheme_Object *os_wxMenuBar_ConstructScheme(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  os_wxMenuBar *realobj;
  if ((n >= 1) && SCHEME_LISTP(p[0])) {
    int x0;
    wxMenu** x1;
    string* x2;

    if (scheme_proper_list_length(p[0]) != scheme_proper_list_length(p[1])) scheme_signal_error("wx:menu-bar%%::initialization: list size mismatch");
    if (n != 2) 
      scheme_wrong_count("wx:menu-bar%::initialization (wx:menu% list case)", 2, 2, n, p);
    x1 = NULL;
    x2 = NULL;

    x1 = __MakewxMenuArray((0 < n) ? p[0] : scheme_null, &x0, "wx:menu-bar%::initialization");x2 = __MakestringArray((1 < n) ? p[1] : scheme_null, &x0, "wx:menu-bar%::initialization");
    realobj = new os_wxMenuBar(obj, x0, x1, x2);
    
    
  } else  {

    
    if (n != 0) 
      scheme_wrong_count("wx:menu-bar%::initialization (no argument case)", 0, 0, n, p);

    
    realobj = new os_wxMenuBar(obj);
    
    
  }

  ((Scheme_Class_Object *)obj)->primdata = realobj;
  objscheme_register_primpointer(&((Scheme_Class_Object *)obj)->primdata);
  ((Scheme_Class_Object *)obj)->primflag = 1;
  return obj;
}

static Scheme_Object *objscheme_classname_os_wxMenuBar(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(obj);
  if (n) scheme_wrong_count("wx:menu-bar%" "::get-class-name", 0, 0, n, p);
  return scheme_intern_symbol("wx:menu-bar%");
}

void objscheme_setup_wxMenuBar(void *env)
{
if (os_wxMenuBar_class) {
    objscheme_add_global_class(os_wxMenuBar_class,  "wx:menu-bar%", env);
} else {
  os_wxMenuBar_class = objscheme_def_prim_class(env, "wx:menu-bar%", "wx:object%", os_wxMenuBar_ConstructScheme, 16);

  scheme_add_method_w_arity(os_wxMenuBar_class,"get-class-name",objscheme_classname_os_wxMenuBar, 0, 0);

 scheme_add_method_w_arity(os_wxMenuBar_class, "set-label-top", os_wxMenuBarSetLabelTop, 2, 2);
 scheme_add_method_w_arity(os_wxMenuBar_class, "set-label", os_wxMenuBarSetLabel, 2, 2);
 scheme_add_method_w_arity(os_wxMenuBar_class, "set-help-string", os_wxMenuBarSetHelpString, 2, 2);
 scheme_add_method_w_arity(os_wxMenuBar_class, "get-title", os_wxMenuBarGetTitle, 0, 0);
 scheme_add_method_w_arity(os_wxMenuBar_class, "get-label-top", os_wxMenuBarGetLabelTop, 1, 1);
 scheme_add_method_w_arity(os_wxMenuBar_class, "get-label", os_wxMenuBarGetLabel, 1, 1);
 scheme_add_method_w_arity(os_wxMenuBar_class, "get-help-string", os_wxMenuBarGetHelpString, 1, 1);
 scheme_add_method_w_arity(os_wxMenuBar_class, "find-menu-item", os_wxMenuBarFindMenuItem, 2, 2);
 scheme_add_method_w_arity(os_wxMenuBar_class, "number", os_wxMenuBarNumber, 0, 0);
 scheme_add_method_w_arity(os_wxMenuBar_class, "enable-top", os_wxMenuBarEnableTop, 2, 2);
 scheme_add_method_w_arity(os_wxMenuBar_class, "enable", os_wxMenuBarEnable, 2, 2);
 scheme_add_method_w_arity(os_wxMenuBar_class, "checked?", os_wxMenuBarChecked, 1, 1);
 scheme_add_method_w_arity(os_wxMenuBar_class, "check", os_wxMenuBarCheck, 2, 2);
 scheme_add_method_w_arity(os_wxMenuBar_class, "delete", os_wxMenuBarDelete, 1, 2);
 scheme_add_method_w_arity(os_wxMenuBar_class, "append", os_wxMenuBarAppend, 2, 2);


  scheme_made_class(os_wxMenuBar_class);


}
}

int objscheme_istype_wxMenuBar(Scheme_Object *obj, const char *stop, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return 1;
  if (SAME_TYPE(SCHEME_TYPE(obj), scheme_object_type)
      && scheme_is_subclass(((Scheme_Class_Object *)obj)->sclass,          os_wxMenuBar_class))
    return 1;
  else {
    if (!stop)
       return 0;
    scheme_wrong_type(stop, nullOK ? "wx:menu-bar% object or " XC_NULL_STR: "wx:menu-bar% object", -1, 0, &obj);
    return 0;
  }
}

Scheme_Object *objscheme_bundle_wxMenuBar(class wxMenuBar *realobj)
{
  Scheme_Class_Object *obj;
  Scheme_Object *sobj;

  if (!realobj) return XC_SCHEME_NULL;

  if (realobj->__gc_external)
    return (Scheme_Object *)realobj->__gc_external;
  if ((sobj = objscheme_bundle_by_type(realobj, realobj->__type)))
    return sobj;
  obj = (Scheme_Class_Object *)scheme_make_uninited_object(os_wxMenuBar_class);

  obj->primdata = realobj;
  objscheme_register_primpointer(&obj->primdata);
  obj->primflag = 0;

  realobj->__gc_external = (void *)obj;
  objscheme_backpointer(&realobj->__gc_external);
  return (Scheme_Object *)obj;
}

class wxMenuBar *objscheme_unbundle_wxMenuBar(Scheme_Object *obj, const char *where, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return NULL;

  (void)objscheme_istype_wxMenuBar(obj, where, nullOK);
  Scheme_Class_Object *o = (Scheme_Class_Object *)obj;
  objscheme_check_valid(obj);
  if (o->primflag)
    return (os_wxMenuBar *)o->primdata;
  else
    return (wxMenuBar *)o->primdata;
}



class wxsMenuItem : public wxObject
{
public:
  wxsMenuItem(void) {
  }

  int Id(void) {
    return (int)this;
  }
};

wxsMenuItem* wxsIdToMenuItem(int id)
{
  return (wxsMenuItem *)id;
}




class os_wxsMenuItem : public wxsMenuItem {
 public:

  os_wxsMenuItem(Scheme_Object * obj);
  ~os_wxsMenuItem();
};

Scheme_Object *os_wxsMenuItem_class;

os_wxsMenuItem::os_wxsMenuItem(Scheme_Object * o)
: wxsMenuItem()
{
  __gc_external = (void *)o;
  objscheme_backpointer(&__gc_external);
  objscheme_note_creation(o);
}

os_wxsMenuItem::~os_wxsMenuItem()
{
    objscheme_destroy(this, (Scheme_Object *)__gc_external);
}

#pragma argsused
static Scheme_Object *os_wxsMenuItemId(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  int r;
  objscheme_check_valid(obj);

  

  
  r = ((wxsMenuItem *)((Scheme_Class_Object *)obj)->primdata)->Id();

  
  
  return scheme_make_integer(r);
}

#pragma argsused
static Scheme_Object *os_wxsMenuItem_ConstructScheme(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
  os_wxsMenuItem *realobj;

  
  if (n != 0) 
    scheme_wrong_count("wx:menu-item%::initialization", 0, 0, n, p);

  
  realobj = new os_wxsMenuItem(obj);
  
  
  ((Scheme_Class_Object *)obj)->primdata = realobj;
  objscheme_register_primpointer(&((Scheme_Class_Object *)obj)->primdata);
  ((Scheme_Class_Object *)obj)->primflag = 1;
  return obj;
}

static Scheme_Object *objscheme_classname_os_wxsMenuItem(Scheme_Object *obj, int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(obj);
  if (n) scheme_wrong_count("wx:menu-item%" "::get-class-name", 0, 0, n, p);
  return scheme_intern_symbol("wx:menu-item%");
}

void objscheme_setup_wxsMenuItem(void *env)
{
if (os_wxsMenuItem_class) {
    objscheme_add_global_class(os_wxsMenuItem_class,  "wx:menu-item%", env);
} else {
  os_wxsMenuItem_class = objscheme_def_prim_class(env, "wx:menu-item%", "wx:object%", os_wxsMenuItem_ConstructScheme, 2);

  scheme_add_method_w_arity(os_wxsMenuItem_class,"get-class-name",objscheme_classname_os_wxsMenuItem, 0, 0);

 scheme_add_method_w_arity(os_wxsMenuItem_class, "id", os_wxsMenuItemId, 0, 0);


  scheme_made_class(os_wxsMenuItem_class);


}
}

int objscheme_istype_wxsMenuItem(Scheme_Object *obj, const char *stop, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return 1;
  if (SAME_TYPE(SCHEME_TYPE(obj), scheme_object_type)
      && scheme_is_subclass(((Scheme_Class_Object *)obj)->sclass,          os_wxsMenuItem_class))
    return 1;
  else {
    if (!stop)
       return 0;
    scheme_wrong_type(stop, nullOK ? "wx:menu-item% object or " XC_NULL_STR: "wx:menu-item% object", -1, 0, &obj);
    return 0;
  }
}

Scheme_Object *objscheme_bundle_wxsMenuItem(class wxsMenuItem *realobj)
{
  Scheme_Class_Object *obj;
  Scheme_Object *sobj;

  if (!realobj) return XC_SCHEME_NULL;

  if (realobj->__gc_external)
    return (Scheme_Object *)realobj->__gc_external;
  if ((sobj = objscheme_bundle_by_type(realobj, realobj->__type)))
    return sobj;
  obj = (Scheme_Class_Object *)scheme_make_uninited_object(os_wxsMenuItem_class);

  obj->primdata = realobj;
  objscheme_register_primpointer(&obj->primdata);
  obj->primflag = 0;

  realobj->__gc_external = (void *)obj;
  objscheme_backpointer(&realobj->__gc_external);
  return (Scheme_Object *)obj;
}

class wxsMenuItem *objscheme_unbundle_wxsMenuItem(Scheme_Object *obj, const char *where, int nullOK)
{
  if (nullOK && XC_SCHEME_NULLP(obj)) return NULL;

  (void)objscheme_istype_wxsMenuItem(obj, where, nullOK);
  Scheme_Class_Object *o = (Scheme_Class_Object *)obj;
  objscheme_check_valid(obj);
  if (o->primflag)
    return (os_wxsMenuItem *)o->primdata;
  else
    return (wxsMenuItem *)o->primdata;
}





#pragma argsused
static Scheme_Object *wxsMenuItemGlobalwxsIdToMenuItem(int n,  Scheme_Object *p[])
{
 WXS_USE_ARGUMENT(n) WXS_USE_ARGUMENT(p)
  class wxsMenuItem* r;
  int x0;

  
  x0 = objscheme_unbundle_integer(p[0], "wx:menu-item%::wx:id-to-menu-item");

  
  r = wxsIdToMenuItem(x0);

  
  
  return objscheme_bundle_wxsMenuItem(r);
}

void objscheme_setup_wxsMenuItemGlobal(void *env)
{
  scheme_install_xc_global("wx:id-to-menu-item", scheme_make_prim_w_arity(wxsMenuItemGlobalwxsIdToMenuItem, "wx:id-to-menu-item", 1, 1), env);
}

