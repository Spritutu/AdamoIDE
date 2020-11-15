#if !defined(AFX_DLGADAPTERS_H__A6FA24EE_4FCC_49E1_AB53_913927CE3616__INCLUDED_)
#define AFX_DLGADAPTERS_H__A6FA24EE_4FCC_49E1_AB53_913927CE3616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgAdapters.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAdapters dialog

class CDlgAdapters : public CDialog
{
// Construction
public:
	CDlgAdapters(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CAdamoAboutBox)
	enum { IDD = IDD_DLG_ADAPTERS };
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnOK ();
	//}}AFX_VIRTUAL
// Implementation
public :
	void SetAdapters (CCmdGetAdapters *pAdapters)   {
		m_pAdapters = *pAdapters;
	}
	void SetCurrentAdapter (CString strCurrentAdapter)   {
		m_strCurrentAdapter = strCurrentAdapter;
	}
	CString GetCurrentAdapter ()   {
		return m_strCurrentAdapter;
	}
protected:
	// Generated message map functions
	//{{AFX_MSG(CAdamoAboutBox)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CListCtrl m_ctrlAdapters;
	CCmdGetAdapters m_pAdapters;
	CString	  m_strCurrentAdapter;
	CImageList m_images;
	int m_nCurrentAdapter;
private :
	void InitListViewAdapters ();
	void FillAdapters ();
	void SelectCurrentAdapter ();
	void SetImageList ();
	void OnDeleteAdapter ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCONNECTTOTCP_H__A6FA24EE_4FCC_49E1_AB53_913927CE3616__INCLUDED_)
