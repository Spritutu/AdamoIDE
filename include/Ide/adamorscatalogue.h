#if !defined(AFX_ADAMORSCATALOGUE_H__BE5E1A08_AA03_4FDE_8C94_3D866DB679D6__INCLUDED_)
#define AFX_ADAMORSCATALOGUE_H__BE5E1A08_AA03_4FDE_8C94_3D866DB679D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// adamorscatalogue.h : header file
//

#define RS_LOAD    -1
#define RS_RESOURCE 0
#define RS_FOLDER   1

#define NROOTITEMS          5
#define NROOTMACHINEITEMS   4
#define ID_TREECTRL_RS_CATALOGUE 1103

/* forward references */
class CParsedEdit;
class CAdamoRSCatalogue;

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSCatalogue view
class CAdamoRSTree : public CXTPTreeCtrl 
{
	public :
		CAdamoRSTree (CAdamoRSCatalogue* pRSCatalogue) : m_pRSCatalogue (pRSCatalogue), m_bCtrlPressed (false)  {
		}
		bool IsCtrlPressed ()		{ return m_bCtrlPressed; }
		void CtrlPressed (bool b)	{ m_bCtrlPressed = b; }
private :
	BOOL PreTranslateMessage(MSG* pMsg);
private :
	CAdamoRSCatalogue *m_pRSCatalogue;
	bool m_bCtrlPressed;
};

class CAdamoRSCatalogue : public CPaneView
{
	friend CAdamoRSTree;
protected:
	CAdamoRSCatalogue();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoRSCatalogue)

// Attributes
public:

// Operations
public:
    bool AddCatalogueElement     (CString strName, CString strTrad, eResources eType, int nKind);
    bool RemoveCatalogueElement  ();
    int  GetValidName  (CString& str, eResources* e);
    int  GetValidFolderName (CString& str, eResources* e);
    bool GetDeleteName (CString& strName, eResources* eType);
    bool GetSelectedName (CString& strName, eResources* eType);
    bool IsAValidResource ();
    void Clear ();
    bool AddImageElement (CString strName);
    CString GetSelectedImageID ();
    void DeleteImageItem (CString strID);
    eResources GetResourceType (HTREEITEM ht);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoRSCatalogue)
    public :
	    virtual void OnInitialUpdate();
	protected:
	    virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoRSCatalogue();
	void RefreshMetrics(AppTheme nTheme);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoRSCatalogue)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnRSCatAddResource();
	afx_msg void OnRSCatDeleteResource();
	afx_msg void OnRSCatAddFolder();
	afx_msg void OnRSCatAddBitmap();
	afx_msg void OnRSCatDeleteBitmap();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CXTTreeCtrl& GetTreeCtrl() { return m_treeCtrl; }
	void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	void OnSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
    void OnContextMenuResource(CPoint point);
    void OnContextMenuBitmap(CPoint point);
    void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
    bool IsRSCatRoot (HTREEITEM ht, eResources* e);
    CString GetNewName (eResources e);
    CString BuildRSPath (HTREEITEM ht, eResources e);
    CString BuildCompleteRSPath (CString str, eResources e, HTREEITEM ht);
    HTREEITEM AddTreeItem (CString token, HTREEITEM ht, int nImage);
    void OnRename();
    void OnRenameRSResource(CString strPrev, CString strNew, eResources e);
    bool OnRenameRSImage (CString strImage, CString strID);
    void OnExpandRequest ();
    void InsertRootItems ();
    void DeleteAllData ();
    void DeleteData (HTREEITEM hItem);
    void MettiLaChiave (HTREEITEM ht, CString strKey);
    void SostituisciLaChiave (HTREEITEM ht, CString strName);
    void InitView ();
    bool ExistItem (HTREEITEM ht, CString strName);
private :
	CAdamoRSTree m_treeCtrl;
    CXTPImageList m_il;
    HTREEITEM  m_ahItem[5];
    CParsedEdit* m_pEdit;
    bool m_bInitialized;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMORSCATALOGUE_H__BE5E1A08_AA03_4FDE_8C94_3D866DB679D6__INCLUDED_)
