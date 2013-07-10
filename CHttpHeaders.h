#pragma once

#include <atlstr.h>
#include <map>
#include <list>
#include <memory.h>

struct HttpHeader
{
  CString strHeader;
  CString strValue;
};

typedef std::list<HttpHeader*> HeaderList;

class CRefCountedHeaders
{
  LONG m_lRef;
  mutable HeaderList m_list;

public:

  CRefCountedHeaders();
  HeaderList& Headers() const;
  void Addref();
  void Release();
};

class CHttpHeaders
{
  CRefCountedHeaders* m_Headers;
  CString m_strVerb;
  int m_iHeadersLength;

  bool ExtractData(LPCTSTR pszText, int iMaxLen);
  void OnLine(const CString& rstrLine);
  static bool IsNewLine(LPTSTR psz, int iMaxLen);

public:
  CHttpHeaders(void);
  CHttpHeaders(const CHttpHeaders& headers);
  ~CHttpHeaders(void);

  CHttpHeaders& operator = (const CHttpHeaders& headers); 
  static bool StartsAsHeader(LPCTSTR pszText, int iMaxLen);

  HeaderList& GetHeaders();
  bool GetHeader(LPCTSTR pszHeader, CString& rstrValue) const;
  const CString& GetVerb() const;
  bool Parse(LPCTSTR pszText, int iMaxLen);
  int  GetHeadersLength() const;
  void GetHeadersFormated(CString& rstrHeadersOut);
  bool IsResponse() const;
  int  GetStatusCode() const;
};
