#if !defined(AFX_ADAMODBGIOVIEW_H__AA55219D_6F27_4C13_BA82_3D15FF02C2BC__INCLUDED_)
#define AFX_ADAMODBGIOVIEW_H__AA55219D_6F27_4C13_BA82_3D15FF02C2BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDbgIOView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgIOView view

/* include files */
#include "iosupergrid.h"

class CAdamoMachine;

class CAdamoDbgIOView : public CView
{
protected:
	CAdamoDbgIOView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoDbgIOView)

// Attributes
public:
    void OnAdamoTimer ();
    static CAdamoDbgIOView* GetDbgIOView ()   { return m_pThis; }
    void SetMachine (CAdamoMachine* pMachine) { m_grid.SetMachine (pMachine); }
    void UpdateData ();
    void UnInitialize ();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDbgIOView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoDbgIOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoDbgIOView)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    static CAdamoDbgIOView* m_pThis;
    CIOSuperGrid            m_grid;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODBGIOVIEW_H__AA55219D_6F27_4C13_BA82_3D15FF02C2BC__INCLUDED_)
