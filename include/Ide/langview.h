#if !defined(AFX_LANGVIEW_H__D353B894_4119_4695_B11B_8426807DCB8C__INCLUDED_)
#define AFX_LANGVIEW_H__D353B894_4119_4695_B11B_8426807DCB8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LangView.h : header file
//

#include "AdamoTraduzione.h"

/////////////////////////////////////////////////////////////////////////////
// CLangView view

class CLangView : public CListView
{
protected:
	CLangView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLangView)

// Attributes
public:

// Operations
public:
    int  ActivateMachine ();
    int  AddMessage ();
    int  DeleteMessage ();
    int  ModifyMessage ();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLangView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLangView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLangView)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CTraduzioni* m_tr;
    CImageList*  m_hciml;
    CImageList*  m_hcLV;
    CBitmap* m_bmpHdrID;
    CBitmap* m_bmpHdrValue;
    CBitmap* m_bmpHdrString;
    CBitmap* m_bmpHdrDesc;
    CBitmap* m_bmpLV;
	int		 m_nSortColumn;
private :
    void SetHeaderLook ();
    void LoadStrings ();
    bool IsEmptyRow (int n);
    long GetPrevRowNumber (int n);
    void GetRowData (int n, CString* pStringa, long* pNumero, CString* pMessaggio, CString* pDescrizione);
    int  GetNextValidNumber (int n);
    int  AddRow (int n, CString strID, long dwNumero, CString strMessaggio, CString strDescrizione);
    int  ModifyRow (int n, CString strID, CString strMessaggio, CString strDescrizione);
    bool EsisteNumero (long l);
    bool EsisteStringa (CString str);
	static int CALLBACK SortColumns (LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
public:
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANGVIEW_H__D353B894_4119_4695_B11B_8426807DCB8C__INCLUDED_)
