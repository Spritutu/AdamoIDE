// DGEcMailBoxView.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrame.h"
#include "HelpOnLineView.h"
#include "AdamoLibrary.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* import directive */
#import <msxml6.dll> 
using namespace MSXML2;

/* type definition */
typedef MSXML2::IXMLDOMDocumentPtr		DocXmlPtr;
typedef MSXML2::IXMLDOMElementPtr		ElementXmlPtr;
typedef MSXML2::IXMLDOMNodePtr			NodeXmlPtr ;
typedef MSXML2::IXMLDOMNodeListPtr		NodeListXmlPtr;
typedef MSXML2::IXMLDOMNamedNodeMapPtr	NodeMapPtr;

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView

IMPLEMENT_DYNCREATE(CHelpOnLineView, CPaneView)

CHelpOnLineView::CHelpOnLineView() : m_pProject (NULL), m_bInitialized (false)
{
	m_htRoot[0] = m_htRoot[1] = m_htRoot[2] = NULL;
}

CHelpOnLineView::~CHelpOnLineView()
{
}


BEGIN_MESSAGE_MAP(CHelpOnLineView, CPaneView)
	//{{AFX_MSG_MAP(CHelpOnLineView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY (NM_CLICK, ID_TREECTRL_VIEWHELPONLINE, OnClick)
	ON_NOTIFY (NM_DBLCLK, ID_TREECTRL_VIEWHELPONLINE, OnDoubleClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView drawing

void CHelpOnLineView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView diagnostics

#ifdef _DEBUG
void CHelpOnLineView::AssertValid() const
{
	CPaneView::AssertValid();
}

void CHelpOnLineView::Dump(CDumpContext& dc) const
{
	CPaneView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView message handlers

int CHelpOnLineView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPaneView::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (CreateTree ())
		SetTreeStyle ();
	return 0;
}

void CHelpOnLineView::OnDestroy() 
{
	DeleteAllData (true);
	CPaneView::OnDestroy();
}

void CHelpOnLineView::OnInitialUpdate() 
{
	CPaneView::OnInitialUpdate();
    if (!m_bInitialized)   {
		AddRootNodes ();
	    // TODO: Add your specialized code here and/or call the base class
        m_bInitialized=true;
    }
}

void CHelpOnLineView::SetTreeStyle () 
{
    CBitmap bm;

    m_il.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    bm.LoadBitmap(IDB_BMP_HELPONLINE);
    m_il.Add(&bm, RGB(0, 128, 128));
    m_treeCtrl.SetFont (((CMainFrame *)AfxGetMainWnd ())->GetProgramFont ());
	m_treeCtrl.SetImageList (&m_il, TVSIL_NORMAL);
}

CString CHelpOnLineView::GetLanguageExtension () 
{
	return ((CMainFrame *) AfxGetMainWnd ())->GetCurrentLanguage ();
}

void CHelpOnLineView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString str;
	CPoint pt;
	GetCursorPos(&pt);
	m_treeCtrl.ScreenToClient(&pt);
	UINT nFlags;
	HTREEITEM hItem = m_treeCtrl.HitTest(pt, &nFlags);
	if (hItem)   {
		if (GetTreeType (hItem) == 0)   {
			TVITEM item;
			TCHAR szText[1024];
			item.hItem = hItem;
			item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;
			item.pszText = szText;
			item.cchTextMax = 1024;
			m_treeCtrl.GetItem (&item);
			int nImage = item.iImage;
			CString *pHTML = (CString *)item.lParam;
			CDockHelpOnLineHtml* dockhelponline = ((CMainFrame *) AfxGetMainWnd ())->GetDockHelpOnLineHtml ();
			if (pHTML != NULL)   {
				str = m_strPath + "\\" + *pHTML;
				dockhelponline->GetView ()->ViewHelpPage (str);
			}
			else   {
				str = m_strPath + "\\empty.html";
				dockhelponline->GetView ()->ViewHelpPage (str);
			}
		}
	}
}

void CHelpOnLineView::OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString str;
	CPoint pt;
	int nLine;
	GetCursorPos(&pt);
	m_treeCtrl.ScreenToClient(&pt);
	UINT nFlags;
	HTREEITEM hItem = m_treeCtrl.HitTest(pt, &nFlags);
	if (hItem)   {
		if (GetTreeType (hItem) == 1)   {
			TVITEM item;
			TVITEM parentitem;
			TCHAR szText[1024];
			item.hItem = hItem;
			item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;
			item.pszText = szText;
			item.cchTextMax = 1024;
			m_treeCtrl.GetItem (&item);
			int nImage = item.iImage;
			CString *pFunction = (CString *)item.lParam;
			HTREEITEM hParentItem = m_treeCtrl.GetParentItem (item.hItem);
			if (pFunction && hParentItem)   {
				parentitem.hItem = hParentItem;
				parentitem.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;
				parentitem.pszText = szText;
				parentitem.cchTextMax = 1024;
				m_treeCtrl.GetItem (&parentitem);
				CString* pModule = (CString *)parentitem.lParam;
				if (pModule)   {
					CFunctionsContainer& sc = m_pProject->GetFunctionsContainer ();
					if (sc.GetFunctionLine (*pModule, *pFunction, nLine))
						((CMainFrame*)AfxGetMainWnd())->GotoFileLine(*pModule, nLine, true);
				}
			}
		}	
	}	
}

void CHelpOnLineView::DeleteData (HTREEITEM hItem)
{
    HTREEITEM ht;

    if (hItem)   {
        ht=m_treeCtrl.GetChildItem(hItem);
        do   {
            if (m_treeCtrl.ItemHasChildren (ht))
                DeleteData (ht);
                CString *p = (CString *)m_treeCtrl.GetItemData (ht);
                if (p)
					delete p;
            ht=m_treeCtrl.GetNextSiblingItem (ht);
        }   while (ht);
    }
}

void CHelpOnLineView::DeleteAllData (bool bDeleteNodes)
{
	HTREEITEM hti=m_treeCtrl.GetRootItem ();
	while (hti)   { 			
		if (m_treeCtrl.ItemHasChildren(hti))
			DeleteData (hti);
		hti=m_treeCtrl.GetNextItem (hti, TVGN_NEXT);
	}	
	if (bDeleteNodes)   {
		for (int i = 0; i<3; i++)   {
			CString *p = (CString *)m_treeCtrl.GetItemData (m_htRoot[i]);
			if (p)
				delete p;
		}
		m_treeCtrl.DeleteAllItems ();
	}
}

/*
** LoadStandardFunctionsTree
*/
void CHelpOnLineView::LoadStandardFunctionsTree (CProject* pProject)
{
	CStringList strlistModules, strlistModulesDesc;
	CStringList strlistFunctions, strlistFunctionsDesc;
	CStringList strlistParameters, strlistParametersDesc;
	HTREEITEM htModules, htFunctions;
	/* settiamo il path dove risiedono i file di sintassi */
	SetPath (pProject);
	/* prendiamo il syntax container */
	CSyntaxContainer& sc = pProject->GetSyntaxContainer ();
	sc.GetLibrariesList (strlistModules, strlistModulesDesc);
	/* scorriamo la lista dei moduli */
	for (POSITION pos = strlistModules.GetHeadPosition (); pos;)   {
		CString strModule = strlistModules.GetNext (pos);
		htModules=m_treeCtrl.InsertItem (strModule, 21, 21, m_htRoot[0]);
		/* liberiamo le funzioni del giro prima */
		strlistFunctions.RemoveAll ();
		strlistFunctionsDesc.RemoveAll ();
		/* scorriamo la lista delle  funzioni */
		sc.GetFunctionsList (strModule, strlistFunctions, strlistFunctionsDesc);
		for (POSITION pos = strlistFunctions.GetHeadPosition (); pos;)   {
			CString strFunction = strlistFunctions.GetNext (pos), strHelp;
			sc.GetFunctionHelp (strModule, strFunction, strHelp);
			htFunctions=m_treeCtrl.InsertItem (strFunction, 2, 2, htModules);
			CString *pstrHelp = new CString (strModule + "\\" + strHelp);
			m_treeCtrl.SetItemData (htFunctions, (DWORD_PTR) pstrHelp);
			strlistParameters.RemoveAll ();
			strlistParametersDesc.RemoveAll ();
			sc.GetParametersList (strModule, strFunction, strlistParameters, strlistParametersDesc);
			/* scorriamo la lista dei parametri */
			POSITION posPar, posParDesc;
			posPar = strlistParameters.GetHeadPosition ();
			posParDesc = strlistParametersDesc.GetHeadPosition ();
			while (posPar)   {
				CString strParameter = strlistParameters.GetNext (posPar);
				CString strParameterDesc = strlistParametersDesc.GetNext (posParDesc);
				CString str = strParameter + " - " + strParameterDesc;
				m_treeCtrl.InsertItem (str, 3, 3, htFunctions);
			}
		}
	}
}

/*
** LoadCustomFunctionsTree :
*/
void CHelpOnLineView::LoadCustomFunctionsTree (CProject* pProject)
{
	CStringList strlistModules, strlistModulesDesc;
	CStringList strlistFunctions, strlistFunctionsDesc;
	CStringList strlistParameters, strlistParametersDesc;
	HTREEITEM htModules, htFunctions;
	/* prendiamo il functions container */
	CFunctionsContainer &fc = pProject->GetFunctionsContainer ();
	fc.GetLibrariesList (strlistModules, strlistModulesDesc);
	/* scorriamo la lista dei moduli */
	for (POSITION pos = strlistModules.GetHeadPosition (); pos;)   {
		CString strPathModule = strlistModules.GetNext (pos);
		CString strModule = ExtractModule (strPathModule);
		htModules=m_treeCtrl.InsertItem (strModule, 5, 5, m_htRoot[1]);
		CString *pstrModule = new CString (strPathModule);
		m_treeCtrl.SetItemData (htModules, (DWORD_PTR) pstrModule);
		/* scorriamo la lista delle  funzioni */
		strlistFunctions.RemoveAll ();
		strlistFunctionsDesc.RemoveAll ();
		fc.GetFunctionsList (strPathModule, strlistFunctions, strlistFunctionsDesc);
		for (POSITION pos = strlistFunctions.GetHeadPosition (); pos;)   {
			CString strFunction = strlistFunctions.GetNext (pos), strHelp;
			htFunctions=m_treeCtrl.InsertItem (strFunction, 6, 6, htModules);
			CString *pstrFunction = new CString (strFunction);
			m_treeCtrl.SetItemData (htFunctions, (DWORD_PTR) pstrFunction);
			strlistParameters.RemoveAll ();
			strlistParametersDesc.RemoveAll ();
			fc.GetParametersList (strPathModule, strFunction, strlistParameters, strlistParametersDesc);
			/* scorriamo la lista dei parametri */
			for (POSITION pos = strlistParameters.GetHeadPosition (); pos;)   {
				CString strParameter = strlistParameters.GetNext (pos);
				m_treeCtrl.InsertItem (strParameter, 7, 7, htFunctions);
			}
		}
	}
}

/*
** LoadTree :
*/
void CHelpOnLineView::LoadTree (CProject* pProject)
{
	DeleteAllData (false);
	DeleteAllNodes ();
	m_pProject = pProject; 
	LoadStandardFunctionsTree (pProject);
	LoadCustomFunctionsTree (pProject);
	LoadLibraryFunctionsTree (pProject);
}

/*
** UpdateTree :
*/
void CHelpOnLineView::UpdateTree (CProject* pProject)
{
	DeleteFunctionsTree ();
	LoadCustomFunctionsTree (pProject);
}

/*
** ExtractModule :
*/
CString CHelpOnLineView::ExtractModule (CString strPathModule)
{
	char szBuffer[1024];
	szBuffer[0] = '\0';
	strcpy (szBuffer, strPathModule);
	const char *p = strrchr (szBuffer, '\\');
	if (p)
		p++;
	else
		p = szBuffer;
	return CString (p);
}

/*
** GetTreeType :
*/
int CHelpOnLineView::GetTreeType (HTREEITEM ht)
{
	TVITEM item;
	HTREEITEM htRoot = ht;
	TCHAR szText[1024];
	int nTreeType = 0;
	while (htRoot)   {
		ht = htRoot;
		htRoot = m_treeCtrl.GetParentItem (ht);
	}
	item.hItem = ht;
	item.mask = TVIF_TEXT | TVIF_PARAM;
	item.pszText = szText;
	item.cchTextMax = sizeof (szText);
	m_treeCtrl.GetItem (&item);
	CString* pTreeType = (CString *)item.lParam;
	if (pTreeType)
		nTreeType = *pTreeType == "0" ? 0 : 1;
	return nTreeType;
}

/*
** DeleteFunctionsTree :
*/
void CHelpOnLineView::DeleteFunctionsTree ()
{
	if (m_treeCtrl.ItemHasChildren(m_htRoot[1]))   {
		DeleteData (m_htRoot[1]);
		HTREEITEM ht=m_treeCtrl.GetChildItem(m_htRoot[1]), htNext;
        do   {
			htNext = m_treeCtrl.GetNextSiblingItem (ht);
			m_treeCtrl.DeleteItem (ht);
			ht = htNext;
        }   while (ht);
	}
}

/*
** DeleteCustomFunctions :
*/
void CHelpOnLineView::DeleteCustomFunction (CString strPath, CString strFunction)
{
	HTREEITEM hti=m_htRoot[1], htModules, htFunctions;
	if (m_treeCtrl.ItemHasChildren(hti))   {
        htModules=m_treeCtrl.GetChildItem(hti);
        do   {
            CString *p = (CString *)m_treeCtrl.GetItemData (htModules);
            if (p)
				if (*p == strPath)   {
					htFunctions=m_treeCtrl.GetChildItem(htModules);
					do   {
						CString *p = (CString *)m_treeCtrl.GetItemData (htFunctions);
						if (p)
							if (*p == strFunction)   {
								m_treeCtrl.DeleteItem (htFunctions);
								return;
							}
						htFunctions=m_treeCtrl.GetNextSiblingItem (htFunctions);
					} while (htFunctions);
				}
            htModules=m_treeCtrl.GetNextSiblingItem (htModules);
        }   while (htModules);
	}
}

/*
** DeleteLibraryFunctions :
*/
void CHelpOnLineView::DeleteLibraryFunctions ()
{
	if (m_treeCtrl.ItemHasChildren(m_htRoot[2]))   {
		DeleteData (m_htRoot[2]);
		HTREEITEM ht=m_treeCtrl.GetChildItem(m_htRoot[2]), htNext;
        do   {
			htNext = m_treeCtrl.GetNextSiblingItem (ht);
			m_treeCtrl.DeleteItem (ht);
			ht = htNext;
        }   while (ht);
	}
}

/*
** AddLibraryFunction :
*/
void CHelpOnLineView::AddLibraryFunction (CString strLibrary, CString strModule, list<CAdamoFunction*>* pLibraryFunctions)
{
    list<CAdamoFunction*>::iterator i;
    list<CAdamoParameter*>::iterator q;
	CAdamoFunction* pFunction;
	CAdamoParameter* pParameter;
	HTREEITEM htLibrary = NULL, htModules = NULL, htFunctions;

    if (pLibraryFunctions->size())   {
        i = pLibraryFunctions->begin();
		/* scorriamo la  lista delle functions */
        while (i != pLibraryFunctions->end())   {
			if (htModules == NULL)   {
				/* creiamo nodo libreria e modulo */
				htLibrary = GetTreeItemLibrary (strLibrary);
				if (htLibrary == NULL)
					htLibrary=m_treeCtrl.InsertItem (strLibrary, 21, 21, m_htRoot[2]);
				htModules=m_treeCtrl.InsertItem (strModule, 5, 5, htLibrary);
			}
			pFunction = *i;
			htFunctions=m_treeCtrl.InsertItem (pFunction->GetName (), 6, 6, htModules);
			/* scorriamo la lista dei parametri */
			list<CAdamoParameter *>*pListParameters =  pFunction->GetListParameters ();
			if (pListParameters->size ())   {
				q = pListParameters->begin();
				while (q != pListParameters->end())   {
					pParameter = *q;
					m_treeCtrl.InsertItem (pParameter->m_strName, 7, 7, htFunctions);
					q++;
				}
			}
			i++;
		}
	}
}

/*
** GetTreeItemLibrary :
*/
HTREEITEM CHelpOnLineView::GetTreeItemLibrary (CString strLibrary)
{
    HTREEITEM ht=m_treeCtrl.GetChildItem(m_htRoot[2]), htRet = NULL;
    do   {
		CString str = m_treeCtrl.GetItemText (ht);
		if (str == strLibrary)   {
			htRet = ht;
			break;
		}
        ht=m_treeCtrl.GetNextSiblingItem (ht);
    }   while (ht);
	return htRet;
}

/*
** LoadLibraryFunctionsTree :
*/
void CHelpOnLineView::LoadLibraryFunctionsTree (CProject* pPrj)
{
	CString strBasePathName = GETWS ()->GetBasePathName (), strDescription;
	int nVersion;
	/* aggiungiamo ora le functions di libreria */
	for (POSITION pos = pPrj->GetLibraries ().GetHeadPosition (); pos; )   {
		CString strLibrary = pPrj->GetLibraries ().GetNext (pos);
		CString strPathName = strBasePathName + "\\" + DIR_LIBRARIES + "\\" + strLibrary;
		if (CAdamoLuaLibrary::IsLuaLibraryFile (strPathName, nVersion, strDescription))   {
			/* apriamo il file e leggiamo l'xml */
			CAdamoLuaLibrary* pLuaLibrary = new CAdamoLuaLibrary;
			if (!pLuaLibrary->Load (strPathName))   {
				/* caricato la libreria, ora accediamo ai moduli */
				for (int i = 0; i<pLuaLibrary->GetLibraryModulesCount (); i++)   {
					CAdamoLuaLibraryModule* pLuaModule = pLuaLibrary->GetLibraryModules (i);
					AddLibraryFunction (strLibrary, pLuaModule->GetName (), pLuaModule->GetListFunctions ());
				}
			}
			delete pLuaLibrary;
		}
	}
}

/*
** AddRootNodes :
*/
void CHelpOnLineView::AddRootNodes ()
{
	/* syntax functions */
	if (m_htRoot[0] == NULL)   {
		CString str = LOADSTRING (IDS_HELPONLINE_1);
		m_htRoot[0]=m_treeCtrl.InsertItem (str, 0, 0, TVI_ROOT);
		CString *pstrRoot = new CString ("0");
		m_treeCtrl.SetItemData (m_htRoot[0], (DWORD_PTR) pstrRoot);
	}
	/* Project functions */
	if (m_htRoot[1] == NULL)   {
		CString str = LOADSTRING (IDS_HELPONLINE_2);
		m_htRoot[1]=m_treeCtrl.InsertItem (str, 4, 4, TVI_ROOT);
		CString *pstrRoot = new CString ("1");
		m_treeCtrl.SetItemData (m_htRoot[1], (DWORD_PTR) pstrRoot);
	}
	/* librerie */
	if (m_htRoot[2] == NULL)   {
		CString str = LOADSTRING (IDS_HELPONLINE_3);
		m_htRoot[2]=m_treeCtrl.InsertItem (str, 1, 1, TVI_ROOT);
		CString *pstrRoot = new CString ("2");
		m_treeCtrl.SetItemData (m_htRoot[2], (DWORD_PTR) pstrRoot);
	}
}

/*
** DeleteAllNodes :
*/
void CHelpOnLineView::DeleteAllNodes ()
{
	for (int i = 0; i < 3; i++)   {
		if (m_treeCtrl.ItemHasChildren(m_htRoot[i]))   {
			HTREEITEM ht=m_treeCtrl.GetChildItem(m_htRoot[i]), htNext;
			do   {
				htNext = m_treeCtrl.GetNextSiblingItem (ht);
				m_treeCtrl.DeleteItem (ht);
				ht = htNext;
			}   while (ht);
		}
	}
}

/*
** SetPath :
*/
void CHelpOnLineView::SetPath (CProject* pProject)
{
	CSyntaxContainer& sc = pProject->GetSyntaxContainer ();
	m_strPath = sc.GetWorkPath ();
}

/*
** SetPath :
*/
void CHelpOnLineView::ViewHelpPage (CString strHelp)
{
	CDockHelpOnLineHtml* dockhelponline = ((CMainFrame *) AfxGetMainWnd ())->GetDockHelpOnLineHtml ();
	CString str = m_strPath + "\\" + strHelp;
	CHtmlHelpOnLineView *pView = dockhelponline->GetView();
	if (pView)   {
		dockhelponline->GetView ()->ViewHelpPage (str);
	}
}

/*
** CreateTree :
*/
int CHelpOnLineView::CreateTree ()
{
	if (m_treeCtrl.GetSafeHwnd() == 0)  	{
		if (!m_treeCtrl.Create ( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT
								| TVS_SHOWSELALWAYS, CRect(0,0, 0, 0), this, ID_TREECTRL_VIEWHELPONLINE))   {
			TRACE0("Unable to create tree control.\n");
			return 0;
		}
	}
	return 1;
}

/*
** OnSize :
*/
void CHelpOnLineView::OnSize(UINT nType, int cx, int cy) 
{
	CPaneView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_treeCtrl.GetSafeHwnd ()) )
		m_treeCtrl.SetWindowPos (NULL, 0, 0, cx, cy, 0);
}


/*
** RefreshMetrics :
*/
void CHelpOnLineView::RefreshMetrics(AppTheme nTheme)
{
	CPaneView::RefreshMetrics(nTheme);
	switch (nTheme)
	{
		case themeVS2010: 
			m_treeCtrl.SetTheme(xtpReportThemeVisualStudio2012Blue); 
			m_treeCtrl.ModifyStyle(TVS_HASLINES, 0);

			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
		case themeVS2012Light: 
			m_treeCtrl.SetTheme(xtpReportThemeVisualStudio2012Light); 
			m_treeCtrl.ModifyStyle(TVS_HASLINES, 0);

			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
		case themeVS2012Dark: 
			m_treeCtrl.SetTheme(xtpReportThemeVisualStudio2012Dark); 
			m_treeCtrl.ModifyStyle(TVS_HASLINES, 0);
			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue: 
			m_treeCtrl.SetTheme (xtpControlThemeVisualStudio2015);
			m_treeCtrl.ModifyStyle (TVS_HASLINES, 0);
			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
	}	
}
