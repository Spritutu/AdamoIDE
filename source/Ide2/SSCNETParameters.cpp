// SSCNETParameters.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoTemplates.h"
#include "sscnetparameters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSCNETParameters dialog


CSSCNETParameters::CSSCNETParameters(CWnd* pParent /*=NULL*/)
	: CDialog(CSSCNETParameters::IDD, pParent), m_pConfigDoc (NULL), m_bInitialized (false), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CSSCNETParameters)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSSCNETParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSCNETParameters)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//DDX_Control(pDX, IDC_GRID_HW_SSCNET_PAR, m_grid);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSSCNETParameters, CDialog)
	//{{AFX_MSG_MAP(CSSCNETParameters)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSCNETParameters message handlers

/*
** OnInitDialog :
*/
BOOL CSSCNETParameters::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetConfigDoc ();
    m_grid.SetConfigDoc (m_pConfigDoc);
	m_grid.SetProject (m_pPrj);
    InitializeGrid (IDC_GRID_HW_SSCNET_PAR);
    InitializeData ();
	m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** OnSize :
*/
void CSSCNETParameters::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        m_grid.MoveWindow (0, 0, cx, cy);
        m_grid.SetColumnWidth (1, 120);
        m_grid.SetColumnWidth (3, 120);
        m_grid.ExpandLastColumn ();
    }
}

/*
** InitializeGrid :
*/
void CSSCNETParameters::InitializeGrid (int nID)
{
    RECT rc;

    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_grid.Create (rc, this, nID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_grid.SetVirtualMode(false);
    m_grid.SetFixedColumnCount(1);
    m_grid.SetColumnCount(5);
    m_grid.SetFixedRowCount ();
    m_grid.SetRowCount (46);
    m_grid.SetEditable ();
    m_grid.SetListMode ();
    m_grid.FillGridCaption ();
}

/*
** InitializeData :
*/
void CSSCNETParameters::InitializeData ()
{
    m_grid.InitializeData ();
}

/*
** SetGridData :
*/
void CSSCNETParameters::SetGridData (int nDataNumber, CString strParNumber, CString strParSymbol, CString strParUnit, CString strParName, bool bIsEditable, bool bIsHex)
{
    m_grid.SetGridData (nDataNumber, strParNumber, strParSymbol, strParUnit, strParName, bIsEditable, bIsHex);
}

/*
** SetConfigDoc :
*/
void CSSCNETParameters::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)
            m_pConfigDoc=pDoc;
    }
}

/*
** InitParData :
*/
void CSSCNETParameters::InitParData (int nPar)
{
    m_grid.InitParData (nPar);
}

/*
** SetParValue :
*/
void CSSCNETParameters::SetParValue (int nPar, WORD w)
{
    m_grid.SetParValue (nPar, w);
}

/*
** GetParValue :
*/
void CSSCNETParameters::GetParValue (int nPar, WORD *pw)
{
    m_grid.GetParValue (nPar, pw);
}

/*
** GetSelectedParams :
*/
void CSSCNETParameters::GetSelectedParams (WORD *pW, int *nNumPar)
{
    m_grid.GetSelectedParams (pW, nNumPar);
}

void CSSCNETParameters::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_grid.DestroyWindow ();
}

void CSSCNETParameters::EnableGrid (bool b)
{
    m_grid.EnableWindow (b);
}
