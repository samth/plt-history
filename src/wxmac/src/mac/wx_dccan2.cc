///////////////////////////////////////////////////////////////////////////////
// File:	wx_dccan2.cc
// Purpose:	Canvas device context implementation (Macintosh version) (part 2)
// Author:	Bill Hale
// Created:	1994
// Updated:	
// Copyright:  (c) 2005 PLT Scheme, Inc.
// Copyright:  (c) 1993-94, AIAI, University of Edinburgh. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "wx_dccan.h"
#include "wx_utils.h"
#include "wx_canvs.h"
#include "wx_area.h"
#include "wx_rgn.h"

extern CGrafPtr wxMainColormap;

// constant to convert radian to degree
#define RAD2DEG 57.2957795131
#define wxPI 3.141592653589793

//-----------------------------------------------------------------------------
void wxCanvasDC::Clear(void)
     //-----------------------------------------------------------------------------
{
  int w, h;
  Rect theClearRect;
  
  if (!Ok() || !cMacDC) return;

  if (canvas)
    canvas->GetVirtualSize(&w, &h);
  else {
    w = pixmapWidth;
    h = pixmapHeight;
  }
  
  if (anti_alias) {
    CGContextRef cg;
    wxColor *c = current_background_color;
    int r, g, b;
      
    SetCurrentDC(TRUE);
    cg = GetCG();

    CGContextSaveGState(cg);

    r = c->Red();
    g = c->Green();
    b = c->Blue();
      
    CGContextSetRGBFillColor(cg, r / 255.0, g / 255.0, b / 255.0, 1.0);

    CGContextMoveToPoint(cg, 0, 0);
    CGContextAddLineToPoint(cg, w, 0);
    CGContextAddLineToPoint(cg, w, h);
    CGContextAddLineToPoint(cg, 0, h);
    CGContextFillPath(cg);

    CGContextRestoreGState(cg);

    ReleaseCurrentDC();

    return;
  }
  
  SetCurrentDC();
  wxMacSetCurrentTool(kBGTool);

  ::SetRect(&theClearRect, 0, 0, w, h);
  OffsetRect(&theClearRect,SetOriginX,SetOriginY);
  ::EraseRect(&theClearRect);

  ReleaseCurrentDC();
}

void wxCanvasDC::GetSize(double *width, double *height)
{
  if (canvas) {
    int w, h;
    canvas->GetVirtualSize(&w, &h);
    *width = w;
    *height = h;
  } else {
    *width = pixmapWidth;
    *height = pixmapHeight;
  }
}

//-----------------------------------------------------------------------------
Bool wxCanvasDC::GetPixel(double x, double y, wxColour *col)
     //=============================================================================
{
  RGBColor rgb;
  int i, j;

  if (!Ok() || !cMacDC) return FALSE;
  
  SetCurrentDC();

  i = XLOG2DEV(x) + SetOriginX;
  j = YLOG2DEV(y) + SetOriginY;
  GetCPixel(i, j, &rgb);
  col->Set(rgb.red >> 8, rgb.green >> 8, rgb.blue >> 8);

  ReleaseCurrentDC();

  return ((i >= 0) && (j >= 0)
	  && (i < pixmapWidth)
	  && (j < pixmapHeight));
}

//-----------------------------------------------------------------------------
void wxCanvasDC::SetPixel(double x, double y, wxColour *col)
     //=============================================================================
{
  if (!Ok() || !cMacDC) return;
  
  SetCurrentDC();

  SetPixelCore(XLOG2DEV(x) + SetOriginX, YLOG2DEV(y) + SetOriginY,
	       col->Red(), col->Green(), col->Blue());

  ReleaseCurrentDC();
}

Bool wxCanvasDC::BeginSetPixelFast(int x, int y, int w, int h)
{
  if ((x >= 0) && (y >= 0)
      && ((x + w) <= pixmapWidth)
      && ((y + h) <= pixmapHeight)) {
    PixMapHandle ph;
    cMacDC->EndCG();
    ph = GetGWorldPixMap(cMacDC->macGrafPort());
    fast_pb = GetPixBaseAddr(ph);
    fast_rb = GetPixRowBytes(ph);
    return TRUE;
  } else
    return FALSE;
}

void wxCanvasDC::EndSetPixelFast()
{
  fast_pb = NULL;
}

void wxCanvasDC::SetPixelFast(int i, int j, int r, int g, int b)
{
  if (Colour) {
    UInt32 *p;
    
    p = (UInt32 *)fast_pb;
    p[(j * (fast_rb >> 2)) + i] = ((r << 16) | (g << 8) | (b << 0));
  } else {
    unsigned char *p, v, bit;
    int pos;

    p = (unsigned char *)fast_pb;
    bit = 1 << (7 - (i & 0x7));
    pos = (j * fast_rb) + (i >> 3);
    v = p[pos];
    if (r || g || b)
      v -= (v & bit);
    else
      v |= bit;
    p[pos] = v;
  }
}

void wxCanvasDC::SetPixelCore(int i, int j, int r, int g, int b)
{
  RGBColor rgb;

  if (Colour) {
    rgb.red = r;
    rgb.red = (rgb.red << 8) | rgb.red;
    rgb.green = g;
    rgb.green = (rgb.green << 8) | rgb.green;
    rgb.blue = b;
    rgb.blue = (rgb.blue << 8) | rgb.blue;
    SetCPixel(i, j, &rgb);
  } else {
    int qcol;

    if ((r == 255) && (b == 255) && (g == 255)) {
      qcol = whiteColor;
    } else {
      qcol = blackColor;      
    }

    GetForeColor(&rgb);
    ForeColor(qcol);
    wxMacDrawPoint(i, j);
    if (rgb.red) {
      if (qcol != whiteColor)
	ForeColor(whiteColor);
    } else {
      if (qcol != blackColor)
	ForeColor(blackColor);
    }
  }
}

Bool wxCanvasDC::BeginGetPixelFast(int x, int y, int w, int h)
{
  return BeginSetPixelFast(x, y, w, h);
}

void wxCanvasDC::EndGetPixelFast()
{
  EndSetPixelFast();
}

void wxCanvasDC::GetPixelFast(int x, int y, int *r, int *g, int *b)
{
  if (Colour) {
    UInt32 *p, v;

    p = (UInt32 *)fast_pb;
    v = p[(y * (fast_rb >> 2)) + x];
    *r = (v >> 16) & 0xFF;
    *g = (v >> 8) & 0xFF;
    *b = v & 0xFF;
  } else {
    unsigned char *p, v, bit;

    p = (unsigned char *)fast_pb;
    bit = 1 << (7 - (x & 0x7));
    v = p[(y * fast_rb) + (x >> 3)];
    if (v & bit)
      *r = *b = *g = 0;
    else
      *r = *b = *g = 255;
  }
}

//-----------------------------------------------------------------------------
void wxCanvasDC::DrawLine(double x1, double y1, double x2, double y2)
     //-----------------------------------------------------------------------------
{
  int dpx, dpy;

  if (!Ok() || !cMacDC) return;
  
  if (!current_pen || current_pen->GetStyle() == wxTRANSPARENT)
    return;

  if (anti_alias) {
    CGContextRef cg;

    SetCurrentDC(TRUE);
    cg = GetCG();

    CGContextSaveGState(cg);

    if ((anti_alias == 2)
	&& (user_scale_x == 1.0)
	&& (user_scale_y == 1.0)) {
      x1 += 0.5;
      y1 += 0.5;
      x2 += 0.5;
      y2 += 0.5;
    }

    CGContextMoveToPoint(cg, x1, y1);
    CGContextAddLineToPoint(cg, x2, y2);
    
    wxMacSetCurrentTool(kPenTool);
    CGContextStrokePath(cg);
    wxMacSetCurrentTool(kNoTool);

    CGContextRestoreGState(cg);

    ReleaseCurrentDC();

    return;
  }

  dpx = (XLOG2DEVREL(current_pen->GetWidth()) >> 1);
  dpy = (YLOG2DEVREL(current_pen->GetWidth()) >> 1);

  SetCurrentDC();
  wxMacSetCurrentTool(kPenTool);
  wxMacDrawLine(XLOG2DEV(x1)-dpx, YLOG2DEV(y1)-dpy, XLOG2DEV(x2)-dpx, YLOG2DEV(y2)-dpy);
  ReleaseCurrentDC();
}

//-----------------------------------------------------------------------------
static void FillWithStipple(wxDC *dc, wxRegion *r, wxBrush *brush)
{
  double x, y, w, h, bw, bh;
  int xstart, xend, ystart, yend, i, j, ibw, ibh;
  wxRegion *old;
  int style;
  wxColour *c;
  wxBitmap *bm;

  old = dc->GetClippingRegion();
  if (old)
    r->Intersect(old);

  if (r->Empty())
    return;

  bm = brush->GetStipple();
  style = brush->GetStyle();
  c = brush->GetColour();

  r->BoundingBox(&x, &y, &w, &h);
  bw = bm->GetWidth();
  bh = bm->GetHeight();

  x = dc->LogicalToDeviceX(x);
  y = dc->LogicalToDeviceY(y);
  w = dc->LogicalToDeviceXRel(w);
  h = dc->LogicalToDeviceYRel(h);
  
  xstart = (int)floor(x / bw);
  xend = (int)floor((x + w + bw - 0.00001) / bw);

  ystart = (int)floor(y / bh);
  yend = (int)floor((y + h + bh - 0.00001) / bh);

  ibw = (int)floor(bw);
  ibh = (int)floor(bh);

  dc->SetClippingRegion(r);

  for (i = xstart; i < xend; i++) {
    for (j = ystart; j < yend; j++) {
      dc->Blit(dc->DeviceToLogicalX(i * ibw), 
               dc->DeviceToLogicalY(j * ibh), 
               dc->DeviceToLogicalXRel(ibw), 
               dc->DeviceToLogicalYRel(ibh),
               bm, 0, 0, style, c);
    }
  }

  dc->SetClippingRegion(old);
}

wxRegion *wxCanvasDC::BrushStipple()
{
  if (current_brush) {
    wxBitmap *bm;
    bm = current_brush->GetStipple();
    if (bm && bm->Ok())
      return new wxRegion(this);
  }
  return NULL;
}

void wxCanvasDC::PaintStipple(wxRegion *r)
{
  FillWithStipple(this, r, current_brush);
}

//-----------------------------------------------------------------------------
void wxCanvasDC::DrawArc(double x,double y,double w,double h,double start,double end)
{
  wxRegion *rgn;
  int xx, yy, xx2, yy2;
  int alpha1, alpha2;
  double degrees1, degrees2;
  Rect rect;

  if (!Ok() || !cMacDC) return;

  if (anti_alias) {
    CGContextRef cg;
    CGMutablePathRef path;
    CGAffineTransform xform;

    SetCurrentDC(TRUE);
    cg = GetCG();

    CGContextSaveGState(cg);

    start = (2 * wxPI) - start;
    end = (2 * wxPI) - end;

    path = CGPathCreateMutable();
    xform = CGAffineTransformScale(CGAffineTransformMakeTranslation(x, y), w, h);
    CGPathAddArc(path, &xform, 0.5, 0.5, 0.5, start, end, TRUE);

    if (current_brush && current_brush->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kBrushTool);
      CGContextBeginPath(cg);
      CGContextAddPath(cg, path);
      if ((end != 0) || (start != (2 * wxPI))) {
	CGContextAddLineToPoint(cg, x + w/2, y + h/2);
	CGContextClosePath(cg);
      }
      CGContextFillPath(cg);
    }
    if (current_pen && current_pen->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kPenTool);
      CGContextBeginPath(cg);
      CGContextAddPath(cg, path);
      CGContextStrokePath(cg);
    }
    wxMacSetCurrentTool(kNoTool);

    CGPathRelease(path);

    CGContextRestoreGState(cg);

    ReleaseCurrentDC();

    return;
  }
  
  if (start == end) {
    DrawEllipse(x, y, w, h);
    return;
  }

  if ((rgn = BrushStipple())) {
    rgn->SetArc(x, y, w, h, start, end);
    PaintStipple(rgn);
  }

  SetCurrentDC();

  xx = XLOG2DEV(x); yy = YLOG2DEV(y);
  xx2 = XLOG2DEV(x+w); yy2 = YLOG2DEV(y+h);
  
  degrees1 = start * RAD2DEG;
  degrees2 = end * RAD2DEG;
  
  /* Convert to QD angles for clockwise arc: */
   alpha1 = (int)(-degrees2 + 90) % 360;
  if (alpha1 < 0)
    alpha1 += 360;
  alpha2 = (int)(-degrees1 + 90) % 360;
  if (alpha2 < 0)
    alpha2 += 360;
  
  /* Alpha2 should be positive difference: */  
  alpha2 -= alpha1;
  if (alpha2 < 0)
    alpha2 += 360;

  rect.left = xx;
  rect.top = yy;
  rect.right = xx2;
  rect.bottom = yy2;
  OffsetRect(&rect,SetOriginX,SetOriginY);

  if (current_brush && current_brush->GetStyle() != wxTRANSPARENT) {
    if (!rgn) {
      wxMacSetCurrentTool(kBrushTool);
      if (paint_brush_with_erase)
	EraseArc(&rect, alpha1, alpha2);
      else
	PaintArc(&rect, alpha1, alpha2);
    }
  }
  if (current_pen && current_pen->GetStyle() != wxTRANSPARENT) {
    wxMacSetCurrentTool(kPenTool);
    FrameArc(&rect, alpha1, alpha2);
  }
  
  ReleaseCurrentDC();
}

//-----------------------------------------------------------------------------
void wxCanvasDC::DrawPoint(double x, double y)
     //-----------------------------------------------------------------------------
{
  if (!Ok() || !cMacDC) return;
  
  if (!current_pen || current_pen->GetStyle() == wxTRANSPARENT)
    return;

  SetCurrentDC();
  wxMacSetCurrentTool(kPenTool);
  wxMacDrawPoint(XLOG2DEV(x), YLOG2DEV(y));
  ReleaseCurrentDC();
}

//-----------------------------------------------------------------------------
void wxCanvasDC::DrawPolygon(int n, wxPoint points[],
			     double xoffset, double yoffset, int fillStyle)
{
  wxRegion *rgn;
  Point *xpoints1;
  int i, j;
  PolyHandle thePolygon;

  if (!Ok() || !cMacDC) return;
  
  if (n <= 0) return;

  if (anti_alias) {
    CGContextRef cg;
    CGMutablePathRef path;
    double pw;

    SetCurrentDC(TRUE);
    cg = GetCG();

    CGContextSaveGState(cg);

    pw = current_pen->GetWidthF();

    if ((anti_alias == 2)
	&& (user_scale_x == 1.0)
	&& (user_scale_y == 1.0)
	&& (pw <= 1.0)) {
      xoffset += 0.5;
      yoffset += 0.5;
    }
    
    path = CGPathCreateMutable();
    CGPathMoveToPoint(path, NULL, points[0].x + xoffset, points[0].y + yoffset);
    for (i = 1; i < n; i++) {
      CGPathAddLineToPoint(path, NULL, points[i].x + xoffset, points[i].y + yoffset);
    }
    CGPathCloseSubpath(path);

    if (current_brush && current_brush->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kBrushTool);
      CGContextBeginPath(cg);
      CGContextAddPath(cg, path);
      if (fillStyle == wxODDEVEN_RULE)
	CGContextEOFillPath(cg);
      else
	CGContextFillPath(cg);
    }
    if (current_pen && current_pen->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kPenTool);
      CGContextBeginPath(cg);
      CGContextAddPath(cg, path);
      CGContextStrokePath(cg);
    }
    wxMacSetCurrentTool(kNoTool);

    CGPathRelease(path);

    CGContextRestoreGState(cg);

    ReleaseCurrentDC();

    return;
  }

  if ((rgn = BrushStipple())) {
    rgn->SetPolygon(n, points, xoffset, yoffset, fillStyle);
    PaintStipple(rgn);
  }

  SetCurrentDC();

  xpoints1 = new Point[n+1];
  for (i = 0; i < n; i++) {
    xpoints1[i].h = XLOG2DEV(points[i].x + xoffset);
    xpoints1[i].v = YLOG2DEV(points[i].y + yoffset);
  }

  // Close figure
  xpoints1[n].h = xpoints1[0].h;
  xpoints1[n].v = xpoints1[0].v;

  thePolygon = OpenPoly();
  MoveTo(xpoints1[0].h + SetOriginX, xpoints1[0].v + SetOriginY);
  for (j = 1; j <= n; j++) {
    LineTo(xpoints1[j].h + SetOriginX, xpoints1[j].v + SetOriginY);
  }
  ClosePoly();

  if (current_brush && current_brush->GetStyle() != wxTRANSPARENT) {
    if (!rgn) {
      if (cMacCurrentTool != kBrushTool) wxMacSetCurrentTool(kBrushTool);
      if (paint_brush_with_erase)
	ErasePoly(thePolygon);
      else
	PaintPoly(thePolygon);
    }
  }

  if (current_pen && current_pen->GetStyle() != wxTRANSPARENT) {
    wxMacSetCurrentTool(kPenTool);
    FramePoly(thePolygon);
  }

  KillPoly(thePolygon);

  ReleaseCurrentDC();
}

//-----------------------------------------------------------------------------
void wxCanvasDC::DrawLines(int n, wxPoint points[], double xoffset, double yoffset)
{
  if (!Ok() || !cMacDC) return;
  
  if (n <= 0) return;

  if (!current_pen || current_pen->GetStyle() == wxTRANSPARENT)
    return;

  if (anti_alias) {
    CGContextRef cg;
    double pw;
    int i;

    pw = current_pen->GetWidthF();

    if ((anti_alias == 2)
	&& (user_scale_x == 1.0)
	&& (user_scale_y == 1.0)
	&& (pw <= 1.0)) {
      xoffset += 0.5;
      yoffset += 0.5;
    }

    SetCurrentDC(TRUE);
    cg = GetCG();

    CGContextSaveGState(cg);

    CGContextMoveToPoint(cg, points[0].x + xoffset, points[0].y + yoffset);
    for (i = 1; i < n; i++) {
      CGContextAddLineToPoint(cg, points[i].x + xoffset, points[i].y + yoffset);
    }
    
    wxMacSetCurrentTool(kPenTool);
    CGContextStrokePath(cg);
    wxMacSetCurrentTool(kNoTool);

    CGContextRestoreGState(cg);

    ReleaseCurrentDC();

    return;
  }

  {
    Point *xpoints;
    int i, j, dpx, dpy;
    PolyHandle thePolygon;

    SetCurrentDC();
    wxMacSetCurrentTool(kPenTool);
    
    xpoints = new Point[n];
      
    dpx = (XLOG2DEVREL(current_pen->GetWidth()) >> 1);
    dpy = (YLOG2DEVREL(current_pen->GetWidth()) >> 1);

    for (i = 0; i < n; i++) {
      xpoints[i].h = XLOG2DEV(points[i].x + xoffset);
      xpoints[i].v = YLOG2DEV(points[i].y + yoffset); // WCH: original mistype "h" for "v"
    }
      
    thePolygon = OpenPoly();
    MoveTo(xpoints[0].h + SetOriginX - dpx, xpoints[0].v + SetOriginY - dpy);
    for (j = 1; j < n; j++) {
      LineTo(xpoints[j].h + SetOriginX - dpx, xpoints[j].v + SetOriginY - dpy);
    }
    ClosePoly();
    
    FramePoly(thePolygon);
    
    KillPoly(thePolygon);
    
    ReleaseCurrentDC();
  }
}

//-----------------------------------------------------------------------------
void wxCanvasDC::DrawRectangle(double x, double y, double width, double height)
     //-----------------------------------------------------------------------------
{
  wxRegion *rgn;

  if (!Ok() || !cMacDC) return;

  if (anti_alias) {
    CGContextRef cg;

    SetCurrentDC(TRUE);
    cg = GetCG();

    CGContextSaveGState(cg);

    if (current_brush && current_brush->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kBrushTool);
      CGContextMoveToPoint(cg, x, y);
      CGContextAddLineToPoint(cg, x + width, y);
      CGContextAddLineToPoint(cg, x + width, y + height);
      CGContextAddLineToPoint(cg, x, y + height);
      CGContextClosePath(cg);
      CGContextFillPath(cg);
    }

    if (current_pen && current_pen->GetStyle() != wxTRANSPARENT) {
      if ((anti_alias == 2)
	  && (user_scale_x == 1.0)
	  && (user_scale_y == 1.0)) {
	x += 0.5;
	y += 0.5;
	width -= 1.0;
	height -= 1.0;
      }

      wxMacSetCurrentTool(kPenTool);
      CGContextMoveToPoint(cg, x, y);
      CGContextAddLineToPoint(cg, x + width, y);
      CGContextAddLineToPoint(cg, x + width, y + height);
      CGContextAddLineToPoint(cg, x, y + height);
      CGContextClosePath(cg);
      CGContextStrokePath(cg);
    }

    wxMacSetCurrentTool(kNoTool);

    CGContextRestoreGState(cg);

    ReleaseCurrentDC();

    return;
  }
  
  if ((rgn = BrushStipple())) {
    rgn->SetRectangle(x, y, width, height);
    PaintStipple(rgn);
  }

  SetCurrentDC();
  
  {
    int top = YLOG2DEV(y);
    int left = XLOG2DEV(x);
    int bottom = YLOG2DEV(y + height);
    int right = XLOG2DEV(x + width);
    Rect theRect = {top, left, bottom, right};
    OffsetRect(&theRect,SetOriginX,SetOriginY);
    if (current_brush && current_brush->GetStyle() != wxTRANSPARENT) {
      if (!rgn) {
	wxMacSetCurrentTool(kBrushTool);
	if (paint_brush_with_erase)
	  EraseRect(&theRect);
	else
	  PaintRect(&theRect);
      }
    }
    
    if (current_pen && current_pen->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kPenTool);
      FrameRect(&theRect);
    }
  }

  ReleaseCurrentDC();
}

//-----------------------------------------------------------------------------
void wxCanvasDC::DrawRoundedRectangle
(double x, double y, double width, double height, double radius)
{
  wxRegion *rgn;

  if (!Ok() || !cMacDC) return;

  if (radius < 0.0) {
    double w = width;
    if (height < w)
      w = height;
    radius = (-radius) * w;
  }
  
  if (anti_alias) {
    CGContextRef cg;

    SetCurrentDC(TRUE);
    cg = GetCG();

    CGContextSaveGState(cg);

    if ((anti_alias == 2)
	&& (user_scale_x == 1.0)
	&& (user_scale_y == 1.0)) {
      x += 0.5;
      y += 0.5;
      width -= 1.0;
      height -= 1.0;
    }
      
    if (current_brush && current_brush->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kBrushTool);
      CGContextMoveToPoint(cg, x + radius, y);
      CGContextAddLineToPoint(cg, x + width - radius, y);
      CGContextAddArc(cg, x + width - radius, y + radius, radius, 1.5 * wxPI, 2 * wxPI, FALSE);
      CGContextAddLineToPoint(cg, x + width, y + height - radius);
      CGContextAddArc(cg, x + width - radius, y + height - radius, radius, 0, 0.5 * wxPI, FALSE);
      CGContextAddLineToPoint(cg, x + radius, y + height);
      CGContextAddArc(cg, x + radius, y + height - radius, radius, 0.5 * wxPI, 1.0 * wxPI, FALSE);
      CGContextAddLineToPoint(cg, x, y + radius);
      CGContextAddArc(cg, x + radius, y + radius, radius, 1.0 * wxPI, 1.5 * wxPI, FALSE);
      CGContextClosePath(cg);
      CGContextFillPath(cg);
    }

    if (current_pen && current_pen->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kPenTool);
      CGContextMoveToPoint(cg, x + radius, y);
      CGContextAddLineToPoint(cg, x + width - radius, y);
      CGContextAddArc(cg, x + width - radius, y + radius, radius, 1.5 * wxPI, 2 * wxPI, FALSE);
      CGContextAddLineToPoint(cg, x + width, y + height - radius);
      CGContextAddArc(cg, x + width - radius, y + height - radius, radius, 0, 0.5 * wxPI, FALSE);
      CGContextAddLineToPoint(cg, x + radius, y + height);
      CGContextAddArc(cg, x + radius, y + height - radius, radius, 0.5 * wxPI, 1.0 * wxPI, FALSE);
      CGContextAddLineToPoint(cg, x, y + radius);
      CGContextAddArc(cg, x + radius, y + radius, radius, 1.0 * wxPI, 1.5 * wxPI, FALSE);
      CGContextClosePath(cg);
      CGContextStrokePath(cg);
    }

    wxMacSetCurrentTool(kNoTool);

    CGContextRestoreGState(cg);

    ReleaseCurrentDC();

    return;
  }
 
  if ((rgn = BrushStipple())) {
    rgn->SetRoundedRectangle(x, y, width, height, radius);
    PaintStipple(rgn);
  }

  SetCurrentDC();
  
  {
    int phys_radius = XLOG2DEVREL(radius);
    
    int phys_rwidth = phys_radius * 2;
    int phys_rheight = phys_rwidth;
    
    int top = YLOG2DEV(y);
    int left = XLOG2DEV(x);
    int bottom = YLOG2DEV(y + height);
    int right = XLOG2DEV(x + width);
    Rect theRect = {top, left, bottom, right};

    OffsetRect(&theRect,SetOriginX,SetOriginY);
    
    if (current_brush && current_brush->GetStyle() != wxTRANSPARENT) {
      if (!rgn) {
	wxMacSetCurrentTool(kBrushTool);
	if (paint_brush_with_erase)
	  EraseRoundRect(&theRect, phys_rwidth, phys_rheight);
	else
	  PaintRoundRect(&theRect, phys_rwidth, phys_rheight);
      }
    }

    if (current_pen && current_pen->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kPenTool);
      FrameRoundRect(&theRect, phys_rwidth, phys_rheight);
    }
  }

  ReleaseCurrentDC();
}

//-----------------------------------------------------------------------------
void wxCanvasDC::DrawEllipse(double x, double y, double width, double height)
{
  wxRegion *rgn;

  if (!Ok() || !cMacDC) return;
  
  if (anti_alias) {
    DrawArc(x, y, width, height, 0, 2 * wxPI);
    return;
  }

  if ((rgn = BrushStipple())) {
    rgn->SetEllipse(x, y, width, height);
    PaintStipple(rgn);
  }

  SetCurrentDC();
  
  {
    int top = YLOG2DEV(y);
    int left = XLOG2DEV(x);
    int bottom = YLOG2DEV(y + height);
    int right = XLOG2DEV(x + width);
    Rect theRect = {top, left, bottom, right};
    OffsetRect(&theRect,SetOriginX,SetOriginY);
    if (current_brush && current_brush->GetStyle() != wxTRANSPARENT) {
      if (!rgn) {
	wxMacSetCurrentTool(kBrushTool);
	if (paint_brush_with_erase)
	  EraseOval(&theRect);
	else
	  PaintOval(&theRect);
      }
    }

    if (current_pen && current_pen->GetStyle() != wxTRANSPARENT) {
      wxMacSetCurrentTool(kPenTool);
      FrameOval(&theRect);
    }
  }

  ReleaseCurrentDC();
}

static int noDCSet = 0; /* back door for GCBlit */

Bool wxCanvasDC::Blit(double xdest, double ydest, double width, double height,
		      wxBitmap *source, double xsrc, double ysrc, int rop, wxColour *c,
		      wxBitmap *mask)
{
  if (!Ok() || !cMacDC || !source->Ok()) return FALSE;
  if (mask && !mask->Ok()) return FALSE;

  if (!noDCSet) {
    SetCurrentDC();
  
    if (source->GetDepth() == 1) {
      wxMacSetCurrentTool(kColorBlitTool);
      if (rop == wxSOLID) BackColor(whiteColor);
      if (c)
	InstallColor(c, TRUE);
      else
	ForeColor(blackColor);
    } else {
      wxMacSetCurrentTool(kBlitTool);
      rop = wxSTIPPLE;
    }
  }

  {
    int mode;
    int ixsrc, iysrc, h, w, x, y;

    if (source->GetDepth() == 1) {
      switch (rop)
	{
	case wxXOR:  
	  mode = srcXor; 
	  break;
	case wxSOLID:
	  mode = srcOr;
	  break;
	case wxSTIPPLE: /* = opaque */
	default:
	  mode = srcCopy;
	  break;
	}
    } else
      mode = srcCopy;
    
    ixsrc = (int)floor(xsrc);
    iysrc = (int)floor(ysrc);
    
    if (ixsrc > source->GetWidth()) {
      if (!noDCSet)
	ReleaseCurrentDC();
      return TRUE;
    }
    if (iysrc > source->GetHeight()) {
      if (!noDCSet)
	ReleaseCurrentDC();
      return TRUE;
    }

    if (iysrc + height > source->GetHeight())
      height = source->GetHeight() - iysrc;
    if (ixsrc + width > source->GetWidth())
      width = source->GetWidth() - ixsrc;

    x = XLOG2DEV(xdest);
    y = YLOG2DEV(ydest);
    h = YLOG2DEV(height + ydest) - y;
    w = XLOG2DEV(width + xdest) - x;

    {
      Rect srcr = {iysrc, ixsrc, iysrc + (int)height, ixsrc + (int)width};
      Rect destr = {y, x, y+h, x+w };
      CGrafPtr theMacGrafPort;
      const BitMap *dstbm;
      const BitMap *srcbm;

      OffsetRect(&destr,SetOriginX,SetOriginY);
      
      theMacGrafPort = cMacDC->macGrafPort();

      dstbm = GetPortBitMapForCopyBits(theMacGrafPort);
      srcbm = GetPortBitMapForCopyBits(source->x_pixmap);
      
      if (mask) {
	const BitMap *maskbm;
	
	maskbm = GetPortBitMapForCopyBits(mask->x_pixmap);

	::CopyDeepMask(srcbm, maskbm, dstbm, &srcr, &srcr, &destr, mode, NULL);
      } else {
	::CopyBits(srcbm, dstbm, &srcr, &destr, mode, NULL);
      }
    }
  }

  if (!noDCSet)
    ReleaseCurrentDC();

  return TRUE;
}

Bool wxCanvasDC::GCBlit(double xdest, double ydest, double width, double height,
			wxBitmap *source, double xsrc, double ysrc)
{
  /* Non-allocating (i.e. no collectable allocation) Blit. Looks like
     the normal one will work, but we need to be careful about shifting the
     current drawing port. So we do the setup manually here and restore it
     completely. */
  Bool isok;
  CGrafPtr savep;
  GDHandle savegd;
  ThemeDrawingState state;
  long ox, oy;
  Rect clientRect = {-32767, -32767, 32767, 32767};
  CGrafPtr theMacGrafPort;
  RgnHandle rgn;

  ::GetGWorld(&savep, &savegd);  

  theMacGrafPort = cMacDC->macGrafPort();
  if (IsPortOffscreen(theMacGrafPort)) {
    ::SetGWorld(theMacGrafPort, NULL);
  } else {
    ::SetGWorld(theMacGrafPort, GetMainDevice());
  }

  ox = SetOriginX;
  oy = SetOriginY;
  SetOriginX = SetOriginY = 0;
  if (canvas) {
    wxArea *area;
    int aw, ah;
    area = canvas->ClientArea();
    area->FrameContentAreaOffset(&SetOriginX, &SetOriginY);
    aw = area->Width();
    ah = area->Height();
    ::SetRect(&clientRect, SetOriginX, SetOriginY, SetOriginX + aw, SetOriginY + ah);
  }

  GetThemeDrawingState(&state);

  noDCSet = 1;

  ForeColor(blackColor);
  BackColor(whiteColor);
  BackPat(GetWhitePattern());
  PenMode(patCopy);

  rgn = NewRgn();
  if (rgn) {
    GetClip(rgn);
    ::ClipRect(&clientRect);
  }

  isok = Blit(xdest, ydest, width, height, source, xsrc, ysrc, wxSTIPPLE, NULL);

  noDCSet = 0;

  if (rgn) {
    SetClip(rgn);
    DisposeRgn(rgn);
  }

  SetThemeDrawingState(state, TRUE);
  SetOriginX = ox;
  SetOriginY = oy;

  ::SetGWorld(savep, savegd);

  if (canvas)
    canvas->FlushDisplay();

  return isok;
}

void wxCanvasDC::TryColour(wxColour *src, wxColour *dest)
{
  if (!Ok() || !cMacDC) return;
  
  SetCurrentDC();

  if (Colour) {
    RGBColor pixel = src->pixel;
    
    Index2Color(Color2Index(&pixel), &pixel);
    
    dest->Set(pixel.red >> 8, pixel.green >> 8, pixel.blue >> 8); 
  } else {
    unsigned char red, blue, green;
    Bool isWhiteColour;

    red = src->Red();
    blue = src->Blue();
    green = src->Green();
    isWhiteColour =
      (red == (unsigned char )255 &&
       blue == (unsigned char)255 &&
       green == (unsigned char)255);
    if (isWhiteColour)
      dest->Set(255, 255, 255);
    else
      dest->Set(0, 0, 0);
  }

  ReleaseCurrentDC();
}
