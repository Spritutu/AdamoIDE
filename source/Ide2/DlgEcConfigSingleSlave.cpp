// DlgEcConfigSingleSlave.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DlgEcConfigSingleSlave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEcConfigSingleSlave dialog


CDlgEcConfigSingleSlave::CDlgEcConfigSingleSlave(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEcConfigSingleSlave::IDD, pParent), m_pEcSlave (NULL), m_nTab (-1), m_pPrj (NULL), m_bSelectAll (false)
{
	//{{AFX_DATA_INIT(CDlgEcConfigSingleSlave)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEcConfigSingleSlave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEcConfigSingleSlave)
	DDX_Control(pDX, IDC_EC_TAB_ASSI, m_ctrlRadioAssi);
	DDX_Control(pDX, IDC_EC_TAB_IO, m_ctrlRadioIO);
	DDX_Control(pDX, IDC_EC_TAB_IOANAG, m_ctrlRadioIOAnag);
	DDX_Control(pDX, IDC_EC_TAB_TYPE, m_ctrlTabType);
	DDX_Control(pDX, IDC_EC_VAR_ENABLE, m_ctrlVarEnable);
	DDX_Control(pDX, IDC_TAB_ENABLE, m_ctrlTabEnable);
	DDX_Control(pDX, IDC_BTN_SELECT_ALL, m_ctrlSelectAll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEcConfigSingleSlave, CDialog)
	//{{AFX_MSG_MAP(CDlgEcConfigSingleSlave)
	ON_BN_CLICKED(IDC_TAB_ENABLE, OnTabEnable)
	ON_BN_CLICKED(IDC_EC_TAB_ASSI, OnEcTabAssi)
	ON_BN_CLICKED(IDC_EC_TAB_IO, OnEcTabIo)
	ON_BN_CLICKED(IDC_EC_TAB_IOANAG, OnEcTabIoanag)
	ON_BN_CLICKED(IDC_BTN_SELECT_ALL, OnBtnSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEcConfigSingleSlave message handlers

/*
** OnInitDialog :
*/
BOOL CDlgEcConfigSingleSlave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitStrings ();
	InitListView ();
	InitControls ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitStrings :
*/
void CDlgEcConfigSingleSlave::InitStrings ()
{
	SetWindowText (LOADSTRING (IDS_EC_DLG_CONFIGURAZIONE_SLAVE));
	m_ctrlTabEnable.SetWindowText (LOADSTRING (IDS_EC_TAB_ENABLE));
	m_ctrlRadioAssi.SetWindowText (LOADSTRING (IDS_EC_TAB_ASSI));
	m_ctrlRadioIO.SetWindowText (LOADSTRING (IDS_EC_TAB_IO));
	m_ctrlRadioIOAnag.SetWindowText (LOADSTRING (IDS_EC_TAB_IOANAG));
	m_ctrlTabType.SetWindowText (LOADSTRING (IDS_EC_TAB_TABTYPE));
	m_ctrlSelectAll.SetWindowText(LOADSTRING(IDS_EC_SELECT_ALL));
}

/*
** InitListView :
*/
void CDlgEcConfigSingleSlave::InitListView ()
{
    LVITEM lvi;
    CRect rect;
	int nIndex=0;
    char szBuffer[256];

	GetClientRect (&rect);
    m_ctrlVarEnable.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP);
    m_iIcons.Create (IDB_CONF_TREE, 16, 1, RGB(128,0,128));
    m_ctrlVarEnable.SetImageList (&m_iIcons, LVSIL_SMALL);
    m_ctrlVarEnable.InsertColumn(0,LOADSTRING (IDS_EC_VARENABLE_COLUMN),LVCFMT_LEFT,(rect.Width()/3*2)+14);
	CHeaderCtrl* hc=m_ctrlVarEnable.GetHeaderCtrl();
    HDITEM *hd = (HDITEM *)new char [10240];
    hc->GetItem(0, hd);
    hd->mask=HDI_IMAGE | HDI_FORMAT;
    hd->fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd->iImage=13;
    hc->SetItem(0, hd);
    delete hd;
	for (POSITION pos=m_pEcSlave->VariablesList.GetHeadPosition (); pos; nIndex++)   {
		stAdamoEthSlaveVariable *p=(stAdamoEthSlaveVariable *) m_pEcSlave->VariablesList.GetNext (pos);
		wsprintf (szBuffer, "%s", p->szName);
		lvi.mask = LVIF_TEXT | LVIF_IMAGE; 
		lvi.iItem = nIndex;
		lvi.iSubItem = 0; 
		lvi.iImage = 23;
		lvi.pszText = szBuffer;
		lvi.cchTextMax=256;
		m_ctrlVarEnable.InsertItem (&lvi);
	}
}

/*
** InitControls :
*/
void CDlgEcConfigSingleSlave::InitControls ()
{
	stHWEthVarMask iEnableMask;
	int nItem;

	switch (m_pEcSlave->nDeviceType[m_nTab])   {
		case eEth :
			m_ctrlTabEnable.SetCheck (FALSE);
			m_ctrlRadioAssi.EnableWindow (FALSE);
			m_ctrlRadioIO.EnableWindow (FALSE);
			m_ctrlRadioIOAnag.EnableWindow (FALSE);
			m_ctrlTabType.EnableWindow (FALSE);
			m_ctrlVarEnable.EnableWindow (FALSE);
			break;							    
		case eEthAsse :
			m_ctrlTabEnable.SetCheck (TRUE);
			m_ctrlRadioAssi.SetCheck (TRUE);
			break;
		case eEthIO :
			m_ctrlTabEnable.SetCheck (TRUE);
			m_ctrlRadioIO.SetCheck (TRUE);
			break;
		case eEthIOAnalogic :
			m_ctrlTabEnable.SetCheck (TRUE);
			m_ctrlRadioIOAnag.SetCheck (TRUE);
			break;
	}
	if (m_pEcSlave->nDeviceType[m_nTab]!=eEth)	 {
		iEnableMask=m_pEcSlave->liEnableVariables[m_nTab];
        nItem=m_ctrlVarEnable.GetNextItem (-1, LVNI_ALL);
        while (nItem!=-1)   {
			if (iEnableMask.BitTest (nItem))
				m_ctrlVarEnable.SetCheck (nItem);
            nItem=m_ctrlVarEnable.GetNextItem (nItem, LVNI_ALL);
        }
	}
}

void CDlgEcConfigSingleSlave::OnTabEnable() 
{
	// TODO: Add your control notification handler code here
	if (m_ctrlTabEnable.GetCheck ())   {
		m_ctrlRadioAssi.EnableWindow ();
		m_ctrlRadioIO.EnableWindow ();
		m_ctrlRadioIOAnag.EnableWindow ();
		m_ctrlTabType.EnableWindow ();
		m_ctrlVarEnable.EnableWindow ();
		GetParent ()->GetParent ()->SendMessage (WM_EC_ENABLE_TAB, 1, MAKELONG ((WORD)m_nTab, eEth));
	}
	else   {
		m_ctrlRadioAssi.EnableWindow (FALSE);
		m_ctrlRadioIO.EnableWindow (FALSE);
		m_ctrlRadioIOAnag.EnableWindow (FALSE);
		m_ctrlTabType.EnableWindow (FALSE);
		m_ctrlVarEnable.EnableWindow (FALSE);
		GetParent ()->GetParent ()->SendMessage (WM_EC_ENABLE_TAB, 0, MAKELONG ((WORD)m_nTab, eEth));
	}
}

/*
** OnOK :
*/
void CDlgEcConfigSingleSlave::OnOK ()
{
	stHWEthVarMask iEnableMask;
	int nId, nItem;

	if (m_ctrlTabEnable.GetCheck ())   {
		nId=GetCheckedRadioButton (IDC_EC_TAB_ASSI, IDC_EC_TAB_IOANAG);
		if (nId>0)   {
			switch (nId)   {
			case IDC_EC_TAB_ASSI :
				m_pEcSlave->nDeviceType[m_nTab]=eEthAsse;
				break;
			case IDC_EC_TAB_IO :
				m_pEcSlave->nDeviceType[m_nTab]=eEthIO;
				break;
			case IDC_EC_TAB_IOANAG :
				m_pEcSlave->nDeviceType[m_nTab]=eEthIOAnalogic;
				break;
			}
			nItem=m_ctrlVarEnable.GetNextItem (-1, LVNI_ALL);
			while (nItem!=-1)   {
				if (m_ctrlVarEnable.GetCheck (nItem))
					iEnableMask.BitSet (nItem);
				nItem=m_ctrlVarEnable.GetNextItem (nItem, LVNI_ALL);
			}
			m_pEcSlave->liEnableVariables[m_nTab]=iEnableMask;
		}
		else   {
			m_pEcSlave->nDeviceType[m_nTab]=eEth;
			m_pEcSlave->liEnableVariables[m_nTab].Reset ();
		}
	}
	else   {
		m_pEcSlave->nDeviceType[m_nTab]=eEth;
		m_pEcSlave->liEnableVariables[m_nTab].Reset ();
	}
	m_pPrj->SetHWConfigModified ();
}

/*
** OnCancel :
*/
void CDlgEcConfigSingleSlave::OnCancel ()
{
}

/*
** IsEnabled :
*/
bool CDlgEcConfigSingleSlave::IsEnabled ()
{
	return m_ctrlTabEnable.GetCheck ()==TRUE;	
}

/*
** OnEcTabAssi :
*/
void CDlgEcConfigSingleSlave::OnEcTabAssi() 
{
	// TODO: Add your control notification handler code here
	GetParent ()->GetParent ()->SendMessage (WM_EC_ENABLE_TAB, 1, MAKELONG ((WORD)m_nTab, eEthAsse));	
}

/*
** OnEcTabIo :
*/
void CDlgEcConfigSingleSlave::OnEcTabIo() 
{
	// TODO: Add your control notification handler code here
	GetParent ()->GetParent ()->SendMessage (WM_EC_ENABLE_TAB, 1, MAKELONG ((WORD)m_nTab, eEthIO));	
}

/*
** OnEcTabIoanag :
*/
void CDlgEcConfigSingleSlave::OnEcTabIoanag() 
{
	// TODO: Add your control notification handler code here
	GetParent ()->GetParent ()->SendMessage (WM_EC_ENABLE_TAB, 1, MAKELONG ((WORD)m_nTab, eEthIOAnalogic));	
}

/*
** OnBtnChooseJpg :
*/
void CDlgEcConfigSingleSlave::OnBtnSelectAll()
{
	m_bSelectAll = !m_bSelectAll;
	for (int nItem = 0; nItem < m_ctrlVarEnable.GetItemCount(); nItem++)
		m_ctrlVarEnable.SetCheck(nItem, m_bSelectAll);
	m_ctrlSelectAll.SetWindowTextA (LOADSTRING(m_bSelectAll ? IDS_EC_UNSELECT_ALL : IDS_EC_SELECT_ALL));
}
