// OutputWnd.cpp: implementation of the COutputWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "OutputWnd.h"
#include "ScintillaView.h"
#include "LuaHelper.h"
#include "MainFrame.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COutputWnd::COutputWnd() : m_pSciView (NULL)
{
}

int COutputWnd::OnSci(CWnd* pView, SCNotification* pNotify)
{
    CLuaEditor* pEditor = m_pSciView->GetEditor();

	CPoint pt;
	int nLine;
	CString strLine;
	switch (pNotify->nmhdr.code)
	{
		case SCN_DOUBLECLICK:
			GetCursorPos(&pt);
			pEditor->ScreenToClient(&pt);
			nLine = pEditor->LineFromPoint(pt) + 1;
			strLine = pEditor->GetLine(nLine);
			GotoLine(strLine);
		break;
	};

	return 0;
}

void COutputWnd::Init (CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)   {
        m_pSciView = (CScintillaView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CScintillaView));
		m_pSciView->GetEditor ()->SetSkin ();
	}
}

void COutputWnd::Write (CString strMsg)
{
    m_pSciView->Write  (strMsg);
}

void COutputWnd::Clear ()
{
    if (m_pSciView)
    	m_pSciView->Clear();
}

void CErrorWnd::GotoLine(CString strLine)
{
    CString strPathName;
    int nLine;
    char szBuffer[256], *p, *q, *s;

    strlimcpy (szBuffer, strLine, 256);
    if (p=strchr (szBuffer, ':'))   {
        *p='\0';
        q=p+1;
        strdblk (szBuffer);
        if (!strcmp (szBuffer, LOADSTRING (IDS_ADAMO_COMPILER)) || !strcmp (szBuffer, LOADSTRING (IDS_ADAMO_PREPROCESSOR)))   {
            if (p=strchr (q, ':'))   {
                if (p=strchr (p+1, ':'))   {
                    s=p+1;
                    *p='\0';
                    if (p=strchr (s, ':'))   {
                        *p='\0';
                        strPathName=strdblk (q);
                        nLine=atoi (strdblk (s));
                    }
                }
            }
        }
    }
    else   {

    }
	((CMainFrame*)AfxGetMainWnd())->GotoFileLine(strPathName, nLine);	
}

void CErrorWnd::GotoNextError ()
{
    CString str;
    LONG l=m_pSciView->GetEditor()->GetLineCount ();
    int i;
    bool bFound=false;

    if (l)   {
        for (i=m_nCurErrorLine+1; i<l; i++)   {
            str=m_pSciView->GetEditor()->GetLine (i);
            if (IsValidErrLine (str))   {
                GotoLine (str);
                m_nCurErrorLine=i;
                bFound=true;
                break;
            }
        }
        if (!bFound)   {
            for (i=0; i<=m_nCurErrorLine; i++)   {
                str=m_pSciView->GetEditor()->GetLine (i);
                if (IsValidErrLine (str))   {
                    GotoLine (str);
                    m_nCurErrorLine=i;
                    break;
                }
            }
        }
        if (m_nCurErrorLine!=-1)
            m_pSciView->GetEditor()->SelectLine (m_nCurErrorLine);
    }
}

void CErrorWnd::GotoPrevError ()
{
    CString str;
    LONG l=m_pSciView->GetEditor()->GetLineCount ();
    int i;
    bool bFound=false;

    if (l)   {
        for (i=m_nCurErrorLine-1; i>=0; i--)   {
            str=m_pSciView->GetEditor()->GetLine (i);
            if (IsValidErrLine (str))   {
                GotoLine (str);
                m_nCurErrorLine=i;
                bFound=true;
                break;
            }
        }
        if (!bFound)   {
            for (i=l-1; i>=m_nCurErrorLine; i--)   {
                str=m_pSciView->GetEditor()->GetLine (i);
                if (IsValidErrLine (str))   {
                    GotoLine (str);
                    m_nCurErrorLine=i;
                    break;
                }
            }
        }
        if (m_nCurErrorLine!=-1)
            m_pSciView->GetEditor()->SelectLine (m_nCurErrorLine);
    }
}

bool CErrorWnd::IsValidErrLine (const char* sz)
{
    bool b=false;
    char szBuffer[256], *p;

    strlimcpy (szBuffer, sz, 256);
    if (p=strchr (szBuffer, ':'))   {
        *p='\0';
        strdblk (szBuffer);
        if (!strcmp (szBuffer, LOADSTRING (IDS_ADAMO_COMPILER)) || !strcmp (szBuffer, LOADSTRING (IDS_ADAMO_PREPROCESSOR)) )
            b=true;
    }
    return b;
}

void CErrorWnd::Clear ()
{
    COutputWnd::Clear ();
    m_nCurErrorLine=-1;
}

void CDebugWnd::GotoLine(CString strLine)
{
    int   nLine;
    char* pBuffer, *p, *q;

    pBuffer=strLine.GetBuffer (strLine.GetLength());
    if (pBuffer)   {
        p=strchr (pBuffer, '(');
        if (p)   {
            q=strchr (p+1, ')');
            if (q)   {
                *q='\0';
                nLine=atoi (p+1);
                *p='\0';
                ((CMainFrame*)AfxGetMainWnd())->GotoFileLine(strdblk (pBuffer), nLine);	
            }
        }
    }
}

void CDebugWnd::GotoNextError ()
{
    CString str;
    LONG l=m_pSciView->GetEditor()->GetLineCount ();
    int i;
    bool bFound=false;

    if (l)   {
        for (i=m_nCurErrorLine+1; i<l; i++)   {
            str=m_pSciView->GetEditor()->GetLine (i);
            GotoLine (str);
            m_nCurErrorLine=i;
            bFound=true;
            break;
        }
        if (!bFound)   {
            for (i=0; i<=m_nCurErrorLine; i++)   {
                str=m_pSciView->GetEditor()->GetLine (i);
                GotoLine (str);
                m_nCurErrorLine=i;
                break;
            }
        }
        if (m_nCurErrorLine!=-1)
            m_pSciView->GetEditor()->SelectLine (m_nCurErrorLine);
    }
}

void CDebugWnd::GotoPrevError ()
{
    CString str;
    LONG l=m_pSciView->GetEditor()->GetLineCount ();
    int i;
    bool bFound=false;

    if (l)   {
        for (i=m_nCurErrorLine-1; i>=0; i--)   {
            str=m_pSciView->GetEditor()->GetLine (i);
            GotoLine (str);
            m_nCurErrorLine=i;
            bFound=true;
            break;
        }
        if (!bFound)   {
            for (i=l-1; i>=m_nCurErrorLine; i--)   {
                str=m_pSciView->GetEditor()->GetLine (i);
                GotoLine (str);
                m_nCurErrorLine=i;
                break;
            }
        }
        if (m_nCurErrorLine!=-1)
            m_pSciView->GetEditor()->SelectLine (m_nCurErrorLine);
    }
}

void CDebugWnd::Clear ()
{
    COutputWnd::Clear ();
    m_nCurErrorLine=-1;
}
