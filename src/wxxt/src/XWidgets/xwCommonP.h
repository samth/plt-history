/* Generated by wbuild
 * (generator version 3.2)
 */
#ifndef ___XWCOMMONP_H
#define ___XWCOMMONP_H
#include <X11/CompositeP.h>
#include <./xwCommon.h>
_XFUNCPROTOBEGIN
#ifndef max
#define max(a, b) ((a )>(b )?(a ):(b ))
#endif
#ifndef min
#define min(a, b) ((a )<(b )?(a ):(b ))
#endif

typedef void (*compute_inside_Proc)(
#if NeedFunctionPrototypes
Widget,Position *,Position *,int *,int *
#endif
);
#define XtInherit_compute_inside ((compute_inside_Proc) _XtInherit)
typedef Dimension  (*total_frame_width_Proc)(
#if NeedFunctionPrototypes
Widget
#endif
);
#define XtInherit_total_frame_width ((total_frame_width_Proc) _XtInherit)
typedef void (*_expose_Proc)(
#if NeedFunctionPrototypes
Widget,XEvent *,Region 
#endif
);
#define XtInherit__expose ((_expose_Proc) _XtInherit)
typedef void (*highlight_border_Proc)(
#if NeedFunctionPrototypes
Widget
#endif
);
#define XtInherit_highlight_border ((highlight_border_Proc) _XtInherit)
typedef void (*unhighlight_border_Proc)(
#if NeedFunctionPrototypes
Widget
#endif
);
#define XtInherit_unhighlight_border ((unhighlight_border_Proc) _XtInherit)
typedef void (*hilite_callbacks_Proc)(
#if NeedFunctionPrototypes
Widget
#endif
);
#define XtInherit_hilite_callbacks ((hilite_callbacks_Proc) _XtInherit)
typedef Boolean  (*would_accept_focus_Proc)(
#if NeedFunctionPrototypes
Widget
#endif
);
#define XtInherit_would_accept_focus ((would_accept_focus_Proc) _XtInherit)
typedef void (*traverse_Proc)(
#if NeedFunctionPrototypes
Widget,TraversalDirection ,Widget ,Time *
#endif
);
#define XtInherit_traverse ((traverse_Proc) _XtInherit)
typedef Boolean  (*lighter_color_Proc)(
#if NeedFunctionPrototypes
Widget,Pixel ,Pixel *
#endif
);
#define XtInherit_lighter_color ((lighter_color_Proc) _XtInherit)
typedef Boolean  (*darker_color_Proc)(
#if NeedFunctionPrototypes
Widget,Pixel ,Pixel *
#endif
);
#define XtInherit_darker_color ((darker_color_Proc) _XtInherit)
typedef Boolean  (*set_color_Proc)(
#if NeedFunctionPrototypes
Widget,Pixel ,Pixel *
#endif
);
#define XtInherit_set_color ((set_color_Proc) _XtInherit)

typedef struct {
/* methods */
compute_inside_Proc compute_inside;
total_frame_width_Proc total_frame_width;
_expose_Proc _expose;
highlight_border_Proc highlight_border;
unhighlight_border_Proc unhighlight_border;
hilite_callbacks_Proc hilite_callbacks;
would_accept_focus_Proc would_accept_focus;
traverse_Proc traverse;
lighter_color_Proc lighter_color;
darker_color_Proc darker_color;
set_color_Proc set_color;
/* class variables */
XtTranslations  traversal_trans;
XtTranslations  traversal_trans_small;
Dimension  travMode;
} XfwfCommonClassPart;

typedef struct _XfwfCommonClassRec {
CoreClassPart core_class;
CompositeClassPart composite_class;
XfwfCommonClassPart xfwfCommon_class;
} XfwfCommonClassRec;

typedef struct {
/* resources */
Boolean  traversalOn;
Boolean  traversalTranslationDone;
Dimension  highlightThickness;
Pixel  highlightColor;
Pixmap  highlightPixmap;
XtCallbackList  nextTop;
XtPointer  userData;
Boolean  useExposeCallback;
XtCallbackList  exposeCallback;
XtCallbackList  focusHiliteChange;
XtCallbackList  onDestroy;
/* private state */
Boolean  traversal_focus;
GC  bordergc;
} XfwfCommonPart;

typedef struct _XfwfCommonRec {
CorePart core;
CompositePart composite;
XfwfCommonPart xfwfCommon;
} XfwfCommonRec;

externalref XfwfCommonClassRec xfwfCommonClassRec;

_XFUNCPROTOEND
#endif /* ___XWCOMMONP_H */
