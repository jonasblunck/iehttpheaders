#pragma once

#include "resource.h"       // main symbols
#include <AtlApp.h>
#include <AtlWin.h>
#include <atlcrack.h>
#include <AtlCtrls.h>
#include <atlmisc.h>
#include <atlctrlx.h>
#include "ITransactionProcessor.h"
#include <map>
#include "ThreadWorker.h"

class CHTTPTraceRichEdit;
typedef CWindowImpl<CHTTPTraceRichEdit, CRichEditCtrl> RichEditBase;

class CHTTPTraceRichEdit : 
  public RichEditBase, 
  public ITransactionProcessor
{
  CMenu m_Menu; 
  SettingsStorage& m_settings;
  std::map<DWORD, CString> m_requests; 

  void OnClear();
  void OnSave();
  void OnCloseBar();
  void OnSettings();

  void PrintHeaders(const CString& text, COLORREF color);

public:
  CHTTPTraceRichEdit();
  ~CHTTPTraceRichEdit();

  LRESULT OnRightUp(UINT ui, CPoint Pos);

  BEGIN_MSG_MAP(CHTTPTraceRichEdit)
    MSG_WM_RBUTTONUP(OnRightUp);
  END_MSG_MAP()

  virtual void OnRequest(DWORD context, CString& request, CHttpHeaders& headers, bool isSSL, DWORD tickCount);
  virtual void OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL, bool shouldBeFiltered, DWORD tickCount);
};
