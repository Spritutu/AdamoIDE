// DockLangView.cpp: implementation of the CDockLangView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mainframe.h"
#include "DockHelpOnLineHtml.h"
#include "HelpOnLineHtmlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockLangView

CDockHelpOnLineHtml::CDockHelpOnLineHtml() : m_pHelpOnLineHtmlView (NULL)
{
}

CDockHelpOnLineHtml::~CDockHelpOnLineHtml()
{
}

void CDockHelpOnLineHtml::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)
		m_pHelpOnLineHtmlView = (CHtmlHelpOnLineView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CHtmlHelpOnLineView));
}

