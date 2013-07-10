#include "stdafx.h"
#include "CHttpHeaders.h"
#include "HttpRequestInfo.h"

struct _HeaderDeclaration
{
  char szDeclaration[32];
} s_HeaderDecl [] = 
{
  { "CONNECT" },
  { "GET" },
  { "POST" },
  { "HEAD" },
  { "PUT" },
  { "LINK" },
  { "UNLINK" },
  { "DELETE" },
  { "OPTIONS" },
  { "TRACE" },
  { "HTTP" }, // responses
};

#ifndef NELM
#define NELM(x) sizeof(x) / sizeof(x[0])
#endif

// CRefCountedHeaders

CRefCountedHeaders::CRefCountedHeaders()
{
  m_lRef = 1;
}

HeaderList& CRefCountedHeaders::Headers() const
{
  return m_list;
}

void CRefCountedHeaders::Addref()
{
  ::InterlockedIncrement(&m_lRef);
}

void CRefCountedHeaders::Release()
{
  LONG l = ::InterlockedDecrement(&m_lRef);

  if (0 == l)
    delete this;
}

//
// CHttpHeaders
//

CHttpHeaders::CHttpHeaders(void)
{
  m_iHeadersLength = 0;
  m_Headers = 0;
}

CHttpHeaders::CHttpHeaders(const CHttpHeaders& headers)
{
  *this = headers;
}

CHttpHeaders& CHttpHeaders::operator = (const CHttpHeaders& headers)
{
  m_strVerb = headers.m_strVerb;
  m_iHeadersLength = headers.m_iHeadersLength;

  if (headers.m_Headers)
  {
    m_Headers = headers.m_Headers;
    headers.m_Headers->Addref();
  }

  return *this;
}

CHttpHeaders::~CHttpHeaders(void)
{
  if (m_Headers)
    m_Headers->Release();
}

bool CHttpHeaders::IsResponse() const
{
  if (m_strVerb.Find("HTTP") == 0)
    return true;

  return false;
}

int CHttpHeaders::GetStatusCode() const
{
  ATLASSERT(IsResponse());

  if (m_strVerb.Find("HTTP") == 0)
  {
    int versionDelim = m_strVerb.Find("/");

    if (-1 != versionDelim)
    {
      int start = m_strVerb.Find(" ", versionDelim + 1);
      int end = m_strVerb.Find(" ", start + 1);

      if (start > 0 && end > 0)
      {
        CString code = m_strVerb.Mid(start, end - start);

        return atoi((LPCTSTR)code);
      }
    }
  }

  return 0xffffffff;
}

const CString& CHttpHeaders::GetVerb() const
{
  return m_strVerb;
}

bool CHttpHeaders::IsNewLine(LPTSTR psz, int iMaxLen)
{
  ATLASSERT(psz);

  if (iMaxLen)
  {
    int iSearchLen = (iMaxLen >= 2) ? 2 : 1;

    if (iSearchLen >= 2)
      if (psz[0] == '\r' && psz[1] == '\n')
        return true;
  }
    
  return false;
}

void CHttpHeaders::OnLine(const CString& rstrLine)
{
  for (int i = 0; i < NELM(s_HeaderDecl) && m_strVerb.IsEmpty(); ++i)
  {
    if (rstrLine.Find(s_HeaderDecl[i].szDeclaration) == 0)
    {
      m_strVerb = rstrLine;
      return;
    }
  }

  // try to find header seperator
  int iPos = rstrLine.Find(':');

  CString strHeader = rstrLine.Left(iPos);
  CString strHeaderValue = rstrLine.Mid(iPos + 1);
  strHeaderValue.Trim();

  HttpHeader* pHeader = new HttpHeader();
  pHeader->strHeader = strHeader;
  pHeader->strValue = strHeaderValue;

  m_Headers->Headers().push_back(pHeader);
}

int CHttpHeaders::GetHeadersLength() const
{
  return m_iHeadersLength;
}

HeaderList& CHttpHeaders::GetHeaders()
{
  return m_Headers->Headers();
}

bool CHttpHeaders::GetHeader(LPCTSTR pszHeader, CString& rstrValue) const
{
  CString strFind(pszHeader);
  strFind.MakeLower();

  for (HeaderList::const_iterator i = 
        m_Headers->Headers().begin();
         i != m_Headers->Headers().end(); ++i)
  {
    HttpHeader* pHeaderInfo = *i;
    
    CString strHeader(pHeaderInfo->strHeader);
    strHeader.MakeLower();

    if (strHeader == strFind)
    {
      rstrValue = (*i)->strValue;

      return true;
    }
  }
  
  return false;
}

void CHttpHeaders::GetHeadersFormated(CString& rstrHeadersOut)
{
  rstrHeadersOut = m_strVerb;

  for (HeaderList::iterator i = m_Headers->Headers().begin(); i != m_Headers->Headers().end(); ++i)
  {
    CString str;
    str.Format("\n%s: %s", (LPCTSTR)(*i)->strHeader, (LPCTSTR)(*i)->strValue);
    
    rstrHeadersOut += str;
  }
}

bool CHttpHeaders::ExtractData(LPCTSTR pszText, int iMaxLen)
{
  ATLASSERT(pszText && iMaxLen);

  LPTSTR psz = (LPTSTR)pszText;
  LPTSTR pszLastNewLinePosition = NULL;
  CString strLine;

  while (psz && *psz && (psz - pszText < iMaxLen))
  {
    // extract each line
    if (!IsNewLine(psz, (iMaxLen - (psz - pszText))))
      strLine += *psz++;
    else
    { 
      // we've got a new line!
      ATLASSERT('\r' == *psz || '\n' == *psz);
      ATLASSERT(2 <= (iMaxLen - (psz - pszText))); // there must be 2 chars left at least!
      ATLASSERT('\r' == psz[0] && '\n' == psz[1]);

      if (psz == pszLastNewLinePosition)
      {
        // but we are done parsing :)
        m_iHeadersLength = (psz - pszText) + 2;
        return true;
      }

      ++psz;
      ++psz; // is done in the end of the while-loop
      pszLastNewLinePosition = psz; 

      OnLine(strLine);

      strLine.Empty();
    }
  }

  return false;
}

bool CHttpHeaders::StartsAsHeader(LPCTSTR pszText, int iMaxLen)
{
  CString str(pszText, (iMaxLen >= 10) ? 10 : iMaxLen);

  for (int i = 0; i < NELM(s_HeaderDecl); ++i)
  {
    if (0 == str.Find(s_HeaderDecl[i].szDeclaration))
      return true;
  }

  return false;
}

// CHttpHeaders::Parser
// Comment: Parse and verify (check if body is has been received)
/////////////////////////////////////////////////////////////////////
bool CHttpHeaders::Parse(LPCTSTR pszText, int iMaxLen)
{
  if (NULL != m_Headers)
  {
    m_Headers->Release();
    m_strVerb.Empty();
    m_iHeadersLength = 0;
  }

  m_Headers = new CRefCountedHeaders();

  if (pszText && iMaxLen && StartsAsHeader(pszText, iMaxLen))
  {
    if (ExtractData(pszText, iMaxLen))
    {
      CHttpRequestInfo requestInfo(*this);
      int iBodyLength = 0;

      if (requestInfo.GetContentLength(iBodyLength))
      {
        if ((GetHeadersLength() + iBodyLength) > iMaxLen)
          return false;
      }

      return true;
    }
  }

  return false;
}
