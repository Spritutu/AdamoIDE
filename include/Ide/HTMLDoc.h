#if !defined(AFX_HTMLDOC_H__4CB06B4D_29E1_4B22_9BB5_4232C7CCFAFB__INCLUDED_)
#define AFX_HTMLDOC_H__4CB06B4D_29E1_4B22_9BB5_4232C7CCFAFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HTMLDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHTMLDoc document

class CHTMLDoc : public CDocument
{
protected:
	CHTMLDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CHTMLDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHTMLDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
    virtual void OnCloseDocument();

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHTMLDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CHTMLDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLDOC_H__4CB06B4D_29E1_4B22_9BB5_4232C7CCFAFB__INCLUDED_)
