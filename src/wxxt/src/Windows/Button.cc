/*								-*- C++ -*-
 *
 * Purpose: button panel item
 *
 * Authors: Markus Holzem and Julian Smart
 *
 * Copyright: (C) 1995, AIAI, University of Edinburgh (Julian)
 * Copyright: (C) 1995, GNU (Markus)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef __GNUG__
#pragma implementation "Button.h"
#endif

#define  Uses_XtIntrinsic
#define  Uses_wxButton
#include "wx.h"
#define  Uses_ButtonWidget
#define  Uses_TraversingEnforcerWidget
#include "widgets.h"

//-----------------------------------------------------------------------------
// create and destroy button
//-----------------------------------------------------------------------------

wxButton::wxButton(wxPanel *panel, wxFunction function, char *label,
		   int x, int y, int width, int height,
		   long style, char *name) : wxItem()
{
    __type = wxTYPE_BUTTON;

    Create(panel, function, label, x, y, width, height, style, name);
}

wxButton::wxButton(wxPanel *panel, wxFunction function, wxBitmap *bitmap,
		   int x, int y, int width, int height,
		   long style, char *name) : wxItem()
{
    __type = wxTYPE_BUTTON;

    Create(panel, function, bitmap, x, y, width, height, style, name);
}

Bool wxButton::Create(wxPanel *panel, wxFunction function, char *label,
		      int x, int y, int width, int height,
		      long style, char *name)
{
    wxWindow_Xintern *ph;
    Widget wgt;

    ChainToPanel(panel, style, name);

    label = wxGetCtlLabel(label);
    bm_label = NULL;

    ph = parent->GetHandle();

    // create frame
    wgt = XtVaCreateManagedWidget
	(name, xfwfTraversingEnforcerWidgetClass, ph->handle,
	 XtNbackground,  bg->GetPixel(cmap),
	 XtNforeground,  label_fg->GetPixel(cmap),
	 XtNfont,        label_font->GetInternalFont(),
	 XtNshrinkToFit, (width < 0 || height < 0),
	 XtNframeWidth,  style ? 2 : 0,
	 XtNframeType,   XfwfSunken,
	 NULL);
    X->frame = wgt;
    // create widget
    wgt = XtVaCreateManagedWidget
	("button", xfwfButtonWidgetClass, X->frame,
	 XtNlabel,       label,
	 XtNbackground,  bg->GetPixel(cmap),
	 XtNforeground,  fg->GetPixel(cmap),
	 XtNfont,        font->GetInternalFont(),
	 XtNshrinkToFit, (width < 0 || height < 0),
	 XtNhighlightThickness, 0,
	 XtNtraversalOn, FALSE,
	 NULL);
    X->handle = wgt;
    // propagate key events from frame to button widget
    XtVaSetValues(X->frame, XtNpropagateTarget, X->handle, NULL);
    // set data declared in wxItem
    callback = function;
    XtAddCallback(X->handle, XtNactivate, wxButton::EventCallback, (XtPointer)saferef);

    panel->PositionItem(this, x, y, width, height);
    AddEventHandlers();
    AllowResize(FALSE);

    return TRUE;
}

Bool wxButton::Create(wxPanel *panel, wxFunction function, wxBitmap *bitmap,
		      int x, int y, int width, int height,
		      long style, char *name)
{
    wxWindow_Xintern *ph;
    Pixmap pm;
    Widget wgt;

    if (!bitmap->Ok() || (bitmap->selectedIntoDC < 0))
      return Create(panel, function, "<bad-image>", x, y, width, height, style, name);

    bitmap->selectedIntoDC++;
    bm_label = bitmap;

    ChainToPanel(panel, style, name);

    ph = parent->GetHandle();

    // create frame
    wgt = XtVaCreateManagedWidget
	(name, xfwfTraversingEnforcerWidgetClass, ph->handle,
	 XtNbackground,  bg->GetPixel(cmap),
	 XtNforeground,  label_fg->GetPixel(cmap),
	 XtNfont,        label_font->GetInternalFont(),
	 XtNshrinkToFit, (width < 0 || height < 0),
	 NULL);
    X->frame = wgt;
    // create widget
    pm = GETPIXMAP(bitmap);
    wgt = XtVaCreateManagedWidget
	("button", xfwfButtonWidgetClass, X->frame,
	 XtNpixmap,      pm,
	 XtNbackground,  bg->GetPixel(cmap),
	 XtNforeground,  fg->GetPixel(cmap),
	 XtNfont,        font->GetInternalFont(),
	 XtNshrinkToFit, (width < 0 || height < 0),
	 XtNhighlightThickness, 0, XtNtraversalOn, FALSE,
	 NULL);
    X->handle = wgt;
    // propagate key events from frame to button widget
    XtVaSetValues(X->frame, XtNpropagateTarget, X->handle, NULL);
    // set data declared in wxItem
    callback = function;
    XtAddCallback(X->handle, XtNactivate, wxButton::EventCallback, (XtPointer)saferef);

    panel->PositionItem(this, x, y, width, height);
    AddEventHandlers();
    AllowResize(FALSE);

    return TRUE;
}

wxButton::~wxButton(void)
{
  if (bm_label) {
    --bm_label->selectedIntoDC;
    XtVaSetValues(X->handle, XtNpixmap, NULL, NULL);
  }
}

//-----------------------------------------------------------------------------
// alternate SetLabel for changing bitmap
//-----------------------------------------------------------------------------

void wxButton::AllowResize(Bool allow)
{
    XtVaSetValues(X->handle, XtNshrinkToFit, allow, NULL);
}

void wxButton::SetAlignment(long alignment)
{
    XtVaSetValues(X->handle, XtNalignment, alignment, NULL);
}

void wxButton::SetDefault(void)
{
    ((wxPanel*)parent)->default_item = this;
}

void wxButton::SetLabel(char *label)
{
  if (!bm_label) {
    label = wxGetCtlLabel(label);

    XtVaSetValues(X->handle, XtNlabel, label, NULL);
  }
}

void wxButton::SetLabel(wxBitmap *bitmap)
{
  if (bm_label && bitmap && bitmap->Ok() && (bitmap->selectedIntoDC >= 0)
      && (bitmap->GetDepth()==1 || bitmap->GetDepth()==wxDisplayDepth())) {
    Pixmap pm;
    --bm_label->selectedIntoDC;
    bm_label = bitmap;
    bm_label->selectedIntoDC++;
    pm = GETPIXMAP(bitmap);
    XtVaSetValues(X->handle, XtNpixmap, pm, NULL);
  }
}

char *wxButton::GetLabel(void)
{
    char *label = NULL;
    
    if (!X->handle) // forbid, if no widget associated
	return NULL;

    XtVaGetValues(X->handle, XtNlabel, &label, NULL);
    return label;
}

void wxButton::ChangeToGray(Bool gray)
{
  if (!X->handle) // forbid, if no widget associated
    return;

  wxItem::ChangeToGray(gray);

  if (gray) {
    /* Un-click it, if during a click */
    XtVaSetValues(X->handle, XtNframeType, XfwfRaised, NULL);
  }
}

//-----------------------------------------------------------------------------
// do the same as if button was clicked
//-----------------------------------------------------------------------------

void wxButton::Command(wxCommandEvent *event)
{
    ProcessCommand(event);
}

//-----------------------------------------------------------------------------
// callback for commandWidgetClass
//-----------------------------------------------------------------------------

void wxButton::EventCallback(Widget WXUNUSED(w), XtPointer clientData,
			     XtPointer WXUNUSED(ptr))
{
    wxButton       *button = (wxButton*)GET_SAFEREF(clientData);
    wxCommandEvent *event;

    if (button) {
      event = new wxCommandEvent(wxEVENT_TYPE_BUTTON_COMMAND);
      
      button->ProcessCommand(event);
    }

#ifdef MZ_PRECISE_GC
    XFORM_RESET_VAR_STACK;
#endif
}
