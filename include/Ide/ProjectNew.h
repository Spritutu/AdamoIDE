#if !defined(AFX_PROJECTNEW_H__D316F089_267A_4279_83F8_3A36D0408563__INCLUDED_)
#define AFX_PROJECTNEW_H__D316F089_267A_4279_83F8_3A36D0408563__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectNew.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProjectNew dialog

class CProject;

class CProjectNew : public CDialog
{
// Construction
public:
	CString GetProjectName();
    eTypeMachine GetTypeMachine ()       { return m_eTypeMachine; }
    int GetModuleNumber ()               { return m_nModule; }
	CProjectNew(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CProjectNew)
	enum { IDD = IDD_NEW_PROJECT };
	CParsedEdit	m_ctrlEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectNew)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    eTypeMachine m_eTypeMachine;
    CString m_strName;
    int m_nModule;
	// Generated message map functions
	//{{AFX_MSG(CProjectNew)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioSim();
	afx_msg void OnRadioPhysic();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTNEW_H__D316F089_267A_4279_83F8_3A36D0408563__INCLUDED_)
