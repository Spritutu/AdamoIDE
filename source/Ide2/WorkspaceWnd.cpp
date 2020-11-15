// WorkspaceWnd.cpp: implementation of the CWorkspaceWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "WorkspaceWnd.h"
#include "TreeViewFiles.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWorkspaceWnd::CWorkspaceWnd():m_pTreeView(NULL)
{
}

CTreeViewFiles* CWorkspaceWnd::GetTreeViewFiles()
{
	return m_pTreeView;
}

void CWorkspaceWnd::Enable(BOOL bEnable)
{
    if(m_pTreeView)
        m_pTreeView->EnableWindow(bEnable);
}

void CWorkspaceWnd::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pTreeView = (CTreeViewFiles*)pPane->AttachView(pWnd,RUNTIME_CLASS(CTreeViewFiles));
}
