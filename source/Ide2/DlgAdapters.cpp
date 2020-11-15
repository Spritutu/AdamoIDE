// DlgConnectToTcp.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DlgAdapters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoAboutBox dialog

CDlgAdapters::CDlgAdapters(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAdapters::IDD, pParent), m_nCurrentAdapter (0)
{
	//{{AFX_DATA_INIT(CAdamoAboutBox)
	//}}AFX_DATA_INIT
}

void CDlgAdapters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ADAPTERS, m_ctrlAdapters);
	//{{AFX_DATA_MAP(CAdamoAboutBox)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgAdapters, CDialog)
	//{{AFX_MSG_MAP(CAdamoAboutBox)
	ON_COMMAND(ID_DELETE_ADAPTER, OnDeleteAdapter)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoAboutBox message handlers

BOOL CDlgAdapters::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitListViewAdapters ();
	SetImageList ();
	FillAdapters ();
	SelectCurrentAdapter ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAdapters::OnDestroy() 
{
}

void CDlgAdapters::InitListViewAdapters ()
{
    CRect rect;

    m_ctrlAdapters.GetClientRect (&rect);
    m_ctrlAdapters.SetExtendedStyle(LVS_EX_FULLROWSELECT);
    m_ctrlAdapters.InsertColumn(0,LOADSTRING (IDS_ADAPTER_CARD),LVCFMT_LEFT,400);
    m_ctrlAdapters.InsertColumn(1,LOADSTRING (IDS_ADAPTER_DESCRIPTION),LVCFMT_LEFT,rect.Width() - 400);
}

void CDlgAdapters::FillAdapters ()
{
    LVITEM lvi;
	int nIndex = 0;
    char szBuffer[256];

    while (nIndex < m_pAdapters.nAdapters)   {
		if (CString (m_pAdapters.strAdaptersName[nIndex]) == m_strCurrentAdapter)
			m_nCurrentAdapter = nIndex;
		wsprintf (szBuffer, "%s", m_pAdapters.strAdaptersName[nIndex]);
		lvi.mask = LVIF_TEXT; 
		lvi.iItem = nIndex;
		lvi.iSubItem = 0; 
		lvi.pszText = szBuffer;
		lvi.cchTextMax=256;
		m_ctrlAdapters.InsertItem (&lvi);
		wsprintf (szBuffer, "%s", m_pAdapters.strAdaptersDesc[nIndex]);
		lvi.mask = LVIF_TEXT; 
		lvi.iItem = nIndex;
		lvi.iSubItem = 1; 
		lvi.pszText = szBuffer;
		lvi.cchTextMax=256;
		m_ctrlAdapters.SetItem (&lvi);
        nIndex++;
    }
}

void CDlgAdapters::SelectCurrentAdapter ()
{
	m_ctrlAdapters.SetItemState (-1, 0, LVIS_SELECTED);
	m_ctrlAdapters.SetItemState (m_nCurrentAdapter, LVIS_SELECTED, LVIS_SELECTED);
	m_ctrlAdapters.SetSelectionMark (m_nCurrentAdapter);
}

void CDlgAdapters::OnOK ()
{
	int index = m_ctrlAdapters.GetNextItem(-1, LVNI_SELECTED);
	if (index != -1)
		m_strCurrentAdapter = m_ctrlAdapters.GetItemText (index, 0);
	__super::OnOK ();
}

void CDlgAdapters::SetImageList ()
{
    HDITEM hd;
	m_images.Create(MAKEINTRESOURCE(IDB_BMP_SELSYMBOLS), 16, 1, RGB(0,128,128));
	m_ctrlAdapters.GetHeaderCtrl ()->SetImageList (&m_images);
    m_ctrlAdapters.GetHeaderCtrl ()->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    m_ctrlAdapters.GetHeaderCtrl ()->SetItem(0, &hd);
    m_ctrlAdapters.GetHeaderCtrl ()->GetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    m_ctrlAdapters.GetHeaderCtrl ()->SetItem(1, &hd);
}

void CDlgAdapters::OnDeleteAdapter ()
{
	m_strCurrentAdapter = "";
	__super::OnOK ();
}
