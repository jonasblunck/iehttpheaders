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
#include <list>
#include "SettingsStorage.h"

class CContentFilterDlg : 
    public CPropertyPageImpl<CContentFilterDlg>, 
    public CWinDataExchange<CContentFilterDlg>
{
public:
  CContentFilterDlg();

	enum { IDD = IDD_FILTERSETTINGS };

	BEGIN_MSG_MAP(CContentFilterDlg )
    MSG_WM_INITDIALOG(OnInitDialog)
    CHAIN_MSG_MAP(CPropertyPageImpl<CContentFilterDlg>)
    COMMAND_HANDLER(IDC_BUTTONADD, BN_CLICKED, OnBnClickedAdd)
    COMMAND_HANDLER(IDC_BUTTON_REMOVE, BN_CLICKED, OnBnClickedRemove)
  END_MSG_MAP()

  LRESULT OnBnClickedAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBnClickedRemove(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
  int OnApply();

private:
  CListViewCtrl m_wndList;
  std::list<CString> m_lstContentType;
  SettingsStorage& m_settings;

  void AddContentTypeToList(LPCTSTR pszContentType, bool filterEnabled);
  void PopulateList();
  void SaveSettings();
};

