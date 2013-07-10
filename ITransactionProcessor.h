
#pragma once

class SettingsStorage;
class CHttpHeaders;

class ITransactionProcessor
{
public:
  ITransactionProcessor() {}
  virtual ~ITransactionProcessor() {}

  virtual void OnRequest(DWORD context, CString& request, CHttpHeaders& headers, bool isSSL, DWORD tickCount) = 0;
  virtual void OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL, bool shouldBeFiltered, DWORD tickCount) = 0;
};