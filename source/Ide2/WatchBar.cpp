// WatchBar.cpp: implementation of the CWatchBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "WatchBar.h"
#include "WatchListView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void CWatchBar::Redraw()
{
    CWnd *pWnd = m_pDockingPane->GetChild();

    if(pWnd)
        ((CWatchListView*)pWnd)->Redraw();    
}

void CWatchBar::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pWatchListView = (CWatchListView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CWatchListView));
}
