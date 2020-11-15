// AdamoDGEthercat.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "workspace.h"
#include "mainframe.h"
#include "AdamoDGEthercat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGEthercat

IMPLEMENT_DYNCREATE(CAdamoDGEthercat, CView)

CAdamoDGEthercat::CAdamoDGEthercat() : m_pMachine (NULL), m_pEthercatGrid (NULL), m_bSingleSlave (false), m_ePswLevel (ePswUser)
{
}

CAdamoDGEthercat::~CAdamoDGEthercat()
{
}


BEGIN_MESSAGE_MAP(CAdamoDGEthercat, CView)
	//{{AFX_MSG_MAP(CAdamoDGEthercat)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(IDC_BTN_ETHERCAT_INIT, OnBtnEthercatInit)
	ON_COMMAND(IDC_BTN_ETHERCAT_INITNETWORK, OnBtnEthercatInitNetwork)
	ON_COMMAND(IDC_BTN_ETHERCAT_OPERATIONAL, OnBtnEthercatOperational)
	ON_COMMAND(IDC_BTN_ETHERCAT_PROPERATIONAL, OnBtnEthercatProperational)
	ON_COMMAND(IDC_BTN_ETHERCAT_SAFEOPERATIONAL, OnBtnEthercatSafeOperational)
	ON_UPDATE_COMMAND_UI(IDC_BTN_ETHERCAT_INIT, OnUpdateBtnEthercatInit)
	ON_UPDATE_COMMAND_UI(IDC_BTN_ETHERCAT_INITNETWORK, OnUpdateBtnEthercatInitNetwork)
	ON_UPDATE_COMMAND_UI(IDC_BTN_ETHERCAT_OPERATIONAL, OnUpdateBtnEthercatOperational)
	ON_UPDATE_COMMAND_UI(IDC_BTN_ETHERCAT_PROPERATIONAL, OnUpdateBtnEthercatProperational)
	ON_UPDATE_COMMAND_UI(IDC_BTN_ETHERCAT_SAFEOPERATIONAL, OnUpdateBtnEthercatSafeOperational)
	ON_COMMAND(IDC_BTN_EC_READYON, OnBtnEthercatReadyOn)
	ON_COMMAND(IDC_BTN_EC_READYOFF, OnBtnEthercatReadyOff)
	ON_COMMAND(IDC_BTN_EC_SERVOON, OnBtnEthercatServoOn)
	ON_COMMAND(IDC_BTN_EC_SERVOOFF, OnBtnEthercatServoOff)
	ON_COMMAND(IDC_BTN_EC_RESETALARM, OnBtnEthercatResetAlarm)
	ON_UPDATE_COMMAND_UI(IDC_BTN_EC_READYON, OnUpdateBtnEthercatReadyOn)
	ON_UPDATE_COMMAND_UI(IDC_BTN_EC_READYOFF, OnUpdateBtnEthercatReadyOff)
	ON_UPDATE_COMMAND_UI(IDC_BTN_EC_SERVOON, OnUpdateBtnEthercatServoOn)
	ON_UPDATE_COMMAND_UI(IDC_BTN_EC_SERVOOFF, OnUpdateBtnEthercatServoOff)
	ON_UPDATE_COMMAND_UI(IDC_BTN_EC_RESETALARM, OnUpdateBtnEthercatResetAlarm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGEthercat drawing

void CAdamoDGEthercat::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGEthercat diagnostics

#ifdef _DEBUG
void CAdamoDGEthercat::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoDGEthercat::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGEthercat message handlers

/*
** OnCreate :
*/
int CAdamoDGEthercat::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
    m_pEthercatGrid=new CAdamoEthercatGrid;
	m_pEthercatGrid->Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,0x600);
    m_pEthercatGrid->InitializeGrid ();
    SetMachine ();
	m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	return 0;
}

/*
** SetMachine :
*/
void CAdamoDGEthercat::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}

/*
** OnDestroy :
*/
void CAdamoDGEthercat::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    SAFE_DELETE (m_pEthercatGrid);
}

void CAdamoDGEthercat::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_pEthercatGrid)
        m_pEthercatGrid->MoveWindow (0, 0, cx, cy);
}

/*
** OnAdamoTimer :
*/
void CAdamoDGEthercat::OnAdamoTimer ()
{
    if (m_pEthercatGrid)
        m_pEthercatGrid->RinfrescaInformazioni ();
}

void CAdamoDGEthercat::OnBtnEthercatInit() 
{
	// TODO: Add your command handler code here
	if (m_pEthercatGrid)
		m_pEthercatGrid->RequestState (DEVICE_STATE_INIT, m_bSingleSlave);
}

void CAdamoDGEthercat::OnBtnEthercatInitNetwork() 
{
	// TODO: Add your command handler code here
}

void CAdamoDGEthercat::OnBtnEthercatOperational() 
{
	// TODO: Add your command handler code here
	if (m_pEthercatGrid)
		m_pEthercatGrid->RequestState (DEVICE_STATE_OP, m_bSingleSlave);
}

void CAdamoDGEthercat::OnBtnEthercatProperational() 
{
	// TODO: Add your command handler code here
	if (m_pEthercatGrid)
		m_pEthercatGrid->RequestState (DEVICE_STATE_PREOP, m_bSingleSlave);
}

void CAdamoDGEthercat::OnBtnEthercatSafeOperational()
{
	// TODO: Add your command handler code here
	if (m_pEthercatGrid)
		m_pEthercatGrid->RequestState (DEVICE_STATE_SAFEOP, m_bSingleSlave);
}

/*
** OnChangePsw :
*/
void CAdamoDGEthercat::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
	m_ePswLevel=newPsw;
}

/*
** OnUpdateBtnEthercatInit 
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatInit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnEthercatInitNetwork 
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatInitNetwork(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnEthercatOperational 
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatOperational(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnEthercatProperational 
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatProperational(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnEthercatSafeOperational 
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatSafeOperational(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnBtnEthercatReadyOn :
*/
void CAdamoDGEthercat::OnBtnEthercatReadyOn() 
{
	int nSlave;
	// TODO: Add your command handler code here
	if (m_pEthercatGrid)   {
		if (m_bSingleSlave)   {
			nSlave = m_pEthercatGrid->GetSelectedSlave ();
			m_pMachine->EcReadyOn (nSlave, 1);
		}
		else
			m_pMachine->EcReadyOn (-1, 1);
	}
}

/*
** OnBtnEthercatReadyOff :
*/
void CAdamoDGEthercat::OnBtnEthercatReadyOff() 
{
	// TODO: Add your command handler code here
	if (m_pEthercatGrid)   {
		if (m_bSingleSlave)   {
			int nSlave = m_pEthercatGrid->GetSelectedSlave ();
			m_pMachine->EcReadyOn (nSlave, 0);
		}
		else
			m_pMachine->EcReadyOn (-1, 0);
	}
}

/*
** OnBtnEthercatServoOn :
*/
void CAdamoDGEthercat::OnBtnEthercatServoOn() 
{
	if (m_pEthercatGrid)   {
		if (m_bSingleSlave)   {
			int nSlave = m_pEthercatGrid->GetSelectedSlave ();
			m_pMachine->EcServoOn (nSlave, 1);
		}
		else
			m_pMachine->EcServoOn (-1, 1);
	}
}

/*
** OnBtnEthercatServoOff :
*/
void CAdamoDGEthercat::OnBtnEthercatServoOff() 
{
	// TODO: Add your command handler code here
	if (m_pEthercatGrid)   {
		if (m_bSingleSlave)   {
			int nSlave = m_pEthercatGrid->GetSelectedSlave ();
			m_pMachine->EcServoOn (nSlave, 0);
		}
		else
			m_pMachine->EcServoOn (-1, 0);
	}
}

/*
** OnBtnEthercatResetErrors :
*/
void CAdamoDGEthercat::OnBtnEthercatResetAlarm() 
{
	// TODO: Add your command handler code here
	if (m_pEthercatGrid)   {
		if (m_bSingleSlave)   {
			int nSlave = m_pEthercatGrid->GetSelectedSlave ();
			m_pMachine->EcResetAlarm (nSlave);
		}
		else
			m_pMachine->EcResetAlarm (-1);
	}
}

/*
** OnUpdateBtnEthercatReadyOn :
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatReadyOn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnEthercatReadyOff :
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatReadyOff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnEthercatServoOn :
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatServoOn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnEthercatServoOff :
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatServoOff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnEthercatResetErrors :
*/
void CAdamoDGEthercat::OnUpdateBtnEthercatResetAlarm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel<=ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}
