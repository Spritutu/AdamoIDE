// SSCNETIO.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoTemplates.h"
#include "SSCNETIO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSCNETIO dialog


CSSCNETIO::CSSCNETIO(CWnd* pParent /*=NULL*/)
	: CDialog(CSSCNETIO::IDD, pParent), m_pConfigDoc (NULL), m_bInitialized (false), m_nKinemaAddress (-1), m_nStation (-1), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CSSCNETIO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSSCNETIO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSCNETIO)
	//DDX_Control(pDX, IDC_GRID_HW_SSCNET_INDIG,  m_GridInpDig);             // associate the grid window with a C++ object
	//DDX_Control(pDX, IDC_GRID_HW_SSCNET_OUTDIG, m_GridOutDig);             // associate the grid window with a C++ object
	DDX_Control(pDX, IDC_ST_SSCNET_INPDIG, m_stInpDigTitle);
	DDX_Control(pDX, IDC_ST_SSCNET_OUTDIG, m_stOutDigTitle);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSSCNETIO, CDialog)
	//{{AFX_MSG_MAP(CSSCNETIO)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSCNETIO message handlers
/*
** InitializeGrid
*/
void CSSCNETIO::InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nMode)
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

BOOL CSSCNETIO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	InitializeGrid (m_GridInpDig, IDC_GRID_HW_SSCNET_INDIG, 3, 0);
	InitializeGrid (m_GridOutDig, IDC_GRID_HW_SSCNET_OUTDIG, 3, 0);
    InitializeTitles ();

	SetConfigDoc ();
    m_GridInpDig.SetConfigDoc (m_pConfigDoc);
    m_GridOutDig.SetConfigDoc (m_pConfigDoc);
    m_GridInpDig.SetProject (m_pPrj);
    m_GridOutDig.SetProject (m_pPrj);

    InitializeGridData ();

    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** SetConfigDoc :
*/
void CSSCNETIO::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)
            m_pConfigDoc=pDoc;
    }
}

void CSSCNETIO::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        m_stInpDigTitle.MoveWindow (0, 8, 100, 16);
        m_stOutDigTitle.MoveWindow (0, 130, 100, 18);
	    m_GridInpDig.MoveWindow (0, 30, cx, 4*19+16);
        m_GridInpDig.ExpandLastColumn();
        ((CWnd &)m_GridInpDig).EnableScrollBarCtrl (SB_HORZ, FALSE);
	    m_GridOutDig.MoveWindow (0, 150, cx, 4*19+16);
        m_GridOutDig.ExpandLastColumn();
        ((CWnd &)m_GridOutDig).EnableScrollBarCtrl (SB_HORZ, FALSE);
    }
}

/*
** InitializeTitles :
*/
void CSSCNETIO::InitializeTitles ()
{
    CString str;
    str.LoadString (IDS_CONFIG_HW_SSCNET_ST_DIGITALINPUT);
    m_stInpDigTitle.SetWindowText (str);
    str.LoadString (IDS_CONFIG_HW_SSCNET_ST_DIGITALOUTPUT);
    m_stOutDigTitle.SetWindowText (str);
}

/*
** InitializeGridData :
*/
void CSSCNETIO::InitializeGridData ()
{
    CString str;

    str.Format ("M%dAx%d", m_nKinemaAddress+1, m_nStation+1);

    m_GridInpDig.SetGridData (1, 1, 1, 2, str+"IDPin1");
    m_GridInpDig.SetGridData (2, 1, 2, 2, str+"IDPin2");
    m_GridInpDig.SetGridData (3, 1, 3, 2, str+"IDPin3");
    m_GridInpDig.SetItemText (1, 0, "ID1");
    m_GridInpDig.SetItemText (2, 0, "ID2");
    m_GridInpDig.SetItemText (3, 0, "ID3");
    m_GridInpDig.SetType (enInputDigitale);
    m_GridInpDig.UpdateData ();

    m_GridOutDig.SetGridData (1, 1, 1, 2, str+"ODPin1");
    m_GridOutDig.SetGridData (2, 1, 2, 2, str+"ODPin2");
    m_GridOutDig.SetGridData (3, 1, 3, 2, str+"ODPin3");
    m_GridOutDig.SetItemText (1, 0, "OD1");
    m_GridOutDig.SetItemText (2, 0, "OD2");
    m_GridOutDig.SetItemText (3, 0, "OD3");
    m_GridOutDig.SetType (enOutputDigitale);
    m_GridOutDig.UpdateData ();
}

/*
** Find :
*/
bool CSSCNETIO::Find (const char *szBuffer)
{
    bool bInpDig, bOutDig;

    bInpDig=m_GridInpDig.Find (szBuffer);
    bOutDig=m_GridOutDig.Find (szBuffer);
    return bInpDig || bOutDig;
}

/*
** RemoveCurrentConnection :
*/
void CSSCNETIO::RemoveCurrentConnection ()
{
    m_GridInpDig.RemoveCurrentConnection ();
    m_GridOutDig.RemoveCurrentConnection ();
}

void CSSCNETIO::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_GridInpDig.DestroyWindow ();
	m_GridOutDig.DestroyWindow ();
}
