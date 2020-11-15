#if !defined(AFX_ADAMOFINDINFILES_H__695AAAF3_0049_4ECE_90F8_0B6BE0F26FB2__INCLUDED_)
#define AFX_ADAMOFINDINFILES_H__695AAAF3_0049_4ECE_90F8_0B6BE0F26FB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoFindInFiles.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoFindInFiles dialog

class CAdamoFindInFiles : public CDialog
{
// Construction
public:
	CAdamoFindInFiles(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdamoFindInFiles)
	enum { IDD = IDD_FINDINFILES };
	CComboBox	m_ctrlInFolder;
	CComboBox	m_ctrlFileType;
	CComboBox	m_ctrlSearchtext;
	BOOL	m_bMatchcase;
	BOOL	m_bMatchregex;
	BOOL	m_bMatchwords;
	//}}AFX_DATA
	CLuaEditor *m_pEdit;   // editcontrol to use
    CString m_strFind, m_strFileType, m_strFolder; 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoFindInFiles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoFindInFiles)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnFind();
	afx_msg void OnBtnSearchFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void StoreStrings (CIdeApp *app, LPCSTR szSearch, LPCSTR szFileType, LPCSTR szFolder);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOFINDINFILES_H__695AAAF3_0049_4ECE_90F8_0B6BE0F26FB2__INCLUDED_)
