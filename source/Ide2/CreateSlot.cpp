// CreateSlot.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "CreateSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateSlot dialog


CCreateSlot::CCreateSlot(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateSlot::IDD, pParent), m_bmpSlot (NULL), m_bmpFunction (NULL), m_bmpFile (NULL), m_bmpLine (NULL), m_bc (NULL)
{
	//{{AFX_DATA_INIT(CCreateSlot)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCreateSlot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateSlot)
	DDX_Control(pDX, IDC_LISTSLOTS, m_ctrlCreateSlotList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateSlot, CDialog)
	//{{AFX_MSG_MAP(CCreateSlot)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SLOTCREATE, OnSlotCreate)
	ON_BN_CLICKED(IDC_SLOTGOTO, OnSlotGoto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateSlot message handlers

BOOL CCreateSlot::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    InitTitles ();
    InitList ();
	LoadFunctions ();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitTitles :
*/
void CCreateSlot::InitTitles() 
{
    SetWindowText (LOADSTRING (IDS_RUNSLOT));
    GetDlgItem (IDC_SLOTCREATE)->SetWindowText (LOADSTRING (IDS_RUNSLOT_RUNSLOT));
    GetDlgItem (IDC_SLOTGOTO)->SetWindowText (LOADSTRING (IDS_RUNSLOT_GOTO));
}

/*
** InitList :
*/
void CCreateSlot::InitList() 
{
    RECT rc;
    CString strColFunction, strColFile, strColLine;
    strColFunction=LOADSTRING (IDS_RUNSLOT_FUNCTION);
    strColFile=LOADSTRING (IDS_RUNSLOT_FILE);
    strColLine=LOADSTRING (IDS_RUNSLOT_LINE);
    m_ctrlCreateSlotList.GetClientRect (&rc);
    m_ctrlCreateSlotList.InsertColumn(0,strColFunction,LVCFMT_LEFT,200);	
    m_ctrlCreateSlotList.InsertColumn(1,strColFile,LVCFMT_LEFT,150);	
    m_ctrlCreateSlotList.InsertColumn(2,strColLine,LVCFMT_LEFT,150);	
    DWORD dw=m_ctrlCreateSlotList.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    m_ctrlCreateSlotList.SetExtendedStyle (dw);
    m_iml=new CImageList;
    m_bmpSlot=new CBitmap;
    m_bmpFunction=new CBitmap;
    m_bmpFile=new CBitmap;
    m_bmpLine=new CBitmap;
    m_bmpSlot->LoadBitmap(IDB_CAPSLOTS);
    m_bmpFunction->LoadBitmap(IDB_CAPFUNCTIONS);
    m_bmpFile->LoadBitmap(IDB_CAPPATH);
    m_bmpLine->LoadBitmap(IDB_CAPLINE);
    m_iml->Create (16, 16, ILC_COLOR24|ILC_MASK, 0, 1);
    m_iml->Add(m_bmpSlot, RGB(0, 128,128));
    m_iml->Add(m_bmpFile, RGB(0, 128,128));
    m_iml->Add(m_bmpLine, RGB(0, 128,128));
    m_iml->Add(m_bmpFunction, RGB(0, 128,128));
    CHeaderCtrl* hc=m_ctrlCreateSlotList.GetHeaderCtrl();
    hc->SetImageList (m_iml);
    HD_ITEM hd;
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    hc->SetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    hc->SetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=2;
    hc->SetItem(2, &hd);
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_ctrlCreateSlotList.SetImageList (m_iml, LVSIL_SMALL);
    m_ctrlCreateSlotList.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
}

/*
** LoadFunctions :
*/
void CCreateSlot::LoadFunctions() 
{
	CPtrList listLua;
    stBrowserDescriptor *p;
    int n=0;
	m_bc->GetLuaFunctions (listLua);
	for (POSITION pos = listLua.GetHeadPosition (); pos; )   {
		p = (stBrowserDescriptor *) listLua.GetNext (pos);
        AddFunction (GetIndex (p->szSymbol), p->szSymbol, p->szModule, p->nLine);
	}
    m_ctrlCreateSlotList.SetItemState (0, LVIS_SELECTED, LVIS_SELECTED);
}

void CCreateSlot::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    _delete (m_iml);	
    _delete (m_bmpSlot);
    _delete (m_bmpFunction);
    _delete (m_bmpFile);
    _delete (m_bmpLine);
}

/*
** AddFunction :
*/
void CCreateSlot::AddFunction (int nIndex, CString strFunction, CString strModule, int nLine) 
{
    CString str;
    LVITEM lvi;
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT|LVIF_IMAGE;
    lvi.iItem=nIndex;
    lvi.iSubItem=0;
    lvi.pszText=strFunction.GetBuffer(64);
    lvi.cchTextMax=strFunction.GetLength()+1;
    lvi.iImage=3;
    m_ctrlCreateSlotList.InsertItem (&lvi);
    strFunction.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nIndex;
    lvi.iSubItem=1;
    char *p=strModule.GetBuffer(_MAX_PATH);
    char *q=strrchr (p, '\\');
    lvi.pszText = q ? q+1 : p;
    lvi.cchTextMax=_MAX_PATH;
    m_ctrlCreateSlotList.SetItem (&lvi);
    strModule.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    str.Format ("%d", nLine);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nIndex;
    lvi.iSubItem=2;
    lvi.pszText=str.GetBuffer(64);
    lvi.cchTextMax=64;
    m_ctrlCreateSlotList.SetItem (&lvi);
    str.ReleaseBuffer ();
}

/*
** OnSlotCreate :
*/
void CCreateSlot::OnSlotCreate() 
{
    CProject *pPrj;
    CString str, strFile;
    int n;

    pPrj=GETWS ()->GetActiveProject ();
    if (pPrj)   {
        CAdamoMachine *pMachine=pPrj->GetMachine ();
        if ((n=m_ctrlCreateSlotList.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
            str=m_ctrlCreateSlotList.GetItemText (n, 0);
            pMachine->RunSlot (str);
            OnOK ();
        }
    }
}

/*
** OnSlotGoto :
*/
void CCreateSlot::OnSlotGoto() 
{
    CProject *pPrj;
    CString str, strFile;
    int n;

    pPrj=GETWS ()->GetActiveProject ();
    if (pPrj)   {
        if ((n=m_ctrlCreateSlotList.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
            str=m_ctrlCreateSlotList.GetItemText (n, 1);
            strFile = pPrj->GetProjectDir() + "\\" + str;
            str=m_ctrlCreateSlotList.GetItemText (n, 2);
	        ((CMainFrame*)AfxGetMainWnd())->GotoFileLine(strFile, atoi (str));
            OnOK ();
        }
    }
}

/*
** GetIndex :
*/
int CCreateSlot::GetIndex(CString strInsert)
{
    CString str;
    int nItem=m_ctrlCreateSlotList.GetNextItem (-1, LVNI_ALL), nIndex=0;
    while (nItem!=-1)   {
        str=m_ctrlCreateSlotList.GetItemText (nItem, 0);
        if (strInsert<str)
            break;
        nItem=m_ctrlCreateSlotList.GetNextItem (nItem, LVNI_ALL);
        nIndex++;
    }
    if (nItem==-1)
        nItem=nIndex;
    return nItem;
}
