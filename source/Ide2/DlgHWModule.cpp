// DlgHWCanOpenNet.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "MainFrame.h"
#include "AdamoTemplates.h"
#include "dlghwmodule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CDlgHWModule, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CDlgHWModule dialog


CDlgHWModule::CDlgHWModule(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHWModule::IDD, pParent), m_bInitialized (false), m_pFontModule (NULL), m_pPrj (NULL), m_bIPCtrlInitialized (false), m_bIPCtrlToggled (false)
{
	//{{AFX_DATA_INIT(CDlgHWModule)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHWModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHWModule)
	DDX_Control(pDX, IDC_HW_MODULE_ST_MACHINE, m_stMachineTitle);
	DDX_Control(pDX, IDC_HW_MODULE_ST_IPADDRESS, m_stIpAddress);
	DDX_Control(pDX, IDC_HW_MODULE_ST_PORT, m_stPort);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_HW_MODULE_IPADDRESS, m_ctrlIPAddress);
	DDX_Control(pDX, IDC_HW_MODULE_CONNECT, m_ctrlConnect);
}

BEGIN_MESSAGE_MAP(CDlgHWModule, CDialog)
	//{{AFX_MSG_MAP(CDlgHWModule)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_HW_MODULE_EDIT_PORT, &CDlgHWModule::OnEnChangeHwModuleEditPort)
	ON_BN_CLICKED(IDC_HW_MODULE_CONNECT, &CDlgHWModule::OnBnClickedHwModuleConnect)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_HW_MODULE_IPADDRESS, &CDlgHWModule::OnIpnFieldchangedHwModuleIpaddress)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHWModule message handlers

BOOL CDlgHWModule::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitializeGrid ();
	InitTitles ();
	InitData ();
	InitPsw ();
	CreateFont ();
	m_bInitialized = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** Save : inizializza le stringhe
*/
void CDlgHWModule::Save ()
{
	SaveData ();
}

/*
** OnDestroy :
*/
void CDlgHWModule::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SaveData ();
}

/*
** SaveData :
*/
void CDlgHWModule::SaveData ()
{
	BYTE achIPAddress[IPADDRESS_BUFFER];

	if (m_pPrj)   {
		CAdamoMachine *pMachine = m_pPrj->GetMachine ();
		if (pMachine)   {
			pMachine->SetHWModuleNumber (m_nKinemaAddress);
			if (m_Grid.GetModuleName () != "")   {
				GetIPAddress (achIPAddress);
				pMachine->SetTcpIpAddress (achIPAddress);
				pMachine->SetPort (GetIPPort ());
				pMachine->SetTypeModule (GetModuleType ());
			}
			else   {
				pMachine->SetTypeModule (enNoModule);
				memset (achIPAddress, 0, sizeof (achIPAddress));
				pMachine->SetTcpIpAddress (achIPAddress);
				pMachine->SetPort (0);
			}
		}
	}
}

/*
** GetIPAddress :
*/
void CDlgHWModule::GetIPAddress (BYTE *achIPAddress)
{
	m_ctrlIPAddress.GetAddress (achIPAddress[0], achIPAddress[1], achIPAddress[2], achIPAddress[3]);
}

/*
** GetIPPort :
*/
int CDlgHWModule::GetIPPort ()
{
	CString str;

	m_EditIPPort.GetWindowText (str);
	return atol (str);
}

/*
** InitPsw :
*/
void CDlgHWModule::InitPsw ()
{
}

/*
** OnChangePsw :
*/
void CDlgHWModule::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}

/*
** InitializeGrid
*/
void CDlgHWModule::InitializeGrid ()
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_Grid.Create (rc, this, IDC_GRID_HW_MODULE, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
	m_Grid.InitData ();
	m_Grid.SetParentWnd (this);
}

void CDlgHWModule::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	int nDescSize;

	// TODO: aggiungere qui il codice per la gestione dei messaggi.
    if (m_bInitialized)   {
        m_stMachineTitle.MoveWindow (10, 10, 100, 40);
        m_Grid.MoveWindow (10, 40, cx-229, 2*19+10);
		nDescSize = cx-229-10-m_Grid.GetColumnWidth (0) * 3;
		if (nDescSize < m_Grid.GetColumnWidth (0))
			nDescSize = m_Grid.GetColumnWidth (0);
		m_Grid.SetColumnWidth (2, nDescSize);
        m_Grid.ExpandLastColumn();
        ((CWnd &)m_Grid).EnableScrollBarCtrl (SB_HORZ, FALSE);
        ((CWnd &)m_Grid).EnableScrollBarCtrl (SB_VERT, FALSE);
	}
}

/*
** InitTitles :
*/
void CDlgHWModule::InitTitles()
{
	CString strMachineTitle;

	strMachineTitle.Format ("%s %d", LOADSTRING (IDS_ADAMOERRORSDLG_3), m_nKinemaAddress);
	m_stMachineTitle.SetWindowText (strMachineTitle);
	m_stIpAddress.SetWindowText (LOADSTRING (IDS_CONFIG_HW_MODULE_IPADDRESS));
	m_stPort.SetWindowText (LOADSTRING (IDS_CONFIG_HW_MODULE_PORT));
    m_EditIPPort.SubclassEdit(IDC_HW_MODULE_EDIT_PORT, this, PES_NUMBERS);
	m_ctrlConnect.SetWindowText (LOADSTRING (IDS_CONFIG_HW_MODULE_CONNECT));
}

/*
** CreateFont :
*/
void CDlgHWModule::CreateFont ()
{
    m_pFontModule=((CMainFrame *) AfxGetMainWnd ())->CreateFont ("Tahoma", 18, FW_NORMAL);
    m_stMachineTitle.SetFont (m_pFontModule);
}

/*
** InitData :
*/
void CDlgHWModule::InitData ()
{
	CString strName, strDesc, strModule;

	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	CProject *pPrj = pPlant->GetFirst ();
	while (pPrj)   {
		if (pPrj->GetMachine()->GetTypeModule () == enEpsilon || pPrj->GetMachine()->GetTypeModule () == enDRTOS && pPrj->GetMachine()->GetHWModuleNumber () == GetKinemaAddress ())   {
			strName = pPrj->GetMachineModule()->GetName();
			strDesc = pPrj->GetMachineModule()->GetDesc();
			strModule.Format ("%d", pPrj->GetModuleNumber ());
			m_Grid.AssignModule (strName, strDesc, strModule);
			SetIPAddress (pPrj->GetMachine ()->GetTcpIpAddress (), pPrj->GetMachine ()->GetPort ());
			SetModuleType (pPrj->GetMachine ()->GetTypeModule ());
			m_pPrj = pPrj;
			break;
		}
		pPrj = (CProject *) pPlant->GetNext ();
	}
}

/*
** SetIPAddress :
*/
void CDlgHWModule::SetIPAddress (BYTE *achIPAddress, int nPort)
{
	CString strPort;

	m_ctrlIPAddress.SetAddress (achIPAddress[0], achIPAddress[1], achIPAddress[2], achIPAddress[3]);
	strPort.Format ("%d", nPort);
	m_EditIPPort.SetWindowText (strPort);
}

/*
** RemoveCurrentConnection :
*/
void CDlgHWModule::RemoveCurrentConnection ()
{
	m_Grid.RemoveCurrentConnection ();
	m_ctrlIPAddress.ClearAddress ();
	m_EditIPPort.SetWindowText ("");
}

void CDlgHWModule::OnEnChangeHwModuleEditPort()
{
	// TODO:  Se si tratta di un controllo RICHEDIT, il controllo non
	// invierà questa notifica a meno che non si esegua l'override della funzione CDialog::OnInitDialog()
	// e venga eseguita la chiamata a CRichEditCtrl().SetEventMask()
	// con il flag ENM_CHANGE introdotto dall'operatore OR nella maschera.

	// TODO:  Aggiungere qui il codice per la gestione della notifica del controllo.
	if (m_bInitialized)   {
		if (m_pPrj)
			m_pPrj->SetMachineModifiedFlag (true);
	}
}

/*
** SetCurrentProject :
*/
void CDlgHWModule::SetCurrentProject (CProject *pPrj)
{
	m_pPrj = pPrj;
}

void CDlgHWModule::OnBnClickedHwModuleConnect()
{
	CString str;

	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	SaveData ();
	BeginWaitCursor ();
	if (m_pPrj && !m_pPrj->ReconnectMachine (enTCPConnection))   {
		EndWaitCursor ();
		str.Format ("%s %s", LOADSTRING (IDS_CONFIG_HW_MODULE_CANNOT_CONNECT), m_pPrj->GetMachineModule()->GetName());
		AfxMessageBox (str);
	}
	else
		EndWaitCursor ();
}

void CDlgHWModule::OnIpnFieldchangedHwModuleIpaddress(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	*pResult = 0;
	if (m_bIPCtrlToggled)   {
		if (m_pPrj)
			m_pPrj->SetMachineModifiedFlag (true);
	}
	else
		m_bIPCtrlToggled = true;
}

void CDlgHWModule::SetModuleType (eTypeModule TypeModule)
{
	int nChecked = TypeModule == enDRTOS ? IDC_RADIO_DRTOS : IDC_RADIO_EPSILON;
    CheckRadioButton (IDC_RADIO_EPSILON, IDC_RADIO_DRTOS, nChecked);
}

eTypeModule CDlgHWModule::GetModuleType ()
{
	int nChecked = GetCheckedRadioButton (IDC_RADIO_EPSILON, IDC_RADIO_DRTOS);
	return nChecked == IDC_RADIO_EPSILON ? enEpsilon : enDRTOS;
}
