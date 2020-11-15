// DlgSelSymbol.cpp: file di implementazione
//

#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "DlgSelSymbol.h"
#include "afxdialogex.h"


// Finestra di dialogo DlgSelSymbol

IMPLEMENT_DYNAMIC(CDlgSelSymbol, CDialog)

CDlgSelSymbol::CDlgSelSymbol(CWnd* pParent /*=nullptr*/) 
	: CDialog(IDD_DLG_SEL_SYMBOL, pParent), m_pSelectedBrowseData (NULL)
{

}

CDlgSelSymbol::~CDlgSelSymbol()
{
}

void CDlgSelSymbol::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SEL_SYMBOL, m_SelSymbol);
}


BEGIN_MESSAGE_MAP(CDlgSelSymbol, CDialog)
END_MESSAGE_MAP()


// Gestori di messaggi di DlgSelSymbol


BOOL CDlgSelSymbol::OnInitDialog()
{
    CRect rect;
    LV_ITEM lv;
    CDialog::OnInitDialog();
    char szBuffer[256];

    SetWindowText(LOADSTRING (IDS_BROWSE_SELSYMBOL));
    SetFont(((CMainFrame*)AfxGetMainWnd())->GetProgramFont());
    m_SelSymbol.GetHeaderCtrl()->SetFont(((CMainFrame*)AfxGetMainWnd())->GetProgramFont());
    m_SelSymbol.GetClientRect(&rect);
    m_SelSymbol.InsertColumn(0, LOADSTRING(IDS_BROWSE_TYPE_SYMBOL), LVCFMT_LEFT, 200);
    m_SelSymbol.InsertColumn(1, LOADSTRING(IDS_BROWSE_DESCRIPTION), LVCFMT_LEFT, rect.Width() - 200);
    DWORD dw = m_SelSymbol.GetExtendedStyle();
    dw |= LVS_EX_FULLROWSELECT;
    m_SelSymbol.SetExtendedStyle(dw);
    // TODO:  Aggiungere qui ulteriori informazioni per l'inizializzazione
    SetImageList();
    for (int i = 0; i<m_nItemSearch; i++) {
        memset(&lv, 0, sizeof(LVITEM));
        lv.iItem = i;
        lv.iSubItem = 0;
        lv.mask = LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM;
        lv.iImage = 2;
        lv.lParam = (LPARAM) &m_pBrowseData[i];
        strcpy(szBuffer, GetSymbolName(&m_pBrowseData[i]));
        lv.pszText = szBuffer;
        lv.cchTextMax = 256;
        m_SelSymbol.InsertItem(&lv);
        memset(&lv, 0, sizeof(LVITEM));
        lv.mask = LVIF_TEXT;
        lv.iItem = i;
        lv.iSubItem = 1;
        strcpy(szBuffer, GetSymbolType(&m_pBrowseData[i]));
        lv.pszText = szBuffer;
        lv.cchTextMax = 256;
        m_SelSymbol.SetItem(&lv);
	}
    m_SelSymbol.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
    return TRUE;  // return TRUE unless you set the focus to a control
				  // ECCEZIONE: le pagine delle proprietà OCX devono restituire FALSE
}

CString CDlgSelSymbol::GetSymbolName (stBrowserDescriptor* bd)
{
    CString str;
    switch (bd->nBrowserType) {
    case BROWSER_TYPE_METHOD:
        str = CString (bd->szObject) + ":" + CString (bd->szSymbol);
        break;
    }
    return str;
}

CString CDlgSelSymbol::GetSymbolType(stBrowserDescriptor* bd)
{
    CString str;
    switch (bd->nBrowserType) {
    case BROWSER_TYPE_FUNCTION :
        str = LOADSTRING (IDS_BROWSE_TYPE_FUNCTION);
        break;
    case BROWSER_TYPE_LOCAL :
        str = LOADSTRING (IDS_BROWSE_TYPE_LOCAL);
        break;
    case BROWSER_TYPE_DEFINE :
        str = LOADSTRING (IDS_BROWSE_TYPE_DEFINE);
        break;
    case BROWSER_TYPE_SYMBOL :
        str = LOADSTRING (IDS_BROWSE_TYPE_SYMBOL);
        break;
    case BROWSER_TYPE_OBJECT :
        str = LOADSTRING (IDS_BROWSE_TYPE_OBJECT);
        break;
    case BROWSER_TYPE_METHOD :
        str = LOADSTRING (IDS_BROWSE_TYPE_METHOD);
        break;
    case BROWSER_TYPE_OBJECT_FIELD:
        str = LOADSTRING (IDS_BROWSE_TYPE_OBJECT_FIELD);
        break;
    case BROWSER_TYPE_TABLE_FIELD :
        str = LOADSTRING (IDS_BROWSE_TYPE_TABLE_FIELD);
        break;
    }
    return str;
}

void CDlgSelSymbol::OnOK()
{
    // TODO: aggiungere qui il codice specifico e/o chiamare la classe di base
    int index = m_SelSymbol.GetNextItem(-1, LVNI_SELECTED);
    if (index != -1)
        m_pSelectedBrowseData = (stBrowserDescriptor *) m_SelSymbol.GetItemData(index);
    CDialog::OnOK();
}

void CDlgSelSymbol::SetImageList()
{
    HDITEM hd;
    m_images.Create(MAKEINTRESOURCE(IDB_BMP_SELSYMBOLS), 16, 1, RGB(0, 128, 128));
    m_SelSymbol.GetHeaderCtrl()->SetImageList(&m_images);
    m_SelSymbol.GetHeaderCtrl()->GetItem(0, &hd);
    hd.mask = HDI_IMAGE | HDI_FORMAT;
    hd.fmt = HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage = 1;
    m_SelSymbol.GetHeaderCtrl()->SetItem(0, &hd);
    m_SelSymbol.GetHeaderCtrl()->GetItem(1, &hd);
    hd.mask = HDI_IMAGE | HDI_FORMAT;
    hd.fmt = HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage = 2;
    m_SelSymbol.GetHeaderCtrl()->SetItem(1, &hd);
    m_SelSymbol.SetImageList (&m_images, LVSIL_SMALL);
}
