// ieHTTPTrace.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f ieHTTPTraceps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "ieHTTPTrace.h"

#include "ieHTTPTrace_i.c"
#include "HTTPTrace.h"
#include "ieHook.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_HTTPTrace, CHTTPTrace)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_IEHTTPTRACELib);
        DisableThreadLibraryCalls(hInstance);
  
        ieHook::GetHook().Initialize();
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        _Module.Term();
    }

    return TRUE;    // ok
}

//////////////////////////////////////////////////////////
// erase registry key
bool EraseRegKey(HKEY parent, LPCTSTR pszKey, LPCTSTR pszDeleteSubKey)
{
  HKEY hKey = NULL;
  bool returnValue = false;

  if (ERROR_SUCCESS == RegOpenKeyEx(parent, pszKey, 0, DELETE, &hKey))
  {
    if (ERROR_SUCCESS == RegDeleteKey(hKey, pszDeleteSubKey))
    {
      returnValue = true;

      CString message;
      message.Format("Erased subkey: %s in parent key %s.", pszDeleteSubKey, pszKey);

      OutputDebugString((LPCTSTR)message);
    }

    RegCloseKey(hKey);
  }

  return returnValue;
}

////////////////////////////////////////////////////////////////////////////////////
// erase installed cache so that, after a reboot, ieHTTPHeaders will show up
// 
// see: http://forums.microsoft.com/MSDN/ShowPost.aspx?PostID=835790&SiteID=1
//
bool EraseInstallCache()
{
  EraseRegKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Discardable\\PostSetup\\Component Categories\\{00021493-0000-0000-C000-000000000046}", "Enum");
  EraseRegKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Discardable\\PostSetup\\Component Categories", "{00021493-0000-0000-C000-000000000046}");

  EraseRegKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Discardable\\PostSetup\\Component Categories\\{00021494-0000-0000-C000-000000000046}", "Enum");
  EraseRegKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Discardable\\PostSetup\\Component Categories", "{00021494-0000-0000-C000-000000000046}");

  return true;
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
  EraseInstallCache();

    // registers object, typelib and all interfaces in typelib
  return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
   return _Module.UnregisterServer(TRUE);
}

