// LangView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "LangView.h"
#include "UserStrings.h"
#include "OleDropSourceEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLangView

IMPLEMENT_DYNCREATE(CLangView, CListView)

CLangView::CLangView() : m_hciml (NULL), m_bmpHdrID (NULL), m_bmpHdrValue (NULL),
                         m_bmpHdrString (NULL), m_bmpHdrDesc (NULL), m_bmpLV (NULL),
                         m_hcLV (NULL)
{
}

CLangView::~CLangView()
{
}


BEGIN_MESSAGE_MAP(CLangView, CListView)
	//{{AFX_MSG_MAP(CLangView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CLangView::OnLvnColumnclick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLangView drawing

void CLangView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLangView diagnostics

#ifdef _DEBUG
void CLangView::AssertValid() const
{
	CListView::AssertValid();
}

void CLangView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLangView message handlers
BOOL CLangView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT;
    cs.style |= LVS_SINGLESEL;
    cs.style |= LVS_SHOWSELALWAYS;
	return CListView::PreCreateWindow(cs);
}

void CLangView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
}

void CLangView::SetHeaderLook()
{

    CRect rect;
    CListCtrl& pCtrl=GetListCtrl ();

    CString strItem1(LOADSTRING (IDS_ADAMOLANGVIEW_1));
    CString strItem2(LOADSTRING (IDS_ADAMOLANGVIEW_2));
    CString strItem3(LOADSTRING (IDS_ADAMOLANGVIEW_3));
    CString strItem4(LOADSTRING (IDS_ADAMOLANGVIEW_4));
    pCtrl.SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFontBold ());
	pCtrl.InsertColumn(0, strItem1, LVCFMT_LEFT, 150, 0);
	pCtrl.InsertColumn(1, strItem2, LVCFMT_LEFT, 150, 1);
	pCtrl.InsertColumn(2, strItem3, LVCFMT_LEFT, 420, 2);
	pCtrl.InsertColumn(3, strItem4, LVCFMT_LEFT, 300, 3);

    CHeaderCtrl* hc=pCtrl.GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    m_hciml=new CImageList;
    m_hcLV=new CImageList;
    m_bmpHdrID=new CBitmap;
    m_bmpHdrValue=new CBitmap;
    m_bmpHdrString=new CBitmap;
    m_bmpHdrDesc=new CBitmap;
    m_bmpLV=new CBitmap;
    m_bmpHdrID->LoadBitmap(IDB_CAPMESSAGGIINDICESTRINGA);
    m_bmpHdrValue->LoadBitmap(IDB_CAPMESSAGGIINDICE);
    m_bmpHdrString->LoadBitmap(IDB_CAPMESSAGGI);
    m_bmpHdrDesc->LoadBitmap(IDB_CAPCOMMENT);
    m_bmpLV->LoadBitmap(IDB_CAPSTRING);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hcLV->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrID, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrValue, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrString, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrDesc, RGB(0, 128,128));
    m_hcLV->Add(m_bmpLV, RGB(0, 128,128));
    hc->SetImageList(m_hciml);
    //pCtrl.SetImageList (m_hcLV, LVSIL_SMALL);

    HDITEM *hd = new HDITEM;
    hc->GetItem(0, hd);
    hd->mask=HDI_IMAGE | HDI_FORMAT;
    hd->fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd->iImage=0;
    hc->SetItem(0, hd);
    hc->GetItem(1, hd);
    hd->mask=HDI_IMAGE | HDI_FORMAT;
    hd->fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd->iImage=1;
    hc->SetItem(1, hd);
    hc->GetItem(2, hd);
    hd->mask=HDI_IMAGE | HDI_FORMAT;
    hd->fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd->iImage=2;
    hc->SetItem(2, hd);
    hc->GetItem(3, hd);
    hd->mask=HDI_IMAGE | HDI_FORMAT;
    hd->fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd->iImage=3;
    hc->SetItem(3, hd);
	delete hd;
    /* settiamo lo stile full row al listview */
    DWORD dw=pCtrl.GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    dw|=LVS_EX_GRIDLINES;
    pCtrl.SetExtendedStyle (dw);
}

/*
 * OnDestroy
 */
void CLangView::OnDestroy() 
{
	CListView::OnDestroy();
	_delete (m_bmpHdrID);
	_delete (m_bmpHdrValue);
	_delete (m_bmpHdrString);
	_delete (m_bmpHdrDesc);
    _delete (m_bmpLV);
	_delete (m_hciml);
    _delete (m_hcLV);
}

/*
 * LoadStrings
 */
void CLangView::LoadStrings ()
{
    CString strStringa, strMessaggio, strDescrizione; 
    int i=0, n, nSel;
    long dwNumero;
    bool b;
    char szBuffer[256];
    LVITEM lvi;
    CListCtrl& pCtrl=GetListCtrl ();

    b=m_tr->PrimoMessaggioNumero (0, 0);
    while (b)   {
        b=m_tr->Successivo (0, &dwNumero, &strStringa, &strMessaggio, &strDescrizione);
        memset (&lvi, 0, sizeof (LVITEM));
        strcpy (szBuffer, strStringa);
        lvi.mask=LVIF_TEXT;
        lvi.iItem=i;
        lvi.iSubItem=0;
        lvi.pszText=szBuffer;
        lvi.cchTextMax=256;
        lvi.iImage=0;
        pCtrl.InsertItem (&lvi);
        memset (&lvi, 0, sizeof (LVITEM));
        sprintf (szBuffer, "%d", dwNumero);
        lvi.mask=LVIF_TEXT;
        lvi.iItem=i;
        lvi.iSubItem=1;
        lvi.pszText=szBuffer;
        lvi.cchTextMax=256;
        pCtrl.SetItem (&lvi);
        memset (&lvi, 0, sizeof (LVITEM));
        strcpy (szBuffer, strMessaggio);
        lvi.mask=LVIF_TEXT;
        lvi.iItem=i;
        lvi.iSubItem=2;
        lvi.pszText=szBuffer;
        lvi.cchTextMax=256;
        pCtrl.SetItem (&lvi);
        memset (&lvi, 0, sizeof (LVITEM));
        strcpy (szBuffer, strDescrizione);
        lvi.mask=LVIF_TEXT;
        lvi.iItem=i;
        lvi.iSubItem=3;
        lvi.pszText=szBuffer;
        lvi.cchTextMax=256;
        pCtrl.SetItem (&lvi);
        i++;
    }
    for (n=0; n<4; n++)   {
        memset (&lvi, 0, sizeof (LVITEM));
        szBuffer[0]='\0';
        if (n==0)   {
            lvi.mask=LVIF_TEXT;
            lvi.iItem=i;
        }
        else   {
            lvi.mask=LVIF_TEXT;
            lvi.iItem=i;
        }
        lvi.iSubItem=n;
        lvi.pszText=szBuffer;
        lvi.cchTextMax=256;
        lvi.iImage=4;
        if (n==0)
            nSel=pCtrl.InsertItem (&lvi);
        else
            pCtrl.SetItem (&lvi);
    }
    pCtrl.SetItemState (nSel, LVIS_SELECTED, LVIS_SELECTED);
}

int CLangView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
    SetHeaderLook ();
	return 0;
}

int CLangView::ActivateMachine ()
{
    CListCtrl& pCtrl=GetListCtrl ();
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        m_tr=pPrj->GetTraduzioni ();
        if (m_tr)   {
            pCtrl.DeleteAllItems ();
            LoadStrings ();
        }
    }
    return 0;
}


void CLangView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    CUserStrings usdlg;
    CString strStringa, strMessaggio, strDescrizione;
    ePswLevel e=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    long dwNumero;
    int n, nPrev, l;
    char szBuffer[256];

    if (e<=ePswService)   {
        CListCtrl& pCtrl=GetListCtrl ();
        if ((n=pCtrl.GetNextItem (-1, LVNI_SELECTED))!=-1)   {
            if (IsEmptyRow (n))    {
                nPrev=GetPrevRowNumber (n);
                if (nPrev!=-1)
                    nPrev++;
                sprintf (szBuffer, "%d", nPrev);
                usdlg.SetNumero (szBuffer);
                CLuaEditor* pLE=((CMainFrame *)AfxGetMainWnd ())->GetActiveLuaEditor ();
                if (usdlg.DoModal ()==IDOK)   {
                    /* aggiungiamo il messaggio */
                    l=atoi (usdlg.GetNumero());
                    if (EsisteNumero (l))
                        AfxMessageBox (LOADSTRING (IDS_ADAMOLANGVIEW_5));
                    else
                        if (usdlg.GetId()!=""&&EsisteStringa (usdlg.GetId()))
                            AfxMessageBox (LOADSTRING (IDS_ADAMOLANGVIEW_6));
                        else   {
                            m_tr->ScriviMessaggio (0, l, usdlg.GetId(), usdlg.GetMessaggio(), usdlg.GetDescrizione());
                            AddRow (n, usdlg.GetId(), l, usdlg.GetMessaggio(), usdlg.GetDescrizione());
                        }
                }
                if (pLE)
                    pLE->SetFocus ();
            }
            else   {
                /* leggiamo i dati dal list-view */
                GetRowData (n, &strStringa, &dwNumero, &strMessaggio, &strDescrizione);
                /* preimpostiamo i dati */
                sprintf (szBuffer, "%d", dwNumero);
                usdlg.SetNumero (szBuffer);
                usdlg.SetId (strStringa);
                usdlg.SetMessaggio (strMessaggio);
                usdlg.SetDescrizione (strDescrizione);
                usdlg.SetModify ();
                if (usdlg.DoModal ()==IDOK)   {
                    /* cancello prima il messaggio */
                    m_tr->CancellaMessaggioConNumero (0, n);
                    m_tr->ScriviMessaggio (0, n, usdlg.GetId(), usdlg.GetMessaggio(), usdlg.GetDescrizione());
                    ModifyRow (n, usdlg.GetId(), usdlg.GetMessaggio(), usdlg.GetDescrizione());
                }
            }
        }
    }
	*pResult = 0;
}

bool CLangView::IsEmptyRow (int n)
{
    CListCtrl& pCtrl=GetListCtrl ();
    bool nB=false;

    if (pCtrl.GetItemText (n, 0)==""&&pCtrl.GetItemText (n, 1)=="")
        nB=true;
    return nB;
}

long CLangView::GetPrevRowNumber (int n)
{
    CListCtrl& pCtrl=GetListCtrl ();
    long lRet=-1;
    int s;
    if ((s=pCtrl.GetNextItem (n, LVNI_ABOVE))!=-1)   {
        CString str=pCtrl.GetItemText (s, 1);
        lRet=atoi (str);
    }
    return lRet;
}

void CLangView::GetRowData (int n, CString* pStringa, long* pNumero, CString* pMessaggio, CString* pDescrizione)
{
    CListCtrl& pCtrl=GetListCtrl ();
    *pStringa=pCtrl.GetItemText (n, 0);
    *pNumero=atoi (pCtrl.GetItemText (n, 1));
    *pMessaggio=pCtrl.GetItemText (n, 2);
    *pDescrizione=pCtrl.GetItemText (n, 3);
}

int CLangView::AddMessage ()
{
    long l;
    int n, nNext, nPrev;
    char szBuffer[256];
    CListCtrl& pCtrl=GetListCtrl ();
    CUserStrings usdlg;
    if ((n=pCtrl.GetNextItem (-1, LVNI_SELECTED))!=-1)   {
        if (IsEmptyRow(n))   {
            nPrev=GetPrevRowNumber (n);
            if (nPrev!=-1)
                nPrev++;
            sprintf (szBuffer, "%d", nPrev);
            usdlg.SetNumero (szBuffer);
            CLuaEditor* pLE=((CMainFrame *)AfxGetMainWnd ())->GetActiveLuaEditor ();
            if (usdlg.DoModal ()==IDOK)   {
                /* aggiungiamo il messaggio */
                l=atoi (usdlg.GetNumero());
                if (EsisteNumero (l))
                    AfxMessageBox (LOADSTRING (IDS_ADAMOLANGVIEW_5));
                else
                    if (EsisteStringa (usdlg.GetId()))
                        AfxMessageBox (LOADSTRING (IDS_ADAMOLANGVIEW_6));
                    else   {
                        m_tr->ScriviMessaggio (0, l, usdlg.GetId(), usdlg.GetMessaggio(), usdlg.GetDescrizione());
                        AddRow (n, usdlg.GetId(), l, usdlg.GetMessaggio(), usdlg.GetDescrizione());
                    }
            }
            if (pLE)
                pLE->SetFocus ();
        }
        else   {
            nNext=GetNextValidNumber (n);
            sprintf (szBuffer, "%d", nNext);
            usdlg.SetNumero (szBuffer);
            if (usdlg.DoModal ()==IDOK)   {
                /* aggiungiamo il messaggio */
                l=atoi (usdlg.GetNumero());
                if (EsisteNumero (l))
                    AfxMessageBox (LOADSTRING (IDS_ADAMOLANGVIEW_5));
                else
                    if (EsisteStringa (usdlg.GetId()))
                        AfxMessageBox (LOADSTRING (IDS_ADAMOLANGVIEW_6));
                    else   {
                        m_tr->ScriviMessaggio (0, l, usdlg.GetId(), usdlg.GetMessaggio(), usdlg.GetDescrizione());
                        AddRow (n, usdlg.GetId(), l, usdlg.GetMessaggio(), usdlg.GetDescrizione());
                    }
            }
        }
    }
    return 0;
}

int CLangView::DeleteMessage ()
{
    CListCtrl& pCtrl=GetListCtrl ();
    CString strStringa, strMessaggio, strDescrizione;
    long dwNumero;
    int n;
    
    if ((n=pCtrl.GetNextItem (-1, LVNI_SELECTED))!=-1)   {
        if (!IsEmptyRow(n))   {
            GetRowData (n, &strStringa, &dwNumero, &strMessaggio, &strDescrizione);
            m_tr->CancellaMessaggioConNumero (0, dwNumero);
            pCtrl.DeleteItem (n);
            pCtrl.SetItemState (n, LVIS_SELECTED, LVIS_SELECTED);
        }
    }
    return 0;
}

int CLangView::ModifyMessage ()
{
    long dwNumero;
    int n;
    CListCtrl& pCtrl=GetListCtrl ();
    CString strStringa, strMessaggio, strDescrizione;
    CUserStrings usdlg;
    char szBuffer[256];

    if ((n=pCtrl.GetNextItem (-1, LVNI_SELECTED))!=-1)   {
        if (!IsEmptyRow (n))   {
            /* leggiamo i dati dal list-view */
            GetRowData (n, &strStringa, &dwNumero, &strMessaggio, &strDescrizione);
            /* preimpostiamo i dati */
            sprintf (szBuffer, "%d", dwNumero);
            usdlg.SetNumero (szBuffer);
            usdlg.SetId (strStringa);
            usdlg.SetMessaggio (strMessaggio);
            usdlg.SetDescrizione (strDescrizione);
            usdlg.SetModify ();
            CLuaEditor* pLE=((CMainFrame *)AfxGetMainWnd ())->GetActiveLuaEditor ();
            if (usdlg.DoModal ()==IDOK)   {
                /* cancello prima il messaggio */
                m_tr->CancellaMessaggioConNumero (0, n);
                m_tr->ScriviMessaggio (0, n, usdlg.GetId(), usdlg.GetMessaggio(), usdlg.GetDescrizione());
                ModifyRow (n, usdlg.GetId(), usdlg.GetMessaggio(), usdlg.GetDescrizione());
            }
            if (pLE)
                pLE->SetFocus ();
        }
        else
            AddMessage ();
    }
    return 0;
}

int CLangView::GetNextValidNumber (int n)
{
    CListCtrl& pCtrl=GetListCtrl ();
    long l=atoi (pCtrl.GetItemText (n, 1)), lDopo;
    int nB;
    while (true)   {
        if ((n=pCtrl.GetNextItem (n, LVNI_BELOW))!=-1)   {
            if (IsEmptyRow (n))   {
                nB=l+1;
                break;
            }
            else   {
                lDopo=atoi (pCtrl.GetItemText (n, 1));
                if (lDopo-l>1)   {
                    nB=l+1;
                    break;
                }
                else
                    l=lDopo;
            }
        }
		else
			break;
    }
    return nB;
}

int CLangView::AddRow (int n, CString strID, long dwNumero, CString strMessaggio, CString strDescrizione)
{
    CListCtrl& pCtrl=GetListCtrl ();
    int s, l;
    char szBuffer[256];

    s=pCtrl.InsertItem (n, strID);
    sprintf (szBuffer, "%d", dwNumero);
    pCtrl.SetItemText (s, 1, szBuffer);
    pCtrl.SetItemText (s, 2, strMessaggio);
    pCtrl.SetItemText (s, 3, strDescrizione);
    pCtrl.SetItemState (s, LVIS_SELECTED, LVIS_SELECTED);
    /* ragioniamo sulla selezione */
    if ((l=pCtrl.GetNextItem (n, LVNI_BELOW))!=-1)
        if (IsEmptyRow (l))
            pCtrl.SetItemState (l, LVIS_SELECTED, LVIS_SELECTED);
        else
            pCtrl.SetItemState (s, LVIS_SELECTED, LVIS_SELECTED);
    else
        pCtrl.SetItemState (s, LVIS_SELECTED, LVIS_SELECTED);
    return 0;
}

int CLangView::ModifyRow (int n, CString strID, CString strMessaggio, CString strDescrizione)
{
    CListCtrl& pCtrl=GetListCtrl ();
    pCtrl.SetItemText (n, 0, strID);
    pCtrl.SetItemText (n, 2, strMessaggio);
    pCtrl.SetItemText (n, 3, strDescrizione);
    return 0;
}

void CLangView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    ePswLevel e=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();

    if (e<=ePswService)   {
	    if (nChar==VK_DELETE)
            DeleteMessage ();
	    CListView::OnKeyDown(nChar, nRepCnt, nFlags);
    }
}

bool CLangView::EsisteNumero (long l)
{
    CString strStringa, strMessaggio, strDescrizione;
    return m_tr->MessaggioNumero (0, l, &strStringa, &strMessaggio, &strDescrizione);
}

bool CLangView::EsisteStringa (CString str)
{
    CString strMessaggio, strDescrizione;
    LONG dw;
    return m_tr->MessaggioStringa (0, str, &dw, &strMessaggio, &strDescrizione);
}


void CLangView::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    CString strStringa, strMessaggio, strDescrizione;
    ePswLevel e=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    long dwNumero;
	int n=pNMListView->iItem, q=pNMListView->iSubItem, nKeyState;
    char szBuffer[256];

    if (e<ePswService)   {
        if (!IsEmptyRow (n))   {
            nKeyState=GetKeyState (VK_CONTROL);
            GetRowData (n, &strStringa, &dwNumero, &strMessaggio, &strDescrizione);
            if (q>1)
                q=1;
            switch (q)   {
                case 0 :if (nKeyState&0x8000)
                        sprintf (szBuffer, "string.load (%s)", (const char*)strStringa);
                    else
                        sprintf (szBuffer, "%s", (const char*)strStringa);
                    break;
                case 1 :
                    if (nKeyState&0x8000)
                        sprintf (szBuffer, "string.load (%d)", dwNumero);
                    else
                        sprintf (szBuffer, "%d", dwNumero);
                    break;
            }
            COleDropSourceEx *pDropSource = new COleDropSourceEx;
            COleDataSource *pDataSource = new COleDataSource;
            CSharedFile file(GMEM_ZEROINIT|GMEM_DDESHARE|GMEM_MOVEABLE);
            CArchive ar(&file, CArchive::store);
            ar.WriteString(szBuffer);
            ar.Close();
            pDataSource->CacheGlobalData(CF_TEXT, file.Detach());
            pDataSource->DoDragDrop(DROPEFFECT_MOVE|DROPEFFECT_COPY,NULL, pDropSource);
            delete pDropSource;
            delete pDataSource;
        }
    }
	*pResult = 0;
}

int CALLBACK CLangView::SortColumns (LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
   // lParamSort contains a pointer to the list view control.
   // The lParam of an item is just its index.
	CLangView *pLangView = (CLangView *)lParamSort;
	if (pLangView->m_nSortColumn == 1)
		return (DWORD)lParam1 > (DWORD)lParam2;
	else
		return strcmp((const char *)lParam1, (const char *)lParam2);
}


void CLangView::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	int n, i;
	char *szBuffer;
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	m_nSortColumn = ((NM_LISTVIEW*)pNMHDR)->iSubItem;
	for (n = 0; n<GetListCtrl ().GetItemCount (); n++)   {
		if (m_nSortColumn != 1)   {
			szBuffer = new char[256];
			strcpy (szBuffer, GetListCtrl ().GetItemText (n, m_nSortColumn));
			GetListCtrl ().SetItemData (n, (DWORD)szBuffer);	
		}
		else   {
			CString strText = GetListCtrl ().GetItemText (n, m_nSortColumn);
			GetListCtrl ().SetItemData (n, atol (strText));	
		}
	}
	GetListCtrl ().SortItems (SortColumns, (LPARAM) this);
	if (m_nSortColumn != 1)
		for (i = 0; i < n; i++)
			delete (char *) GetListCtrl ().GetItemData (i);
	*pResult = 0;
}
