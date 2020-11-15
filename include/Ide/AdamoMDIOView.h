#if !defined(AFX_ADAMOMDIOVIEW_H__648EA516_A559_42CD_B9AF_223628E6347D__INCLUDED_)
#define AFX_ADAMOMDIOVIEW_H__648EA516_A559_42CD_B9AF_223628E6347D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* include files */
#include "iosupergrid.h"
#include "AdamoMDOscBar.h"
#include "DlgOscilloscope.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDIOView view

class CAdamoMDIOView : public CView
{
protected:
	CAdamoMDIOView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoMDIOView)

// Attributes
public:

// Operations
public:
    void OnAdamoTimer ();
    void OnViewSettings (stMDSettings* MDS);
    void GetViewSettings (stMDSettings* MDS);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDIOView)
	protected:
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoMDIOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoMDIOView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CIOSuperGrid      m_grid;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDIOVIEW_H__648EA516_A559_42CD_B9AF_223628E6347D__INCLUDED_)
