#include "stdafx.h"
#include "mainframe.h"
#include "ShowModulesSuperGrid.h"

CShowModulesSuperGrid::CShowModulesSuperGrid(void) : m_nNumMachines (0), m_bMultimodule (false)
{
}

CShowModulesSuperGrid::~CShowModulesSuperGrid(void)
{
}

BEGIN_MESSAGE_MAP(CShowModulesSuperGrid, CSuperGridCtrl)
	//{{AFX_MSG_MAP(CTablesSuperGrid)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
** OnCreate :
*/
int CShowModulesSuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
    if (!m_image.Create (16, 16, ILC_COLOR24|ILC_MASK, 1, 1))
        return -1;
    CBitmap* pBmp=new CBitmap;
    pBmp->LoadBitmap (IDB_SHOWMODULES);
    m_image.Add (pBmp, RGB(0, 128, 128));
	SetImageList (&m_image, LVSIL_SMALL);
    SetBkColor (GetSysColor (COLOR_WINDOW));
    m_cxImage=m_cyImage=16;
    delete pBmp;
	return 0;
}

/*
** InitializeGrid :
*/
void CShowModulesSuperGrid::InitializeGrid ()
{
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if (pFrame)
	    SetFont(pFrame->GetProgramFont());
    FillColumns ();
	FillData ();
}

/*
** FillColumns :
*/
void CShowModulesSuperGrid::FillColumns ()
{
    CString str;
	LV_COLUMN   lvColumn;
    int n=1;

    /* creiamo le colonne */
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM | LVCF_IMAGE;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 150;
    str=LOADSTRING (IDS_ADAMODEVICE_10);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=0;
	InsertColumn(0,&lvColumn);
    str=LOADSTRING (IDS_CONFIG_HW_DESCRIZIONE);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=4;
	lvColumn.cx = 250;
    InsertColumn(1,&lvColumn);
    str=LOADSTRING (IDS_ADAMOOPT_4);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=5;
	lvColumn.cx = 150;
    InsertColumn(2,&lvColumn);
    str=LOADSTRING (IDS_CONFIG_HW_REMOTE_MODULE);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=6;
    InsertColumn(3,&lvColumn);
    str=LOADSTRING (IDS_ADAMODGDEVICE_10);
    lvColumn.pszText = str.GetBuffer (0);
    str.ReleaseBuffer ();
    lvColumn.iImage=7;
    InsertColumn(4,&lvColumn);
}

/*
** FillData :
*/
void CShowModulesSuperGrid::FillData ()
{
	/* andiamo a leggere i progetti attualmente in presa */
	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();
	m_bMultimodule = pPlant->GetCount () > 1;
	CProject *pPrj = pPlant->GetFirst ();
	while (pPrj)   {
		AddMachine (pPrj);
		pPrj = (CProject *) pPlant->GetNext ();
	}
}

/*
** RefreshData :
*/
void CShowModulesSuperGrid::RefreshData ()
{
	/* cancelliamo tutte le righe del grid */
	DeleteAll ();
	/* aggiorniamo i dati */
	FillData ();
}

/*
** AddMachine :
*/
void CShowModulesSuperGrid::AddMachine (CProject *pPrj)
{
	CSuperGridCtrl::CTreeItem* ti;
	CItemInfo *lp = new CItemInfo;
	CString strMachineName = pPrj->GetMachineModule()->GetName();
	CString strMachineDescription = pPrj->GetMachineModule()->GetDesc();
	CString strConnectionType, strModule, strStato;
	int nImageIndex, nHWModuleNumber = -1;
	bool bActivePrj = GETWS()->GetActiveProject () == pPrj;

	switch (pPrj->GetMachine()->GetConnectionType ())   {
		case enLocalConnection :
			strConnectionType = LOADSTRING (IDS_ADAMOOPT_5);
			if (!m_bMultimodule)
				nImageIndex = 2;
			else
				nImageIndex = bActivePrj ? 3 : 2;
			break;
		case enTCPConnection :
			strConnectionType = LOADSTRING (IDS_ADAMOOPT_6);
			nHWModuleNumber = pPrj->GetMachine()->GetHWModuleNumber ();
			if (!m_bMultimodule)
				nImageIndex = 0;
			else
				nImageIndex = bActivePrj ? 1 : 0;
			break;
	};
	if (nHWModuleNumber == -1)
		strModule = LOADSTRING (IDS_LOCAL_PC);
	else
		strModule.Format ("%s %d", LOADSTRING (IDS_REMOTE), nHWModuleNumber);
	strStato = pPrj->GetMachine()->IsCncOnline () ? LOADSTRING (IDS_CONNECTED) : LOADSTRING (IDS_NOT_CONNECTED);
	lp->SetImage (nImageIndex);
	lp->SetItemText (strMachineName);
	lp->m_lParam = (LPARAM) pPrj;
	lp->AddSubItemText (strMachineDescription);
	lp->AddSubItemText (strConnectionType);
	lp->AddSubItemText (strModule);
	lp->AddSubItemText (strStato);
	ti = InsertRootItem (lp);
	if (m_nNumMachines++ == 0)
		SelectNode (ti);
}

/*
** GetIcon :
*/
int CShowModulesSuperGrid::GetIcon(const CTreeItem* pItem)
{
    int n = 0;

    if (pItem != NULL)
		n = GetData (pItem)->GetImage();
	return n;
}

/*
** OnControlLButtonDown :
*/
void CShowModulesSuperGrid::OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
}

/*
** GetSelectedPrj :
*/
CProject *CShowModulesSuperGrid::GetSelectedPrj ()
{
	CProject *pPrj = NULL;
	int nItem = GetSelectedItem ();

	if (nItem != -1)  {
		CTreeItem *pTreeItem = GetTreeItem (nItem);
		if (pTreeItem)   {
			CItemInfo *pItemInfo = pTreeItem->GetNodeInfo ();
			if (pItemInfo)
				pPrj = (CProject *) pItemInfo->m_lParam;
		}
	}
	return pPrj;
}

/*
** GetCellRGB :
*/
COLORREF CShowModulesSuperGrid::GetCellRGB (void)
{
    return GetSysColor (COLOR_HIGHLIGHT);
}

/*
** OnVkReturn :
*/
BOOL CShowModulesSuperGrid::OnVkReturn(void)
{
	GetParent ()->SendMessage (WM_ACTIVATE_MACHINE, 0, 0);
	return 1;
}
