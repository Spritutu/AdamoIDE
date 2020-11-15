// DlgSamples.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoGlobal.h"
#include "DlgSamples.h"
#include "DlgOscilloscope.h"
#include "ShowGlobalVars.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CString *aszCaption[6];

/////////////////////////////////////////////////////////////////////////////
// CDlgSamples dialog

CDlgSamples::CDlgSamples(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSamples::IDD, pParent), m_bInitialized (false), m_pDlgOscilloscope (NULL), m_nCurRow (-1), m_nCurColumn (-1), m_bGridButtonPressed (false)
{
	//{{AFX_DATA_INIT(CDlgSamples)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset (aszCaption, 0, sizeof (aszCaption));
}


void CDlgSamples::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSamples)
	DDX_Control(pDX, IDC_BUTSAMPLESDELETE, m_btnDelete);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSamples, CDialog)
	//{{AFX_MSG_MAP(CDlgSamples)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTSAMPLESDELETE, OnButSamplesDelete)
    ON_MESSAGE (WM_DRAGOSCDEVICE, DragOscDevice)
	ON_NOTIFY(GVN_BUTTONPRESSED, IDC_GRIDSAMPLES, OnGridButtonPressed)
	ON_NOTIFY(GVN_SELCHANGING, IDC_GRIDSAMPLES, OnGridStartSelChange)
	ON_NOTIFY(NM_CLICK, IDC_GRIDSAMPLES, OnGridClick)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSamples message handlers

BOOL CDlgSamples::OnInitDialog() 
{
	RECT rc;

	CDialog::OnInitDialog();
    InitStrings ();
    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_Grid.Create (rc, this, IDC_GRIDSAMPLES, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xF5, 0xF5, 0xF5));
    m_Grid.SetEditable(true);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedRowCount();
    m_Grid.SetFixedColumnCount(0);
    m_Grid.SetRowCount(MAXSAMPLESROWS);
    m_Grid.SetColumnCount(MAXSAMPLESSCOLS);
	m_Grid.ExpandColumnsToFit ();
    m_Grid.SetGridLines (GVL_NONE);
    m_Grid.InitControl ();
    FillColumnsCaption ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSamples::OnOK ()
{
}

void CDlgSamples::OnCancel ()
{
}

void CDlgSamples::FillColumnsCaption ()
{
    GV_ITEM Item;
    int i;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_SAMPLESSYMBOLS), 17, 1, RGB(0,128,128));
	m_Grid.SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
    for (i=0; i<MAXSAMPLESSCOLS; i++)   {
		Item.col=i;
        Item.iImage=i;
        Item.strText=*aszCaption[i];
        m_Grid.SetItem(&Item);
    }
}

void CDlgSamples::AddDevice (CAdamoBase* pab)
{
    m_Grid.AddDevice(pab);
    m_Grid.Invalidate();
}

int CDlgSamples::GetSamples (CPtrList& lSamples)
{
    return m_Grid.GetSamples (lSamples);
}

void CDlgSamples::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        m_Grid.MoveWindow (0, 0, cx-100, cy, TRUE);
        SetColumnSize ();
        m_btnDelete.MoveWindow (cx-100, 0, 100, 29);
    }
}

void CDlgSamples::SetColumnSize ()
{
    RECT rc;
    int n;

    GetClientRect (&rc);
    m_Grid.SetColumnWidth (0, 100);
    m_Grid.SetColumnWidth (2, 160);
    m_Grid.SetColumnWidth (3, 100);
    m_Grid.SetColumnWidth (4, 50);
    m_Grid.SetColumnWidth (5, 100);
    if (rc.right<850)
        n=220;
    else
        n=rc.right-630;
    m_Grid.SetColumnWidth (1, n);
}

LONG CDlgSamples::DragOscDevice(UINT wParam, LONG lParam)
{
    AddDevice ((CAdamoBase*)lParam);
    return 0;
}

void CDlgSamples::OnButSamplesDelete() 
{
	m_Grid.DeleteRow ();
}

void CDlgSamples::DeleteAllRows ()
{
    int i, n=m_Grid.GetCount ();
    for (i=0; i<n; i++)
        m_Grid.DeleteRow ();
}

CAdamoBase* CDlgSamples::AddFromFile (CString str, int nTipoDev, int nDatoAsse)
{
    CAdamoBase* pDev=NULL;
    /* cominciamo con il cercare il dispositivo passato per parametro */
    CAdamoCfgMachine* pMachine=(CAdamoCfgMachine*) GETWS()->GetActiveProject()->GetMachine ();
    if (pMachine)   {
        pDev=(CAdamoBase*)pMachine->GetItem (str);
        if (pDev&&pDev->GetType()==nTipoDev)   {
            AddDevice (pDev);
            if (pDev->GetType()==enAsse)
                m_Grid.AddAsseFromFile (nDatoAsse);
        }
    }
    return pDev;
}

void CDlgSamples::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    m_Grid.CloseControl ();	
    DestroyStrings ();
}

/*
** InitStrings : inizializza le stringhe della dialog
*/
void CDlgSamples::InitStrings ()
{
    aszCaption[0]=new CString;
    *aszCaption[0]=LOADSTRING (IDS_ADAMODLGSAMPLES_1);
    aszCaption[1]=new CString;
    *aszCaption[1]=LOADSTRING (IDS_ADAMODLGSAMPLES_2);
    aszCaption[2]=new CString;
    *aszCaption[2]=LOADSTRING (IDS_ADAMODLGSAMPLES_3);
    aszCaption[3]=new CString;
    *aszCaption[3]=LOADSTRING (IDS_ADAMODLGSAMPLES_4);
    aszCaption[4]=new CString;
    *aszCaption[4]=LOADSTRING (IDS_ADAMODLGSAMPLES_5);
    aszCaption[5]=new CString;
    *aszCaption[5]=LOADSTRING (IDS_ADAMODLGSAMPLES_6);
}

/*
** DestroyStrings : distrugge le stringhe della dialog
*/
void CDlgSamples::DestroyStrings ()
{
    for (int i=0; i<6; i++)
        _delete (aszCaption[i]);
}

/*
** OnGridButtonPressed :
*/
void CDlgSamples::OnGridButtonPressed(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    CShowGlobalVars sg;
    CString strName, strCol;
    int nRow, nType;
    //GV_CHECKNOTIFY* pItem = (GV_CHECKNOTIFY*) pNotifyStruct;

	((CGridCellToggleButton*)m_Grid.GetCell (m_nCurRow, m_nCurColumn))->ResetToggle ();
	sg.DoModal ();
	sg.GetResults (&strName, &nRow, &strCol, &nType);
	if (strName != "")   {
		if (nType != LUA_TSTRING)   {
			m_Grid.AddVariable (strName, nRow, strCol, nType);
			m_bGridButtonPressed = true;
		}
		else
			AfxMessageBox (LOADSTRING (IDS_CANNOT_TRACE_STRING), MB_ICONSTOP);
	}
}

void CDlgSamples::OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nCurRow=pItem->iRow;
    m_nCurColumn=pItem->iColumn;
}

void CDlgSamples::OnGridClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nCurRow=pItem->iRow;
    m_nCurColumn=pItem->iColumn;
	if (m_bGridButtonPressed)   {
		m_Grid.UpdateRow ();
		m_bGridButtonPressed = false;
	}
}
