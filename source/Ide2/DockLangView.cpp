// DockLangView.cpp: implementation of the CDockLangView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DockLangView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockLangView

CDockLangView::CDockLangView() : m_pLangView (NULL)
{
}

CDockLangView::~CDockLangView()
{
}

void CDockLangView::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pLangView = (CLangView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CLangView));
}

