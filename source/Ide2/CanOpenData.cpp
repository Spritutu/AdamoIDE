// CanOpenData.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "canopendata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanOpenData dialog


CCanOpenData::CCanOpenData(CWnd* pParent /*=NULL*/)
	: CDialog(CCanOpenData::IDD, pParent), m_nSelected (0), m_bIsInitialized (false), m_ePswLevel (ePswUser), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CCanOpenData)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCanOpenData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCanOpenData)
	DDX_Control(pDX, IDC_CHECK_POLLINGRECEIVER, m_ctrlPollingReceiver);
	DDX_Control(pDX, IDC_CHECK_DISABLEDRECEIVER, m_ctrlDisabledReceiver);
	DDX_Control(pDX, IDC_CHECK_VIRTUALRECEIVER, m_ctrlVirtualReceiver);
	DDX_Control(pDX, IDC_CHECK_GUARDING, m_ctrlEnableGuarding);
	DDX_Control(pDX, IDC_CHECK_TPDO, m_ctrlEnableTPDO);
	DDX_Control(pDX, IDC_COMBO_TPDO, m_ctrlTPDO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCanOpenData, CDialog)
	//{{AFX_MSG_MAP(CCanOpenData)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_TPDO, OnSelChangeComboTpdo)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_TPDO_AA, OnRadioTpdoAA)
	ON_BN_CLICKED(IDC_RADIO_TPDO_CS, OnRadioTpdoCS)
	ON_BN_CLICKED(IDC_RADIO_TPDO_CUS, OnRadioTpdoCUS)
	ON_BN_CLICKED(IDC_RADIO_TPDO_DEV, OnRadioTpdoDEV)
	ON_BN_CLICKED(IDC_RADIO_TPDO_RTRA, OnRadioTpdoRTRA)
	ON_BN_CLICKED(IDC_RADIO_TPDO_RTRS, OnRadioTpdoRTRS)
	ON_EN_CHANGE(IDC_EDIT_TPDO, OnChangeEditTpdo)
	ON_BN_CLICKED(IDC_CHECK_TPDO, OnCheckTpdo)
	ON_BN_CLICKED(IDC_CHECK_GUARDING, OnCheckGuarding)
	ON_BN_CLICKED(IDC_RADIO_GUARDING, OnRadioGuarding)
	ON_BN_CLICKED(IDC_RADIO_HEARTBEAT, OnRadioHeartbeat)
	ON_EN_CHANGE(IDC_EDIT_LIFETIME, OnChangeEditLifeTime)
	ON_BN_CLICKED(IDC_CHECK_VIRTUALRECEIVER, OnCheckVirtualReceiver)
	ON_BN_CLICKED(IDC_CHECK_DISABLEDRECEIVER, OnCheckDisabledReceiver)
	ON_BN_CLICKED(IDC_CHECK_POLLINGRECEIVER, OnCheckPollingReceiver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanOpenData message handlers

void CCanOpenData::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

BOOL CCanOpenData::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    SetConfigDoc ();
	InitTitles ();
    LoadData ();
    InitPsw ();
    m_bIsInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitTitles :
*/
void CCanOpenData::InitTitles ()
{
    CString str;
    GetDlgItem (IDC_CHECK_POLLINGRECEIVER)->SetWindowText (LOADSTRING (IDS_POLLINGRECEIVER_ENABLE));
    GetDlgItem (IDC_CHECK_VIRTUALRECEIVER)->SetWindowText (LOADSTRING (IDS_VIRTUALRECEIVER_ENABLE));
    GetDlgItem (IDC_CHECK_DISABLEDRECEIVER)->SetWindowText (LOADSTRING (IDS_DISABLEDRECEIVER_ENABLE));
    GetDlgItem (IDC_CHECK_TPDO)->SetWindowText (LOADSTRING (IDS_TPDO_ENABLE));
    GetDlgItem (IDC_RADIO_TPDO_AA)->SetWindowText (LOADSTRING (IDS_TPDO_AA));
    GetDlgItem (IDC_RADIO_TPDO_CS)->SetWindowText (LOADSTRING (IDS_TPDO_CS));
    GetDlgItem (IDC_RADIO_TPDO_RTRS)->SetWindowText (LOADSTRING (IDS_TPDO_RTRS));
    GetDlgItem (IDC_RADIO_TPDO_RTRA)->SetWindowText (LOADSTRING (IDS_TPDO_RTRA));
    GetDlgItem (IDC_RADIO_TPDO_CUS)->SetWindowText (LOADSTRING (IDS_TPDO_CUS));
    GetDlgItem (IDC_RADIO_TPDO_DEV)->SetWindowText (LOADSTRING (IDS_TPDO_DEV));
    GetDlgItem (IDC_ST_GUARDING)->SetWindowText (LOADSTRING (IDS_GUARDING));
    GetDlgItem (IDC_CHECK_GUARDING)->SetWindowText (LOADSTRING (IDS_GUARDING_ENABLE));
    GetDlgItem (IDC_RADIO_GUARDING)->SetWindowText (LOADSTRING (IDS_GUARDING_NODEGUARDING));
    GetDlgItem (IDC_RADIO_HEARTBEAT)->SetWindowText (LOADSTRING (IDS_GUARDING_HEARTBEAT));
    GetDlgItem (IDC_ST_LIFETIME)->SetWindowText (LOADSTRING (IDS_GUARDING_LIFETIME));
    for (int i=0; i<4; i++)   {
        str.Format ("%s %d", (const char *)LOADSTRING (IDS_TPDO_NUMBER), i);
        m_ctrlTPDO.AddString (str);        
    }
    m_ctrlTPDOSync.SubclassEdit(IDC_EDIT_TPDO, this, PES_NUMBERS);
    m_ctrlLifeTime.SubclassEdit(IDC_EDIT_LIFETIME, this, PES_NUMBERS);
    m_ctrlTPDO.SetCurSel (0);
}

/*
** LoadData :
*/
void CCanOpenData::LoadData ()
{
    CString str;
    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
    bool bVirtualReceiver=pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).bIsVirtual;
    bool bDisabledReceiver=pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).bIsDisabled;
    bool bPollingReceiver=pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).bIsPolling;
    ((CButton *)GetDlgItem (IDC_CHECK_VIRTUALRECEIVER))->SetCheck (bVirtualReceiver);
    ((CButton *)GetDlgItem (IDC_CHECK_DISABLEDRECEIVER))->SetCheck (bDisabledReceiver);
    ((CButton *)GetDlgItem (IDC_CHECK_POLLINGRECEIVER))->SetCheck (bPollingReceiver);
    WORD wTPDO=pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).wTPDOType[m_nSelected];
    BYTE bTPDO=(BYTE)(wTPDO&0x00ff);
    if (wTPDO&0x8000)   {
        /* e' abilitato */
        EnableTPDOControls ();
	    m_ctrlTPDOSync.EnableWindow (FALSE);
        switch (bTPDO)   {
            case 0 :
                CheckRadioButton (IDC_RADIO_TPDO_AA, IDC_RADIO_TPDO_DEV, IDC_RADIO_TPDO_AA);
                break;
            case 252 :
                CheckRadioButton (IDC_RADIO_TPDO_AA, IDC_RADIO_TPDO_DEV, IDC_RADIO_TPDO_RTRS);
                break;
            case 253 :
                CheckRadioButton (IDC_RADIO_TPDO_AA, IDC_RADIO_TPDO_DEV, IDC_RADIO_TPDO_RTRA);
                break;
            case 254 :
                CheckRadioButton (IDC_RADIO_TPDO_AA, IDC_RADIO_TPDO_DEV, IDC_RADIO_TPDO_CUS);
                break;
            case 255 :
                CheckRadioButton (IDC_RADIO_TPDO_AA, IDC_RADIO_TPDO_DEV, IDC_RADIO_TPDO_DEV);
                break;
            default :
                if (bTPDO>0 && bTPDO<=240)   {
                    CheckRadioButton (IDC_RADIO_TPDO_AA, IDC_RADIO_TPDO_DEV, IDC_RADIO_TPDO_CS);
                    str.Format ("%d", (int)bTPDO);
                    m_ctrlTPDOSync.SetWindowText (str);
                    m_ctrlTPDOSync.EnableWindow (TRUE);
                }
        }
    }
    else
        EnableTPDOControls (false);
    DWORD dwGuarding=pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).dwNodeGuarding;
    DWORD dwHeartBeat=pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).dwHeartBeatTime;
    if (dwGuarding&0x80000000)   {
        EnableGuardingControls ();
        m_ctrlLifeTime.EnableWindow (FALSE);
        CheckRadioButton (IDC_RADIO_GUARDING, IDC_RADIO_HEARTBEAT, IDC_RADIO_GUARDING);
    }
    else   {
        if (dwHeartBeat&0x80000000)   {
            EnableGuardingControls ();
            m_ctrlLifeTime.EnableWindow ();
            CheckRadioButton (IDC_RADIO_GUARDING, IDC_RADIO_HEARTBEAT, IDC_RADIO_HEARTBEAT);
            str.Format ("%d", dwHeartBeat&0x0000ffff);
            m_ctrlLifeTime.SetWindowText (str);
        }
        else
            EnableGuardingControls (FALSE);
    }
}


/*
** SetConfigDoc :
*/
void CCanOpenData::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)
            m_pConfigDoc=pDoc;
    }
}

/*
** EnableTPDOControls :
*/
void CCanOpenData::EnableTPDOControls (bool bEnable)
{
    GetDlgItem (IDC_RADIO_TPDO_AA)->EnableWindow (bEnable ? TRUE : FALSE);
    GetDlgItem (IDC_RADIO_TPDO_CS)->EnableWindow (bEnable ? TRUE : FALSE);
    GetDlgItem (IDC_RADIO_TPDO_RTRS)->EnableWindow (bEnable ? TRUE : FALSE);
    GetDlgItem (IDC_RADIO_TPDO_RTRA)->EnableWindow (bEnable ? TRUE : FALSE);
    GetDlgItem (IDC_RADIO_TPDO_CUS)->EnableWindow (bEnable ? TRUE : FALSE);
    GetDlgItem (IDC_RADIO_TPDO_DEV)->EnableWindow (bEnable ? TRUE : FALSE);
	m_ctrlEnableTPDO.EnableWindow ();
    m_ctrlEnableTPDO.SetCheck (bEnable ? TRUE : FALSE);
	m_ctrlTPDOSync.EnableWindow (bEnable ? TRUE : FALSE);
}

/*
** EnableGuardingControls :
*/
void CCanOpenData::EnableGuardingControls (bool bEnable)
{
    GetDlgItem (IDC_RADIO_HEARTBEAT)->EnableWindow (bEnable ? TRUE : FALSE);
    GetDlgItem (IDC_RADIO_GUARDING)->EnableWindow (bEnable ? TRUE : FALSE);
	m_ctrlEnableGuarding.EnableWindow ();
    m_ctrlEnableGuarding.SetCheck (bEnable ? TRUE : FALSE);
    m_ctrlLifeTime.EnableWindow (bEnable ? TRUE : FALSE);
}

/*
** OnSelChangeComboTpdo :
*/
void CCanOpenData::OnSelChangeComboTpdo() 
{
	// TODO: Add your control notification handler code here
    int nSelected=m_ctrlTPDO.GetCurSel ();
    StoreData ();
    m_ctrlTPDOSync.SetWindowText ("");
    m_nSelected=nSelected;
    LoadData ();
}

/*
** SaveData :
*/
void CCanOpenData::Save () 
{
    StoreData ();
}

/*
** StoreData :
*/
void CCanOpenData::StoreData () 
{
    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
    CString str;
    DWORD dwGuarding=0;
    DWORD dwHeartBeat=0;
    WORD wTPDO;

    if (pACF)   {
        int nRadio, nCheck=((CButton *)GetDlgItem (IDC_CHECK_VIRTUALRECEIVER))->GetCheck ();
        int nDisabled=((CButton *)GetDlgItem (IDC_CHECK_DISABLEDRECEIVER))->GetCheck ();
        int nPolling=((CButton *)GetDlgItem (IDC_CHECK_POLLINGRECEIVER))->GetCheck ();
        pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).bIsVirtual=nCheck!=0;
        pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).bIsDisabled=nDisabled!=0;
        pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).bIsPolling=nPolling!=0;
        nCheck=((CButton *)GetDlgItem (IDC_CHECK_TPDO))->GetCheck ();
        if (nCheck)   {
            nRadio=GetCheckedRadioButton (IDC_RADIO_TPDO_AA, IDC_RADIO_TPDO_DEV);
            switch (nRadio)   {
                case IDC_RADIO_TPDO_AA :
                    wTPDO=0;
                    break;
                case IDC_RADIO_TPDO_CS :
                    m_ctrlTPDOSync.GetWindowText (str);
                    str.TrimLeft (); str.TrimRight ();
                    wTPDO=(WORD)atoi (str);
                    if (wTPDO<1)
                        wTPDO=1;
                    if (wTPDO>240)
                        wTPDO=240;
                    break;
                case IDC_RADIO_TPDO_RTRS :
                    wTPDO=252;
                    break;
                case IDC_RADIO_TPDO_RTRA :
                    wTPDO=253;
                    break;
                case IDC_RADIO_TPDO_CUS :
                    wTPDO=254;
                    break;
                case IDC_RADIO_TPDO_DEV :
                    wTPDO=255;
                    break;
            }
            wTPDO|=0x8000;
        }
        else
            wTPDO=0x0000;
        pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).wTPDOType[m_nSelected]=wTPDO;
        nCheck=((CButton *)GetDlgItem (IDC_CHECK_GUARDING))->GetCheck ();
        if (nCheck)   {
            nRadio=GetCheckedRadioButton (IDC_RADIO_GUARDING, IDC_RADIO_HEARTBEAT);
            switch (nRadio)   {
            case IDC_RADIO_GUARDING :
                dwGuarding|=0x80000000;
                break;
            case IDC_RADIO_HEARTBEAT :
                m_ctrlLifeTime.GetWindowText (str);
                str.TrimLeft (); str.TrimRight ();
                dwHeartBeat=(DWORD)atoi (str);
                if (dwHeartBeat>USHRT_MAX)
                    dwHeartBeat=USHRT_MAX;
                dwHeartBeat|=0x80000000;
                break;
            }
            pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).dwNodeGuarding=dwGuarding;
            pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).dwHeartBeatTime=dwHeartBeat;
        }
        else   {
            dwGuarding=0;
            dwHeartBeat=0;
        }
        pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).dwNodeGuarding=dwGuarding;
        pACF->GetCanNode (m_nKinemaAddress, m_nNode-1).dwHeartBeatTime=dwHeartBeat;
    }
}

void CCanOpenData::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    Save ();	
}

void CCanOpenData::OnRadioTpdoAA() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();
    m_ctrlTPDOSync.EnableWindow (FALSE);
    m_ctrlTPDOSync.SetWindowText ("");
}

void CCanOpenData::OnRadioTpdoCS() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
    m_ctrlTPDOSync.EnableWindow ();
}

void CCanOpenData::OnRadioTpdoCUS() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
    m_ctrlTPDOSync.EnableWindow (FALSE);
    m_ctrlTPDOSync.SetWindowText ("");
}

void CCanOpenData::OnRadioTpdoDEV() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
    m_ctrlTPDOSync.EnableWindow (FALSE);
    m_ctrlTPDOSync.SetWindowText ("");
}

void CCanOpenData::OnRadioTpdoRTRA() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
    m_ctrlTPDOSync.EnableWindow (FALSE);
    m_ctrlTPDOSync.SetWindowText ("");
}

void CCanOpenData::OnRadioTpdoRTRS() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();	
    m_ctrlTPDOSync.EnableWindow (FALSE);
    m_ctrlTPDOSync.SetWindowText ("");
}

void CCanOpenData::OnChangeEditTpdo() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    if (m_bIsInitialized)
        m_pPrj->SetHWConfigModified ();	
}

void CCanOpenData::OnCheckTpdo() 
{
	// TODO: Add your control notification handler code here
    EnableTPDOControls (m_ctrlEnableTPDO.GetCheck ()!=0);
}

void CCanOpenData::OnCheckGuarding() 
{
	// TODO: Add your control notification handler code here
    EnableGuardingControls (m_ctrlEnableGuarding.GetCheck ()!=0);
}

void CCanOpenData::OnRadioGuarding() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();
    m_ctrlLifeTime.EnableWindow (FALSE);
    m_ctrlLifeTime.SetWindowText ("");
}

void CCanOpenData::OnRadioHeartbeat() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();
    m_ctrlLifeTime.EnableWindow ();
}

void CCanOpenData::OnChangeEditLifeTime() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
    if (m_bIsInitialized)
        m_pPrj->SetHWConfigModified ();	
}

void CCanOpenData::OnCheckVirtualReceiver() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();
}

void CCanOpenData::OnCheckDisabledReceiver() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();
}

void CCanOpenData::OnCheckPollingReceiver() 
{
	// TODO: Add your control notification handler code here
    m_pPrj->SetHWConfigModified ();
}

/*
** InitPsw :
*/
void CCanOpenData::InitPsw ()
{
    if (m_ePswLevel<=ePswBuilder)   {
        GetDlgItem (IDC_CHECK_DISABLEDRECEIVER)->EnableWindow ();
        GetDlgItem (IDC_CHECK_VIRTUALRECEIVER)->EnableWindow ();
        GetDlgItem (IDC_CHECK_POLLINGRECEIVER)->EnableWindow ();
        GetDlgItem (IDC_CHECK_TPDO)->EnableWindow ();
        GetDlgItem (IDC_RADIO_TPDO_AA)->EnableWindow ();
        GetDlgItem (IDC_RADIO_TPDO_CS)->EnableWindow ();
        GetDlgItem (IDC_EDIT_TPDO)->EnableWindow ();
        GetDlgItem (IDC_RADIO_TPDO_RTRS)->EnableWindow ();
        GetDlgItem (IDC_RADIO_TPDO_RTRA)->EnableWindow ();
        GetDlgItem (IDC_RADIO_TPDO_CUS)->EnableWindow ();
        GetDlgItem (IDC_RADIO_TPDO_DEV)->EnableWindow ();
        GetDlgItem (IDC_CHECK_GUARDING)->EnableWindow ();
        GetDlgItem (IDC_RADIO_HEARTBEAT)->EnableWindow ();
        GetDlgItem (IDC_RADIO_GUARDING)->EnableWindow ();
        GetDlgItem (IDC_EDIT_LIFETIME)->EnableWindow ();
        GetDlgItem (IDC_COMBO_TPDO)->EnableWindow ();
    }
    else   {
        GetDlgItem (IDC_CHECK_DISABLEDRECEIVER)->EnableWindow (false);
        GetDlgItem (IDC_CHECK_VIRTUALRECEIVER)->EnableWindow (false);
        GetDlgItem (IDC_CHECK_POLLINGRECEIVER)->EnableWindow (false);
        GetDlgItem (IDC_CHECK_TPDO)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_TPDO_AA)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_TPDO_CS)->EnableWindow (false);
        GetDlgItem (IDC_EDIT_TPDO)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_TPDO_RTRS)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_TPDO_RTRA)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_TPDO_CUS)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_TPDO_DEV)->EnableWindow (false);
        GetDlgItem (IDC_CHECK_GUARDING)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_HEARTBEAT)->EnableWindow (false);
        GetDlgItem (IDC_RADIO_GUARDING)->EnableWindow (false);
        GetDlgItem (IDC_EDIT_LIFETIME)->EnableWindow (false);
        GetDlgItem (IDC_COMBO_TPDO)->EnableWindow (false);
    }
}

/*
** OnChangePsw :
*/
void CCanOpenData::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}
