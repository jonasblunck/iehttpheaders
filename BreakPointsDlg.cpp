#include "stdafx.h"
#include "BreakPointsDlg.h"
#include "SettingsStorage.h"

LRESULT CBreakPointsDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow();

  CheckDlgButton(IDC_CHECK_BREAKNEXT, SettingsStorage::GetStorage().GetBreakOnNextTransaction() ? BST_CHECKED : BST_UNCHECKED);

  return 0;
}

LRESULT CBreakPointsDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);

  return 0;
}

LRESULT CBreakPointsDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  bool bBreakOnNext = (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_BREAKNEXT));

  SettingsStorage::GetStorage().SetBreakOnNextTransaction(bBreakOnNext);

  EndDialog(wID);

  return 0;
}

