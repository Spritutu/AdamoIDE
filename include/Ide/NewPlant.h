#if !defined(AFX_NEWPLANT_H__9FE53D88_21D3_47F1_98FC_6673AE57EB85__INCLUDED_)
#define AFX_NEWPLANT_H__9FE53D88_21D3_47F1_98FC_6673AE57EB85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPlant.h : header file
//

/* include files */
#include "paredit.h"

/////////////////////////////////////////////////////////////////////////////
// CNewPlant dialog

class CNewPlant : public CDialog
{
// Construction
public:
	CNewPlant(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewPlant)
	enum { IDD = IDD_NEW_PLANT };
	CParsedEdit	m_ctrlName;
	CParsedEdit	m_ctrlDesc;
	//}}AFX_DATA

    void	SetName (CString strName)			{ m_strName = strName; }
    void    SetDesc (CString strDescription)	{ m_strDesc = strDescription; }
    CString GetName ()   { return m_strName; }
    CString GetDesc ()   { return m_strDesc; }
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPlant)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewPlant)
		// NOTE: the ClassWizard will add member functions here
        virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void OnOK ();
private :
    CString m_strName;
    CString m_strDesc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPLANT_H__9FE53D88_21D3_47F1_98FC_6673AE57EB85__INCLUDED_)
