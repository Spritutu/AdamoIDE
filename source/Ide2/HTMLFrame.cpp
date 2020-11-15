// HTMLFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "HTMLFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTMLFrame

IMPLEMENT_DYNCREATE(CHTMLFrame, CMDIChildWnd)

CHTMLFrame::CHTMLFrame()
{
}

CHTMLFrame::~CHTMLFrame()
{
}


BEGIN_MESSAGE_MAP(CHTMLFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CHTMLFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTMLFrame message handlers

void CHTMLFrame::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::ActivateFrame(SW_MAXIMIZE);	
}
