// CallStack.cpp: implementation of the CCallStack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "CallStack.h"

#include "ScintillaView.h"
#include "MainFrame.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CCallStack::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)   {
        m_pSciView = (CScintillaView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CScintillaView));
        CLuaEditor* pEditor = m_pSciView->GetEditor();
	    pEditor->SetCallStackMargins();
	    Clear();
    }
}

int CCallStack::OnSci(CWnd* pView, SCNotification* pNotify)
{
    if(m_pSciView){
	    CLuaEditor* pEditor = m_pSciView->GetEditor();

	    CPoint pt;
	    int nLine;
	    CString strLine;
	    switch (pNotify->nmhdr.code)
	    {
		    case SCN_DOUBLECLICK:
			    GetCursorPos(&pt);
			    pEditor->ScreenToClient(&pt);
			    nLine = pEditor->LineFromPoint(pt);
			    GotoStackTraceLevel(nLine);
		    break;
	    };
    }

	return 0;
}

void CCallStack::Clear()
{
    if(m_pSciView)
    	m_pSciView->Clear();

	m_nCurrentLevel = -1;
	m_lines.RemoveAll();
	m_files.RemoveAll();
}

void CCallStack::Add(const char *szDesc, const char *szFile, int nLine)
{
    if(m_pSciView)
    	m_pSciView->Write(CString(szDesc)+"\n");

	m_files.Add(szFile);
	m_lines.Add(nLine);
}

void CCallStack::GotoStackTraceLevel(int nLevel)
{
	if ( nLevel<0 || nLevel>=m_files.GetSize() )
		return;

	m_nCurrentLevel = nLevel;

    if(m_pSciView){
	    CLuaEditor* pEditor = m_pSciView->GetEditor();
	    pEditor->SetStackTraceLevel(nLevel);
    }

	((CMainFrame*)AfxGetMainWnd())->GotoFileLine(m_files[nLevel], m_lines[nLevel]);	
	((CMainFrame*)AfxGetMainWnd())->GetDebugger()->StackLevelChanged();
}
