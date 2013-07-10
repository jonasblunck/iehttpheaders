#pragma once

#include <list>
#include <atlstr.h>

using namespace ATL;
class CHttpHeaders;

class SettingsStorage
{
  CRITICAL_SECTION m_cs;

  COLORREF m_colInPT;
  COLORREF m_colOutPT;
  COLORREF m_colInSSL;
  COLORREF m_colOutSSL;

  bool m_bShowRequestHeader;
  bool m_bShowRequestBody;
  bool m_bShowResponseHeader;
  bool m_bShowResponseBody;
  bool m_bScrollDuringCapture;
  bool m_bGroupRequestResponse;

  bool m_bBreakOnNext;
  bool m_bSaveToFile;
  CString m_saveFilename;

  std::list<CString> m_lstFilteredContentType;
  std::list<CString> m_lstContentTypes;
  std::list<int> m_lstFilteredHttpCodes;

  void ParseAndPopulateContentTypes(CString& strPersistedContentTypes);
  SettingsStorage(void);
  ~SettingsStorage(void);

public:
  static SettingsStorage& GetStorage();

  COLORREF GetColInPT() const;
  COLORREF GetColInSSL() const;
  COLORREF GetColOutPT() const;
  COLORREF GetColOutSSL() const;

  void SetColInPT(const COLORREF& rColor);
  void SetColInSSL(const COLORREF& rColor);
  void SetColOutPT(const COLORREF& rColor);
  void SetColOutSSL(const COLORREF& rColor);

  bool GetShowRequestHeader() const;
  bool GetShowRequestBody() const;
  bool GetShowResponseHeader() const;
  bool GetShowResponseBody() const;
  bool GetScrollDuringCapture() const;
  bool GetGroupRequestResponse() const;

  void SetShowRequestHeader(bool b);
  void SetShowRequestBody(bool b);
  void SetShowResponseHeader(bool b);
  void SetShowResponseBody(bool b);
  void SetScrollDuringCapture(bool b);
  void SetGroupRequestResponse(bool b);

  void SetSaveToFile(bool b);
  void SetSaveFilename(const CString& filename);
  CString GetSaveFilename() const;

  std::list<CString> GetFilteredContentType() const;
  std::list<CString> GetContentTypes() const;

  bool ShouldBeFiltered(const CHttpHeaders& headers) const;
  bool ShouldBeFiltered(int httpStatusCode) const;
  bool ShouldBeFiltered(const CString& strContentType) const;
  void SetFilteredContentType(std::list<CString>& lstFilteredContentType);
  void SetAvailableContentTypes(std::list<CString>& lstContentTypes);
  void SetFilteredHttpCodes(std::list<int>& lstFilteredCodes);
  bool GetSaveToFile() const;

  bool Load();
  bool Save();

  // Temp
  void SetBreakOnNextTransaction(bool bBreak)
  {
    m_bBreakOnNext = bBreak;
  }

  bool GetBreakOnNextTransaction() const
  {
    return m_bBreakOnNext;
  }
};
