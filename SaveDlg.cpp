#include "stdafx.h"
#include "resource.h"
#include "SaveDlg.h"
#include "SettingsStorage.h"

CSaveDlg::CSaveDlg()
{
  //m_psp.dwFlags |= PSP_USEICONID;
  //m_psp.pszIcon = MAKEINTRESOURCE(IDI_SETTINGS);
  //m_psp.hInstance = _Module.GetResourceInstance();
}

BOOL CSaveDlg::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
  SettingsStorage& store = SettingsStorage::GetStorage();

  CheckDlgButton(IDC_CHECK_SAVE_TO_FILE, store.GetSaveToFile() ? BST_CHECKED : BST_UNCHECKED);

  CRichEditCtrl edit(GetDlgItem(IDC_EDIT_FILENAME));
  edit.SetWindowText(store.GetSaveFilename());

  return 1;
}

LRESULT CSaveDlg::OnChooseSaveFilename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  OPENFILENAME FileInfo;
  memset(&FileInfo, 0, sizeof(FileInfo));

  char szFileName[2048];
  szFileName[0] = 0;

  FileInfo.lStructSize = sizeof(FileInfo);
  FileInfo.lpstrFilter = ".txt";
  FileInfo.lpstrFile = szFileName;
  FileInfo.nMaxFile = sizeof(szFileName);
  FileInfo.Flags = OFN_EXPLORER;
  FileInfo.lpstrTitle = "Save as...";

  if (GetSaveFileName(&FileInfo))
  {
    CString filename(szFileName);

    if (-1 == filename.Find("."))
      filename += ".txt";

    CRichEditCtrl edit(GetDlgItem(IDC_EDIT_FILENAME));
    edit.SetWindowText(filename);
  }

  return 0;
}

void CSaveDlg::SaveSettings()
{
  CString filename;

  CRichEditCtrl edit(GetDlgItem(IDC_EDIT_FILENAME));
  edit.GetWindowText(filename);

  SettingsStorage& store = SettingsStorage::GetStorage();

  store.SetSaveToFile(BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_SAVE_TO_FILE));
  store.SetSaveFilename(filename);
}

int CSaveDlg::OnApply()
{
  SaveSettings();

  return 1;
}




