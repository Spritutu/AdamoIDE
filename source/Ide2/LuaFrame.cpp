// LuaFrame.cpp : implementation of the CLuaFrame class
//

#include "stdafx.h"
#include "ide2.h"

#include "LuaFrame.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLuaFrame

CLuaFrame* CLuaFrame::m_pThis=NULL;

IMPLEMENT_DYNCREATE(CLuaFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CLuaFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLuaFrame)
                ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLuaFrame construction/destruction

CLuaFrame::CLuaFrame()
{
	m_strWindowPos = _T("");	
    m_pThis=this;
}

CLuaFrame::~CLuaFrame()
{
}

BOOL CLuaFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLuaFrame diagnostics

#ifdef _DEBUG
void CLuaFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CLuaFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLuaFrame message handlers

BOOL CLuaFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

void CLuaFrame::ActivateFrame(int nCmdShow) 
{
	//m_strWindowPos.Format(IDS_REG_MDI_WNDPOS, 1);

	// then restore the previous window position.
	//m_wndPosition.LoadWindowPos(this, m_strWindowPos);
	
	CMDIChildWnd::ActivateFrame(SW_MAXIMIZE);
}

void CLuaFrame::OnDestroy() 
{
	// Save frame window size and position.
	//m_wndPosition.SaveWindowPos(this, m_strWindowPos);
    CMDIChildWnd::OnDestroy();
}

void CLuaFrame::OnTimer() 
{
    ((CMainFrame *) AfxGetMainWnd ())->TimerDialogList ();
}

void CLuaFrame::OnClose() 
{
    ((CMainFrame *) AfxGetMainWnd ())->CloseDialogList ();
    ((CMainFrame *) AfxGetMainWnd ())->RemoveDialogList ();
}

/*
** OnChangePsw :
*/
void CLuaFrame::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    ePswLevel e = ((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    bool b=true;

    if (e<=ePswService)
        b=false;
    ((CMainFrame *)AfxGetMainWnd ())->SetReadOnly (b);
}
