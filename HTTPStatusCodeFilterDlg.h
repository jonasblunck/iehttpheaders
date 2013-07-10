// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "SettingsStorage.h"

class CHTTPStatusCodeFilterDlg :
  public CPropertyPageImpl<CHTTPStatusCodeFilterDlg>, 
  public CWinDataExchange<CHTTPStatusCodeFilterDlg>
{
public:
  CHTTPStatusCodeFilterDlg();

	enum { IDD = IDD_FILTERHTTPCODES };

  // Message handlers
  BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
 
  // Property page notification handlers
  int OnApply();

	BEGIN_MSG_MAP(CHTTPStatusCodeFilterDlg )
    MSG_WM_INITDIALOG(OnInitDialog)
    CHAIN_MSG_MAP(CPropertyPageImpl<CHTTPStatusCodeFilterDlg>)
  END_MSG_MAP()

  BEGIN_DDX_MAP(CHTTPStatusCodeFilterDlg)
    DDX_CONTROL_HANDLE(IDC_LIST_FILTER_CODES, m_wndList)
    //DDX_RADIO(IDC_BLUE, m_nColor)
    //DDX_RADIO(IDC_ALYSON, m_nPicture)
  END_DDX_MAP()

private:
  CListViewCtrl m_wndList;
  SettingsStorage& m_settings;

  void PopulateList();
  void SaveSettings();
};

