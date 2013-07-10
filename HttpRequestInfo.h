#pragma once

#include "CHttpHeaders.h"

class CHttpRequestInfo
{
  CHttpHeaders& m_rHeaders;

public:
  CHttpRequestInfo(CHttpHeaders& rHeaders);
  ~CHttpRequestInfo(void);

  bool GetContentLength(int& riBytes);
};
