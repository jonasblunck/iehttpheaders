#include "stdafx.h"

#include <algorithm>
#include "ContentFilterDlg.h"
#include "AddContentTypeDlg.h"
#include "resource.h"

CContentFilterDlg::CContentFilterDlg() : m_settings(SettingsStorage::GetStorage())
{
  std::list<CString> contentTypes = m_settings.GetContentTypes();

  for (std::list<CString>::const_iterator i = contentTypes.begin(); i != contentTypes.end(); ++i)
    m_lstContentType.push_back(*i);

  //m_psp.dwFlags |= PSP_USEICONID;
  //m_psp.pszIcon = MAKEINTRESOURCE(IDI_FILTER);
  //m_psp.hInstance = _Module.GetResourceInstance();
}

void CContentFilterDlg::AddContentTypeToList(LPCTSTR pszContentType, bool filterEnabled)
{
  int item = m_wndList.AddItem(0, 0, "");

  m_wndList.SetItemText(item, 1, pszContentType);
  m_wndList.SetCheckState(item, filterEnabled);
}


BOOL CContentFilterDlg::OnInitDialog(HWND hwndFocus, LPARAM lParam)
{
  CenterWindow();

  m_wndList.Attach(GetDlgItem(IDC_LIST_FILTER));
  m_wndList.SetExtendedListViewStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_SINGLESEL);

  m_wndList.InsertColumn ( 0, _T("Filter"), LVCFMT_LEFT, 50, 0 );
  m_wndList.InsertColumn ( 1, _T("Content-Type"), LVCFMT_LEFT, 240, 1 );

  PopulateList();

  return 1;
}

void CContentFilterDlg::PopulateList()
{
  std::list<CString> lst = m_settings.GetFilteredContentType();

  for (std::list<CString>::const_iterator t = m_lstContentType.begin(); 
       t != m_lstContentType.end(); ++t)
  {
    bool filtered = false;

    if (lst.end() != std::find(lst.begin(), lst.end(), (*t)))
      filtered = true;

    AddContentTypeToList(*t, filtered);
  }
}

void CContentFilterDlg::SaveSettings()
{
  std::list<CString> lstFilteredContentTypes;
  std::list<CString> lstContentTypes;

  int items = m_wndList.GetItemCount();

  for (int i = 0; i < items; ++i)
  {
    CString strContentType;
    m_wndList.GetItemText(i, 1, strContentType);
    
    lstContentTypes.push_back(strContentType);

    if (m_wndList.GetCheckState(i))
    {
      lstFilteredContentTypes.push_back(strContentType);
    }
  }

  m_settings.SetFilteredContentType(lstFilteredContentTypes);
  m_settings.SetAvailableContentTypes(lstContentTypes);
}

int CContentFilterDlg::OnApply()
{
  SaveSettings();

  return 1;
}

LRESULT CContentFilterDlg::OnBnClickedAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  CAddContentTypeDlg dlg;

  if (IDOK == dlg.DoModal())
  {
    m_lstContentType.push_back(dlg.GetContentType());
    
    m_wndList.DeleteAllItems();

    PopulateList();
  }

  return 0;
}

LRESULT CContentFilterDlg::OnBnClickedRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  if (m_wndList.GetSelectedCount() > 0)
  {
    m_wndList.DeleteItem(m_wndList.GetSelectionMark());
  }

  return 0;
}

