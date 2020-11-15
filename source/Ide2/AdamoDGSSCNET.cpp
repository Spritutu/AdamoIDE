// AdamoMDSSCNET.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "workspace.h"
#include "mainframe.h"
#include "adamodgsscnet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSSCNET

CAdamoDGSSCNET::CAdamoDGSSCNET() : m_pSSCNETGrid (NULL), m_bAllNetwork (true), m_pMachine (NULL), m_nCard (0), m_ePswLevel (ePswUser)
{
}

CAdamoDGSSCNET::~CAdamoDGSSCNET()
{
}


BEGIN_MESSAGE_MAP(CAdamoDGSSCNET, CView)
	//{{AFX_MSG_MAP(CAdamoDGSSCNET)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSSCNET drawing

void CAdamoDGSSCNET::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSSCNET diagnostics

#ifdef _DEBUG
void CAdamoDGSSCNET::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoDGSSCNET::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSSCNET message handlers

int CAdamoDGSSCNET::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    m_pSSCNETGrid=new CAdamoSSCNETGrid;
    m_pSSCNETGrid->SetCard (m_nCard);
	m_pSSCNETGrid->Create(WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,0x600);
    m_pSSCNETGrid->InitializeGrid ();
    SetMachine ();
	m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	return 0;
}

void CAdamoDGSSCNET::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    SAFE_DELETE (m_pSSCNETGrid);
}

/*
** OnSize :
*/
void CAdamoDGSSCNET::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_pSSCNETGrid)
        m_pSSCNETGrid->MoveWindow (0, 0, cx, cy);
}

/*
** OnSetAllSSCNETStations :
*/
void CAdamoDGSSCNET::OnSetAllSSCNETStations(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPCONTROL* tagNMCONTROL = (NMXTPCONTROL*)pNMHDR;
		
	CXTPControlComboBox* pControl = (CXTPControlComboBox*)tagNMCONTROL->pControl;
	if (pControl->GetType() == xtpControlComboBox)
		m_bAllNetwork=!pControl->GetCurSel();
}

/*
** OnUpdateSetAllSSCNETStations :
*/
void CAdamoDGSSCNET::OnUpdateSetAllSSCNETStations(CCmdUI* pCmdUI)
{
    if (m_ePswLevel < ePswService)
	pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CAdamoDGSSCNET::OnBtnSSCNETReadyOn() 
{
	// TODO: Add your command handler code here
    int nStation=-1;
    if (m_pMachine)   {
        if (!m_bAllNetwork)   {
            nStation=GetCurrentStation ();
            if (nStation==-1)
                return;
        }
        m_pMachine->SSCNETReadyOn (m_nCard, nStation, true);
    }
}

void CAdamoDGSSCNET::OnUpdateBtnSSCNETReadyOn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel < ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

void CAdamoDGSSCNET::OnBtnSSCNETReadyOff() 
{
	// TODO: Add your command handler code here
    int nStation=-1;
    if (m_pMachine)   {
        if (!m_bAllNetwork)   {
            nStation=GetCurrentStation ();    
            if (nStation==-1)
                return;
        }
        m_pMachine->SSCNETReadyOn (m_nCard, nStation, false);
    }
}

void CAdamoDGSSCNET::OnUpdateBtnSSCNETReadyOff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel < ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

void CAdamoDGSSCNET::OnBtnSSCNETServoOn() 
{
	// TODO: Add your command handler code here
    int nStation=-1;
    if (m_pMachine)   {
        if (((CMainFrame *)AfxGetMainWnd ())->CheckParameters ())   {
            if (!m_bAllNetwork)   {
                nStation=GetCurrentStation ();
                if (nStation==-1)
                    return;
            }
            m_pMachine->SSCNETServoOn (m_nCard, nStation, true);
        }
    }
}

void CAdamoDGSSCNET::OnBtnSSCNETServoOff() 
{
	// TODO: Add your command handler code here
    int nStation=-1;
    if (m_pMachine)   {
        if (!m_bAllNetwork)   {
            nStation=GetCurrentStation ();    
            if (nStation==-1)
                return;
        }
        m_pMachine->SSCNETServoOn (m_nCard, nStation, false);
    }
}

void CAdamoDGSSCNET::OnUpdateBtnSSCNETServoOff(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel < ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

void CAdamoDGSSCNET::OnUpdateBtnSSCNETServoOn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel < ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

void CAdamoDGSSCNET::OnBtnResetAlarm() 
{
	// TODO: Add your command handler code here
    int nStation=-1;
    if (m_pMachine)   {
        if (!m_bAllNetwork)   {
            nStation=GetCurrentStation ();    
            if (nStation==-1)
                return;
        }
        m_pMachine->SSCNETResetAlarm (m_nCard, nStation);
    }
}

void CAdamoDGSSCNET::OnUpdateBtnResetAlarm(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel < ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** GetCurrentStation :
*/
int CAdamoDGSSCNET::GetCurrentStation ()
{
    return m_pSSCNETGrid->GetCurrentStation ();
}

/*
** SetMachine :
*/
void CAdamoDGSSCNET::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}

/*
** OnAdamoTimer :
*/
void CAdamoDGSSCNET::OnAdamoTimer ()
{
    if (m_pSSCNETGrid)
        m_pSSCNETGrid->RinfrescaInformazioni ();
}

/*
** OnAdamoTimer :
*/
void CAdamoDGSSCNET::InizializzaInformazioni ()
{
    if (m_pSSCNETGrid)
        m_pSSCNETGrid->InizializzaInformazioni ();
}

/*
** OnBtnSetpointAbs :
*/
void CAdamoDGSSCNET::OnBtnSetpointAbs() 
{
    /* chiama la procedura per eseguire il setpoint in automatico */
    int nStation=-1;
    if (m_pMachine)   {
        nStation=GetCurrentStation ();    
        if (nStation==-1)
            return;
        if (AfxMessageBox (LOADSTRING(IDS_ADAMODGSSCNET_6), MB_YESNO | MB_ICONQUESTION)==IDYES)
            m_pMachine->SetAbsPosition (m_nCard, nStation);
    }
}

/*
** OnBtnInitSSCNET :
*/
void CAdamoDGSSCNET::OnBtnInitSSCNET () 
{
    /* chiama la procedura per eseguire il setpoint in automatico */
    int nStation=-1;
    if (m_pMachine)
        if (AfxMessageBox (LOADSTRING(IDS_ADAMODGSSCNET_7), MB_YESNO | MB_ICONQUESTION)==IDYES)
            m_pMachine->InitSSCNET (m_nCard);
}

/*
** OnBtnDownloadAbsEncoder :
*/
void CAdamoDGSSCNET::OnBtnDownloadAbsEncoder ()
{
    /* chiama la procedura per eseguire il setpoint in automatico */
    int nStation=-1;
    WORD  wMultiRevCounter;
    DWORD dwOneRevPos;

    if (m_pMachine)   {
        nStation=GetCurrentStation ();
        if (nStation==-1)
            return;
        if (AfxMessageBox (LOADSTRING(IDS_ADAMODGSSCNET_8), MB_YESNO | MB_ICONQUESTION)==IDYES)   {
            if (!ReadAbsPosition (nStation, &wMultiRevCounter, &dwOneRevPos))
                m_pMachine->WriteAbsEncoder (m_nCard, nStation, wMultiRevCounter, dwOneRevPos);
        }
    }
}

/*
** OnBtnUploadAbsEncoder :
*/
void CAdamoDGSSCNET::OnBtnUploadAbsEncoder () 
{
    int nStation=-1;
    WORD  wMultiRevCounter;
    DWORD dwOneRevPos;

    if (m_pMachine)   {
        nStation=GetCurrentStation ();
        if (nStation==-1)
            return;
        if (m_pMachine->ReadAbsEncoder (m_nCard, nStation, &wMultiRevCounter, &dwOneRevPos) == 0)
			WriteAbsPosition (nStation, wMultiRevCounter, dwOneRevPos);
    }
}

/*
** ReadAbsPosition :
*/
int CAdamoDGSSCNET::ReadAbsPosition (int nStation, WORD *pwMultiRevCounter, DWORD *pdwOneRevPos)
{
    int nB=-1;
    CProject* pPrj=GETWS()->GetActiveProject();
    if (pPrj)   {
        CAdamoHWConfigFile *hwcf=new CAdamoHWConfigFile;
        if (hwcf)   {
            if (hwcf->Open (pPrj->GetHWConfigPathName(), TRUE))   {
                hwcf->ReadData ();
                hwcf->GetAbsPosition (m_nCard, nStation+1, *pwMultiRevCounter, *pdwOneRevPos);
                hwcf->CloseFile();
                nB=0;
            }
            delete hwcf;
        }
    }
    return nB;
}

/*
** WriteAbsPosition :
*/
int CAdamoDGSSCNET::WriteAbsPosition (int nStation, WORD wMultiRevCounter, DWORD dwOneRevPos)
{
    int nB=-1;
    CProject* pPrj=GETWS()->GetActiveProject();
    int nSize=sizeof (stHWAdamoConf);
    if (pPrj)   {
        CAdamoHWConfigFile *hwcf=new CAdamoHWConfigFile;
        if (hwcf)   {
            if (hwcf->Open (pPrj->GetHWConfigPathName(), TRUE))   {
                hwcf->ReadData ();
                hwcf->SetAbsPosition (m_nCard, nStation+1, wMultiRevCounter, dwOneRevPos);
                hwcf->SeekToBegin ();
                hwcf->WriteData ();
                hwcf->CloseFile();
            }
            delete hwcf;
        }
    }
    return nB;
}

/*
** OnChangePsw :
*/
void CAdamoDGSSCNET::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
	m_ePswLevel=newPsw;
}

/*
** OnUpdateBtnSetpointAbs 
*/
void CAdamoDGSSCNET::OnUpdateBtnSetpointAbs(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel < ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnInitSSCNET 
*/
void CAdamoDGSSCNET::OnUpdateBtnInitSSCNET(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel < ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnDownloadAbsEncoder 
*/
void CAdamoDGSSCNET::OnUpdateBtnDownloadAbsEncoder(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel < ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}

/*
** OnUpdateBtnUploadAbsEncoder 
*/
void CAdamoDGSSCNET::OnUpdateBtnUploadAbsEncoder(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_ePswLevel < ePswService)   {
		pCmdUI->Enable ();
	}
	else
		pCmdUI->Enable (false);
}
