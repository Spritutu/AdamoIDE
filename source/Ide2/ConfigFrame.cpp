// ConfigFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "MainFrame.h"
#include "DockDevProperty.h"
#include "ConfTreeView.h"

#include "HTMLCtrl.h"
#include "ConfigFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigFrame

IMPLEMENT_DYNCREATE(CConfigFrame, CMDIChildWnd)

CConfigFrame::CConfigFrame() : m_pLogicConf (NULL), m_pHWConf (NULL)
{
}

CConfigFrame::~CConfigFrame()
{
}

BEGIN_MESSAGE_MAP(CConfigFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CConfigFrame)
	ON_WM_DESTROY()
	ON_COMMAND(ID_MODE_LOGIC, OnModeLogic)
	ON_COMMAND(ID_MODE_HARDWARE, OnModeHardware)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigFrame message handlers

void CConfigFrame::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::ActivateFrame(SW_MAXIMIZE);	
}

void CConfigFrame::OnDestroy() 
{
	CMDIChildWnd::OnDestroy();
	
    CMainFrame *pFrame = (CMainFrame *) AfxGetMainWnd ();
    if (::IsWindow (pFrame)){
        CDockDevProperty *pDock = pFrame->GetDockDevProperty();
        if(pDock)
            pDock->SetObject(NULL);
    }	
}

/*
** OnModeHardware : modo hardware
*/
void  CConfigFrame::OnModeHardware() 
{
    if (!m_pHWConf)
        CreateConfHWView ();
	if (!m_pLogicConf)
        m_pLogicConf=(CConfTreeView *)GetActiveView ();
    if (GetActiveView ()==m_pLogicConf)   {
        CMainFrame *pMainFrame=(CMainFrame *)GetMDIFrame();
        if (pMainFrame)   {
            pMainFrame->SetLayout (pMainFrame->GetLayoutHW());
            pMainFrame->SetMode (CMainFrame::modeHardware);
        }
        GestisciMappaEId ();
        SwitchView ();
    }
}
	
/*
** OnModeLogic : modo logico
*/
void CConfigFrame::OnModeLogic() 
{
    if (GetActiveView ()==m_pHWConf)   {
        CMainFrame *pMainFrame=(CMainFrame *)GetMDIFrame();
        if (pMainFrame)   {
            pMainFrame->SetLayout (pMainFrame->GetLayoutConfig());
            pMainFrame->SetMode (CMainFrame::modeLogic);
        }
        SwitchView ();
		m_pLogicConf->SearchItemDrop (true);
    }
}

/*
** CreateConfHWView :
*/
int CConfigFrame::CreateConfHWView ()
{
	/* creiamo la view hardware */
    m_pHWConf=new CAdamoConfHWView;
    CDocument* pCurrentDoc = GetActiveDocument();
    CCreateContext newContext;
    newContext.m_pNewViewClass = NULL;
    newContext.m_pNewDocTemplate = NULL;
    newContext.m_pLastView = NULL;
    newContext.m_pCurrentFrame = NULL;
    newContext.m_pCurrentDoc = pCurrentDoc;
    UINT viewID = AFX_IDW_PANE_FIRST + 1;
    CRect rect(0, 0, 0, 0); // Gets resized later.
    m_pHWConf->Create(NULL, NULL, WS_CHILD, rect, this, viewID, &newContext);
    m_pHWConf->SendMessage(WM_INITIALUPDATE, 0, 0);
	return 0;
}

/*
** SwitchView :
*/
CView *CConfigFrame::SwitchView () 
{
    CView* pActiveView=GetActiveView(), *pNewView;
    if (pActiveView == m_pLogicConf)
      pNewView= m_pHWConf;
    else
      pNewView= m_pLogicConf;
    // Exchange view window IDs so RecalcLayout() works.
    #ifndef _WIN32
    UINT temp = ::GetWindowWord(pActiveView->m_hWnd, GWW_ID);
    ::SetWindowWord(pActiveView->m_hWnd, GWW_ID, ::GetWindowWord(pNewView->m_hWnd, GWW_ID));
    ::SetWindowWord(pNewView->m_hWnd, GWW_ID, temp);
    #else
    UINT temp = ::GetWindowLong(pActiveView->m_hWnd, GWL_ID);
    ::SetWindowLong(pActiveView->m_hWnd, GWL_ID, ::GetWindowLong(pNewView->m_hWnd, GWL_ID));
    ::SetWindowLong(pNewView->m_hWnd, GWL_ID, temp);
    #endif

    pActiveView->ShowWindow(SW_HIDE);
    pNewView->ShowWindow(SW_MAXIMIZE);
    SetActiveView(pNewView);
    RecalcLayout();
    pNewView->Invalidate();
    return pActiveView;
}

/*
** GestisciMappaEId :
*/
void CConfigFrame::GestisciMappaEId ()
{
    CProject *pPrj=GETWS()->GetActiveProject ();
    if (pPrj)
        pPrj->GestisciMappaEId ();
}

/*
** OnChangePsw :
*/
void CConfigFrame::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    CView *pView=GetActiveView ();
    if (pView->IsKindOf(RUNTIME_CLASS (CAdamoConfHWView)))
        ((CAdamoConfHWView *)pView)->OnChangePsw (oldPsw, newPsw);
    else
    if (pView->IsKindOf(RUNTIME_CLASS (CConfTreeView)))
        ((CConfTreeView *)pView)->OnChangePsw (oldPsw, newPsw);
}
