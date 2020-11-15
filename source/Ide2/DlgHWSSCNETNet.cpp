// DlgHWSSCNETNet.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoTemplates.h"
#include "dlghwsscnetnet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define J4TOJ3 0
#define J3TOJ4 1

/////////////////////////////////////////////////////////////////////////////
// CDlgHWSSCNETNet dialog
IMPLEMENT_DYNAMIC (CDlgHWSSCNETNet, CDialog)


CDlgHWSSCNETNet::CDlgHWSSCNETNet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHWSSCNETNet::IDD, pParent), m_pConfigDoc (NULL), m_ePswLevel (ePswUser), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CDlgHWSSCNETNet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHWSSCNETNet::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDlgHWSSCNETNet)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_ST_SSCNET_CONF, m_stSSCNETConf);
}


BEGIN_MESSAGE_MAP(CDlgHWSSCNETNet, CDialog)
	//{{AFX_MSG_MAP(CDlgHWSSCNETNet)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_ENABLE_WARNINGS, &CDlgHWSSCNETNet::OnBnClickedCheckEnableWarnings)
	ON_BN_CLICKED(IDC_CHECK_DISABLE_NETWORK, &CDlgHWSSCNETNet::OnBnClickedCheckDisableNetwork)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_SSCNET_TYPE_J3, &CDlgHWSSCNETNet::OnBnClickedSscnetTypeJ3)
    ON_BN_CLICKED(IDC_SSCNET_TYPE_J4, &CDlgHWSSCNETNet::OnBnClickedSscnetTypeJ4)
    ON_BN_CLICKED(IDC_BTN_MONOAX, &CDlgHWSSCNETNet::OnBnClickedBtnMonoax)
    ON_BN_CLICKED(IDC_BTN_BIAX, &CDlgHWSSCNETNet::OnBnClickedBtnBiax)
    ON_BN_CLICKED(IDC_BTN_TRIAX, &CDlgHWSSCNETNet::OnBnClickedBtnTriax)
    ON_BN_CLICKED(IDC_BTN_DELETE, &CDlgHWSSCNETNet::OnBnClickedBtnDelete)
    ON_BN_CLICKED(IDC_BTN_J3_J4, &CDlgHWSSCNETNet::OnBnClickedBtnJ3J4)
    ON_BN_CLICKED(IDC_BTN_J4_J3, &CDlgHWSSCNETNet::OnBnClickedBtnJ4J3)
    ON_BN_CLICKED(IDC_BTN_RESTORE, &CDlgHWSSCNETNet::OnBnClickedBtnRestore)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHWSSCNETNet message handlers
void CDlgHWSSCNETNet::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)
            m_pConfigDoc=pDoc;
    }
}

BOOL CDlgHWSSCNETNet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	SetConfigDoc ();
    InitTitles ();
    InitData ();
    InitPsw ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitTitles :
*/
void CDlgHWSSCNETNet::InitTitles ()
{
    GetDlgItem (IDC_ST_FREQLAV)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_FREQLAV));
    GetDlgItem (IDC_ST_SSCNET_TYPE)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_SSCNET_TYPE));
    GetDlgItem (IDC_CHECK_ENABLE_WARNINGS)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_ENABLE_WARNINGS));
    GetDlgItem (IDC_CHECK_DISABLE_NETWORK)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_DISABLENETWORK));
    GetDlgItem (IDC_RADIO_220MS)->EnableWindow (FALSE);
    GetDlgItem (IDC_RADIO_440MS)->EnableWindow (FALSE);
    GetDlgItem (IDC_RADIO_880MS)->EnableWindow (FALSE);
}

/*
** InitData :
*/
void CDlgHWSSCNETNet::InitData ()
{
    int nNetworkType;

    CAdamoHWConfigFile *pCF=m_pPrj->GetHWConfigFile ();
    ((CButton *)GetDlgItem (IDC_CHECK_ENABLE_WARNINGS))->SetCheck (pCF->GetSSCNETWarnings (m_nKinemaAddress));
    ((CButton *)GetDlgItem (IDC_CHECK_DISABLE_NETWORK))->SetCheck (pCF->IsSSCNETNetworkEnabled (m_nKinemaAddress));
    nNetworkType = (int) pCF->GetSSCNETNetworkType (m_nKinemaAddress);
    CheckRadioButton (IDC_RADIO_220MS, IDC_RADIO_880MS, IDC_RADIO_880MS);
    CheckRadioButton (IDC_SSCNET_TYPE_J3, IDC_SSCNET_TYPE_J4, IDC_SSCNET_TYPE_J3 + nNetworkType);
    GetDlgItem (IDC_ST_SSCNET_CONF)->MoveWindow (12, 120, 644, 132);
    GetDlgItem (IDC_BTN_MONOAX)->MoveWindow (12, 256, 104, 32);
    GetDlgItem (IDC_BTN_BIAX)->MoveWindow (120, 256, 104, 32);
    GetDlgItem (IDC_BTN_TRIAX)->MoveWindow (228, 256, 104, 32);
    GetDlgItem (IDC_BTN_DELETE)->MoveWindow (552, 256, 104, 32);
	m_stSSCNETConf.SetKinemaAddress (m_nKinemaAddress);
    m_stSSCNETConf.Init (pCF);
}

/*
** SaveData :
*/
void CDlgHWSSCNETNet::SaveData ()
{
    CAdamoHWConfigFile *pCF=m_pPrj->GetHWConfigFile ();
    int nNetworkType;

    if (pCF)   {
        int nChecked=((CButton *)GetDlgItem (IDC_CHECK_ENABLE_WARNINGS))->GetCheck ();
        pCF->EnableSSCNETWarnings (m_nKinemaAddress, nChecked!=0);
		nChecked=((CButton *)GetDlgItem (IDC_CHECK_DISABLE_NETWORK))->GetCheck ();
		pCF->EnableSSCNETNetwork (m_nKinemaAddress, nChecked!=0);
        nNetworkType = GetCheckedRadioButton (IDC_SSCNET_TYPE_J3, IDC_SSCNET_TYPE_J4) - IDC_SSCNET_TYPE_J3;
        pCF->SetSSCNETNetworkType (m_nKinemaAddress, nNetworkType != 0);
        m_stSSCNETConf.SaveData ();
    }    
}

void CDlgHWSSCNETNet::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SaveData ();
}

/*
** Save :
*/
void CDlgHWSSCNETNet::Save ()
{
    SaveData ();    
}

void CDlgHWSSCNETNet::OnBnClickedCheckEnableWarnings() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
}

/*
** OnChangePsw :
*/
void CDlgHWSSCNETNet::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}

/*
** InitPsw :
*/
void CDlgHWSSCNETNet::InitPsw ()
{
    GetDlgItem (IDC_RADIO_220MS)->EnableWindow (false);
    GetDlgItem (IDC_RADIO_440MS)->EnableWindow (false);
    GetDlgItem (IDC_RADIO_880MS)->EnableWindow (false);
    if (m_ePswLevel<=ePswBuilder)   {
        GetDlgItem (IDC_CHECK_ENABLE_WARNINGS)->EnableWindow ();
        GetDlgItem (IDC_CHECK_DISABLE_NETWORK)->EnableWindow ();
        GetDlgItem (IDC_SSCNET_TYPE_J3)->EnableWindow ();
        GetDlgItem (IDC_SSCNET_TYPE_J4)->EnableWindow ();
        GetDlgItem (IDC_BTN_J3_J4)->EnableWindow ();
        GetDlgItem (IDC_BTN_J4_J3)->EnableWindow ();
        GetDlgItem (IDC_BTN_RESTORE)->EnableWindow ();
    }
    else   {
        GetDlgItem (IDC_CHECK_ENABLE_WARNINGS)->EnableWindow (false);
        GetDlgItem (IDC_CHECK_DISABLE_NETWORK)->EnableWindow (false);
        GetDlgItem (IDC_SSCNET_TYPE_J3)->EnableWindow (false);
        GetDlgItem (IDC_SSCNET_TYPE_J4)->EnableWindow (false);
        GetDlgItem (IDC_BTN_J3_J4)->EnableWindow (false);
        GetDlgItem (IDC_BTN_J4_J3)->EnableWindow (false);
        GetDlgItem (IDC_BTN_RESTORE)->EnableWindow (false);
    }
}

void CDlgHWSSCNETNet::OnBnClickedCheckDisableNetwork()
{
	// TODO: Add your control notification handler code here
	   m_pPrj->SetHWConfigModified ();	
}

void CDlgHWSSCNETNet::OnBnClickedSscnetTypeJ3()
{
    // TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    m_pPrj->SetHWConfigModified ();	
}

void CDlgHWSSCNETNet::OnBnClickedSscnetTypeJ4()
{
    // TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    m_pPrj->SetHWConfigModified ();	
}

void CDlgHWSSCNETNet::OnBnClickedBtnMonoax()
{
    // TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    if (!m_stSSCNETConf.AddSingleStation ())
        m_pPrj->SetHWConfigModified ();	
}

void CDlgHWSSCNETNet::OnBnClickedBtnBiax()
{
    // TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    if (!m_stSSCNETConf.AddDoubleStation ())
        m_pPrj->SetHWConfigModified ();	
}

void CDlgHWSSCNETNet::OnBnClickedBtnTriax()
{
    // TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    if (!m_stSSCNETConf.AddTripleStation ())
        m_pPrj->SetHWConfigModified ();	
}

void CDlgHWSSCNETNet::OnBnClickedBtnDelete()
{
    // TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    if (!m_stSSCNETConf.DeleteStation ())
        m_pPrj->SetHWConfigModified ();	
}

void CDlgHWSSCNETNet::OnBnClickedBtnJ3J4()
{
    // TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    CAdamoMachine *pMachine = GETWS ()->GetActiveProject ()->GetMachine ();
    pMachine->ConvertSSCNETAbsPos (m_nKinemaAddress, J3TOJ4);
}

void CDlgHWSSCNETNet::OnBnClickedBtnJ4J3()
{
    // TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    CAdamoMachine *pMachine = GETWS ()->GetActiveProject ()->GetMachine ();
    pMachine->ConvertSSCNETAbsPos (m_nKinemaAddress, J4TOJ3);
}

void CDlgHWSSCNETNet::OnBnClickedBtnRestore()
{
    // TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    CAdamoMachine *pMachine = GETWS ()->GetActiveProject ()->GetMachine ();
    pMachine->ConvertSSCNETAbsPos (m_nKinemaAddress, -1);
}