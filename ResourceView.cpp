#include "stdafx.h"
#include "CHttpHeaders.h"
#include "SettingsStorage.h"
#include "ResourceView.h"
#include "TabbedSettings.h"
#include "AutoMutex.h"

#define ID_CLEAR    1
#define ID_SETTINGS 2

CResourceView::CResourceView(void) : m_settings(SettingsStorage::GetStorage())
{
  ::InitializeCriticalSection(&m_cs);
  if (m_menu.CreatePopupMenu())
  {
    m_menu.AppendMenu(MF_STRING, ID_CLEAR, (LPCTSTR)"Clear");
    m_menu.AppendMenu(MF_STRING, ID_SETTINGS, (LPCTSTR)"Settings...");
  }
}

CResourceView::~CResourceView(void)
{
  ::DeleteCriticalSection(&m_cs);
}

void CResourceView::OnRequest(DWORD context, CString& request, CHttpHeaders& headers, bool isSSL,
                              DWORD tickCount)
{
  CAutoMutex mutex(m_cs);

  HTREEITEM hRoot = InsertItem(headers.GetVerb(), 0, 0);
  HTREEITEM hRequestRoot = InsertItem(headers.GetVerb(), hRoot, 0);

  for (HeaderList::const_iterator i = headers.GetHeaders().begin(); i != headers.GetHeaders().end(); ++i)
  {
    CString item;
    item.Format("%s: %s", (*i)->strHeader, (*i)->strValue);

    InsertItem(item, hRequestRoot, 0);
  }

  m_contextParents.insert(std::map<DWORD, HTREEITEM>::value_type(context, hRoot));
  m_tickCounts.insert(std::map<DWORD, DWORD>::value_type(context, tickCount));
}

void CResourceView::OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL, 
                               bool shouldBeFiltered, DWORD tickCount)
{
  CAutoMutex mutex(m_cs);

  // insert in correct HTREEITEM root (context)
  std::map<DWORD, HTREEITEM>::const_iterator i =
   m_contextParents.find(context);

  std::map<DWORD, DWORD>::const_iterator tickIterator
    = m_tickCounts.find(context);

  if (m_contextParents.end() != i)
  {
    HTREEITEM hRoot = i->second;

    if (!shouldBeFiltered)
    {
      //
      // insert response items
      //
      HTREEITEM hResponseRoot = InsertItem(headers.GetVerb(), hRoot, 0);

      for (HeaderList::const_iterator t = headers.GetHeaders().begin(); t != headers.GetHeaders().end(); ++t)
      {
        CString item;
        item.Format("%s: %s", (*t)->strHeader, (*t)->strValue);

        InsertItem(item, hResponseRoot, 0);
      }

      //
      // insert transaction time
      //
      if (m_tickCounts.end() != tickIterator)
      {
        DWORD elapsed = tickCount - (*tickIterator).second;

        CString item;
        item.Format("Transaction time: %ld ms", elapsed);

        InsertItem(item, hRoot, 0);
      }

    }
    else
    {
      DeleteItem(hRoot);
    }

    m_contextParents.erase(i);
    m_tickCounts.erase(tickIterator);
  }

}

void CResourceView::OnClear()
{
  DeleteAllItems();
}

void CResourceView::OnSettings()
{
  CTabbedSettings settings;

  if (IDOK == settings.DoModal())
    m_settings.Save();
}

LRESULT CResourceView::OnRightButtonDown(UINT ui, CPoint pos)
{
  SetCapture();

  return 1;
}

LRESULT CResourceView::OnRightUp(UINT ui, CPoint Pos)
{
  if (m_menu.m_hMenu)
  {
    ClientToScreen(&Pos);
  
    int iCommand = m_menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY|TPM_RETURNCMD, Pos.x, Pos.y, m_hWnd);

    switch (iCommand)
    {
    case ID_CLEAR:
      OnClear();
      break;
    case ID_SETTINGS:
      OnSettings();
      break;
    default:
      break;
     
    };

  }

  ReleaseCapture();

  return 1;
}
