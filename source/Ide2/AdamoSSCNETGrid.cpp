// AdamoSSCNETGrid.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "adamosscnetgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoSSCNETGrid

CAdamoSSCNETGrid::CAdamoSSCNETGrid() : m_nCount (0)
{
}

CAdamoSSCNETGrid::~CAdamoSSCNETGrid()
{
}


BEGIN_MESSAGE_MAP(CAdamoSSCNETGrid, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CAdamoSSCNETGrid)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
** InitializeGrid :
*/
void CAdamoSSCNETGrid::InitializeGrid ()
{
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if (pFrame)
	    SetFont(pFrame->GetProgramFont());
    FillColumns ();
    FillAxesArray ();
}

/*
** FillAxesArray :
*/
void CAdamoSSCNETGrid::FillAxesArray ()
{
    CString str;
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)   {
	        CAdamoDatiFile *pDatiFile = new CAdamoDatiFile();
	        if (pDatiFile)   {
		        if (pDatiFile->Open (pActivePrj->GetProjectDir() + "\\" + DEVICE_FILE, TRUE))   {
                    pDatiFile->ReadData ();
                    stAdamoParametri *pap=pDatiFile->GetAdamoParametri ();
                    m_aAxes.SetSize (NMAXMGASTATION);
                    for (int n = 0; n<NMAXMGASTATION; n++)   {
                        str.Format ("M%dAx%d", m_nCard+1, n + 1);
                        for (int i = 0; i < NMAXAXIS; i++)   {
                            if (pap->AdamoAsse[i].szNome[0] != '\0')   {
                                if (CString (pap->AdamoAsse[i].szHWAddress) == str)   {
                                    m_aAxes[n] = CString (pap->AdamoAsse[i].szNome);
                                    break;
                                }
                            }
                        }
                    }
                    pDatiFile->Close ();
                }
            }
            delete pDatiFile;
        }
    }
}

/*
** FillColumns :
*/
void CAdamoSSCNETGrid::FillColumns ()
{
    CString str;
	LV_COLUMN   lvColumn;
    int n=1;

    /* creiamo le colonne */
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_IMAGE;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 200;
    str=LOADSTRING (IDS_ADAMODGSSCNET_1);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=0;
	InsertColumn(0,&lvColumn);
	lvColumn.cx = 200;
    str=LOADSTRING (IDS_ADAMODGSSCNET_9);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=3;
    InsertColumn(1,&lvColumn);
	lvColumn.cx = 100;
    str=LOADSTRING (IDS_ADAMODGSSCNET_2);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=1;
    InsertColumn(2,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGSSCNET_3);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=2;
	lvColumn.cx = 600;
    InsertColumn(3,&lvColumn);
    SetGraphicalColumn (2, TRUE);
    SetGraphicalColumn (3, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoSSCNETGrid message handlers

int CAdamoSSCNETGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
    if (!m_image.Create (16, 16, ILC_COLOR24|ILC_MASK, 1, 1))
        return -1;
    CBitmap* pBmp=new CBitmap;
    pBmp->LoadBitmap (IDB_DG_SSCNET);
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
void CAdamoSSCNETGrid::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}


/*
** RinfrescaInformazioni :
*/
void CAdamoSSCNETGrid::RinfrescaInformazioni () 
{
    CString strAlarm;
    DWORD dwStatus, dwDisplay;
    int i;
    /* completiamo il refresh dei dispositivi andando a interrogare quelli che sono nel frattempo cambiati */
    if (m_pMachine)   {
        if (m_nCount==0)   {
            m_nCount=m_pMachine->GetSSCNETStations (m_nCard);
            if (m_nCount>0)   {
                for (i=0; i<m_nCount; i++)   {
					m_pMachine->GetSSCNETDisplay (m_nCard, i+1, &dwStatus, &dwDisplay);
                    AddSSCNETStation (i, 0, dwDisplay);
                }
            }
        }
        else
            for (i=0; i<m_nCount; i++)   {
                CTreeItem* pItem=GetRootItem (i);
				m_pMachine->GetSSCNETDisplay (m_nCard, i+1, &dwStatus, &dwDisplay);
                CAdamoSSCNETItemInfo *pItemInfo=(CAdamoSSCNETItemInfo *) pItem->GetNodeInfo ();
                pItemInfo->SetStatus (dwStatus);
				pItemInfo->SetDisplay (dwDisplay);
            }
        /* rinfreschiamo il controllo */
        Invalidate (FALSE);
    }
}

/*
** AddSSCNETStation :
*/
void CAdamoSSCNETGrid::AddSSCNETStation (int nStation, DWORD dwStatus, DWORD dwDisplay) 
{
    CString str;

    str.Format ("%s %d", LOADSTRING (IDS_ADAMODGSSCNET_1), nStation+1);
    CAdamoSSCNETItemInfo* lp = new CAdamoSSCNETItemInfo;
    lp->SetCard (m_nCard);
    lp->SetStation (nStation+1);
    lp->SetMachine (m_pMachine);
    lp->SetStatus (dwStatus);
	lp->SetDisplay (dwDisplay);
	lp->SetImage (3);
	lp->SetItemText (str);
	lp->AddSubItemText (m_aAxes[nStation]);
	lp->AddSubItemText ("");
	lp->AddSubItemText ("");
	InsertRootItem (lp);
}

/*
** OnPaintColumn :
*/
void CAdamoSSCNETGrid::OnPaintColumn (LPDRAWITEMSTRUCT lpDrawItemStruct, int nColumn, CRect* rc)
{
    CTreeItem* pItem=(CTreeItem*)lpDrawItemStruct->itemData;
    if (pItem)   {
        CAdamoSSCNETItemInfo *pItemInfo=(CAdamoSSCNETItemInfo *) pItem->GetNodeInfo ();
        if (pItemInfo)   {
            switch (nColumn)   {
                case 2 :
                    DrawStatus (pItemInfo->GetStatus (), pItemInfo->IsInAlarm (), lpDrawItemStruct, rc);
                    break;
                case 3 :
                    DrawAlarm  (pItemInfo->GetAlarm (), lpDrawItemStruct, rc);
                    break;
            }
        }
    }
}

/*
** DrawStatus :
*/
void CAdamoSSCNETGrid::DrawStatus (CString strStatus, bool bInAlarm, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc)
{
		::SetTextColor (lpDrawItemStruct->hDC, RGB (255, 0, 0));
    ::DrawText (lpDrawItemStruct->hDC, strStatus, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
}

/*
** DrawAlarm :
*/
void CAdamoSSCNETGrid::DrawAlarm (CString strAlarm, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect* rc)
{
    ::SetTextColor (lpDrawItemStruct->hDC, RGB (255, 0, 0));
    ::DrawText (lpDrawItemStruct->hDC, strAlarm, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
}

/*
** OnControlLButtonDown :
*/
void CAdamoSSCNETGrid::OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
}

/*
** GetIcon :
*/
int CAdamoSSCNETGrid::GetIcon(const CTreeItem* pItem)
{
    int n = 0;

    if (pItem != NULL)
		n = GetData (pItem)->GetImage();
	return n;
}

/*
** GetCellRGB :
*/
COLORREF CAdamoSSCNETGrid::GetCellRGB (void)
{
    return GetSysColor (COLOR_HIGHLIGHT);
}

/*
** GetCurrentStation :
*/
int CAdamoSSCNETGrid::GetCurrentStation ()
{
    return GetSelectedItem ();
}

/*
** SetStatus :
*/
void CAdamoSSCNETItemInfo::SetStatus (DWORD dwStatus)
{
    DWORD dwInAlarm=(dwStatus&0x00000080) | (dwStatus&0x00008000);
    /* vediamo se la stazione e' in errore */
    if (dwStatus&0x00000080)   {
        /* vediamo se era in errore */
        if (!(m_dwStatus&0x00000080))
            /* andiamo a leggere l'errore */
            m_pMachine->GetSSCNETAlarm (m_nCard, m_nStation, m_wAlarm, m_strAlarm);
    }
    else
        /* non e' in errore */
        if (m_dwStatus&0x00000080)
            /* era in errore, azzeriamo l'allarme */
            m_strAlarm="";
    /* vediamo se la stazione e' in warning */
    if (dwStatus&0x00008000)   {
        /* vediamo se era in warning */
        if (!(m_dwStatus&0x00008000))
            /* andiamo a leggere il warning */
            m_pMachine->GetSSCNETAlarm (m_nCard, m_nStation, m_wAlarm, m_strAlarm);
    }
    else
        /* non e' in warning */
        if (m_dwStatus&0x00008000)
            /* era in warning, azzeriamo l'allarme */
            m_strAlarm="";
	m_bInAlarm = (dwStatus&0x00000080) != 0;
	m_dwStatus = dwStatus;
}

/*
** SetStatus :
*/
void CAdamoSSCNETItemInfo::SetDisplay (DWORD dwDisplay)
{
	if (m_dwStatus&0x80000000)
		m_strStatus = "aa";
	else   {
		char c1 = dwDisplay&0x000000ff;
		char c2 = (dwDisplay&0x0000ff00)>>8;
		char c3 = (char)((dwDisplay&0x00ff0000)>>16);
		char c4 = (char)((dwDisplay&0xff000000)>>24);
		m_strStatus.Format ("%c%c%c%c", c1, c2, c3, c4);
	}
	m_dwDisplay = dwDisplay;
}

/*
** InizializzaInformazioni :
*/
void CAdamoSSCNETGrid::InizializzaInformazioni ()
{
	DWORD dwStatus, dwDisplay;
    if (m_nCount)   {
        for (int i=0; i<m_nCount; i++)   {
            CTreeItem* pItem=GetRootItem (i);
            CAdamoSSCNETItemInfo *pItemInfo=(CAdamoSSCNETItemInfo *) pItem->GetNodeInfo ();
            m_pMachine->GetSSCNETDisplay (m_nCard, i+1, &dwStatus, &dwDisplay);
			if (pItemInfo)   {
				pItemInfo->SetStatus (dwStatus);
				pItemInfo->SetDisplay (dwDisplay);
			}
        }
    }
}

/*
** OnVkReturn :
*/
BOOL CAdamoSSCNETGrid::OnVkReturn(void)
{
	return 1;
}

