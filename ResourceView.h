#pragma once

#include <map>
#include "ITransactionProcessor.h"

class CResourceView : 
  public CWindowImpl<CResourceView, CTreeViewCtrl>,
  public ITransactionProcessor
{
  CRITICAL_SECTION m_cs;
  std::map<DWORD, HTREEITEM> m_contextParents;
  std::map<DWORD, DWORD> m_tickCounts;
  SettingsStorage& m_settings;
  CMenu m_menu;

  void OnClear();
  void OnSettings();

public:
  CResourceView(void);
  ~CResourceView(void);

  LRESULT OnRightUp(UINT ui, CPoint Pos);
  LRESULT OnRightButtonDown(UINT ui, CPoint pos);

  BEGIN_MSG_MAP(CResourceView)
    MSG_WM_RBUTTONDOWN(OnRightButtonDown)
    MSG_WM_RBUTTONUP(OnRightUp);
  END_MSG_MAP()

  virtual void OnRequest(DWORD context, CString& request, CHttpHeaders& headers, bool isSSL, DWORD tickCount);
  virtual void OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL, bool shouldBeFiltered, DWORD tickCount);

};
