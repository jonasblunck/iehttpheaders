// HTTPTrace.h : Declaration of the CHTTPTrace

#ifndef __HTTPTRACE_H_
#define __HTTPTRACE_H_

#include "resource.h"       // main symbols
#include <AtlApp.h>
#include <AtlWin.h>
#include <atlcrack.h>
#include <AtlCtrls.h>
#include <atlmisc.h>
#include "CHTTPTraceRichEdit.h"
#include "ResourceView.h"
#include "SaveProcessor.h"

#define EDSTYLE     ES_MULTILINE| WS_CHILD | WS_VISIBLE | /*WS_DISABLED | */ WS_VSCROLL
#define EDEXSTYLE   WS_EX_TOOLWINDOW


/////////////////////////////////////////////////////////////////////////////
// CHTTPTrace
class ATL_NO_VTABLE CHTTPTrace : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CHTTPTrace, &CLSID_HTTPTrace>,
	public CWindowImpl<CHTTPTrace>,
  public IPersistStreamInitImpl<CHTTPTrace>,
  public IObjectWithSiteImpl<CHTTPTrace>,
  public IOleCommandTarget,
	public IDeskBand
{
private:
  CComPtr<IUnknown> m_spUnknownSite;

  CHTTPTraceRichEdit   m_ctrlEdit;
  CResourceView m_resourceView;
  CTabView m_tabView;
  CRITICAL_SECTION m_cs;
  //CSaveProcessor m_saveProcessor;

public:
  CHTTPTrace()//  : m_saveProcessor(SettingsStorage::GetStorage())
	{
    m_bRequiresSave = FALSE;
    m_hWnd = NULL;
    ::InitializeCriticalSection(&m_cs);
	}

  ~CHTTPTrace()
  {
    ::DeleteCriticalSection(&m_cs);
  }

DECLARE_REGISTRY_RESOURCEID(IDR_HTTPTRACE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CHTTPTrace)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
  COM_INTERFACE_ENTRY(IObjectWithSite)
  COM_INTERFACE_ENTRY(IOleCommandTarget)
	COM_INTERFACE_ENTRY_IID(IID_IDeskBand,IDeskBand)
END_COM_MAP()

BEGIN_PROP_MAP(CHTTPTrace)
END_PROP_MAP()


// IHTTPTrace
public:
  BOOL m_bRequiresSave;

  //IObjectWithSite
public:
	STDMETHODIMP SetSite(IUnknown* pUnkSite);
	STDMETHODIMP GetSite(REFIID riid, void **ppvSite);

/////////////////////////////////////////////////////////////////////////////
// IDockingWindow, IDeskBand Methods
public:

  LRESULT OnCreate(LPCREATESTRUCT pcs);
  LRESULT OnSize(UINT ui, CSize Size);
  LRESULT OnRightButton(UINT ui, CPoint Pos);

  STDMETHOD(GetWindow)(HWND* phwnd);
	STDMETHOD(ContextSensitiveHelp)(BOOL);
	STDMETHOD(ShowDW)(BOOL fShow);
	STDMETHOD(CloseDW)(unsigned long dwReserved);
	STDMETHOD(ResizeBorderDW)(const RECT* prcBorder,IUnknown* punkToolbarSite, BOOL fReserved);
	STDMETHOD(GetBandInfo)(DWORD dwBandID, DWORD dwViewMode,DESKBANDINFO* pdbi);
  
  STDMETHOD(QueryStatus)(const GUID* pguidCmdGroup, ULONG cCmds, OLECMD* prgCmds, OLECMDTEXT* pCmdText);
  STDMETHOD(Exec)(const GUID* pguidCmdGroup, DWORD nCmdID, DWORD nCmdOptions, VARIANT* pIn, VARIANT* pPut);

BEGIN_MSG_MAP_EX(CHTTPTrace)
	MSG_WM_CREATE(OnCreate)
  MSG_WM_SIZE(OnSize);
END_MSG_MAP()

};

#endif //__HTTPTRACE_H_
