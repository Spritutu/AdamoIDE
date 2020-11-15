// dlgstrings.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "dlgstrings.h"
#include "mainframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgStrings dialog


CDlgStrings::CDlgStrings(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgStrings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgStrings)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hd = new HD_ITEM[10240];
}

CDlgStrings::~CDlgStrings ()
{
	delete m_hd;
}

void CDlgStrings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgStrings)
	DDX_Control(pDX, IDC_LISTSTRINGS, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgStrings, CDialog)
	//{{AFX_MSG_MAP(CDlgStrings)
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgStrings message handlers

BOOL CDlgStrings::OnInitDialog() 
{
	CDialog::OnInitDialog();
    CRect rect;
    
    m_list.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_list.GetWindowRect(&rect);
	m_list.InsertColumn(0, LOADSTRING (IDS_ADAMODLGSTRINGS_1), LVCFMT_LEFT, rect.Width()  * 15/100, 0);
	m_list.InsertColumn(1, LOADSTRING (IDS_ADAMODLGSTRINGS_2), LVCFMT_LEFT, rect.Width()  * 70/100, 1);
	m_list.InsertColumn(2, LOADSTRING (IDS_ADAMODLGSTRINGS_3), LVCFMT_LEFT, (rect.Width() * 15/100)-1, 2);
    CHeaderCtrl* hc=m_list.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_iml=new CImageList;
    m_bmpString=new CBitmap;
    m_bmpPriority=new CBitmap;
    m_bmpDateTime=new CBitmap;
    m_bmpString->LoadBitmap(IDB_CAPSTRING);
    m_bmpPriority->LoadBitmap(IDB_CAPPRIORITY);
    m_bmpDateTime->LoadBitmap(IDB_CAPTIME);
    m_iml->Create (16, 16, ILC_MASK, 0, 1);
    m_iml->Add(m_bmpDateTime, RGB(0, 128,128));
    m_iml->Add(m_bmpString, RGB(0, 128,128));
    m_iml->Add(m_bmpPriority, RGB(0, 128,128));
    hc->SetImageList(m_iml);
    hc->GetItem(0, m_hd);
    m_hd->mask=HDI_IMAGE;
    m_hd->fmt= HDF_IMAGE;
    m_hd->iImage=0;
    hc->SetItem(0, m_hd);
    hc->GetItem(1, m_hd);
    m_hd->mask=HDI_IMAGE;
    m_hd->fmt= HDF_IMAGE;
    m_hd->iImage=1;
    hc->SetItem(1, m_hd);
    hc->GetItem(2, m_hd);
    m_hd->mask=HDI_IMAGE | HDI_FORMAT;
    m_hd->fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    m_hd->iImage=2;
    hc->SetItem(2, m_hd);
    DWORD dw=m_list.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    m_list.SetExtendedStyle (dw);
    SetWindowText ("Messaggi");
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgStrings::OnDestroy() 
{
	CDialog::OnDestroy();
    _delete (m_iml)
    _delete (m_bmpString);
    _delete (m_bmpPriority);
    _delete (m_bmpDateTime);
}

void CDlgStrings::OnOK() 
{
	m_pParentWnd->SendMessage (WM_CLOSEDIALOGSTRINGS, 0, 0);
}

void CDlgStrings::OnCancel()
{
	m_pParentWnd->SendMessage (WM_CLOSEDIALOGSTRINGS, 0, 0);
}

void CDlgStrings::FillList ()
{
    CString strModule, strLine, strError;
    POSITION pos;
    int i;
    char szBufPriority[16];
    /* riempiamo ora il listview */
    if (m_pData->GetCount ())  {
        for (i=0, pos=m_pData->GetTailPosition(); pos; i++)    {
            stStringElement* p=(stStringElement*) m_pData->GetPrev (pos);
            CString strHour=COleDateTime(p->dt).Format ("%H:%M:%S");
            sprintf (szBufPriority, "%d", p->nPriority);
            LVITEM lvi;
            memset (&lvi, 0, sizeof (LVITEM));
            lvi.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
            lvi.iItem=i;
            lvi.iSubItem=0;
            lvi.pszText=strHour.GetBuffer(64);
            lvi.cchTextMax=NMAXLENGTHERRORE;
            lvi.lParam=(long)p;
            m_list.InsertItem (&lvi);
            memset (&lvi, 0, sizeof (LVITEM));
            lvi.mask=LVIF_TEXT;
            lvi.iItem=i;
            lvi.iSubItem=1;
            lvi.pszText=p->str;
            lvi.cchTextMax=NMAXLENGTHERRORE;
            m_list.SetItem (&lvi);
            memset (&lvi, 0, sizeof (LVITEM));
            lvi.mask=LVIF_TEXT;
            lvi.iItem=i;
            lvi.iSubItem=2;
            lvi.pszText=szBufPriority;
            lvi.cchTextMax=NMAXLENGTHERRORE;
            m_list.SetItem (&lvi);
        }
    }
}

void CDlgStrings::OnClose() 
{
	m_pParentWnd->SendMessage (WM_CLOSEDIALOGSTRINGS, 0, 0);
}
