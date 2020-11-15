// AdamoDGHardware.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoDGHardware.h"
#include "AdamoDGSoftware.h"
#include "AdamoDGCards.h"
#include "AdamoDGDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGHardware

IMPLEMENT_DYNCREATE(CAdamoDGHardware, CView)

CAdamoDGHardware::CAdamoDGHardware() : m_pFlatTabCtrl (NULL), m_nCurSel (-1), m_pDGCards (NULL), m_pDGDevice (NULL), m_pDGSoftware (NULL), m_nMode (0), m_ePswLevel (ePswUser), m_pMachine (NULL), m_nNumCards (0)
{
}

CAdamoDGHardware::~CAdamoDGHardware()
{
}


BEGIN_MESSAGE_MAP(CAdamoDGHardware, CView)
	//{{AFX_MSG_MAP(CAdamoDGHardware)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(IDM_DG_REFRESH_CAN, OnDgRefresh)
	ON_COMMAND(IDM_DG_SET_OPERATIONAL, OnDgSetOperational)
	ON_COMMAND(IDM_DG_SET_PREOPERATIONAL, OnDgSetPreoperational)
	ON_UPDATE_COMMAND_UI(IDM_DG_REFRESH_CAN, OnUpdateDgRefresh)
	ON_UPDATE_COMMAND_UI(IDM_DG_SET_OPERATIONAL, OnUpdateDgSetOperational)
	ON_UPDATE_COMMAND_UI(IDM_DG_SET_PREOPERATIONAL, OnUpdateDgSetPreoperational)
	ON_COMMAND(IDM_DG_SET_INITIALIZE, OnDgSetInitialize)
	ON_UPDATE_COMMAND_UI(IDM_DG_SET_INITIALIZE, OnUpdateDgSetInitialize)
	ON_UPDATE_COMMAND_UI(IDM_DG_SET_START, OnUpdateDgSetStart)
	ON_COMMAND(IDM_DG_SET_START, OnDgSetStart)
	ON_COMMAND(IDM_DG_SET_STOP, OnDgSetStop)
	ON_UPDATE_COMMAND_UI(IDM_DG_SET_STOP, OnUpdateDgSetStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGHardware drawing

void CAdamoDGHardware::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

}

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGHardware diagnostics

#ifdef _DEBUG
void CAdamoDGHardware::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoDGHardware::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGHardware message handlers

/*
** OnCreate : creiamo il tab control flat che ospitera'diagnostica hardware, dispositivi e software.
*/
int CAdamoDGHardware::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    RECT rc;
    int nB=0;
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    memset (&rc, 0, sizeof (RECT));
    SetMachine ();
    m_pFlatTabCtrl = new CXTPTabControl();
	if (m_pFlatTabCtrl->Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
		rc, this, IDC_FLAT_TABCTRL_DG_HARDWARE))  {
        m_pFlatTabCtrl->GetPaintManager ()->SetAppearance(xtpTabAppearanceExcel);
        m_pFlatTabCtrl->GetPaintManager ()->SetColor(xtpTabColorVisualStudio2012);
        m_pFlatTabCtrl->GetPaintManager ()->SetPosition (xtpTabPositionBottom);
		m_pFlatTabCtrl->GetPaintManager ()->DisableLunaColors (TRUE);
        CreateViews ();
	}
    else
        nB=-1;
	m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	return nB;
}

/*
** OnDestroy : distruggiamo le finestre e il tab.
*/
void CAdamoDGHardware::OnDestroy () 
{
	CView::OnDestroy();
    SAFE_DELETE (m_pFlatTabCtrl);
    SAFE_DELETE (m_pDGCards);
    SAFE_DELETE (m_pDGDevice);
    SAFE_DELETE (m_pDGSoftware);
}

/*
** CreateViews :
*/
void CAdamoDGHardware::CreateViews ()
{
    m_pMachine->GetNumCards (&m_nNumCards);
    if (m_nNumCards > 0)   {
        m_pDGCards=new    CAdamoDGCards;
        m_pDGDevice=new   CAdamoDGDevice;
	    m_pDGCards->Create(WS_CHILD,CRect(0,0,0,0),m_pFlatTabCtrl,0x600);
	    m_pDGDevice->Create(WS_CHILD,CRect(0,0,0,0),m_pFlatTabCtrl,0x601);
    }
    m_pDGSoftware=new CAdamoDGSoftware;
    m_pDGSoftware->Create (IDD_DLG_DG_SOFTWARE, m_pFlatTabCtrl);

    m_pFlatTabCtrl->InsertItem (0, LOADSTRING (IDS_ADAMODGHARDWARE_1), m_pDGSoftware->GetSafeHwnd ());
    if (m_nNumCards > 0)  {
        m_pFlatTabCtrl->InsertItem (0, LOADSTRING (IDS_ADAMODGHARDWARE_2), m_pDGDevice->GetSafeHwnd ());
        m_pFlatTabCtrl->SetSelectedItem (m_pFlatTabCtrl->InsertItem (0, LOADSTRING (IDS_ADAMODGHARDWARE_3), m_pDGCards->GetSafeHwnd ()));
        m_pDGCards->InitializeGrid ();
        m_pDGDevice->InitializeGrid ();
    }
}

/*
** OnSize :
*/
void CAdamoDGHardware::OnSize (UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	m_pFlatTabCtrl->MoveWindow (0, 0, cx, cy);
}

/*
** OnAdamoTimer
*/
void CAdamoDGHardware::OnAdamoTimer () 
{
    if (m_pDGCards&&m_pDGDevice)   {
        m_pDGCards->RinfrescaInformazioni ();
        m_pDGDevice->RinfrescaInformazioni ();
    }
    if (m_pDGSoftware)
        m_pDGSoftware->RinfrescaInformazioni ();
}

/*
** OnDgRefresh :
*/
void CAdamoDGHardware::OnDgRefresh() 
{
    /* puliamo per prima cosa tutto il tree */
    if (m_pDGDevice)
        m_pDGDevice->OnRefresh ();
}

/*
** OnDgSetOperational :
*/
void CAdamoDGHardware::OnDgSetOperational() 
{
    /* Setta un nodo a operational */
    if (m_pDGDevice)
        m_pDGDevice->OnSetOperational (m_nMode);    
}

/*
** OnDgSetPreoperational :
*/
void CAdamoDGHardware::OnDgSetPreoperational() 
{
    /* Setta un nodo a preoperational */
    if (m_pDGDevice)
        m_pDGDevice->OnSetPreOperational (m_nMode);
}

/*
** OnNotify :
*/
BOOL CAdamoDGHardware::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if ( IDC_FLAT_TABCTRL_DG_HARDWARE == (UINT)wParam)   {
		NMHDR* pNMHDR = (NMHDR*)lParam;
		switch(pNMHDR->code)   {
		    case TCN_SELCHANGING:
			    break;
		    case TCN_SELCHANGE:
                m_nCurSel=m_pFlatTabCtrl->GetCurSel();
 			    break;
		}
    }
    return CView::OnNotify(wParam, lParam, pResult);
}

/*
** OnUpdateDgRefresh :
*/
void CAdamoDGHardware::OnUpdateDgRefresh(CCmdUI* pCmdUI) 
{
	if (m_nCurSel==1)
	    if (m_ePswLevel < ePswService)
			pCmdUI->Enable ();
		else
			pCmdUI->Enable (false);
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateDgSetOperational :
*/
void CAdamoDGHardware::OnUpdateDgSetOperational(CCmdUI* pCmdUI) 
{
	if (m_nCurSel==1)
	    if (m_ePswLevel < ePswService)
			pCmdUI->Enable ();
		else
			pCmdUI->Enable (false);
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateDgSetPreoperational :
*/
void CAdamoDGHardware::OnUpdateDgSetPreoperational(CCmdUI* pCmdUI) 
{
	if (m_nCurSel==1)
	    if (m_ePswLevel < ePswService)
			pCmdUI->Enable ();
		else
			pCmdUI->Enable (false);
	else
		pCmdUI->Enable (false);
}

void CAdamoDGHardware::OnDgSetInitialize() 
{
	// TODO: Add your command handler code here
    /* Setta un nodo a operational */
    if (m_pDGDevice)
        m_pDGDevice->OnSetInitialize ();
}

void CAdamoDGHardware::OnDgSetStart() 
{
	// TODO: Add your command handler code here
    if (m_pDGDevice)
        m_pDGDevice->OnSetStart ();	
}

void CAdamoDGHardware::OnDgSetStop() 
{
	// TODO: Add your command handler code here
    if (m_pDGDevice)
        m_pDGDevice->OnSetStop ();	
}

void CAdamoDGHardware::OnUpdateDgSetInitialize(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_nCurSel==1)
	    if (m_ePswLevel < ePswService)
			pCmdUI->Enable ();
		else
			pCmdUI->Enable (false);
	else
		pCmdUI->Enable (false);
}

void CAdamoDGHardware::OnUpdateDgSetStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_nCurSel==1)
	    if (m_ePswLevel < ePswService)
			pCmdUI->Enable ();
		else
			pCmdUI->Enable (false);
	else
		pCmdUI->Enable (false);
}

void CAdamoDGHardware::OnUpdateDgSetStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (m_nCurSel==1)
	    if (m_ePswLevel < ePswService)
			pCmdUI->Enable ();
		else
			pCmdUI->Enable (false);
	else
		pCmdUI->Enable (false);
}

/*
** OnChangePsw :
*/
void CAdamoDGHardware::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
	m_ePswLevel=newPsw;
    if (m_nNumCards > 0)   {
        switch (GetCurTabSel ())   {
	        case 0 :
		        m_pDGCards->OnChangePsw (oldPsw, newPsw);
		        break;
	        case 1 :
		        m_pDGDevice->OnChangePsw (oldPsw, newPsw);
		        break;
	        case 2 :
		        m_pDGSoftware->OnChangePsw (oldPsw, newPsw);
		        break;
        }
    }
    else
        m_pDGSoftware->OnChangePsw (oldPsw, newPsw);
}

/*
** SetMachine :
*/
void CAdamoDGHardware::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}
