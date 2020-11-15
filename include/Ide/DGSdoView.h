/*
** DGSdoView.h
*/

#ifndef __ADAMO_DGSDOVIEW__
#define __ADAMO_DGSDOVIEW__

#include "DlgSdoRead.h"
#include "DlgSdoWrite.h"

class CDGSdoView : public CView
{
protected:
	CDGSdoView ();
	DECLARE_DYNCREATE(CDGSdoView)

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
	virtual ~CDGSdoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(COscView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CXTPTabControl*			 m_pFlatTabCtrl;
    CDlgSdoRead*             m_pSdoRead;
    CDlgSdoWrite*            m_pSdoWrite;
    int                      m_nCurSel;
private :
    void CreateTab ();
};

#endif
