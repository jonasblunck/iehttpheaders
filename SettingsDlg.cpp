#include "stdafx.h"

#include "SettingsDlg.h"
#include "SettingsStorage.h"
#include ".\settingsdlg.h"
#include "resource.h"

CSettingsDlg::CSettingsDlg()
{
  //m_psp.dwFlags |= PSP_USEICONID;
  //m_psp.pszIcon = MAKEINTRESOURCE(IDI_SETTINGS);
  //m_psp.hInstance = _Module.GetResourceInstance();
}

BOOL CSettingsDlg::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
  m_ColorIncomingPT.SubclassWindow(GetDlgItem(IDC_INCOMING_PLAINTEXT));
  m_ColorIncomingPT.SetBkColor(SettingsStorage::GetStorage().GetColInPT());

  m_ColorOutgoingPT.SubclassWindow(GetDlgItem(IDC_OUTGOING_PLAINTEXT));
  m_ColorOutgoingPT.SetBkColor(SettingsStorage::GetStorage().GetColOutPT());

  m_ColorIncomingSSL.SubclassWindow(GetDlgItem(IDC_INCOMING_SSL));
  m_ColorIncomingSSL.SetBkColor(SettingsStorage::GetStorage().GetColInSSL());

  m_ColorOutgoingSSL.SubclassWindow(GetDlgItem(IDC_OUTGOING_SSL));
  m_ColorOutgoingSSL.SetBkColor(SettingsStorage::GetStorage().GetColOutSSL());

  CheckDlgButton(IDC_CHECK_REQUEST_HEADERS, SettingsStorage::GetStorage().GetShowRequestHeader() ? BST_CHECKED : BST_UNCHECKED);
  CheckDlgButton(IDC_CHECK_REQUEST_BODY, SettingsStorage::GetStorage().GetShowRequestBody() ? BST_CHECKED : BST_UNCHECKED);
  CheckDlgButton(IDC_CHECK_RESPONSE_HEADERS, SettingsStorage::GetStorage().GetShowResponseHeader() ? BST_CHECKED : BST_UNCHECKED);
  CheckDlgButton(IDC_CHECK_RESPONSE_BODY, SettingsStorage::GetStorage().GetShowResponseBody() ? BST_CHECKED : BST_UNCHECKED);
  CheckDlgButton(IDC_CHECK_SCROLL, SettingsStorage::GetStorage().GetScrollDuringCapture() ? BST_CHECKED : BST_UNCHECKED);
  CheckDlgButton(IDC_CHECK_GROUP_REQ_RESP, SettingsStorage::GetStorage().GetGroupRequestResponse() ? BST_CHECKED : BST_UNCHECKED);

  return 1;
}

void CSettingsDlg::SaveSettings()
{
  SettingsStorage& store = SettingsStorage::GetStorage();

  store.SetColInPT(m_ColorIncomingPT.GetBkColor());
  store.SetColInSSL(m_ColorIncomingSSL.GetBkColor());
  store.SetColOutPT(m_ColorOutgoingPT.GetBkColor());
  store.SetColOutSSL(m_ColorOutgoingSSL.GetBkColor());

  store.SetShowRequestHeader(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_REQUEST_HEADERS));
  store.SetShowRequestBody(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_REQUEST_BODY));
  store.SetShowResponseHeader(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_RESPONSE_HEADERS));
  store.SetShowResponseBody(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_RESPONSE_BODY));
  store.SetScrollDuringCapture(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_SCROLL));
  store.SetGroupRequestResponse(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_GROUP_REQ_RESP));
}

int CSettingsDlg::OnApply()
{
  SaveSettings();

  return 1;
}

LRESULT CSettingsDlg::OnColorInPT(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  COLORREF rgbColor;

  if (UserSelectColor(rgbColor))
    m_ColorIncomingPT.SetBkColor(rgbColor);

  return 0;
}

LRESULT CSettingsDlg::OnColorOutPT(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  COLORREF rgbColor;

  if (UserSelectColor(rgbColor))
    m_ColorOutgoingPT.SetBkColor(rgbColor);

  return 0;
}

LRESULT CSettingsDlg::OnColorInSSL(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  COLORREF rgbColor;

  if (UserSelectColor(rgbColor))
    m_ColorIncomingSSL.SetBkColor(rgbColor);

  return 0;
}

LRESULT CSettingsDlg::OnColorOutSSL(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  COLORREF rgbColor;

  if (UserSelectColor(rgbColor))
    m_ColorOutgoingSSL.SetBkColor(rgbColor);

  return 0;
}

BOOL CSettingsDlg::UserSelectColor(COLORREF& rResult)
{
  CHOOSECOLOR colInfo;
  memset(&colInfo, 0, sizeof(colInfo));

  COLORREF Cols[16];
  //memset(&Cols, RGB(255, 255, 255), sizeof(Cols)); // we should remember these!

  colInfo.lStructSize = sizeof (colInfo);
  colInfo.hwndOwner = m_hWnd;
  colInfo.Flags = CC_ANYCOLOR;
  colInfo.hInstance = m_hWnd;
  colInfo.lpCustColors = Cols;

  BOOL b = ChooseColor(&colInfo);

  if (b)
    rResult = colInfo.rgbResult;

  return b;
}



