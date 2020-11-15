// DlgTriggers.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ide2.h"
#include "mainframe.h"
#include "AdamoGlobal.h"
#include "DlgTriggers.h"
#include "GridCellCombo.h"
#include "GridCellNumeric.h"

static CString *aszCaption[5];

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggers dialog
CDlgTriggers::CDlgTriggers(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTriggers::IDD, pParent), m_bInitialized (false)
{
	//{{AFX_DATA_INIT(CDlgTriggers)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	memset (aszCaption, 0, sizeof (aszCaption));
}

void CDlgTriggers::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTriggers)
	DDX_Control(pDX, IDC_BUTTRIGGERSDELETE, m_btnDelete);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTriggers, CDialog)
	//{{AFX_MSG_MAP(CDlgTriggers)
	ON_WM_DESTROY()
	ON_WM_SIZE()
    ON_MESSAGE (WM_DRAGOSCDEVICE, DragOscDevice)
	ON_BN_CLICKED(IDC_BUTTRIGGERSDELETE, OnButTriggersDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTriggers message handlers

BOOL CDlgTriggers::OnInitDialog() 
{
	RECT rc;

	CDialog::OnInitDialog();
	InitStrings ();
	// TODO: Add extra initialization here
    rc.left=0; rc.top=0; rc.right=0; rc.bottom=0;
    m_Grid.Create (rc, this, IDC_GRIDTRIGGERS, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xF5, 0xF5, 0xF5));
    m_Grid.SetEditable(true);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedRowCount();
    m_Grid.SetFixedColumnCount(0);
    m_Grid.SetRowCount(MAXTRIGGERSROWS);
    m_Grid.SetColumnCount(MAXTRIGGERSCOLS);
    //m_Grid.SetColumnResize ();
    //m_Grid.SetAutoSizeStyle ();
    m_Grid.SetGridLines (GVL_NONE);
    m_Grid.InitControl ();
    FillColumnsCaption ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgTriggers::FillColumnsCaption ()
{
    GV_ITEM Item;
    int i;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_TRIGGERSSYMBOLS), 17, 1, RGB(0,128,128));
	m_Grid.SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
    for (i=0; i<MAXTRIGGERSCOLS; i++)   {
		Item.col=i;
        Item.iImage=i;
        Item.strText=*aszCaption[i];
        m_Grid.SetItem(&Item);
    }
}

int CDlgTriggers::AddDevice (CAdamoBase* pab)
{
    int nB=0;
    switch (pab->GetType())   {
        case enInputDigitale :
            m_Grid.AddAD (pab);
            break;
        case enOutputDigitale :
            m_Grid.AddAD (pab);
            break;
        case enInputAnalogico :
            m_Grid.AddCompare (pab);
            break;
        case enOutputAnalogico :
            m_Grid.AddCompare (pab);
            break;
        case enAsse :
            m_Grid.AddAsse (pab);
            break;
        case enTimer :
            m_Grid.AddCompare (pab);
            break;
        case enCounter :
            m_Grid.AddCompare (pab);
            break;
    }
    m_Grid.Invalidate();
    return nB;
}

void CDlgTriggers::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    m_Grid.CloseControl ();
    m_Grid.DeleteAllItems();
    DestroyStrings ();
}

void CDlgTriggers::OnOK ()
{
}

void CDlgTriggers::OnCancel ()
{
}

int CDlgTriggers::GetTriggers (CPtrList& lTriggers)
{
    return m_Grid.GetTriggers (lTriggers);
}

void CDlgTriggers::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
    if (m_bInitialized)   {
        m_Grid.MoveWindow (0, 0, cx-100, cy, TRUE);    	
        SetColumnSize ();
        m_btnDelete.MoveWindow (cx-100, 0, 100, 29);
    }
}

void CDlgTriggers::SetColumnSize ()
{
    RECT rc;
    int n;

    GetClientRect (&rc);
    m_Grid.SetColumnWidth (0, 100);
    m_Grid.SetColumnWidth (2, 160);
    m_Grid.SetColumnWidth (3, 100);
    m_Grid.SetColumnWidth (4, 100);
    if (rc.right<700)
        n=121;
    else
        n=rc.right-579;
    m_Grid.SetColumnWidth (1, n);
}

LONG CDlgTriggers::DragOscDevice(UINT wParam, LONG lParam)
{
    AddDevice ((CAdamoBase*)lParam);
    return 0;
}

void CDlgTriggers::OnButTriggersDelete() 
{
    m_Grid.DeleteRow ();
}

void CDlgTriggers::DeleteAllRows ()
{
    int i, n=m_Grid.GetCount ();
    for (i=0; i<n; i++)
        m_Grid.DeleteRow ();
}

CAdamoBase* CDlgTriggers::AddFromFile (CString str, int nTipoDev, int nDatoAsse, int nCmp, int nValue, double fpValue)
{
    CAdamoBase* pDev=NULL;
    /* cominciamo con il cercare il dispositivo passato per parametro */
    CAdamoCfgMachine* pMachine=(CAdamoCfgMachine*) GETWS()->GetActiveProject()->GetMachine ();
    if (pMachine)   {
        pDev=(CAdamoBase*)pMachine->GetItem (str);
        if (pDev&&pDev->GetType()==nTipoDev)   {
            AddDevice (pDev);
            switch (pDev->GetType())   {
                case enInputDigitale :
                case enOutputDigitale :
                    m_Grid.AddADFromFile (nValue);
                    break;
                case enAsse :
                    m_Grid.AddAsseFromFile (nDatoAsse, nCmp, nValue, fpValue);
                    break;
                case enInputAnalogico :
                case enOutputAnalogico :
                case enTimer :
                case enCounter :
                    m_Grid.AddCompareFromFile (nCmp, fpValue);
                    break;
            }
        }
    }
    return pDev;
}

/*
** InitStrings : inizializza le stringhe della dialog
*/
void CDlgTriggers::InitStrings ()
{
    for (int i=0; i<5; i++)   {
        aszCaption[i]=new CString;
        *aszCaption[i]=LOADSTRING (IDS_ADAMODLGTRIGGERS_1+i);
    }
}

/*
** DestroyStrings : distrugge le stringhe della dialog
*/
void CDlgTriggers::DestroyStrings ()
{
    for (int i=0; i<5; i++)
        _delete (aszCaption[i]);
}
