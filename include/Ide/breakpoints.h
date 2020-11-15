#if !defined(AFX_BREAKPOINTS_H__3DF8B8E9_1986_4AEC_9ED8_F830E5304EA7__INCLUDED_)
#define AFX_BREAKPOINTS_H__3DF8B8E9_1986_4AEC_9ED8_F830E5304EA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// breakpoints.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBreakpoints dialog

class CBreakpoints : public CDialog
{
// Construction
public:
	CBreakpoints(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBreakpoints)
	enum { IDD = IDD_DLG_BREAKPOINTS };
	CListCtrl	m_ctrlBreakPointsList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBreakpoints)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBreakpoints)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnGoto();
	afx_msg void OnBtnRemove();
	afx_msg void OnBtnRemoveAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void InitTitles ();
    void InitList ();
    void LoadBreakPoints ();
    void AddBreakPoint (int nIndex, CString strFile, int nLine);
private :
    CImageList *m_iml;
    CBitmap    *m_bmpFile;
    CBitmap    *m_bmpLine;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BREAKPOINTS_H__3DF8B8E9_1986_4AEC_9ED8_F830E5304EA7__INCLUDED_)
