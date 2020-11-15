// DlgKC.cpp : file di implementazione
//

#include "stdafx.h"
#include "ide2.h"
#include "MainFrame.h"
#include "DlgKC.h"

// finestra di dialogo CDlgKC

IMPLEMENT_DYNAMIC(CDlgKC, CDialog)

CDlgKC::CDlgKC(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgKC::IDD, pParent), m_tbiml (NULL), m_bmpKCXYZBC (NULL), m_bmpKCTRT (NULL), m_bFirstTime (true)
{

}

CDlgKC::~CDlgKC()
{
}

void CDlgKC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_KINEMATIC_COMPENSATION, m_ctrlTabKC);
}

BEGIN_MESSAGE_MAP(CDlgKC, CDialog)
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_KINEMATIC_COMPENSATION, OnSelchangeTabKC)
END_MESSAGE_MAP()

CKCGrid::CKCGrid () : m_pDlg (NULL)
{
}

void CKCGrid::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
    if (nCol == 2)
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
}

void CKCGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
    CGridCtrl::OnEndEditCell(nRow, nCol, str);
    Invalidate ();
}

// gestori di messaggi CDlgKC
BOOL CDlgKC::OnInitDialog() 
{
    GV_ITEM Item;
	TC_ITEM TabCtrlItem;
	CString str;
    int i;
    RECT rc;

	CDialog::OnInitDialog();
    /* settiamo il font */	
    m_ctrlTabKC.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
	m_ctrlTabKC.SetMinTabWidth(156);
    m_tbiml=new CImageList;
	m_gridiml = new CImageList;
    m_bmpKCXYZBC=new CBitmap;  
    m_bmpKCTRT=new CBitmap;  
    m_bmpKCComment=new CBitmap;  
    m_bmpKCNumber=new CBitmap;  
    m_bmpKCXYZBC->LoadBitmap(IDB_CAPHW);
    m_bmpKCTRT->LoadBitmap(IDB_CAPHW);
    m_bmpKCComment->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpKCNumber->LoadBitmap(IDB_CAP_NUMBER_TYPE);
    m_tbiml->Create (16, 16, ILC_MASK, 0, 1);
    m_tbiml->Add(m_bmpKCXYZBC, RGB(0, 128,128));
    m_tbiml->Add(m_bmpKCTRT, RGB(0, 128,128));
    m_gridiml->Create (16, 16, ILC_MASK, 0, 1);
    m_gridiml->Add(m_bmpKCComment, RGB(0, 128,128));
    m_gridiml->Add(m_bmpKCNumber, RGB(0, 128,128));
    m_ctrlTabKC.SetImageList(m_tbiml);
	TabCtrlItem.mask = TCIF_TEXT|TCIF_IMAGE;
    str=LOADSTRING (IDS_KC_KINEMATIC_COMP_XYZCB);
	TabCtrlItem.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    TabCtrlItem.iImage=0;
	m_ctrlTabKC.InsertItem( 0, &TabCtrlItem );
	TabCtrlItem.mask = TCIF_TEXT|TCIF_IMAGE;
    str=LOADSTRING (IDS_KC_KINEMATIC_COMP_TRT);
	TabCtrlItem.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    TabCtrlItem.iImage=1;
	m_ctrlTabKC.InsertItem( 1, &TabCtrlItem );
	/* creiamo ora il grid */
    rc.left=18; rc.top=40; rc.right=456; rc.bottom=199;
    m_grid.Create (rc, this, IDC_GRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_grid.GetDefaultCell(FALSE, FALSE)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    m_grid.SetEditable(true);
    m_grid.SetVirtualMode(false);
    m_grid.SetFixedRowCount();
    m_grid.SetFixedColumnCount();
    m_grid.SetRowCount (6+1);
    m_grid.SetColumnCount(2+1);
    m_grid.ExpandColumnsToFit (false);
    /* inizializziamo ora le righe fisse */
    for (i=0; i<6; i++)   {
        m_grid.GetCell (i+1, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        str.Format("%d", i+1);
        m_grid.SetItemText(i+1, 0, str);
    }
    /* ora le colonne fisse */
	m_grid.SetImageList(m_gridiml);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 1;
    Item.iImage = 0;
    Item.strText=LOADSTRING (IDS_KC_KINEMATIC_COMP_PARAMETER);
    m_grid.SetItem(&Item);
    Item.mask = GVIF_TEXT | GVIF_IMAGE; 
	Item.row = 0;
	Item.col = 2;
    Item.iImage = 1;
    Item.strText=LOADSTRING (IDS_KC_KINEMATIC_COMP_VALUE);
    m_grid.SetItem(&Item);
    m_grid.SetKCDlg (this);
    m_grid.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
	SelectGrid (0);
	return TRUE;
}

void CDlgKC::OnDestroy() 
{
	CDialog::OnDestroy();
	delete m_tbiml;
	delete m_gridiml;
	delete m_bmpKCXYZBC;
	delete m_bmpKCTRT;
	delete m_bmpKCComment;
	delete m_bmpKCNumber;
}

void CDlgKC::OnSelchangeTabKC(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    int nCurSel=m_ctrlTabKC.GetCurSel ();
    SelectGrid (nCurSel);
	*pResult = 0;
}

void CDlgKC::SelectGrid (int nCurSel) 
{
	m_nCurSel = nCurSel;
	switch (nCurSel)   {
		case 0 :
			SelectXYZBCGrid ();
			break;
		case 1 :
			SelectTRTGrid ();
			break;
	}
	m_grid.Invalidate ();
}

void CDlgKC::SelectXYZBCGrid () 
{
	GetTRTData ();
	//m_grid.SetRowCount (3+1);

	m_grid.GetCell(1, 1)->SetText (LOADSTRING (IDS_KC_KINEMATIC_COMP_XYZBC_1));
	m_grid.GetCell(2, 1)->SetText (LOADSTRING (IDS_KC_KINEMATIC_COMP_XYZBC_2));
	m_grid.GetCell(3, 1)->SetText (LOADSTRING (IDS_KC_KINEMATIC_COMP_XYZBC_3));
	m_grid.GetCell(4, 1)->SetText ("");
	m_grid.GetCell(5, 1)->SetText ("");
	m_grid.GetCell(6, 1)->SetText ("");
	SetXYZBCData ();
}

void CDlgKC::SelectTRTGrid () 
{
	GetXYZBCData ();
	//m_grid.SetRowCount (6+1);
	m_grid.GetCell(1, 1)->SetText (LOADSTRING (IDS_KC_KINEMATIC_COMP_TRT_1));
	m_grid.GetCell(2, 1)->SetText (LOADSTRING (IDS_KC_KINEMATIC_COMP_TRT_2));
	m_grid.GetCell(3, 1)->SetText (LOADSTRING (IDS_KC_KINEMATIC_COMP_TRT_3));
	m_grid.GetCell(4, 1)->SetText (LOADSTRING (IDS_KC_KINEMATIC_COMP_TRT_4));
	m_grid.GetCell(5, 1)->SetText (LOADSTRING (IDS_KC_KINEMATIC_COMP_TRT_5));
	m_grid.GetCell(6, 1)->SetText (LOADSTRING (IDS_KC_KINEMATIC_COMP_TRT_6));
	SetTRTData ();
}

void CDlgKC::OnOK () 
{
	// TODO: Add extra validation here
	if (FillData ())
	    CDialog::OnOK();
}

int CDlgKC::FillData () 
{
	switch (m_nCurSel)   {
		case 0 :
			GetXYZBCData ();
			break;
		case 1 :
			GetTRTData ();
			break;
	}
	return 1;
}

void CDlgKC::GetTRTData () 
{
	CString str;
	double fp = 0.0;
	char* p;
	if (m_bFirstTime)
		m_bFirstTime = false;
	else   {
		for (int i = 0; i<sizeof (m_afpGeometry_TRT) / sizeof (double); i++)   {
			str = m_grid.GetCell (i+1, 2)->GetText ();
			str.Trim ();
			fp = strtod (str, &p);
			if (*p == '\0')
				m_afpGeometry_TRT[i] = fp;
		}
	}
}

void CDlgKC::GetXYZBCData () 
{
	CString str;
	double fp = 0.0;
	char* p;
	for (int i = 0; i<sizeof (m_afpGeometry_XYZCB) / sizeof (double); i++)   {
		str = m_grid.GetCell (i+1, 2)->GetText ();
		str.Trim ();
		fp = strtod (str, &p);
		if (*p == '\0')
			m_afpGeometry_XYZCB[i] = fp;
	}
}

void CDlgKC::SetTRTData () 
{
	CString str;
	for (int i = 0; i<sizeof (m_afpGeometry_TRT) / sizeof (double); i++)   {
		str.Format ("%.3f", m_afpGeometry_TRT[i]);
		m_grid.GetCell (i+1, 2)->SetText (str);
	}
}

void CDlgKC::SetXYZBCData () 
{
	CString str;
	for (int i = 0; i<sizeof (m_afpGeometry_XYZCB) / sizeof (double); i++)   {
		str.Format ("%.3f", m_afpGeometry_XYZCB[i]);
		m_grid.GetCell (i+1, 2)->SetText (str);
	}
	for (int i = 0; i<3; i++)   {
		m_grid.GetCell (i+4, 2)->SetText ("");
	}
}
