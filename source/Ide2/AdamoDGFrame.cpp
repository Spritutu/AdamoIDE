// AdamoDGFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoDGFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGFrame

IMPLEMENT_DYNCREATE(CAdamoDGFrame, CMDIChildWnd)

CAdamoDGFrame::CAdamoDGFrame()
{
}

CAdamoDGFrame::~CAdamoDGFrame()
{
}


BEGIN_MESSAGE_MAP(CAdamoDGFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAdamoDGFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGFrame message handlers
void CAdamoDGFrame::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::ActivateFrame(SW_MAXIMIZE);	
}
