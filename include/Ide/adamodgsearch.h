#if !defined(AFX_ADAMODGSEARCH_H__18F5D4A7_D8AE_4414_9D28_EA09447D45F5__INCLUDED_)
#define AFX_ADAMODGSEARCH_H__18F5D4A7_D8AE_4414_9D28_EA09447D45F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGSearch.h : header file
//

class CAdamoDGComm;

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSearch dialog

class CAdamoDGSearch : public CDialog
{
// Construction
public:
	CAdamoDGSearch(CWnd* pParent = NULL);   // standard constructor
    void SetDGView (CAdamoDGComm* pDGComm)   { m_pDGComm=pDGComm; }
// Dialog Data
	//{{AFX_DATA(CAdamoDGSearch)
	enum { IDD = IDD_DLGFINDDIAGNOSTIC };
	CComboBox	m_ctrlSearchText;
	BOOL	m_bMatchCase;
	BOOL	m_bMatchWords;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGSearch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAdamoDGSearch)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnNext();
	afx_msg void OnPrev();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoDGComm* m_pDGComm;
private :
    void StoreSearchString (CIdeApp *pApp, LPCSTR szSearch);
    void Find (const char* sz, int nDirection);
    void OnOK ();
    void OnCancel ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGSEARCH_H__18F5D4A7_D8AE_4414_9D28_EA09447D45F5__INCLUDED_)
