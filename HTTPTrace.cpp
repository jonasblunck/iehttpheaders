// HTTPTrace.cpp : Implementation of CHTTPTrace
#include "stdafx.h"
#include "IeHTTPTrace.h"
#include "HTTPTrace.h"
#include "ieHook.h"
#include <atlctrls.h>
#include <comdef.h>
#include "ITransactionProcessor.h"
#include "AutoMutex.h"
/////////////////////////////////////////////////////////////////////////////
// CHTTPTrace


STDMETHODIMP CHTTPTrace::SetSite(IUnknown* pUnkSite)
{
  if (m_spUnknownSite)
    m_spUnknownSite.Release();

  if (pUnkSite)
  {
    m_spUnknownSite = pUnkSite;

	  CComQIPtr<IOleWindow> spOleWindow = pUnkSite;

    HWND hWndBar;

    if (spOleWindow)
		  spOleWindow->GetWindow(&hWndBar);

    if (!hWndBar)
      return E_FAIL;

		Create(hWndBar, CWindow::rcDefault);
  }
  return S_OK;
}

STDMETHODIMP CHTTPTrace::GetSite(REFIID riid, void** ppvSite)
{
  return m_spUnknownSite->QueryInterface(riid, ppvSite);
}

STDMETHODIMP CHTTPTrace::GetBandInfo(DWORD dwBandID, DWORD dwViewMode,DESKBANDINFO* pdbi)
{
	if (pdbi)
	{
		if(pdbi->dwMask & DBIM_MINSIZE)
		{
			pdbi->ptMinSize.x = -1;
			pdbi->ptMinSize.y = -1;
		}

		if(pdbi->dwMask & DBIM_MAXSIZE)
		{
			pdbi->ptMaxSize.x = -1;
			pdbi->ptMaxSize.y = -1;
		}

		if(pdbi->dwMask & DBIM_INTEGRAL)
		{
			pdbi->ptIntegral.x = 1;
			pdbi->ptIntegral.y = 1;
		}

		if(pdbi->dwMask & DBIM_ACTUAL)
		{
			pdbi->ptActual.x = 0;
			pdbi->ptActual.y = 0;
		}

		//
		// Set the title of the Explorer Bar
		//
		if(pdbi->dwMask & DBIM_TITLE)
		{
			lstrcpyW(pdbi->wszTitle, L"ieHTTPHeaders");
		}

		if(pdbi->dwMask & DBIM_MODEFLAGS)
		{
			pdbi->dwModeFlags = DBIMF_VARIABLEHEIGHT;

		}

		if(pdbi->dwMask & DBIM_BKCOLOR)
		{
			pdbi->dwMask &= ~DBIM_BKCOLOR;

		}
		return S_OK;

	}

	return E_INVALIDARG;
}

LRESULT CHTTPTrace::OnCreate(LPCREATESTRUCT pcs)
{
  LoadLibrary(m_ctrlEdit.GetLibraryName());

  m_tabView.Create(m_hWnd);
  m_ctrlEdit.Create(m_tabView, NULL, NULL, WS_CHILD|EDSTYLE, EDEXSTYLE);
	m_resourceView.Create(m_tabView, rcDefault, NULL, WS_CHILD|TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT);
  
  if (!m_ctrlEdit.m_hWnd || !m_tabView.m_hWnd || !m_resourceView.m_hWnd) 
    return -1;

  m_ctrlEdit.SetFont(AtlGetStockFont(DEFAULT_GUI_FONT)); 

  PARAFORMAT Format;
  memset(&Format, 0, sizeof(Format));
  Format.cbSize = sizeof(Format);
  Format.dwMask = PFM_STARTINDENT;
  Format.dxStartIndent = 100; // 100;

  m_ctrlEdit.SetParaFormat(Format);
  m_tabView.InsertPage(0, m_ctrlEdit.m_hWnd, "Headers view");
  m_tabView.InsertPage(1, m_resourceView, "Resource view");
  m_tabView.SetActivePage(0);

  COrchestrator& orchestrator =
    ieHook::GetHook().GetOrchestrator();

  orchestrator.AddProcessor(static_cast<ITransactionProcessor*>(&m_ctrlEdit));
  orchestrator.AddProcessor(static_cast<ITransactionProcessor*>(&m_resourceView));
//  orchestrator.AddProcessor(static_cast<ITransactionProcessor*>(&m_saveProcessor));

  return 0; // -1 if failed
}

LRESULT CHTTPTrace::OnRightButton(UINT ui, CPoint Pos)
{
  return 0;
}

LRESULT CHTTPTrace::OnSize(UINT ui, CSize size)
{
  if (m_tabView.IsWindow())
  {
		CRect rc(0, 0, size.cx, size.cy);
		m_tabView.MoveWindow(rc, TRUE);
  }

  return 0;
}         

STDMETHODIMP CHTTPTrace::GetWindow(HWND* phwnd)
{
	*phwnd = m_hWnd;
	return S_OK;
}


STDMETHODIMP CHTTPTrace::ContextSensitiveHelp(BOOL)
{
	return E_NOTIMPL;
}


STDMETHODIMP CHTTPTrace::ShowDW(BOOL fShow)
{
	if(m_hWnd)
	{
		if(fShow)
		{
			::ShowWindow(m_hWnd,SW_SHOW);
		}
		else
		{
			::ShowWindow(m_hWnd,SW_HIDE);
		}
	}
	return S_OK;
}


STDMETHODIMP CHTTPTrace::CloseDW(unsigned long dwReserved)
{
  CAutoMutex mutex(m_cs);
	ShowDW(FALSE);

	if(::IsWindow(m_hWnd))
  {
    COrchestrator& orchestrator =
      ieHook::GetHook().GetOrchestrator();
    
    orchestrator.RemoveProcessor(static_cast<ITransactionProcessor*>(&m_ctrlEdit));
    orchestrator.RemoveProcessor(static_cast<ITransactionProcessor*>(&m_resourceView));
    //orchestrator.RemoveProcessor(static_cast<ITransactionProcessor*>(&m_saveProcessor));

    m_ctrlEdit.DestroyWindow();
    m_resourceView.DestroyWindow();
    m_tabView.DestroyWindow();

    ::DestroyWindow(m_hWnd);
  }

	m_hWnd = NULL;
	return S_OK;
}


STDMETHODIMP CHTTPTrace::ResizeBorderDW(const RECT*  prcBorder,
                                   IUnknown* punkToolbarSite,
                                   BOOL      fReserved)
{
	return E_NOTIMPL;
}

STDMETHODIMP CHTTPTrace::QueryStatus(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD* prgCmds, OLECMDTEXT* pCmdText)
{
  if (NULL != prgCmds)
  {
    prgCmds->cmdf = OLECMDF_SUPPORTED|OLECMDF_ENABLED;

    return S_OK;
  }

  return E_FAIL;
}

STDMETHODIMP CHTTPTrace::Exec(const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdOptions, VARIANT* pIn, VARIANT* pOut)
{
  ATLASSERT(m_spUnknownSite.p);

  try
  {
    if (nCmdID == 0 && pIn == NULL && pOut == NULL)
    {
      CComPtr<IWebBrowser2> spWB;
      CComPtr<IServiceProvider> spSP;

      _com_util::CheckError(m_spUnknownSite.QueryInterface(&spSP));
      _com_util::CheckError(spSP->QueryService(SID_SWebBrowserApp, IID_IWebBrowser2, (LPVOID*)&spWB));

      CComBSTR clsid("{ED2DD609-156E-44EE-B2EC-B93544F5A0D6}");
      CComVariant vtClsid(clsid);
      CComVariant vtTrue;
      CComVariant dummy;

      vtTrue.vt = VT_BOOL;
      vtTrue.boolVal = VARIANT_TRUE;

      _com_util::CheckError(spWB->ShowBrowserBar(&vtClsid, &vtTrue, &dummy));
    }
  }
  catch(_com_error&)
  {
    ::MessageBox(NULL, "Unable to display explorer bar. Probably not registered.", "Error", MB_ICONEXCLAMATION);
  }

  return S_OK;
}


