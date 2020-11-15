#if !defined(AFX_DEVICELISTVIEW_H__1E413C08_0F81_4CC0_9AAA_D5AD6789105E__INCLUDED_)
#define AFX_DEVICELISTVIEW_H__1E413C08_0F81_4CC0_9AAA_D5AD6789105E__INCLUDED_

#include "SubEdit.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeviceListView view

class CDeviceListView : public CListView
{

typedef struct tagITEMINFO {
    CString     strName; 
    int         iRxNumber; 
    int         iNode; 
    int         iIndex;
    int         iSubIndex;
    int         iDataType;
    CObject     *pObj;
} ITEMINFO;

protected:
	CDeviceListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDeviceListView)
	
// Attributes
public:
	CListCtrl* GetListPtr() { return &GetListCtrl (); };
// Operations
public:
    void SetCurrentObj(CObject *pObj);
    void SetCurNode(CObject *pObj,CProject *pPrj = NULL);
	void DeleteItem(CObject *pObj);
	void AddItem(CObject *pObj);
	void OnUpdateView(CView* pSender, LPARAM lHint, CObject* pHint);
	void DropItemOnList(int iIndexItem);
	void Refresh();
    void CmdRouteMenu(CWnd* pWnd,CMenu* pPopupMenu);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
    CObject*            m_objCurNode;
    CObject*            m_objCurObj;
    CProject*           m_objCurProject;
	BOOL CreateNewDev(CRuntimeClass* pRunTime,CArchive *pAr = NULL);
	void ClearObj(CObject *pObj);
	void CopyNameManage(CObject *pObj);
    void Iterator(CAdamoGroup *pGrp);
	void AddTail(CObject *pNode,CObject *pObj);	
	CProject *NewProject();
	virtual ~CDeviceListView();
	CImageList m_iIcons;
	CSubEdit m_editWnd;
	int m_subitem;
	int m_item;
    CAdamoGroup *m_objCurrentGroup;
	static int CALLBACK CompareFunc (LPARAM lParam1, LPARAM lParam2,
	    LPARAM lParamSort);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void AddTail(CObject *obj);
	void AddItem(int nIndex,CObject *pObj);
	void RefreshPrj(CProject *pPrj);
	void Refresh(CAdamoGroup *pGrp);
	void FreeItemMemory();
	//{{AFX_MSG(CDeviceListView)
	afx_msg void OnEditClear();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);	
	afx_msg void OnEditDeviceAddaxis();
    afx_msg void OnEditAddgroup();
	afx_msg void OnEditDeviceAddanaloginput();
	afx_msg void OnEditDeviceAddanalogout();
	afx_msg void OnEditDeviceAddcounter();
	afx_msg void OnEditDeviceAdddigitalin();
	afx_msg void OnEditDeviceAdddigitalout();
	afx_msg void OnEditDeviceAddtimer();
	afx_msg void OnUpdateEditAddDevice(CCmdUI* pCmdUI);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnEditDeviceAddgeneric();
	//}}AFX_MSG
public:
	afx_msg void OnEditAddmachine();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICELISTVIEW_H__1E413C08_0F81_4CC0_9AAA_D5AD6789105E__INCLUDED_)
