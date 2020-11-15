// AdamoDGDevice.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "workspace.h"
#include "mainframe.h"
#include "AdamoDGDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGDevice
CAdamoDGDevice::CAdamoDGDevice() : m_nCanScanStatus (NO_CAN_SCAN)
{
}

CAdamoDGDevice::~CAdamoDGDevice()
{
}

BEGIN_MESSAGE_MAP(CAdamoDGDevice, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CAdamoDGDevice)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAdamoDGDevice message handlers

/*
** OnCreate :
*/
int CAdamoDGDevice::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_image.Create (16, 16, ILC_COLOR24|ILC_MASK, 1, 1))
        return -1;
    CBitmap* pBmp=new CBitmap;
    pBmp->LoadBitmap (IDB_DG_DEVICE);
    m_image.Add (pBmp, RGB(0, 128, 128));
	SetImageList (&m_image, LVSIL_SMALL);
    SetMachine ();
    m_cxImage=m_cyImage=16;
    delete pBmp;
	return 0;
}

/*
** SetMachine :
*/
void CAdamoDGDevice::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}

/*
** InitializeGrid :
*/
void CAdamoDGDevice::InitializeGrid ()
{
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if (pFrame)
	    SetFont(pFrame->GetProgramFont());
    FillColumns ();
    FillGrid ();
}

/*
** FillColumns : configura le colonne del supergridctrl
*/
void CAdamoDGDevice::FillColumns ()
{
    CString str;
	LV_COLUMN   lvColumn;
    int n=1;

    /* creiamo le colonne */
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_IMAGE;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 150;
    str=LOADSTRING (IDS_ADAMODGDEVICE_1);
	lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=0;
	InsertColumn(0,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_2);
	lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=1;
	InsertColumn(1,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_3);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=2;
    InsertColumn(2,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_4);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
	lvColumn.cx = 120;
    lvColumn.iImage=3;
    InsertColumn(3,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_5);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=4;
    InsertColumn(4,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_6);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=5;
    InsertColumn(5,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_7);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=6;
    InsertColumn(6,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_8);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=7;
    InsertColumn(7,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_9);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=8;
    InsertColumn(8,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_10);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=9;
    InsertColumn(9,&lvColumn);
    SetGraphicalColumn (9, TRUE);
}

/*
** FillColumns : configura le colonne del supergridctrl
*/
void CAdamoDGDevice::FillGrid ()
{
    /* vediamo lo stato dello scan, se e' stato gia' eseguito uno scan, visualizziamolo */
    if (m_pMachine)   {
        int m_nCanScanStatus=m_pMachine->GetScanCanStatus ();
        if (m_nCanScanStatus==DONE_CAN_SCAN)   {
            /* e' stato eseguito lo scan, leggiamo i dati */
            CCmdGetDeviceInfo *pDeviceInfo=new CCmdGetDeviceInfo[NMAX_CANNODES];
            if (!m_pMachine->GetCanNodeData (pDeviceInfo))   {
                /* ok, recuperati i dati, ora presentiamoli nel grid */
                FillGridWithDeviceInfo (pDeviceInfo);
            }
            delete pDeviceInfo;
        }
    }
}

/*
** FillGridWithDeviceInfo :
*/
void CAdamoDGDevice::FillGridWithDeviceInfo (CCmdGetDeviceInfo *pDeviceInfo)
{
    CString str;

    /* scandiamo l'array di devide che ha ritornato il controllo */
    for (int i=0; i<NMAX_CANNODES; i++)   {
        if (pDeviceInfo[i].nStato!=-1)   {
            /* abbiamo trovato un nodo esistente */
	        CDGDeviceItemInfo* lp = new CDGDeviceItemInfo();
	        lp->SetImage (0);
            lp->SetNode (i+1);
	        lp->SetItemText (pDeviceInfo[i].szNomeNodo);
            str.Format ("%d", i+1);
            lp->AddSubItemText (str);
            str.Format ("%x", pDeviceInfo[i].nProfilo);
	        lp->AddSubItemText (str);
	        lp->AddSubItemText (pDeviceInfo[i].szVersioneHW);
	        lp->AddSubItemText (pDeviceInfo[i].szVersioneSW);
            str.Format ("%x", pDeviceInfo[i].nVendorID);
	        lp->AddSubItemText (str);
            str.Format ("%x", pDeviceInfo[i].nProductID);
	        lp->AddSubItemText (str);
            str.Format ("%x", pDeviceInfo[i].nRevisionNumber);
	        lp->AddSubItemText (str);
            str.Format ("%x", pDeviceInfo[i].nSerialNumber);
	        lp->AddSubItemText (str);
            lp->AddSubItemText ("");
            lp->SetDI (&pDeviceInfo[i]);
	        InsertRootItem (lp);
        }
    }
}

/*
** GetProfile :
*/
CString CAdamoDGDevice::GetProfile (CCmdGetDeviceInfo *pDeviceInfo)
{
    return CString ();
}

/*
** OnRefresh :
*/
void CAdamoDGDevice::OnRefresh ()
{
    if (m_pMachine)   {
        m_nCanScanStatus=m_pMachine->GetScanCanStatus ();
        if (m_nCanScanStatus==DONE_CAN_SCAN||m_nCanScanStatus==NO_CAN_SCAN)   {
            m_pMachine->StartCanScan ();
            m_nCanScanStatus=EXECUTING_CAN_SCAN;
        }
    }
}

/*
** OnSetOperational :
*/
void CAdamoDGDevice::OnSetOperational (int nMode)
{
    if (m_pMachine)   {
        if (nMode)   {
            int nIndex=GetSelectedItem ();
            if (nIndex!=-1)   {
                CTreeItem* pTreeItem=GetTreeItem (nIndex);
                if (pTreeItem)   {
                    CDGDeviceItemInfo* pData=(CDGDeviceItemInfo*) GetData (pTreeItem);
                    if (pData)   {
                        int nNode=pData->GetNode ();
                        m_pMachine->SetCanOperatingMode (nNode, 1);
                    }
                }
            }
        }
        else
            m_pMachine->SetCanOperatingMode (-1, 1);
    }
}

/*
** OnSetPreOperational :
*/
void CAdamoDGDevice::OnSetPreOperational (int nMode)
{
    if (m_pMachine)   {
        if (nMode)   {
            int nIndex=GetSelectedItem ();
            if (nIndex!=-1)   {
                CTreeItem* pTreeItem=GetTreeItem (nIndex);
                if (pTreeItem)   {
                    CDGDeviceItemInfo* pData=(CDGDeviceItemInfo*) GetData (pTreeItem);
                    if (pData)   {
                        int nNode=pData->GetNode ();
                        m_pMachine->SetCanOperatingMode (nNode, 0);
                    }
                }
            }
        }
        else
            m_pMachine->SetCanOperatingMode (-1, 0);
    }
}

/*
** RinfrescaInformazioni :
*/
void CAdamoDGDevice::RinfrescaInformazioni ()
{
    if (m_nCanScanStatus==EXECUTING_CAN_SCAN)   {
        int nCanScanStatus=m_pMachine->GetScanCanStatus ();
        if (nCanScanStatus==DONE_CAN_SCAN)   {
            DeleteAll ();
            FillGrid ();
            m_nCanScanStatus=nCanScanStatus;
			SetProgressPosition (0);
        }
		else
            if (nCanScanStatus == ERROR_CAN_SCAN)   {
                DeleteAll ();
                m_nCanScanStatus=DONE_CAN_SCAN;
			    SetProgressPosition (0);
            }
            else
			    SetProgressPosition (nCanScanStatus);
    }
}

/*
** OnControlLButtonDown :
*/
void CAdamoDGDevice::OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
}

/*
** GetCellRGB :
*/
COLORREF CAdamoDGDevice::GetCellRGB (void)
{
    return GetSysColor (COLOR_HIGHLIGHT);
}

/*
** OnSetInitialize :
*/
void CAdamoDGDevice::OnSetInitialize ()
{
    m_pMachine->SetCanInitialization ();
}

/*
** OnSetStart :
*/
void CAdamoDGDevice::OnSetStart ()
{
    m_pMachine->SetCanOperatingMode (-1, 2);
}

/*
** OnSetStop :
*/
void CAdamoDGDevice::OnSetStop ()
{
    m_pMachine->SetCanOperatingMode (-1, 3);
}

/*
** SetProgressPosition :
*/
void CAdamoDGDevice::SetProgressPosition (int nPosition)
{
	CProgressCtrl *pProgressCanOpen = ((CMainFrame *)AfxGetMainWnd ())->GetCanOpenProgressBar ();
	pProgressCanOpen->SetPos (nPosition);
}

/*
** OnChangePsw :
*/
void CAdamoDGDevice::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
}
