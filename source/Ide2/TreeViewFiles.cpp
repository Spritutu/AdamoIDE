// TreeViewFiles.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "PaneView.h"
#include "TreeViewFiles.h"

#include "MainFrame.h"
#include "LuaDoc.h"
#include "LuaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeViewFiles

IMPLEMENT_DYNCREATE(CTreeViewFiles, CPaneView)

CTreeViewFiles::CTreeViewFiles()
{
}

CTreeViewFiles::~CTreeViewFiles()
{
}

BEGIN_MESSAGE_MAP(CTreeViewFiles, CPaneView)
	//{{AFX_MSG_MAP(CTreeViewFiles)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY (NM_RCLICK, ID_TREECTRL_VIEWFILES, OnRclick)
	ON_NOTIFY (NM_DBLCLK, ID_TREECTRL_VIEWFILES, OnDblclk)
	ON_NOTIFY (TVN_KEYDOWN, ID_TREECTRL_VIEWFILES, OnKeydown)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_PROJECT_ADD_FILES, OnProjectAddFiles)
	ON_COMMAND(ID_PROJECT_PROPERTIES, OnProjectProperties)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeViewFiles drawing

void CTreeViewFiles::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CTreeViewFiles diagnostics

#ifdef _DEBUG
void CTreeViewFiles::AssertValid() const
{
	CPaneView::AssertValid();
}

void CTreeViewFiles::Dump(CDumpContext& dc) const
{
	CPaneView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTreeViewFiles message handlers
int CTreeViewFiles::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPaneView::OnCreate(lpCreateStruct) == -1)
		return -1;
	CreateTree ();
	m_images.Create (IDB_IL_FILE, 16, 1, RGB(0,255,0));
	m_treeCtrl.SetImageList (&m_images, TVSIL_NORMAL);
	return 0;
}

/*
** CreateTree :
*/
int CTreeViewFiles::CreateTree ()
{
	if (m_treeCtrl.GetSafeHwnd() == 0)  	{
		if (!m_treeCtrl.Create ( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT
								| TVS_SHOWSELALWAYS, CRect(0,0, 0, 0), this, ID_TREECTRL_VIEWFILES))   {
			TRACE0("Unable to create tree control.\n");
			return 0;
		}
	}
	return 1;
}

void CTreeViewFiles::RemoveAll()
{
	m_treeCtrl.DeleteAllItems();
}

void CTreeViewFiles::RemoveItem(CString strName)
{
    DeleteItem(GetItemFromName(strName));
}

void CTreeViewFiles::AddRoot(CString strProject,BOOL isActive)
{
	CString strLabel, strTV;
	strLabel.Format(LOADSTRING (IDS_ADAMOTREEVIEWFILES_1), strProject);
    
    HITEM_RF *str = new HITEM_RF;

	TV_INSERTSTRUCT root;
	root.hParent = NULL;
	root.hInsertAfter = TVI_ROOT;
	root.item.iImage = 1;
	root.item.iSelectedImage = 1;
	root.item.pszText = strLabel.GetBuffer(0);
    strLabel.ReleaseBuffer ();
    root.item.lParam = NULL;
	root.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
    str->hRoot = m_treeCtrl.InsertItem(&root);

    SetItemBold(str->hRoot, isActive);

    TV_INSERTSTRUCT files;
	files.hParent = str->hRoot;
	files.hInsertAfter = TVI_SORT;
	files.item.iImage = 2;
	files.item.iSelectedImage = 2;
    strTV=LOADSTRING (IDS_ADAMOTREEVIEWFILES_2);
	files.item.pszText = strTV.GetBuffer (0);
    strTV.ReleaseBuffer ();
    files.item.lParam = NULL;
	files.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
  	str->hFilesFolder = m_treeCtrl.InsertItem(&files);

    m_treeCtrl.Select(str->hRoot,TVGN_CARET);

    m_lhItem.AddTail(str);
}

HTREEITEM CTreeViewFiles::AddProjectFile(CString strName, long lParam)
{
    HTREEITEM hItem = m_treeCtrl.GetSelectedItem();
    HITEM_RF *str=NULL;

    if(hItem){
        HTREEITEM hParent;
        while ((hParent = m_treeCtrl.GetParentItem (hItem)) != NULL) {
            hItem = hParent;
        }

        POSITION pos = m_lhItem.GetHeadPosition();

        while(pos){
            str = (HITEM_RF*)m_lhItem.GetNext(pos);
            if ( hItem == str->hRoot)
                break;
        }
    }else{
        POSITION pos = m_lhItem.GetTailPosition();
        str = (HITEM_RF*)m_lhItem.GetAt(pos);
    }

	TV_INSERTSTRUCT file;
//	file.hParent = m_hFilesFolder;
    if(str)
        file.hParent = str->hFilesFolder;
	file.hInsertAfter = TVI_LAST;
	file.item.iImage = 4;
	file.item.iSelectedImage = 4;
	file.item.pszText = strName.GetBuffer(0);
	file.item.lParam = lParam;
	file.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
	
	return m_treeCtrl.InsertItem(&file);
}

void CTreeViewFiles::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    if (((CMainFrame *) AfxGetMainWnd ())->GetPswLevel ()<=ePswBuilder)   {
	    CPoint pt;
	    GetCursorPos(&pt);
	    m_treeCtrl.ScreenToClient(&pt);
	    UINT nFlags;
	    HTREEITEM item = m_treeCtrl.HitTest(pt, &nFlags);
	    
        //forza la selezione
        m_treeCtrl.Select(item,TVGN_CARET);
	    
        HMENU hMenu = NULL;

        POSITION pos = m_lhItem.GetHeadPosition();

        while(pos){
            HITEM_RF *str = (HITEM_RF*)m_lhItem.GetAt(pos);
            if ( item == str->hRoot)
		        hMenu = LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_PROJECT_MENU));
            m_lhItem.GetNext(pos);
        }

	    if ( !hMenu )
		    return;

	    HMENU hSubMenu = GetSubMenu(hMenu, 0);
	    if (!hSubMenu) 
	    {
		    DestroyMenu(hMenu);
		    return;
	    }

	    POINT mouse;
	    GetCursorPos(&mouse);
	    ::SetForegroundWindow(m_hWnd);	
	    ::TrackPopupMenu(hSubMenu, 0, mouse.x, mouse.y, 0, m_hWnd, NULL);

	    DestroyMenu(hMenu);
    }	    
	*pResult = 0;
}

void CTreeViewFiles::OnProjectAddFiles() 
{
    CString strText = m_treeCtrl.GetItemText(GetSelectedItem());

	CWorkspace* pWorkspace = ((CMainFrame*)AfxGetMainWnd())->GetWorkSpace();

    CProject* pProject = pWorkspace->GetPtrProject(strText);

    if(pProject)
	    pProject->ImportFiles();
}

void CTreeViewFiles::OnProjectProperties() 
{
	CWorkspace* pWorkspace = ((CMainFrame*)AfxGetMainWnd())->GetWorkSpace();

    CString strText = m_treeCtrl.GetItemText(GetSelectedItem());

    CProject* pProject = pWorkspace->GetPtrProject(strText);

    if(pProject)
    	pProject->Properties();
}

void CTreeViewFiles::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	GetCursorPos(&pt);
	m_treeCtrl.ScreenToClient(&pt);
	UINT nFlags;
	HTREEITEM hItem = m_treeCtrl.HitTest(pt, &nFlags);
    if (hItem)   {
        CProject* pPrj=GETWS()->GetActiveProject ();

	    CProjectFile* pPF = (CProjectFile*)m_treeCtrl.GetItemData(hItem);
	    if ( pPF )
	    {
            CString strCF=pPrj->GetCompilingFile ();
            CString strPF=pPF->GetPathName ();
            if (strCF!=strPF)   {
	            CLuaView* pView = theApp.OpenProjectFilesView(pPF);
                if (pView)
	                pView->Activate();
	        }
	    }
    }
	*pResult = 0;
}

void CTreeViewFiles::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem = m_treeCtrl.GetSelectedItem();
	if (hItem && pTVKeyDown->wVKey == VK_DELETE && m_treeCtrl.GetItemData(hItem))   {
        CProjectFile* pPF = (CProjectFile*)m_treeCtrl.GetItemData(hItem);
        CProject* pPrj=GETWS()->GetActiveProject ();
        if (pPF&&pPrj)   {
            if (pPF->GetName ()!=pPrj->GetName ())   {
                if (AfxMessageBox (LOADSTRING (IDS_ADAMOTREEVIEWFILES_3), MB_YESNO | MB_ICONQUESTION)==IDYES)   {
			        m_treeCtrl.DeleteItem(hItem);
    	            pPrj->RemoveFile(pPF);
		        }
	        }
            else
                AfxMessageBox (LOADSTRING (IDS_ADAMOTREEVIEWFILES_4));
        }
    }
    else 
        if (hItem && pTVKeyDown->wVKey == VK_RETURN)    {
            if (m_treeCtrl.GetItemData(hItem)) {
                CProjectFile* pPF = (CProjectFile*)m_treeCtrl.GetItemData(hItem);
	            if  ( pPF )   {
		            CLuaView* pView = theApp.OpenProjectFilesView(pPF);
		            pView->Activate();
                }
        }
        else
            m_treeCtrl.Expand(hItem, TVE_TOGGLE);
	}
	*pResult = 0;
}

HTREEITEM CTreeViewFiles::GetSelectedItem()
{
    HTREEITEM hItem = m_treeCtrl.GetSelectedItem();
    HITEM_RF *str=NULL;

    HTREEITEM hParent;
    while ((hParent = m_treeCtrl.GetParentItem (hItem)) != NULL) {
        hItem = hParent;
    }

    POSITION pos = m_lhItem.GetHeadPosition();

    while(pos){
        str = (HITEM_RF*)m_lhItem.GetNext(pos);
        if ( hItem == str->hRoot)
            return str->hRoot;
    }

    return NULL;
}

void CTreeViewFiles::DeleteItem(HTREEITEM hItem)
{
    POSITION pos = m_lhItem.GetHeadPosition();

    while(pos){
        HITEM_RF *str = (HITEM_RF*)m_lhItem.GetAt(pos);
        if ( hItem == str->hRoot){
            m_treeCtrl.DeleteItem(str->hRoot);
            m_lhItem.RemoveAt(pos);
            delete str;
            break;
        }
        m_lhItem.GetNext(pos);
    }

}

void CTreeViewFiles::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
    m_treeCtrl.SetItemState( hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD );
}

HTREEITEM CTreeViewFiles::GetItemFromName(CString strName)
{
    POSITION pos = m_lhItem.GetHeadPosition();
    CString dummy;

    dummy.Format(LOADSTRING (IDS_ADAMOTREEVIEWFILES_1),strName);

    while(pos){
        HITEM_RF *str = (HITEM_RF*)m_lhItem.GetAt(pos);
        if ( dummy == m_treeCtrl.GetItemText(str->hRoot))
            return str->hRoot;
        m_lhItem.GetNext(pos);
    }

    return NULL;
}

void CTreeViewFiles::SetItemBold(CString itemName, BOOL bBold)
{
    SetItemBold(GetItemFromName(itemName), bBold);
}

void CTreeViewFiles::OnInitialUpdate() 
{
	CPaneView::OnInitialUpdate();
	
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if(pFrame)
	    m_treeCtrl.SetFont(pFrame->GetProgramFont());		
}

/*
** ActivateMachine : 
*/
void CTreeViewFiles::ActivateMachine (int nModule)
{
    RemoveAll ();
    CProject* pPrj=GETWS()->GetActiveProject ();
    pPrj->RedrawFilesTree (this);
}

/*
** OnSize :
*/
void CTreeViewFiles::OnSize(UINT nType, int cx, int cy) 
{
	CPaneView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_treeCtrl.GetSafeHwnd ()) )
		m_treeCtrl.SetWindowPos (NULL, 0, 0, cx, cy, 0);
}

/*
** RefreshMetrics :
*/
void CTreeViewFiles::RefreshMetrics(AppTheme nTheme)
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

/*
** OnDestroy :
*/
void CTreeViewFiles::OnDestroy ()
{
	CPaneView::OnDestroy();
	for (POSITION pos = m_lhItem.GetHeadPosition (); pos; )   {
		HITEM_RF* p = (HITEM_RF*) m_lhItem.GetNext (pos);
		delete p;
	}
	m_lhItem.RemoveAll ();
}

/*
** PreCreateWindow :
*/
/*
BOOL CTreeViewFiles::PreCreateWindow (CREATESTRUCT& cs) 
{
	cs.style |= TVS_FULLROWSELECT;
	return CTreeView::PreCreateWindow(cs);
}
*/