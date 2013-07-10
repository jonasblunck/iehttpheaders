#include "stdafx.h"

#include <algorithm>
#include "HTTPStatusCodeFilterDlg.h"
#include "resource.h"

#define NELM(x) sizeof(x) / sizeof(x[0])

CHTTPStatusCodeFilterDlg::CHTTPStatusCodeFilterDlg() : m_settings(SettingsStorage::GetStorage())
{
  std::list<CString> contentTypes = m_settings.GetContentTypes();

  //m_psp.dwFlags |= PSP_USEICONID;
  //m_psp.pszIcon = MAKEINTRESOURCE(IDI_FILTER);
  //m_psp.hInstance = _Module.GetResourceInstance();
}

BOOL CHTTPStatusCodeFilterDlg::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
  m_wndList.Attach(GetDlgItem(IDC_LIST_FILTER_CODES));
  m_wndList.SetExtendedListViewStyle(LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_SINGLESEL);

  m_wndList.InsertColumn ( 0, _T("Filter"), LVCFMT_LEFT, 40, 0 );
  m_wndList.InsertColumn ( 1, _T("Status code"), LVCFMT_LEFT, 70, 1 );
  m_wndList.InsertColumn ( 2, _T("Description"), LVCFMT_LEFT, 370, 1 );

  PopulateList();

  return 1;
}

void CHTTPStatusCodeFilterDlg::PopulateList()
{
  static struct codes
  {
    int code;
    CString description;
  } s_codes[] =
  {
    { 100, "Continue. The client should continue with its request" },
    { 200, "OK. The request has succeeded." },
    { 201, "Created. The request has been fulfilled and resulted in a new resource." },
    { 206, "Partial Content. The server has fulfilled the partial GET request for the resource." },
    { 301, "Moved permanently. The requested resource has been assigned a new permanent URI and any future references to this resource should use one of the returned URIs." },
    { 302, "Found. The requested resource resides temporarily under a different URI." },
    { 304, "Not modified. The client has performed a conditional GET request and access is allowed, but the document has not been modified." },
    { 307, "Temporary Redirect. The requested resource resides temporarily under a different URI." },
    { 400, "Bad request. The request could not be understood by the server due to malformed syntax." },
    { 401, "Unauthorized. The request requires user authentication." },
    { 403, "Forbidden. The server understood the request, but is refusing to fulfill it."},
    { 404, "Not found. The server has not found anything matching the Request-URI. "},
    { 407, "Error; proxy authentication required"},
    { 408, "Request timeout. The client did not produce a request within the time that the server was prepared to wait." },
    { 500, "Internal server error. The server encountered an unexpected condition which prevented it from fulfilling the request."},
    { 501, "Not implemented. The server does not support the functionality required to fulfill the request. "},
    { 503, "Service Unavailable. The server is currently unable to handle the request due to a temporary overloading or maintenance of the server." },
  };

  for (int i = (NELM(s_codes) -1 ); i >= 0; --i)
  {
    CString strCode;
    strCode.Format("%d", s_codes[i].code);

    CString strDescription = s_codes[i].description;

    int item = m_wndList.AddItem(0, 0, "");

    m_wndList.SetItemText(item, 1, strCode);
    m_wndList.SetItemText(item, 2, strDescription);

    if (m_settings.ShouldBeFiltered(s_codes[i].code))
    {
      m_wndList.SetCheckState(item, TRUE);
    }
  }
}

int CHTTPStatusCodeFilterDlg::OnApply()
{
  SaveSettings();

  return 1;
}

void CHTTPStatusCodeFilterDlg::SaveSettings()
{
  std::list<int> lstFilteredCodes;

  int items = m_wndList.GetItemCount();

  for (int i = 0; i < items; ++i)
  {
    if (m_wndList.GetCheckState(i))
    {
      CString strCode;
      m_wndList.GetItemText(i, 1, strCode);

      int code = atoi((LPCTSTR)strCode);

      lstFilteredCodes.push_back(code);
    }
  }

  m_settings.SetFilteredHttpCodes(lstFilteredCodes);
}

  