
#pragma once

#include "SettingsStorage.h"
#include "AboutDlg.h"
#include "SettingsDlg.h"
#include "ContentFilterDlg.h"
#include "HTTPStatusCodeFilterDlg.h"
#include "SaveDlg.h"

class CTabbedSettings :
  public CPropertySheetImpl<CTabbedSettings>
{
  CAboutDlg m_about;
  CHTTPStatusCodeFilterDlg m_statusCodeFilter;
  CSettingsDlg m_settings;
  CContentFilterDlg m_filterType;
//  CSaveDlg m_saveDlg;

public:

  CTabbedSettings() : CPropertySheetImpl<CTabbedSettings>(_T("ieHTTPHeaders settings"), 0, 0)
  {
    SettingsStorage::GetStorage().Load();

    AddPage ( m_settings );
//    AddPage ( m_saveDlg );
    AddPage ( m_statusCodeFilter );
    AddPage ( m_filterType );
    AddPage ( m_about );

    m_psh.dwFlags |= PSH_NOAPPLYNOW|PSH_NOCONTEXTHELP;
  }
};
