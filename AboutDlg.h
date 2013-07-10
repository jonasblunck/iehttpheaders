// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "resource.h"

#include <AtlApp.h>
#include <AtlWin.h>
#include <atlcrack.h>
#include <AtlCtrls.h>
#include <atlmisc.h>
#include <atlctrlx.h>

class CAboutDlg : 
  public CPropertyPageImpl<CAboutDlg>
  //public CWinDataExchange<CAboutDlg>

{
public:

  CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

  BEGIN_MSG_MAP(CAboutDlg)
    MSG_WM_INITDIALOG(OnInitDialog)
    CHAIN_MSG_MAP(CPropertyPageImpl<CAboutDlg>)
  END_MSG_MAP()
 
  // Message handlers
  BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
 
  // Property page notification handlers
  int OnApply();

private:
	CHyperLink m_hlBlunck;

	void CenterHyperLinkLabel(CHyperLink& hl);
};

