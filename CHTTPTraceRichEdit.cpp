#include "stdafx.h"
#include "chttptracerichedit.h"
#include "ieHook.h"
#include "TabbedSettings.h"
#include "CHttpHeaders.h"

#define ID_CLEAR             1
#define ID_SAVE              2
#define ID_REQ_HEADERS       3
#define ID_RESP_HEADERS      4
#define ID_CLOSE             5
#define ID_SETTINGS          7

struct HeaderPrintInfo
{
  CString text;
  COLORREF color;
};

CHTTPTraceRichEdit::CHTTPTraceRichEdit(void) : m_settings(SettingsStorage::GetStorage()), RichEditBase()
{
  if (m_Menu.CreatePopupMenu())
  {
    m_Menu.AppendMenu(MF_STRING, ID_CLEAR, (LPCTSTR)"Clear");
    m_Menu.AppendMenu(MF_STRING, ID_SAVE, (LPCTSTR)"Save...");
    m_Menu.AppendMenu(MF_STRING, ID_SETTINGS, (LPCTSTR)"Settings...");
  }
}

CHTTPTraceRichEdit::~CHTTPTraceRichEdit()
{
}

LRESULT CHTTPTraceRichEdit::OnRightUp(UINT ui, CPoint Pos)
{
  if (m_Menu.m_hMenu)
  {
    ClientToScreen(&Pos);
  
    int iCommand = m_Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY|TPM_RETURNCMD, Pos.x, Pos.y, m_hWnd);

    switch (iCommand)
    {
    case ID_CLEAR:
      OnClear();
      break;
    case ID_SAVE:
      OnSave();
      break;
    case ID_SETTINGS:
      OnSettings();
      break;
    default:
      break;
     
    };

  }

  return 1;
}

void CHTTPTraceRichEdit::OnSettings()
{
  CTabbedSettings settings;

  if (IDOK == settings.DoModal())
    m_settings.Save();
}

void CHTTPTraceRichEdit::OnClear()
{
  SetSel(0, -1);
  Clear();
}

void CHTTPTraceRichEdit::OnSave()
{
  long lLen = GetTextLength();

  if (lLen)
  {
    CString strHeaders;

    SetSel(0, - 1);
    GetSelText(strHeaders.GetBuffer(lLen + 1)); strHeaders.ReleaseBuffer();

    strHeaders.Replace("\r", "\r\n");

    OPENFILENAME FileInfo;
    memset(&FileInfo, 0, sizeof(FileInfo));

    char szFileName[256];
    szFileName[0] = 0;
  
    FileInfo.lStructSize = sizeof(FileInfo);
    FileInfo.lpstrFilter = ".txt";
    FileInfo.lpstrFile = szFileName;
    FileInfo.nMaxFile = sizeof(szFileName);
    FileInfo.Flags = OFN_EXPLORER;
    FileInfo.lpstrTitle = "Save HTTP Headers As...";

    if (GetSaveFileName(&FileInfo))
    {
      CString strFile(szFileName);

      if (-1 == strFile.Find("."))
        strFile += _T(".txt");

      HANDLE hFile = CreateFile((LPCTSTR)strFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);

      if (hFile)
      {
        DWORD dwBytesWritten = 0;
        
        SetEndOfFile(hFile);
        WriteFile(hFile, (LPCTSTR)strHeaders, lLen, &dwBytesWritten, NULL);

        CloseHandle(hFile);
      }

    }

  }
}

void CHTTPTraceRichEdit::OnRequest(DWORD context, CString& request, CHttpHeaders& headers, 
                                   bool isSSL, DWORD tickCount)
{
  COLORREF color = isSSL ? m_settings.GetColOutSSL() : m_settings.GetColOutPT();

  CString trace;

  if (m_settings.GetShowRequestHeader())
    headers.GetHeadersFormated(trace);

  if (m_settings.GetShowRequestBody() && request.GetLength() > headers.GetHeadersLength())
  {
    trace.Append("\r\n\r\n");
    trace.Append(request.Mid(headers.GetHeadersLength()));
    trace.Append("\r\n\r\n");
  }
  else
  {
    trace.Append("\r\n\r\n");
  }

  if (m_settings.GetGroupRequestResponse())
  {
    m_requests.insert(std::map<DWORD, CString>::value_type(context, trace));
  }
  else
  {
    PrintHeaders(trace, color);
  }
}

void CHTTPTraceRichEdit::OnResponse(DWORD context, CString& response, CHttpHeaders& headers, bool isSSL,
                                    bool shouldBeFiltered, DWORD tickCount)
{
  if (!shouldBeFiltered)
  {
    if (m_settings.GetGroupRequestResponse())
    {
      CString request;
      std::map<DWORD, CString>::const_iterator i = m_requests.find(context);

      if (i != m_requests.end())
      {
        request = i->second;
        m_requests.erase(i);

        PrintHeaders(request, isSSL ? m_settings.GetColOutSSL() : m_settings.GetColOutPT());
      }
    }

    CString trace;
    CString tmp;

    if (m_settings.GetShowResponseHeader())
    {
      headers.GetHeadersFormated(trace);
      trace.Append("\r\n");
    }

    if (m_settings.GetShowResponseBody() && !headers.GetHeader("Content-Encoding", tmp))
    {
      //
      // we should display response body and it is not encoded
      //
      CString contentType;
      
      if (headers.GetHeader("Content-Type", contentType))
      {
        if (contentType.Find("text") >= 0 || contentType.Find("x-json") >= 0 || contentType.Find("xml") >= 0)
        {
          // add response body
          trace.Append("\r\n");
          trace.Append(response.Mid(headers.GetHeadersLength()));
        }
      }
    }

    if (trace.GetLength() > 0)
    {
      trace.Append("\r\n");
      PrintHeaders(trace, isSSL ? m_settings.GetColInSSL() : m_settings.GetColInPT());
    }
  }
  else
  {
    // transaction should be filtered, cleanup
    CString request;
    std::map<DWORD, CString>::const_iterator i = m_requests.find(context);

    if (i != m_requests.end())
      m_requests.erase(i);
  }

}

void CHTTPTraceRichEdit::PrintHeaders(const CString& text, COLORREF color)
{
  CHARFORMAT cf;
	::memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
  cf.dwMask = CFM_COLOR;
  cf.crTextColor = color;

  CRichEditCtrl RichEdit(*this);

  int iTotalLength = RichEdit.GetWindowTextLength();

  RichEdit.HideSelection(TRUE, TRUE);
  RichEdit.SetSel(iTotalLength, iTotalLength);
  RichEdit.SetSelectionCharFormat(cf);
  RichEdit.ReplaceSel((LPCTSTR)text);

  RichEdit.HideSelection(FALSE, TRUE);

  if (m_settings.GetScrollDuringCapture())
    RichEdit.SendMessage(EM_SCROLLCARET, 0, 0);
}

