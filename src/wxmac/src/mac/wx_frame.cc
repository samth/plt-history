///////////////////////////////////////////////////////////////////////////////
// File:	wx_frame.cc
// Purpose:	wxFrame implementation (Macintosh version)
// Author:	Bill Hale
// Created:	1994
// Updated:	
// Copyright:  (c) 1993-94, AIAI, University of Edinburgh. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////

#include "wx_frame.h"
#include "wx_menu.h"
#include "wx_mnuit.h"
#include "wx_gdi.h"
#include "wxMacDC.h"
#include "wx_stdev.h"
#include "wx_panel.h"
#include "wx_area.h"
#include "wx_screen.h"
#include "wx_mac_utils.h"
#include "wx_main.h"
#include "wx_messg.h"
#include "wx_utils.h"
#include "wx_het.h"

static wxMenuBar *close_menu_bar;

extern int mred_current_thread_is_handler(void *ctx);
extern int mred_in_restricted_context();

//=============================================================================
// Public constructors
//=============================================================================

static OSStatus update_if_in_handler(EventHandlerCallRef inHandlerCallRef, 
				     EventRef inEvent, 
				     void *inUserData);

//-----------------------------------------------------------------------------
wxFrame::wxFrame // Constructor (for frame window)
(
 wxFrame*	parentFrame,		// this is ignored
 char*		windowTitle,
 int 		x,
 int			y,
 int			width,
 int			height,
 long		style,
 char*		windowName,
 WXTYPE		objectType
 ) :
 wxbFrame (windowName, wxScreen::gScreenWindow,
	   x, y, 
	   (width < 30) ? 30 : width, 
	   (height < 40) ? 40 : height, style)
{
  int X, Y, theMacX, theMacY, theMacWidth, theMacHeight;
  Rect theBoundsRect;
  OSErr result;
  WindowClass windowClass;
  WindowAttributes windowAttributes;
  Rect theStrucRect;
  Rect theContRect;
  ControlRef rootControl;
  wxMargin contentAreaMargin;
  WindowPtr theMacWindow;
  wxArea *carea, *parea;
  wxMargin pam;
  
  InitDefaults();

  SetEraser(wxCONTROL_BACKGROUND_BRUSH);

  X = cWindowX;
  Y = cWindowY;

  ::SetRect(&theBoundsRect, X, Y, X + cWindowWidth, Y + cWindowHeight);

  cUserHidden = TRUE;

  if (parentFrame)
    if (wxSubType(parentFrame->__type, wxTYPE_DIALOG_BOX))
      parentFrame = (wxFrame *)parentFrame->GetParent();
  
  if (cStyle & wxMDI_CHILD) { // hack : MDI_CHILD means dialog box
#ifdef OS_X
    if (parentFrame && parentFrame->GetSheetParent()) {
      WXGC_IGNORE(this, cSheetParent);
      cSheetParent = parentFrame->GetSheetParent();
      windowClass = kSheetWindowClass;
    } else
#endif
      {
	if (cStyle & wxNO_CAPTION)
	  windowClass = kPlainWindowClass;
	else
	  windowClass = kDocumentWindowClass;  /* kMovableModalWindowClass => OS X does modality, which we don't want */
      }
    if (cStyle & wxNO_RESIZE_BORDER) {
      windowAttributes = kWindowNoAttributes;
    } else {
      windowAttributes = kWindowResizableAttribute;
    }
  } else {
    if (cStyle & wxNO_CAPTION) {
      windowClass = kPlainWindowClass;
      if (cStyle & wxNO_RESIZE_BORDER)
	windowAttributes = kWindowNoAttributes;
      else
	windowClass = kWindowResizableAttribute;
    } else {
      windowClass = kDocumentWindowClass;
      if (cStyle & wxNO_RESIZE_BORDER)
	windowAttributes = kWindowStandardFloatingAttributes;
      else
	windowAttributes = kWindowStandardDocumentAttributes;
    }
  }

#ifdef OS_X
  if (cStyle & wxTOOLBAR_BUTTON)
    windowAttributes |= kWindowToolbarButtonAttribute;
#endif

  result = ::CreateNewWindow(windowClass, windowAttributes, &theBoundsRect, &theMacWindow);  
  
  if (result != noErr) {
    char error[256];
    sprintf(error,"wxFrameConstructor: Attempt to create window failed with error: %d.",
	    result);
    wxFatalError(error);
  }

  if (windowTitle) {
    CFStringRef wtitle;
    wtitle = CFStringCreateWithCString(NULL, windowTitle,kCFStringEncodingISOLatin1);
    SetWindowTitleWithCFString(theMacWindow, wtitle);
    CFRelease(wtitle);
  }
  
  {
    void *rc;
    rc = WRAP_SAFEREF(this);
    refcon = rc;
    SetWRefCon(theMacWindow, (long)refcon);
  }
  
  CheckMemOK(theMacWindow);

  cMacDC = new wxMacDC(GetWindowPort(theMacWindow));

  // Calculate the platformArea size
  GetWindowBounds(theMacWindow,kWindowStructureRgn,&theStrucRect);
  GetWindowBounds(theMacWindow,kWindowContentRgn,&theContRect);
  wxMargin platformMargin;
  platformMargin.SetMargin(theContRect.left - theStrucRect.left, wxLeft);
  platformMargin.SetMargin(theContRect.top - theStrucRect.top, wxTop);
  platformMargin.SetMargin(theStrucRect.right - theContRect.right, wxRight);
  platformMargin.SetMargin(theStrucRect.bottom - theContRect.bottom, wxBottom);
  parea = PlatformArea();
  parea->SetMargin(platformMargin);

  // The client has the requested window position, not the window: must move
  SetCurrentDC();
  carea = ContentArea();
  contentAreaMargin = carea->Margin(wxScreen::gScreenWindow);
  theMacX = contentAreaMargin.Offset(wxLeft);
  theMacY = contentAreaMargin.Offset(wxTop);
  MoveWindow(theMacWindow, theMacX, theMacY, FALSE);

  // The client has the requested window size, not the window: must resize
  parea = PlatformArea();
  pam = parea->Margin();
  theMacWidth = cWindowWidth - pam.Offset(wxHorizontal);
  theMacHeight = cWindowHeight - pam.Offset(wxVertical);
  SizeWindow(theMacWindow, theMacWidth, theMacHeight, FALSE);
  
  windowStyle |= (style & wxHIDE_MENUBAR);

  wx_cursor = wxSTANDARD_CURSOR;
  
  if (wxIsBusy())
    cBusyCursor = 1;

  // create a root control, to enable control embedding
  ::CreateRootControl(theMacWindow,&rootControl);
  cMacControl = rootControl;

#ifdef OS_X
  {
    /* Enable updates for a frame just before it is shown.
       The same handler could probably be used for live resizing. */
    EventTypeSpec spec;
    spec.eventClass = kEventClassWindow;
    spec.eventKind = kEventWindowDrawContent;
    InstallEventHandler(GetWindowEventTarget(theMacWindow),
			update_if_in_handler,
			1,
			&spec,
			refcon,
			NULL);
  }
#endif      
}

//=============================================================================
// Public destructor
//=============================================================================

wxFrame::~wxFrame(void)
{
  CWindowPtr theMacWindow;

  if (IsVisible()) 
    Show(FALSE);

  // Kludge needed here:
  // Bad:  DisposeWindow deletes the mac controls, but not their wxItems.
  // Good: DestroyChildren deletes the wxItems and their associated mac controls.
  if (cDialogPanel)
    cDialogPanel = NULL;
  DestroyChildren();

  theMacWindow = GetWindowFromPort(cMacDC->macGrafPort());
  ::DisposeWindow(theMacWindow);
  DELETE_OBJ cMacDC;
  if (wx_menu_bar)
    DELETE_OBJ wx_menu_bar;
  if (cControlArea)
    DELETE_OBJ cControlArea;
  if (cContentArea)
    DELETE_OBJ cContentArea;
  if (cPlatformArea)
    DELETE_OBJ cPlatformArea;
}

//=============================================================================
// Paint callback when showing
//=============================================================================

#define wxNO_UPDATE 0
#define wxDO_UPDATE 1
#define wxDID_UPDATE 2

static int DoPaint(void *_f)
{
  wxFrame *f = (wxFrame *)_f;
  f->Paint();
  return 0;
}

static OSStatus update_if_in_handler(EventHandlerCallRef inHandlerCallRef, 
				     EventRef inEvent, 
				     void *inUserData)
{
  wxFrame *f;

  f = (wxFrame*)GET_SAFEREF(inUserData);

  /* This event handler should only be used when showing a window,
     where we've set up a trampoline around the ShowWindow call.
     The trampoline is necessary because Paint() might invoke arbitrary
     Scheme code, and because the system called us, we can't
     allow thread swaps (which would copy a part of the stack that
     the system owns). See the use of wxHETShowWindow below. */

  if (f->cCanUpdateOnCallback == wxDO_UPDATE) {
    int c = 0;
    f->cCanUpdateOnCallback = wxDID_UPDATE;
    /* wxHETYield doesn't actually wxYield(). It calls our DoPaint
       proc. This might run some Scheme code and then time out
       for a thread swap. We give the thread up to 10 times its
       normal alotment to finish drawing the frame. */
    while (wxHETYield(f, DoPaint, f)) {
      c++;
      if (c == 10)
	break;
    }
  } else if (f->cCanUpdateOnCallback == wxNO_UPDATE) {
    wxMacDC *dc;
    Rect bounds;
    dc = f->MacDC();
    ::GetPortBounds(dc->macGrafPort(), &bounds);
    ::InvalWindowRect(GetWindowFromPort(dc->macGrafPort()), &bounds);
  }

  return noErr;
}

//=============================================================================
// Root frame
//=============================================================================

wxFrame *wxRootFrame = NULL;

void wxFrame::DesignateRootFrame(void)
{
  if (!wxRootFrame) {
    CGrafPtr graf;
    WindowRef win;

    graf = cMacDC->macGrafPort();
    win = GetWindowFromPort(graf);

    cWindowWidth = 0;
    cWindowHeight = 0;
    cWindowX = 32000;
    cWindowY = 32000;
    ::MoveWindow(win, 32000, 32000, FALSE);
    ::SizeWindow(win, 0, 0, FALSE);

    wxREGGLOB(wxRootFrame);
    wxRootFrame = this;
    Show(TRUE);
    
    if (!FrontWindow()) {
      ::ShowWindow(win);
    }
  }
}

//=============================================================================
// Private methods
//=============================================================================

//-----------------------------------------------------------------------------
void wxFrame::InitDefaults(void)
{
  cMaximized = FALSE;
  cIsModal = FALSE;
  cBusyCursor = 0;
  cFocusWindow = NULL;
  cStatusPanel = NULL;
  cStatusText = NULL;
  cDialogPanel = NULL;
  cControlArea = new wxArea(this);
  cContentArea = new wxArea(this);
  cPlatformArea = new wxArea(this);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Geometry methods
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//-----------------------------------------------------------------------------
wxArea* wxFrame::PlatformArea(void) { return cPlatformArea; } // mac platform only

//-----------------------------------------------------------------------------
wxArea* wxFrame::ContentArea(void) { return cContentArea; } // mac platform only

//-----------------------------------------------------------------------------
wxArea* wxFrame::ControlArea(void) { return cControlArea; } // mac platform only

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Sizing methods
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define max(x, y) ((x > y) ? x : y)

//-----------------------------------------------------------------------------
// Note: Can't set width < width of borders + etc.
// Note: Can't set height < height of borders + statusLine + etc.
// Note: Mac platform can't set width = width of borders + etc.
// Note: Mac platform can't set height = height of borders + statusLine + etc.
//-----------------------------------------------------------------------------
void wxFrame::DoSetSize(int x, int y, int width, int height)
{
  Bool xIsChanged, yIsChanged, widthIsChanged, heightIsChanged;
  WindowPtr theMacWindow;
  int dw, dh;

  if (width==-1) 
    width = cWindowWidth;
  if (height==-1) 
    height = cWindowHeight;
  
  xIsChanged = (x != cWindowX);
  yIsChanged = (y != cWindowY);
  widthIsChanged = (width != cWindowWidth);
  heightIsChanged = (height != cWindowHeight);

  if (! wxTheApp->MacOS85WindowManagerPresent) {
    if (width > cWindowWidth || height > cWindowHeight)
      {
	// Invalidate grow box:
	int oldMacWidth, oldMacHeight;
	Rect oldGrowRect;
	wxArea *parea;
	wxMargin pam;

	parea = PlatformArea();
	pam = parea->Margin();
      
	oldMacWidth = cWindowWidth - pam.Offset(wxHorizontal);
	oldMacHeight = cWindowHeight - pam.Offset(wxVertical);
	::SetRect(&oldGrowRect, oldMacWidth - 15, oldMacHeight - 15, oldMacWidth, oldMacHeight);
	if (SetCurrentMacDC()) {
	  InvalWindowRect(GetWindowFromPort(cMacDC->macGrafPort()),&oldGrowRect);
	  ::EraseRect(&oldGrowRect);
	}
      }
  }

  dw = width - cWindowWidth;
  dh = height - cWindowHeight;

  if (xIsChanged) cWindowX = x;
  if (yIsChanged) cWindowY = y;
  if (widthIsChanged) cWindowWidth = width;
  if (heightIsChanged) cWindowHeight = height;

  SetCurrentDC();
  theMacWindow = GetWindowFromPort(cMacDC->macGrafPort());

  if (xIsChanged || yIsChanged)
    {
      wxMargin contentAreaMargin;
      int theMacX, theMacY;
      wxArea *carea;
      carea = ContentArea();
      contentAreaMargin = carea->Margin(wxScreen::gScreenWindow);
      theMacX = contentAreaMargin.Offset(wxLeft);
      theMacY = contentAreaMargin.Offset(wxTop);
      ::MoveWindow(theMacWindow, theMacX, theMacY, FALSE);
    }

  if (widthIsChanged || heightIsChanged)
    {
      int theMacWidth, theMacHeight;
      wxArea *parea;
      wxMargin pam;
      parea = PlatformArea();
      pam = parea->Margin();
      theMacWidth = cWindowWidth - pam.Offset(wxHorizontal);
      theMacHeight = cWindowHeight - pam.Offset(wxVertical);
      ::SizeWindow(theMacWindow, theMacWidth, theMacHeight, TRUE);
      // Resizing puts windows into the unzoomed state
      cMaximized = FALSE;

      if (dw > 0 || dh > 0) {
	// Invalidate new region:
	int w, h;
	Rect r;
	w = cWindowWidth - pam.Offset(wxHorizontal);
	h = cWindowHeight - pam.Offset(wxVertical);
	if (SetCurrentMacDC()) {
	  if (dw) {
	    r.top = 0;
	    r.bottom = h;
	    r.left = max(0, w - dw);
	    r.right = w;
	    ::InvalWindowRect(theMacWindow, &r);
	  }
	  if (dh) {
	    r.top = max(0, h - dh);
	    r.bottom = h;
	    r.left = 0;
	    r.right = w;
	    ::InvalWindowRect(theMacWindow, &r);
	  }
	}
      }
      
      if (cStatusPanel) {
	int w, h;

	if (cStatusPanel->SetCurrentDC()) {
	  Rect r = {0, 0, 10000, 10000};
	  OffsetRect(&r,SetOriginX,SetOriginY);
	  EraseRect(&r);
	}
	
	cStatusPanel->SetSize(0, theMacHeight - cStatusPanel->Height(),
			      theMacWidth, -1);
	cStatusPanel->GetClientSize(&w, &h);
	cStatusText->SetSize(-1, -1, w, -1);
	
	if (cStatusPanel->SetCurrentDC()) {
	  Rect s = {0, 0, 10000, 10000};
	  OffsetRect(&s,SetOriginX,SetOriginY);
	  EraseRect(&s);
	}
      }
      
      // Call OnSize handler
      if (this != wxRootFrame) {
	OnSize(width, height);
      }
    }
}

//-----------------------------------------------------------------------------
void wxFrame::Maximize(Bool maximize)
{
  if (cMaximized != maximize)
    {
      int oldWindowX = cWindowX;
      int oldWindowY = cWindowY;
      int oldWindowWidth = cWindowWidth;
      int oldWindowHeight = cWindowHeight;
      CGrafPtr theMacGrafPort;
      WindowPtr theMacWindow;

      theMacGrafPort = cMacDC->macGrafPort();
      theMacWindow = GetWindowFromPort(theMacGrafPort);
      if (SetCurrentDC()) {
	Rect portBounds;
	::EraseRect(GetPortBounds(theMacGrafPort,&portBounds));
	::ZoomWindow(theMacWindow, maximize ? inZoomOut : inZoomIn, TRUE);
	InvalWindowRect(theMacWindow,&portBounds);
      } else {
	::ZoomWindow(theMacWindow, maximize ? inZoomOut : inZoomIn, TRUE);
      }

      cMaximized = maximize;

      wxMacRecalcNewSize();
      
      if (cStatusPanel) {
	int theMacWidth, theMacHeight;
	int w, h;
	wxArea *parea;
	wxMargin pam;

	parea = PlatformArea();
	pam = parea->Margin();

	theMacWidth = cWindowWidth - pam.Offset(wxHorizontal);
	theMacHeight = cWindowHeight - pam.Offset(wxVertical);
	if (cStatusPanel->SetCurrentDC()) {
	  Rect r = {0, 0, 10000, 10000};
	  OffsetRect(&r,SetOriginX,SetOriginY);
	  EraseRect(&r);
	}
	
	cStatusPanel->SetSize(0, theMacHeight - cStatusPanel->Height(),
			      theMacWidth, -1);
	cStatusPanel->GetClientSize(&w, &h);
	cStatusText->SetSize(-1, -1, w, -1);

	if (cStatusPanel->SetCurrentDC()) {
	  Rect s = {0, 0, 10000, 10000};
	  OffsetRect(&s,SetOriginX,SetOriginY);
	  EraseRect(&s);
	}
      }
      
      {
	int dW = cWindowWidth - oldWindowWidth;
	int dH = cWindowHeight - oldWindowHeight;
	int dX = cWindowX - oldWindowX;
	int dY = cWindowY - oldWindowY;
	OnWindowDSize(dW, dH, dX, dY);
      }
    }
}

//-----------------------------------------------------------------------------
// Mac platform only; internal use only.
//-----------------------------------------------------------------------------
void wxFrame::wxMacRecalcNewSize(Bool resize)
{
  Rect theStrucRect;
  Rect theContRect;
  int mbh;

  GetWindowBounds(GetWindowFromPort(cMacDC->macGrafPort()),kWindowStructureRgn,&theStrucRect);
  GetWindowBounds(GetWindowFromPort(cMacDC->macGrafPort()),kWindowContentRgn,&theContRect);
  cWindowX = theStrucRect.left;
  mbh = GetMBarHeight();
  cWindowY = theStrucRect.top - mbh;
  if (resize) {
    cWindowWidth = theStrucRect.right - theStrucRect.left;
    cWindowHeight = theStrucRect.bottom - theStrucRect.top;
  }

  if (sheet)
    sheet->wxMacRecalcNewSize(FALSE);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Status line methods
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//-----------------------------------------------------------------------------
void wxFrame::CreateStatusLine(int number, char* name)
{
  wxMargin clientAreaMargin;
  int statusLineHeight;
  int clientWidth, clientHeight;
  wxArea *carea;

  if (status_line_exists) return;

  nb_status = number;
  status_line_exists = TRUE;
  cStatusPanel = new wxPanel(ControlArea());
  cStatusText = new wxMessage(cStatusPanel, "");
  cStatusPanel->SetEraser(cEraser);
  cStatusText->SetEraser(cEraser);
  cStatusText->SetFont(wxNORMAL_FONT);
  statusLineHeight = (int)(cStatusText->GetCharHeight() * nb_status);
  GetClientSize(&clientWidth, &clientHeight);
  cStatusText->SetWidthHeight(clientWidth, statusLineHeight);
  cStatusPanel->Fit();
#if 1
  // it is a hack to put the line down..
  cStatusPanel->SetSize(0, clientHeight - cStatusPanel->Height(),
			cStatusPanel->Width(), cStatusPanel->Height()); // tom!!
  // tom: here the Statuspanel is placed over the controlArea!!
  cControlArea->SetMargin(cStatusPanel->Height() + 1, wxBottom);
#else
  cStatusPanel->SetSize(0, 0, cStatusPanel->Width(), cStatusPanel->Height());
  cControlArea->SetMargin(cStatusPanel->Height() + 1, wxTop);
#endif
  // cStatusPanel->SetJustify(wxLeft);

  carea = ClientArea();

  clientAreaMargin = carea->Margin(wxScreen::gScreenWindow);
  {
    int o;
    o = clientAreaMargin.Offset(wxBottom);
    clientAreaMargin.SetMargin(o + statusLineHeight - 1, wxBottom);
  }
  carea->SetMargin(clientAreaMargin);
  OnSize(cWindowWidth, cWindowHeight);
}

//-----------------------------------------------------------------------------
void wxFrame::SetStatusText(char* text, int number)
{
  if (!status_line_exists) 
    return;
  cStatusText->SetLabel(text);
}

// tom: perhaps this could be done otherwise:
//      get statusline from Frame
//		do statusline->SetEraser
void wxFrame::SetStatusEraser(wxBrush* b)
{
  if (!status_line_exists) 
    return;
  cStatusPanel->SetEraser(b);
  cStatusText->SetEraser(b);
  
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Menubar methods
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//-----------------------------------------------------------------------------
void wxFrame::SetMenuBar(wxMenuBar* menu_bar)
{
  WindowPtr theMacWindow,front;
  wxMenuBar* oldMenuBar;
  CGrafPtr graf;
  
  // if this menu bar is already in use, give up
  if (menu_bar && menu_bar->menu_bar_frame)
    return;

  graf = cMacDC->macGrafPort();
  theMacWindow = GetWindowFromPort(graf);
  oldMenuBar = wx_menu_bar;
  if (oldMenuBar) {
    wx_menu_bar = NULL;
    oldMenuBar->menu_bar_frame = NULL;
    // must correct each menu->menu_bar and menuItem->menuBar
  }

  menu_bar->handle = NULL; // menu_bar->handle is not used
  if (menu_bar) menu_bar->menu_bar_frame = this;
  wx_menu_bar = menu_bar;

  front = FrontWindow();
  if (front == theMacWindow) {
    NowFront(TRUE);
  }
}

//-----------------------------------------------------------------------------
void wxFrame::Command(int id)
{ // Call this to simulate a menu command
  wxMenuBar* menuBar;

  menuBar = GetMenuBar();
  if (menuBar) {
    wxMenuItem* item;
    item = menuBar->FindItemForId(id);
    if (item) {
      wxMenu* theParentMenu;
      theParentMenu = item->ParentMenu();
      if (theParentMenu) {
	wxPrepareMenuDraw();
	HiliteMenu(theParentMenu->cMacMenuId); // hilite the menu
	wxDoneMenuDraw(TRUE);
	ProcessCommand(id);
      }
    }
  }
}

void wxFrame::OnMenuClick()
     /* Called when the user clicks on the menu bar */
{
  /* Do nothing; MrEd overrides it. */
}

//-----------------------------------------------------------------------------
void wxFrame::ProcessCommand(int id)
{
  OnMenuCommand(id);
  wxPrepareMenuDraw();
  // HiliteMenu(0); // calling wxPrepareMenuDraw unhlites the menu
  wxDoneMenuDraw();
}

//-----------------------------------------------------------------------------
void wxFrame::NowFront(Bool flag) // mac platform only
{
  // Show the menuBar for this frame on becoming active
  if (flag)
    {
      wxWindow::gMouseWindow = NULL; // If the frame changes, force capture off
      
      if (wx_menu_bar)
	wx_menu_bar->Install(this);
      else {
	if (!close_menu_bar) {
	  wxREGGLOB(close_menu_bar);
	  close_menu_bar = new wxMenuBar;
#ifndef OS_X
	  /* When a frame doesn't have a menubar, doMacInMenuBar
	     assumes that any menulelection is the close item. */
	  wxMenu *file = new wxMenu();
	  file->Append(1, "Close\tCmd+W");
	  close_menu_bar->Append(file, "File");
#endif
	}
	close_menu_bar->Install(this);
      }
    }
}

void wxFrame::ShowAsActive(Bool flag)
{
  if (! wxTheApp->MacOS85WindowManagerPresent) {
    // Invalidate grow box (appearance changes with window active/inactive)
    if (SetCurrentDC()) {
      int theMacWidth, theMacHeight;
      wxArea *parea;
      wxMargin pam;

      parea = PlatformArea();
      pam = parea->Margin();

      theMacWidth = cWindowWidth - pam.Offset(wxHorizontal);
      theMacHeight = cWindowHeight - pam.Offset(wxVertical);
      {
	Rect growRect = {theMacHeight - 15, theMacWidth - 15, theMacHeight, theMacWidth};
	// Erase it now if we're becoming inactive
	if (!flag) {
	  ::EraseRect(&growRect);
	}
	::InvalWindowRect(GetWindowFromPort(cMacDC->macGrafPort()),&growRect);
      }
    }
  }
  
  if (flag && !cFocusWindow && children) {
    wxChildNode *node;
    wxWindow *win;
    node = children->First();
    while (node) {
      win = (wxWindow *)(node->Data());
      if (win->WantsFocus() && win->CanAcceptEvent()) {
	cFocusWindow = win;
	break;
      }
      node = node->Next();
    }
  }

  if (cFocusWindow) {
    if (flag)
      cFocusWindow->OnSetFocus();
    else
      cFocusWindow->OnKillFocus();
  }
}

void wxFrame::OnToolbarButton(void)
{
}

void wxFrame::SetFrameModified(Bool is_modified)
{
  SetWindowModified(GetWindowFromPort(cMacDC->macGrafPort()), is_modified);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Icon methods
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void wxFrame::SetIcon(wxBitmap* wx_icon, wxBitmap* mask, int kind) { }		// not implemented

void wxFrame::Iconize(Bool iconize) {
  CollapseWindow(GetWindowFromPort(cMacDC->macGrafPort()), iconize);
}

Bool wxFrame::Iconized(void) { 
  return IsWindowCollapsed(GetWindowFromPort(cMacDC->macGrafPort()));
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Platform methods
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//-----------------------------------------------------------------------------
WindowPtr wxFrame::macWindow(void)
{
  return cMacDC ? GetWindowFromPort(cMacDC->macGrafPort()) : NULL;
}

//-----------------------------------------------------------------------------
Bool wxFrame::IsMacWindow(void) { return TRUE; }

//-----------------------------------------------------------------------------
Bool wxFrame::IsVisible(void)
{
  if (cMacDC)
    return IsWindowVisible(GetWindowFromPort(cMacDC->macGrafPort()));
  else
    return FALSE;
}

//-----------------------------------------------------------------------------
void wxFrame::MacUpdateWindow(void)
{
  WindowPtr theMacWindow;
  theMacWindow = macWindow();
  if (!is_in_update && theMacWindow && SetCurrentDC()) {
    is_in_update++;
    ::BeginUpdate(theMacWindow);

    Paint();

    ::EndUpdate(theMacWindow);
    --is_in_update;
  }
}

//-----------------------------------------------------------------------------
void wxFrame::MacDrawGrowIcon(void)
{
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Other methods
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//-----------------------------------------------------------------------------
char* wxFrame::GetTitle(void) // WCH: return type should be "const char*"
{
  Str255 theTitle;
  WindowPtr theMacWindow;

  theMacWindow = GetWindowFromPort(cMacDC->macGrafPort());
  ::GetWTitle(theMacWindow, theTitle);
  return wxP2C(theTitle);
}

//-----------------------------------------------------------------------------
void wxFrame::SetTitle(char* title)
{
  WindowPtr theMacWindow;
  theMacWindow = GetWindowFromPort(cMacDC->macGrafPort());
  {
    CFStringRef wtitle;
    wtitle = CFStringCreateWithCString(NULL, title, kCFStringEncodingISOLatin1);
    SetWindowTitleWithCFString(theMacWindow, wtitle);
    CFRelease(wtitle);
  }
}

//-----------------------------------------------------------------------------
void wxFrame::Show(Bool show)
{
  WindowPtr theMacWindow;
  wxChildList *tlw;

  if (!show == cUserHidden) {
    if (show) {
      CGrafPtr graf;
      graf = cMacDC->macGrafPort();
      ::SelectWindow(GetWindowFromPort(graf));
    }
    return;
  }
  
  cUserHidden = !show;

  if (window_parent) {
    wxChildList *cl;
    cl = window_parent->GetChildren();
    cl->Show(this, show);
  }
  if (cParentArea) {
    wxChildList *cl;
    cl = cParentArea->Windows();
    cl->Show(this, show);
  }
  tlw = wxTopLevelWindows(ContextWindow());
  tlw->Show(this, show);

  if (wxRootFrame == this)
    return;

  theMacWindow = GetWindowFromPort(cMacDC->macGrafPort());
  if (show) {
#ifdef OS_X
    if (cSheetParent && !cSheetParent->sheet) {
      WindowPtr pwin;
      CGrafPtr graf;

      graf = cSheetParent->cMacDC->macGrafPort();
      pwin = GetWindowFromPort(graf);
      if (mred_current_thread_is_handler(context)
	  && !mred_in_restricted_context()) {
	/* Enable the paint callback, which must run in
	   a special trampoline mode; see update_if_in_handler
	   above. */
	SetCurrentDC();
	cCanUpdateOnCallback = wxDO_UPDATE;
	wxHETShowSheetWindow(theMacWindow, pwin);
	if (cCanUpdateOnCallback == wxDO_UPDATE) {
	  cCanUpdateOnCallback = wxNO_UPDATE;
	  Refresh();
	} else
	  cCanUpdateOnCallback = wxNO_UPDATE;
      } else {
	ShowSheetWindow(theMacWindow, pwin);
	Refresh();
      }
      cSheetParent->sheet = this;
      ChangeWindowAttributes(pwin, 0, kWindowCloseBoxAttribute);
      wxMacRecalcNewSize(FALSE); // recalc new position only
    } else { 
      if (mred_current_thread_is_handler(context)
	  && !mred_in_restricted_context()) {
	/* Enable the paint callback, which must run in
	   a special trampoline mode; see update_if_in_handler
	   above. */
	SetCurrentDC();
	cCanUpdateOnCallback = TRUE;
	wxHETShowWindow(theMacWindow);
	if (cCanUpdateOnCallback == wxDO_UPDATE) {
	  cCanUpdateOnCallback = wxNO_UPDATE;
	  Refresh();
	} else
	  cCanUpdateOnCallback = wxNO_UPDATE;
      } else {
	ShowWindow(theMacWindow);
	Refresh();
      }
    }
#else
    ShowWindow(theMacWindow);
#endif
      
    ::SelectWindow(theMacWindow); 

    if (cMacDC->currentUser() == this)
      /* b/c may be optimized for hidden: */
      cMacDC->setCurrentUser(NULL);

    /* Hide root frame, if any, in case it's shown */
    if (wxRootFrame) {
      wxMacDC *dc;
      dc = wxRootFrame->MacDC();
      theMacWindow = GetWindowFromPort(dc->macGrafPort());
      ::HideWindow(theMacWindow);
    }
  } else {
    if (cFocusWindow) {
      cFocusWindow->OnKillFocus();
      cFocusWindow = NULL;
    }
#ifdef OS_X
    if (cSheetParent && (cSheetParent->sheet == this)) {
      WindowPtr pwin;
      CGrafPtr graf;

      ::HideSheetWindow(theMacWindow);
      cSheetParent->sheet = NULL;

      graf = cSheetParent->cMacDC->macGrafPort();
      pwin = GetWindowFromPort(graf);
      ChangeWindowAttributes(pwin, kWindowCloseBoxAttribute, 0);
    } else
#endif
      {
	::HideWindow(theMacWindow);
      }

    /* If all frames are closed, it's time for the root window (if any) */
    if (wxRootFrame && !FrontWindow()) {
      wxMacDC *dc;
      dc = wxRootFrame->MacDC();
      theMacWindow = GetWindowFromPort(dc->macGrafPort());
      ::ShowWindow(theMacWindow);
    }
  }

  /* Paint(); */
}

wxFrame *wxFrame::GetSheetParent()
{
#ifdef OS_X
  if (cSheetParent)
    return NULL; /* No nested sheets */
  else
#endif
    return this;
}

wxFrame *wxFrame::GetSheetChild()
{
#ifdef OS_X
  return sheet;
#else
  return NULL;
#endif
}

//-----------------------------------------------------------------------------
Bool wxFrame::IsFrontWindow(void)
{
  WindowPtr theMacWindow, front;
  theMacWindow = macWindow();
  if (theMacWindow) {
    front = FrontWindow();
    return (theMacWindow == front);
  } else
    return FALSE;
}

//-----------------------------------------------------------------------------
Bool wxFrame::IsModal(void) { return cIsModal; } //cjc, mflatt

//-----------------------------------------------------------------------------
void wxFrame::MakeModal(Bool modal)
{
  cIsModal = modal;
  // wxbWindow::MakeModal(modal);
}

//-----------------------------------------------------------------------------
wxWindow* wxFrame::GetFocusWindow(void) { return cFocusWindow; }

//-----------------------------------------------------------------------------
void wxFrame::SetFocusWindow(wxWindow* window)
{
  if (window != cFocusWindow) {
    if (cFocusWindow && cActive) cFocusWindow->OnKillFocus();
    cFocusWindow = window;
    if (window && cActive) window->OnSetFocus();
  }
}

//-----------------------------------------------------------------------------

void wxFrame::LoadAccelerators(char* table) { } // Not Applicable for Mac platform

//-----------------------------------------------------------------------------
void wxFrame::Paint(void)
{
  if (SetCurrentDC()) {
    RgnHandle rgn, subrgn, noerasergn;
    rgn = NewRgn();
    if (rgn) {
      subrgn = NewRgn();
      if (subrgn) {
	noerasergn = NewRgn();
	if (noerasergn) {
	  RGBColor save;
	  
	  SetRectRgn(rgn, 0, 0, cWindowWidth, cWindowHeight + 1);
	  AddWhiteRgn(subrgn, noerasergn);
	  DiffRgn(rgn, subrgn, rgn);
	  DiffRgn(rgn, noerasergn, rgn);
	  EraseRgn(rgn);
	  GetForeColor(&save);
	  ForeColor(whiteColor);
	  PaintRgn(subrgn);
	  RGBForeColor(&save);

	  DisposeRgn(noerasergn);
	}
	DisposeRgn(subrgn);
      }
      DisposeRgn(rgn);
    }
    wxWindow::Paint();
#if 0
    if (cStatusPanel)
      cStatusPanel->Paint();
#endif
  }
}

RgnHandle wxFrame::GetCoveredRegion(int x, int y, int w, int h)
{
  return NULL;
}

//-----------------------------------------------------------------------------
void wxFrame::OnChar(wxKeyEvent *event) // mac platform only
{
  if (cFocusWindow) {
    if (cFocusWindow != this) // kludge to prevent infinite loop
      cFocusWindow->OnChar(event);
  }
}

//-----------------------------------------------------------------------------
void wxFrame::Enable(Bool enable)
{
  wxWindow::Enable(enable);
  // Enable/disbale menubar
  if (wx_menu_bar)
    wx_menu_bar->Install(this);	
}


wxFrame *wxFrame::GetRootFrame()
{
  return this;
}

ControlHandle wxFrame::GetRootControl(void)
{
  return cMacControl;
}
