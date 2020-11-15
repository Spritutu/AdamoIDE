// AdamoMDFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoMDFrame.h"
#include "AdamoMDSplittedView.h"
#include "AdamoMDAxesView.h"
#include "OscView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAdamoMDFrame* CAdamoMDFrame::m_pThis=NULL;

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDFrame message handlers
/////////////////////////////////////////////////////////////////////////////
// CAdamoMDFrame

IMPLEMENT_DYNCREATE(CAdamoMDFrame, CMDIChildWnd)

CAdamoMDFrame::CAdamoMDFrame() : m_hAccel (NULL)
{
    m_pThis=this;
}

CAdamoMDFrame::~CAdamoMDFrame()
{
}

void CAdamoMDFrame::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::ActivateFrame(SW_MAXIMIZE);	
}

int CAdamoMDFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
    /* carichiamo gli acceleratori */
    m_hAccel = LoadAccelerators(AfxGetInstanceHandle(),
        MAKEINTRESOURCE(IDR_ACCEL_MANDIAG));
    return 0;
}

BEGIN_MESSAGE_MAP(CAdamoMDFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAdamoMDFrame)
        ON_WM_DESTROY()
        ON_WM_CREATE()
        ON_COMMAND (IDA_PLUS, OnButtonPlus)
        ON_COMMAND (IDA_MINUS, OnButtonMinus)
        ON_COMMAND (IDA_STOP, OnButtonStop)
        ON_COMMAND (IDA_SETVEL, OnSetVel)
        ON_COMMAND (IDA_JOG, OnJog)
        ON_COMMAND (IDA_ABS, OnAbs)
        ON_COMMAND (IDA_STEP, OnStep)
        ON_COMMAND (IDA_OPENLOOP, OnOpenLoop)
        ON_COMMAND (IDA_CLOSELOOP, OnCloseLoop)
        ON_COMMAND (IDA_AXARROWUP, OnAxArrowUp)
        ON_COMMAND (IDA_AXARROWDOWN, OnAxArrowDown)
        ON_COMMAND (IDM_RUN_GO,   OnRunPlc)
		ON_UPDATE_COMMAND_UI(IDM_RUN_GO, OnUpdateRunPlc)
        ON_COMMAND (IDM_RUN_STOP, OnStopPlc)
		ON_UPDATE_COMMAND_UI(IDM_RUN_STOP, OnUpdateStopPlc)
        ON_COMMAND (ID_FORCEALLINPUT_OFF, OnForceAllInputsOff)
		ON_UPDATE_COMMAND_UI(ID_FORCEALLINPUT_OFF, OnUpdateForceAllInputsOff)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAdamoMDFrame::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message==WM_KEYUP&&m_btnPressed)   {
        CAdamoMDAxesView* pMDAxesView=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView());
        if (pMDAxesView)   {
            CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
            switch (m_btnPressed)   {
                case 1 :
                    pCmdBar->ReleasePlusButton ();
                    break;
                case 2 :
                    pCmdBar->ReleaseMinusButton ();
                    break;
                case 3 :
                    pCmdBar->ReleaseStopButton ();
                    break;
                case 4 :
                    pCmdBar->ReleaseOpenLoopButton ();
                    break;
                case 5 :
                    pCmdBar->ReleaseCloseLoopButton ();
                    break;
            }
            m_btnPressed=0;
        }
    }
	static int nMsgN = 0;
    if (m_hAccel != NULL &&
       ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
       return TRUE;
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

void CAdamoMDFrame::OnDestroy() 
{
    if (m_hAccel != NULL)
        DestroyAcceleratorTable (m_hAccel);
}

void CAdamoMDFrame::OnButtonPlus ()
{
    CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
    pCmdBar->PushPlusButton ();
    m_btnPressed=1;
}

void CAdamoMDFrame::OnButtonMinus ()
{
    CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
    pCmdBar->PushMinusButton ();
    m_btnPressed=2;
}

void CAdamoMDFrame::OnButtonStop ()
{
    CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
    pCmdBar->PushStopButton ();
    m_btnPressed=3;
}

void CAdamoMDFrame::OnSetVel ()
{
    CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
    pCmdBar->SetVel ();
    m_btnPressed=3;
}

void CAdamoMDFrame::OnJog ()
{
    CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
    pCmdBar->SetJogMode ();
}

void CAdamoMDFrame::OnAbs ()
{
    CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
    pCmdBar->SetAbsMode ();
}

void CAdamoMDFrame::OnStep ()
{
    CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
    pCmdBar->SetStepMode ();
}

void CAdamoMDFrame::OnOpenLoop ()
{
    CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
    pCmdBar->PushOpenLoopButton ();
    m_btnPressed=4;
}

void CAdamoMDFrame::OnCloseLoop ()
{
    CAdamoMDCommandBar* pCmdBar=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetCommandBar ();
    pCmdBar->PushCloseLoopButton ();
    m_btnPressed=5;
}

void CAdamoMDFrame::OnAxArrowUp ()
{
    CAdamoMDAxStatus* pAxStatus=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetAxStatus ();
    pAxStatus->OnFrecciaSu ();
}

void CAdamoMDFrame::OnAxArrowDown ()
{
    CAdamoMDAxStatus* pAxStatus=((CAdamoMDAxesView*)CAdamoMDView::GetMDView()->GetAxesView())->GetAxStatus ();
    pAxStatus->OnFrecciaGiu ();
}

void CAdamoMDFrame::OnTimer()
{
    CAdamoMDSplittedView* pMDIOView=CAdamoMDView::GetMDView()->GetIOView();
    if (pMDIOView)
        pMDIOView->OnAdamoTimer();
    CAdamoMDAxesView* pMDAxesView=CAdamoMDView::GetMDView()->GetAxesView();
    if (pMDAxesView)
        pMDAxesView->OnAdamoTimer();
    ((CMainFrame *) AfxGetMainWnd ())->TimerDialogList ();
}

void CAdamoMDFrame::OnClose()
{
    ((CMainFrame *) AfxGetMainWnd ())->CloseDialogList ();
    ((CMainFrame *) AfxGetMainWnd ())->RemoveDialogList ();
}

void CAdamoMDFrame::OnRunPlc ()
{
    ((CMainFrame *) AfxGetMainWnd ())->OnBuildGoFromMD ();
}

void CAdamoMDFrame::OnUpdateRunPlc (CCmdUI* pCmdUI)
{
	ePswLevel eLevel = ((CMainFrame *) AfxGetMainWnd ())->GetPswLevel ();
	if (eLevel < ePswService)
		pCmdUI->Enable ();
	else
		pCmdUI->Enable (FALSE);
}

void CAdamoMDFrame::OnStopPlc ()
{
    ((CMainFrame *) AfxGetMainWnd ())->OnDebugStopdebuggingFromMD ();
}

void CAdamoMDFrame::OnUpdateStopPlc (CCmdUI* pCmdUI)
{
	ePswLevel eLevel = ((CMainFrame *) AfxGetMainWnd ())->GetPswLevel ();
	if (eLevel < ePswService)
		pCmdUI->Enable ();
	else
		pCmdUI->Enable (FALSE);
}

void CAdamoMDFrame::OnForceAllInputsOff ()
{
    ((CMainFrame *) AfxGetMainWnd ())->OnForceAllInputsOff ();
}

void CAdamoMDFrame::OnUpdateForceAllInputsOff (CCmdUI* pCmdUI)
{
	ePswLevel eLevel = ((CMainFrame *) AfxGetMainWnd ())->GetPswLevel ();
	if (eLevel <= ePswBuilder)
		pCmdUI->Enable ();
	else
		pCmdUI->Enable (FALSE);
}
