// Breakpoints.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "breakpoints.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBreakpoints dialog


CBreakpoints::CBreakpoints(CWnd* pParent /*=NULL*/)
	: CDialog(CBreakpoints::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBreakpoints)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBreakpoints::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBreakpoints)
	DDX_Control(pDX, IDC_LIST_BREAKPOINTS, m_ctrlBreakPointsList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBreakpoints, CDialog)
	//{{AFX_MSG_MAP(CBreakpoints)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_GOTO, OnBtnGoto)
	ON_BN_CLICKED(IDC_BTN_REMOVE, OnBtnRemove)
	ON_BN_CLICKED(IDC_BTN_REMOVEALL, OnBtnRemoveAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBreakpoints message handlers

BOOL CBreakpoints::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    InitTitles ();
    InitList ();
	LoadBreakPoints ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitTitles :
*/
void CBreakpoints::InitTitles () 
{
    SetWindowText (LOADSTRING (IDS_BREAKPOINTS));
    GetDlgItem (IDC_ST_BREAKPOINTS)->SetWindowText (LOADSTRING (IDS_BREAKPOINTS));
    GetDlgItem (IDC_BTN_REMOVE)->SetWindowText (LOADSTRING (IDS_BREAKPOINTS_REMOVE));
    GetDlgItem (IDC_BTN_REMOVEALL)->SetWindowText (LOADSTRING (IDS_BREAKPOINTS_REMOVE_ALL));
    GetDlgItem (IDC_BTN_GOTO)->SetWindowText (LOADSTRING (IDS_BREAKPOINTS_GOTO));
}

/*
** InitList :
*/
void CBreakpoints::InitList () 
{
    RECT rc;
    CString strColFile, strColLine;
    strColFile=LOADSTRING (IDS_BREAKPOINTS_FILE);
    strColLine=LOADSTRING (IDS_BREAKPOINTS_LINE);
    m_ctrlBreakPointsList.GetClientRect (&rc);
    m_ctrlBreakPointsList.InsertColumn(0,strColFile,LVCFMT_LEFT,200);	
    m_ctrlBreakPointsList.InsertColumn(1,strColLine,LVCFMT_LEFT,rc.right-200);	
    DWORD dw=m_ctrlBreakPointsList.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    m_ctrlBreakPointsList.SetExtendedStyle (dw);
    m_iml=new CImageList;
    m_bmpFile=new CBitmap;
    m_bmpLine=new CBitmap;
    m_bmpFile->LoadBitmap(IDB_BMPFILE);
    m_bmpLine->LoadBitmap(IDB_BMP_BREAKPOINTS);
    m_iml->Create (16, 16, ILC_COLOR24|ILC_MASK, 0, 1);
    m_iml->Add(m_bmpFile, RGB(0, 128,128));
    m_iml->Add(m_bmpLine, RGB(0, 128,128));
    CHeaderCtrl* hc=m_ctrlBreakPointsList.GetHeaderCtrl();
    hc->SetImageList (m_iml);
    HDITEM hd;
    hc->GetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    hc->SetItem(0, &hd);
    hc->GetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    hc->SetItem(1, &hd);
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_ctrlBreakPointsList.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
}

/*
** LoadBreakPoints :
*/
void CBreakpoints::LoadBreakPoints ()
{
    CProject *pPrj;
    CProjectFile *pPFile;
    int i=0, nIndex=0;

    pPrj=GETWS ()->GetActiveProject ();
    if (pPrj)   {
        while (pPFile=pPrj->GetProjectFile (i))   {
            CMap<int, int, BOOL, BOOL>& bp=pPFile->GetBreakPoints ();
            POSITION pos = bp.GetStartPosition();
            while (pos != NULL)   {
                int nLine, nTemp;
	            bp.GetNextAssoc (pos, nLine, nTemp);
                AddBreakPoint (nIndex++, pPFile->GetName (), nLine);
            }
            i++;
        }
    }
}

/*
** AddBreakPoint :
*/
void CBreakpoints::AddBreakPoint (int nIndex, CString strFile, int nLine) 
{
    CString str;
    LVITEM lvi;
    memset (&lvi, 0, sizeof (LVITEM));
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nIndex;
    lvi.iSubItem=0;
    lvi.pszText=strFile.GetBuffer(64);
    lvi.cchTextMax=64;
    m_ctrlBreakPointsList.InsertItem (&lvi);
    strFile.ReleaseBuffer ();
    memset (&lvi, 0, sizeof (LVITEM));
    str.Format ("%d", nLine);
    lvi.mask=LVIF_TEXT;
    lvi.iItem=nIndex;
    lvi.iSubItem=1;
    lvi.pszText=str.GetBuffer(64);
    lvi.cchTextMax=64;
    m_ctrlBreakPointsList.SetItem (&lvi);
    str.ReleaseBuffer ();
}

void CBreakpoints::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    _delete (m_iml);	
    _delete (m_bmpFile);
    _delete (m_bmpLine);
}

/*
** OnBtnGoto :
*/
void CBreakpoints::OnBtnGoto() 
{
	// TODO: Add your control notification handler code here
    CProject *pPrj;
    CString str, strFile;
    int n;

    pPrj=GETWS ()->GetActiveProject ();
    if (pPrj)   {
        if ((n=m_ctrlBreakPointsList.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
            str=m_ctrlBreakPointsList.GetItemText (n, 0);
            strFile = pPrj->GetProjectDir() + "\\" + str + ".ada";
            str=m_ctrlBreakPointsList.GetItemText (n, 1);
	        ((CMainFrame*)AfxGetMainWnd())->GotoFileLine(strFile, atoi (str));
            OnOK ();
        }
    }
}

/*
** OnBtnRemove :
*/
void CBreakpoints::OnBtnRemove() 
{
    CProject *pPrj;
    CProjectFile *pPFile;
    CString str, strFile;
    int nModule, nItem, n;

    pPrj=GETWS ()->GetActiveProject ();
    if (pPrj)   {
        if ((nItem=m_ctrlBreakPointsList.GetNextItem (-1, LVIS_SELECTED))!=-1)   {
            str=m_ctrlBreakPointsList.GetItemText (nItem, 0);
            strFile = pPrj->GetProjectDir() + "\\" + str + ".ada";
            str=m_ctrlBreakPointsList.GetItemText (nItem, 1);
            n=atoi (str);
            m_ctrlBreakPointsList.DeleteItem (nItem);
            pPFile=pPrj->GetProjectFile (strFile, &nModule);
            if (pPFile)   {
                pPFile->RemoveBreakPoint (n);
                CLuaView *pView=((CIdeApp *)AfxGetApp ())->FindProjectFilesView (pPFile);
                if (pView)
                    pView->ToggleBreakPoint (n);
            }
        }
    }
}

/*
** OnBtnRemoveAll :
*/
void CBreakpoints::OnBtnRemoveAll() 
{
	// TODO: Add your control notification handler code here
    CProject *pPrj;
    CProjectFile *pPFile;
    int i=0, nIndex=0;

    pPrj=GETWS ()->GetActiveProject ();
    if (pPrj)   {
        while (pPFile=pPrj->GetProjectFile (i))   {
            CMap<int, int, BOOL, BOOL>& bp=pPFile->GetBreakPoints ();
            CLuaView *pView=((CIdeApp *)AfxGetApp ())->FindProjectFilesView (pPFile);
            POSITION pos = bp.GetStartPosition();
            if (pView)
                while (pos != NULL)   {
                    int nLine, nTemp;
	                bp.GetNextAssoc (pos, nLine, nTemp);
                    pView->ToggleBreakPoint (nLine);
                }
            pPFile->RemoveAllBreakPoints ();
            i++;
        }
    }
    OnOK ();
}
