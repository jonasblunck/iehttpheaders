#include "stdafx.h"
#include "AboutDlg.h"
#include "resource.h"

CAboutDlg::CAboutDlg()
{
  //m_psp.dwFlags |= PSP_USEICONID;
  //m_psp.pszIcon = MAKEINTRESOURCE(IDI_ABOUT);
  //m_psp.hInstance = _Module.GetResourceInstance();
}

BOOL CAboutDlg::OnInitDialog ( HWND hwndFocus, LPARAM lParam )
{
	m_hlBlunck.SubclassWindow(GetDlgItem(IDC_BLUNCK_LINK));
	m_hlBlunck.SetHyperLink(_T("http://www.blunck.se"));
	m_hlBlunck.SetLabel(_T("www.blunck.se"));

  CenterHyperLinkLabel(m_hlBlunck);

  return TRUE;
}

int CAboutDlg::OnApply()
{
  return 1;
}

void CAboutDlg::CenterHyperLinkLabel(CHyperLink& hl)
{
	// Tim Tabor does it more or less like this, so it can't be all wrong
	CRect rLinkCtl;

	hl.GetClientRect(&rLinkCtl);

  CRect rLinkLabel(hl.m_rcLink);	
	rLinkLabel.OffsetRect((rLinkCtl.Width() - rLinkLabel.Width()) / 2 , 0);
	
	CopyRect(&hl.m_rcLink, rLinkCtl);

}