// CanOpenAnalogIO.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "canopenanalogio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanOpenAnalogIO dialog


CCanOpenAnalogIO::CCanOpenAnalogIO(CWnd* pParent /*=NULL*/)
	: CDialog(CCanOpenAnalogIO::IDD, pParent), m_pConfigDoc (NULL), m_bInitialized (false), m_ePswLevel (ePswUser), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CCanOpenAnalogIO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCanOpenAnalogIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCanOpenAnalogIO)
	DDX_Control(pDX, IDC_ST_OUTANAG, m_stOutAnag);
	DDX_Control(pDX, IDC_ST_INPANAG, m_stInpAnag);
	//DDX_Control(pDX, IDC_GRID_HW_CAN_INPANAG, m_GridInpAna);
	//DDX_Control(pDX, IDC_GRID_HW_CAN_OUTANAG, m_GridOutAna);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCanOpenAnalogIO, CDialog)
	//{{AFX_MSG_MAP(CCanOpenAnalogIO)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanOpenAnalogIO message handlers

BOOL CCanOpenAnalogIO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    m_GridInpAna.SetConfigDoc   (m_pConfigDoc);
    m_GridOutAna.SetConfigDoc   (m_pConfigDoc);
    m_GridInpAna.SetProject   (m_pPrj);
    m_GridOutAna.SetProject   (m_pPrj);
	InitializeGrid (m_GridInpAna, IDC_GRID_HW_CAN_INPANAG, 12, 4, 2);
	InitializeGrid (m_GridOutAna, IDC_GRID_HW_CAN_OUTANAG, 12, 4, 2);
    InitializeGridData ();
    InitializeTitles ();
    InitPsw ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitializeGrid
*/
void CCanOpenAnalogIO::InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode)
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_Grid.Create (rc, this, nID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_Grid.SetMode (nMode);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_Grid.SetEditable(false);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedColumnCount(1);
    m_Grid.SetColumnCount(nCol+1);
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
** InitializeTitles :
*/
void CCanOpenAnalogIO::InitializeTitles ()
{
    CString str;
    str.LoadString (IDS_CONFIG_HW_TITLE_INPANA);
    m_stInpAnag.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_TITLE_OUTANA);
    m_stOutAnag.SetWindowText (str);
}

void CCanOpenAnalogIO::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        int nDelta=cx-20, nDimPrimaColonna, nDeltaColumn;
        if (nDelta>=0)   {
            m_stInpAnag.MoveWindow (10, 2, 100, 16);
	        m_GridInpAna.MoveWindow (10, 18, nDelta, 13*19+44);
            m_stOutAnag.MoveWindow (10, 322, 100, 16);
	        m_GridOutAna.MoveWindow (10, 338, nDelta, 13*19+44);
            nDimPrimaColonna=m_GridInpAna.GetColumnWidth (0);
            nDeltaColumn=nDelta-4*nDimPrimaColonna-4;
            if (nDeltaColumn>0)   {
                m_GridInpAna.SetColumnWidth (2, nDeltaColumn);
            m_GridInpAna.SetColumnWidth (3, nDimPrimaColonna);
            m_GridInpAna.SetColumnWidth (4, nDimPrimaColonna);
            }
            nDimPrimaColonna=m_GridOutAna.GetColumnWidth (0);
            nDeltaColumn=nDelta-4*nDimPrimaColonna-4;
            if (nDeltaColumn>0)   {
                m_GridOutAna.SetColumnWidth (2, nDeltaColumn);
            m_GridOutAna.SetColumnWidth (3, nDimPrimaColonna);
            m_GridOutAna.SetColumnWidth (4, nDimPrimaColonna);
            }
            m_GridInpAna.ExpandLastColumn ();
            m_GridOutAna.ExpandLastColumn ();
        }
    }
}

/*
** InitializaGridData :
*/
void CCanOpenAnalogIO::InitializeGridData ()
{
    CString str;
    str.Format ("C%dR%d", m_nKinemaAddress+1, m_nNode);
    m_GridInpAna.SetGridData (1, 1, 1, 2, str+"IAPin1");
    m_GridInpAna.SetGridData (2, 1, 2, 2, str+"IAPin2");
    m_GridInpAna.SetGridData (3, 1, 3, 2, str+"IAPin3");
    m_GridInpAna.SetGridData (4, 1, 4, 2, str+"IAPin4");
    m_GridInpAna.SetGridData (5, 1, 5, 2, str+"IAPin5");
    m_GridInpAna.SetGridData (6, 1, 6, 2, str+"IAPin6");
    m_GridInpAna.SetGridData (7, 1, 7, 2, str+"IAPin7");
    m_GridInpAna.SetGridData (8, 1, 8, 2, str+"IAPin8");
    m_GridInpAna.SetGridData (9, 1, 9, 2, str+"IAPin9");
    m_GridInpAna.SetGridData (10, 1, 10, 2, str+"IAPin10");
    m_GridInpAna.SetGridData (11, 1, 11, 2, str+"IAPin11");
    m_GridInpAna.SetGridData (12, 1, 12, 2, str+"IAPin12");
    m_GridInpAna.SetItemText (1, 0, "IA1");
    m_GridInpAna.SetItemText (2, 0, "IA2");
    m_GridInpAna.SetItemText (3, 0, "IA3");
    m_GridInpAna.SetItemText (4, 0, "IA4");
    m_GridInpAna.SetItemText (5, 0, "IA5");
    m_GridInpAna.SetItemText (6, 0, "IA6");
    m_GridInpAna.SetItemText (7, 0, "IA7");
    m_GridInpAna.SetItemText (8, 0, "IA8");
    m_GridInpAna.SetItemText (9, 0, "IA9");
    m_GridInpAna.SetItemText (10, 0, "IA10");
    m_GridInpAna.SetItemText (11, 0, "IA11");
    m_GridInpAna.SetItemText (12, 0, "IA12");
    m_GridInpAna.SetType (enInputAnalogico);
    m_GridInpAna.UpdateData ();

    m_GridOutAna.SetGridData (1, 1, 1, 2, str+"OAPin1");
    m_GridOutAna.SetGridData (2, 1, 2, 2, str+"OAPin2");
    m_GridOutAna.SetGridData (3, 1, 3, 2, str+"OAPin3");
    m_GridOutAna.SetGridData (4, 1, 4, 2, str+"OAPin4");
    m_GridOutAna.SetGridData (5, 1, 5, 2, str+"OAPin5");
    m_GridOutAna.SetGridData (6, 1, 6, 2, str+"OAPin6");
    m_GridOutAna.SetGridData (7, 1, 7, 2, str+"OAPin7");
    m_GridOutAna.SetGridData (8, 1, 8, 2, str+"OAPin8");
    m_GridOutAna.SetGridData (9, 1, 9, 2, str+"OAPin9");
    m_GridOutAna.SetGridData (10, 1, 10, 2, str+"OAPin10");
    m_GridOutAna.SetGridData (11, 1, 11, 2, str+"OAPin11");
    m_GridOutAna.SetGridData (12, 1, 12, 2, str+"OAPin12");
    m_GridOutAna.SetItemText (1, 0, "OA1");
    m_GridOutAna.SetItemText (2, 0, "OA2");
    m_GridOutAna.SetItemText (3, 0, "OA3");
    m_GridOutAna.SetItemText (4, 0, "OA4");
    m_GridOutAna.SetItemText (5, 0, "OA5");
    m_GridOutAna.SetItemText (6, 0, "OA6");
    m_GridOutAna.SetItemText (7, 0, "OA7");
    m_GridOutAna.SetItemText (8, 0, "OA8");
    m_GridOutAna.SetItemText (9, 0, "OA9");
    m_GridOutAna.SetItemText (10, 0, "OA10");
    m_GridOutAna.SetItemText (11, 0, "OA11");
    m_GridOutAna.SetItemText (12, 0, "OA12");
    m_GridOutAna.SetType (enOutputAnalogico);
    m_GridOutAna.UpdateData ();
}

/*
** RemoveCurrentConnection :
*/
void CCanOpenAnalogIO::RemoveCurrentConnection ()
{
    m_GridInpAna.RemoveCurrentConnection ();
    m_GridOutAna.RemoveCurrentConnection ();
}

/*
** Find :
*/
void CCanOpenAnalogIO::Find (const char *szBuffer)
{
    m_GridInpAna.Find (szBuffer);
    m_GridOutAna.Find (szBuffer);
}


void CCanOpenAnalogIO::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_GridInpAna.DestroyWindow ();
	m_GridOutAna.DestroyWindow ();
}

/*
** InitPsw :
*/
void CCanOpenAnalogIO::InitPsw ()
{
    if (m_ePswLevel<=ePswBuilder)   {
        m_GridInpAna.EnableWindow ();
        m_GridOutAna.EnableWindow ();
    }
    else   {
        m_GridInpAna.EnableWindow (false);
        m_GridOutAna.EnableWindow (false);
    }
}

/*
** OnChangePsw :
*/
void CCanOpenAnalogIO::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    InitPsw ();
}
