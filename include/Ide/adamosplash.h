#if !defined(AFX_ADAMOSPLASH_H__049605F8_27F1_496C_87C9_AF848B7557BF__INCLUDED_)
#define AFX_ADAMOSPLASH_H__049605F8_27F1_496C_87C9_AF848B7557BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoSplash.h : header file
//
#define DIMENSIONE_SPLASH 750

/////////////////////////////////////////////////////////////////////////////
// CAdamoSplash dialog

class CAdamoSplash : public CDialog
{
// Construction
public:
	CAdamoSplash(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdamoSplash)
	enum { IDD = IDD_DLG_SPLASH };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoSplash)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoSplash)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	void InitVersionLabel();
private :
    CBrush m_bkg;
	CLabel m_ctlVersion;
	double m_fpSizeX;
	double m_fpSizeY;
	bool m_bInitialized;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOSPLASH_H__049605F8_27F1_496C_87C9_AF848B7557BF__INCLUDED_)
