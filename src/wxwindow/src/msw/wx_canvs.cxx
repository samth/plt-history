/*
 * File:        wx_canvs.cc
 * Purpose:     wxCanvas implementation (MSW)
 * Author:      Julian Smart
 * Created:     1993
 * Updated:	August 1994
 * Copyright:   (c) 1993, AIAI, University of Edinburgh
 *
 * Renovated by Matthew for MrEd, 1995-2000
 */

#include "wx.h"

#include <math.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

static wxCanvas *current_gl_context;

class wxGLContext : public wxObject
{
public:
  HGLRC			m_hGLRC;
  HDC				m_hDC;
  wxColourMap*	m_palette;
  wxWindow*		m_window;
  Bool			m_deletePalette;

  wxGLContext(wxWindow *win);
  ~wxGLContext(void);

  void SetupPixelFormat(void);
  void SetupPalette(void);
  wxColourMap* CreateDefaultPalette(void);
};

extern char wxCanvasClassName[];

wxCanvas::wxCanvas (void)
{
  wxWinType = wxTYPE_XWND;
  handle = NULL;
  window_parent = NULL;
  horiz_units = 0;
  vert_units = 0;
  wx_dc = NULL;
}

wxCanvas::wxCanvas (wxWindow *parent, int x, int y, int width, int height, long style,
	  char *name):
wxbCanvas (parent, x, y, width, height, style, name)
{
  Create(parent, x, y, width, height, style, name);
}

Bool wxCanvas::
Create (wxWindow * parent, int x, int y, int width, int height, long style,
	char *name)
{
  wxWinType = wxTYPE_XWND;
  windowStyle = style;
  wxWnd *cparent = NULL;
  if (parent)
    cparent = (wxWnd *) parent->handle;

  if (wxSubType(parent->__type, wxTYPE_PANEL))
    ((wxPanel *)parent)->GetValidPosition(&x, &y);

  DWORD msflags = 0;
  if (style & wxBORDER)
    msflags |= WS_BORDER;
  msflags |= WS_CHILD | WS_VISIBLE;
  if (style & wxHSCROLL)
    msflags |= WS_HSCROLL;
  if (style & wxVSCROLL)
    msflags |= WS_VSCROLL;
  msflags |= WS_CLIPSIBLINGS;

  wxCanvasWnd *wnd = new wxCanvasWnd (cparent, this, x, y, width, height, msflags);
  handle = (char *) wnd;

  if (parent)
    parent->AddChild (this);
  window_parent = parent;

  horiz_units = 0;
  vert_units = 0;

  if ((style & wxHSCROLL) || (style & wxVSCROLL))
    SetScrollbars(style & wxHSCROLL, style & wxVSCROLL,
		  0, 0, 1, 1, 0, 0, FALSE);

  wx_dc = new wxCanvasDC (this);

  if (wxSubType(parent->__type, wxTYPE_PANEL))
    ((wxPanel *)parent)->AdvanceCursor(this);

  if (style & wxGL_CONTEXT) {
    m_wxglc = new wxGLContext(this);
  }

  return TRUE;
}

wxCanvas::~wxCanvas (void)
{
  if (current_gl_context == this) {
    wglMakeCurrent(NULL, NULL);
    current_gl_context = NULL;
  }

  if (m_wxglc)
    delete m_wxglc;

  if (wx_dc) {
    wxWnd *wnd = (wxWnd *)handle;
    HDC dc = wxwmGetDC(wnd->handle);
    wx_dc->SelectOldObjects(dc);
    wxwmReleaseDC(wnd->handle, dc);
    delete wx_dc;
  }
}

void wxCanvas::SetSize (int x, int y, int w, int h, int sizeFlags)
{
  int currentX, currentY;
  GetPosition (&currentX, &currentY);
  if (x == -1)
    x = currentX;
  if (y == -1)
    y = currentY;

  int ww, hh;
  GetSize (&ww, &hh);
  if (w == -1)
    w = ww;
  if (h == -1)
    h = hh;

  wxWnd *wnd = (wxWnd *) handle;
  if (wnd)
    {
      MoveWindow (wnd->handle, x, y, w, h, TRUE);
      OnSize (w, h);
    }
}

wxWindow *wxCanvas::FindFocusWindow()
{
  if (!wxSubType(__type, wxTYPE_PANEL))
    return IsShown() ? this : NULL;
  else
    return wxWindow::FindFocusWindow();
}

/*
 * horizontal/vertical: number of pixels per unit (e.g. pixels per text line)
 * x/y_length:        : no. units per scrollbar
 * x/y_page:          : no. units per page scrolled
 */
void wxCanvas::
SetScrollbars (int horizontal, int vertical,
	       int x_length, int y_length,
	       int x_page, int y_page,
	       int x_pos, int y_pos, Bool setVirtualSize)
{
  if (!(GetWindowStyleFlag() & wxHSCROLL))
    horizontal = -1;
  if (!(GetWindowStyleFlag() & wxVSCROLL))
    vertical = -1;
  
  if (!horizontal) horizontal = -1;
  if (!vertical) vertical = -1;
  if (x_length < 1) horizontal = -1;
  if (y_length < 1) vertical = -1;
  if (x_page < 1) x_page = 1;
  if (y_page < 1) y_page = 1;
  
  if (x_pos < 0)
    x_pos = 0;
  if (x_pos > x_length)
    x_pos = x_length;
  if (y_pos < 0)
    y_pos = 0;
  if (y_pos > y_length)
    y_pos = y_length;
  
  horiz_units = horizontal;
  vert_units = vertical;
  
  wxWnd *wnd = (wxWnd *) handle;
  if (wnd) {
    Bool h_is_on, v_is_on;
    
    wnd->calcScrolledOffset = setVirtualSize;
    
    int w, h;
    RECT rect;
    GetClientRect(wnd->handle, &rect);
    w = rect.right - rect.left;
    h = rect.bottom - rect.top;

    if (!w) w = 1;
    if (!h) h = 1;
    
    SCROLLINFO hinfo, vinfo;
    hinfo.cbSize = vinfo.cbSize = sizeof(SCROLLINFO);
    hinfo.fMask = vinfo.fMask = SIF_PAGE | SIF_RANGE | SIF_POS | SIF_DISABLENOSCROLL;
    hinfo.nMin = vinfo.nMin = 0;
    
    // Recalculate scroll bar range and position
    // ShowScrollBar(handle, SB_HORZ, wnd->xscroll_lines > 0);
    if (horizontal > 0)	{
      h_is_on = 1;
      
      if (setVirtualSize) {
	wnd->xscroll_pixels_per_line = 1;
	wnd->xscroll_lines = (x_length * horizontal);
      } else {
	wnd->xscroll_pixels_per_line = horizontal;
	wnd->xscroll_lines = x_length;
	wnd->xscroll_lines_per_page = x_page;
      }

      hinfo.fMask |= SIF_DISABLENOSCROLL;
      
      int nHscrollMax;
      if (setVirtualSize) {
	int nMaxWidth = wnd->xscroll_lines;
	nHscrollMax = (nMaxWidth - w);
	nHscrollMax = max(0, nHscrollMax);
	wnd->xscroll_lines_per_page = nHscrollMax ? w : 1;
      } else
	nHscrollMax = wnd->xscroll_lines;

      wnd->xscroll_position = min(nHscrollMax, x_pos);
      
      hinfo.nPos = wnd->xscroll_position;
      hinfo.nPage = wnd->xscroll_lines_per_page;
      hinfo.nMax = nHscrollMax + hinfo.nPage - 1;
    } else {
      h_is_on = 0;
      
      wnd->xscroll_pixels_per_line = -1;
      wnd->xscroll_lines = -1;
      wnd->xscroll_lines_per_page = 0;
      
      hinfo.nPos = 0;
      hinfo.nPage = 1;
      hinfo.nMax = 0;
    }
    
    
    // ShowScrollBar(handle, SB_VERT, wnd->yscroll_lines > 0);
    if (vertical > 0) {
      v_is_on = 1;
      
      if (setVirtualSize) {
	wnd->yscroll_pixels_per_line = 1;
	wnd->yscroll_lines = (y_length * vertical);
      } else {
	wnd->yscroll_pixels_per_line = vertical;
	wnd->yscroll_lines = y_length;
	wnd->yscroll_lines_per_page = y_page;
      }
      
      vinfo.fMask |= SIF_DISABLENOSCROLL;
      
      int nVscrollMax;
      if (setVirtualSize) {
	int nMaxHeight = wnd->yscroll_lines;
	nVscrollMax = (nMaxHeight - h);
	nVscrollMax = max(nVscrollMax, 0);
	wnd->yscroll_lines_per_page = nVscrollMax ? h : 1;
      } else
	nVscrollMax  = wnd->yscroll_lines;
      
      wnd->yscroll_position = min (nVscrollMax, y_pos);
      
      vinfo.nPos = wnd->yscroll_position;
      vinfo.nPage = wnd->yscroll_lines_per_page;
      vinfo.nMax = nVscrollMax + vinfo.nPage - 1;
    } else {
      v_is_on = 0;
      
      wnd->yscroll_pixels_per_line = -1;
      wnd->yscroll_lines = -1;
      wnd->yscroll_lines_per_page = 0;
      
      vinfo.nPos = 0;
      vinfo.nPage = 1;
      vinfo.nMax = 0;
    }
    
    if (GetWindowStyleFlag() & wxVSCROLL)
      ::SetScrollInfo(wnd->handle, SB_VERT, &vinfo, TRUE);
    if (GetWindowStyleFlag() & wxHSCROLL)
      ::SetScrollInfo(wnd->handle, SB_HORZ, &hinfo, TRUE);
    
    InvalidateRect (wnd->handle, NULL, TRUE);
    UpdateWindow (wnd->handle);
  }
}

void wxCanvas::GetScrollUnitsPerPage (int *x_page, int *y_page)
{
  wxWnd *wnd = (wxWnd *) handle;
  if (wnd)
    {
      *x_page = wnd->xscroll_lines_per_page;
      *y_page = wnd->yscroll_lines_per_page;
    }
}

/*
 * Scroll to given position (scroll position, not pixel position)
 */
void wxCanvas::Scroll (int x_pos, int y_pos)
{
  if (x_pos > -1)
    SetScrollPos(-wxHORIZONTAL, x_pos);
  if (y_pos > -1)
    SetScrollPos(-wxVERTICAL, y_pos);
}

void wxCanvas::ScrollPercent(float x, float y)
{
  wxWnd *wnd = (wxWnd *) handle;
  if (!wnd) return;

  if (!wnd->calcScrolledOffset) {
    /* Not managing  - do nothing */
  } else {
    /* Managing */
    int xp, yp, vw, vh, cw, ch;
    GetVirtualSize(&vw, &vh);
    GetClientSize(&cw, &ch);

    if (vw > cw)
      vw -= cw;
    else
      vw = 0;
    if (vh > ch)
      vh -= ch;
    else
      vh = 0;

    if (x >= 0)
      xp = (int)floor(x * vw);
    else
      xp = -1;

    if (y >= 0)
      yp = (int)floor(y * vh);
    else
      yp = -1;

    Scroll(xp, yp);

    if ((xp > 0) || (yp > 0))
      Refresh();
  }
}

void wxCanvas::EnableScrolling (Bool x_scroll, Bool y_scroll)
{
  wxWnd *wnd = (wxWnd *) handle;
  wnd->x_scrolling_enabled = x_scroll;
  wnd->y_scrolling_enabled = y_scroll;
}

void wxCanvas::GetVirtualSize (int *x, int *y)
{
 GetClientSize(x, y);

  wxWnd *wnd = (wxWnd *) handle;
  if (wnd && wnd->calcScrolledOffset) {
    if (wnd->xscroll_lines > 0)
      *x = wnd->xscroll_pixels_per_line * wnd->xscroll_lines;
    if (wnd->yscroll_lines > 0)
      *y = wnd->yscroll_pixels_per_line * wnd->yscroll_lines;
  }
}

void wxCanvas::WarpPointer (int x_pos, int y_pos)
{
  // Move the pointer to (x_pos,y_pos) coordinates. They are expressed in
  // pixel coordinates, relatives to the canvas -- So, we first need to
  // substract origin of the window, then convert to screen position

  wxWnd *wnd = (wxWnd *) handle;

  if (wnd)
    {
      x_pos -= wnd->xscroll_position * wnd->xscroll_pixels_per_line;
      y_pos -= wnd->yscroll_position * wnd->yscroll_pixels_per_line;

      RECT rect;
      GetWindowRect (wnd->handle, &rect);

      x_pos += rect.left;
      y_pos += rect.top;

      SetCursorPos (x_pos, y_pos);
    }
}

// Where the current view starts from
void wxCanvas::ViewStart(int *x, int *y, Bool)
{
  wxWnd *wnd = (wxWnd *) handle;

  if (!wnd->calcScrolledOffset) {
    *x = *y = 0;
  } else {
    *x = wnd->xscroll_position;
    *y = wnd->yscroll_position;
  }
}

void wxWnd::DeviceToLogical (float *x, float *y)
{
  if (is_canvas)
    {
      wxCanvas *canvas = (wxCanvas *) wx_window;
      if (canvas->wx_dc)
      {
        *x = canvas->wx_dc->DeviceToLogicalX ((int) *x);
        *y = canvas->wx_dc->DeviceToLogicalY ((int) *y);
      }
    }
}

wxCanvasWnd::wxCanvasWnd (wxWnd * parent, wxWindow * wx_win,
			  int x, int y, int width, int height, DWORD style)
: wxSubWnd (parent, wxCanvasClassName, wx_win, x, y, width, height, style)
{
  is_canvas = TRUE;
}


BOOL wxCanvasWnd::OnEraseBkgnd (HDC pDC)
{
  RECT rect;
  GetClientRect(handle, &rect);
  int mode = SetMapMode(pDC, MM_TEXT);
  FillRect(pDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
  SetMapMode(pDC, mode);

  wxCanvas *canvas = (wxCanvas *)wx_window;
  if (canvas->wx_dc) {
    SetViewportExtEx(pDC, VIEWPORT_EXTENT, VIEWPORT_EXTENT, NULL);
    SetWindowExtEx(pDC, canvas->wx_dc->window_ext_x, canvas->wx_dc->window_ext_y, NULL);
  }
  
  return TRUE;
}

extern void MrEdQueuePaint(wxWindow *wx_window);

BOOL wxCanvasWnd::OnPaint(void)
{
  int retval = 0;

  if (wx_window) {
    HRGN tRgn;
    tRgn = CreateRectRgn(0,0,0,0);
    
    if (GetUpdateRgn(handle, tRgn, FALSE)) {
      PAINTSTRUCT ps;

      BeginPaint(handle, &ps);

      /* We used to call wx_window->OnPaint directly;
	 now we queue an event. */
      MrEdQueuePaint(wx_window);

      EndPaint(handle, &ps);
      cdc = NULL;
      
      retval = 1;
    }
    
    DeleteObject(tRgn);
  }

  return retval;
}

void wxCanvas::CanvasSwapBuffers(void)
{
  if (m_wxglc)
    SwapBuffers(this->m_wxglc->m_hDC);
}

void wxCanvas::ThisContextCurrent(void)
{
  if (m_wxglc) {
    wglMakeCurrent(m_wxglc->m_hDC, m_wxglc->m_hGLRC);
    current_gl_context = this;
  }
}

/**************************************************/

/*
 * GLContext implementation
 */

wxGLContext::wxGLContext(wxWindow *win)
{
  m_window = win;

  m_palette = NULL;
  m_deletePalette = FALSE;


  m_hDC = NULL;
  m_hGLRC = NULL;

  m_hDC = ::GetDC(win->GetHWND());
 
  SetupPixelFormat();
  SetupPalette();

  m_hGLRC = wglCreateContext(m_hDC);	

  WXGC_IGNORE(this, m_window);
}

wxGLContext::~wxGLContext(void)
{
  if (m_hGLRC) {
    wglDeleteContext(m_hGLRC);
  }
  if ( m_palette && m_deletePalette)
    delete m_palette;

  ::ReleaseDC(m_window->GetHWND(), m_hDC);
}

void wxGLContext::SetupPixelFormat(void) // (HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),	/* size */
	1,				/* version */
	PFD_SUPPORT_OPENGL |
	PFD_DRAW_TO_WINDOW |
	PFD_DOUBLEBUFFER,		/* support double-buffering */
	PFD_TYPE_RGBA,			/* color type */
	16,				/* prefered color depth */
	0, 0, 0, 0, 0, 0,		/* color bits (ignored) */
	0,				/* no alpha buffer */
	0,				/* alpha bits (ignored) */
	0,				/* no accumulation buffer */
	0, 0, 0, 0,			/* accum bits (ignored) */
	16,				/* depth buffer */
	0,				/* no stencil buffer */
	0,				/* no auxiliary buffers */
	PFD_MAIN_PLANE,			/* main layer */
	0,				/* reserved */
	0, 0, 0,			/* no layer, visible, damage masks */
    };
    int pixelFormat;

    pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
    if (pixelFormat == 0) {
	MessageBox(WindowFromDC(m_hDC), "ChoosePixelFormat failed.", "Error",
		MB_ICONERROR | MB_OK);
	exit(1);
    }

    if (SetPixelFormat(m_hDC, pixelFormat, &pfd) != TRUE) {
	MessageBox(WindowFromDC(m_hDC), "SetPixelFormat failed.", "Error",
		MB_ICONERROR | MB_OK);
	exit(1);
    }
}

void wxGLContext::SetupPalette() // (HDC hDC)
{
    int pixelFormat = GetPixelFormat(m_hDC); // GetPixelFormat is an OpenGL call.
    PIXELFORMATDESCRIPTOR pfd;

    DescribePixelFormat(m_hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE)
    {
    }
    else
    {
	  return;
    }

    m_palette = CreateDefaultPalette();
	m_deletePalette = TRUE;
    
    if (m_palette && m_palette->ms_palette)
    {
        SelectPalette(m_hDC, m_palette->ms_palette, FALSE);
        RealizePalette(m_hDC);
    }
}

wxColourMap* wxGLContext::CreateDefaultPalette(void)
{
    PIXELFORMATDESCRIPTOR pfd;
    int paletteSize;
    int pixelFormat = GetPixelFormat(m_hDC); // GetPixelFormat is an OpenGL call.

    DescribePixelFormat(m_hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	paletteSize = 1 << pfd.cColorBits;

    LOGPALETTE* pPal =
     (LOGPALETTE*) malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
    pPal->palVersion = 0x300;
    pPal->palNumEntries = paletteSize;

    /* build a simple RGB color palette */
    {
	int redMask = (1 << pfd.cRedBits) - 1;
	int greenMask = (1 << pfd.cGreenBits) - 1;
	int blueMask = (1 << pfd.cBlueBits) - 1;
	int i;

	for (i=0; i<paletteSize; ++i) {
	    pPal->palPalEntry[i].peRed =
		    (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
	    pPal->palPalEntry[i].peGreen =
		    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
	    pPal->palPalEntry[i].peBlue =
		    (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
	    pPal->palPalEntry[i].peFlags = 0;
	}
    }

    HPALETTE hPalette = CreatePalette(pPal);
    free(pPal);

    wxColourMap* cmap = new wxColourMap;
    cmap->ms_palette = hPalette;

    return cmap;
}
