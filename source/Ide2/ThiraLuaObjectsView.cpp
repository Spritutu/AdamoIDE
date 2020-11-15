// DGEcMailBoxView.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrame.h"
#include "ThiraLuaObjectsView.h"
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

IMPLEMENT_DYNCREATE(CThiraLuaObjectsView, CPaneView)

CThiraLuaObjectsView::CThiraLuaObjectsView() : m_pProject (NULL), m_bInitialized (false)
{
	m_htRoot[0] = m_htRoot[1] = NULL;
}

CThiraLuaObjectsView::~CThiraLuaObjectsView()
{
}


BEGIN_MESSAGE_MAP(CThiraLuaObjectsView, CPaneView)
	//{{AFX_MSG_MAP(CHelpOnLineView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY (NM_DBLCLK, ID_TREECTRL_VIEWLUAOBJECTS, OnDoubleClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView drawing

void CThiraLuaObjectsView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView diagnostics

#ifdef _DEBUG
void CThiraLuaObjectsView::AssertValid() const
{
	CPaneView::AssertValid();
}

void CThiraLuaObjectsView::Dump(CDumpContext& dc) const
{
	CPaneView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView message handlers
int CThiraLuaObjectsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPaneView::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (CreateTree ())
		SetTreeStyle ();
	return 0;
}

void CThiraLuaObjectsView::OnInitialUpdate() 
{
	CPaneView::OnInitialUpdate();
    if (!m_bInitialized)   {
		AddRootNodes ();
		// TODO: Add your specialized code here and/or call the base class
        m_bInitialized=true;
    }
}

/*
** CreateTree :
*/
int CThiraLuaObjectsView::CreateTree ()
{
	if (m_treeCtrl.GetSafeHwnd() == 0)  	{
		if (!m_treeCtrl.Create ( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT
								| TVS_SHOWSELALWAYS, CRect(0,0, 0, 0), this, ID_TREECTRL_VIEWLUAOBJECTS))   {
			TRACE0("Unable to create tree control.\n");
			return 0;
		}
	}
	return 1;
}

/*
** SetTreeStyle :
*/
void CThiraLuaObjectsView::SetTreeStyle () 
{
    CBitmap bm;

    m_il.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    bm.LoadBitmap(IDB_BMP_HELPONLINE);
    m_il.Add(&bm, RGB(0, 128, 128));
    m_treeCtrl.SetFont (((CMainFrame *)AfxGetMainWnd ())->GetProgramFont ());
	m_treeCtrl.SetImageList (&m_il, TVSIL_NORMAL);
}

/*
** OnDestroy :
*/ 
void CThiraLuaObjectsView::OnDestroy() 
{
	DeleteAllData (true);
	CPaneView::OnDestroy();
}

/*
** DeleteAllData :
*/
void CThiraLuaObjectsView::DeleteAllData (bool bDeleteNodes)
{
	HTREEITEM hti=m_treeCtrl.GetRootItem ();
	while (hti)   { 			
		if (m_treeCtrl.ItemHasChildren(hti))
			//itero l'abero
			DeleteData (hti);
		hti=m_treeCtrl.GetNextItem (hti, TVGN_NEXT);
	}
	if (bDeleteNodes)   {
		for (int i = 0; i<2; i++)   {
			CString *p = (CString *)m_treeCtrl.GetItemData (m_htRoot[i]);
			if (p)
				delete p;
		}
		m_treeCtrl.DeleteAllItems ();
	}
}

/*
** DeleteData :
*/
void CThiraLuaObjectsView::DeleteData (HTREEITEM hItem)
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

/*
** LoadTree :
*/
void CThiraLuaObjectsView::LoadTree (CProject* pProject)
{
	DeleteAllData (false);
	DeleteAllNodes ();
	m_pProject = pProject; 
	LoadObjectsTree (pProject);
	LoadObjectsLibraryTree (pProject);
}

/*
** LoadObjectsTree :
*/
void CThiraLuaObjectsView::LoadObjectsTree (CProject* pProject)
{
	CStringList strlistModules, strlistModulesDesc;
	CStringList strlistObjects, strlistObjectsDesc;
	CStringList strlistParameters, strlistParametersDesc;
	HTREEITEM htModules, htObjects, htObjectElement, htObjectParameter;
	/* prendiamo il functions container */
	CObjectsContainer &oc = pProject->GetObjectsContainer ();
	oc.GetLibrariesList (strlistModules, strlistModulesDesc);
	/* scorriamo la lista dei moduli */
	for (POSITION pos = strlistModules.GetHeadPosition (); pos;)   {
		CString strPathModule = strlistModules.GetNext (pos);
		CString strModule = ExtractModule (strPathModule);
		htModules=m_treeCtrl.InsertItem (strModule, 5, 5, m_htRoot[0]);
		CString *pstrModule = new CString (strPathModule);
		m_treeCtrl.SetItemData (htModules, (DWORD_PTR) pstrModule);
		/* scorriamo la lista degli oggetti */
		strlistObjects.RemoveAll ();
		strlistObjectsDesc.RemoveAll ();
		oc.GetObjectsList (strPathModule, strlistObjects, strlistObjectsDesc);
		for (POSITION pos = strlistObjects.GetHeadPosition (); pos;)   {
			list<CAdamoObjectElement *> lObjectElement;
		    list<CAdamoObjectElement*>::iterator i;
			CString strObject = strlistObjects.GetNext (pos), strHelp;
			htObjects=m_treeCtrl.InsertItem (strObject, 8, 8, htModules);
			CString *pstrObject = new CString (strObject);
			m_treeCtrl.SetItemData (htObjects, (DWORD_PTR) pstrObject);
			oc.GetObjectsElements (strPathModule, strObject, lObjectElement);
			if (lObjectElement.size())   {
				i=lObjectElement.begin();
				while (i!=lObjectElement.end())   {
					CAdamoObjectElement* pObjectsElement = *i;
					int nImageIndex;
					CString strObjectElement = pObjectsElement->GetName ();
					htObjectElement = NULL;
					switch (pObjectsElement->KindOf ())   {
						case eData :
							nImageIndex = GetImageIndex (((CAdamoObjectData *)pObjectsElement)->GetData ().nType);
							htObjectElement = m_treeCtrl.InsertItem (strObjectElement, nImageIndex, nImageIndex, htObjects);
							break;
						case eMethod :
							htObjectElement = m_treeCtrl.InsertItem (strObjectElement, 6, 6, htObjects);
							strlistParameters.RemoveAll ();
							strlistParametersDesc.RemoveAll ();
							oc.GetParameterList (strPathModule, strObject, strObjectElement, strlistParameters, strlistParametersDesc);
							/* scorriamo la lista dei parametri */
							for (POSITION pos = strlistParameters.GetHeadPosition (); pos;)   {
								CString strParameter = strlistParameters.GetNext (pos);
								htObjectParameter = m_treeCtrl.InsertItem (strParameter, 7, 7, htObjectElement);
								m_treeCtrl.SetItemData (htObjectParameter, (DWORD_PTR) NULL);
							}
							break;
					}
					if (htObjectElement != NULL)   {
						CString *pstrObjectData = new CString (strObjectElement);
						m_treeCtrl.SetItemData (htObjectElement, (DWORD_PTR) pstrObjectData);
					}
					i++;
				}
			}
		}
	}
}

/*
** ExtractModule :
*/
CString CThiraLuaObjectsView::ExtractModule (CString strPathModule)
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
** GetImageIndex :
*/
int CThiraLuaObjectsView::GetImageIndex (int nType)
{
	int nImageIndex = 0;
	switch (nType)   {
		case LUA_TSTRING :
			nImageIndex = 9;
			break;
		case LUA_TNUMBER :
			nImageIndex = 10;
			break;
		case LUA_TBOOLEAN :
			nImageIndex = 11;
			break;
		case LUA_TTABLE :
			nImageIndex = 12;
			break;
		case OBJ_DIGITAL_INPUT :
			nImageIndex = 13;
			break;
		case OBJ_DIGITAL_OUPUT :
			nImageIndex = 14;
			break;
		case OBJ_ANALOG_INPUT :
			nImageIndex = 15;
			break;
		case OBJ_ANALOG_OUTPUT :
			nImageIndex = 16;
			break;
		case OBJ_AXIS :
			nImageIndex = 17;
			break;
		case OBJ_ENCODER :
			nImageIndex = 0;
			break;
		case OBJ_TIMER :			
			nImageIndex = 18;
			break;
		case OBJ_MEMORY	:	
			nImageIndex = 19;
			break;
		case OBJ_GENERIC_CAN :
			nImageIndex = 20;
			break;
	}
	return nImageIndex;
}

/*
** LoadTree :
*/
void CThiraLuaObjectsView::UpdateTree (CProject* pProject)
{
	DeleteObjectsTree ();
	LoadObjectsTree (pProject);
}

/*
** DeleteObjectsTree :
*/
void CThiraLuaObjectsView::DeleteObjectsTree ()
{
	if (m_treeCtrl.ItemHasChildren(m_htRoot[0]))   {
		//itero l'abero
		DeleteData (m_htRoot[0]);
		HTREEITEM ht=m_treeCtrl.GetChildItem(m_htRoot[0]), htNext;
        do   {
			htNext = m_treeCtrl.GetNextSiblingItem (ht);
			m_treeCtrl.DeleteItem (ht);
			ht = htNext;
        }   while (ht);
	}
}

void CThiraLuaObjectsView::OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString str;
	CPoint pt;
	int nLine;
	GetCursorPos(&pt);
	m_treeCtrl.ScreenToClient(&pt);
	UINT nFlags;
	int nLevel;
	HTREEITEM hItem = m_treeCtrl.HitTest(pt, &nFlags);
	if (hItem)   {
		if (GetTreeType (hItem, nLevel) == 0)   {
			if (nLevel == 3)   {
				TVITEM item;
				TVITEM parentitem;
				TCHAR szText[1024];
				item.hItem = hItem;
				item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;
				item.pszText = szText;
				item.cchTextMax = 1024;
				m_treeCtrl.GetItem (&item);
				int nImage = item.iImage;
				CString *pObject = (CString *)item.lParam;
				HTREEITEM hParentItem = m_treeCtrl.GetParentItem (item.hItem);
				if (pObject && hParentItem)   {
					parentitem.hItem = hParentItem;
					parentitem.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;
					parentitem.pszText = szText;
					parentitem.cchTextMax = 1024;
					m_treeCtrl.GetItem (&parentitem);
					CString* pModule = (CString *)parentitem.lParam;
					if (pModule)   {
						CObjectsContainer& oc = m_pProject->GetObjectsContainer ();
						if (oc.GetObjectLine (*pModule, *pObject, nLine))
							((CMainFrame*)AfxGetMainWnd())->GotoFileLine(*pModule, nLine, true);
					}
				}
			}
			else
				if (nLevel == 4)   {
					TVITEM item;
					TVITEM parentitem;
					TCHAR szText[1024];
					item.hItem = hItem;
					item.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;
					item.pszText = szText;
					item.cchTextMax = 1024;
					m_treeCtrl.GetItem (&item);
					int nImage = item.iImage;
					CString *pObjectElement = (CString *)item.lParam;
					HTREEITEM hParentItem = m_treeCtrl.GetParentItem (item.hItem);
					if (pObjectElement && hParentItem)   {
						parentitem.hItem = hParentItem;
						parentitem.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;
						parentitem.pszText = szText;
						parentitem.cchTextMax = 1024;
						m_treeCtrl.GetItem (&parentitem);
						CString *pObject = (CString *)parentitem.lParam;
						hParentItem = m_treeCtrl.GetParentItem (hParentItem);
						if (pObject && hParentItem)   {
							parentitem.hItem = hParentItem;
							parentitem.mask = TVIF_IMAGE | TVIF_TEXT | TVIF_PARAM;
							parentitem.pszText = szText;
							parentitem.cchTextMax = 1024;
							m_treeCtrl.GetItem (&parentitem);
							CString* pModule = (CString *)parentitem.lParam;
							if (pModule)   {
								CObjectsContainer& oc = m_pProject->GetObjectsContainer ();
								if (oc.GetObjectElementLine (*pModule, *pObject, *pObjectElement, nLine))
									((CMainFrame*)AfxGetMainWnd())->GotoFileLine(*pModule, nLine, true);
							}
						}
					}
				}
		}
	}
}

/*
** GetTreeType :
*/
int CThiraLuaObjectsView::GetTreeType (HTREEITEM ht, int& nLevel)
{
	TVITEM item;
	HTREEITEM htRoot = ht;
	TCHAR szText[1024];
	int nTreeType = 0;
	nLevel = 0;
	while (htRoot)   {
		ht = htRoot;
		htRoot = m_treeCtrl.GetParentItem (ht);
		nLevel++;
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
** DeleteLibraryObjects :
*/
void CThiraLuaObjectsView::DeleteLibraryObjects ()
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
** AddLibraryObjects :
*/
void CThiraLuaObjectsView::AddLibraryObjects (CString strLibrary, CString strModule, list<CAdamoObj*>* pLibraryObjects)
{
    list<CAdamoObj*>::iterator i;
	HTREEITEM htLibrary = NULL, htModules = NULL, htObjects, htObjectElement;

    if (pLibraryObjects->size())   {
        i = pLibraryObjects->begin();
        while (i != pLibraryObjects->end())   {
			if (htModules == NULL)   {
				htLibrary = GetTreeItemLibrary (strLibrary);
				if (htLibrary == NULL)
					htLibrary=m_treeCtrl.InsertItem (strLibrary, 21, 21, m_htRoot[1]);
				htModules=m_treeCtrl.InsertItem (strModule, 5, 5, htLibrary);
			}
			list<CAdamoObjectElement *> lObjectElement;
		    list<CAdamoObjectElement*>::iterator q;
			CAdamoObj* pAdamoObj = *i;
			htObjects=m_treeCtrl.InsertItem (pAdamoObj->GetName (), 8, 8, htModules);
			lObjectElement = pAdamoObj->GetListObjectElements ();
			if (lObjectElement.size())   {
				q=lObjectElement.begin();
				while (q!=lObjectElement.end())   {
					CAdamoObjectElement* pObjectsElement = *q;
					int nImageIndex;
					CString strObjectElement = pObjectsElement->GetName ();
					htObjectElement = NULL;
					switch (pObjectsElement->KindOf ())   {
						case eData :
							nImageIndex = GetImageIndex (((CAdamoObjectData *)pObjectsElement)->GetData ().nType);
							htObjectElement = m_treeCtrl.InsertItem (strObjectElement, nImageIndex, nImageIndex, htObjects);
							break;
						case eMethod :   {
							CAdamoParameter* pParameter;
							htObjectElement = m_treeCtrl.InsertItem (strObjectElement, 6, 6, htObjects);
							list<CAdamoParameter *>*pListParameters =  ((CAdamoFunction*)pObjectsElement)->GetListParameters ();
							if (pListParameters->size ())   {
								list<CAdamoParameter*>::iterator s;
								s = pListParameters->begin();
								while (s != pListParameters->end())   {
									pParameter = *s;
									m_treeCtrl.InsertItem (pParameter->m_strName, 7, 7, htObjectElement);
									s++;
								}
							}
							break;
						}
					}
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
HTREEITEM CThiraLuaObjectsView::GetTreeItemLibrary (CString strLibrary)
{
    HTREEITEM ht=m_treeCtrl.GetChildItem(m_htRoot[1]), htRet = NULL;
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
** InstallLibraries :
*/
void CThiraLuaObjectsView::LoadObjectsLibraryTree(CProject* pPrj)
{
	CString strBasePathName = GETWS ()->GetBasePathName (), strDescription;
	int nVersion;
	DeleteLibraryObjects ();
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
					AddLibraryObjects (strLibrary, pLuaModule->GetName (), pLuaModule->GetListObjects ());
				}
			}
			delete pLuaLibrary;
		}
	}
}

/*
** AddRootNodes :
*/
void CThiraLuaObjectsView::AddRootNodes ()
{
	/* oggetti nel custom */
	if (m_htRoot[0] == NULL)   {
		CString str = LOADSTRING (IDS_THIRA_LUA_OBJECTS_ROOT);
		m_htRoot[0]=m_treeCtrl.InsertItem (str, 4, 4, TVI_ROOT);
		CString *pstrRoot = new CString ("0");
		m_treeCtrl.SetItemData (m_htRoot[0], (DWORD_PTR) pstrRoot);
	}
	/* oggetti nelle librerie */
	if (m_htRoot[1] == NULL)   {
		CString str = LOADSTRING (IDS_HELPONLINE_4);
		m_htRoot[1]=m_treeCtrl.InsertItem (str, 1, 1, TVI_ROOT);
		CString *pstrRoot = new CString ("1");
		m_treeCtrl.SetItemData (m_htRoot[1], (DWORD_PTR) pstrRoot);
	}
}

/*
** DeleteAllNodes :
*/
void CThiraLuaObjectsView::DeleteAllNodes ()
{
	for (int i = 0; i < 2; i++)   {
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
** OnSize :
*/
void CThiraLuaObjectsView::OnSize(UINT nType, int cx, int cy) 
{
	CPaneView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_treeCtrl.GetSafeHwnd ()) )
		m_treeCtrl.SetWindowPos (NULL, 0, 0, cx, cy, 0);
}

/*
** RefreshMetrics :
*/
void CThiraLuaObjectsView::RefreshMetrics(AppTheme nTheme)
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
