#if !defined(AFX_CONFTREE_H__12996894_5A7F_4AAF_934D_124D35A30A4C__INCLUDED_)
#define AFX_CONFTREE_H__12996894_5A7F_4AAF_934D_124D35A30A4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CConfTreeView view
#include "ColumnTreeView.h"
#include "ConfFind.h"

#define ID_REPCTRL_CONFTREE	1001

class CHWTabHtmlView;

class CAdamoCXTPReportControl : public CXTPReportControl 
{
	public :
		CAdamoCXTPReportControl () : m_ButtonDown (false), m_Dragging (false) {
		}
	protected :
		afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
		afx_msg void OnMouseMove (UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()
	private :
		bool	   m_ButtonDown;
		bool	   m_Dragging;
		CPoint     m_point;
};

class CConfTreeView : public CPaneView
{
protected:
	CConfTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CConfTreeView)
	//CImageList m_images;
    CAdamoPlant *m_pPlant;
    BOOL m_isConfig;
    BOOL m_isHW;
    CString strPreviousRoot;
	//HTREEITEM m_hItemDrag;
	
// Attributes
public:
// Operations
public:
	void SearchItemDrop(bool bLoadFromProject = false);
    void DelColorItemDrag(const char *szDelete);
	void SetColorItemDrag();
	void Update(CObject *pObj);
	void DeleteItem(CObject *pObj);
	void UpdateTree();
    void SetIsConfig(BOOL isConfig);
    void SetIsHW(BOOL isHW);
    void SetPlant(CAdamoPlant *pPlant) { m_pPlant = pPlant; };
    void DropItemOnList(int iIndexItem);
	CXTPReportRecord* AddItem(CObject *pObj);
	CXTPReportRecord* AddItemRecord (CObject *pObj, CXTPReportRecord* hItem);
    void SetItemBold (CProject* pPrj);
    void ActivateMachine (int nModule);
    void ClearAllRoot ();
    void Activate();
    CAdamoBase *GetSelectedDevice ();
    CProject *GetSelectedProject ();
    //HTREEITEM FindTreeNode (HTREEITEM hItem, HTREEITEM hItemBS, bool* bs);
    void FlushData ()   { }
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void SetNoHeader (BOOL bShow);
	void OnDrag (CXTPReportRecord* pRecord);
	CXTPReportControl* GetReportControl ()	{ return &m_wndReport; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CConfTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
// Generated message map functions
protected:
    CObject*            m_objCurNode;
    CObject*            m_objCurObj;
    CProject*           m_objCurProject;
    CAdamoGroup*        m_objCurrentGroup;
    BOOL SearchFileBin (CAdamoDatiFile* pDatiFile, enDispositivi enDev, int nIndex, CString& pStr);
	void SearchHWAddress (CXTPReportRecord* pRecord, CAdamoDatiFile* pDatiFile);
	void SearchDrop(CXTPReportRecord* pRecord, CAdamoDatiFile* pDatiFile);
	void IteratorItem(CXTPReportRecord* pRecord, CAdamoDatiFile* pDatiFile);
	void IteratorItem(CXTPReportRecord* pRecord, const char* szDelete);
    void AddTail(CObject *pNode,CObject *pObj);	
    void Iterator(CAdamoGroup *pGrp);
	void CopyNameManage(CObject *pObj);
    //void SortItem(HTREEITEM hRoot);
    static int GetDeviceOrder(CObject *pObj);
	static int CALLBACK CompareFunc (LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void SendObj(CXTPReportRecord* pRecord, CObject *pObj);
	void StoreObj(CXTPReportRecord* pRecord, CObject *pObj);
    CProject* GetProject(CXTPReportRecord* pRecord);
	void Iterator(CAdamoGroup *pGrp,CXTPReportRecord* pRecord);
    void CmdRouteMenu(CWnd* pWnd,CMenu* pPopupMenu);
   	void ClearObj(CObject *pObj);
	BOOL CreateNewDev(CRuntimeClass* pRunTime,CArchive *pAr = NULL);
	CString GetRootMachine(CXTPReportRecord* pRecord);
    CString GetPathMachine(CXTPReportRecord* pRecord);
	CString GetDeviceIndex(CAdamoBase *pObjBase);
	CString GetOffSetObj(CAdamoBase* pObjBase);
	CString GetTypeObj(CAdamoBase* pObjBase);
    CXTPReportRecord* GetRootItem ();
	CXTPReportRecord* AggiungiDispositivoAlTree (CObject *pObj, CXTPReportRecord* pRecord);
    void TogliIGruppiSenzaFigli ();
	void TogliIGruppiSenzaFigli (CXTPReportRecord* pRecord);
	void SearchItem(CXTPReportRecord* pRecord, const char* szDelete);
	//{{AFX_MSG(CConfTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnEditClear();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);	
	afx_msg void OnUpdateEditDeviceAddaxis(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditAddgroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDeviceAddanaloginput(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDeviceAddanalogout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDeviceAddcounter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDeviceAdddigitalin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDeviceAdddigitalout(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDeviceAddtimer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDeviceAddgeneric(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDeviceAddmachine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateActivateMachine(CCmdUI* pCmdUI);
	afx_msg void OnEditDeviceAddaxis();
    afx_msg void OnEditAddgroup();
	afx_msg void OnEditDeviceAddanaloginput();
	afx_msg void OnEditDeviceAddanalogout();
	afx_msg void OnEditDeviceAddcounter();
	afx_msg void OnEditDeviceAdddigitalin();
	afx_msg void OnEditDeviceAdddigitalout();
	afx_msg void OnEditDeviceAddtimer();
   	afx_msg void OnEditDeviceAddgeneric();
    afx_msg void OnEditAddmachine();
    afx_msg void OnActivateMachine();
	//afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR*  pNotifyStruct, LRESULT* pResult);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnReportSelChanged(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    afx_msg LRESULT OnCloseDialogFind (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	void RefreshMetrics(AppTheme nTheme);
    private :
        int FindInLogicTree();
        //bool FindText (HTREEITEM hItem);
        BOOL CheckPathLength ();
        void InitView ();
		void SetItemColor (CXTPReportRecordItem* pItem, COLORREF rgb);
		void UpdateImages ();
		void SelectRecord (CXTPReportRecord* pRecord);
    private :
		CAdamoCXTPReportControl m_wndReport;
		CXTPReportRecord* m_pRecord;
        CConfFind* m_pConfFind;
        bool       m_bLoaded; 
        bool       m_bInitialized;
        ePswLevel  m_ePswLevel;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFTREE_H__12996894_5A7F_4AAF_934D_124D35A30A4C__INCLUDED_)
