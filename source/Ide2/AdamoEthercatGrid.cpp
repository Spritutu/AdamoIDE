// AdamoEthercatGrid.cpp: implementation of the CAdamoEthercatGrid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "AdamoEthercatGrid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CAdamoEthercatGrid, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CAdamoSSCNETGrid)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAdamoEthercatGrid::CAdamoEthercatGrid() : m_pMachine (NULL)
{

}

CAdamoEthercatGrid::~CAdamoEthercatGrid()
{

}

/*
** InitializeGrid :
*/
void CAdamoEthercatGrid::InitializeGrid()
{
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if (pFrame)
	    SetFont(pFrame->GetProgramFont());
    FillColumns ();
	AddEthercatSlave ();
}

/*
** FillColumns :
*/
void CAdamoEthercatGrid::FillColumns ()
{
    CString str;
	LV_COLUMN   lvColumn;
    int n=1;

    /* creiamo le colonne */
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_IMAGE;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 200;
    str=LOADSTRING (IDS_ADAMODGETHERCAT_1);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=0;
	InsertColumn(0,&lvColumn);
	lvColumn.cx = 150;
    str=LOADSTRING (IDS_ADAMODGETHERCAT_2);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=1;
    InsertColumn(1,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGETHERCAT_3);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=2;
    InsertColumn(2,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGETHERCAT_4);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=3;
    InsertColumn(3,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGETHERCAT_5);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=4;
    InsertColumn(4,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGETHERCAT_6);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=5;
    InsertColumn(5,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGETHERCAT_7);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=6;
    InsertColumn(6,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGETHERCAT_8);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=7;
    InsertColumn(7,&lvColumn);
    SetGraphicalColumn (7, TRUE);
}

/*
** OnCreate :
*/
int CAdamoEthercatGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
    if (!m_image.Create (16, 16, ILC_COLOR24|ILC_MASK, 1, 1))
        return -1;
    CBitmap* pBmp=new CBitmap;
    pBmp->LoadBitmap (IDB_DG_ETHERCAT);
    m_image.Add (pBmp, RGB(0, 128, 128));
    SetMachine ();
	SetImageList(&m_image, LVSIL_SMALL);
    m_cxImage=m_cyImage=16;
    delete pBmp;
	return 0;
}

/*
** SetMachine :
*/
void CAdamoEthercatGrid::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}

/*
** AddEthercatSlave :
*/
void CAdamoEthercatGrid::AddEthercatSlave ()
{
	CString str;
	CPtrArray &pSlaveArray=GetEthercatSlave ();
	stAdamoEthSlave *pSlave;
	int i, nIndexImage;

	for (i=0; i<pSlaveArray.GetSize (); i++)   {
		pSlave=(stAdamoEthSlave *)pSlaveArray[i];
		CAdamoEthercatItemInfo* lp = new CAdamoEthercatItemInfo;
		lp->SetSlave (i);
		switch (pSlave->nDeviceType[0])   {
		case eEth :
			nIndexImage=8;
			break;
		case eEthAsse :
			nIndexImage=9;
			break;
		case eEthIO :
			nIndexImage=10;
			break;
		case eEthIOAnalogic :
			nIndexImage=11;
			break;
		default :
			nIndexImage=8;
		}
		lp->SetImage (nIndexImage);
		lp->SetItemText (pSlave->szName);
		str.Format ("%d", pSlave->nPhysAddress);
		lp->AddSubItemText (str);
		str.Format ("%d", pSlave->nVendorID);
		lp->AddSubItemText (str);
		str.Format ("%d", pSlave->nProductCode);
		lp->AddSubItemText (str);
		str.Format ("%d", pSlave->nRevisionNumber);
		lp->AddSubItemText (str);
		str.Format ("%d", pSlave->nSerialNumber);
		lp->AddSubItemText (str);
		lp->AddSubItemText (pSlave->szProductRevision);
		InsertRootItem (lp);
	}
}

/*
** GetEthercatSlave :
*/
CPtrArray &CAdamoEthercatGrid::GetEthercatSlave ()
{
    CWorkspace* pWorkSpace=GETWS();
	CProject *pActivePrj = pWorkSpace->GetActiveProject();
	return pActivePrj->GetSlaveArray ();
}

/*
** OnControlLButtonDown :
*/
void CAdamoEthercatGrid::OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
}

/*
** GetIcon :
*/
int CAdamoEthercatGrid::GetIcon(const CTreeItem* pItem)
{
    int n = 0;

    if (pItem != NULL)
		n = GetData (pItem)->GetImage();
	return n;
}

/*
** GetCellRGB :
*/
COLORREF CAdamoEthercatGrid::GetCellRGB (void)
{
    return GetSysColor (COLOR_HIGHLIGHT);
}

/*
** OnPaintColumn :
*/
void CAdamoEthercatGrid::OnPaintColumn (LPDRAWITEMSTRUCT lpDrawItemStruct, int nColumn, CRect* rc)
{
    CTreeItem* pItem=(CTreeItem*)lpDrawItemStruct->itemData;
    if (pItem)   {
        CAdamoEthercatItemInfo *pItemInfo=(CAdamoEthercatItemInfo *) pItem->GetNodeInfo ();
        if (pItemInfo)
            if (nColumn==7)
				DisegnaStato (pItemInfo, lpDrawItemStruct, rc);
	}
}

/*
** DisegnaStato :
*/
void CAdamoEthercatGrid::DisegnaStato (CAdamoEthercatItemInfo *pItemInfo, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc)
{
	CDC dc;
	int nImageIndex=12;

	switch (pItemInfo->GetActualStatus ())   {
	case DEVICE_STATE_INIT :
		nImageIndex=12;
		break;
	case DEVICE_STATE_PREOP :
		nImageIndex=13;
		break;
	case DEVICE_STATE_SAFEOP :
		nImageIndex=14;
		break;
	case DEVICE_STATE_OP :
		nImageIndex=15;
		break;
	}
	dc.Attach (lpDrawItemStruct->hDC);
	m_image.Draw (&dc, nImageIndex, CPoint (rc->left, rc->top), ILD_TRANSPARENT);
	pItemInfo->SetStatus (pItemInfo->GetActualStatus ());
	dc.Detach ();
}

/*
** RinfrescaInformazioni :
*/
void CAdamoEthercatGrid::RinfrescaInformazioni () 
{
    CCmdGetCardInfo CI;
	CPtrArray &pSlaveArray=GetEthercatSlave ();
	int i;

    if (m_pMachine->GetCardInfo (ETHERCAT_PORT, &CI) == S_OK)   {
	    if (CI.eTipo==enEthercat)
		    for (i=0; i<pSlaveArray.GetSize (); i++)   {
			    CTreeItem* pItem=GetRootItem (i);
			    CAdamoEthercatItemInfo *pItemInfo=(CAdamoEthercatItemInfo *) pItem->GetNodeInfo ();
			    pItemInfo->SetActualStatus (CI.Ec.status[i]);
		    }
        /* rinfreschiamo il controllo */
        Invalidate (FALSE);
    }
}

/*
** RequestState :
*/
void CAdamoEthercatGrid::RequestState (int nRequestState, bool bSingleSlave)
{
    if (m_pMachine)   {
        if (bSingleSlave)   {
            int nIndex=GetSelectedItem ();
            if (nIndex!=-1)   {
                CTreeItem* pTreeItem=GetTreeItem (nIndex);
                if (pTreeItem)   {
                    CAdamoEthercatItemInfo* pData=(CAdamoEthercatItemInfo*) GetData (pTreeItem);
                    if (pData)   {
                        int nSlave=pData->GetSlave ();
                        m_pMachine->RequestState (nRequestState, nSlave);
                    }
                }
            }
        }
        else
            m_pMachine->RequestState (nRequestState, -1);
    }
}

/*
** GetSelectedSlave :
*/
int CAdamoEthercatGrid::GetSelectedSlave ()
{
	int nSlave = -1;
    if (m_pMachine)   {
        int nIndex=GetSelectedItem ();
        if (nIndex!=-1)   {
            CTreeItem* pTreeItem=GetTreeItem (nIndex);
            if (pTreeItem)   {
                CAdamoEthercatItemInfo* pData=(CAdamoEthercatItemInfo*) GetData (pTreeItem);
                if (pData)
                    nSlave=pData->GetSlave ();
            }
        }
    }
	return nSlave;
}