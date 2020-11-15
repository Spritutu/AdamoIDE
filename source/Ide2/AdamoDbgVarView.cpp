// AdamoDbgVarView.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoDbgVarView.h"
#include "idemisc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* static variables */
static CString *aszCaption[2];

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgVarView

IMPLEMENT_DYNCREATE(CAdamoDbgVarView, CView)

CAdamoDbgVarView::CAdamoDbgVarView() : m_pMachine (NULL), m_nCurRow (1), m_nCurColumn (1), m_nIniRow (-1), m_nGridRow (-1)
{
}

CAdamoDbgVarView::~CAdamoDbgVarView()
{
}


BEGIN_MESSAGE_MAP(CAdamoDbgVarView, CView)
	//{{AFX_MSG_MAP(CAdamoDbgVarView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
    ON_NOTIFY(GVN_ENDLABELEDIT, ID_VIEW_DBGVARGRID, OnGridEndEdit)
	ON_NOTIFY(GVN_BUTTONCHECKED, ID_VIEW_DBGVARGRID, OnGridButtonChecked)
	ON_NOTIFY(GVN_BUTTONPRESSED, ID_VIEW_DBGVARGRID, OnGridButtonPressed)
	ON_NOTIFY(GVN_SELCHANGING, ID_VIEW_DBGVARGRID, OnGridStartSelChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgVarView drawing

void CAdamoDbgVarView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgVarView diagnostics

#ifdef _DEBUG
void CAdamoDbgVarView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoDbgVarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgVarView message handlers

int CAdamoDbgVarView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    RECT rc;
    
    memset (&rc, 0, sizeof (RECT));
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    /* carichiamo le stringhe */
    LoadStrings ();
    /* creiamo il grid */
    if (!m_Grid.Create (rc, this, ID_VIEW_DBGVARGRID, WS_CHILD | WS_VISIBLE))
        return -1;
	/* inizializziamo il grid */
    InitializeGrid ();
    /* inizializziamo la caption */
    FillColumnsCaption ();
	return 0;
}

void CAdamoDbgVarView::OnDestroy() 
{
	CView::OnDestroy();
    m_Grid.DestroyWindow ();
    for (int i=0; i<2; i++)
        _delete (aszCaption[i]);
}

void CAdamoDbgVarView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	m_Grid.MoveWindow (-2, -2, cx+3, cy+3);
	m_Grid.SetColumnWidth (0, 200);
 	m_Grid.ExpandLastColumn ();
}

void CAdamoDbgVarView::FillColumnsCaption ()
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
        Item.strText=*aszCaption[i];
        m_Grid.SetItem(&Item);
    }
}

void CAdamoDbgVarView::UpdateData ()
{
    /* ripuliamo tutto */
    Clear ();
    /* riempiamo ora tutto */
    FillVariables ();
}

void CAdamoDbgVarView::FillVariables ()
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

void CAdamoDbgVarView::InitializeGrid ()
{
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_Grid.SetEditable(true);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedColumnCount(1);
    m_Grid.SetColumnCount(2);
    m_Grid.SetFixedRowCount ();
    m_Grid.SetRowCount (1);
    m_Grid.SetSortColumn (0);
    m_Grid.SetSortAscending (TRUE);
	m_Grid.SetColumnWidth (0, 200);
	m_Grid.ExpandLastColumn ();
}

void CAdamoDbgVarView::Clear ()
{
    int n=m_Grid.GetRowCount()-1;
    for (int i=0; i<n; i++)
        m_Grid.DeleteRow (1);
}

int CAdamoDbgVarView::InsertRow (CString str)
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

void CAdamoDbgVarView::SpecilizeRow (stGlobalDescriptor* gd, int nRow)
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

void CAdamoDbgVarView::OnAdamoTimer ()
{
    COleVariant v;
    int i, nVars;
    char szBuffer[256];
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

void CAdamoDbgVarView::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
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

void CAdamoDbgVarView::OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
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

void CAdamoDbgVarView::OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nCurRow=pItem->iRow;
    m_nCurColumn=pItem->iColumn;
}

void CAdamoDbgVarView::OnGridButtonPressed(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    COleVariant v;
    GV_CHECKNOTIFY* pItem = (GV_CHECKNOTIFY*) pNotifyStruct;
    stGlobalDescriptor* gd=(stGlobalDescriptor*) m_Grid.GetItemData (m_nCurRow, m_nCurColumn);
    if (gd->nType==GLOBALARRAY||gd->nType==GLOBALMATRIX)   {
        ((CMainFrame*)AfxGetMainWnd ())->OnShowGlobalVariable (gd);
        ((CGridCellToggleButton*)m_Grid.GetCell (m_nCurRow, m_nCurColumn))->ResetToggle ();
    }
}

void CAdamoDbgVarView::LoadStrings ()
{
    for (int i=0; i<2; i++)   {
        aszCaption[i]=new CString;
        *aszCaption[i]=LOADSTRING (IDS_ADAMODBGVARVIEW_1+i);
    }
}
