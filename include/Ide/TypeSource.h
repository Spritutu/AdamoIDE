#if !defined(AFX_TYPESOURCE_H__02873347_E14A_4CF0_A5EC_466DE3FF331A__INCLUDED_)
#define AFX_TYPESOURCE_H__02873347_E14A_4CF0_A5EC_466DE3FF331A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TypeSource.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTypeSource dialog

class CTypeSource : public CDialog
{
// Construction
public:
	CTypeSource(CWnd* pParent = NULL);   // standard constructor
    CString GetResult ()   { return m_strResult; }

// Dialog Data
	//{{AFX_DATA(CTypeSource)
	enum { IDD = IDD_DLGTYPESOURCE };
	CListCtrl	  m_lvTypeSource;
    CEdit         m_editFileName;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTypeSource)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTypeSource)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CImageList* m_hlviml;
    CBitmap*    m_bmpSAda;
    CString     m_strResult;
private :
    void SetLvStyle ();
    void AddLvItems ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYPESOURCE_H__02873347_E14A_4CF0_A5EC_466DE3FF331A__INCLUDED_)
