#include "stdafx.h"

#include "AddContentTypeDlg.h"

LRESULT CAddContentTypeDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  CenterWindow();

  CRichEditCtrl edit(GetDlgItem(IDC_EDIT_CONTENTTYPE));
  edit.SetFocus();

  return 0;
}

LRESULT CAddContentTypeDlg::OnBnClickedCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);

  return 0;
}

LRESULT CAddContentTypeDlg::OnBnClickedOk(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CRichEditCtrl edit(GetDlgItem(IDC_EDIT_CONTENTTYPE));

  edit.GetWindowText(m_strContentType);

  if (m_strContentType.GetLength() > 0)
  {
    EndDialog(wID);
  }

  return 0;
}

const CString& CAddContentTypeDlg::GetContentType() const
{
  return m_strContentType;
}