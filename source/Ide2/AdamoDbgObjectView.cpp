/*
** AdamoDbgObjectView.cpp
*/

/* include files */
#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoDbgObjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgObjectView

IMPLEMENT_DYNCREATE(CAdamoDbgObjectView, CView)

CAdamoDbgObjectView::CAdamoDbgObjectView () : m_pMachine (NULL), m_bSymbolsLoaded (false)
{
}

CAdamoDbgObjectView::~CAdamoDbgObjectView()
{
}


BEGIN_MESSAGE_MAP(CAdamoDbgObjectView, CView)
	//{{AFX_MSG_MAP(CAdamoDbgObjectView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgObjectView drawing

void CAdamoDbgObjectView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgObjectView diagnostics

#ifdef _DEBUG
void CAdamoDbgObjectView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoDbgObjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgObjectView message handlers
int CAdamoDbgObjectView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_wndReport.Create (WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0,0,0,0),this,0x1001);	
	m_wndReport.InitializeGrid ();
	return 0;
}

void CAdamoDbgObjectView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_wndReport.GetSafeHwnd())
        m_wndReport.SetWindowPos (NULL, 0, 0, cx, cy, SWP_SHOWWINDOW);
}

void CAdamoDbgObjectView::OnAdamoTimer ()
{
	CString strSymbol ("__InMain__");
	int nGL, nType, nProg;


	if (m_pMachine && m_pMachine->ExistSymbol (strSymbol, -1, &nGL, &nType, &nProg) && !m_bSymbolsLoaded)   {
		LoadData ();
		m_bSymbolsLoaded = true;
	}
	if (m_bSymbolsLoaded)
		m_wndReport.RinfrescaInformazioni ();
}

void CAdamoDbgObjectView::LoadData ()
{
    CProject *pActivePrj = GETWS()->GetActiveProject();
    if (pActivePrj)   {
        stGlobalDescriptor* gd;
        CString str;
        CMapStringToPtr& pVC=pActivePrj->GetVariableContainer().GetVarsContainer ();
        for (POSITION pos=pVC.GetStartPosition (); pos;)   {
            pVC.GetNextAssoc (pos, str, (void*&) gd);
			if (gd->nType == GLOBALOBJECT)
				m_wndReport.AddVariable (gd->strName, 0, -1, 0);
        }
		m_wndReport.FillGrid ();
    }
}

void CAdamoDbgObjectView::ClearData ()
{
	if (m_bSymbolsLoaded)   {
		m_wndReport.Clear ();
		m_bSymbolsLoaded = false;
	}
}
