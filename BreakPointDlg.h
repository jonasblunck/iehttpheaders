// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "settingsstorage.h"
#include "CEditCtrl.h"
#include "resource.h"

class CBreakPointDlg : public CDialogImpl<CBreakPointDlg>
{
public:
	enum { IDD = IDD_BREAKPOINT };

	BEGIN_MSG_MAP(CBreakPointDlg )
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
    COMMAND_HANDLER(ID_BREAKPOINT_SETTINGS, BN_CLICKED, OnBreakPointsSettings)
    COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
  END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBreakPointsSettings(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  CBreakPointDlg();
  void SetResponseText(const CString& strResponse);
  void SetRequestText(const CString& strRequest);

  void Show();

private:
  CString m_strRequest;
  CString m_strResponse;
  CEditCtrl m_RequestEditCtrl;
  CEditCtrl m_ResponseEditCtrl;
  
};

