// DlgEcConfigSlaves.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DlgEcConfigSlaves.h"
#include "DlgEcConfigSingleSlave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgEcConfigSlaves dialog


CDlgEcConfigSlaves::CDlgEcConfigSlaves(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgEcConfigSlaves::IDD, pParent), m_pEcSlave (NULL), m_pFlatTabCtrl (NULL), m_bInitialized (false), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CDlgEcConfigSlaves)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgEcConfigSlaves::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgEcConfigSlaves)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgEcConfigSlaves, CDialog)
	//{{AFX_MSG_MAP(CDlgEcConfigSlaves)
	ON_WM_SIZE()
	ON_WM_DESTROY()
    ON_MESSAGE (WM_EC_ENABLE_TAB, OnEnableTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgEcConfigSlaves message handlers

/*
**
*/
BOOL CDlgEcConfigSlaves::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CreateTab ();
	m_bInitialized=true;	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** CreateTab :
*/
void CDlgEcConfigSlaves::CreateTab ()
{
    RECT rc;

    memset (&rc, 0, sizeof (RECT));
	rc.bottom=100; rc.left=20; rc.right=500; rc.top=20;
	m_pFlatTabCtrl = new CXTFlatTabCtrl();
	if (m_pFlatTabCtrl->Create(WS_CHILD | WS_VISIBLE | FTS_XT_BOTTOM | FTS_XT_TOOLTIPS,
		&rc, this, IDC_FLAT_TABCTRL))   {
		CDlgEcConfigSingleSlave *pDialog=new CDlgEcConfigSingleSlave;
		pDialog->SetEcSlave (m_pEcSlave, 0);
		pDialog->SetProject (m_pPrj);
		pDialog->Create (IDD_DLG_EC_CONFIG_SINGLE_SLAVE, m_pFlatTabCtrl);
		m_pFlatTabCtrl->InsertItem(0, "", pDialog);
		pDialog=new CDlgEcConfigSingleSlave;
		pDialog->SetEcSlave (m_pEcSlave, 1);
		pDialog->SetProject (m_pPrj);
		pDialog->Create (IDD_DLG_EC_CONFIG_SINGLE_SLAVE, m_pFlatTabCtrl);
		m_pFlatTabCtrl->InsertItem(1, "", pDialog);
		pDialog=new CDlgEcConfigSingleSlave;
		pDialog->SetEcSlave (m_pEcSlave, 2);
		pDialog->SetProject (m_pPrj);
		pDialog->Create (IDD_DLG_EC_CONFIG_SINGLE_SLAVE, m_pFlatTabCtrl);
		m_pFlatTabCtrl->InsertItem(2, "", pDialog);
		pDialog=new CDlgEcConfigSingleSlave;
		pDialog->SetEcSlave (m_pEcSlave, 3);
		pDialog->SetProject (m_pPrj);
		pDialog->Create (IDD_DLG_EC_CONFIG_SINGLE_SLAVE, m_pFlatTabCtrl);
		m_pFlatTabCtrl->InsertItem(3, "", pDialog);
		m_pFlatTabCtrl->SetCurSel (0);
	}
	for (int i=0; i<MAX_ETHERCAT_SLAVE_TYPE; i++)	{
		switch (m_pEcSlave->nDeviceType[i])   {
		case eEth :
			m_pFlatTabCtrl->SetItemText (i, LOADSTRING (IDS_EC_TAB_EMPTY));
			break;
		case eEthAsse :
			m_pFlatTabCtrl->SetItemText (i, LOADSTRING (IDS_EC_TAB_ASSI));
			break;
		case eEthIO :
			m_pFlatTabCtrl->SetItemText (i, LOADSTRING (IDS_EC_TAB_IO));
			break;
		case eEthIOAnalogic :
			m_pFlatTabCtrl->SetItemText (i, LOADSTRING (IDS_EC_TAB_IOANAG));
			break;
		}
	}
}

/*
** OnSize :
*/
void CDlgEcConfigSlaves::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if (m_bInitialized)
        m_pFlatTabCtrl->MoveWindow (10, 10, cx-20, cy-40);
}

/*
** OnDestroy :
*/
void CDlgEcConfigSlaves::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete m_pFlatTabCtrl;
}

/*
** OnOK :
*/
void CDlgEcConfigSlaves::OnOK ()
{
	for (int i=0; i<MAX_ETHERCAT_SLAVE_TYPE; i++)	{
		CDlgEcConfigSingleSlave *pDialog=(CDlgEcConfigSingleSlave *) m_pFlatTabCtrl->GetItemWindow (i);
		if (pDialog)
			pDialog->OnOK ();
	}
	CDialog::OnOK ();
}

/*
** OnCancel :
*/
void CDlgEcConfigSlaves::OnCancel ()
{
	CDialog::OnCancel ();
}

BOOL CDlgEcConfigSlaves::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	int cursel;
	if (IDC_FLAT_TABCTRL == (UINT)wParam)  {
		NMHDR* pNMHDR = (NMHDR*)lParam;
		switch(pNMHDR->code)   {
		case TCN_SELCHANGING:
			m_nCurTabSel = m_pFlatTabCtrl->GetCurSel ();
			break;
		case TCN_SELCHANGE:
			cursel=m_pFlatTabCtrl->GetCurSel ();
			if (cursel>0)   {
				CDlgEcConfigSingleSlave *pDialog=(CDlgEcConfigSingleSlave *) m_pFlatTabCtrl->GetItemWindow (cursel-1);
				if (!pDialog->IsEnabled ())
					m_pFlatTabCtrl->SetCurSel (m_nCurTabSel);
			}
			break;
		}
	}
    return CDialog::OnNotify(wParam, lParam, pResult);
}

/*
** OnEnableTab :
*/
LRESULT CDlgEcConfigSlaves::OnEnableTab (WPARAM wParam, LPARAM lParam)
{
	CString str;
	WORD wTab=LOWORD (lParam);
	WORD wSel=HIWORD (lParam);
	WORD wEnabled=wParam;

	if (wEnabled)   {
		switch (wSel)   {
		case eEth :
			str=LOADSTRING (IDS_EC_TAB_EMPTY);
			break;
		case eEthAsse :
			str=LOADSTRING (IDS_EC_TAB_ASSI);
			break;
		case eEthIO :
			str=LOADSTRING (IDS_EC_TAB_IO);
			break;
		case eEthIOAnalogic :
			str=LOADSTRING (IDS_EC_TAB_IOANAG);
			break;
		}
	}
	else
		str=LOADSTRING (IDS_EC_TAB_EMPTY);
	m_pFlatTabCtrl->SetItemText (wTab, str);
	return 0;
}

