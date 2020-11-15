#pragma once

/////////////////////////////////////////////////////////////////////////////
// CHelpOnLineView view

class CHtmlHelpOnLineView : public CHtmlView
{
protected:
	CHtmlHelpOnLineView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHtmlHelpOnLineView)
	
// Attributes
// Operations
public:
	int ViewHelpPage (CString strHelpPage);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpOnLineView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CHtmlHelpOnLineView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
public:
	// Generated message map functions
protected:
	//{{AFX_MSG(CHelpOnLineView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
