#if !defined(AFX_PREVIEWRESOURCE_H__308F0821_1F02_4663_A1ED_1260762FF09E__INCLUDED_)
#define AFX_PREVIEWRESOURCE_H__308F0821_1F02_4663_A1ED_1260762FF09E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewResource.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewResource html view

#include "AdamoSynContainer.h"

class CPreviewResource : public CView
{
protected:
	CPreviewResource();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPreviewResource)

// html Data
public:
	//{{AFX_DATA(CPreviewResource)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	int LoadPreviewPage (CString strResource, CString strXML);
	void OnAdamoTimer ();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewResource)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL
// Implementation
protected:
	virtual ~CPreviewResource();
	virtual void OnInitialUpdate();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPreviewResource)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CAdamoSynContainer m_SynContainer;
private :
	void AddContainerStrings ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWRESOURCE_H__308F0821_1F02_4663_A1ED_1260762FF09E__INCLUDED_)
