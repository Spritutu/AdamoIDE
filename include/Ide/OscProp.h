#if !defined(AFX_OSCPROP_H__39F48575_D78C_4066_A205_05F0279FF137__INCLUDED_)
#define AFX_OSCPROP_H__39F48575_D78C_4066_A205_05F0279FF137__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OscProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COscProp dialog

class COscProp : public CDialog
{
// Construction
public:
	COscProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COscProp)
	enum { IDD = IDD_DLGOSCPROP };
	int		m_nMode;
	int		m_nTriggerAnd;
	//}}AFX_DATA

    int m_nFrequency;
    int m_nDimBuffer;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COscProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COscProp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void OnOK();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OSCPROP_H__39F48575_D78C_4066_A205_05F0279FF137__INCLUDED_)
