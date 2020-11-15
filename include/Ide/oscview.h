#if !defined(AFX_OSCVIEW_H__19D84460_FA90_48A2_AC65_3532D0A7DA01__INCLUDED_)
#define AFX_OSCVIEW_H__19D84460_FA90_48A2_AC65_3532D0A7DA01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OscView.h : header file
//

#include "dlgtriggers.h"
#include "dlgsamples.h"
#include "dlgoscilloscope.h"

/////////////////////////////////////////////////////////////////////////////
// COscView view

class COscView : public CView
{
protected:
	COscView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COscView)

// Attributes
public:
    int  GetCurSel ()       { return m_nCurSel; }
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COscView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COscView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(COscView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CXTPTabControl*			 m_pFlatTabCtrl;
    CDlgTriggers*            m_pTriggers;
    CDlgSamples*             m_pSamples;
    CDlgOscilloscope*        m_pOscilloscope;
    int                      m_nCurSel;
private :
    void CreateTab ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OSCVIEW_H__19D84460_FA90_48A2_AC65_3532D0A7DA01__INCLUDED_)
