// AdamoDGSSCNETView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ide2.h"
#include "workspace.h"
#include "mainframe.h"
#include "Adamodgsscnet.h"
#include "adamodgsscnetview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CAdamoDGSSCNETView, CView)

BEGIN_MESSAGE_MAP(CAdamoDGSSCNETView, CView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(IDC_BTN_SSCNETREADYON, OnBtnSSCNETReadyOn)
	ON_UPDATE_COMMAND_UI(IDC_BTN_SSCNETREADYON, OnUpdateBtnSSCNETReadyOn)
	ON_COMMAND(IDC_BTN_SSCNETREADYOFF, OnBtnSSCNETReadyOff)
	ON_UPDATE_COMMAND_UI(IDC_BTN_SSCNETREADYOFF, OnUpdateBtnSSCNETReadyOff)
	ON_COMMAND(IDC_BTN_SSCNETSERVOON, OnBtnSSCNETServoOn)
	ON_COMMAND(IDC_BTN_SSCNETSERVOOFF, OnBtnSSCNETServoOff)
	ON_UPDATE_COMMAND_UI(IDC_BTN_SSCNETSERVOOFF, OnUpdateBtnSSCNETServoOff)
	ON_UPDATE_COMMAND_UI(IDC_BTN_SSCNETSERVOON, OnUpdateBtnSSCNETServoOn)
	ON_COMMAND(IDC_BTN_SETPOINTABS, OnBtnSetpointAbs)
	ON_COMMAND(IDC_BTN_INITSSCNET, OnBtnInitSSCNET)
	ON_COMMAND(IDC_BTN_DOWNLOADABSENCODER, OnBtnDownloadAbsEncoder)
	ON_COMMAND(IDC_BTN_UPLOADABSENCODER, OnBtnUploadAbsEncoder)
	ON_UPDATE_COMMAND_UI(IDC_BTN_SETPOINTABS, OnUpdateBtnSetpointAbs)
	ON_UPDATE_COMMAND_UI(IDC_BTN_INITSSCNET, OnUpdateBtnInitSSCNET)
	ON_UPDATE_COMMAND_UI(IDC_BTN_DOWNLOADABSENCODER, OnUpdateBtnDownloadAbsEncoder)
	ON_UPDATE_COMMAND_UI(IDC_BTN_UPLOADABSENCODER, OnUpdateBtnUploadAbsEncoder)
	ON_COMMAND(IDC_BTN_RESETALARM, OnBtnResetAlarm)
	ON_UPDATE_COMMAND_UI(IDC_BTN_RESETALARM, OnUpdateBtnResetAlarm)
	//}}AFX_MSG_MAP
    ON_XTP_EXECUTE(IDC_BTN_ALLSTATIONS, OnSetAllSSCNETStations)
	ON_UPDATE_COMMAND_UI(IDC_BTN_ALLSTATIONS, OnUpdateSetAllSSCNETStations)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSSCNET
CAdamoDGSSCNETView::CAdamoDGSSCNETView() : m_pFlatTabCtrl (NULL), m_nCurSel (0), m_nNumCards (0),
    m_pMachine (NULL), m_ePswLevel (ePswUser)
{
    m_pDGSSCNET[0] = m_pDGSSCNET[1] = m_pDGSSCNET[2] = m_pDGSSCNET[3] = NULL;
}

CAdamoDGSSCNETView::~CAdamoDGSSCNETView ()
{
}

#ifdef _DEBUG
void CAdamoDGSSCNETView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoDGSSCNETView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/*
** OnDraw :
*/
void CAdamoDGSSCNETView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here

}

/*
** OnCreate : creiamo il tab control flat che ospitera'diagnostica hardware, dispositivi e software.
*/
int CAdamoDGSSCNETView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    RECT rc;
    int nB=0;
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    memset (&rc, 0, sizeof (RECT));
	m_pFlatTabCtrl = new CXTPTabControl();
    SetMachine ();
	if (m_pFlatTabCtrl->Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,
		rc, this, IDC_FLAT_TABCTRL_DG_SSCNET))  {
        m_pFlatTabCtrl->GetPaintManager ()->SetAppearance(xtpTabAppearanceExcel);
        m_pFlatTabCtrl->GetPaintManager ()->SetColor(xtpTabColorOffice2003);
        m_pFlatTabCtrl->GetPaintManager ()->SetPosition (xtpTabPositionBottom);
        m_pFlatTabCtrl->GetPaintManager ()->DisableLunaColors (FALSE);
        CreateViews ();
	}
    else
        nB=-1;
	return nB;
}

/*
** OnAdamoTimer :
*/
void CAdamoDGSSCNETView::OnAdamoTimer ()
{
    for (int i = 0; i<m_nNumCards; i++)   {
        m_pDGSSCNET[i]->OnAdamoTimer ();
    }
}

/*
** CreateViews :
*/
void CAdamoDGSSCNETView::CreateViews ()
{
    CXTPTabManagerItem* pItem = NULL;
    CString str;

    m_pMachine->GetNumCards (&m_nNumCards);
    for (int i = 0; i<m_nNumCards; i++)   {
        m_pDGSSCNET[i] = new CAdamoDGSSCNET;
        m_pDGSSCNET[i]->SetCard (i);
    	m_pDGSSCNET[i]->Create(NULL, NULL, WS_CHILD,CRect(0,0,0,0),m_pFlatTabCtrl,0x600 + 1);
        str.Format ("%s %d", LOADSTRING (IDS_ADAMODGHARDWARE_4), i);
        if (i == 0)
            pItem = m_pFlatTabCtrl->InsertItem (i, str, m_pDGSSCNET[i]->GetSafeHwnd ());
        else
            m_pFlatTabCtrl->InsertItem (i, str, m_pDGSSCNET[i]->GetSafeHwnd ());
    }
    if (pItem)
        m_pFlatTabCtrl->SetSelectedItem (pItem);
}

/*
** OnSize :
*/
void CAdamoDGSSCNETView::OnSize (UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	m_pFlatTabCtrl->MoveWindow (0, 0, cx, cy);
}

/*
** OnDestroy : distruggiamo le finestre e il tab.
*/
void CAdamoDGSSCNETView::OnDestroy () 
{
	CView::OnDestroy();
    for (int i = 0; i<m_nNumCards; i++)
        SAFE_DELETE (m_pDGSSCNET[i]);
    SAFE_DELETE (m_pFlatTabCtrl);
}

/*
** OnNotify :
*/
BOOL CAdamoDGSSCNETView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if ( IDC_FLAT_TABCTRL_DG_SSCNET == (UINT)wParam)   {
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
** SetMachine :
*/
void CAdamoDGSSCNETView::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}

/*
** InizializzaInformazioni :
*/
void CAdamoDGSSCNETView::InizializzaInformazioni ()
{
    if (m_pDGSSCNET[m_nCurSel])
        m_pDGSSCNET[m_nCurSel]->InizializzaInformazioni ();
}

/*
** OnChangePsw :
*/
void CAdamoDGSSCNETView::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
	m_ePswLevel=newPsw;
}

void CAdamoDGSSCNETView::OnBtnSSCNETReadyOn() 
{
    m_pDGSSCNET[m_nCurSel]->OnBtnSSCNETReadyOn ();
}

void CAdamoDGSSCNETView::OnUpdateBtnSSCNETReadyOn(CCmdUI* pCmdUI) 
{
    m_pDGSSCNET[m_nCurSel]->OnUpdateBtnSSCNETReadyOn (pCmdUI);
}

void CAdamoDGSSCNETView::OnBtnSSCNETReadyOff() 
{
    m_pDGSSCNET[m_nCurSel]->OnBtnSSCNETReadyOff ();
}

void CAdamoDGSSCNETView::OnUpdateBtnSSCNETReadyOff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    m_pDGSSCNET[m_nCurSel]->OnUpdateBtnSSCNETReadyOff (pCmdUI);
}

void CAdamoDGSSCNETView::OnBtnSSCNETServoOn() 
{
    m_pDGSSCNET[m_nCurSel]->OnBtnSSCNETServoOn ();
}

void CAdamoDGSSCNETView::OnBtnSSCNETServoOff() 
{
    m_pDGSSCNET[m_nCurSel]->OnBtnSSCNETServoOff ();
}

void CAdamoDGSSCNETView::OnUpdateBtnSSCNETServoOff(CCmdUI* pCmdUI) 
{
    m_pDGSSCNET[m_nCurSel]->OnUpdateBtnSSCNETServoOff (pCmdUI);
}

void CAdamoDGSSCNETView::OnUpdateBtnSSCNETServoOn(CCmdUI* pCmdUI) 
{
    m_pDGSSCNET[m_nCurSel]->OnUpdateBtnSSCNETServoOn (pCmdUI);
}

void CAdamoDGSSCNETView::OnBtnResetAlarm()
{
    m_pDGSSCNET[m_nCurSel]->OnBtnResetAlarm ();
}

void CAdamoDGSSCNETView::OnUpdateBtnSetpointAbs(CCmdUI* pCmdUI)
{
    m_pDGSSCNET[m_nCurSel]->OnUpdateBtnSetpointAbs (pCmdUI);
}

void CAdamoDGSSCNETView::OnUpdateBtnInitSSCNET(CCmdUI* pCmdUI)
{
    m_pDGSSCNET[m_nCurSel]->OnUpdateBtnInitSSCNET (pCmdUI);
}

void CAdamoDGSSCNETView::OnUpdateBtnDownloadAbsEncoder(CCmdUI* pCmdUI)
{
    m_pDGSSCNET[m_nCurSel]->OnUpdateBtnDownloadAbsEncoder (pCmdUI);
}

void CAdamoDGSSCNETView::OnUpdateBtnUploadAbsEncoder(CCmdUI* pCmdUI)
{
    m_pDGSSCNET[m_nCurSel]->OnUpdateBtnUploadAbsEncoder (pCmdUI);
}

void CAdamoDGSSCNETView::OnUpdateBtnResetAlarm(CCmdUI* pCmdUI)
{
    m_pDGSSCNET[m_nCurSel]->OnUpdateBtnResetAlarm (pCmdUI);
}

void CAdamoDGSSCNETView::OnBtnSetpointAbs()
{
    m_pDGSSCNET[m_nCurSel]->OnBtnSetpointAbs ();
}

void CAdamoDGSSCNETView::OnBtnInitSSCNET()
{
    m_pDGSSCNET[m_nCurSel]->OnBtnInitSSCNET ();
}

void CAdamoDGSSCNETView::OnBtnDownloadAbsEncoder()
{
    m_pDGSSCNET[m_nCurSel]->OnBtnDownloadAbsEncoder ();
}

void CAdamoDGSSCNETView::OnBtnUploadAbsEncoder()
{
    m_pDGSSCNET[m_nCurSel]->OnBtnUploadAbsEncoder ();
}

void CAdamoDGSSCNETView::OnSetAllSSCNETStations(NMHDR* pNMHDR, LRESULT* pResult)
{
    m_pDGSSCNET[m_nCurSel]->OnSetAllSSCNETStations (pNMHDR, pResult);
}

void CAdamoDGSSCNETView::OnUpdateSetAllSSCNETStations(CCmdUI* pCmdUI)
{
    m_pDGSSCNET[m_nCurSel]->OnUpdateSetAllSSCNETStations (pCmdUI);
}
