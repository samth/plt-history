///////////////////////////////////////////////////////////////////////////////
// File:	wx_rbox.cc
// Purpose:	Panel item radioBox implementation (Macintosh version)
// Author:	Bill Hale
// Created:	1994
// Updated:	
// Copyright:  (c) 1993-94, AIAI, University of Edinburgh. All Rights Reserved.
///////////////////////////////////////////////////////////////////////////////

#include "wx_rbox.h"
#include "wx_rbut.h"
#include "wx_messg.h"
#include "wxMacDC.h"
#include "wx_stdev.h"
#include "wx_gdi.h"
#include "wx_area.h"
#include "wxBorderArea.h"

//-----------------------------------------------------------------------------
static void wxRadioButtonProc(wxRadioButton *radioButton, wxCommandEvent *event)
{
  wxPanel* radioPanel;
  wxWindow *rb;
  wxRadioBox* radioBox;
  wxCommandEvent *commandEvent;
  int radioButtonIndex;

  radioPanel = (wxPanel*)radioButton->GetParent();
  rb = radioPanel;
  while (wxSubType(rb->__type, wxTYPE_PANEL)) {
    rb = rb->GetParent();
  }
  radioBox = (wxRadioBox *)rb;
  radioButtonIndex = radioBox->cRadioButtons->MemberIndex(radioButton);
  radioBox->SetSelection(radioButtonIndex);

  commandEvent = new wxCommandEvent(wxEVENT_TYPE_RADIOBOX_COMMAND);
  radioBox->ProcessCommand(commandEvent);
}

//=============================================================================
// Public constructors
//=============================================================================

//-----------------------------------------------------------------------------
wxRadioBox::wxRadioBox // Constructor (given parentPanel, label choices)
(
 wxPanel*	parentPanel,
 wxFunction	function,
 char*		Title,
 int 		x,
 int			y,
 int			width,
 int			height,
 int			N,
 char**		Choices,
 int			majorDim,
 long		style,
 char*		windowName,
 WXTYPE		objectType
 ) : wxbRadioBox (parentPanel, function, Title, x, y, width, height, N, majorDim, style, windowName)
{
  wxPanel *buttonHolder;
  int i;
  
  cRadioButtons = new wxList(wxList::kNoDestroyData);

  Callback(function);

  cRadioPanel = new wxPanel(this->ClientArea(), 0, 0, 0, 0, 0);

  cRadioPanel->SetButtonFont(buttonFont);
  cRadioPanel->SetLabelFont(labelFont);

  buttonHolder = cRadioPanel;

  if (Title) {
    Title = wxItemStripLabel(Title);
    cRadioTitle = new wxMessage(cRadioPanel, Title, labelFont);
    if (labelPosition != wxVERTICAL) {
      buttonHolder = new wxPanel(cRadioPanel->ClientArea(), -1, -1, 0, 0, 0);
      cButtonHolder = buttonHolder;
    } else
      buttonHolder->NewLine();
  } else 
    cRadioTitle = NULL;

  for (i = 0; i < N; i++) {
    char *choice;
    wxRadioButton* radioButton;

    choice = wxItemStripLabel(Choices[i]);
    if (i && ((style & wxVERTICAL) == wxVERTICAL))
      buttonHolder->NewLine();
    radioButton = new wxRadioButton(buttonHolder, (wxFunction)wxRadioButtonProc, choice);
    cRadioButtons->Append(radioButton);
  }
  SetSelection(0);

  buttonHolder->Fit();
  if (buttonHolder != cRadioPanel) {
    cRadioPanel->Fit();
    cRadioTitle->Centre(wxVERTICAL);
  }

  if (style & wxBORDER) new wxBorderArea(this);

  if (width < 0 || height < 0)
    Fit();
	
  if (GetParent()->IsHidden())
    DoShow(FALSE);
  InitInternalGray();
}

//-----------------------------------------------------------------------------
wxRadioBox::wxRadioBox // Constructor (given parentPanel, bitmap choices)
(
 wxPanel*	parentPanel,
 wxFunction	function,
 char*		Title,
 int 		x,
 int			y,
 int			width,
 int			height,
 int			N,
 wxBitmap**	Choices,
 int			majorDim,
 long		style,
 char*		windowName,
 WXTYPE		objectType
 ) : wxbRadioBox (parentPanel, function, Title, x, y, width, height, N, majorDim, style, windowName)
{
  int i;
  wxPanel *buttonHolder;

  cRadioButtons = new wxList(wxList::kNoDestroyData);

  Callback(function);

  cRadioPanel = new wxPanel(this->ClientArea(), 0, 0, 0, 0, 0);
  cRadioPanel->SetButtonFont(buttonFont);
  cRadioPanel->SetLabelFont(labelFont);
	
  buttonHolder = cRadioPanel;
	
  if (Title) {
    cRadioTitle = new wxMessage(cRadioPanel, Title, labelFont);
    if (labelPosition != wxVERTICAL) {
      buttonHolder = new wxPanel(cRadioPanel->ClientArea(), -1, -1, 0, 0, 0);
      cButtonHolder = buttonHolder;
    } else
      buttonHolder->NewLine();
  } else
    cRadioTitle = NULL;

  for (i = 0; i < N; i++) {
    wxRadioButton* radioButton;
    if (i && ((style & wxVERTICAL) == wxVERTICAL))
      buttonHolder->NewLine();
    radioButton = new wxRadioButton(buttonHolder, (wxFunction)wxRadioButtonProc, Choices[i]);
    cRadioButtons->Append(radioButton);
  }
  SetSelection(0);
  
  buttonHolder->Fit();
  if (buttonHolder != cRadioPanel) {
    cRadioPanel->Fit();
    cRadioTitle->Centre(wxVERTICAL);
  }

  if (style & wxBORDER) new wxBorderArea(this);

  if (width < 0 || height < 0)
    Fit(); // WCH: need wxHorizontal and wxVertical for Fit(direction)

  if (GetParent()->IsHidden())
    DoShow(FALSE);
  InitInternalGray();
}

//=============================================================================
// Public destructor
//=============================================================================

//-----------------------------------------------------------------------------
wxRadioBox::~wxRadioBox(void)
{
  cRadioButtons->Clear();
}


//-----------------------------------------------------------------------------
void wxRadioBox::ChangeColour(void)
{
}

//-----------------------------------------------------------------------------
char* wxRadioBox::GetLabel(void)
{
  return (cRadioTitle ? cRadioTitle->GetLabel() : NULL);
}

//-----------------------------------------------------------------------------
void wxRadioBox::SetLabel(char* label)
{
  if (cRadioTitle) cRadioTitle->SetLabel(label);
}

//-----------------------------------------------------------------------------
char* wxRadioBox::GetLabel(int item)
{
  char* result = NULL;
  int numberItems;
  wxNode* node;
  wxRadioButton *radioButton;

  numberItems = cRadioButtons->Number();
  if (0 <= item && item < numberItems) {
    node = cRadioButtons->Nth(item);
    radioButton = (wxRadioButton*)node->Data();
    result = radioButton->GetLabel();
  }
  return result;
}

//-----------------------------------------------------------------------------
void wxRadioBox::SetLabel(int item, char* label)
{
  int numberItems;
  wxNode* node;
  wxRadioButton* radioButton;

  numberItems = cRadioButtons->Number();
  
  if (0 <= item && item < numberItems) {
    node = cRadioButtons->Nth(item);
    radioButton = (wxRadioButton*)node->Data();
    radioButton->SetLabel(label);
  }
}

//-----------------------------------------------------------------------------
void wxRadioBox::SetLabel(int item, wxBitmap* bitmap)
{
  int numberItems;
  wxNode* node;
  wxRadioButton* radioButton;

  numberItems = cRadioButtons->Number();

  if (0 <= item && item < numberItems) {
    node = cRadioButtons->Nth(item);
    radioButton = (wxRadioButton*)node->Data();
    radioButton->SetLabel(bitmap);
  }
}

//-----------------------------------------------------------------------------
int wxRadioBox::FindString(char* s)
{
  int result = -1, i;
  int numberItems;
  char* radioButtonLabel;

  numberItems = cRadioButtons->Number();

  for (i = 0; i < numberItems && result == -1; i++) {
    radioButtonLabel = GetLabel(i);
    if (strcmp(s, radioButtonLabel) == 0)
      result = i;
  }
  return result;
}

//-----------------------------------------------------------------------------
void wxRadioBox::SetSelection(int N)
{
  int numberItems;
  wxNode* selectedNode;
  wxRadioButton* selectedRadioButton;
  wxNode* node;
  wxRadioButton* radioButton;

  numberItems = cRadioButtons->Number();

  if (0 <= N && N < numberItems) {
    if (selected != N) {
      if (0 <= selected && selected < numberItems) {
	selectedNode = cRadioButtons->Nth(selected);
	selectedRadioButton = (wxRadioButton*)selectedNode->Data();
	selectedRadioButton->SetValue(FALSE);
      }
      
      node = cRadioButtons->Nth(N);
      radioButton = (wxRadioButton*)node->Data();
      radioButton->SetValue(TRUE);
      
      selected = N;
    }
  }
}

//-----------------------------------------------------------------------------
// Get selection
//-----------------------------------------------------------------------------
int wxRadioBox::GetSelection(void)
{
  return selected;
}

//-----------------------------------------------------------------------------
// Find string for position
//-----------------------------------------------------------------------------
char* wxRadioBox::GetString(int N) // WCH: duplicates GetLabel; so delete this
{
  return GetLabel(N);
}

//-----------------------------------------------------------------------------
void wxRadioBox::DoShow(Bool show)
{
  if (!CanShow(show)) return;

  if (show)
    wxWindow::DoShow(show);
  cRadioPanel->DoShow(show);	
  if (!show)
    wxWindow::DoShow(show);
}

//-----------------------------------------------------------------------------

void wxRadioBox::Enable(Bool enable)
{
  wxItem::Enable(enable);
}

void wxRadioBox::Enable(int item, Bool enable)
{
  int numberItems;
  wxNode* node;
  wxRadioButton* radioButton;

  numberItems = cRadioButtons->Number();
  if (0 <= item && item < numberItems) {
    node = cRadioButtons->Nth(item);
    radioButton = (wxRadioButton*)node->Data();
    radioButton->Enable(enable);
  }
}
 
//-----------------------------------------------------------------------------
void wxRadioBox::Show(int item, Bool show)
{
  int numberItems;
  wxNode* node;
  wxRadioButton* radioButton;

  numberItems = cRadioButtons->Number();
  
  if (0 <= item && item < numberItems) {
    node = cRadioButtons->Nth(item);
    radioButton = (wxRadioButton*)node->Data();
    radioButton->Show(show);
  }
}

//-----------------------------------------------------------------------------

int wxRadioBox::ButtonFocus(int)
{
  return -1;
}


void wxRadioBox::MaybeMoveControls()
{
  cRadioPanel->MaybeMoveControls();
  if (cButtonHolder)
    cButtonHolder->MaybeMoveControls();
}

void wxRadioBox::OnClientAreaDSize(int dW, int dH, int dX, int dY)
{
  wxNode* node;
  wxWindow* rbut;

  node = cRadioButtons->First();
  while (node) {
    rbut = (wxWindow*)node->Data();
    rbut->OnClientAreaDSize(dW, dH, dX, dY);
    node = node->Next();
  }

 wxWindow::OnClientAreaDSize(dW, dH, dX, dY);
}
