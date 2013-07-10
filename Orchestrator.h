#pragma once

#include <map>
#include <list>
#include "ThreadWorker.h"

class SettingsStorage;
class CHttpHeaders;
class ITransactionProcessor;

class COrchestrator :
  public CThreadWorker
{
  CRITICAL_SECTION m_cs;
  SettingsStorage& m_settings;
  std::map<DWORD, CString> m_incompleteRequests;
  std::map<DWORD, CString> m_incompleteResponses;
  std::list<ITransactionProcessor*> m_transactionProcessors;

  bool CheckIntegrity(DWORD context, CHttpHeaders& headers, CString& text, std::map<DWORD, CString>& incompletes);

public:
  COrchestrator(void);
  ~COrchestrator(void);

  void OnRequest(DWORD context, LPCTSTR text, DWORD length, bool isSSL);
  void OnResponse(DWORD context, LPCTSTR text, DWORD length, bool isSSL);

  void AddProcessor(ITransactionProcessor* processor);
  void RemoveProcessor(ITransactionProcessor* processor);

  virtual void OnRunBackground(DWORD dwContext);

};
