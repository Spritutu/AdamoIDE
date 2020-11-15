// AdamoDlgShowModules.cpp : file di implementazione
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "SuperGridCtrl.h"
#include "AdamoDlgShowModules.h"


// finestra di dialogo CAdamoDlgShowModules

IMPLEMENT_DYNAMIC(CAdamoDlgShowModules, CDialog)

CAdamoDlgShowModules::CAdamoDlgShowModules(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoDlgShowModules::IDD, pParent)
{

}

CAdamoDlgShowModules::~CAdamoDlgShowModules()
{
}

void CAdamoDlgShowModules::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_FRAME, m_stFrame);
}


BEGIN_MESSAGE_MAP(CAdamoDlgShowModules, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_RECONNECT, &CAdamoDlgShowModules::OnBnClickedBtnReconnect)
	ON_BN_CLICKED(IDC_BTN_ACTIVATE, &CAdamoDlgShowModules::OnBnClickedBtnActivate)
    ON_MESSAGE (WM_ACTIVATE_MACHINE, OnActivateMachine)
END_MESSAGE_MAP()


// gestori di messaggi CAdamoDlgShowModules
int CAdamoDlgShowModules::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_sg.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_SHOWSELALWAYS, CRect(0,0,0,0),this,0x1000);
	m_sg.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	return 0;
}

/*
** OnInitDialog :
*/
BOOL CAdamoDlgShowModules::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (m_sg.GetSafeHwnd())   {
		CRect rc;
		InizializzaBottoni ();
		m_stFrame.GetWindowRect(rc);
		ScreenToClient(rc);
	    m_sg.MoveWindow(rc);
		m_sg.InitializeGrid ();
	}
	return TRUE;
}

/*
** InizializzaBottoni :
*/
void CAdamoDlgShowModules::InizializzaBottoni () 
{
	BOOL bEnable = FALSE;

	SetWindowText (LOADSTRING (IDS_MODULES_TITLE));
	GetDlgItem (IDC_BTN_ACTIVATE)->SetWindowText (LOADSTRING (IDS_ACTIVATE_MACHINE));
	GetDlgItem (IDC_BTN_RECONNECT)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_MODULE_CONNECT));
	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	if (pPlant->GetCount () == 1)
		GetDlgItem (IDC_BTN_ACTIVATE)->EnableWindow (FALSE);
	CProject *pPrj = pPlant->GetFirst ();
	while (pPrj)   {
		if (pPrj->GetMachine()->GetConnectionType () == enTCPConnection && !pPrj->GetMachine()->IsCncOnline ())   {
			bEnable = TRUE;
			break;
		}
		pPrj = (CProject *) pPlant->GetNext ();
	}
	GetDlgItem (IDC_BTN_RECONNECT)->EnableWindow (bEnable);
}

/*
** OnBnClickedBtnReconnect :
*/
void CAdamoDlgShowModules::OnBnClickedBtnReconnect()
{
	CString str;

	/* per prima cosa vediamo il modulo selezionato */
	BeginWaitCursor ();
	CProject *pPrj = m_sg.GetSelectedPrj ();
	if (pPrj)   {
		/* tentiamo la riconnessione */
		if (!pPrj->ReconnectMachine (pPrj->GetMachine()->GetConnectionType ()))   {
			/* e' andata male */
			EndWaitCursor ();
			str.Format ("%s %s", LOADSTRING (IDS_CONFIG_HW_MODULE_CANNOT_CONNECT), pPrj->GetMachineModule()->GetName());
			AfxMessageBox (str);
		}
		else   {
			pPrj->GetMachine()->Run ();
			m_sg.RefreshData ();
			EndWaitCursor ();
		}
	}
}

/*
** OnBnClickedBtnActivate :
*/
void CAdamoDlgShowModules::OnBnClickedBtnActivate ()
{
	int nActiveMode ;
	/* recuperiamo il progetto selezionato */
	BeginWaitCursor ();
	CProject *pActivePrj = GETWS ()->GetActiveProject ();
	CProject *pPrj = m_sg.GetSelectedPrj ();
	/* operiamo solo se si vuole attivare un'altra macchina */
	if (pPrj != pActivePrj)   {
		/* per prima cosa chiudiamo il modo corrente */
		if (((CMainFrame *)AfxGetMainWnd ())->GetTemplate ()!=enNoTemplate)   {
			pActivePrj->SetActiveMode (((CMainFrame *)AfxGetMainWnd ())->GetTemplate ());
			((CMainFrame *)AfxGetMainWnd ())->OnCloseMode ();
		}
		/* ora settiamo la macchina corrente */
		if (pPrj)
			/* comunichiamo al workspace che si vuole cambiare la macchina attiva */
			GETWS()->ActivateMachine (pPrj->GetModuleNumber());
		/* aggiorniamo la title bar */
		((CMainFrame *) AfxGetMainWnd ())->UpdateActiveProject();
		/* apriamo la pagina di programma che era aperta prima */
		nActiveMode = pPrj->GetActiveMode ();
		if (nActiveMode != -1)
			((CMainFrame *) AfxGetMainWnd ())->OpenTemplate (nActiveMode);
		EndWaitCursor ();
		/* usciamo dal dialog box */
		OnOK ();
	}
}

/*
** OnActivateMachine :
*/
LRESULT CAdamoDlgShowModules::OnActivateMachine (WPARAM wParam, LPARAM lParam)
{
	OnBnClickedBtnActivate ();
	return 0;
}
