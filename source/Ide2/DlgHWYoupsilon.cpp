// DlgHWYoupsilon.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoTemplates.h"
#include "DlgHWYoupsilon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgHWYoupsilon dialog
IMPLEMENT_DYNAMIC (CDlgHWYoupsilon, CDialog)

CDlgHWYoupsilon::CDlgHWYoupsilon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHWYoupsilon::IDD, pParent), m_bInitialized (false), m_pConfigDoc (NULL), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CDlgHWYoupsilon)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgHWYoupsilon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgHWYoupsilon)
	DDX_Control(pDX, IDC_ST_OUTDIG, m_stOutDigTitle);
	DDX_Control(pDX, IDC_ST_OUTANA, m_stInpDigTitle);
	DDX_Control(pDX, IDC_ST_INPDIG, m_stOutAnaTitle);
	DDX_Control(pDX, IDC_ST_INPANA, m_stInpAnaTitle);
	DDX_Control(pDX, IDC_ST_ASSI, m_stAxisTitle);
	DDX_Control(pDX, IDC_BMP_YOUPSILON, m_bmpYoupsilon);
	//DDX_Control(pDX, IDC_GRID_HW_INANA,  m_GridInpAna);             // associate the grid window with a C++ object
	//DDX_Control(pDX, IDC_GRID_HW_OUTANA, m_GridOutAna);             // associate the grid window with a C++ object
	//DDX_Control(pDX, IDC_GRID_HW_INDIG,  m_GridInpDig);             // associate the grid window with a C++ object
	//DDX_Control(pDX, IDC_GRID_HW_OUTDIG, m_GridOutDig);             // associate the grid window with a C++ object
	//DDX_Control(pDX, IDC_GRID_HW_YOUPSILON, m_GridAssi);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgHWYoupsilon, CDialog)
	//{{AFX_MSG_MAP(CDlgHWYoupsilon)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgHWYoupsilon message handlers

BOOL CDlgHWYoupsilon::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitializeGrid (m_GridAssi, IDC_GRID_HW_YOUPSILON, 5, 0);
	InitializeGrid (m_GridInpAna, IDC_GRID_HW_INANA, 2, 0);
	InitializeGrid (m_GridOutAna, IDC_GRID_HW_OUTANA, 2, 0);
	InitializeGrid (m_GridInpDig, IDC_GRID_HW_INDIG, 10, 0);
	InitializeGrid (m_GridOutDig, IDC_GRID_HW_OUTDIG, 10, 0);
    InitializeTitles ();
	SetConfigDoc ();
    m_GridAssi.SetConfigDoc   (m_pConfigDoc);
    m_GridInpAna.SetConfigDoc (m_pConfigDoc);
    m_GridOutAna.SetConfigDoc (m_pConfigDoc);
    m_GridInpDig.SetConfigDoc (m_pConfigDoc);
    m_GridOutDig.SetConfigDoc (m_pConfigDoc);
    m_GridAssi.SetProject   (m_pPrj);
    m_GridInpAna.SetProject   (m_pPrj);
    m_GridOutAna.SetProject   (m_pPrj);
    m_GridInpDig.SetProject   (m_pPrj);
    m_GridOutDig.SetProject   (m_pPrj);
    InitializeGridData ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitializeTitles :
*/
void CDlgHWYoupsilon::InitializeTitles ()
{
    CString str;
    str.LoadString (IDS_CONFIG_HW_TITLE_ASSI);
    m_stAxisTitle.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_TITLE_INPANA);
    m_stInpAnaTitle.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_TITLE_OUTANA);
    m_stOutAnaTitle.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_TITLE_INPDIG);
    m_stInpDigTitle.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_TITLE_OUTDIG);
    m_stOutDigTitle.SetWindowText (str);
}

/*
** InitializeGrid
*/
void CDlgHWYoupsilon::InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nMode)
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_Grid.Create (rc, this, nID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_Grid.SetMode (nMode);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_Grid.SetEditable(false);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedColumnCount(1);
    m_Grid.SetColumnCount(3);
    m_Grid.SetFixedRowCount ();
    m_Grid.SetRowCount (nRow+1);
    m_Grid.SetEditable ();
    m_Grid.EnableDragAndDrop ();
    m_Grid.SetListMode ();
    m_Grid.FillGridCaption ();
    for (int i=1; i<nRow+1; i++)
        m_Grid.SetCellType (i, 1, RUNTIME_CLASS (CConfigHWGridCell));
}

/*
** OnSize :
*/
void CDlgHWYoupsilon::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        m_stAxisTitle.MoveWindow (10, 12, 100, 18);
	    m_GridAssi.MoveWindow (10, 30, cx-345, 6*19+5);
        m_stInpAnaTitle.MoveWindow (10, 152, 100, 18);
	    m_GridInpAna.MoveWindow (10, 170, cx-345, 3*19+5);
        m_stOutAnaTitle.MoveWindow (10, 234, 100, 18);
	    m_GridOutAna.MoveWindow (10, 252, cx-345, 3*19+5);
        m_stInpDigTitle.MoveWindow (10, 316, 100, 18);
	    m_GridInpDig.MoveWindow (10, 334, cx/2-10, 11*19+5);
        m_stOutDigTitle.MoveWindow (cx/2+10, 316, 100, 18);
	    m_GridOutDig.MoveWindow (cx/2+10, 334, cx-(cx/2)-20, 11*19+5);
        m_GridAssi.ExpandLastColumn();
        m_GridInpAna.ExpandLastColumn();
        m_GridOutAna.ExpandLastColumn();
        m_GridInpDig.ExpandLastColumn();
        m_GridOutDig.ExpandLastColumn();
        m_bmpYoupsilon.MoveWindow (cx-324, 10, 314, 314);
        ((CWnd &)m_GridAssi).EnableScrollBarCtrl (SB_HORZ, FALSE);
        ((CWnd &)m_GridAssi).EnableScrollBarCtrl (SB_VERT, FALSE);
        ((CWnd &)m_GridInpAna).EnableScrollBarCtrl (SB_HORZ, FALSE);
        ((CWnd &)m_GridInpAna).EnableScrollBarCtrl (SB_VERT, FALSE);
        ((CWnd &)m_GridOutAna).EnableScrollBarCtrl (SB_HORZ, FALSE);
        ((CWnd &)m_GridOutAna).EnableScrollBarCtrl (SB_VERT, FALSE);
        ((CWnd &)m_GridInpDig).EnableScrollBarCtrl (SB_HORZ, FALSE);
        ((CWnd &)m_GridInpDig).EnableScrollBarCtrl (SB_VERT, FALSE);
        ((CWnd &)m_GridOutDig).EnableScrollBarCtrl (SB_HORZ, FALSE);
        ((CWnd &)m_GridOutDig).EnableScrollBarCtrl (SB_VERT, FALSE);
    }
}

/*
** SetConfigDoc :
*/
void CDlgHWYoupsilon::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)
            m_pConfigDoc=pDoc;
    }
}

/*
** InitializeGridData :
*/
void CDlgHWYoupsilon::InitializeGridData ()
{
    CString str;
    str.Format ("Y%d", m_nKinemaAddress+1);
    m_GridAssi.SetGridData (1, 1, 1, 2, str+"Ax0");
    m_GridAssi.SetGridData (2, 1, 2, 2, str+"Ax1");
    m_GridAssi.SetGridData (3, 1, 3, 2, str+"Ax2");
    m_GridAssi.SetGridData (4, 1, 4, 2, str+"Ax3");
    m_GridAssi.SetGridData (5, 1, 5, 2, str+"Ax4");
    m_GridAssi.SetItemText (1, 0, "Ax0");
    m_GridAssi.SetItemText (2, 0, "Ax1");
    m_GridAssi.SetItemText (3, 0, "Ax2");
    m_GridAssi.SetItemText (4, 0, "Ax3");
    m_GridAssi.SetItemText (5, 0, "Ax4");
    m_GridAssi.SetType (enAsse);
    m_GridAssi.UpdateData ();

    m_GridInpAna.SetGridData (1, 1, 1, 2, str+"IAPin1");
    m_GridInpAna.SetGridData (2, 1, 2, 2, str+"IAPin2");
    m_GridInpAna.SetItemText (1, 0, "IA1");
    m_GridInpAna.SetItemText (2, 0, "IA2");
    m_GridInpAna.SetType (enInputAnalogico);
    m_GridInpAna.UpdateData ();

    m_GridOutAna.SetGridData (1, 1, 1, 2, str+"OAPin1");
    m_GridOutAna.SetGridData (2, 1, 2, 2, str+"OAPin2");
    m_GridOutAna.SetItemText (1, 0, "OA1");
    m_GridOutAna.SetItemText (2, 0, "OA2");
    m_GridOutAna.SetType (enOutputAnalogico);
    m_GridOutAna.UpdateData ();

    m_GridInpDig.SetGridData (1, 1, 1, 2, str+"IDPin1");
    m_GridInpDig.SetGridData (2, 1, 2, 2, str+"IDPin2");
    m_GridInpDig.SetGridData (3, 1, 3, 2, str+"IDPin3");
    m_GridInpDig.SetGridData (4, 1, 4, 2, str+"IDPin4");
    m_GridInpDig.SetGridData (5, 1, 5, 2, str+"IDPin5");
    m_GridInpDig.SetGridData (6, 1, 6, 2, str+"IDPin6");
    m_GridInpDig.SetGridData (7, 1, 7, 2, str+"IDPin7");
    m_GridInpDig.SetGridData (8, 1, 8, 2, str+"IDPin8");
    m_GridInpDig.SetGridData (9, 1, 9, 2, str+"IDPin9");
    m_GridInpDig.SetGridData (10, 1, 10, 2, str+"IDPin10");
    m_GridInpDig.SetItemText (1, 0, "ID1");
    m_GridInpDig.SetItemText (2, 0, "ID2");
    m_GridInpDig.SetItemText (3, 0, "ID3");
    m_GridInpDig.SetItemText (4, 0, "ID4");
    m_GridInpDig.SetItemText (5, 0, "ID5");
    m_GridInpDig.SetItemText (6, 0, "ID6");
    m_GridInpDig.SetItemText (7, 0, "ID7");
    m_GridInpDig.SetItemText (8, 0, "ID8");
    m_GridInpDig.SetItemText (9, 0, "ID9");
    m_GridInpDig.SetItemText (10, 0, "ID10");
    m_GridInpDig.SetType (enInputDigitale);
    m_GridInpDig.UpdateData ();

    m_GridOutDig.SetGridData (1, 1, 1, 2, str+"ODPin1");
    m_GridOutDig.SetGridData (2, 1, 2, 2, str+"ODPin2");
    m_GridOutDig.SetGridData (3, 1, 3, 2, str+"ODPin3");
    m_GridOutDig.SetGridData (4, 1, 4, 2, str+"ODPin4");
    m_GridOutDig.SetGridData (5, 1, 5, 2, str+"ODPin5");
    m_GridOutDig.SetGridData (6, 1, 6, 2, str+"ODPin6");
    m_GridOutDig.SetGridData (7, 1, 7, 2, str+"ODPin7");
    m_GridOutDig.SetGridData (8, 1, 8, 2, str+"ODPin8");
    m_GridOutDig.SetGridData (9, 1, 9, 2, str+"ODPin9");
    m_GridOutDig.SetGridData (10, 1, 10, 2, str+"ODPin10");
    m_GridOutDig.SetItemText (1, 0, "OD1");
    m_GridOutDig.SetItemText (2, 0, "OD2");
    m_GridOutDig.SetItemText (3, 0, "OD3");
    m_GridOutDig.SetItemText (4, 0, "OD4");
    m_GridOutDig.SetItemText (5, 0, "OD5");
    m_GridOutDig.SetItemText (6, 0, "OD6");
    m_GridOutDig.SetItemText (7, 0, "OD7");
    m_GridOutDig.SetItemText (8, 0, "OD8");
    m_GridOutDig.SetItemText (9, 0, "OD9");
    m_GridOutDig.SetItemText (10, 0, "OD10");
    m_GridOutDig.SetType (enOutputDigitale);
    m_GridOutDig.UpdateData ();
}

/*
** RemoveCurrentConnection :
*/
void CDlgHWYoupsilon::RemoveCurrentConnection ()
{
    m_GridAssi.RemoveCurrentConnection ();
    m_GridInpAna.RemoveCurrentConnection ();
    m_GridOutAna.RemoveCurrentConnection ();
    m_GridInpDig.RemoveCurrentConnection ();
    m_GridOutDig.RemoveCurrentConnection ();
}

/*
** OnCancel :
*/
void CDlgHWYoupsilon::OnCancel ()
{
}

/*
** OnOK :
*/
void CDlgHWYoupsilon::OnOK ()
{
}

/*
** Find :
*/
void CDlgHWYoupsilon::Find (const char *szBuffer)
{
    m_GridAssi.Find (szBuffer);
    m_GridInpAna.Find (szBuffer);
    m_GridOutAna.Find (szBuffer);
    m_GridInpDig.Find (szBuffer);
    m_GridOutDig.Find (szBuffer);
}

void CDlgHWYoupsilon::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    m_GridAssi.DestroyWindow ();
    m_GridInpAna.DestroyWindow ();
    m_GridOutAna.DestroyWindow ();
    m_GridInpDig.DestroyWindow ();
    m_GridOutDig.DestroyWindow ();
}
