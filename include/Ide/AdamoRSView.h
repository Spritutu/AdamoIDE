#if !defined(AFX_ADAMORSVIEW_H__12915C2F_A698_4BF7_AB67_EA4B1C054899__INCLUDED_)
#define AFX_ADAMORSVIEW_H__12915C2F_A698_4BF7_AB67_EA4B1C054899__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoRSView.h : header file
//

/* include files */
#include "path.h"
#include "HTMLFilter.h"

/* namespace */
using namespace nsPath;

#define RS_VIEW_RESOURCES 0
#define RS_VIEW_BITMAPS   1

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSView view

class CAdamoRSView : public CView
{
protected:
	CAdamoRSView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoRSView)

// Attributes
public:

// Operations
public:
    void Activate();
    void OnCloseRSView ();
    void AddRSResource ();
    void AddRSFolder ();
    void EditRSResource ();
    void DeleteRSResource ();
    void RenameRSResource (CString strPrev, CString strNew, eResources e);
    bool RenameRSImage (CString strImage, CString strID);
    void UpdateDiagramEntitySize ();
    void UpdateDiagramSize ();
    void UpdateDiagramValue ();
    void UpdateDiagramSelected ();
    void AddRSBitmap();
    void DeleteRSBitmap();
    void SelectRSBitmap (CString strID);
    int  GetFirstRSImage (CString& str);
    int  GetNextRSImage (CString& str);
    void CloseTabMode ();
	void OnAdamoTimer ();
	void SetTabStyle ();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoRSView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL
// Implementation
protected:
	virtual ~CAdamoRSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoRSView)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnRefreshMetrics(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelChange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void CreateImageList();
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon);
	CView* GetView (int nItem);
	CView* GetActiveView ();
private :
	CXTPTabControl m_wndTabControl;
    CBitmap*    m_bmpDlgs;
    CBitmap*    m_bmpPreview;
    CBitmap*    m_bmpImages;
    CXTPImageList* m_tbiml;
	AppTheme	m_theme;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMORSVIEW_H__12915C2F_A698_4BF7_AB67_EA4B1C054899__INCLUDED_)
