#if !defined(AFX_ADAMODGVIEW_H__086A6805_041B_4C15_A3FD_C7095791CC54__INCLUDED_)
#define AFX_ADAMODGVIEW_H__086A6805_041B_4C15_A3FD_C7095791CC54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGView.h : header file
//

#define IDC_TABDGCONTROL 2002

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGView view

class CAdamoDGView : public CView
{
protected:
	CAdamoDGView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoDGView)

// Attributes
public:

// Operations
public:
    void Activate();
    void OnCloseDGView ();
    int  Find (const char* str, int nDirection, bool bMatchWholeWord, bool bUpperCase);
    void OnAdamoTimer ();
    bool IsDGDeviceOpened ();
	bool IsEcDeviceOpened ();
    void SetDGMode (int nMode);
    void SetEcMode (int nMode);
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoDGView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoDGView)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnRefreshMetrics(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelChange (NMHDR* pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CXTPTabControl m_wndTabControl;
    CAdamoMachine *m_pMachine;
    CBitmap*    m_bmpHardware;
    CBitmap*    m_bmpComm;
    CBitmap*    m_bmpSSCNET;
    CBitmap*    m_bmpEthercat;
    CImageList  m_tbiml;
    bool        m_bExistSSCNET;
	bool        m_bExistEthercat;
    bool        m_bExistDGComm;
    ePswLevel   m_ePswLevel;
	AppTheme	m_theme;
private :
    void CreateImageList();
    bool ExistSSCNET ();
	bool ExistEthercat ();
    void SetMachine ();
	void GestionePswCanOpen ();
	void GestionePswEthercat ();
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon);
	CView* GetView (int nItem);
	CView* GetView (CRuntimeClass* pClass);
	CView* GetActiveView ();
	void SetTabStyle ();
	void DeleteView (CRuntimeClass* pClass);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGVIEW_H__086A6805_041B_4C15_A3FD_C7095791CC54__INCLUDED_)
