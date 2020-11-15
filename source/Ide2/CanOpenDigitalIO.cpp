// CanOpenDigitalIO.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "canopendigitalio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCanOpenDigitalIO dialog


CCanOpenDigitalIO::CCanOpenDigitalIO(CWnd* pParent /*=NULL*/)
	: CDialog(CCanOpenDigitalIO::IDD, pParent), m_pConfigDoc (NULL), m_bInitialized (false), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CCanOpenDigitalIO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCanOpenDigitalIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCanOpenDigitalIO)
	DDX_Control(pDX, IDC_ST_OUTDIG, m_stOutDig);
	DDX_Control(pDX, IDC_ST_INPDIG, m_stInpDig);
	//DDX_Control(pDX, IDC_GRID_HW_CAN_INPDIG,  m_GridInpDig);
	//DDX_Control(pDX, IDC_GRID_HW_CAN_OUTDIG,  m_GridOutDig);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCanOpenDigitalIO, CDialog)
	//{{AFX_MSG_MAP(CCanOpenDigitalIO)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCanOpenDigitalIO message handlers

BOOL CCanOpenDigitalIO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_GridInpDig.SetConfigDoc   (m_pConfigDoc);
    m_GridOutDig.SetConfigDoc   (m_pConfigDoc);
    m_GridInpDig.SetProject   (m_pPrj);
    m_GridOutDig.SetProject   (m_pPrj);
	InitializeGrid (m_GridInpDig, IDC_GRID_HW_CAN_INPDIG, 8, 8, 1);
	InitializeGrid (m_GridOutDig, IDC_GRID_HW_CAN_OUTDIG, 8, 8, 1);
    InitializeGridData ();
    InitializeTitles ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitializeGrid
*/
void CCanOpenDigitalIO::InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode)
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
    m_Grid.FillGridCaption ();
    for (int i=1; i<nRow+1; i++)
        for (int n=1; n<nCol+1; n++)
            m_Grid.SetCellType (i, n, RUNTIME_CLASS(CConfigHWGridCell));
}

/*
** InitializeTitles :
*/
void CCanOpenDigitalIO::InitializeTitles ()
{
    CString str;
    str.LoadString (IDS_CONFIG_HW_TITLE_INPDIG);
    m_stInpDig.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_TITLE_OUTDIG);
    m_stOutDig.SetWindowText (str);
}

/*
** OnSize :
*/
void CCanOpenDigitalIO::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        int i;

        m_stInpDig.MoveWindow (10, 2, 100, 16);
	    m_GridInpDig.MoveWindow (10, 18, cx-20, 9*19+32);
        m_stOutDig.MoveWindow (10, 226, 100, 16);
	    m_GridOutDig.MoveWindow (10, 242, cx-20, 9*19+32);
        for (i=0; i<8; i++)   {
            m_GridInpDig.SetColumnWidth (i, cx/9);
            m_GridOutDig.SetColumnWidth (i, cx/9);
        }
        m_GridInpDig.SetColumnWidth (i, 1);
        m_GridOutDig.SetColumnWidth (i, 1);
        m_GridInpDig.ExpandLastColumn ();
        m_GridOutDig.ExpandLastColumn ();
    }
}

/*
** InitializaGridData :
*/
void CCanOpenDigitalIO::InitializeGridData ()
{
    CString str;
    str.Format ("C%dR%d", m_nKinemaAddress+1, m_nNode);
    m_GridInpDig.SetGridData (1, 1, -1, -1, str+"IDPin1");
    m_GridInpDig.SetGridData (1, 2, -1, -1, str+"IDPin2");
    m_GridInpDig.SetGridData (1, 3, -1, -1, str+"IDPin3");
    m_GridInpDig.SetGridData (1, 4, -1, -1, str+"IDPin4");
    m_GridInpDig.SetGridData (1, 5, -1, -1, str+"IDPin5");
    m_GridInpDig.SetGridData (1, 6, -1, -1, str+"IDPin6");
    m_GridInpDig.SetGridData (1, 7, -1, -1, str+"IDPin7");
    m_GridInpDig.SetGridData (1, 8, -1, -1, str+"IDPin8");
    m_GridInpDig.SetGridData (2, 1, -1, -1, str+"IDPin9");
    m_GridInpDig.SetGridData (2, 2, -1, -1, str+"IDPin10");
    m_GridInpDig.SetGridData (2, 3, -1, -1, str+"IDPin11");
    m_GridInpDig.SetGridData (2, 4, -1, -1, str+"IDPin12");
    m_GridInpDig.SetGridData (2, 5, -1, -1, str+"IDPin13");
    m_GridInpDig.SetGridData (2, 6, -1, -1, str+"IDPin14");
    m_GridInpDig.SetGridData (2, 7, -1, -1, str+"IDPin15");
    m_GridInpDig.SetGridData (2, 8, -1, -1, str+"IDPin16");
    m_GridInpDig.SetGridData (3, 1, -1, -1, str+"IDPin17");
    m_GridInpDig.SetGridData (3, 2, -1, -1, str+"IDPin18");
    m_GridInpDig.SetGridData (3, 3, -1, -1, str+"IDPin19");
    m_GridInpDig.SetGridData (3, 4, -1, -1, str+"IDPin20");
    m_GridInpDig.SetGridData (3, 5, -1, -1, str+"IDPin21");
    m_GridInpDig.SetGridData (3, 6, -1, -1, str+"IDPin22");
    m_GridInpDig.SetGridData (3, 7, -1, -1, str+"IDPin23");
    m_GridInpDig.SetGridData (3, 8, -1, -1, str+"IDPin24");
    m_GridInpDig.SetGridData (4, 1, -1, -1, str+"IDPin25");
    m_GridInpDig.SetGridData (4, 2, -1, -1, str+"IDPin26");
    m_GridInpDig.SetGridData (4, 3, -1, -1, str+"IDPin27");
    m_GridInpDig.SetGridData (4, 4, -1, -1, str+"IDPin28");
    m_GridInpDig.SetGridData (4, 5, -1, -1, str+"IDPin29");
    m_GridInpDig.SetGridData (4, 6, -1, -1, str+"IDPin30");
    m_GridInpDig.SetGridData (4, 7, -1, -1, str+"IDPin31");
    m_GridInpDig.SetGridData (4, 8, -1, -1, str+"IDPin32");
    m_GridInpDig.SetGridData (5, 1, -1, -1, str+"IDPin33");
    m_GridInpDig.SetGridData (5, 2, -1, -1, str+"IDPin34");
    m_GridInpDig.SetGridData (5, 3, -1, -1, str+"IDPin35");
    m_GridInpDig.SetGridData (5, 4, -1, -1, str+"IDPin36");
    m_GridInpDig.SetGridData (5, 5, -1, -1, str+"IDPin37");
    m_GridInpDig.SetGridData (5, 6, -1, -1, str+"IDPin38");
    m_GridInpDig.SetGridData (5, 7, -1, -1, str+"IDPin39");
    m_GridInpDig.SetGridData (5, 8, -1, -1, str+"IDPin40");
    m_GridInpDig.SetGridData (6, 1, -1, -1, str+"IDPin41");
    m_GridInpDig.SetGridData (6, 2, -1, -1, str+"IDPin42");
    m_GridInpDig.SetGridData (6, 3, -1, -1, str+"IDPin43");
    m_GridInpDig.SetGridData (6, 4, -1, -1, str+"IDPin44");
    m_GridInpDig.SetGridData (6, 5, -1, -1, str+"IDPin45");
    m_GridInpDig.SetGridData (6, 6, -1, -1, str+"IDPin46");
    m_GridInpDig.SetGridData (6, 7, -1, -1, str+"IDPin47");
    m_GridInpDig.SetGridData (6, 8, -1, -1, str+"IDPin48");
    m_GridInpDig.SetGridData (7, 1, -1, -1, str+"IDPin49");
    m_GridInpDig.SetGridData (7, 2, -1, -1, str+"IDPin50");
    m_GridInpDig.SetGridData (7, 3, -1, -1, str+"IDPin51");
    m_GridInpDig.SetGridData (7, 4, -1, -1, str+"IDPin52");
    m_GridInpDig.SetGridData (7, 5, -1, -1, str+"IDPin53");
    m_GridInpDig.SetGridData (7, 6, -1, -1, str+"IDPin54");
    m_GridInpDig.SetGridData (7, 7, -1, -1, str+"IDPin55");
    m_GridInpDig.SetGridData (7, 8, -1, -1, str+"IDPin56");
    m_GridInpDig.SetGridData (8, 1, -1, -1, str+"IDPin57");
    m_GridInpDig.SetGridData (8, 2, -1, -1, str+"IDPin58");
    m_GridInpDig.SetGridData (8, 3, -1, -1, str+"IDPin59");
    m_GridInpDig.SetGridData (8, 4, -1, -1, str+"IDPin60");
    m_GridInpDig.SetGridData (8, 5, -1, -1, str+"IDPin61");
    m_GridInpDig.SetGridData (8, 6, -1, -1, str+"IDPin62");
    m_GridInpDig.SetGridData (8, 7, -1, -1, str+"IDPin63");
    m_GridInpDig.SetGridData (8, 8, -1, -1, str+"IDPin64");
    m_GridInpDig.SetItemText (1, 0, "ID1");
    m_GridInpDig.SetItemText (2, 0, "ID2");
    m_GridInpDig.SetItemText (3, 0, "ID3");
    m_GridInpDig.SetItemText (4, 0, "ID4");
    m_GridInpDig.SetItemText (5, 0, "ID5");
    m_GridInpDig.SetItemText (6, 0, "ID6");
    m_GridInpDig.SetItemText (7, 0, "ID7");
    m_GridInpDig.SetItemText (8, 0, "ID8");
    m_GridInpDig.SetItemText (0, 1, "Bit1");
    m_GridInpDig.SetItemText (0, 2, "Bit2");
    m_GridInpDig.SetItemText (0, 3, "Bit3");
    m_GridInpDig.SetItemText (0, 4, "Bit4");
    m_GridInpDig.SetItemText (0, 5, "Bit5");
    m_GridInpDig.SetItemText (0, 6, "Bit6");
    m_GridInpDig.SetItemText (0, 7, "Bit7");
    m_GridInpDig.SetItemText (0, 8, "Bit8");
    m_GridInpDig.SetType (enInputDigitale);
    m_GridInpDig.UpdateData ();

    m_GridOutDig.SetGridData (1, 1, -1, -1, str+"ODPin1");
    m_GridOutDig.SetGridData (1, 2, -1, -1, str+"ODPin2");
    m_GridOutDig.SetGridData (1, 3, -1, -1, str+"ODPin3");
    m_GridOutDig.SetGridData (1, 4, -1, -1, str+"ODPin4");
    m_GridOutDig.SetGridData (1, 5, -1, -1, str+"ODPin5");
    m_GridOutDig.SetGridData (1, 6, -1, -1, str+"ODPin6");
    m_GridOutDig.SetGridData (1, 7, -1, -1, str+"ODPin7");
    m_GridOutDig.SetGridData (1, 8, -1, -1, str+"ODPin8");
    m_GridOutDig.SetGridData (2, 1, -1, -1, str+"ODPin9");
    m_GridOutDig.SetGridData (2, 2, -1, -1, str+"ODPin10");
    m_GridOutDig.SetGridData (2, 3, -1, -1, str+"ODPin11");
    m_GridOutDig.SetGridData (2, 4, -1, -1, str+"ODPin12");
    m_GridOutDig.SetGridData (2, 5, -1, -1, str+"ODPin13");
    m_GridOutDig.SetGridData (2, 6, -1, -1, str+"ODPin14");
    m_GridOutDig.SetGridData (2, 7, -1, -1, str+"ODPin15");
    m_GridOutDig.SetGridData (2, 8, -1, -1, str+"ODPin16");
    m_GridOutDig.SetGridData (3, 1, -1, -1, str+"ODPin17");
    m_GridOutDig.SetGridData (3, 2, -1, -1, str+"ODPin18");
    m_GridOutDig.SetGridData (3, 3, -1, -1, str+"ODPin19");
    m_GridOutDig.SetGridData (3, 4, -1, -1, str+"ODPin20");
    m_GridOutDig.SetGridData (3, 5, -1, -1, str+"ODPin21");
    m_GridOutDig.SetGridData (3, 6, -1, -1, str+"ODPin22");
    m_GridOutDig.SetGridData (3, 7, -1, -1, str+"ODPin23");
    m_GridOutDig.SetGridData (3, 8, -1, -1, str+"ODPin24");
    m_GridOutDig.SetGridData (4, 1, -1, -1, str+"ODPin25");
    m_GridOutDig.SetGridData (4, 2, -1, -1, str+"ODPin26");
    m_GridOutDig.SetGridData (4, 3, -1, -1, str+"ODPin27");
    m_GridOutDig.SetGridData (4, 4, -1, -1, str+"ODPin28");
    m_GridOutDig.SetGridData (4, 5, -1, -1, str+"ODPin29");
    m_GridOutDig.SetGridData (4, 6, -1, -1, str+"ODPin30");
    m_GridOutDig.SetGridData (4, 7, -1, -1, str+"ODPin31");
    m_GridOutDig.SetGridData (4, 8, -1, -1, str+"ODPin32");
    m_GridOutDig.SetGridData (5, 1, -1, -1, str+"ODPin33");
    m_GridOutDig.SetGridData (5, 2, -1, -1, str+"ODPin34");
    m_GridOutDig.SetGridData (5, 3, -1, -1, str+"ODPin35");
    m_GridOutDig.SetGridData (5, 4, -1, -1, str+"ODPin36");
    m_GridOutDig.SetGridData (5, 5, -1, -1, str+"ODPin37");
    m_GridOutDig.SetGridData (5, 6, -1, -1, str+"ODPin38");
    m_GridOutDig.SetGridData (5, 7, -1, -1, str+"ODPin39");
    m_GridOutDig.SetGridData (5, 8, -1, -1, str+"ODPin40");
    m_GridOutDig.SetGridData (6, 1, -1, -1, str+"ODPin41");
    m_GridOutDig.SetGridData (6, 2, -1, -1, str+"ODPin42");
    m_GridOutDig.SetGridData (6, 3, -1, -1, str+"ODPin43");
    m_GridOutDig.SetGridData (6, 4, -1, -1, str+"ODPin44");
    m_GridOutDig.SetGridData (6, 5, -1, -1, str+"ODPin45");
    m_GridOutDig.SetGridData (6, 6, -1, -1, str+"ODPin46");
    m_GridOutDig.SetGridData (6, 7, -1, -1, str+"ODPin47");
    m_GridOutDig.SetGridData (6, 8, -1, -1, str+"ODPin48");
    m_GridOutDig.SetGridData (7, 1, -1, -1, str+"ODPin49");
    m_GridOutDig.SetGridData (7, 2, -1, -1, str+"ODPin50");
    m_GridOutDig.SetGridData (7, 3, -1, -1, str+"ODPin51");
    m_GridOutDig.SetGridData (7, 4, -1, -1, str+"ODPin52");
    m_GridOutDig.SetGridData (7, 5, -1, -1, str+"ODPin53");
    m_GridOutDig.SetGridData (7, 6, -1, -1, str+"ODPin54");
    m_GridOutDig.SetGridData (7, 7, -1, -1, str+"ODPin55");
    m_GridOutDig.SetGridData (7, 8, -1, -1, str+"ODPin56");
    m_GridOutDig.SetGridData (8, 1, -1, -1, str+"ODPin57");
    m_GridOutDig.SetGridData (8, 2, -1, -1, str+"ODPin58");
    m_GridOutDig.SetGridData (8, 3, -1, -1, str+"ODPin59");
    m_GridOutDig.SetGridData (8, 4, -1, -1, str+"ODPin60");
    m_GridOutDig.SetGridData (8, 5, -1, -1, str+"ODPin61");
    m_GridOutDig.SetGridData (8, 6, -1, -1, str+"ODPin62");
    m_GridOutDig.SetGridData (8, 7, -1, -1, str+"ODPin63");
    m_GridOutDig.SetGridData (8, 8, -1, -1, str+"ODPin64");
    m_GridOutDig.SetItemText (1, 0, "OD1");
    m_GridOutDig.SetItemText (2, 0, "OD2");
    m_GridOutDig.SetItemText (3, 0, "OD3");
    m_GridOutDig.SetItemText (4, 0, "OD4");
    m_GridOutDig.SetItemText (5, 0, "OD5");
    m_GridOutDig.SetItemText (6, 0, "OD6");
    m_GridOutDig.SetItemText (7, 0, "OD7");
    m_GridOutDig.SetItemText (8, 0, "OD8");
    m_GridOutDig.SetItemText (0, 1, "Bit1");
    m_GridOutDig.SetItemText (0, 2, "Bit2");
    m_GridOutDig.SetItemText (0, 3, "Bit3");
    m_GridOutDig.SetItemText (0, 4, "Bit4");
    m_GridOutDig.SetItemText (0, 5, "Bit5");
    m_GridOutDig.SetItemText (0, 6, "Bit6");
    m_GridOutDig.SetItemText (0, 7, "Bit7");
    m_GridOutDig.SetItemText (0, 8, "Bit8");
    m_GridOutDig.SetType (enOutputDigitale);
    m_GridOutDig.UpdateData ();
}

/*
** RemoveCurrentConnection :
*/
void CCanOpenDigitalIO::RemoveCurrentConnection ()
{
    m_GridInpDig.RemoveCurrentConnection ();
    m_GridOutDig.RemoveCurrentConnection ();
}

/*
** Find :
*/
void CCanOpenDigitalIO::Find (const char *szBuffer)
{
    m_GridInpDig.Find (szBuffer);
    m_GridOutDig.Find (szBuffer);
}

void CCanOpenDigitalIO::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_GridInpDig.DestroyWindow ();
	m_GridOutDig.DestroyWindow ();
}
