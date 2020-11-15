/*
**
*/

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "AdamoFindInFilesView.h"
#include "ScintillaView.h"
#include "MainFrame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
** Init : inizializzazione finestra.
*/
void CAdamoFindInFilesView::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)   {
        m_pSciView = (CScintillaView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CScintillaView));
		m_pSciView->GetEditor ()->SetSkin ();
	}
}

/*
** Clear : ripulisce l'editor
*/ 
void CAdamoFindInFilesView::Clear ()
{
    if (m_pSciView)
    	m_pSciView->Clear();
}

/*
** WriteCanceled :
*/
void CAdamoFindInFilesView::WriteCanceled ()
{
    m_pSciView->Write(LOADSTRING (IDS_ADAMOFINDINFILESVIEW_1));
}

/*
** WriteOccurences :
*/
void CAdamoFindInFilesView::WriteOccurences (int n)
{
    CString strBuffer, str;
    if (n==1)
        str=LOADSTRING (IDS_ADAMOFINDINFILESVIEW_2);
    else
        str=LOADSTRING (IDS_ADAMOFINDINFILESVIEW_3);
    strBuffer.Format (str, n);
    m_pSciView->Write  (strBuffer);
}

/*
** Add : aggiunge un file trovato dal find in files all'editor
*/
void CAdamoFindInFilesView::Add (const char* szPath, const char* szLine)
{
    char szBuffer[_MAX_PATH+512+1];
    if (m_pSciView)   {
        if (strlen (szPath)>_MAX_PATH)
            return;
        if (strlen (szLine)>512)
            return;
        sprintf (szBuffer, "%s %s\n", szPath, szLine);
    	m_pSciView->Write(szBuffer);
    }
}

/*
** OnSci : la funzione e' chiamata su una notifica dello scntilla
*/
int CAdamoFindInFilesView::OnSci(CWnd* pView, SCNotification* pNotify)
{
	int nLine;

    if (m_pSciView)   {
	    CLuaEditor* pEditor = m_pSciView->GetEditor();
	    switch (pNotify->nmhdr.code)   {
		    case SCN_DOUBLECLICK:
                nLine = pEditor->GetCurrentLine ();
                CString str=pEditor->GetLine (nLine);
                GotoFileLine (str);
		        break;
	    };
    }
	return 0;
}

int CAdamoFindInFilesView::GotoFileLine (CString str)
{
    int   nLine, nB=-1;
    char* pBuffer, *pBegin, *pEnd;

    if (str!="")   {
        pBuffer=str.GetBuffer (str.GetLength());
        if (pBuffer)   {
            pBegin=strchr (pBuffer, '(');
            if (pBegin)   {
                pEnd=strchr (pBegin+1, ')');
                if (pEnd)   {
                    *pEnd='\0';
                    nLine=atoi (pBegin+1);
                    *pBegin='\0';
                    ((CMainFrame*)AfxGetMainWnd())->GotoFileLine(strdblk (pBuffer), nLine);	
                    nB=0;
                }
            }
        }
    }
    return nB;
}

void CAdamoFindInFilesView::GotoNextFFLine ()
{
    CString str;
    LONG l=m_pSciView->GetEditor()->GetLineCount ();
    bool bFound=false;

    if (l)   {
        int i;

        for (i=m_nCurErrorLine+1; i<l; i++)   {
            str=m_pSciView->GetEditor()->GetLine (i);
            if (IsValidFFLine (str))   {
                GotoFileLine (str);
                m_nCurErrorLine=i;
                bFound=true;
                break;
            }
        }
        if (!bFound)   {
            for (i=0; i<=m_nCurErrorLine; i++)   {
                str=m_pSciView->GetEditor()->GetLine (i);
                if (IsValidFFLine (str))   {
                    GotoFileLine (str);
                    m_nCurErrorLine=i;
                    break;
                }
            }
        }
        if (m_nCurErrorLine!=-1)
            m_pSciView->GetEditor()->SelectLine (m_nCurErrorLine);
    }
}

void CAdamoFindInFilesView::GotoPrevFFLine ()
{
    CString str;
    LONG l=m_pSciView->GetEditor()->GetLineCount ();
    int i;
    bool bFound=false;

    if (l)   {
        for (i=m_nCurErrorLine-1; i>=0; i--)   {
            str=m_pSciView->GetEditor()->GetLine (i);
            if (IsValidFFLine (str))   {
                GotoFileLine (str);
                m_nCurErrorLine=i;
                bFound=true;
                break;
            }
        }
        if (!bFound)   {
            for (i=l-1; i>=m_nCurErrorLine; i--)   {
                str=m_pSciView->GetEditor()->GetLine (i);
                if (IsValidFFLine (str))   {
                    GotoFileLine (str);
                    m_nCurErrorLine=i;
                    break;
                }
            }
        }
        if (m_nCurErrorLine!=-1)
            m_pSciView->GetEditor()->SelectLine (m_nCurErrorLine);
    }
}

bool CAdamoFindInFilesView::IsValidFFLine (CString str)
{
    char *szBuffer=new char [str.GetLength ()+1];
    char *p;
    bool b=false;

    strcpy (szBuffer, str);
    p=strchr (szBuffer, ' ');
    if (p)   {
        *p='\0';
        if (!_access (szBuffer, 00))
            b=true;
    }
    delete szBuffer;
    return b;
}
    