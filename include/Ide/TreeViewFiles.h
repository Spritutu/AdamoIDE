#if !defined(AFX_TREEVIEWFILES_H__D5303282_718E_4AB7_A667_017E34E216BB__INCLUDED_)
#define AFX_TREEVIEWFILES_H__D5303282_718E_4AB7_A667_017E34E216BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TreeViewFiles.h : header file
//

#include "PaneView.h"

#define ID_TREECTRL_VIEWFILES	1101

/////////////////////////////////////////////////////////////////////////////
// CTreeViewFiles view

class CTreeViewFiles : public CPaneView
{
	public:
		CTreeViewFiles();           // protected constructor used by dynamic creation
		virtual ~CTreeViewFiles();
	protected:
		DECLARE_DYNCREATE(CTreeViewFiles)

	// Attributes
	public:
		struct HITEM_RF
		{
			HTREEITEM hRoot;
			HTREEITEM hFilesFolder;
		};
	// Operations
	public:
		void RemoveAll();
		void RemoveItem(CString strName);
		HTREEITEM AddProjectFile(CString strName, long lParam);
		void AddRoot(CString strProject,BOOL isActive);
		void SetItemBold(CString itemName, BOOL bBold);
		void ActivateMachine (int nModule);
	protected:
		CXTPTreeCtrl m_treeCtrl;
	//	HTREEITEM m_hRoot, m_hFilesFolder;
		CPtrList m_lhItem;
		CXTPImageList m_images;

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CTreeViewFiles)
		public:
		virtual void OnInitialUpdate();
		protected:
		virtual void OnDraw(CDC* pDC);      // overridden to draw this view
		virtual void OnProjectAddFiles();
		virtual void OnProjectProperties();
		//}}AFX_VIRTUAL
		void RefreshMetrics(AppTheme nTheme);
// Implementation
	protected:
   		HTREEITEM GetSelectedItem();
   		void DeleteItem(HTREEITEM hItem);
		HTREEITEM GetItemFromName(CString strName);
		void SetItemBold(HTREEITEM hItem, BOOL bBold);
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

	// Generated message map functions
	protected:
		//{{AFX_MSG(CTreeViewFiles)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnDestroy();
		afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	private :
		int CreateTree ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TREEVIEWFILES_H__D5303282_718E_4AB7_A667_017E34E216BB__INCLUDED_)
