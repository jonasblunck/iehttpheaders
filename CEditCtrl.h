#pragma once

class CEditCtrl;
typedef CWindowImpl<CEditCtrl, CRichEditCtrl> EditBase;

class CEditCtrl : public EditBase 
{
public:
  CEditCtrl();

  BEGIN_MSG_MAP(CEditCtrl)
  END_MSG_MAP()

  void SetText(const CString& text, COLORREF color);
};
