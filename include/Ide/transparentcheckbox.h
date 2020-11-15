#if !defined(AFX_TRANSPARENTCHECKBOX_H__172503EB_1F5B_44A5_9E73_FADEB603FE29__INCLUDED_)
#define AFX_TRANSPARENTCHECKBOX_H__172503EB_1F5B_44A5_9E73_FADEB603FE29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransparentCheckBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransparentCheckBox window

class CTransparentCheckBox : public CButton
{
// Construction
public:
	CTransparentCheckBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransparentCheckBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTransparentCheckBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTransparentCheckBox)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private :
    CFont* m_pFnt;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSPARENTCHECKBOX_H__172503EB_1F5B_44A5_9E73_FADEB603FE29__INCLUDED_)
