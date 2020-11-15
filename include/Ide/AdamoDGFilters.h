#if !defined(AFX_ADAMODGFILTERS_H__22C1EFF5_61E4_4E98_92D1_97FB63AEDA48__INCLUDED_)
#define AFX_ADAMODGFILTERS_H__22C1EFF5_61E4_4E98_92D1_97FB63AEDA48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGFilters.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGFilters dialog

class CAdamoDGFilters : public CDialog
{
// Construction
public:
	CAdamoDGFilters(CWnd* pParent = NULL);   // standard constructor
    void SetTreeView (CTreeView* tv)        { m_tv=tv; }
// Dialog Data
	//{{AFX_DATA(CAdamoDGFilters)
	enum { IDD = IDD_DG_FILTER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGFilters)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
    void OnOK ();

protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoDGFilters)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonResettaFiltri();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CParsedEdit m_editFilterDown;
    CParsedEdit m_editFilterUp;
    CTreeView*  m_tv;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGFILTERS_H__22C1EFF5_61E4_4E98_92D1_97FB63AEDA48__INCLUDED_)
