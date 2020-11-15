// DockConfigTree.cpp: implementation of the CDockConfigTree class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DockConfigTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDockConfigTree::CDockConfigTree() : m_pTreeView (NULL)
{

}

CDockConfigTree::~CDockConfigTree()
{

}

void CDockConfigTree::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pTreeView = (CConfTreeView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CConfTreeView));
}

void CDockConfigTree::Enable(BOOL bEnable)
{
    if(m_pTreeView)
        m_pTreeView->EnableWindow(bEnable);
}
