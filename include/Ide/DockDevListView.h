#if !defined(AFX_DOCKDEVLISTVIEW_H__AB759339_BB38_4464_A3B1_73FD24EED637__INCLUDED_)
#define AFX_DOCKDEVLISTVIEW_H__AB759339_BB38_4464_A3B1_73FD24EED637__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DockDevListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDockDevListView view

#define	ID_REPCTRL_CONFDEVLIST	 1002

class CAdamoDDLVCXTPReportControl : public CXTPReportControl 
{
	public :
		CAdamoDDLVCXTPReportControl () : m_ButtonDown (false), m_Dragging (false), m_pRecord (NULL)   {
		}
		void OnDrag (CXTPReportRecord* pRecord, CPoint point);
	protected :
		afx_msg void OnLButtonDown (UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp (UINT nFlags, CPoint point);
		afx_msg void OnMouseMove (UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()
	private :
		int GetDraggedDevice ();
	private :
		CXTPReportRecord* m_pRecord;
		bool	   m_ButtonDown;
		bool	   m_Dragging;
		CPoint     m_point;
};

class CDockDevListView : public CPaneView
{
	friend CAdamoDDLVCXTPReportControl;
protected:
	CDockDevListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDockDevListView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDockDevListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CAdamoDDLVCXTPReportControl m_wndReport;
	CImageList  m_ilLarge;
	CImageList* m_pDragImage;
	BOOL		m_bDragging;
	int			m_nDragIndex, m_nDropIndex;
	HTREEITEM	m_hDropItem;
	CWnd*		m_pDropWnd;
    bool        m_bIsHighlight;
	virtual ~CDockDevListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CView* GetActiveView();
	//{{AFX_MSG(CDockDevListView)
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void RefreshMetrics(AppTheme nTheme);
private :
    bool        m_bInitialized;
private :
    void InitView ();
	void UpdateImages ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOCKDEVLISTVIEW_H__AB759339_BB38_4464_A3B1_73FD24EED637__INCLUDED_)
