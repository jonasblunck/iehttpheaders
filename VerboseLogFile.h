#pragma once

class VerboseLogFile
{
protected:
  VerboseLogFile(void);
  ~VerboseLogFile(void);

  HANDLE m_hFile;
  bool m_bInitialized;

  bool Init();

public:
  static VerboseLogFile& GetLogFile();

  void Log(LPCTSTR pszText, int iLen);
  void Close();

};
