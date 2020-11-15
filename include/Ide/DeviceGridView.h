#if !defined(AFX_DEVICEGRIDVIEW_H__5645266D_8FF5_45B1_AD24_049E4A45F837__INCLUDED_)
#define AFX_DEVICEGRIDVIEW_H__5645266D_8FF5_45B1_AD24_049E4A45F837__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceGridView.h : header file
//

#include "GridCtrlDevCan.h"
#include "GridCtrlDev.h"
#include "FlatTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDeviceGridView view

class CDeviceGridView : public CView
{
protected:
	CDeviceGridView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDeviceGridView)

// Attributes
public:

// Operations
public:
    CFlatTabCtrl*			m_pFlatTabCtrl;
    void DropItemOnList(int iIndexItem);
    void SetCurrentObj(CObject *pObj);
    void SetCurNode(CObject *pObj,CProject *pPrj = NULL);
	void OnUpdateView(CView* pSender, LPARAM lHint, CObject* pHint);
	void Refresh();
    CGridCtrlDevCan* GetGridCtrlDevCan() { return &m_pGridCtrlCan; };
    CGridCtrlDev* GetGridCtrlDev() { return &m_pGridCtrl; };
    CFlatTabCtrl* GetFlatTabCtrl() { return m_pFlatTabCtrl; };
    void CmdRouteMenu(CWnd* pWnd,CMenu* pPopupMenu);
    void ResetDataGrid();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceGridView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
protected:
    CRect m_rcTabs;
    CGridCtrlDevCan m_pGridCtrlCan;
    CGridCtrlDev m_pGridCtrl;
    int   m_dwTabStyle;
	virtual ~CDeviceGridView();

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
    CGridCtrl *GetGridFromObj(CObject *pObj);
    void DeleteItem(CObject *pObj);
   	void ClearObj(CObject *pObj);
    int GetTab(CObject *pObj);
	void SelectItem(CObject *pObj);
    void AddItem(CObject *pObj);
	void UpdateTabsStyle();
	void UpdateActiveView();
	int CreateViews();
	void CreateTabs();
	BOOL CreateNewDev(CRuntimeClass* pRunTime,CArchive *pAr = NULL);
	void CopyNameManage(CObject *pObj);
    void Iterator(CAdamoGroup *pGrp);
	void AddTail(CObject *pNode,CObject *pObj);	
    CProject* NewProject();
	void RefreshPrj(CProject *pPrj);
	void Refresh(CAdamoGroup *pGrp);
    void AddItem(int &nIndex, int &nIndexCan, CObject *pObj);
	//{{AFX_MSG(CDeviceGridView)
	afx_msg void OnEditClear();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#define TAB_DEVICECAN       0
#define TAB_DEVICE          1

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEGRIDVIEW_H__5645266D_8FF5_45B1_AD24_049E4A45F837__INCLUDED_)
