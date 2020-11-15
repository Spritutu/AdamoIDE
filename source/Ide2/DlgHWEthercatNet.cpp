// DlgHWEthercatNet.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DlgHWEthercatNet.h"
#include "DlgEcConfigSlaves.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHWEthercatNet dialog

IMPLEMENT_DYNAMIC(CDlgHWEthercatNet, CDialog)

CDlgHWEthercatNet::CDlgHWEthercatNet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHWEthercatNet::IDD, pParent), m_bInitialized (false), m_pHWView (NULL), m_ePswLevel (ePswUser), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CDlgHWEthercatNet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHWEthercatNet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHWEthercatNet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHWEthercatNet, CDialog)
	//{{AFX_MSG_MAP(CDlgHWEthercatNet)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_EC_INIT, OnEcInit)
	ON_BN_CLICKED(IDC_EC_PREOPERATIONAL, OnEcPreoperational)
	ON_BN_CLICKED(IDC_EC_SAFEOPERATIONAL, OnEcSafeoperational)
	ON_BN_CLICKED(IDC_EC_OPERATIONAL, OnEcOperational)
	ON_EN_CHANGE(IDC_EDIT_DC_START, OnChangeDCStart)
	ON_EN_CHANGE(IDC_EDIT_FREQUENCY, OnChangeFrequency)
	ON_EN_CHANGE(IDC_EDIT_DC_OFFSET, OnChangeDCOffset)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_BUTTONPRESSED, ID_DEVICE_GROUP_ID, OnGridButtonPressed)
	ON_NOTIFY(GVN_SELCHANGING, ID_DEVICE_GROUP_ID, OnGridStartSelChange)
	ON_BN_CLICKED(IDC_CHECK_CONTINOUS_MEASURING, &CDlgHWEthercatNet::OnBnClickedCheckContinousMeasuring)
	ON_CBN_SELCHANGE(IDC_COMBO_DRIVER_HPE, &CDlgHWEthercatNet::OnCbnSelchangeComboDriverHpe)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHWEthercatNet message handlers

BOOL CDlgHWEthercatNet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	SetConfigDoc ();
	InitializeGrid ();
	InitializeTitles ();
	InitializeCaption ();
	InitializeControls ();
	InitPsw ();
	LoadDevices ();
    m_bInitialized=true;
	return TRUE;
}

/*
** SetConfigDoc :
*/
void CDlgHWEthercatNet::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)
            m_pConfigDoc=pDoc;
    }
}

/*
** InitializeGrid :
*/
void CDlgHWEthercatNet::InitializeGrid ()
{
	CString str;
	RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_grid.Create (rc, this, ID_DEVICE_GROUP_ID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_grid.GetDefaultCell(FALSE, FALSE)->SetFormat(DT_VCENTER|DT_SINGLELINE);
    m_grid.SetEditable(false);
    m_grid.SetVirtualMode(false);
    m_grid.SetFixedRowCount();
    m_grid.SetFixedColumnCount();
    m_grid.SetRowCount (MAX_ETHERCAT_SLAVE+1);
    m_grid.SetColumnCount(2+1);
    m_grid.SetListMode ();
    for (int i=0; i<MAX_ETHERCAT_SLAVE; i++)   {
        m_grid.GetCell (i+1, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        str.Format("%d", i+1);
        m_grid.SetItemText(i+1, 0, str);
    }
	CDockConfigHW* pDockHW=((CMainFrame *) AfxGetMainWnd ())->GetDockConfigHW ();
	if (pDockHW)
		m_pHWView=pDockHW->GetView ();
}

/*
** OnSize :
*/
void CDlgHWEthercatNet::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
		m_grid.MoveWindow (10, 50, cx-20, 21*19+5);		
		m_grid.SetColumnWidth (1, 300);
		m_grid.ExpandLastColumn ();
	}
}

/*
** OnDestroy :
*/
void CDlgHWEthercatNet::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SaveData ();
	m_grid.DestroyWindow ();	
}

/*
** InitializeCaption :
*/
void CDlgHWEthercatNet::InitializeCaption() 
{
    GV_ITEM Item;
    CString str;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_BMPETHDEVICEGROUP), 16, 1, RGB(0,128,128));
	m_grid.SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 1;
    Item.iImage = 0;
    Item.strText=LOADSTRING (IDS_ETHERCAT_DEVICE);
    m_grid.SetItem(&Item);
	Item.row = 0;
	Item.col = 2;
    Item.iImage = 1;
    Item.strText=LOADSTRING (IDS_ETHERCAT_GROUP);
    m_grid.SetItem(&Item);
}

/*
** LoadDevices :
*/
void CDlgHWEthercatNet::LoadDevices ()
{
    CProject* pPrj=GETWS()->GetActiveProject();
	if (pPrj)   {
		CPtrArray &pSlaves=pPrj->GetSlaveArray ();
		for (int i=0; i<pSlaves.GetSize (); i++)   {
			m_grid.GetCell(i+1, 1)->SetText(((stAdamoEthSlave *)pSlaves[i])->szName);
			m_grid.SetCellType(i+1, 2, RUNTIME_CLASS(CGridCellToggleButton));
		}
	}
}

/*
** Save :
*/
void CDlgHWEthercatNet::Save ()
{
	SaveData ();
}

/*
** SaveData :
*/
void CDlgHWEthercatNet::SaveData ()
{
    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
	CString str, strHpeDriver;
	int nID=GetCheckedRadioButton (IDC_EC_INIT, IDC_EC_OPERATIONAL), nRequestedState;
	__int64 nDCStartTime;
	int nDCFrequency, nDCOffsetTime;

	if (pACF)   {
		switch (nID)   {
		case IDC_EC_INIT :
			nRequestedState=DEVICE_STATE_INIT;
			break;
		case IDC_EC_PREOPERATIONAL :
			nRequestedState=DEVICE_STATE_PREOP;
			break;
		case IDC_EC_SAFEOPERATIONAL :
			nRequestedState=DEVICE_STATE_SAFEOP;
			break;
		case IDC_EC_OPERATIONAL :
			nRequestedState=DEVICE_STATE_OP;
			break;
		}
		CAdamoTemplate *pTmpl=((CIdeApp *)AfxGetApp ())->GetTemplate (enConfTemplate);
		if (pTmpl)   {
			CProject *pPrj=GETWS ()->GetActiveProject ();
			if (pPrj)   {
				CPtrArray &slaveArray=pPrj->GetSlaveArray ();
				for (int i=0; i<slaveArray.GetSize (); i++)
					((stAdamoEthSlave *)slaveArray[i])->nRequestedState=nRequestedState;
			}
		}
		m_ctrlEditDCStartTime.GetWindowText (str);
		nDCStartTime=_atoi64 (str);
		m_ctrlEditFrequency.GetWindowText (str);
		nDCFrequency=atoi (str);
		m_ctrlEditDCOffsetTime.GetWindowText (str);
		nDCOffsetTime=atoi (str);
		pACF->SetEthercatDCStartTime (nDCStartTime);
		pACF->SetEthercatFrequency (nDCFrequency);
		pACF->SetEthercatContinousMeasuring (((CButton *)GetDlgItem (IDC_CHECK_CONTINOUS_MEASURING))->GetCheck ());
		pACF->SetEthercatDCOffsetTime (nDCOffsetTime);
		((CComboBox*)GetDlgItem (IDC_COMBO_DRIVER_HPE))->GetWindowText (strHpeDriver);
		pACF->SetHpeDriver (strHpeDriver);
		m_pHWView->SaveHWConfiguration (m_pPrj);
	}
}

/*
** SetHWModified :
*/
void CDlgHWEthercatNet::SetHWModified ()
{
	m_pPrj->SetHWConfigModified ();
}

/*
** RemoveCurrentConnection :
*/
void CDlgHWEthercatNet::RemoveCurrentConnection ()
{
	int nState;
    for (int i=0; i<MAX_ETHERCAT_SLAVE; i++)   {
        nState=m_grid.GetItemState (i+1, 2);
        if (nState & GVNI_SELECTED)   {
			ClearSlaveInfo (i+1);
			break;
		}
	}
	m_grid.Invalidate ();
	SetHWModified ();
}

/*
** OnGridButtonPressed :
*/
void CDlgHWEthercatNet::OnGridButtonPressed(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    COleVariant v;
    GV_CHECKNOTIFY* pItem = (GV_CHECKNOTIFY*) pNotifyStruct;
    CProject* pPrj=GETWS()->GetActiveProject();
	if (pPrj)   {
		CPtrArray &pSlaves=pPrj->GetSlaveArray ();
		CDlgEcConfigSlaves dlgEcConfigSlaves;
		dlgEcConfigSlaves.SetProject (m_pPrj);
		dlgEcConfigSlaves.SetEcSlave ((stAdamoEthSlave *)pSlaves[m_nCurRow-1]);
		dlgEcConfigSlaves.DoModal ();
	}
	((CGridCellToggleButton*)m_grid.GetCell (m_nCurRow, 2))->ResetToggle ();
}

/*
** OnGridStartSelChange :
*/
void CDlgHWEthercatNet::OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nCurRow=pItem->iRow;
}

/*
** ClearSlaveInfo :
*/
void CDlgHWEthercatNet::ClearSlaveInfo (int nIndex)
{
	CProject *pPrj=((CMainFrame *)AfxGetMainWnd ())->GetProject ();
	if (pPrj)   {
		for (int i=0; i<MAX_ETHERCAT_SLAVE_TYPE; i++)   {
			((stAdamoEthSlave *)pPrj->GetSlaveArray ()[nIndex])->nDeviceType[i]=eEth;
			((stAdamoEthSlave *)pPrj->GetSlaveArray ()[nIndex])->liEnableVariables[i].Reset ();
		}
	}
}

/*
** InitializeTitles :
*/
void CDlgHWEthercatNet::InitializeTitles ()
{
	GetDlgItem (IDC_DEVICE_TYPE)->SetWindowText (LOADSTRING (IDS_EC_DEVICE_TYPE));
	GetDlgItem (IDC_REQUESTED_STATE)->SetWindowText (LOADSTRING (IDS_REQUESTED_STATE));
	GetDlgItem (IDC_EC_INIT)->SetWindowText (LOADSTRING (IDS_EC_INIT));
	GetDlgItem (IDC_EC_PREOPERATIONAL)->SetWindowText (LOADSTRING (IDS_EC_PREOPERATIONAL));
	GetDlgItem (IDC_EC_SAFEOPERATIONAL)->SetWindowText (LOADSTRING (IDS_EC_SAFEOPERATIONAL));
	GetDlgItem (IDC_EC_OPERATIONAL)->SetWindowText (LOADSTRING (IDS_EC_OPERATIONAL));
	GetDlgItem (IDC_ST_DC_START)->SetWindowText (LOADSTRING (IDS_EC_ST_DC_START));
	GetDlgItem (IDC_ST_FREQUENCY)->SetWindowText (LOADSTRING (IDS_EC_ST_FREQUENCY));
	GetDlgItem (IDC_CHECK_CONTINOUS_MEASURING)->SetWindowText (LOADSTRING (IDS_EC_CHECK_CONTINOUS_MEASURING));
	return;
}

/*
** InitializeControls :
*/
void CDlgHWEthercatNet::InitializeControls ()
{
    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
	CString str;
	__int64 nDCStartTime;
	int nDCFrequency, nDCOffsetTime;
	int nID, nRequestedState, nSelectedDriver = 0;
	CAdamoTemplate *pTmpl=((CIdeApp *)AfxGetApp ())->GetTemplate (enConfTemplate);

	if (pTmpl)   {
		CProject *pPrj=GETWS ()->GetActiveProject ();
		if (pPrj)   {
			CPtrArray &slaveArray=pPrj->GetSlaveArray ();
			nRequestedState=((stAdamoEthSlave *)slaveArray[0])->nRequestedState;
		}
	}
	switch (nRequestedState)   {
		case DEVICE_STATE_INIT :
			nID=IDC_EC_INIT;
			break;
		case DEVICE_STATE_PREOP :
			nID=IDC_EC_PREOPERATIONAL;
			break;
		case DEVICE_STATE_SAFEOP :
			nID=IDC_EC_SAFEOPERATIONAL;
			break;
		case DEVICE_STATE_OP :
			nID=IDC_EC_OPERATIONAL;
			break;
	};
	CheckRadioButton (IDC_EC_INIT, IDC_EC_OPERATIONAL, nID);
    m_ctrlEditDCStartTime.SubclassEdit(IDC_EDIT_DC_START, this, PES_NUMBERS);
    m_ctrlEditFrequency.SubclassEdit(IDC_EDIT_FREQUENCY, this, PES_NUMBERS);
	m_ctrlEditDCOffsetTime.SubclassEdit(IDC_EDIT_DC_OFFSET, this, PES_NUMBERS);
	((CButton *)GetDlgItem (IDC_CHECK_CONTINOUS_MEASURING))->SetCheck (pACF->GetEthercatContinousMeasuring ());
	nDCStartTime=pACF->GetEthercatDCStartTime ();
	nDCFrequency=pACF->GetEthercatFrequency ();
	nDCOffsetTime=pACF->GetEthercatDCOffsetTime ();
	str.Format ("%lld", nDCStartTime);
	m_ctrlEditDCStartTime.SetWindowText (str);
	str.Format ("%d", nDCFrequency);
	m_ctrlEditFrequency.SetWindowText (str);
	str.Format ("%d", nDCOffsetTime);
	m_ctrlEditDCOffsetTime.SetWindowText (str);
    ((CComboBox*)GetDlgItem (IDC_COMBO_DRIVER_HPE))->AddString ("ie1g0");
    ((CComboBox*)GetDlgItem (IDC_COMBO_DRIVER_HPE))->AddString ("ie1g");
    ((CComboBox*)GetDlgItem (IDC_COMBO_DRIVER_HPE))->AddString ("rtl1g");
	CString strHpeDriver = pACF->GetHpeDriver ();
	if (strHpeDriver == "ie1g")
		nSelectedDriver = 0;
	else
	if (strHpeDriver == "ie1g0")
		nSelectedDriver = 1;
	else
	if (strHpeDriver == "rtl1g")
		nSelectedDriver = 2;
    ((CComboBox*)GetDlgItem (IDC_COMBO_DRIVER_HPE))->SetCurSel (nSelectedDriver);
	return;
}

/*
** OnEcInit :
*/
void CDlgHWEthercatNet::OnEcInit() 
{
	// TODO: Add your control notification handler code here
	SetHWModified ();
}

/*
** OnEcPreoperational :
*/
void CDlgHWEthercatNet::OnEcPreoperational() 
{
	// TODO: Add your control notification handler code here
	SetHWModified ();
}

/*
** OnEcSafeoperational :
*/
void CDlgHWEthercatNet::OnEcSafeoperational() 
{
	// TODO: Add your control notification handler code here
	SetHWModified ();
}

/*
** OnEcOperational :
*/
void CDlgHWEthercatNet::OnEcOperational() 
{
	// TODO: Add your control notification handler code here
	SetHWModified ();
}

/*
** OnChangePsw :
*/
void CDlgHWEthercatNet::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}

/*
** InitPsw :
*/
void CDlgHWEthercatNet::InitPsw ()
{
    if (m_ePswLevel<=ePswBuilder)   {
        GetDlgItem (IDC_EC_INIT)->EnableWindow ();
        GetDlgItem (IDC_EC_PREOPERATIONAL)->EnableWindow ();
        GetDlgItem (IDC_EC_SAFEOPERATIONAL)->EnableWindow ();
        GetDlgItem (IDC_EC_OPERATIONAL)->EnableWindow ();
		m_grid.EnableWindow ();
    }
    else   {
        GetDlgItem (IDC_EC_INIT)->EnableWindow (false);
        GetDlgItem (IDC_EC_PREOPERATIONAL)->EnableWindow (false);
        GetDlgItem (IDC_EC_SAFEOPERATIONAL)->EnableWindow (false);
        GetDlgItem (IDC_EC_OPERATIONAL)->EnableWindow (false);
		m_grid.EnableWindow (false);
    }
}

/*
** OnChangeDCStart :
*/
void CDlgHWEthercatNet::OnChangeDCStart() 
{
	if (m_bInitialized)
		SetHWModified ();
}

/*
** OnChangeDCOffset :
*/
void CDlgHWEthercatNet::OnChangeDCOffset() 
{
	if (m_bInitialized)
		SetHWModified ();
}

/*
** OnChangeFrequency :
*/
void CDlgHWEthercatNet::OnChangeFrequency() 
{
	if (m_bInitialized)
		SetHWModified ();
}

/*
** OnBnClickedCheckContinousMeasuring :
*/
void CDlgHWEthercatNet::OnBnClickedCheckContinousMeasuring()
{
	// TODO: Add your control notification handler code here
	SetHWModified ();
}

void CDlgHWEthercatNet::OnCbnSelchangeComboDriverHpe()
{
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	if (m_bInitialized)
		SetHWModified ();
}
