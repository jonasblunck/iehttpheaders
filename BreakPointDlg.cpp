#include "stdafx.h"
#include "BreakPointDlg.h"
#include "SettingsStorage.h"
#include "BreakPointsDlg.h"

CBreakPointDlg::CBreakPointDlg() : CDialogImpl<CBreakPointDlg>()
{
}

LRESULT CBreakPointDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow();

  m_RequestEditCtrl.SubclassWindow(GetDlgItem(IDC_REQUEST));
  m_ResponseEditCtrl.SubclassWindow(GetDlgItem(IDC_RESPONSE));

  m_RequestEditCtrl.SetText(m_strRequest, SettingsStorage::GetStorage().GetColOutPT());
  m_ResponseEditCtrl.SetText(m_strResponse, SettingsStorage::GetStorage().GetColOutPT());

  return 0;
}

void CBreakPointDlg::SetResponseText(const CString& strResponse)
{
  m_strResponse = strResponse;
  m_strResponse.Replace("\r\n", "\n");
  m_strResponse.Replace("\r", "\n");
  m_strResponse.Replace("\n", "\r\n");

  if (m_ResponseEditCtrl.IsWindow())
    m_ResponseEditCtrl.SetText(m_strResponse, SettingsStorage::GetStorage().GetColInPT());
}

void CBreakPointDlg::SetRequestText(const CString& strRequest)
{
  m_strRequest = strRequest;
  m_strRequest.Replace("\r\n", "\n");
  m_strRequest.Replace("\r", "\n");
  m_strRequest.Replace("\n", "\r\n");

  if (m_RequestEditCtrl.IsWindow())
    m_RequestEditCtrl.SetText(m_strRequest, SettingsStorage::GetStorage().GetColOutPT());
}

void CBreakPointDlg::Show()
{
  if (!IsWindow())
  {
    DoModal();
  }
  else
  {
    ShowWindow(SW_SHOW);
  }
}

LRESULT CBreakPointDlg::OnBreakPointsSettings(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CBreakPointsDlg dlg;
  
  dlg.DoModal();

  return 0;
}

LRESULT CBreakPointDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);

  return 0;
}

LRESULT CBreakPointDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);

  return 0;
}

