#include "stdafx.h"
#include "Orchestrator.h"
#include "AutoMutex.h"
#include "SettingsStorage.h"
#include "CHttpHeaders.h"
#include "ITransactionProcessor.h"
#include <memory>

struct CallbackInfo
{
  DWORD dwContext;
  CString text;
  DWORD length;
  bool isSSL;
  bool isRequest; // if false then it is a response
  DWORD dwTickCount;
};

COrchestrator::COrchestrator(void) : m_settings(SettingsStorage::GetStorage())
{
  InitializeCriticalSection(&m_cs);
}

COrchestrator::~COrchestrator(void)
{
  DeleteCriticalSection(&m_cs);

  m_transactionProcessors.clear();
}

void COrchestrator::AddProcessor(ITransactionProcessor* processor)
{
  CAutoMutex mutex(m_cs);

  if (0 == m_transactionProcessors.size())
    CThreadWorker::Start();

  m_transactionProcessors.push_back(processor);
}

void COrchestrator::RemoveProcessor(ITransactionProcessor* processor)
{
  CAutoMutex mutex(m_cs);

  for (std::list<ITransactionProcessor*>::const_iterator i = m_transactionProcessors.begin();
       i != m_transactionProcessors.end(); ++i)
  {
    if (*i == processor)
    {
      m_transactionProcessors.erase(i);
      break;
    }
  }

  if (0 == m_transactionProcessors.size())
      CThreadWorker::Stop();
}


bool COrchestrator::CheckIntegrity(DWORD context, CHttpHeaders& headers, CString& text, std::map<DWORD, CString>& incompletes)
{
  //
  // check if request/response is complete
  //  - if not complete, check book-keeping where left-overs could be stored
  //
  if (headers.Parse((LPCTSTR)text, text.GetLength()))
    return true;

  //
  // request/response not complete
  //
  std::map<DWORD, CString>::const_iterator i = incompletes.find(context);

  if (i != incompletes.end())
  {
    text = i->second + text;
    incompletes.erase(i);
  }

  if (headers.Parse((LPCTSTR)text, text.GetLength()))
    return true;

  // headers are not complete, store what we have and wait for more
  if (CHttpHeaders::StartsAsHeader((LPCTSTR)text, text.GetLength()))
    incompletes.insert(std::map<DWORD, CString>::value_type(context, text));

  return false;

}

void COrchestrator::OnRequest(DWORD context, LPCTSTR text, DWORD length, bool isSSL)
{
  CallbackInfo* info = new CallbackInfo();
  info->dwContext = context;
  info->length = length;
  info->isRequest = true;
  info->isSSL = isSSL;
  info->text = CString(text, length);
  info->dwTickCount = ::GetTickCount();

  RunOperationInBackThread((DWORD)info);
}

void COrchestrator::OnResponse(DWORD context, LPCTSTR text, DWORD length, bool isSSL)
{
  CallbackInfo* info = new CallbackInfo();
  info->dwContext = context;
  info->length = length;
  info->isRequest = false;
  info->isSSL = isSSL;
  info->text = CString(text, length);
  info->dwTickCount = ::GetTickCount();

  RunOperationInBackThread((DWORD)info);
}

void COrchestrator::OnRunBackground(DWORD dwContext)
{
  std::auto_ptr<CallbackInfo> info((CallbackInfo*)dwContext);

  CAutoMutex mutex(m_cs);

  CString text(info->text, info->length);
  CHttpHeaders headers;

  if (info->isRequest)
  {
    if (CheckIntegrity(info->dwContext, headers, text, m_incompleteRequests))
    {
      for (std::list<ITransactionProcessor*>::const_iterator i = m_transactionProcessors.begin();
        i != m_transactionProcessors.end(); ++i)
      {
        (*i)->OnRequest(info->dwContext, text, headers, info->isSSL, info->dwTickCount);
      }
    }
  }
  else
  {
    if (CheckIntegrity(info->dwContext, headers, text, m_incompleteResponses))
    {
      bool shouldBeFiltered = m_settings.ShouldBeFiltered(headers);

      for (std::list<ITransactionProcessor*>::const_iterator i = m_transactionProcessors.begin();
        i != m_transactionProcessors.end(); ++i)
      {
        (*i)->OnResponse(info->dwContext, text, headers, info->isSSL, shouldBeFiltered, info->dwTickCount);
      }
    }

  }
}


