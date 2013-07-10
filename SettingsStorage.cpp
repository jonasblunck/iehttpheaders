#include "stdafx.h"
#include "SettingsStorage.h"
#include "AutoMutex.h"
#include "patternmatch.h"
#include "CHttpHeaders.h"

#define REG_ROOT          "Software\\BlunckSoftware\\ieHTTPHeaders\\Settings"
#define KEY_COL_IN_PT     "ColorIncomingPT"
#define KEY_COL_OUT_PT    "ColorOutgoingPT"
#define KEY_COL_IN_SSL    "ColorIncomingSSL"
#define KEY_COL_OUT_SSL   "ColorOutgoingSSL"
#define KEY_REQ_HEADER    "ShowRequestHeader"
#define KEY_REQ_BODY      "ShowRequestBody"
#define KEY_RESP_HEADER   "ShowResponseHeader"
#define KEY_RESP_BODY     "ShowResponseBody"
#define KEY_RESP_SCROLL   "ScrollDuringCapture"
#define KEY_GROUP_REQ_RESP "GroupRequestResponse"
#define KEY_CONTENT_TYPES "ContentTypes"
#define KEY_SAVE_TO_FILE  "SaveToFile"
#define KEY_SAVE_FILENAME "SaveFilename"

SettingsStorage::SettingsStorage(void)
{
  m_colInPT = RGB(0, 0, 0);
  m_colInSSL = RGB(0, 0, 0);
  m_colOutPT = RGB(0, 0, 0);
  m_colOutSSL = RGB(0, 0, 0);

  m_bShowRequestBody = true;
  m_bShowRequestHeader = true;
  m_bShowResponseHeader = true;
  m_bShowResponseBody = false;
  m_bScrollDuringCapture = true;
  m_bGroupRequestResponse = false;
  m_bSaveToFile = false;

  InitializeCriticalSection(&m_cs);

  Load();
}

SettingsStorage::~SettingsStorage(void)
{
  DeleteCriticalSection(&m_cs);
}

void SettingsStorage::ParseAndPopulateContentTypes(CString& strPersistedContentTypes)
{
  m_lstContentTypes.clear();

  int nextQuote = strPersistedContentTypes.Find('\"');

  while (nextQuote >= 0)
  {
    int endQuote = strPersistedContentTypes.Find('\"', nextQuote + 1);

    CString strType = strPersistedContentTypes.Mid(nextQuote + 1, (endQuote - nextQuote - 1));
    m_lstContentTypes.push_back(strType);

    nextQuote = strPersistedContentTypes.Find('\"', endQuote + 1);
  }
}

bool SettingsStorage::Load()
{
  CAutoMutex am(m_cs);
  CRegKey Reg;

  if (ERROR_SUCCESS == Reg.Open(HKEY_CURRENT_USER, REG_ROOT, KEY_READ))
  {
    DWORD dwTmp = 0;
    CString strContentTypes;
    ULONG buffSize = 1023;

    Reg.QueryStringValue(KEY_CONTENT_TYPES, strContentTypes.GetBuffer(buffSize+1), &buffSize);
    strContentTypes.ReleaseBuffer();
    ParseAndPopulateContentTypes(strContentTypes);

    Reg.QueryStringValue(KEY_SAVE_FILENAME, m_saveFilename.GetBuffer(buffSize+1), &buffSize);
    m_saveFilename.ReleaseBuffer();
    
    if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_REQ_HEADER, dwTmp))
      m_bShowRequestHeader = (1 == dwTmp);

    if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_REQ_BODY, dwTmp))
      m_bShowRequestBody = (1 == dwTmp);

    if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_RESP_HEADER, dwTmp))
      m_bShowResponseHeader = (1 == dwTmp);

    if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_RESP_BODY, dwTmp))
      m_bShowResponseBody = (1 == dwTmp);

    if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_RESP_SCROLL, dwTmp))
      m_bScrollDuringCapture = (1 == dwTmp);

    if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_GROUP_REQ_RESP, dwTmp))
      m_bGroupRequestResponse = (1 == dwTmp);

    if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_SAVE_TO_FILE, dwTmp))
      m_bSaveToFile = (1 == dwTmp);

    if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_COL_IN_PT, m_colInPT))
      if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_COL_IN_SSL, m_colInSSL))
        if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_COL_OUT_PT, m_colOutPT))
          if (ERROR_SUCCESS == Reg.QueryDWORDValue(KEY_COL_OUT_SSL, m_colOutSSL))
            return true;

  }

  return false;
}

bool SettingsStorage::Save()
{
  CAutoMutex am(m_cs);

  CRegKey Reg;

  if (ERROR_SUCCESS == Reg.Open(HKEY_CURRENT_USER, REG_ROOT, KEY_READ|KEY_WRITE))
  {
    Reg.SetDWORDValue(KEY_REQ_HEADER, m_bShowRequestHeader ? 1 : 0);
    Reg.SetDWORDValue(KEY_REQ_BODY, m_bShowRequestBody ? 1 : 0);
    Reg.SetDWORDValue(KEY_RESP_HEADER, m_bShowResponseHeader ? 1 : 0);
    Reg.SetDWORDValue(KEY_RESP_BODY, m_bShowResponseBody ? 1 : 0);
    Reg.SetDWORDValue(KEY_RESP_SCROLL, m_bScrollDuringCapture ? 1 : 0);
    Reg.SetDWORDValue(KEY_GROUP_REQ_RESP, m_bGroupRequestResponse ? 1 : 0);
    Reg.SetDWORDValue(KEY_SAVE_TO_FILE, m_bSaveToFile ? 1 : 0);

    Reg.SetDWORDValue(KEY_COL_IN_PT, m_colInPT);
    Reg.SetDWORDValue(KEY_COL_OUT_PT, m_colOutPT);
    Reg.SetDWORDValue(KEY_COL_IN_SSL, m_colInSSL);
    Reg.SetDWORDValue(KEY_COL_OUT_SSL, m_colOutSSL);

    CString strContentTypes;
    for (std::list<CString>::const_iterator i = m_lstContentTypes.begin();
         i != m_lstContentTypes.end(); ++i)
    {
      CString strTmp;
      strTmp.Format("\"%s\" ", *i);
      strContentTypes += strTmp;
    }

    Reg.SetStringValue(KEY_CONTENT_TYPES, strContentTypes);
    Reg.SetStringValue(KEY_SAVE_FILENAME, m_saveFilename);

    return true;
  }

  return false;
}

std::list<CString> SettingsStorage::GetContentTypes() const
{
  CAutoMutex am(m_cs);

  return m_lstContentTypes;
}

void SettingsStorage::SetFilteredHttpCodes(std::list<int>& lst)
{
  CAutoMutex am(m_cs);

  m_lstFilteredHttpCodes.clear();

  for (std::list<int>::const_iterator i = lst.begin(); i != lst.end(); ++i)
    m_lstFilteredHttpCodes.push_back(*i);
}

void SettingsStorage::SetAvailableContentTypes(std::list<CString>& lstContentTypes)
{
  CAutoMutex am(m_cs);

  m_lstContentTypes.clear();

  for (std::list<CString>::const_iterator i = lstContentTypes.begin(); i != lstContentTypes.end(); ++i)
    m_lstContentTypes.push_back(*i); 
}

std::list<CString> SettingsStorage::GetFilteredContentType() const
{
  CAutoMutex am(m_cs);

  return m_lstFilteredContentType;
}

bool SettingsStorage::ShouldBeFiltered(const CString& strContentType) const
{
  CAutoMutex am(m_cs);

  for (std::list<CString>::const_iterator i = m_lstFilteredContentType.begin(); 
      i != m_lstFilteredContentType.end(); ++i)
  {
    if (pattern_match((LPCTSTR)strContentType, (LPCTSTR)*i))
      return true;
  }

  return false;
}

bool SettingsStorage::ShouldBeFiltered(int httpStatusCode) const
{
  CAutoMutex am(m_cs);

  for (std::list<int>::const_iterator i = m_lstFilteredHttpCodes.begin(); i != m_lstFilteredHttpCodes.end(); ++i)
  {
    if (httpStatusCode == *i)
      return true;
  }

  return false;
}

void SettingsStorage::SetFilteredContentType(std::list<CString>& lstFilteredContentType)
{
  CAutoMutex am(m_cs);

  m_lstFilteredContentType.clear();

  for (std::list<CString>::const_iterator i = lstFilteredContentType.begin();
       i != lstFilteredContentType.end(); ++i)
  {
    m_lstFilteredContentType.push_back(*i);
  }
}

COLORREF SettingsStorage::GetColInPT() const
{
  return m_colInPT;
}

COLORREF SettingsStorage::GetColInSSL() const
{
  return m_colInSSL;
}

COLORREF SettingsStorage::GetColOutPT() const
{
  return m_colOutPT;
}

COLORREF SettingsStorage::GetColOutSSL() const
{
  return m_colOutSSL;
}

bool SettingsStorage::GetGroupRequestResponse() const
{
  return m_bGroupRequestResponse;
}

void SettingsStorage::SetGroupRequestResponse(bool b)
{
  m_bGroupRequestResponse = b;
}

void SettingsStorage::SetColInPT(const COLORREF& rColor)
{
  m_colInPT = rColor;
}

void SettingsStorage::SetColInSSL(const COLORREF& rColor)
{
  m_colInSSL = rColor;
}

void SettingsStorage::SetColOutPT(const COLORREF& rColor)
{
  m_colOutPT = rColor;
}

void SettingsStorage::SetColOutSSL(const COLORREF& rColor)
{
  m_colOutSSL = rColor;
}

bool SettingsStorage::GetShowRequestHeader() const
{
  return m_bShowRequestHeader;
}

bool SettingsStorage::GetShowResponseBody() const
{
  return m_bShowResponseBody;
}

bool SettingsStorage::GetShowRequestBody() const
{
  return m_bShowRequestBody;
}

bool SettingsStorage::GetShowResponseHeader() const
{
  return m_bShowResponseHeader;
}

bool SettingsStorage::GetScrollDuringCapture() const
{
  return m_bScrollDuringCapture;
}

void SettingsStorage::SetShowRequestHeader(bool b)
{
  m_bShowRequestHeader = b;
}

void SettingsStorage::SetShowRequestBody(bool b)
{
  m_bShowRequestBody = b;
}

void SettingsStorage::SetShowResponseBody(bool b)
{
  m_bShowResponseBody = b;
}

void SettingsStorage::SetShowResponseHeader(bool b)
{
  m_bShowResponseHeader = b;
}

void SettingsStorage::SetScrollDuringCapture(bool b)
{
  m_bScrollDuringCapture = b;
}

void SettingsStorage::SetSaveToFile(bool b)
{
  m_bSaveToFile = b;
}

bool SettingsStorage::GetSaveToFile() const
{
  return m_bSaveToFile;
}

void SettingsStorage::SetSaveFilename(const CString& filename)
{
  m_saveFilename = filename;
}

CString SettingsStorage::GetSaveFilename() const
{
  return m_saveFilename;
}

SettingsStorage& SettingsStorage::GetStorage()
{
  static SettingsStorage s_Storage;

  return s_Storage;
}

bool SettingsStorage::ShouldBeFiltered(const CHttpHeaders& headers) const
{
  if (ShouldBeFiltered(headers.GetStatusCode()))
    return true;

  CString contentType;
  
  if (headers.GetHeader("Content-Type", contentType))
  {
    if (ShouldBeFiltered(contentType))
      return true;
  }

  return false;
}

