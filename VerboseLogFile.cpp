#include "stdafx.h"
#include "Verboselogfile.h"
#include "SettingsStorage.h"

VerboseLogFile::VerboseLogFile(void)
{
  m_hFile = INVALID_HANDLE_VALUE;
  m_bInitialized = false;
}

VerboseLogFile::~VerboseLogFile(void)
{
}

bool VerboseLogFile::Init()
{
  ATLASSERT(INVALID_HANDLE_VALUE == m_hFile);

  if (m_hFile != INVALID_HANDLE_VALUE)
    CloseHandle(m_hFile);

  m_hFile = CreateFile((LPCTSTR)SettingsStorage::GetStorage().GetVerboseLogfile(),GENERIC_READ|GENERIC_WRITE,
                       FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  if (INVALID_HANDLE_VALUE != m_hFile)
  {
    SetEndOfFile(m_hFile);

    return true;
  }

  return false;
}

void VerboseLogFile::Log(LPCTSTR pszText, int iLen)
{
  ATLASSERT(m_bInitialized);

  DWORD dummy;

  if (INVALID_HANDLE_VALUE != m_hFile)
    WriteFile(m_hFile, pszText, iLen, &dummy, NULL);
}

void VerboseLogFile::Close()
{
  if (INVALID_HANDLE_VALUE != m_hFile)
  {
    CloseHandle(m_hFile);
    m_hFile = INVALID_HANDLE_VALUE;
  }
}

VerboseLogFile& VerboseLogFile::GetLogFile()
{
  static VerboseLogFile s_Log;

  if (!s_Log.m_bInitialized)
  {
    s_Log.m_bInitialized = true;
    s_Log.Init();
  }
  
  return s_Log;
}