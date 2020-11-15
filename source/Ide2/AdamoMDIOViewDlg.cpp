// AdamoMDIOViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoDbgVarView.h"
#include "idemisc.h"
#include "AdamoMDIOViewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDIOViewDlg dialog

/* static variables */
static char *aszCaption[] = {
    "Nome",
    "Valore"
};

CAdamoMDIOViewDlg::CAdamoMDIOViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoMDIOViewDlg::IDD, pParent), m_pMachine (NULL), m_nCurRow (1), m_nCurColumn (1),
    m_nIniRow (-1), m_nGridRow (-1)
{
	//{{AFX_DATA_INIT(CAdamoMDIOViewDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoMDIOViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoMDIOViewDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoMDIOViewDlg, CDialog)
	//{{AFX_MSG_MAP(CAdamoMDIOViewDlg)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
    ON_NOTIFY(GVN_ENDLABELEDIT, ID_VIEW_DBGVARGRID, OnGridEndEdit)
	ON_NOTIFY(GVN_BUTTONCHECKED, ID_VIEW_DBGVARGRID, OnGridButtonChecked)
	ON_NOTIFY(GVN_BUTTONPRESSED, ID_VIEW_DBGVARGRID, OnGridButtonPressed)
	ON_NOTIFY(GVN_SELCHANGING, ID_VIEW_DBGVARGRID, OnGridStartSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDIOViewDlg message handlers

BOOL CAdamoMDIOViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    /* assegnamo il livello corrente di password */
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	// TODO: Add extra initialization here
    m_list.Create(WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,0x100);
	m_list.InitializeGrid ();
    if (!m_Grid.Create (CRect(0,0,0,0), this, ID_VIEW_DBGVARGRID, WS_CHILD))
        return -1;
	/* inizializziamo il grid */
    InitializeGrid ();
    /* inizializziamo la caption */
    FillColumnsCaption ();
    /* riempiamo il grid con i dati delle variabili globali */
    UpdateData ();
    /* inizializziamo la macchina */
    m_pMachine = GETWS()->GetActiveProject()->GetMachine ();
	/* se il livello di password e' adeguato visualizziamo il grid */
	if (m_ePswLevel < ePswService)
		m_Grid.ShowWindow (SW_SHOWNORMAL);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoMDIOViewDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	//m_list.ResetContent();
    m_Grid.DestroyWindow ();
}

void CAdamoMDIOViewDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	int nOffset = m_ePswLevel < ePswService ? 360 : 0;
    if (m_list.GetSafeHwnd ())
	    m_list.MoveWindow (-1, -1, cx-nOffset, cy, SWP_SHOWWINDOW);
    if (m_Grid.GetSafeHwnd ())   {
		m_Grid.MoveWindow (cx-nOffset+1, -1, nOffset, cy+1);
		m_Grid.SetColumnWidth (0, 200);
		m_Grid.ExpandLastColumn ();
    }
}

void CAdamoMDIOViewDlg::OnAdamoTimer()
{
    COleVariant v;
    int i, nVars;
    char szBuffer[256];

    m_list.RinfrescaDispositivi ();
	if (m_pMachine&&m_pMachine->IsCncRunning()&&m_Grid.GetRowCount()>1)	   {
        CCellRange cr=m_Grid.GetVisibleNonFixedCellRange ();
        if (cr.GetMinRow()!=m_nIniRow)   {
            m_nIniRow=cr.GetMinRow(); m_nGridRow=m_nIniRow;
        }
        nVars=0;
        for (i=m_nGridRow; i<=cr.GetMaxRow() && nVars<5; i++, m_nGridRow++)   {
            stGlobalDescriptor* gd=(stGlobalDescriptor*) m_Grid.GetItemData (i, 1);
            if (gd&&gd->nType==GLOBALVARIABLE)   {
                if (m_pMachine->GetVariable (gd->strName, v)==S_OK)   {
                    if (v.vt!=VT_EMPTY)   {
                        switch (gd->nVarType)   {
                            case LUA_TBOOLEAN :
                                if (v.lVal)   {
                                    strcpy (szBuffer, "Vero");
                                    ((CGridCellCheck*)m_Grid.GetCell (i, 1))->SetCheck ();
                                }
                                else   {
                                    strcpy (szBuffer, "Falso");
                                    ((CGridCellCheck*)m_Grid.GetCell (i, 1))->SetCheck (FALSE);
                                }
                                break;
                            case LUA_TNUMBER :
                                sprintf (szBuffer, "%.3f", v.dblVal);
                                break;
                            case LUA_TSTRING :
                                strcpy (szBuffer, CString (v.bstrVal));
                                break;
                        }
                        if (strcmp (m_Grid.GetItemText (i, 1), szBuffer))   {
                            m_Grid.SetItemText (i, 1, szBuffer);
                            m_Grid.Invalidate (false);
                        }
                    }
                }
                nVars++;
            }
        }
        if (m_nGridRow>cr.GetMaxRow())
            m_nGridRow=m_nIniRow;
    }
}

void CAdamoMDIOViewDlg::OnViewSettings (stMDSettings* MDS)
{

}

void CAdamoMDIOViewDlg::GetViewSettings (stMDSettings* MDS)
{

}

void CAdamoMDIOViewDlg::FillColumnsCaption ()
{
    GV_ITEM Item;
    int i;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_DGBVARSSYMBOLS), 16, 1, RGB(0,128,128));
	m_Grid.SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
    for (i=0; i<NMAXDBGVARSCOLS; i++)   {
		Item.col=i;
        Item.iImage=i;
        Item.strText=aszCaption[i];
        m_Grid.SetItem(&Item);
    }
}

void CAdamoMDIOViewDlg::UpdateData ()
{
    /* ripuliamo tutto */
    Clear ();
    /* riempiamo ora tutto */
    FillVariables ();
}

void CAdamoMDIOViewDlg::FillVariables ()
{
    CProject *pActivePrj = GETWS()->GetActiveProject();
    if (pActivePrj)   {
        stGlobalDescriptor* gd;
        CString str;
        int nRow, i=0;
        CMapStringToPtr& pVC=pActivePrj->GetVariableContainer().GetVarsContainer ();
        for (POSITION pos=pVC.GetStartPosition (); pos; i++)   {
            pVC.GetNextAssoc (pos, str, (void*&) gd);
			if (pActivePrj->GetVariableContainer().IsLegacyVariable (str))   {
				nRow=InsertRow (str);
				SpecilizeRow (gd, nRow);
			}
        }
    }
}

void CAdamoMDIOViewDlg::InitializeGrid ()
{
    bool bEditable=false;
    //m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xF5, 0xF5, 0xF5));
    if (m_ePswLevel<ePswUser)
        bEditable=true;    
    m_Grid.SetEditable(bEditable);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedColumnCount(1);
    m_Grid.SetColumnCount(2);
    m_Grid.SetFixedRowCount ();
    m_Grid.SetRowCount (1);
    m_Grid.SetSortColumn (0);
    m_Grid.SetSortAscending (TRUE);
}

void CAdamoMDIOViewDlg::Clear ()
{
    int n=m_Grid.GetRowCount()-1;
    for (int i=0; i<n; i++)
        m_Grid.DeleteRow (1);
}

int CAdamoMDIOViewDlg::InsertRow (CString str)
{
    CString strGrid;
    int n=m_Grid.GetRowCount()-1;

    for (int i=0; i<n; i++)   {
        strGrid=m_Grid.GetItemText (i+1, 0);
        if (strcmp (str, strGrid)<0)   {
            m_Grid.InsertRow ("", i+1);
            return i+1;
        }
    }
    return m_Grid.InsertRow ("");
}

void CAdamoMDIOViewDlg::SpecilizeRow (stGlobalDescriptor* gd, int nRow)
{
    GV_ITEM Item;

    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = nRow;
    Item.col=0;
    switch (gd->nType)   {
        case GLOBALVARIABLE :
            switch (gd->nVarType)   {
                case LUA_TBOOLEAN :
                    Item.iImage=4;
                    break;
                case LUA_TNUMBER :
                    Item.iImage=3;
                    break;
                case LUA_TSTRING :
                    Item.iImage=2;
                    break;
            }
            break;
        case GLOBALARRAY :
            Item.iImage=6;
            break;
        case GLOBALMATRIX :
            Item.iImage=5;
            break;
    }
    Item.strText=gd->strName;
    m_Grid.SetItem(&Item);
    /* se boolean, caratterizziamo la variabile */
    if (gd->nType==GLOBALVARIABLE&&gd->nVarType==LUA_TBOOLEAN)   {
        m_Grid.SetItemState(nRow, 1, m_Grid.GetItemState(nRow, 1) | GVIS_READONLY);
        m_Grid.SetCellType(nRow, 1, RUNTIME_CLASS(CGridCellCheck));
    }
    if (gd->nType==GLOBALARRAY||gd->nType==GLOBALMATRIX)   {
        m_Grid.SetItemState(nRow, 1, m_Grid.GetItemState(nRow, 1) | GVIS_READONLY);
        m_Grid.SetCellType(nRow, 1, RUNTIME_CLASS(CGridCellToggleButton));
    }
    m_Grid.SetItemData (nRow, 1, (LPARAM) gd);
}

void CAdamoMDIOViewDlg::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    COleVariant v;
    char* c=NULL;
    if (m_pMachine->IsCncRunning())	   {
        NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
        stGlobalDescriptor* gd=(stGlobalDescriptor*) m_Grid.GetItemData (m_nCurRow, m_nCurColumn);
        if (gd&&gd->nType==GLOBALVARIABLE)   {
            CString str=m_Grid.GetItemText (m_nCurRow, m_nCurColumn);
            str.TrimLeft(), str.TrimRight ();
            switch (gd->nVarType)   {
                case LUA_TNUMBER :
                    v=strtod (str, &c);
                    break;
                case LUA_TSTRING :
                    v=str;
                    break;
            }
            if (!c||(c&&(*c)=='\0'))
                m_pMachine->SetVariable (gd->strName, v);
        }
    }
}

void CAdamoMDIOViewDlg::OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    COleVariant v;
    GV_CHECKNOTIFY* pItem = (GV_CHECKNOTIFY*) pNotifyStruct;
    stGlobalDescriptor* gd=(stGlobalDescriptor*) m_Grid.GetItemData (m_nCurRow, m_nCurColumn);
    if (gd&&gd->nVarType==LUA_TBOOLEAN)   {
        v.vt=VT_BOOL;
        v.boolVal=pItem->iCheck;
        m_pMachine->SetVariable (gd->strName, v);
    }
}

void CAdamoMDIOViewDlg::OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nCurRow=pItem->iRow;
    m_nCurColumn=pItem->iColumn;
	if (m_nCurColumn == 0)   {
		m_nCurColumn = 1;
		m_Grid.SetFocusCell (m_nCurRow, m_nCurColumn);
	}
}

void CAdamoMDIOViewDlg::OnGridButtonPressed(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    COleVariant v;
    GV_CHECKNOTIFY* pItem = (GV_CHECKNOTIFY*) pNotifyStruct;
    stGlobalDescriptor* gd=(stGlobalDescriptor*) m_Grid.GetItemData (m_nCurRow, m_nCurColumn);
    if (gd->nType==GLOBALARRAY||gd->nType==GLOBALMATRIX)   {
        ((CMainFrame*)AfxGetMainWnd ())->OnShowGlobalVariable (gd);
        ((CGridCellToggleButton*)m_Grid.GetCell (m_nCurRow, m_nCurColumn))->ResetToggle ();
    }
}

void CAdamoMDIOViewDlg::OnOK ()
{
}

void CAdamoMDIOViewDlg::OnCancel ()
{
}

void CAdamoMDIOViewDlg::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
	CRect rc;
	bool bVisible = false;

    m_ePswLevel=newPsw;
    if (newPsw < ePswService)
		bVisible = true;
	GetClientRect (&rc);
	if (bVisible)   {
		rc.right -= 360;
		m_Grid.ShowWindow (SW_SHOWNORMAL);
		m_list.MoveWindow (rc);
	}
	else   {
		m_Grid.ShowWindow (SW_HIDE);
		m_list.MoveWindow (rc);
	}
    m_list.OnChangePsw (oldPsw, newPsw);
	m_Grid.Invalidate ();
}
