// DlgHWCanOpenNet.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "MainFrame.h"
#include "AdamoTemplates.h"
#include "dlghwcanopennet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CDlgHWCanOpenNet, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CDlgHWCanOpenNet dialog


CDlgHWCanOpenNet::CDlgHWCanOpenNet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHWCanOpenNet::IDD, pParent), m_pConfigDoc (NULL), m_bIsInitialized (false), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CDlgHWCanOpenNet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHWCanOpenNet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHWCanOpenNet)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_ENABLE_CANOPEN_WARNING, m_btnEnableCanOpenWarnings);
}

BEGIN_MESSAGE_MAP(CDlgHWCanOpenNet, CDialog)
	//{{AFX_MSG_MAP(CDlgHWCanOpenNet)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_BAUDRATE1000, OnRadioBaudrate1000)
	ON_BN_CLICKED(IDC_RADIO_BAUDRATE500, OnRadioBaudrate500)
	ON_BN_CLICKED(IDC_RADIO_BAUDRATE250, OnRadioBaudrate250)
	ON_BN_CLICKED(IDC_RADIO_BAUDRATE125, OnRadioBaudrate125)
	ON_EN_CHANGE(IDC_EDIT_SYNCFREQUENCY, OnChangeEditSyncfrequency)
	ON_EN_CHANGE(IDC_EDIT_TIMEOUTSDO, OnChangeEditTimeoutsdo)
	ON_EN_CHANGE(IDC_EDIT_NETGUARDTIME, OnChangeEditNetguardtime)
	ON_EN_CHANGE(IDC_EDIT_NETLIFETIME, OnChangeEditNetlifetime)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ENABLE_CANOPEN_WARNING, &CDlgHWCanOpenNet::OnBnClickedEnableCanopenWarning)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHWCanOpenNet message handlers
void CDlgHWCanOpenNet::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)
            m_pConfigDoc=pDoc;
    }
}

BOOL CDlgHWCanOpenNet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	SetConfigDoc ();
    InitTitles ();
    InitData ();
    InitPsw ();
    m_bIsInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitTitles : inizializza le stringhe
*/
void CDlgHWCanOpenNet::InitTitles () 
{
    GetDlgItem (IDC_ST_BAUDRATE)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_CANOPENBAUDRATE));
    GetDlgItem (IDC_ST_SYNCFREQUENCY)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_CANOPENSYNCFREQUENCY));
    GetDlgItem (IDC_ST_SDOTIMEOUT)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_CANOPENSDOTIMEOUT));
    GetDlgItem (IDC_ST_SYNC)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_CANOPENSTSYNC));
    GetDlgItem (IDC_ST_TIMEOUTSDO)->SetWindowText (LOADSTRING (IDS_CONFIG_HW_CANOPENSTSDOTIMEOUT));
    GetDlgItem (IDC_ST_NETGUARDING)->SetWindowText (LOADSTRING (IDS_NET_GUARDING));
    GetDlgItem (IDC_ST_NETGUARDTIME)->SetWindowText (LOADSTRING (IDS_NET_GUARDTIME));
    GetDlgItem (IDC_ST_NETLIFETIME)->SetWindowText (LOADSTRING (IDS_NET_LIFETIME));
	GetDlgItem (IDC_ST_ENABLEWARNINGS)->SetWindowText (LOADSTRING (IDS_NET_ENABLEWARNINGS));
	GetDlgItem (IDC_ENABLE_CANOPEN_WARNING)->SetWindowText (LOADSTRING (IDS_NET_ENABLE_CANOPEN_WARNINGS));
    m_editSDOTimeot.SubclassEdit(IDC_EDIT_TIMEOUTSDO, this, PES_NUMBERS);
    m_editSync.SubclassEdit(IDC_EDIT_SYNCFREQUENCY, this, PES_NUMBERS);
    m_ctrlEditGuardTime.SubclassEdit(IDC_EDIT_NETGUARDTIME, this, PES_NUMBERS);
    m_ctrlEditLifeTime.SubclassEdit(IDC_EDIT_NETLIFETIME, this, PES_NUMBERS);
}

/*
** InitData : inizializza le stringhe
*/
void CDlgHWCanOpenNet::InitData ()
{
    CString str;
    CAdamoHWConfigFile *pCF=m_pPrj->GetHWConfigFile ();
    int nBaudRate=pCF->GetCanBaudrate (m_nKinemaAddress);
    CheckRadioButton (IDC_RADIO_BAUDRATE125, IDC_RADIO_BAUDRATE1000, nBaudRate + IDC_RADIO_BAUDRATE125);
    int nSDOTimeOut=pCF->GetSDOTimeOut (m_nKinemaAddress);
    str.Format ("%d", nSDOTimeOut);
    m_editSDOTimeot.SetWindowText (str);
    int nSyncFrequency=pCF->GetSyncFrequency (m_nKinemaAddress);
    str.Format ("%d", nSyncFrequency);
    m_editSync.SetWindowText (str);
    int nGuardTime=pCF->GetGuardTime (m_nKinemaAddress);
    str.Format ("%d", nGuardTime);
    m_ctrlEditGuardTime.SetWindowText (str);
    int nLifeTime=pCF->GetLifeTime (m_nKinemaAddress);
    str.Format ("%d", nLifeTime);
    m_ctrlEditLifeTime.SetWindowText (str);
	int nEnableWarnings=pCF->GetEnableWarnings (m_nKinemaAddress);
	m_btnEnableCanOpenWarnings.SetCheck (nEnableWarnings);
}

/*
** Save : inizializza le stringhe
*/
void CDlgHWCanOpenNet::Save ()
{
    SaveData ();
}

/*
** OnDestroy :
*/
void CDlgHWCanOpenNet::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SaveData ();
}

/*
** SaveData :
*/
void CDlgHWCanOpenNet::SaveData ()
{
    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
    CString str;
    int nSyncFrequency, nSDOTimeout, nBaudrate;
    int nGuardTime, nLifeTime, nEanbleWarnings;

    if (pACF)   {
        m_editSync.GetWindowText (str);
        nSyncFrequency=atoi (str);
        m_editSDOTimeot.GetWindowText (str);
        nSDOTimeout=atoi (str);
        m_ctrlEditGuardTime.GetWindowText (str);
        nGuardTime=atoi (str);
        m_ctrlEditLifeTime.GetWindowText (str);
        nLifeTime=atoi (str);
        nBaudrate=GetCheckedRadioButton (IDC_RADIO_BAUDRATE125, IDC_RADIO_BAUDRATE1000)-IDC_RADIO_BAUDRATE125;
		nEanbleWarnings = m_btnEnableCanOpenWarnings.GetCheck ();
        pACF->SetCanBaudrate (m_nKinemaAddress, nBaudrate);
        pACF->SetSDOTimeOut (m_nKinemaAddress, nSDOTimeout);
        pACF->SetSyncFrequency (m_nKinemaAddress, nSyncFrequency);
        pACF->SetGuardTime (m_nKinemaAddress, nGuardTime);
        pACF->SetLifeTime (m_nKinemaAddress, nLifeTime);
		pACF->SetEnableWarnings (m_nKinemaAddress, nEanbleWarnings);
    }
}

void CDlgHWCanOpenNet::OnRadioBaudrate1000() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
}

void CDlgHWCanOpenNet::OnRadioBaudrate500() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
}

void CDlgHWCanOpenNet::OnRadioBaudrate250() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
}

void CDlgHWCanOpenNet::OnRadioBaudrate125() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
}

void CDlgHWCanOpenNet::OnChangeEditSyncfrequency() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    if (m_bIsInitialized)
        m_pPrj->SetHWConfigModified ();	
}

void CDlgHWCanOpenNet::OnChangeEditTimeoutsdo() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    if (m_bIsInitialized)
        m_pPrj->SetHWConfigModified ();	
}

void CDlgHWCanOpenNet::OnChangeEditNetguardtime() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    if (m_bIsInitialized)
        m_pPrj->SetHWConfigModified ();	
}

void CDlgHWCanOpenNet::OnChangeEditNetlifetime() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    if (m_bIsInitialized)
        m_pPrj->SetHWConfigModified ();	
}

/*
** InitPsw :
*/
void CDlgHWCanOpenNet::InitPsw ()
{
    if (m_ePswLevel<=ePswService)   {
        GetDlgItem (IDC_RADIO_BAUDRATE125)->EnableWindow ();
        GetDlgItem (IDC_RADIO_BAUDRATE250)->EnableWindow ();
        GetDlgItem (IDC_RADIO_BAUDRATE500)->EnableWindow ();
        GetDlgItem (IDC_RADIO_BAUDRATE1000)->EnableWindow ();
        GetDlgItem (IDC_EDIT_SYNCFREQUENCY)->EnableWindow ();
        GetDlgItem (IDC_EDIT_NETGUARDTIME)->EnableWindow ();
        GetDlgItem (IDC_EDIT_NETLIFETIME)->EnableWindow ();
    }
    else   {
        GetDlgItem (IDC_RADIO_BAUDRATE125)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_BAUDRATE250)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_BAUDRATE500)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_BAUDRATE1000)->EnableWindow (false);
        GetDlgItem (IDC_EDIT_SYNCFREQUENCY)->EnableWindow (false);
        GetDlgItem (IDC_EDIT_NETGUARDTIME)->EnableWindow (false);
        GetDlgItem (IDC_EDIT_NETLIFETIME)->EnableWindow (false);
    }
    if (m_ePswLevel<=ePswBuilder)
        GetDlgItem (IDC_EDIT_TIMEOUTSDO)->EnableWindow ();
    else
        GetDlgItem (IDC_EDIT_TIMEOUTSDO)->EnableWindow (false);
}

/*
** OnChangePsw :
*/
void CDlgHWCanOpenNet::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}

void CDlgHWCanOpenNet::OnBnClickedEnableCanopenWarning()
{
	// TODO: Add your control notification handler code here
    if (m_bIsInitialized)
        m_pPrj->SetHWConfigModified ();	
}
