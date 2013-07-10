#include "stdafx.h"
#include "CEditCtrl.h"

CEditCtrl::CEditCtrl() : EditBase()
{
}


void CEditCtrl::SetText(const CString& text, COLORREF color)
{
  CHARFORMAT cf;
  ::memset(&cf, 0, sizeof(CHARFORMAT));
  cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_COLOR;
  cf.crTextColor = color;

  int iTotalLength = GetWindowTextLength();

  HideSelection(TRUE, TRUE);
  SetSel(iTotalLength, iTotalLength);
  SetSelectionCharFormat(cf);
  ReplaceSel((LPCTSTR)text);

  HideSelection(FALSE, TRUE);
}
