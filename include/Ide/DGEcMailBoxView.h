#if !defined(AFX_DGECMAILBOXVIEW_H__E1D17B82_003A_43AA_AEDB_B0E28AE95E37__INCLUDED_)
#define AFX_DGECMAILBOXVIEW_H__E1D17B82_003A_43AA_AEDB_B0E28AE95E37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGEcMailBoxView.h : header file
//

#include "DlgEcMailBoxRead.h"
#include "DlgEcMailBoxWrite.h"

/////////////////////////////////////////////////////////////////////////////
// CDGEcMailBoxView view

class CDGEcMailBoxView : public CView
{
protected:
	CDGEcMailBoxView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDGEcMailBoxView)
	
// Attributes
public:
	void LoadEthercatXMLFile (CPtrArray &slaveArray);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDGEcMailBoxView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDGEcMailBoxView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDGEcMailBoxView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CXTPTabControl*			 m_pFlatTabCtrl;
    CDlgEcMailBoxRead*       m_pEcMailBoxRead;
    CDlgEcMailBoxWrite*      m_pEcMailBoxWrite;
    int                      m_nCurSel;
private :
    void CreateTab ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGECMAILBOXVIEW_H__E1D17B82_003A_43AA_AEDB_B0E28AE95E37__INCLUDED_)
