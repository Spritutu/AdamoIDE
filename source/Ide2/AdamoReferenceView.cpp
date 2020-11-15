#include "stdafx.h"
#include "ide2.h"
#include "AdamoReferenceView.h"
#include "ScintillaView.h"
#include "MainFrame.h"
#include "resource.h"

CAdamoReferenceView::CAdamoReferenceView(void) : m_pSciView (NULL)
{
}

CAdamoReferenceView::~CAdamoReferenceView(void)
{
}

/*
** Init : inizializzazione finestra.
*/
void CAdamoReferenceView::Init(CWnd *pWnd, int nId)
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
void CAdamoReferenceView::Clear ()
{
    if (m_pSciView)
    	m_pSciView->Clear();
}

/*
** OnSci : la funzione e' chiamata su una notifica dello scntilla
*/
int CAdamoReferenceView::OnSci(CWnd* pView, SCNotification* pNotify)
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

/*
** Add : aggiunge un file trovato dal find in files all'editor
*/
void CAdamoReferenceView::Add (const char* szPath, int nLine, int nItem)
{
    char szBuffer[_MAX_PATH+512+1];
    if (m_pSciView)   {
        if (strlen (szPath)>_MAX_PATH)
            return;
        sprintf (szBuffer, "%s (%d) (%d)\n", szPath, nLine, nItem);
    	m_pSciView->Write(szBuffer);
    }
}

/*
** WritePrologue : aggiunge un file trovato dal find in files all'editor
*/
void CAdamoReferenceView::WritePrologue (CString strSymbol, int nRefCount)
{
	CString str;
	str.Format ("%s %s - %s %d\n\n", LOADSTRING (IDS_ADAMOVIEWREFERENCES_1), strSymbol, LOADSTRING (IDS_ADAMOVIEWREFERENCES_2), nRefCount);
	m_pSciView->Write(str);
	m_strSymbol = strSymbol;
}

/*
** WritePrologue : aggiunge un file trovato dal find in files all'editor
*/
void CAdamoReferenceView::WritePrologue ()
{
	m_pSciView->Write(LOADSTRING (IDS_ADAMOMAINFRAME_37));
}

/*
** GotoFileLine :
*/
int CAdamoReferenceView::GotoFileLine (CString str)
{
	CLuaView* pLuaView;
	CString strSymbol;
    int   nLine, nItem, nB=-1;
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
					pBegin = pEnd + 1;
					pBegin=strchr (pBegin, '(');
					if (pBegin)   {
						pBegin++;
						pEnd=strchr (pBegin, ')');
						*pEnd = '\0';
						nItem = atoi (pBegin);
						pLuaView = (CLuaView*) ((CMainFrame*)AfxGetMainWnd())->GotoFileLine(strdblk (pBuffer), nLine, FALSE, FALSE);
						pLuaView->GetEditor ()->SelectWord (m_strSymbol, nLine, nItem + 1);
						nB=0;
					}
                }
            }
        }
    }
    return nB;
}
