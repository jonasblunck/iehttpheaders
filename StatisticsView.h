#pragma once

#include <map>
#include "ITransactionProcessor.h"
#include <atltime.h>

class CStatisticsView : 
  public CWindowImpl<CStatisticsView, CTreeViewCtrl>,
  public ITransactionProcessor
{
  CRITICAL_SECTION m_cs;
  SettingsStorage& m_settings;
  CMenu m_menu;

  void OnClear();
  void UpdateStats();

  int m_averageResponseSize;
  int m_maxResponseSize;
  int m_totalResponseSize;
  int m_minResponseSize;
  int m_numTransactions;
  CTime m_firstRequestTime;
  CTime m_lastResponseTime;
  CTime m_startOfTime;

  void ClearCounters()
  {
	  m_startOfTime = CTime(1970, 01, 01);
      m_maxResponseSize = 0;
	  m_averageResponseSize = 0;
	  m_totalResponseSize = 0;
	  m_numTransactions = 0;
	  m_minResponseSize = 0;
	  m_maxResponseSize = 0;
	  m_firstRequestTime = m_startOfTime;
	  m_lastResponseTime = m_startOfTime;
  }

public:
  CStatisticsView(void);
  ~CStatisticsView(void);

  LRESULT OnRightUp(UINT ui, CPoint Pos);
  LRESULT OnRightButtonDown(UINT ui, CPoint pos);

  BEGIN_MSG_MAP(CStatisticsView)
    MSG_WM_RBUTTONDOWN(OnRightButtonDown)
    MSG_WM_RBUTTONUP(OnRightUp);
  END_MSG_MAP()

  virtual void OnRequest(DWORD context, CString& request, CHttpHeaders& headers, bool isSSL, DWORD tickCount);
  virtual void OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL, bool shouldBeFiltered, DWORD tickCount);

};
