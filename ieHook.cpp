// ieHook.cpp: implementation of the ieHook class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ieHook.h"
#include <atlmisc.h>
//#include "HTTPHeaders.h"
#include "SettingsStorage.h"
#include <map>
#define SECURITY_WIN32
#include <sspi.h>
#include <schannel.h>
#include "CHttpHeaders.h"
#include "HttpRequestInfo.h"
#include "AutoMutex.h"
#include "BreakPointDlg.h"

#pragma comment(lib, "Ws2_32.lib")              

// declare the hooks
DECLARE_HOOK(int, __stdcall, send, (SOCKET, const char*, int, int));
DECLARE_HOOK(int, __stdcall, WSARecv, (SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE));
DECLARE_HOOK(int, __stdcall, recv, (SOCKET, char*, int, int));

typedef SECURITY_STATUS (__stdcall* procEncryptMessage)(PCtxtHandle, ULONG, PSecBufferDesc, ULONG);
typedef SECURITY_STATUS (__stdcall* procDecryptMessage)(PCtxtHandle, PSecBufferDesc, ULONG, ULONG*);

procEncryptMessage EncryptMessage_Chain = NULL;
procDecryptMessage DecryptMessage_Chain = NULL;

#define MIN_LEN_OUTGOING 3

SECURITY_STATUS __stdcall DecryptMessage_Hook(PCtxtHandle phContext,
                                              PSecBufferDesc pMessage,
                                              ULONG MessageSeqNo,
                                              PULONG pfQOP)
{
  SECURITY_STATUS Status =
    DecryptMessage_Chain(phContext, pMessage, MessageSeqNo, pfQOP);

  if (SEC_E_OK == Status)
  {
    for (int i = 0; i < (int)pMessage->cBuffers; ++i)
    {
      SecBuffer pBuffer =  pMessage->pBuffers[i];

      if (pBuffer.BufferType == SECBUFFER_DATA)
      {
        ieHook::GetHook().OnIncoming(NULL, (LPCTSTR)pBuffer.pvBuffer, pBuffer.cbBuffer, true);
      }
    }
  }

  return Status;
}


SECURITY_STATUS __stdcall EncryptMessage_Hook(PCtxtHandle phContext,
                                              ULONG fQOP,
                                              PSecBufferDesc pMessage,
                                              ULONG ulMessageSeqNo)
{
  for (int i = 0; i < (int)pMessage->cBuffers; ++i)
  {
    SecBuffer pBuffer =  pMessage->pBuffers[i];

    if (pBuffer.BufferType == SECBUFFER_DATA)
    {
      ieHook::GetHook().OnOutgoing(NULL, (LPCTSTR)pBuffer.pvBuffer, pBuffer.cbBuffer, true);
    }
  }

  return EncryptMessage_Chain(phContext, fQOP, pMessage, ulMessageSeqNo);
}

int __stdcall recv_Hook(SOCKET s, char* buffer, int len, int flags)
{
  int ret = recv_Chain(s, buffer, len, flags);

  if (SOCKET_ERROR != ret && ret > 0)
    ieHook::GetHook().OnIncoming(s, buffer, ret, FALSE);

  return ret;
}
 
int __stdcall WSARecv_Hook(SOCKET s,
                           LPWSABUF lpBuffers,
                           DWORD dwBufferCount,
                           LPDWORD lpNumberOfBytesRecvd,
                           LPDWORD lpFlags,
                           LPWSAOVERLAPPED lpOverlapped,
                           LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
)
{

  int iRet = WSARecv_Chain(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);

  if (0 == iRet) 
  { // success

    for (DWORD dw = 0; dw < dwBufferCount; ++dw)
    {
      if (lpNumberOfBytesRecvd && *lpNumberOfBytesRecvd)   
      {
         ieHook::GetHook().OnIncoming(s, lpBuffers[dw].buf, *lpNumberOfBytesRecvd);
      }
    }
  }

  return iRet;
}

int __stdcall send_Hook(SOCKET s, const char* pszBuffer, int iBufLen, int iFlags)
{
  ieHook::GetHook().OnOutgoing(s, pszBuffer, iBufLen);

  int iRet =  send_Chain(s, pszBuffer, iBufLen, iFlags);

  return iRet;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ieHook::ieHook() : m_settings(SettingsStorage::GetStorage())
{
  m_bHooksInstalled = false;
}

ieHook::~ieHook()
{
  Cleanup();
}

ieHook& ieHook::GetHook()
{
  static ieHook s_Hook;

  return s_Hook;
}

void ieHook::Cleanup()
{
  REMOVE_HOOK(WSARecv);
  REMOVE_HOOK(send);
  REMOVE_HOOK(recv);
}

void ieHook::HookSSLFunctions()
{
  HMODULE hSecurDLL = LoadLibrary("Secur32.dll");

  if (!hSecurDLL)
    hSecurDLL = LoadLibrary("Security.dll"); // NT4

  VOID* pFuncEncrypt = GetProcAddress(hSecurDLL, "EncryptMessage");
  VOID* pFuncDecrypt = GetProcAddress(hSecurDLL, "DecryptMessage");

  if (pFuncEncrypt)
     EncryptMessage_Chain = (procEncryptMessage)CHHookProc(GetCurrentProcess(), pFuncEncrypt, EncryptMessage_Hook);

  if (pFuncDecrypt)
     DecryptMessage_Chain = (procDecryptMessage)CHHookProc(GetCurrentProcess(), pFuncDecrypt, DecryptMessage_Hook);
}

bool ieHook::Initialize()
{
  ATLASSERT(!m_bHooksInstalled);

  if (!m_bHooksInstalled)
  {
    m_bHooksInstalled = true;

    SETUP_HOOK(WSARecv);
    SETUP_HOOK(send);
    SETUP_HOOK(recv);

    HookSSLFunctions();
  }

  return true;
}

COrchestrator& ieHook::GetOrchestrator() 
{
  return m_orchestrator;
}

void ieHook::OnIncoming(SOCKET sock, LPCTSTR pszText, DWORD dwLen, bool bSSL)
{
  if (pszText && dwLen)
    m_orchestrator.OnResponse((DWORD)sock, pszText, dwLen, bSSL);
}

void ieHook::OnOutgoing(SOCKET sock, LPCTSTR pszText, int iLen, bool bSSL)
{
  if (pszText && iLen > MIN_LEN_OUTGOING)
    m_orchestrator.OnRequest((DWORD)sock, pszText, (DWORD)iLen, bSSL);
}
