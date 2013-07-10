#include "stdafx.h"
#include ".\httprequestinfo.h"

#define HEADER_CONTENT_LENGTH  "Content-Length"

CHttpRequestInfo::CHttpRequestInfo(CHttpHeaders& rHeaders) : m_rHeaders(rHeaders)
{
}

CHttpRequestInfo::~CHttpRequestInfo(void)
{
}

bool CHttpRequestInfo::GetContentLength(int& riBytes)
{
  CString strHeaderValue;

  if (m_rHeaders.GetHeader(HEADER_CONTENT_LENGTH, strHeaderValue))
  {
    strHeaderValue.Trim();
    CString strBytes;

    for (int i = 0; i < strHeaderValue.GetLength(); ++i)
    {
      if (strHeaderValue[i] < '0' || strHeaderValue[i] > '9')
        break;

      strBytes += strHeaderValue[i];
    }

    riBytes = atoi((LPCTSTR)strBytes);

    return true;
  }

  return false;
}