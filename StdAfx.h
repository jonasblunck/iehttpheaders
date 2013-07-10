// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__23DCF066_116F_474D_A459_7D0E890AF379__INCLUDED_)
#define AFX_STDAFX_H__23DCF066_116F_474D_A459_7D0E890AF379__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#define _ATL_APARTMENT_THREADED

#define _WTL_NO_CSTRING
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;
#include <atlcom.h>
#include <atlstr.h>
#include <shlobj.h>

#include "resource.h"

#include <AtlApp.h>
#include <AtlWin.h>
#include <atlcrack.h>
#include <AtlCtrls.h>
#include <atlmisc.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <list>
#include <atlddx.h>

#include "chainhook\ChainHookAPI.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__23DCF066_116F_474D_A459_7D0E890AF379__INCLUDED)
