// AdamoSEView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoTemplates.h"
#include "adamoSEDoc.h"
#include "AdamoSEView.h"
#include "AdamoSEListView.h"
#include "dlgfinderrors.h"
#include "DlgIDRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern UINT m_nRefreshMetrics;

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEView

IMPLEMENT_DYNCREATE(CAdamoSEView, CView)

CAdamoSEView::CAdamoSEView() : m_pDlgErrors (NULL), m_bEnableFilter (false), m_theme (themeNone)
{
}

CAdamoSEView::~CAdamoSEView()
{
}


BEGIN_MESSAGE_MAP(CAdamoSEView, CView)
	//{{AFX_MSG_MAP(CAdamoSEView)
	ON_WM_DESTROY()
	ON_UPDATE_COMMAND_UI(IDM_SE_REFRESH, OnUpdateSeRefresh)
	ON_COMMAND(IDM_SE_REFRESH, OnSeRefresh)
    ON_MESSAGE (WM_OPENDIALOGFINDERRORS, OnOpenDialogFindErrors)
    ON_MESSAGE (WM_CLOSEDIALOGFINDERRORS, OnCloseDialogFindErrors)
	ON_REGISTERED_MESSAGE(m_nRefreshMetrics, OnRefreshMetrics)
	ON_WM_SIZE ()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEView drawing

void CAdamoSEView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEView diagnostics

#ifdef _DEBUG
void CAdamoSEView::AssertValid() const
{
	CView::AssertValid();
}

void CAdamoSEView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEView message handlers
void CAdamoSEView::Activate()
{
	CFrameWnd* pFrame = GetParentFrame();

	if (pFrame != NULL)
		pFrame->ActivateFrame();
	else
		TRACE0("Error: Can not find a frame for document to activate.\n");

	CFrameWnd* pAppFrame;
	if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
	{
		ASSERT_KINDOF(CFrameWnd, pAppFrame);
		pAppFrame->ActivateFrame();
	}
    /* comunichiamo al template che  siamo stati attivati */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enShowErrorsTemplate);
    pTmpl->SetActiveView (this);
}

int CAdamoSEView::CreateImageList ()
{
    m_bmpTblEH=new CBitmap;    
    m_bmpTblEI=new CBitmap;
    m_bmpTblEC=new CBitmap;
    m_bmpTblEISO=new CBitmap;
    m_bmpTblEEvent=new CBitmap;
    m_bmpTblEAll=new CBitmap;
    m_bmpTblEH->LoadBitmap(IDB_CAPHW);
    m_bmpTblEI->LoadBitmap(IDB_CAPINTERRORS);
    m_bmpTblEC->LoadBitmap(IDB_CAPCYCERRORS);
    m_bmpTblEISO->LoadBitmap(IDB_CAPISOERRORS);
    m_bmpTblEEvent->LoadBitmap(IDB_CAPEVENTS);
    m_bmpTblEAll->LoadBitmap(IDB_CAPALLERR);
    m_tbiml.Create (16, 16, ILC_MASK, 0, 1);
    m_tbiml.Add (m_bmpTblEH, RGB(0, 128,128));
    m_tbiml.Add (m_bmpTblEI, RGB(0, 128,128));
    m_tbiml.Add (m_bmpTblEC, RGB(0, 128,128));
    m_tbiml.Add (m_bmpTblEISO, RGB(0, 128,128));
    m_tbiml.Add (m_bmpTblEEvent, RGB(0, 128,128));
    m_tbiml.Add (m_bmpTblEAll, RGB(0, 128,128));
    return 0;
}


void CAdamoSEView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
    _delete (m_bmpTblEH);
    _delete (m_bmpTblEI);
    _delete (m_bmpTblEC);
    _delete (m_bmpTblEISO);
    _delete (m_bmpTblEEvent);
    _delete (m_bmpTblEAll);
}

void CAdamoSEView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
    CreateImageList ();
    AddView (RUNTIME_CLASS (CAdamoSEListView), LOADSTRING (IDS_ADAMOERRORSTYPE_1), 0);
    AddView (RUNTIME_CLASS (CAdamoSEListView), LOADSTRING (IDS_ADAMOERRORSTYPE_2), 1);
    AddView (RUNTIME_CLASS (CAdamoSEListView), LOADSTRING (IDS_ADAMOERRORSTYPE_3), 2);
    AddView (RUNTIME_CLASS (CAdamoSEListView), LOADSTRING (IDS_ADAMOERRORSTYPE_4), 3);
    AddView (RUNTIME_CLASS (CAdamoSEListView), LOADSTRING (IDS_ADAMOERRORSTYPE_5), 4);
    AddView (RUNTIME_CLASS (CAdamoSEListView), LOADSTRING (IDS_ADAMOERRORSTYPE_0), 5);
	((CAdamoSEListView*)GetView (0))->GetListCtrl ().SetRedraw (FALSE);
	((CAdamoSEListView*)GetView (1))->GetListCtrl ().SetRedraw (FALSE);
	((CAdamoSEListView*)GetView (2))->GetListCtrl ().SetRedraw (FALSE);
	((CAdamoSEListView*)GetView (3))->GetListCtrl ().SetRedraw (FALSE);
	((CAdamoSEListView*)GetView (4))->GetListCtrl ().SetRedraw (FALSE);
	((CAdamoSEListView*)GetView (5))->GetListCtrl ().SetRedraw (FALSE);

    ((CAdamoSEListView*)GetView (0))->SetAdamoSEDoc ((CAdamoSEDoc*)GetDocument ());
    ((CAdamoSEListView*)GetView (1))->SetAdamoSEDoc ((CAdamoSEDoc*)GetDocument ());
    ((CAdamoSEListView*)GetView (2))->SetAdamoSEDoc ((CAdamoSEDoc*)GetDocument ());
    ((CAdamoSEListView*)GetView (3))->SetAdamoSEDoc ((CAdamoSEDoc*)GetDocument ());
    ((CAdamoSEListView*)GetView (4))->SetAdamoSEDoc ((CAdamoSEDoc*)GetDocument ());
    ((CAdamoSEListView*)GetView (5))->SetAdamoSEDoc ((CAdamoSEDoc*)GetDocument ());
    ((CAdamoSEListView*)GetView (0))->SetAdamoSEView (this);
    ((CAdamoSEListView*)GetView (1))->SetAdamoSEView (this);
    ((CAdamoSEListView*)GetView (2))->SetAdamoSEView (this);
    ((CAdamoSEListView*)GetView (3))->SetAdamoSEView (this);
    ((CAdamoSEListView*)GetView (4))->SetAdamoSEView (this);
    ((CAdamoSEListView*)GetView (5))->SetAdamoSEView (this);
    ((CAdamoSEListView*)GetView (0))->SetErrorType (TE_HARDWARE);
    ((CAdamoSEListView*)GetView (1))->SetErrorType (TE_INTERPRETER);
    ((CAdamoSEListView*)GetView (2))->SetErrorType (TE_CYCLE);
    ((CAdamoSEListView*)GetView (3))->SetErrorType (TE_ISO);
    ((CAdamoSEListView*)GetView (4))->SetErrorType (TE_EVENT);
    ((CAdamoSEListView*)GetView (0))->CreateColumnHeader ();
    ((CAdamoSEListView*)GetView (1))->CreateColumnHeader ();
    ((CAdamoSEListView*)GetView (2))->CreateColumnHeader ();
    ((CAdamoSEListView*)GetView (3))->CreateColumnHeader ();
    ((CAdamoSEListView*)GetView (4))->CreateColumnHeader ();
    ((CAdamoSEListView*)GetView (5))->CreateColumnHeader ();
    ((CAdamoSEListView*)GetView (5))->GetListCtrl ().SetFocus ();
	LoadErrors ();
	((CAdamoSEListView*)GetView (0))->GetListCtrl ().SetRedraw (TRUE);
	((CAdamoSEListView*)GetView (1))->GetListCtrl ().SetRedraw (TRUE);
	((CAdamoSEListView*)GetView (2))->GetListCtrl ().SetRedraw (TRUE);
	((CAdamoSEListView*)GetView (3))->GetListCtrl ().SetRedraw (TRUE);
	((CAdamoSEListView*)GetView (4))->GetListCtrl ().SetRedraw (TRUE);
	((CAdamoSEListView*)GetView (5))->GetListCtrl ().SetRedraw (TRUE);
	SetTabStyle ();
	OnRefreshMetrics (((CIdeApp *) AfxGetApp ())->GetTheme (), 0);
	m_wndTabControl.SetCurSel (5);
}

LRESULT CAdamoSEView::OnOpenDialogFindErrors(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your command handler code here
    if (!m_pDlgErrors)   {
        m_pDlgErrors = new CDlgFindErrors(this);
        if (m_pDlgErrors != NULL)   {
            m_pDlgErrors->Create(CDlgFindErrors::IDD);
            m_pDlgErrors->SetSEView (this);
        }
    }
    return 0;
}

void CAdamoSEView::OnCloseSEView () 
{
	// TODO: Add your command update UI handler code here
    _delete (m_pDlgErrors);
}

LRESULT CAdamoSEView::OnCloseDialogFindErrors (WPARAM wParam, LPARAM lParam)
{
    OnCloseSEView ();
    return 0;
}

int CAdamoSEView::Find (const char* str, int nDirection, bool bMatchWholeWord, bool bUpperCase)
{
	CString strItem;
	int nItem = GetActiveItemSelected (), nItemNumber;
	bool bFound;

	/* se non c'e' niente di selezionato partiamo dalla prima riga */
	if (nItem == -1)
		nItem = 0;
	else
		nDirection ? nItem-- : nItem++;
	/* prendiamo il view corrente */
	CAdamoSEListView *pSEListView = (CAdamoSEListView*) GetActiveView ();
	if (pSEListView)   {
		switch (pSEListView->GetErrorType ())   {
			case TE_HARDWARE     :
				nItemNumber = 7;
				break;
			case TE_INTERPRETER  :
				nItemNumber = 7;
				break;
			case TE_CYCLE        :
				nItemNumber = 7;
				break;
			case TE_ISO          :
				nItemNumber = 7;
				break;
			case TE_EVENT        :
				nItemNumber = 3;
				break;
			case TE_ALL			 :
				nItemNumber = 5;
				break;
		};
		bFound = false;
		while (nItem < pSEListView->GetListCtrl ().GetItemCount () && !bFound)   {
			for (int i = 0; i<nItemNumber; i++)   {
				strItem = pSEListView->GetListCtrl ().GetItemText (nItem, i);
				if (((CMainFrame *)AfxGetMainWnd ())->MatchString (str, strItem, bMatchWholeWord, bUpperCase))   {
					/* selezioniamo l'item trovato */
					pSEListView->GetListCtrl ().SetItemState (nItem, LVNI_SELECTED | LVNI_FOCUSED, LVNI_SELECTED | LVNI_FOCUSED);
					/* job done, eschenderian */
					bFound = true;
					break;
				}
			}
			nItem++;
		}
	}
    return 0;
}

/*
** GetActiveErrorType
*/
int CAdamoSEView::GetActiveErrorType ()
{ 
    return ((CAdamoSEListView*) GetActiveView ())->GetErrorType ();
}

/*
** GetActiveItemSelected
*/
int CAdamoSEView::GetActiveItemSelected ()
{ 
    return ((CAdamoSEListView*) GetActiveView ())->GetSelection (); 
}

/*
** SetActiveItemSelected
*/
void CAdamoSEView::SetActiveItemSelected (int nItem)   
{ 
    ((CAdamoSEListView*) GetActiveView ())->SetSelection (nItem); 
}

/*
** SetActiveItemSelected
*/
void CAdamoSEView::Reload (int nMonth)   
{
	DeleteAllItems ();
	LoadErrors (nMonth);
}

/*
** DeleteAllItems :
*/
void CAdamoSEView::DeleteAllItems ()   
{
    ((CAdamoSEListView*)GetView (0))->ClearAllItems ();
    ((CAdamoSEListView*)GetView (1))->ClearAllItems ();
    ((CAdamoSEListView*)GetView (2))->ClearAllItems ();
    ((CAdamoSEListView*)GetView (3))->ClearAllItems ();
    ((CAdamoSEListView*)GetView (4))->ClearAllItems ();
    ((CAdamoSEListView*)GetView (5))->ClearAllItems ();
}

/*
** OnCreate :
*/
int CAdamoSEView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_wndTabControl.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, CRect(0, 0, 0, 0), this, IDC_TABCONTROL);
	return 0;
}

/*
** LoadErrors :
*/
int CAdamoSEView::LoadErrors (int nMonth) 
{
	struct stAdamoErrore AE;
	BYTE *pAE, *p;
	CAdamoSEDoc* pDoc = (CAdamoSEDoc*)GetDocument ();
	int nRecord;
	int nRecordLetti, n, s;

	/* clessidra */
	BeginWaitCursor ();
	m_nMonth = nMonth;
	/* allochiamo il buffer */
	pAE = new BYTE [COMMBUF];
	/* apriamo per prima cosa il file */
	nRecord = pDoc->OpenLogFile (nMonth);
	if (nRecord > 0)   {
		/* leggiamo i blocchi di frame */
		n = 0;
		while (n < nRecord)   {
			nRecordLetti = pDoc->ReadLogFile (n, 0, pAE);
			p = pAE; s = 0;
			while (s++ < nRecordLetti)   {
				strcpy (AE.strErrore, (char *) p); p+=strlen ((const char *) p) + 1;
				strcpy (AE.szTaskName, (char *) p); p+=strlen ((const char *) p) + 1;
				memcpy (&AE.nDeleteError, (char *)p, 6*sizeof (int)); p+=6*sizeof (int);
				AE.dt = *(time_t *) p; p+=sizeof (time_t);
				AddError (&AE, 0);
			}
			n+=nRecordLetti;
		}
		pDoc->CloseLogFile ();
	}
	else
		if (nRecord == -1)
			AfxMessageBox (LOADSTRING (IDS_ADAMOSELISTVIEW_28), MB_ICONSTOP);
	delete pAE;
	EndWaitCursor ();
	return 0;
}

/*
** AddError :
*/
void CAdamoSEView::AddError (stAdamoErrore *p, int nItem)
{
	bool bCarica = false, bAdd = false;

	if (m_bEnableFilter)
		if (p->nErrore >= m_nFilterDa && p->nErrore <= m_nFilterA)
			bCarica = true;
		else
			;
	else
		bCarica = true;
	if (bCarica)   {
		switch (p->nTypeError)   {
			case TE_HARDWARE     :
				((CAdamoSEListView *)GetView (0))->AddError (p, nItem); bAdd = true;
				break;
			case TE_INTERPRETER  :
				((CAdamoSEListView *)GetView (1))->AddError (p, nItem); bAdd = true;
				break;
			case TE_CYCLE        :
				((CAdamoSEListView *)GetView (2))->AddError (p, nItem); bAdd = true;
				break;
			case TE_ISO          :
				((CAdamoSEListView *)GetView (3))->AddError (p, nItem); bAdd = true;
				break;
			case TE_EVENT        :
				((CAdamoSEListView *)GetView (4))->AddError (p, nItem); bAdd = true;
				break;
		};
		if (bAdd)
			((CAdamoSEListView *)GetView (5))->AddError (p, nItem); bAdd = true;
	}
}

/*
** OnSeFilterErrors :
*/
void CAdamoSEView::OnSeFilterErrors() 
{
	// TODO: Add your command handler code here
    CDlgIDRange dlg;

    if (dlg.DoModal ()==IDOK)   {
		/* per prima cosa seghiamo tutti i messaggi */
		SetIdFilter (dlg.GetDaID (), dlg.GetAID ());
		EnableIdFilter ();
		Reload (m_nMonth);
    }
}

/*
** OnSeClearFilter :
*/
void CAdamoSEView::OnSeClearFilter() 
{
	// TODO: Add your command handler code here
	SetIdFilter (-1, -1);
	DisableIdFilter ();
	Reload (m_nMonth);
}

/*
** OnSeGotoLine :
*/
void CAdamoSEView::OnSeGotoLine() 
{
	// TODO: Add your command handler code here
	switch (GetActiveErrorType ())   {
		case TE_INTERPRETER :
			((CAdamoSEListView *)GetView (1))->GotoLine ();
			break;
		case TE_ALL :
			((CAdamoSEListView *)GetView (5))->GotoLine ();
			break;
	}
}

/*
** OnSeRefresh :
*/
void CAdamoSEView::OnSeRefresh() 
{
	OnSeClearFilter ();
}

/*
** OnUpdateSeRefresh :
*/
void CAdamoSEView::OnUpdateSeRefresh(CCmdUI* pCmdUI) 
{
}

/*
** SetTabStyle :
*/
void CAdamoSEView::SetTabStyle () 
{
	m_wndTabControl.GetImageManager()->SetIcons (m_tbiml, NULL, 0, CSize(16, 16), xtpImageNormal);
	switch (((CIdeApp *) AfxGetApp ())->GetTheme ())   {
		case themeVS2010 :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2010);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2010);
			break;
		case themeVS2012Light :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2012);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2012);
			break;
		case themeVS2012Dark :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2012Dark);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2012);
			break;
		case themeVS2015Light :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2015);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2015);
			break;
		case themeVS2015Dark :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2015);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2015);
			break;
		case themeVS2015Blue :
			m_wndTabControl.GetPaintManager ()->SetColor (xtpTabColorVisualStudio2015);
			m_wndTabControl.GetPaintManager ()->SetAppearance (xtpTabAppearanceVisualStudio2015);
			break;
	}
}

/*
** OnSize :
*/
void CAdamoSEView::OnSize (UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_wndTabControl.GetSafeHwnd())
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
}

/*
** AddView :
*/
BOOL CAdamoSEView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon)
{
	CCreateContext contextT;
	contextT.m_pCurrentDoc	 = GetDocument();
	contextT.m_pNewViewClass   = pViewClass;
	contextT.m_pNewDocTemplate = GetDocument()->GetDocTemplate();

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	dwStyle &= ~WS_BORDER;

	int nTab = m_wndTabControl.GetItemCount();

	// Create with the right size (wrong position)
	CRect rect(0, 0, 0, 0);
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, &m_wndTabControl, (AFX_IDW_PANE_FIRST + nTab),
					  &contextT))
	{
		TRACE0("Warning: couldn't create client tab for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}
	m_wndTabControl.InsertItem(nTab, lpszTitle, pWnd->GetSafeHwnd(), nIcon);

	pWnd->SetOwner(this);

	return TRUE;
}

/*
** GetView :
*/
CView* CAdamoSEView::GetView (int nItem)
{
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetItem(nItem)->GetHandle()));
	ASSERT_KINDOF(CView, pView);
	return pView;
}

/*
** GetActiveView :
*/
CView* CAdamoSEView::GetActiveView ()
{
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pView);
	return pView;
}

/*
** OnRefreshMetrics :
*/
LRESULT CAdamoSEView::OnRefreshMetrics(WPARAM wParam, LPARAM /*lParam*/)
{
	if (m_theme != wParam)   {
		SetTabStyle ();
		for (int i = 0; i<m_wndTabControl.GetItemCount (); i++)   {
			((CAdamoSEListView*)GetView (i))->GetListCtrl ().GetSEHeaderCtrl ().SetSkin ((AppTheme)wParam);
			((CAdamoSEListView*)GetView (i))->GetListCtrl ().SetSkin ((AppTheme)wParam);
			((CAdamoSEListView*)GetView (i))->GetListCtrl ().Invalidate ();
		}
		m_wndTabControl.Invalidate ();
	}
	m_theme = (AppTheme) wParam;
	return 0;
}
