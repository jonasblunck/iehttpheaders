
// static back color CBackCol

#include <atlbase.h>
#include <AtlApp.h>
#include <atlwin.h>
#include <AtlCrack.h>
#include <AtlMisc.h>
#include <AtlScrl.h>
#include <AtlCtrls.h>

class CBackCol;

typedef CWindowImpl<CBackCol, CStatic> CBackColBase;

class CBackCol : public CBackColBase
{
  COLORREF m_Color;
  HBRUSH   m_Background;

public:

  CBackCol() : CBackColBase()
  {
    m_Color = ::GetSysColor(COLOR_BACKGROUND);
    m_Background = NULL;
  }

  ~CBackCol()
  {
    if (m_Background)
      ::DeleteObject(m_Background);
  }

  void SetBkColor(COLORREF Col)
  {
    m_Color = Col;

    if (m_Background)
    {
      ::DeleteObject(m_Background);
      m_Background = NULL;
    }

    RedrawWindow();
  }

  COLORREF GetBkColor() const
  {
    return m_Color;
  }

  BEGIN_MSG_MAP(CBackCol)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
  END_MSG_MAP()

  LRESULT OnPaint(UINT, WPARAM wParam, LPARAM, BOOL& bHandled)
  {
    if (NULL == m_Background)
      m_Background = CreateSolidBrush(m_Color);

    CPaintDC dc(m_hWnd); 

	  CRect rc;
	  GetClientRect(rc);

    dc.FillRect(rc, m_Background);

    return 0;
  }


};


