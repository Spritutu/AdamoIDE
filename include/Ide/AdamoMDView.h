#if !defined(AFX_ADAMOMDVIEW_H__9A79B351_ACE1_4C8F_8C55_F405DE0A2FE7__INCLUDED_)
#define AFX_ADAMOMDVIEW_H__9A79B351_ACE1_4C8F_8C55_F405DE0A2FE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoMDTabbedView.h : header file
//

#include "AdamoMDAxesView.h"
#include "AdamoMDSplittedView.h"

#define IDC_TABMDCONTROL 2001

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDView view

class CAdamoMDView : public CView
{
protected:
	CAdamoMDView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoMDView)
public:
    static CAdamoMDView* GetMDView ()   { return m_pThis; }
    CAdamoMDAxesView*       GetAxesView ();
    CAdamoMDSplittedView*   GetIOView ();
// Attributes
public:

// Operations
public:
    void Activate();
    void OnClose ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDView)
	protected:
	//}}AFX_VIRTUAL
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view

// Implementation
protected:
	virtual ~CAdamoMDView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoMDView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnRefreshMetrics(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CXTPTabControl m_wndTabControl;
    CImageList m_il;
    static CAdamoMDView* m_pThis;
    int m_nAxes;
    ePswLevel       m_ePswLevel;
	AppTheme		m_theme;
private :
    bool ExistAxis ();
    void Iterator(CAdamoGroup *pGrp, CAdamoDatiFile *pFile, bool bIsPhysical);
    void RefreshData ();
    void CreateImageList ();
	void GestionePswOscilloscopio ();
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle, int nIcon);
	CView* GetView (int nItem);
	CView* GetActiveView ();
	void SetTabStyle ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDTABBEDVIEW_H__9A79B351_ACE1_4C8F_8C55_F405DE0A2FE7__INCLUDED_)
