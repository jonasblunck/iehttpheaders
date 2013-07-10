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

class CAddContentTypeDlg : public CDialogImpl<CAddContentTypeDlg>
{
public:
	enum { IDD = IDD_DIALOG_ADD_CONTENTTYPE };

	BEGIN_MSG_MAP(CAddContentTypeDlg )
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_HANDLER(IDOK, BN_CLICKED, OnBnClickedOk)
    COMMAND_HANDLER(IDCANCEL, BN_CLICKED, OnBnClickedCancel)
  END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  const CString& GetContentType() const;

private:
  CString m_strContentType;

};

