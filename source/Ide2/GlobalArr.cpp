// GlobalArr.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "GlobalArr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlobalArr dialog


CGlobalArr::CGlobalArr(CWnd* pParent /*=NULL*/)
	: CAdamoDialog(CGlobalArr::IDD, pParent), m_bFreeze (false)
{
	//{{AFX_DATA_INIT(CGlobalArr)
	//}}AFX_DATA_INIT
}


void CGlobalArr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGlobalArr)
	DDX_Control(pDX, IDOK, m_ctrlButOk);
	DDX_Control(pDX, IDC_ARRAYFREEZE, m_ctrlButFreeze);
	DDX_Control(pDX, IDC_ARRAYAGGIORNA, m_ctrlButUpdate);
	DDX_Control(pDX, IDC_STNAME, m_ctrlName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGlobalArr, CDialog)
	//{{AFX_MSG_MAP(CGlobalArr)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ARRAYFREEZE, OnArrayFreeze)
	//}}AFX_MSG_MAP
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRIDARRAY, OnGridEndEdit)
	ON_NOTIFY(GVN_SELCHANGING, IDC_GRIDARRAY, OnGridStartSelChange)
	ON_NOTIFY(GVN_BUTTONCHECKED, IDC_GRIDARRAY, OnGridButtonChecked)
	ON_BN_CLICKED(IDC_ARRAYAGGIORNA, &CGlobalArr::OnBnClickedArrayaggiorna)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlobalArr message handlers

BOOL CGlobalArr::OnInitDialog() 
{
    RECT rc;
    ePswLevel PswLevel;
    bool bEditable;

    CDialog::OnInitDialog();
    PswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    rc.left=11; rc.top=41; rc.right=260; rc.bottom=369;
    m_Grid.Create (rc, this, IDC_GRIDARRAY, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    bEditable=m_nType!=LUA_TBOOLEAN && PswLevel<ePswUser;
    m_Grid.SetEditable(bEditable);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedRowCount();
    m_Grid.SetFixedColumnCount();
    m_Grid.SetRowCount(m_nElements+1);
    m_Grid.SetColumnCount(2);
    FillCaption ();
    FillElements ();
    m_Grid.ExpandColumnsToFit (false);
    m_ctrlName.SetWindowText (GetVariableName ());
    SetWindowText (GetVariableName ());
    if (m_nElements<NMAX_GRIDELEMENTS)
        ResizeDialog (m_nElements);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CGlobalArr::OnDestroy() 
{
	CDialog::OnDestroy();
    m_Grid.DestroyWindow ();
}

void CGlobalArr::SetData (const char* szArrayName, int nType, int nElements)
{
    SetVariableName (szArrayName);
    SetKey (szArrayName);
    m_nType=nType;
    m_nElements=nElements;
}

void CGlobalArr::FillCaption ()
{
    GV_ITEM Item;
    CString str;
    int nIndex;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_MATRIXSYMBOLS), 16, 1, RGB(0,128,128));
	m_Grid.SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	Item.col = 1;
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
            str="Boolean";
            nIndex=2;
        }
    Item.iImage = nIndex;
    Item.strText=str;
    m_Grid.SetItem(&Item);
}

void CGlobalArr::FillElements ()
{
    int i;
    CString str;

    for (i=0; i<m_nElements; i++)   {
        str.Format(_T("%d"), i);
        m_Grid.SetItemText(i+1,0,str);
        if (m_nType==LUA_TBOOLEAN)
         	m_Grid.SetCellType(i+1,1, RUNTIME_CLASS(CGridCellCheck));
    }    
}

HBRUSH CGlobalArr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
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

void CGlobalArr::OnTimer() 
{
    int i, n;
    char szBuffer[NMAXSTRING];
    if (!m_bFreeze&&GetMachine()->IsCncRunning())	   {
        CCellRange cr=m_Grid.GetVisibleNonFixedCellRange ();
        CAdamoData* pData=new CAdamoData [cr.GetMaxRow()-cr.GetMinRow()+1];
        if (GetMachine()->GetArray (GetVariableName (), cr.GetMinRow()-1, cr.GetMaxRow()-1, pData)==S_OK)   {
            for (i=cr.GetMinRow()-1, n=0; i<=cr.GetMaxRow()-1; i++, n++)   {
                switch (m_nType)   {
                    case LUA_TBOOLEAN :
                        if (pData[n].nType==LUA_TNIL)   {
                            strcpy (szBuffer, LOADSTRING (IDS_ADAMONIL));
                            ((CGridCellCheck*)m_Grid.GetCell (i+1, 1))->SetCheck (FALSE);
                        }
                        else    {
                            if (pData[n].AdamoData.b)   {
                                strcpy (szBuffer, LOADSTRING (IDS_ADAMOVERO));
                                ((CGridCellCheck*)m_Grid.GetCell (i+1, 1))->SetCheck ();
                            }
                            else   {
                                strcpy (szBuffer, LOADSTRING (IDS_ADAMOFALSO));
                                ((CGridCellCheck*)m_Grid.GetCell (i+1, 1))->SetCheck (FALSE);
                            }    
                        }    
                        break;
                    case LUA_TNUMBER :
                        if (pData[n].nType==LUA_TNIL)
                            szBuffer[0]='\0';
                        else
                            sprintf (szBuffer, "%.3f", pData[n].AdamoData.fp);
                        break;
                    case LUA_TSTRING :
                        if (pData[n].nType==LUA_TNIL)
                            szBuffer[0]='\0';
                        else
                            strcpy (szBuffer, pData[n].AdamoData.str);
                        break;
                }
                if (strcmp (m_Grid.GetItemText (i+1, 1), szBuffer))
                    m_Grid.SetItemText (i+1, 1, szBuffer);
            }
        }
        delete pData;
        m_Grid.Invalidate (false);
    }
}

void CGlobalArr::OnArrayFreeze() 
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

void CGlobalArr::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    CAdamoData stData;
    bool bError=false;
    char* c;
    if (GetMachine()->IsCncRunning())	   {
        NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
        CString str=m_Grid.GetItemText (pItem->iRow, pItem->iColumn);
        str.TrimLeft(), str.TrimRight ();
        switch (m_nType)   {
            case LUA_TBOOLEAN :
                stData.nType=LUA_TBOOLEAN;
                if (((CGridCellCheck*)m_Grid.GetCell (pItem->iRow, pItem->iColumn))->GetCheck())
                    stData.AdamoData.b=1;
                else
                    stData.AdamoData.b=0;
                break;
            case LUA_TNUMBER :
                stData.nType=LUA_TNUMBER;
                stData.AdamoData.fp=strtod (str, &c);
                if (*c)
                    bError=true;
                break;
            case LUA_TSTRING :
                stData.nType=LUA_TSTRING;
                strcpy (stData.AdamoData.str, str);
                break;
        }
        if (!bError)
            GetMachine()->SetArray (GetVariableName (), pItem->iRow-1, pItem->iRow-1, &stData);
    }
}

void CGlobalArr::OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nRow=pItem->iRow;
    m_nColumn=pItem->iColumn;
}

void CGlobalArr::OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    GV_CHECKNOTIFY* pItem = (GV_CHECKNOTIFY*) pNotifyStruct;
    if (m_nType==LUA_TBOOLEAN)   {
        CAdamoData stData;
        stData.nType=LUA_TBOOLEAN;
        stData.AdamoData.b=pItem->iCheck;
        GetMachine()->SetArray (GetVariableName (), m_nRow-1, m_nRow-1, &stData);
    }
}

void CGlobalArr::ResizeDialog (int nElements)
{
    CRect rc;
	if (GETWINDOWSVERSION() == WINDOWS_7)   {
		int nLimiteInferiore = 41 + ((nElements+1) * ROW_HEIGTH_W7) + 4;
		rc.top=41;
		rc.left=11;
		rc.right=260;
		rc.bottom = nLimiteInferiore;
		m_Grid.MoveWindow (&rc);

		rc.top=nLimiteInferiore - 24 - 28;
		rc.left=273;
		rc.right=348;
		rc.bottom=nLimiteInferiore - 28;
		m_ctrlButFreeze.MoveWindow (&rc);

		rc.top=nLimiteInferiore - 24 -28 -6 -24;
		rc.left=273;
		rc.right=348;
		rc.bottom=nLimiteInferiore - 24 - 28 - 6;
		m_ctrlButUpdate.MoveWindow (&rc);

		rc.top= nLimiteInferiore - 24;
		rc.left=273;
		rc.right=348;
		rc.bottom=nLimiteInferiore;
		m_ctrlButOk.MoveWindow (&rc);

		GetWindowRect (&rc);
		rc.bottom=rc.top + nLimiteInferiore + 40;
		MoveWindow (&rc);
	}
	else   {
		int nOff=((NMAX_GRIDELEMENTS-nElements)*ROW_HEIGTH_XP);
		rc.top=41;
		rc.left=11;
		rc.right=260;
		rc.bottom=369-nOff;
		m_Grid.MoveWindow (&rc);

		rc.top=316-nOff;
		rc.left=273;
		rc.right=348;
		rc.bottom=339-nOff;
		m_ctrlButFreeze.MoveWindow (&rc);

		rc.top=346-nOff;
		rc.left=273;
		rc.right=348;
		rc.bottom=369-nOff;
		m_ctrlButOk.MoveWindow (&rc);

		GetWindowRect (&rc);
		rc.bottom=rc.bottom-nOff;
		MoveWindow (&rc);
	}
    CenterWindow ();
}

void CGlobalArr::OnClose() 
{
    ((CMainFrame *) AfxGetMainWnd ())->RemoveDialogList (GetKey ());
}

void CGlobalArr::OnOK() 
{
    CAdamoDialog::OnOK ();
    OnClose ();
}

void CGlobalArr::OnCancel() 
{
    CAdamoDialog::OnCancel ();
    OnClose ();
}

void CGlobalArr::OnBnClickedArrayaggiorna()
{
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	CCellID cid;
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	m_Grid.SetFocus ();
	//m_Grid.GetCell (1, 1)->SetState (GVIS_FOCUSED | GVIS_SELECTED);
	cid = m_Grid.GetTopleftNonFixedCell ();
	m_Grid.SetSelectedRange (cid.row, cid.col, cid.row, cid.col);
	m_Grid.SetFocusCell (cid.row, cid.col);
}
