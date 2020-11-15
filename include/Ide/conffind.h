#if !defined(AFX_CONFFIND_H__547C70E2_13F9_4128_94CA_0C4B9C080AAA__INCLUDED_)
#define AFX_CONFFIND_H__547C70E2_13F9_4128_94CA_0C4B9C080AAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// conffind.h : header file
//

class CConfTreeView;
class CIdeApp;

/////////////////////////////////////////////////////////////////////////////
// CConfFind dialog

class CConfFind : public CDialog
{
// Construction
public:
	CConfFind(CWnd* pParent = NULL);   // standard constructor
    void SetConfTreeView (CConfTreeView* pCTV)   { m_pCTV=pCTV; }
// Dialog Data
	//{{AFX_DATA(CConfFind)
	enum { IDD = IDD_DLGCONFFIND };
	CComboBox	m_ctrlSearchText;
	BOOL	m_bMatchCase;
	BOOL	m_bMatchWords;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfFind)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfFind)
	virtual BOOL OnInitDialog();
	afx_msg void OnNext();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CConfTreeView* m_pCTV;
    bool m_bs;
    CString m_strLastFind;
private :
    void OnOK ();
    void OnCancel ();
    void OnClose ();
    void StoreSearchString(CIdeApp *app, LPCSTR szSearch);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFFIND_H__547C70E2_13F9_4128_94CA_0C4B9C080AAA__INCLUDED_)
