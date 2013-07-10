// ieHook.h: interface for the ieHook class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IEHOOK_H__D3117041_5000_4688_944F_F329A7C3C89F__INCLUDED_)
#define AFX_IEHOOK_H__D3117041_5000_4688_944F_F329A7C3C89F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AtlApp.h>
#include <AtlWin.h>
#include <AtlCtrls.h>
#include <list>
#include <atlmisc.h>
#include <map>
#include "BreakPointDlg.h"
#include "Orchestrator.h"

typedef std::map<SOCKET, CString> HeadersMap;
typedef HeadersMap RequestMap;
class CHttpHeaders;
class SettingsStorage;

class ieHook  
{
private:
  bool       m_bHooksInstalled;
  SettingsStorage& m_settings;
  CBreakPointDlg  m_dlgBreakpoint;
  COrchestrator m_orchestrator;

  void HookSSLFunctions();

  ieHook();

public:
	~ieHook();

  static ieHook& GetHook(); 

  void Cleanup();
  bool Initialize();

  void OnOutgoing(SOCKET sock, LPCTSTR pszText, int iLen, bool bSSL = false);
  void OnIncoming(SOCKET sock, LPCTSTR pszText, DWORD dwLen, bool bSSL = false);

  COrchestrator& GetOrchestrator();

};

#endif // !defined(AFX_IEHOOK_H__D3117041_5000_4688_944F_F329A7C3C89F__INCLUDED_)
