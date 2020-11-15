// AdamoConfHWView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "adamoconfhwview.h"
#include "DlgHWYoupsilon.h"
#include "dlghwcanopennet.h"
#include "dlghwcanopennodes.h"
#include "dlghwsscnetnet.h"
#include "dlghwsscnetstation.h"
#include "DlgHWEthercatNet.h"
#include "DlgHWModule.h"
#include "AdamoEthercatNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfHWView

IMPLEMENT_DYNCREATE(CAdamoConfHWView, CView)

CAdamoConfHWView::CAdamoConfHWView() : m_pHWDlg (NULL)
{
}

CAdamoConfHWView::~CAdamoConfHWView()
{
}


BEGIN_MESSAGE_MAP(CAdamoConfHWView, CView)
	//{{AFX_MSG_MAP(CAdamoConfHWView)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
	ON_WM_DESTROY()
	ON_COMMAND(IDC_BTN_DOWNLOAD, OnDownloadSSCNETParams)
	ON_UPDATE_COMMAND_UI(IDC_BTN_DOWNLOAD, OnUpdateDownloadSSCNETParams)
	ON_COMMAND(IDC_BTN_UPLOAD, OnUploadSSCNETParams)
	ON_UPDATE_COMMAND_UI(IDC_BTN_UPLOAD, OnUpdateUploadSSCNETParams)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfHWView drawing

void CAdamoConfHWView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfHWView diagnostics

#ifdef _DEBUG
void CAdamoConfHWView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoConfHWView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfHWView message handlers
void CAdamoConfHWView::CreateDlgHW (int nCardType, int nCard, int i, int n)
{
	CProject *pPrj;
    CRect rc;
	int nModule = -1;

    DestroyDlgHW ();
	pPrj = GetAvailablePrj (nCardType, nCard);
	if (pPrj)   {
		if (nCardType == NET_EPSILON)   {
			nModule = nCard; nCard = 0;
		}
		GetClientRect (&rc);
		switch (n)   {
			case 0 :
				m_pHWDlg = new CDlgHWYoupsilon;
				((CDlgHWYoupsilon *)m_pHWDlg)->SetKinemaAddress (nCard);
					((CDlgHWYoupsilon *)m_pHWDlg)->SetProject (pPrj);
				m_pHWDlg->Create (IDD_DLG_HW_YOUPSILON, this);
				m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
				break;
			case 1 :
				m_pHWDlg = new CDlgHWCanOpenNet;
				((CDlgHWCanOpenNet *)m_pHWDlg)->SetKinemaAddress (nCard);
					((CDlgHWCanOpenNet *)m_pHWDlg)->SetProject (pPrj);
				m_pHWDlg->Create (IDD_DLG_HW_CANOPENNET, this);
				m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
				break;
			case 2 :
				m_pHWDlg = new CDlgHWCanOpenNodes;
				((CDlgHWCanOpenNodes *)m_pHWDlg)->SetKinemaAddress (nCard);
				((CDlgHWCanOpenNodes *)m_pHWDlg)->SetModuleNumber (nModule);
				((CDlgHWCanOpenNodes *)m_pHWDlg)->SetCanOpenNode (i);
					((CDlgHWCanOpenNodes *)m_pHWDlg)->SetProject (pPrj);
				m_pHWDlg->Create (IDD_DLG_HW_CANOPENNODES, this);
				m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
				break;
			case 3 :
				m_pHWDlg = new CDlgHWSSCNETNet;
				((CDlgHWSSCNETNet *)m_pHWDlg)->SetKinemaAddress (nCard);
				((CDlgHWSSCNETNet *)m_pHWDlg)->SetModuleNumber (nModule);
					((CDlgHWSSCNETNet *)m_pHWDlg)->SetProject (pPrj);
				m_pHWDlg->Create (IDD_DLG_HW_SSCNETNET, this);
				m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
				break;
			case 4 :
				m_pHWDlg = new CDlgHWSSCNETStation;
				((CDlgHWSSCNETStation *)m_pHWDlg)->SetKinemaAddress (nCard);
				((CDlgHWSSCNETStation *)m_pHWDlg)->SetModuleNumber (nModule);
				((CDlgHWSSCNETStation *)m_pHWDlg)->SetSSCNETStation (i);
					((CDlgHWSSCNETStation *)m_pHWDlg)->SetProject (pPrj);
				m_pHWDlg->Create (IDD_DLG_HW_SSCNETSTATIONS, this);
				m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
				break;
			case 5 :
				m_pHWDlg = new CDlgHWEthercatNet;
					((CDlgHWEthercatNet *)m_pHWDlg)->SetProject (pPrj);
				m_pHWDlg->Create (IDD_DLG_ETHERCAT_NETWORK, this);
				m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
				break;
			case 6 :
				if (IsSlaveConfigured (pPrj, i)!=eEth)   {
					m_pHWDlg = new CAdamoEthercatNode;
					((CAdamoEthercatNode *)m_pHWDlg)->SetEthercatNode (i);
						((CAdamoEthercatNode *)m_pHWDlg)->SetProject (pPrj);
					m_pHWDlg->Create (IDD_DLG_ETHERCAT_NODE, this);
					m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
				}
				break;
				case 7 :
					m_pHWDlg = new CDlgHWModule;
					((CDlgHWModule *)m_pHWDlg)->SetKinemaAddress (nCard);
					m_pHWDlg->Create (IDD_DLG_HW_MODULE, this);
					m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
					break;
		}
    }
}

void CAdamoConfHWView::DestroyDlgHW ()
{
    if (m_pHWDlg)
        m_pHWDlg->DestroyWindow ();
    _delete (m_pHWDlg);
}

void CAdamoConfHWView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_pHWDlg)
        m_pHWDlg->MoveWindow (0, 0, cx, cy);
}

void CAdamoConfHWView::OnEditClear() 
{
    if (m_pHWDlg)
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWYoupsilon)))
            ((CDlgHWYoupsilon *)m_pHWDlg)->RemoveCurrentConnection ();
        else
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETStation)))
            ((CDlgHWSSCNETStation *)m_pHWDlg)->RemoveCurrentConnection ();
        else
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWCanOpenNodes)))
            ((CDlgHWCanOpenNodes *)m_pHWDlg)->RemoveCurrentConnection ();
        else
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CAdamoEthercatNode)))
            ((CAdamoEthercatNode *)m_pHWDlg)->RemoveCurrentConnection ();
		else
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWEthercatNet)))
            ((CDlgHWEthercatNet *)m_pHWDlg)->RemoveCurrentConnection ();
		else
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWModule)))
            ((CDlgHWModule *)m_pHWDlg)->RemoveCurrentConnection ();
}

void CAdamoConfHWView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
    bool b=false;

    if (m_ePswLevel<=ePswBuilder)
        b=true;
	pCmdUI->Enable(b);
}

void CAdamoConfHWView::OnEditFind()
{
    char szBuffer[256];

    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()==CMainFrame::modeHardware)   {
        CAdamoBase *pDev=((CMainFrame*)AfxGetMainWnd ())->GetDockConfigTree ()->GetView ()->GetSelectedDevice ();
		CProject *pPrj = ((CMainFrame*)AfxGetMainWnd ())->GetDockConfigTree ()->GetView ()->GetSelectedProject ();
        if (pDev && pPrj)   {
            CAdamoDatiFile *ADF=pPrj->GetAdamoDatiFile ();
                if (!ADF->GetDeviceHW (pDev->GetDeviceID(), pDev->GetType(), szBuffer))
                FindInDialog (pPrj, szBuffer);
        }
		else
			if (pPrj)
				FindModule (pPrj);
    }
}

void CAdamoConfHWView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable ();
}

void CAdamoConfHWView::FindInDialog (CProject *pPrj, char *szBuffer)
{
    CRect rc;
    bool bClose=CloseCurrentDialog (pPrj, szBuffer);
    int nCard=szBuffer[1]-'1';

    GetClientRect (&rc);
    switch (szBuffer[0])   {
        case 'Y' :
            if (bClose)   {
                m_pHWDlg = new CDlgHWYoupsilon;
                ((CDlgHWYoupsilon *)m_pHWDlg)->SetKinemaAddress (nCard);
				((CDlgHWYoupsilon *)m_pHWDlg)->SetProject (pPrj);
                m_pHWDlg->Create (IDD_DLG_HW_YOUPSILON, this);
                m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
            }
            ((CDlgHWYoupsilon *)m_pHWDlg)->Find (szBuffer);
            break;
        case 'M' :
            if (bClose)   {
                m_pHWDlg = new CDlgHWSSCNETStation;
                ((CDlgHWSSCNETStation *)m_pHWDlg)->SetKinemaAddress (nCard);
                ((CDlgHWSSCNETStation *)m_pHWDlg)->SetSSCNETStation (GetStationFromHWAddress (szBuffer));
				((CDlgHWSSCNETStation *)m_pHWDlg)->SetProject (pPrj);
                m_pHWDlg->Create (IDD_DLG_HW_SSCNETSTATIONS, this);
                m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
            }
            ((CDlgHWSSCNETStation *)m_pHWDlg)->Find (szBuffer);
            break;
        case 'C' :
            if (bClose)   {
                m_pHWDlg = new CDlgHWCanOpenNodes;
				((CDlgHWCanOpenNodes *)m_pHWDlg)->SetModuleNumber (pPrj->GetMachine ()->GetHWModuleNumber ());
				((CDlgHWCanOpenNodes *)m_pHWDlg)->SetKinemaAddress (nCard);
                ((CDlgHWCanOpenNodes *)m_pHWDlg)->SetCanOpenNode (((CMainFrame *)AfxGetMainWnd ())->GetNodeFromHWAddress (szBuffer)+1);
				((CDlgHWCanOpenNodes *)m_pHWDlg)->SetProject (pPrj);
                m_pHWDlg->Create (IDD_DLG_HW_CANOPENNODES, this);
                m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
            }
            ((CDlgHWCanOpenNodes *)m_pHWDlg)->Find (szBuffer);
            break;
        case 'E' :
            if (bClose)   {
                m_pHWDlg = new CAdamoEthercatNode;
				((CAdamoEthercatNode *)m_pHWDlg)->SetEthercatNode (((CMainFrame *)AfxGetMainWnd ())->GetNodeFromHWAddress (szBuffer)+1);
				((CAdamoEthercatNode *)m_pHWDlg)->SetProject (pPrj);
                m_pHWDlg->Create (IDD_DLG_ETHERCAT_NODE, this);
                m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
            }
            ((CAdamoEthercatNode *)m_pHWDlg)->Find (szBuffer);
            break;
    }
}

void CAdamoConfHWView::FindModule (CProject *pPrj)
{
    CRect rc;
	CAdamoMachine *pMachine;

	DestroyDlgHW ();
	GetClientRect (&rc);
	pMachine = pPrj->GetMachine ();
	if (pMachine->GetTypeModule () == enEpsilon || pMachine->GetTypeModule () == enDRTOS)   {
		m_pHWDlg = new CDlgHWModule;
		((CDlgHWModule *)m_pHWDlg)->SetKinemaAddress (pMachine->GetHWModuleNumber ());
		m_pHWDlg->Create (IDD_DLG_HW_MODULE, this);
		m_pHWDlg->MoveWindow (0, 0, rc.Width (), rc.Height ());
	}
}

bool CAdamoConfHWView::CloseCurrentDialog (CProject *pPrj, char *szBuffer)
{
    bool bClose=true;
    int nCard=szBuffer[1]-'1';
    if (m_pHWDlg)   {
        switch (szBuffer[0])   {
            case 'Y' :
                if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWYoupsilon)))
                    if (((CDlgHWYoupsilon *)m_pHWDlg)->GetProject ()==pPrj)
                        bClose=false;
                break;
            case 'M' :
                if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETStation)))
                    if (((CDlgHWSSCNETStation *)m_pHWDlg)->GetProject ()==pPrj)   {
                        if (GetStationFromHWAddress (szBuffer) == ((CDlgHWSSCNETStation *)m_pHWDlg)->GetSSCNETStation ())
                            bClose=false;
                    }
                break;
            case 'C' :
                if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWCanOpenNodes)))
                    if (((CDlgHWCanOpenNodes *)m_pHWDlg)->GetProject ()==pPrj)
                        if (((CMainFrame *)AfxGetMainWnd ())->GetNodeFromHWAddress (szBuffer)+1 == ((CDlgHWCanOpenNodes *)m_pHWDlg)->GetCanOpenNode ())
                            bClose=false;
                break;
            case 'E' :
                if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CAdamoEthercatNode)))
                    if (((CMainFrame *)AfxGetMainWnd ())->GetNodeFromHWAddress (szBuffer)+1 == ((CAdamoEthercatNode *)m_pHWDlg)->GetEthercatNode ())
                        bClose=false;
                break;
        }
        if (bClose)
            DestroyDlgHW ();
    }
    return bClose;
}

/*
** GetStationFromHWAddress :
*/
int CAdamoConfHWView::GetStationFromHWAddress (const char *szBuffer)
{
    int n=0;
    const char *p=strrchr (szBuffer, 'x');
    if (p)
        n=atoi (p+1);
    return n-1;
}

/*
** FlushData :
*/
void CAdamoConfHWView::FlushData ()
{
	if (m_pHWDlg)   {
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETStation)))
			((CDlgHWSSCNETStation *)m_pHWDlg)->Save ();
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWCanOpenNet)))
			((CDlgHWCanOpenNet *)m_pHWDlg)->Save ();
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWCanOpenNodes)))
			((CDlgHWCanOpenNodes *)m_pHWDlg)->Save ();
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETNet)))
			((CDlgHWSSCNETNet *)m_pHWDlg)->Save ();
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWEthercatNet)))
			((CDlgHWEthercatNet*)m_pHWDlg)->Save ();
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWModule)))
			((CDlgHWModule*)m_pHWDlg)->Save ();
	}
}

void CAdamoConfHWView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    DestroyDlgHW ();	
}

/*
** OnDownloadSSCNETParams :
*/
void CAdamoConfHWView::OnDownloadSSCNETParams() 
{
    if (m_pHWDlg)
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETStation)))
            ((CDlgHWSSCNETStation *)m_pHWDlg)->DownloadSSCNETParams ();
}

/*
** OnUploadSSCNETParams :
*/
void CAdamoConfHWView::OnUploadSSCNETParams() 
{
    if (m_pHWDlg)
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETStation)))
            ((CDlgHWSSCNETStation *)m_pHWDlg)->UploadSSCNETParams ();
}

/*
** OnUpdateDownloadSSCNETParams :
*/
void CAdamoConfHWView::OnUpdateDownloadSSCNETParams(CCmdUI* pCmdUI) 
{
    bool b=false;

    if (m_pHWDlg)
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETStation)))
            b=true;
    pCmdUI->Enable (b);
}

/*
** OnUpdateUploadSSCNETParams :
*/
void CAdamoConfHWView::OnUpdateUploadSSCNETParams(CCmdUI* pCmdUI) 
{
    bool b=false;

    if (m_pHWDlg)
        if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETStation)))
            b=true;
    pCmdUI->Enable (b);
}

/*
** OnChangePsw :
*/
void CAdamoConfHWView::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;

	if (m_pHWDlg)   {
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETStation)))
			((CDlgHWSSCNETStation *)m_pHWDlg)->OnChangePsw (oldPsw, newPsw);
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWCanOpenNet)))
			((CDlgHWCanOpenNet *)m_pHWDlg)->OnChangePsw (oldPsw, newPsw);
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWCanOpenNodes)))
			((CDlgHWCanOpenNodes *)m_pHWDlg)->OnChangePsw (oldPsw, newPsw);
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWSSCNETNet)))
			((CDlgHWSSCNETNet *)m_pHWDlg)->OnChangePsw (oldPsw, newPsw);
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CDlgHWEthercatNet)))
			((CDlgHWEthercatNet *)m_pHWDlg)->OnChangePsw (oldPsw, newPsw);
		else
		if (m_pHWDlg->IsKindOf (RUNTIME_CLASS (CAdamoEthercatNode)))
			((CAdamoEthercatNode *)m_pHWDlg)->OnChangePsw (oldPsw, newPsw);
	}
}

int CAdamoConfHWView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	return 0;
}

eEthercatDeviceType CAdamoConfHWView::IsSlaveConfigured (CProject *pPrj, int nNode)
{
	eEthercatDeviceType e=eEth;
	CAdamoHWConfigFile *HWConfigFile=pPrj->GetHWConfigFile ();
		if (HWConfigFile)
				e=(eEthercatDeviceType) HWConfigFile->GetEthercatDeviceType (nNode-1, 0);
	return e;
}

CProject *CAdamoConfHWView::GetAvailablePrj (int nCardType, int nCard)
{
	CAdamoMachine *pMachine;
	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	bool bFound = false;
	CProject *pPrj = pPlant->GetFirst ();
	while (pPrj)   {
		pMachine = pPrj->GetMachine ();
		if (pMachine)   {
			switch (nCardType)   {
				case PCI_YOUPSILON :
					if (pMachine->GetTypeModule () == enNoModule) 
						bFound = true;
					break;
				case PCI_OMIKRON :
					if (pMachine->GetTypeModule () == enNoModule) 
						bFound = true;
					break;
				case NET_EPSILON :
					if (pMachine->GetTypeModule () == enNoModule || pMachine->GetTypeModule () == enEpsilon || pMachine->GetTypeModule () == enDRTOS && pMachine->GetHWModuleNumber () == nCard) 
						bFound = true;
					break;
				default :
					bFound = true;		// Ethercat
					break;
			}
			if (bFound)
				break;
		}
		pPrj = (CProject *) pPlant->GetNext ();
	}
	return pPrj;
}