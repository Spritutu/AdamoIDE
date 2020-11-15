#if !defined(AFX_DLGIMPORTIMAGE_H__F69E601F_25F8_44EA_9836_2AE717E223D3__INCLUDED_)
#define AFX_DLGIMPORTIMAGE_H__F69E601F_25F8_44EA_9836_2AE717E223D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgimportimage.h : header file
//

/* include files */
#include "path.h"

/* namespace */
using namespace nsPath;

/////////////////////////////////////////////////////////////////////////////
// CDlgImportImage dialog

class CDlgImportImage : public CDialog
{
// Construction
public:
	CDlgImportImage(CWnd* pParent = NULL);   // standard constructor
    CString GetID ()        { return m_strID; }
    CPath   GetPath ()      { return m_strPath; }
// Dialog Data
	//{{AFX_DATA(CDlgImportImage)
	enum { IDD = IDD_RS_IMPORTBITMAP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgImportImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgImportImage)
	afx_msg void OnBtnChooseJpg();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CParsedEdit	m_editID;
    CString m_strID;
    CPath   m_strPath;
private :
    void OnOK ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIMPORTIMAGE_H__F69E601F_25F8_44EA_9836_2AE717E223D3__INCLUDED_)
