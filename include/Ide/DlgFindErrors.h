#if !defined(AFX_DLGFINDERRORS_H__3DBD29A7_81AF_4563_B8F0_DE3596ECF445__INCLUDED_)
#define AFX_DLGFINDERRORS_H__3DBD29A7_81AF_4563_B8F0_DE3596ECF445__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFindErrors.h : header file
//

/* include files */
#include "AdamoSEView.h"
#include "adamoSEListView.h"

class CIdeApp;

/////////////////////////////////////////////////////////////////////////////
// CDlgFindErrors dialog

class CDlgFindErrors : public CDialog
{
// Construction
public:
	CDlgFindErrors(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFindErrors)
	enum { IDD = IDD_FINDERRORS };
	CComboBox	m_ctrlSearchText;
	BOOL	m_bMatchCase;
	BOOL	m_bMatchWords;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFindErrors)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
    void SetSEView (CAdamoSEView* pSEView)   { m_pSEView=pSEView; }
protected:
    void OnOK ();
    void OnCancel ();
	// Generated message map functions
	//{{AFX_MSG(CDlgFindErrors)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnNext();
	afx_msg void OnPrev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoSEView* m_pSEView;
private :
    void StoreSearchString (CIdeApp* pApp, LPCSTR szSearch);
    void Find (const char* sz, int nDirection);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFINDERRORS_H__3DBD29A7_81AF_4563_B8F0_DE3596ECF445__INCLUDED_)
