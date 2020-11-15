// LuaView.cpp : implementation of the CLuaView class
//

#include "stdafx.h"
#include "ide2.h"

#include "LuaDoc.h"
#include "LuaView.h"
#include "ProjectFile.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef ADAMO_PROTECTED
char limoncello = (char)0xAA;
#endif

extern UINT m_nRefreshMetrics;

/////////////////////////////////////////////////////////////////////////////
// CLuaView

IMPLEMENT_DYNCREATE(CLuaView, CView)

BEGIN_MESSAGE_MAP(CLuaView, CView)
	//{{AFX_MSG_MAP(CLuaView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_TOGGLE_BREAKPOINT, OnToggleBreakpoint)
	ON_UPDATE_COMMAND_UI(ID_TOGGLE_BREAKPOINT, OnUpdateToggleBreakpoint)
	ON_COMMAND(ID_EDIT_LIBRARY, OnEditCompleteword)
	ON_COMMAND(ID_EDIT_ENUM, OnEditShowEnums)
	ON_COMMAND(ID_EDIT_PARAM, OnEditShowParams)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_LINE, OnUpdateLine) 
	ON_COMMAND(IDM_BOOKMARK_TOGGLE, OnBookmarkToggle)
	ON_COMMAND(IDM_BOOKMARK_NEXT, OnBookmarkNext)
	ON_COMMAND(IDM_BOOKMARK_PREV, OnBookmarkPrev)
	ON_COMMAND(IDM_BOOKMARK_CLEARALL, OnBookmarkClearall)
	ON_COMMAND(ID_EDIT_COMMENT_TOGGLE, OnCommentToggle)
	ON_COMMAND(ID_EDIT_COMMENTA_BLOCCO, OnCommentBlock)
	ON_COMMAND(ID_EDIT_DECOMMENTA_BLOCCO, OnUncommentBlock)
	ON_COMMAND(ID_EDIT_MATCH_BRACE, OnMatchBrace)
	ON_COMMAND(ID_EDIT_SELECT_MATCH_BRACE, OnSelectMatchBrace)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_EDIT_FOLD_ALL, &CLuaView::OnEditFoldAll)
	ON_COMMAND(ID_EDIT_UNFOLD_ALL, &CLuaView::OnEditUnfoldAll)
	ON_REGISTERED_MESSAGE(m_nRefreshMetrics, OnRefreshMetrics)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLuaView construction/destruction

CLuaView::CLuaView()
{
	// TODO: add construction code here

}

CLuaView::~CLuaView()
{
}

BOOL CLuaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLuaView drawing

void CLuaView::OnDraw(CDC* pDC)
{
	CLuaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CLuaView printing

BOOL CLuaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	if ( m_editor.CanCutOrClear() )
		pInfo->m_pPD->m_pd.Flags &= (~PD_NOSELECTION);

	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLuaView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_editor.PreparePrint(pDC, pInfo);
}


void CLuaView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	m_editor.PrintPage(pDC, pInfo);	
}

void CLuaView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	m_editor.EndPrint(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CLuaView diagnostics

#ifdef _DEBUG
void CLuaView::AssertValid() const
{
	CView::AssertValid();
}

void CLuaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLuaDoc* CLuaView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLuaDoc)));
	return (CLuaDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLuaView message handlers

int CLuaView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
    /* creiamo l'editor */
	if ( !m_editor.Create(this, ID_EDIT_CTRL) )
		return -1;
	/* impostiamo i settaggi dell'editor */
	m_editor.SetEditorMargins();
	m_editor.SetLuaLexer();
    /* registriamo il view nella lista del view */
    GetDocument ()->SetViewPosition (((CMainFrame*)AfxGetMainWnd ())->AddEditView (NULL, this));
	return 0;
}

void CLuaView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_editor.m_hWnd) )
		m_editor.SetWindowPos(NULL, 0, 0, cx, cy, 0);
}

BOOL CLuaView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR *lpnmhdr = (LPNMHDR) lParam; 

	if (lpnmhdr->hwndFrom == m_editor.m_hWnd)
	{
		*pResult = OnSci((SCNotification*)lParam);
		return TRUE;
	}
	
	return CView::OnNotify(wParam, lParam, pResult);
}

int CLuaView::OnSci(SCNotification* pNotify)
{
	CPoint pt;
	int nLine;
    if (pNotify->nmhdr.code==2013)
        return TRUE;
	switch (pNotify->nmhdr.code)
	{
		case SCN_MARGINCLICK:
			if ( GetDocument()->IsInProject() )
			{
				if (pNotify->margin == 2)
				{
					int lineClick = m_editor.GetLineFromPosition (pNotify->position);
					m_editor.marginClick (pNotify->position, pNotify->modifiers);
					m_editor.fold (lineClick, m_editor.isFolded(lineClick));
				}
				else
				if (pNotify->margin == 1)   {
					GetCursorPos(&pt);
					ScreenToClient(&pt);
					nLine = m_editor.LineFromPoint(pt);
					ToggleBreakPoint(nLine + 1);
				}
			}
		break;

		case SCI_SETSAVEPOINT:
		case SCN_SAVEPOINTREACHED:
			GetDocument()->SetModifiedFlag(FALSE);
		break;

		case SCN_SAVEPOINTLEFT:
			GetDocument()->SetModifiedFlag(TRUE);
		break;
		case SCN_CHARADDED:
			 m_editor.CharAdded(static_cast<char>(pNotify->ch));
		break;
		case SCN_CALLTIPCLICK: {
				if (pNotify->position == 1 && m_editor.currentCallTip > 0) {
					m_editor.currentCallTip--;
					m_editor.FillFunctionDefinition();
				} else if (pNotify->position == 0 && m_editor.currentCallTip + 1 < m_editor.maxCallTips) {
					m_editor.currentCallTip++;
					m_editor.FillFunctionDefinition();
				}
			}
			break;
		case SCN_UPDATEUI :
			m_editor.bracematch ();
			m_editor.smarthilight ();
		break;
		case SCN_FOLDINGSTATECHANGED :   {
			int lineClicked = pNotify->line;
			//m_editor.fold(lineClicked, m_editor.isFolded(lineClicked));
		}
		break;
		case SCN_MODIFIED :   {
			if ((pNotify->modificationType & SC_MOD_INSERTTEXT) || (pNotify->modificationType & SC_MOD_DELETETEXT))   {
				CStringList ListaFunzioni;
				CString str;
				int nLineeModificate = pNotify->linesAdded;
				int nPosition = pNotify->position;
				int nLine = m_editor.GetLineFromPosition (nPosition) + 1;
				int nLineaCancellata = -1;
				if (pNotify->modificationType & SC_MOD_DELETETEXT)
					FunzioniCancellate (pNotify->text, ListaFunzioni);
				CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
				if (pProject)   {
					CBrowserContainer &bc = pProject->GetBrowserContainer ();
					CFunctionsContainer &fc = pProject->GetFunctionsContainer ();
					CProjectFile* pPrjFile = GetDocument ()->GetProjectFile ();
					if (pPrjFile != NULL)   {
						CString strPathName = pPrjFile->GetPathName ();
						for (POSITION pos = ListaFunzioni.GetHeadPosition (); pos; )   {
							CString strFunction = ListaFunzioni.GetNext (pos);
							fc.DeleteFunction (strFunction);
							((CMainFrame *) AfxGetMainWnd ())->DeleteHelpOnLineCustomFunction (GetDocument ()->GetProjectFile ()->GetPathName (), strFunction); 
						}
						bc.UpdateLineNumbers (strPathName, nLine, nLineeModificate, nLineaCancellata);
						fc.UpdateLineNumbers (strPathName, nLine, nLineeModificate, nLineaCancellata);

					}
				}
			}
			if (pNotify->modificationType & SC_MOD_DELETETEXT)   {
				char szBuffer[256];
				if (pNotify->length < 256)   {
					memcpy (szBuffer, pNotify->text, pNotify->length);
					szBuffer[pNotify->length] = '\0';
					if (strchr (szBuffer, '.'))
						m_editor.DestroyHelpOptionsWindow ();
				}
			}
			break;
		}
	}
	return TRUE;
}

void CLuaView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	m_editor.Copy();
}

void CLuaView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_editor.CanCutOrClear());	
}

void CLuaView::OnEditCut() 
{
	m_editor.Cut();
}

void CLuaView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_editor.CanCutOrClear());	
}

void CLuaView::OnEditClear() 
{
	m_editor.Clear();
}

void CLuaView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_editor.CanCutOrClear());	
}

void CLuaView::OnEditPaste() 
{
	m_editor.Paste();
}

void CLuaView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_editor.CanPaste());
}

void CLuaView::OnEditUndo() 
{
	m_editor.Undo();
}

void CLuaView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_editor.CanUndo());
}

void CLuaView::OnEditRedo() 
{
	m_editor.Redo();
}

void CLuaView::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_editor.CanRedo());	
}

void CLuaView::OnEditSelectAll() 
{
	m_editor.SelectAll();	
}

void CLuaView::Activate()
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
    if (pFrame)
        ((CMDIFrameWnd *)pAppFrame)->MDIActivate (pFrame);
    /* comunichiamo al template che  siamo stati attivati */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enEditDebugTemplate);
    pTmpl->SetActiveView (this);
    pTmpl->SetActiveFrame (pFrame);

	GetEditor()->GrabFocus();
}


void CLuaView::CloseFrame()
{
	CFrameWnd* pFrame = GetParentFrame();

	if (pFrame != NULL)
		pFrame->SendMessage(WM_CLOSE);
}

void CLuaView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	// use main frame dynamic accel table
	GetParentFrame()->m_hAccelTable = NULL;
    /* gestiamo la password */
    if (((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ()>ePswService)
        m_editor.SetReadOnly (true);
}

void CLuaView::ToggleBreakPoint(int nLine)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    CProjectFile* pPrj=GetDocument()->GetProjectFile();
	if ( pFrame->GetMode() == CMainFrame::modeDebug ||
		pFrame->GetMode() == CMainFrame::modeDebugBreak )
	{
		nLine = pPrj->GetNearestDebugLine(nLine);
		if ( nLine==0 )
			return;
	}

	if ( m_editor.ToggleBreakpoint(nLine) )
		pPrj->AddBreakPoint(nLine);
	else
		pPrj->RemoveBreakPoint(nLine);
	if ( pFrame->GetMode() == CMainFrame::modeDebug ||
		pFrame->GetMode() == CMainFrame::modeDebugBreak )
	{
        ((CMainFrame*)AfxGetMainWnd())->GetProject()->GetMachine ()->SendDebugData (pPrj->GetPathName(), pPrj->GetBreakPoints ());
	}
}

void CLuaView::OnToggleBreakpoint() 
{
	if ( GetDocument()->IsInProject() )
		ToggleBreakPoint(m_editor.GetCurrentLine());
}

void CLuaView::OnUpdateToggleBreakpoint(CCmdUI* pCmdUI) 
{
    ePswLevel e=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();

    if (e<=ePswService)
	    pCmdUI->Enable(	GetDocument()->IsInProject() );
    else
        pCmdUI->Enable(false);
}

void CLuaView::OnEditCompleteword() 
{
	m_editor.StartAutoComplete();
}

void CLuaView::OnEditShowEnums() 
{
	m_editor.OnEditShowEnums();
}

void CLuaView::OnEditShowParams() 
{
	m_editor.OnEditShowParams();
}

void CLuaView::OnUpdateLine(CCmdUI* pCmdUI) //@parm handle of textpane for linenumbers - see mainfrm.cpp at top
{
   CString strLine;
   long lLine = m_editor.GetCurrentLine();
   long lColumn = m_editor.GetCurrentColumn();
   long lPos = m_editor.GetCurrentPosition();
   strLine.Format(IDS_LINE_INDICATOR, lLine, lColumn, lPos);
   pCmdUI->SetText(strLine);
} 

void CLuaView::OnBookmarkToggle() 
{
    m_editor.BookmarkToggle();
}

void CLuaView::OnBookmarkNext() 
{
	m_editor.BookmarkNext(true);	
}

void CLuaView::OnBookmarkPrev() 
{
	m_editor.BookmarkNext(false);		
}

void CLuaView::OnBookmarkClearall() 
{
	m_editor.BookmarkClearAll();	
}

void CLuaView::OnDestroy() 
{
	CView::OnDestroy();
    /* deregistriamo il view nella lista del view */
    ((CMainFrame*)AfxGetMainWnd ())->RemoveEditView (GetDocument ()->GetViewPosition());
}

/*
** OnChangePsw :
*/
void CLuaView::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{

}

/*
** OnCommentToggle :
*/
void CLuaView::OnCommentToggle () 
{
	m_editor.blockcomment (eToggle);
}

/*
** OnCommentBlock :
*/
void CLuaView::OnCommentBlock () 
{
	m_editor.blockcomment (eComment);
}

/*
** OnCommentToggle :
*/
void CLuaView::OnUncommentBlock () 
{
	m_editor.blockcomment (eUncomment);
}

/*
** OnMatchBrace :
*/
void CLuaView::OnMatchBrace ()
{
	m_editor.gotobracematch (0);
}

/*
** OnSelectMatchBrace :
*/
void CLuaView::OnSelectMatchBrace ()
{
	m_editor.gotobracematch (1);
}

void CLuaView::OnEditFoldAll()
{
	// TODO: Add your command handler code here
	m_editor.foldAll (false);
}

void CLuaView::OnEditUnfoldAll()
{
	// TODO: Add your command handler code here
	m_editor.foldAll (true);
}

/*
** GotoDefinition :
*/
CLuaView* CLuaView::GotoDefinition (CString strSymbol)
{
	CLuaView* pRetLuaView = NULL;
	int nSymbolType = -1;
	/* per prima cosa leggiamo il simbolo dove c'e' il cursore */
	if (strSymbol == "")
		GetEditor ()->GetCaretSymbol (strSymbol, nSymbolType);
	/* leggiamo la function dove siamo */
	CString strFunction = GetEditor ()->GetCaretFunction ();
	/* leggiamo l'eventuale matrice */
	CString strVariable = GetEditor ()->GetCaretMatrix (strSymbol);
	/* prendiamo il browser */
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
	if (pProject)   {
		CString strModule;
		int nLine;
		CBrowserContainer& bc = pProject->GetBrowserContainer ();
		/* prendiamo la function in cui ci troviamo */
		if (bc.GetDefinitionSymbol (strSymbol, strFunction, strVariable, nSymbolType, strModule, nLine))   {
			/* se l'abbiamo trovato, andiamo a posizionarci sul modulo e alla linea ritornata */
			CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
			pRetLuaView = (CLuaView*) pMainFrame->GotoFileLine (strModule, nLine, false, false);
			/* ora selezioniamo la parola */
			pRetLuaView->GetEditor ()->SelectWord (strSymbol, nLine, 1);
		}
	}
	return pRetLuaView;
}

/*
** GotoReference :
*/
void CLuaView::GotoReference (CString strSymbol)
{
	int nSymbolType = -1;

	/* per prima cosa leggiamo il simbolo dove c'e' il cursore */
	if (strSymbol == "")
		strSymbol = GetEditor ()->GetCaretSymbol (strSymbol, nSymbolType);
	/* leggiamo la function dove siamo */
	CString strFunction = GetEditor ()->GetCaretFunction ();
	/* leggiamo l'eventuale matrice */
	CString strVariable = GetEditor ()->GetCaretMatrix (strSymbol);
	/* prendiamo il browser */
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
	if (pProject)   {
		CString strModule;
		int nLine;
		CBrowserContainer& bc = pProject->GetBrowserContainer ();
		/* prendiamo la function in cui ci troviamo */
		if (bc.GetDefinitionSymbol (strSymbol, strFunction, strVariable, nSymbolType, strModule, nLine))   {
			/* iniziamo la ricerca del reference */
			bc.BeginGetReferenceSymbol (strSymbol, nSymbolType, strFunction, strVariable);
			/* selezioniamo la parola corrente  */
			GetEditor ()->SelectCurrentSymbol ();
		}
		else   {
			/* verifichiamo prima di tutto che non siamo in presenza di un dispositivo */
			CString strMachineName = ((CAdamoCfgMachine*) pProject->GetMachine ())->GetAdamoModule ()->GetName ();
			CString strDevicePath = strMachineName + "." + strSymbol;
			if (((CAdamoCfgMachine*) pProject->GetMachine ())->GetItem (strDevicePath))   {
				/* iniziamo la ricerca del reference */
				bc.BeginGetReferenceSymbol (strSymbol, BROWSER_TYPE_SYMBOL, "", "");
				/* selezioniamo la parola corrente  */
				GetEditor ()->SelectCurrentSymbol ();
			}
			else    {
				CStringArray sa;
				int nNumElements = ((CMainFrame* )AfxGetMainWnd ())->ParseDottedString (strSymbol, sa);
				if (nNumElements == 2)   {
					if (pProject->GetSyntaxContainer ().ExistFunction (sa[0], sa[1]))   {
						/* iniziamo la ricerca del reference */
						bc.BeginGetReferenceSymbol (strSymbol, BROWSER_TYPE_SYMBOL, "", "");
						/* selezioniamo la parola corrente  */
						GetEditor ()->SelectCurrentSymbol ();
					}
				}
			}
		}
	}
}

/*
** GotoNextReference :
*/
CLuaView* CLuaView::GotoNextReference ()
{
	CLuaView* pRetLuaView = NULL;
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
	CString strSymbol;

	if (pProject)   {
		CString strModule;
		int nLine, nItem;
		/* prendiamo il container del browser */
		CBrowserContainer& bc = pProject->GetBrowserContainer ();
		/* prendiamo la function in cui ci troviamo */
		if (!bc.GetNextReferenceSymbol (strModule, nLine, strSymbol, nItem))   {
			/* andiamo a posizionarci sul modulo e alla linea ritornata */
			CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
			pRetLuaView = (CLuaView*) pMainFrame->GotoFileLine (strModule, nLine, false, false);
			/* ora selezioniamo la parola */
			pRetLuaView->GetEditor ()->SelectSymbol (strSymbol, nLine, nItem + 1);
		}
	}
	return pRetLuaView;
}

/*
** GotoPrevReference :
*/
CLuaView* CLuaView::GotoPrevReference ()
{
	CLuaView* pRetLuaView = NULL;
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
	CString strSymbol;

	if (pProject)   {
		CString strModule;
		int nLine, nItem;
		/* prendiamo il container del browser */
		CBrowserContainer& bc = pProject->GetBrowserContainer ();
		/* prendiamo la function in cui ci troviamo */
		if (!bc.GetPrevReferenceSymbol (strModule, nLine, strSymbol, nItem))   {
			/* andiamo a posizionarci sul modulo e alla linea ritornata */
			CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
			pRetLuaView = (CLuaView*) pMainFrame->GotoFileLine (strModule, nLine, false, false);
			/* ora selezioniamo la parola */
			pRetLuaView->GetEditor ()->SelectSymbol (strSymbol, nLine, nItem + 1);
		}
	}
	return pRetLuaView;
}

/*
** GotoPrevReference :
*/
int CLuaView::GetReferenceData (stBrowserDescriptor** pReferenceData)
{
	CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
	int n = 0;
	if (pProject)   {
		/* prendiamo il container del browser */
		CBrowserContainer& bc = pProject->GetBrowserContainer ();
		n = bc.GetReferenceData (pReferenceData);
	}
	return n;
}

/*
** FunzioniCancellate :
*/
int CLuaView::FunzioniCancellate (CString strText, CStringList& ListaFunzioni)
{
	int n, nState, nName, nStartPos = 0, nB = 1;
	char *pName = new char [2048];
	bool bOk;

	while ((n = strText.Find ("function", nStartPos)) != -1)   {
			n += CString ("function").GetLength ();
		nStartPos = n;
			nName = nState = 0;
			bOk = false;
		while (n < strText.GetLength ())   {
				switch (nState)   {
					case 0 :
						/* andiamo a cercare l'inizio del nome della function */
					if (strText[n] == ' ' || strText[n] == '\t')
							break;
						else
							nState = 1;
					case 1 :
						/* andiamo a cercare la fine del nome */
					if (strText[n] == ' ' || strText[n] == '\t' || strText[n] == '(')
							nState = 2;
						else   {
						pName[nName++] = strText[n];
							break;
						}
					case 2 :
						pName[nName] = '\0';
						ListaFunzioni.AddTail (pName);
						bOk = true;
						break;
				}
				if (bOk)
					break;
				n++;
			}
		}
	return 0;
}

/*
** GotoHelpSymbol :
*/
void CLuaView::GotoHelpSymbol ()
{
	CHelpOnLineView* pHelpOnlineView = ((CMainFrame*) AfxGetMainWnd ())->GetDockHelpOnLine ()->GetView ();
	CHtmlHelpOnLineView* pHelpOnlineViewHTML = ((CMainFrame*) AfxGetMainWnd ())->GetDockHelpOnLineHtml ()->GetView ();
	if (pHelpOnlineView && pHelpOnlineViewHTML)   {
		CProject* pProject = ((CMainFrame*)AfxGetMainWnd())->GetProject();
		int nIndex;
		if (pProject)   {
			CSyntaxContainer& sc = pProject->GetSyntaxContainer ();
			CString strSymbol;
			int nSymbolType;
			GetEditor ()->GetCaretSymbol (strSymbol, nSymbolType);
			if (strSymbol != "")   {
				if ((nIndex = strSymbol.Find ('.')) != -1)	 {
					/* c'e' un simbolo potenzialmente valido */
					CString strLibrary = strSymbol.Left (nIndex);
					CString strFunction = strSymbol.Right (strSymbol.GetLength () - nIndex - 1);
					if (sc.ExistFunction (strLibrary, strFunction))   {
						CString strHelp;
						/* esiste la funzione, prendiamo la stringa di help */
						sc.GetFunctionHelp (strLibrary, strFunction, strHelp);
						strHelp = strLibrary + "\\" + strHelp;
						/* visualizziamo la pagina */
						pHelpOnlineView->ViewHelpPage (strHelp);
						/* selezioniamo il pane */
						CXTPDockingPane* pHelpOnlineViewHTMLPane = ((CMainFrame*) AfxGetMainWnd ())->GetPaneManager ()->FindPane(ID_VIEW_HELP_ON_LINE_HTML);
						if (pHelpOnlineViewHTMLPane)
							pHelpOnlineViewHTMLPane->Select ();
					}
				}
			}
		}
	}
}

/*
** OnRefreshMetrics :
*/
LRESULT CLuaView::OnRefreshMetrics(WPARAM wParam, LPARAM /*lParam*/)
{
	GetEditor()->SetSkin ();
	return 0;
}
