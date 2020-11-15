// GlobalVariable.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "GlobalVariable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlobalVariable dialog

CGlobalVariable::CGlobalVariable(CWnd* pParent /*=NULL*/)
	: CAdamoDialog(CGlobalVariable::IDD, pParent), m_bFreeze (false), m_bRunAlways (false), m_nGL (0), m_nLevel (-1), m_nProg (-1), m_bLocalTimer (false), m_pParentWnd (NULL)
{
	//{{AFX_DATA_INIT(CGlobalVariable)
	//}}AFX_DATA_INIT
}

void CGlobalVariable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGlobalVariable)
	DDX_Control(pDX, IDC_STATICVALUE, m_ctrlStaticValue);
	DDX_Control(pDX, IDC_STATICNAME, m_ctrlStaticName);
	DDX_Control(pDX, IDC_FREEZEVARIABLE, m_ctrlButFreeze);
	DDX_Control(pDX, IDC_STNAME, m_ctrlName);
	DDX_Control(pDX, IDOK, m_ctrlButOk);
    //DDX_Control(pDX, IDC_GLOBALGRID, m_Grid);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGlobalVariable, CDialog)
	//{{AFX_MSG_MAP(CGlobalVariable)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_FREEZEVARIABLE, OnFreezeVariable)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GLOBALGRID, OnGridEndEdit)
	ON_NOTIFY(GVN_BUTTONCHECKED, IDC_GLOBALGRID, OnGridButtonChecked)
	ON_BN_CLICKED(IDC_AGGIORNA, &CGlobalVariable::OnBnClickedUpdatevariable)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlobalVariable message handlers

BOOL CGlobalVariable::OnInitDialog() 
{
	RECT rc;
    ePswLevel PswLevel;
    bool bEditable;

    CDialog::OnInitDialog();
    PswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	if (GETWINDOWSVERSION() == WINDOWS_7)   {
		rc.left=93; rc.top=44; rc.right=273; rc.bottom=92;
		m_Grid.Create (rc, this, IDC_GLOBALGRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
		((CMainFrame *) AfxGetMainWnd ())->MoveDlgButton (m_ctrlButFreeze, 98, 119, 124, 192);
		((CMainFrame *) AfxGetMainWnd ())->MoveDlgButton (m_ctrlButOk, 98, 198, 124, 271);
	}
	else   {
		rc.left=93; rc.top=44; rc.right=273; rc.bottom=86;
		m_Grid.Create (rc, this, IDC_GLOBALGRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
	}
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    bEditable=m_nType!=LUA_TBOOLEAN && PswLevel<ePswUser;
    m_Grid.SetEditable(bEditable);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedRowCount(1);
    m_Grid.SetFixedColumnCount(0);
    m_Grid.SetRowCount(2);
    m_Grid.SetColumnCount(1);
    m_Grid.ExpandColumnsToFit (false);
    FillCaption ();
	SetWindowText (m_strCaption);
    m_ctrlName.SetWindowText (GetVariableName ());
    SetFont (((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_ctrlName.SetFont (((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_ctrlStaticName.SetFont (((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_ctrlStaticValue.SetFont (((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    if (m_nType==LUA_TBOOLEAN)
        m_Grid.SetCellType(1, 0, RUNTIME_CLASS(CGridCellCheck));
    if (m_bLocalTimer)
        m_nTimer=SetTimer (ID_GLOBALVARIBLETIMER, ID_GLOBALVARIBLETIMER_PERIOD, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGlobalVariable::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO: Change any attributes of the DC here
	if (nCtlColor==CTLCOLOR_STATIC)
        if (pWnd->GetDlgCtrlID()==IDC_STNAME)    {
            pDC->SetBkColor (RGB (255, 255, 255));
            return (HBRUSH)GetStockObject (WHITE_BRUSH);
        }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CGlobalVariable::OnDestroy() 
{
	CDialog::OnDestroy();
    if (m_bLocalTimer)
        KillTimer (m_nTimer);
    m_Grid.DestroyWindow ();
}

void CGlobalVariable::OnTimer() 
{
	COleVariant v;
    bool b;
    char szBuffer[256];
    if (m_bRunAlways)
        b=!m_bFreeze&&GetMachine()->IsCncOnline();
    else
        b=!m_bFreeze&&GetMachine()->IsCncRunning();
	if (b)	   {
        if (ReadData (v))   {
            if (v.vt!=VT_EMPTY)   {
                switch (m_nType)   {
                    case LUA_TBOOLEAN :
                        if (v.lVal)   {
                            strcpy (szBuffer, LOADSTRING (IDS_ADAMOVERO));
                            ((CGridCellCheck*)m_Grid.GetCell (1, 0))->SetCheck ();
                        }
                        else   {
                            strcpy (szBuffer, LOADSTRING (IDS_ADAMOFALSO));
                            ((CGridCellCheck*)m_Grid.GetCell (1, 0))->SetCheck (FALSE);
                        }    
                        break;
                    case LUA_TNUMBER :
                        sprintf (szBuffer, "%.3f", v.dblVal);
                        break;
                    case LUA_TSTRING :
                        strcpy (szBuffer, CString (v.bstrVal));
                        break;
                }
            }
            else
                szBuffer[0]='\0';
            if (strcmp (m_Grid.GetItemText (1, 0), szBuffer))   {
                m_Grid.SetItemText (1, 0, szBuffer);
                m_Grid.Invalidate (false);
            }
        }
    }
}

void CGlobalVariable::OnFreezeVariable() 
{
    if (m_bFreeze)	 {
        m_bFreeze=false;
        m_ctrlButFreeze.SetWindowText (LOADSTRING (IDS_ADAMODLGSLOTVIEW_1)); 
        OnTimer ();
    }
    else   {
        m_bFreeze=true;
        m_ctrlButFreeze.SetWindowText (LOADSTRING (IDS_ADAMODLGSLOTVIEW_2));
    }
}

void CGlobalVariable::FillCaption ()
{
    GV_ITEM Item;
    CString str;
    int nIndex;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_MATRIXSYMBOLS), 16, 1, RGB(0,128,128));
	m_Grid.SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 0;
    if (m_nType==LUA_TSTRING)   {
        nIndex=0;
        str="String";
    }
    else
        if (m_nType==LUA_TNUMBER)   {
            str="Number";
            nIndex=1;
        }
        else   {
            if (m_nType==LUA_TBOOLEAN)   {
                str="Boolean";
                nIndex=2;
            }
            else     {
                if (m_nType==LUA_TNIL)   {
                    str="Nil";
                    nIndex=3;
                }
            }
        }
    Item.iImage = nIndex;
    Item.strText=str;
    m_Grid.SetItem(&Item);
}

void CGlobalVariable::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    COleVariant v;
    bool b;
    char* c;

    if (m_bRunAlways)
        b=GetMachine()->IsCncOnline()!=0;
    else
        b=GetMachine()->IsCncRunning()!=0;
    if (b)	   {
        NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
        CString str=m_Grid.GetItemText (pItem->iRow, pItem->iColumn);
        str.TrimLeft(), str.TrimRight ();
        switch (m_nType)   {
            case LUA_TNUMBER :
                v=strtod (str, &c);
                break;
            case LUA_TSTRING :
                v=str;
                break;
        }
        WriteData (v);
    }
}

void CGlobalVariable::OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    COleVariant v;
    bool b;

    if (m_bRunAlways)
        b=GetMachine()->IsCncOnline()!=0;
    else
        b=GetMachine()->IsCncRunning()!=0;
    if (b)	   {
        GV_CHECKNOTIFY* pItem = (GV_CHECKNOTIFY*) pNotifyStruct;
        if (m_nType==LUA_TBOOLEAN)   {
            v.vt=VT_BOOL;
            v.boolVal=pItem->iCheck;
            WriteData (v);
        }
    }
}

bool CGlobalVariable::ReadData (COleVariant& v)
{
    bool b;

    switch (m_nGL)   {
    case 0 :
        b=GetMachine()->GetVariable (GetVariableName (), v) == S_OK;
        break;
    case 1 :
        b=GetMachine()->GetLocalVariable (GetVariableName (), m_nLevel, m_nProg, v) == S_OK;
        break;
    case 2 :
        b=GetMachine()->GetExpressionVariable (GetVariableName (), m_nLevel, v) == S_OK;
        break;
    }
    return b;
}

bool CGlobalVariable::WriteData (COleVariant& v)
{
    bool b;

    switch (m_nGL)   {
    case 0 :
        b=GetMachine()->SetVariable (GetVariableName (), v) == S_OK;
        break;
    case 1 :
        b=GetMachine()->SetLocalVariable (GetVariableName (), m_nLevel, m_nProg, v) == S_OK;
        break;
    case 2 :
        b=GetMachine()->SetExpressionVariable (GetVariableName (), m_nLevel, v) == S_OK;
    }
    return b;
}

void CGlobalVariable::OnClose() 
{
	CWnd* pParent = GetParentWnd ();
    ((CMainFrame *) AfxGetMainWnd ())->RemoveDialogList (GetKey ());
	if (pParent)
		pParent->SetFocus ();
}

void CGlobalVariable::OnOK() 
{
    CAdamoDialog::OnOK ();
    OnClose ();
}

void CGlobalVariable::OnCancel() 
{
    CAdamoDialog::OnCancel ();
    OnClose ();
}

void CGlobalVariable::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	OnTimer ();
	CDialog::OnTimer(nIDEvent);
}

void CGlobalVariable::OnBnClickedUpdatevariable()
{
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
    CRect rect;
    CCellID cell(1, 0);
    if (!m_Grid.GetCellRect(cell, rect))
        return;
    CGridCellBase* pCell = m_Grid.GetCell(1, 0);
    if (pCell)
        pCell->Edit(1, 0, rect, CPoint (-1, -1), IDC_INPLACE_CONTROL, VK_END);
	return;
}
