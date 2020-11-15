#if !defined(AFX_OPENSOURCE_H__C4971F19_9718_47C9_8BF0_ADC651B97D16__INCLUDED_)
#define AFX_OPENSOURCE_H__C4971F19_9718_47C9_8BF0_ADC651B97D16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenSource.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenSource dialog

class COpenSource : public CDialog
{
// Construction
public:
	COpenSource(CWnd* pParent = NULL);   // standard constructor
    CString GetResult ()   { return m_strResult; }
// Dialog Data
	//{{AFX_DATA(COpenSource)
	enum { IDD = IDD_DLGOPENSOURCE };
	CListCtrl	m_lvShowSource;
	CListCtrl	m_lvTypeSource;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenSource)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenSource)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTSItemChanged (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblClkShowSource (NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CImageList* m_hlviml;
    CBitmap*    m_bmpSAda;
    CString     m_strResult;
private :
    void SetLvStyle ();
    void AddLvItems ();
    void LoadFiles (CString strExtension);
    void AddItem (CString strFileName, int nIndex);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENSOURCE_H__C4971F19_9718_47C9_8BF0_ADC651B97D16__INCLUDED_)
