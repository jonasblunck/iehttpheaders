
#pragma once

#include "SettingsStorage.h"
#include "ITransactionProcessor.h"
#include <atlfile.h>

class CSaveProcessor :
  public ITransactionProcessor
{
  SettingsStorage& m_settings;
  CAtlFile m_file;
  bool m_open;

  void Process(const CString& text)
  {
    if (m_settings.GetSaveToFile())
    {
      if (!m_open)
      {   
        if (SUCCEEDED(
          m_file.Create(m_settings.GetSaveFilename(), GENERIC_READ|GENERIC_WRITE,
            FILE_SHARE_READ, CREATE_ALWAYS)))
        {
          m_file.Seek(FILE_END);
          m_open = true;
        }
      }

      if (m_open)
      {
        m_file.Write((LPVOID)(LPCTSTR)text, text.GetLength());
      }
    }

  }

public:
  CSaveProcessor(SettingsStorage& settings) : m_settings(settings), m_open(false) {}

  virtual void OnRequest(DWORD context, CString& request, CHttpHeaders& headers, bool isSSL, DWORD tickCount)
  {
   // Process(request);
  }

  virtual void OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL, bool shouldBeFiltered, DWORD tickCount)
  {
    //if (!shouldBeFiltered)
    //  Process(response);
  }

};