// AdamoEthercatIO.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoEthercatIO.h"
#include "ConfigHWGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* grid di configurazione Eth IO*/
CAdamoEthercatIOGrid::CAdamoEthercatIOGrid ()
{
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatIO dialog

IMPLEMENT_DYNAMIC(CAdamoEthercatIO, CDialog)

CAdamoEthercatIO::CAdamoEthercatIO(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoEthercatIO::IDD, pParent), m_bInitialized (false), m_pDoc (NULL), m_pSlave (NULL), m_nNode (-1), m_nTab (-1), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CAdamoEthercatIO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoEthercatIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoEthercatIO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_ST_OUTDIG, m_stOutDig);
	DDX_Control(pDX, IDC_ST_INPDIG, m_stInpDig);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoEthercatIO, CDialog)
	//{{AFX_MSG_MAP(CAdamoEthercatIO)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatIO message handlers

BOOL CAdamoEthercatIO::OnInitDialog() 
{
	int nRow;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_GridInpDig.SetConfigDoc   (m_pDoc);
    m_GridOutDig.SetConfigDoc   (m_pDoc);
    m_GridInpDig.SetProject   (m_pPrj);
    m_GridOutDig.SetProject   (m_pPrj);
	LoadSlaveInfo ();
	nRow=m_pSlave->GetInputVariables ();
	if (nRow<12)
		nRow=12;
	InitializeGrid (m_GridInpDig, IDC_GRID_HW_ETH_INPDIG, nRow, 4, 4);
	nRow=m_pSlave->GetOutputVariables ();
	if (nRow<12)
		nRow=12;
	InitializeGrid (m_GridOutDig, IDC_GRID_HW_ETH_OUTDIG, nRow, 4, 4);
    InitializeGridData ();
    InitializeTitles ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitializeGrid
*/
void CAdamoEthercatIO::InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode)
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
	m_Grid.SetListMode ();
    for (int i=1; i<nRow+1; i++)
        for (int n=1; n<nCol+1; n++)
            m_Grid.SetCellType (i, n, RUNTIME_CLASS(CConfigHWGridCell));
}

/*
** FillGridCaption :
*/
void CAdamoEthercatIOGrid::FillGridCaption ()
{
    GV_ITEM Item;
    CString str;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_CONFIG_HW_GRID), 16, 1, RGB(0,128,128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 1;
    Item.iImage = 0;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_NOME);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 2;
    Item.iImage = 1;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_DESCRIZIONE);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 3;
    Item.iImage = 2;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_VARIABILE);
    SetItem(&Item);
	Item.row = 0;
	Item.col = 4;
    Item.iImage = 3;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_POSIZIONE);
    SetItem(&Item);
}

/*
** InitializeGridData :
*/
void CAdamoEthercatIO::InitializeGridData ()
{
    CString str;
	int i=0, n=0, q=0;

	for (POSITION pos=m_pSlave->VariablesList.GetHeadPosition (); pos; n++)   {
		stAdamoEthSlaveVariable *pVar=(stAdamoEthSlaveVariable *)m_pSlave->VariablesList.GetNext (pos);
		if (m_pSlave->liEnableVariables[m_nTab].BitTest (n))   {
			if (pVar->nInputOutput==0)   {
				str.Format ("E1R%dIDPin%d", m_nNode, pVar->nBitOffs);
				m_GridInpDig.SetGridData (q+1, 1, q+1, 2, str);
				str.Format ("ID%d", q+1);
				m_GridInpDig.SetItemText (q+1, 0, str);
				m_GridInpDig.SetItemText (q+1, 3, pVar->szName);
				str.Format ("%d --> %d", pVar->nBitOffs, pVar->nBitSize);
				m_GridInpDig.SetItemText (q+1, 4, str);
				q++;
			}
			else   {
				str.Format ("E1R%dODPin%d", m_nNode, pVar->nBitOffs);
				m_GridOutDig.SetGridData (i+1, 1, i+1, 2, str);
				str.Format ("OD%d", i+1);
				m_GridOutDig.SetItemText (i+1, 0, str);
				m_GridOutDig.SetItemText (i+1, 3, pVar->szName);
				str.Format ("%d --> %d", pVar->nBitOffs, pVar->nBitSize);
				m_GridOutDig.SetItemText (i+1, 4, str);
				i++;
			}
		}
	}
    m_GridInpDig.SetType (enInputDigitale);
    m_GridInpDig.UpdateData ();
    m_GridOutDig.SetType (enOutputDigitale);
    m_GridOutDig.UpdateData ();
}

/*
** InitializeTitles :
*/
void CAdamoEthercatIO::InitializeTitles ()
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
void CAdamoEthercatIO::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        m_stInpDig.MoveWindow (10, 2, 100, 16);
	    m_GridInpDig.MoveWindow (10, 18, cx-20, 13*19+5);
        m_stOutDig.MoveWindow (10, 272, 100, 16);
	    m_GridOutDig.MoveWindow (10, 288, cx-20, 13*19+5);
        m_GridInpDig.SetColumnWidth (0, 50);
        m_GridOutDig.SetColumnWidth (0, 50);
		m_GridInpDig.SetColumnWidth (1, 100);
        m_GridOutDig.SetColumnWidth (1, 100);
		m_GridInpDig.SetColumnWidth (2, 250);
        m_GridOutDig.SetColumnWidth (2, 250);
		m_GridInpDig.SetColumnWidth (3, 350);
        m_GridOutDig.SetColumnWidth (3, 350);
        m_GridInpDig.ExpandLastColumn ();
        m_GridOutDig.ExpandLastColumn ();
    }
}

/*
** LoadSlaveInfo :
*/
void CAdamoEthercatIO::LoadSlaveInfo ()
{
    CProject* pPrj=GETWS()->GetActiveProject();
	if (pPrj)   {
		CPtrArray &rSlaveArray=pPrj->GetSlaveArray ();
		m_pSlave=(stAdamoEthSlave *)rSlaveArray[m_nNode-1];
	}
}

/*
** Find :
*/
void CAdamoEthercatIO::Find (const char *szBuffer)
{
    m_GridInpDig.Find (szBuffer);
    m_GridOutDig.Find (szBuffer);
}

/*
** RemoveCurrentConnection :
*/
void CAdamoEthercatIO::RemoveCurrentConnection ()
{
    m_GridInpDig.RemoveCurrentConnection ();
    m_GridOutDig.RemoveCurrentConnection ();
}
