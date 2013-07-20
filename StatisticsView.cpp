#include "stdafx.h"
#include "CHttpHeaders.h"
#include "SettingsStorage.h"
#include "StatisticsView.h"
#include "AutoMutex.h"

#define ID_CLEAR    1

CStatisticsView::CStatisticsView(void) : m_settings(SettingsStorage::GetStorage())
{
  ::InitializeCriticalSection(&m_cs);
  if (m_menu.CreatePopupMenu())
  {
    m_menu.AppendMenu(MF_STRING, ID_CLEAR, (LPCTSTR)"Reset statistics...");
  }

  ClearCounters();
}

CStatisticsView::~CStatisticsView(void)
{
  ::DeleteCriticalSection(&m_cs);
}

void CStatisticsView::UpdateStats()
{
  DeleteAllItems();

  CString tmp;
  tmp.Format("Total response size: %ld", m_totalResponseSize);
  InsertItem((LPCSTR)tmp, 0, 0);

  tmp.Format("Total number of transactions: %ld", m_numTransactions);
  InsertItem((LPCSTR)tmp, 0, 0);

  CTimeSpan timeSpan = (m_lastResponseTime - m_firstRequestTime);

  //tmp.Format("Total time elapsed: %d", (int)timeSpan.GetTotalSeconds());
  //InsertItem((LPCSTR)tmp, 0, 0);

  tmp.Format("Max response size: %ld", m_maxResponseSize);
  InsertItem((LPCSTR)tmp, 0, 0);

  tmp.Format("Min response size: %ld", m_minResponseSize);
  InsertItem((LPCSTR)tmp, 0, 0);

  tmp.Format("Average response size: %ld", m_totalResponseSize / m_numTransactions);
  InsertItem((LPCSTR)tmp, 0, 0);
}

void CStatisticsView::OnRequest(DWORD context, CString& request, CHttpHeaders& headers, bool isSSL,
                              DWORD tickCount)
{
  CAutoMutex mutex(m_cs);

  m_numTransactions++;

  if (m_firstRequestTime == m_startOfTime)
    m_firstRequestTime = ::GetCurrentTime();
}

void CStatisticsView::OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL, 
                               bool shouldBeFiltered, DWORD tickCount)
{
  CAutoMutex mutex(m_cs);
  m_lastResponseTime = ::GetCurrentTime();

  CString contentLength;
  if (headers.GetHeader("Content-Length", contentLength))
  {
	  int responseSize = atoi((LPCSTR)contentLength);
	  m_totalResponseSize += responseSize;

	  if (0 == m_minResponseSize)
		  m_minResponseSize = responseSize;

	  if (responseSize > m_maxResponseSize)
		  m_maxResponseSize = responseSize;

	  if (responseSize < m_minResponseSize)
		  m_minResponseSize = responseSize;
  }

  UpdateStats();
}

void CStatisticsView::OnClear()
{
  CAutoMutex mutex(m_cs);

  DeleteAllItems();

  ClearCounters();
}

LRESULT CStatisticsView::OnRightButtonDown(UINT ui, CPoint pos)
{
  SetCapture();

  return 1;
}

LRESULT CStatisticsView::OnRightUp(UINT ui, CPoint Pos)
{
  if (m_menu.m_hMenu)
  {
    ClientToScreen(&Pos);
  
    int iCommand = m_menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY|TPM_RETURNCMD, Pos.x, Pos.y, m_hWnd);

    switch (iCommand)
    {
    case ID_CLEAR:
      OnClear();
      break;
    default:
      break;
     
    };

  }

  ReleaseCapture();

  return 1;
}
