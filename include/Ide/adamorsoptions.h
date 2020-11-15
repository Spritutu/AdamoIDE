#if !defined(AFX_ADAMORSOPTIONS_H__F06631B6_8C79_49BA_947E_A8C65B5795B8__INCLUDED_)
#define AFX_ADAMORSOPTIONS_H__F06631B6_8C79_49BA_947E_A8C65B5795B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoRSOptions.h : header file
//

#include "AdamoRSObjects.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSOptions dialog

class CAdamoRSOptions : public CDialog
{
// Construction
public:
	CAdamoRSOptions(CWnd* pParent = NULL);   // standard constructor
    void SetOptions (stRSOpzioni* pOptions, int nNumOptions)    { m_pOptions=pOptions; m_nNumOptions=nNumOptions; }
    stRSOpzioni* GetOptions (int* pNumOptions)                  { *pNumOptions=m_nNumOptions; return m_pOptions; }
    void GetItem (CString& strOption, bool& bSelected, CString& strValue, int nIndex);
// Dialog Data
	//{{AFX_DATA(CAdamoRSOptions)
	enum { IDD = IDD_DLG_RS_OPTIONS };
	CListCtrl	m_listOptions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoRSOptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoRSOptions)
	afx_msg void OnBtnRsAggiungi();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnRsRimuovi();
	afx_msg void OnBtnRsSpostaGiu();
	afx_msg void OnBtnRsSpostaSu();
	afx_msg void OnBtnRsModifica();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    int  AddItem (CString strOption, bool bSelected, CString strValue, int nIndex);
    void FillOptions ();
private :
    stRSOpzioni*  m_pOptions;
    int           m_nNumOptions;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMORSOPTIONS_H__F06631B6_8C79_49BA_947E_A8C65B5795B8__INCLUDED_)
