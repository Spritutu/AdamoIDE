/*
** AdamoDbgObjectView.h
*/

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDbgVarView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgVarView view

/* include files */
#include "AdamoTablesSupergrid.h"
#include "AdamoMachine.h"

class CAdamoDbgObjectView : public CView
{
protected:
	CAdamoDbgObjectView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoDbgObjectView)

// Attributes
public:

// Operations
public:
    void SetMachine (CAdamoMachine* pMachine) { m_pMachine = pMachine; m_wndReport.SetMachine (pMachine); }
	void LoadData ();
	void ClearData ();
	void OnAdamoTimer ();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDbgVarView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoDbgObjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoDbgVarView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CTablesSuperGrid m_wndReport;
	CAdamoMachine* m_pMachine;
	bool m_bSymbolsLoaded;
};

/////////////////////////////////////////////////////////////////////////////
