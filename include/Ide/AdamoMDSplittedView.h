#if !defined(AFX_ADAMOMDSPLITTEDVIEW_H__BF6FB336_F846_4885_99FB_1FBFF8AC9058__INCLUDED_)
#define AFX_ADAMOMDSPLITTEDVIEW_H__BF6FB336_F846_4885_99FB_1FBFF8AC9058__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoMDSplittedView.h : header file
//

#include "AdamoMDIOViewDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDSplittedView view

class CAdamoMDSplittedView : public CView
{
protected:
	CAdamoMDSplittedView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoMDSplittedView)
    
// Attributes
public:

// Operations
public:
    void OnAdamoTimer();
    void OnViewSettings (stMDSettings* MDS);
    void GetViewSettings (stMDSettings* MDS);
    CAdamoMDIOViewDlg* GetIOViewDlg ()   { return m_pIODlg; }
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDSplittedView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoMDSplittedView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoMDSplittedView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoMDIOViewDlg* m_pIODlg;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDSPLITTEDVIEW_H__BF6FB336_F846_4885_99FB_1FBFF8AC9058__INCLUDED_)
