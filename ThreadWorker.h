
#pragma once

/*

  CThreadWorker is a simple class where a operation needs to run in a background thread
   - inherit CThreadWorker
   - call Start/Stop to init/un-init (stop is called in destructor)
   - call RunOperationInBackThread(DWORD) to start a operation in the background thread
   - override OnRunBackground(DWORD) to run your operation

*/

class CThreadWorker
{
protected:

  HANDLE m_hIOCP;
  HANDLE m_hThread;
  
  static DWORD __stdcall TraceThreadProc(LPVOID pvThis)
  {
    return ((CThreadWorker*)pvThis)->Proc();
  }

  DWORD Proc()
  {
	  ATLASSERT(m_hIOCP);
    
	  DWORD dwBytes = 0;
	  ULONG ulKey = 0;
	  OVERLAPPED* pOverlapped = NULL;

	  while (GetQueuedCompletionStatus(m_hIOCP, &dwBytes, &ulKey, &pOverlapped, INFINITE))
	  {
		  if (!dwBytes)
			  break;

      OnRunBackground((DWORD)ulKey);
	  }

	  return 0;
  }

  void RunOperationInBackThread(DWORD dwContext)
  {
    //ATLASSERT(m_hIOCP);

    PostQueuedCompletionStatus(m_hIOCP, 1, (ULONG_PTR)dwContext, NULL);
  }

  void Stop()
  {
    if (m_hIOCP)
    {
      PostQueuedCompletionStatus(m_hIOCP, 0, 0, NULL);

      WaitForSingleObject(m_hThread, 5000);

      CloseHandle(m_hIOCP);
      CloseHandle(m_hThread);

      m_hIOCP = NULL;
      m_hThread = NULL;
    }
  }

  void Start()
  {
    if (!m_hIOCP)
    {
      m_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

      m_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CThreadWorker::TraceThreadProc, (LPVOID)this, 0, NULL);
    }
  }

  CThreadWorker()
  {
    m_hIOCP = NULL;
    m_hThread = NULL;
  }

  virtual ~CThreadWorker()
  {
    Stop();
  }

  virtual void OnRunBackground(DWORD dwContext) = 0;
};