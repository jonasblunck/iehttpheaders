// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CSaveDlg :
  public CPropertyPageImpl<CSaveDlg>, 
  public CWinDataExchange<CSaveDlg>
{
public:
	enum { IDD = IDD_SAVESETTINGS };

	BEGIN_MSG_MAP(CSaveDlg)
    MSG_WM_INITDIALOG(OnInitDialog)
    CHAIN_MSG_MAP(CPropertyPageImpl<CSaveDlg>)
    COMMAND_HANDLER(IDC_CHOOSE_SAVE_FILENAME, BN_CLICKED, OnChooseSaveFilename)
  END_MSG_MAP()

  LRESULT OnChooseSaveFilename(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

  void SaveSettings();

public:
  BOOL OnInitDialog ( HWND hwndFocus, LPARAM lParam );
  int OnApply();
  
  CSaveDlg();

};

