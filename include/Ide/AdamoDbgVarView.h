#if !defined(AFX_ADAMODBGVARVIEW_H__4C340022_1D76_4F1E_A856_95B86608581B__INCLUDED_)
#define AFX_ADAMODBGVARVIEW_H__4C340022_1D76_4F1E_A856_95B86608581B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDbgVarView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoDbgVarView view

/* include files */
#include "gridctrl.h"
#include "gridcellcheck.h"
#include "gridcelltogglebutton.h"
#include "AdamoMachine.h"

/* constants definitions */
#define NMAXDBGVARSCOLS 2

class CAdamoDbgVarView : public CView
{
protected:
	CAdamoDbgVarView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoDbgVarView)

// Attributes
public:

// Operations
public:
    void SetMachine (CAdamoMachine* pMachine) { m_pMachine=pMachine; }
    void UpdateData ();
    void OnAdamoTimer ();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDbgVarView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoDbgVarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoDbgVarView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridButtonPressed(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private :
    CGridCtrl  m_Grid;
    CImageList m_ImageList;
    CAdamoMachine* m_pMachine;
    int m_nCurRow;
    int m_nCurColumn;
    int m_nIniRow;
    int m_nGridRow;
private :
    void FillColumnsCaption ();
    void FillVariables ();
    void InitializeGrid ();
    void Clear ();
    int  InsertRow (CString str);
    void SpecilizeRow (stGlobalDescriptor* gd, int nRow);
    void LoadStrings ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODBGVARVIEW_H__4C340022_1D76_4F1E_A856_95B86608581B__INCLUDED_)
