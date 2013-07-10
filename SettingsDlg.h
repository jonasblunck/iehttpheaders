// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CBackCol.h"

class CSettingsDlg :
  public CPropertyPageImpl<CSettingsDlg>, 
  public CWinDataExchange<CSettingsDlg>
{
  CBackCol m_ColorIncomingPT;
  CBackCol m_ColorOutgoingPT;
  CBackCol m_ColorIncomingSSL;
  CBackCol m_ColorOutgoingSSL;

  BOOL UserSelectColor(COLORREF& rResult);
  void SaveSettings();

public:
	enum { IDD = IDD_SETTINGS };

	BEGIN_MSG_MAP(CSettingsDlg )
    MSG_WM_INITDIALOG(OnInitDialog)
    CHAIN_MSG_MAP(CPropertyPageImpl<CSettingsDlg>)
    COMMAND_ID_HANDLER(IDC_COL_IN_PT, OnColorInPT)
    COMMAND_ID_HANDLER(IDC_COL_OUT_PT, OnColorOutPT)
    COMMAND_ID_HANDLER(IDC_COL_IN_SSL, OnColorInSSL)
    COMMAND_ID_HANDLER(IDC_COL_OUT_SSL, OnColorOutSSL)
  END_MSG_MAP()

  LRESULT OnColorInPT(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnColorOutPT(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnColorInSSL(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnColorOutSSL(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
  BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
  int OnApply();
  
  CSettingsDlg();

};

