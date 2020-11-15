// MDIClientWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "MDIClientWnd.h"

#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDIClientWnd

CMDIClientWnd::CMDIClientWnd()
{
}

CMDIClientWnd::~CMDIClientWnd()
{
}


BEGIN_MESSAGE_MAP(CMDIClientWnd, CWnd)
	//{{AFX_MSG_MAP(CMDIClientWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMDIClientWnd message handlers

LRESULT CMDIClientWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
    if(message == WM_MDIREFRESHMENU){
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
        if (::IsWindow (pFrame))
	        pFrame->SwitchMenu();	
	}

	return CWnd::WindowProc(message, wParam, lParam);
}
