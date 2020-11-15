// DockLangView.cpp: implementation of the CDockLangView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mainframe.h"
#include "DockHelpOnLine.h"
#include "HelpOnLineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockLangView

CDockHelpOnLine::CDockHelpOnLine() : m_pHelpOnLineView (NULL)
{
}

CDockHelpOnLine::~CDockHelpOnLine()
{
}

void CDockHelpOnLine::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);

	if (!pPane)   {
		pPane=((CMainFrame *)AfxGetMainWnd ())->GetPaneManager ()->CreatePane(ID_VIEW_HELP_ON_LINE, CRect(0, 0,200, 120), xtpPaneDockRight);
		m_pHelpOnLineView = (CHelpOnLineView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CHelpOnLineView));
	}
	else
		m_pHelpOnLineView = (CHelpOnLineView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CHelpOnLineView));
}

