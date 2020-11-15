#if !defined(AFX_ERRORSCOMBOBOX_H__511DD370_0B26_45A9_93DF_BAE87420F911__INCLUDED_)
#define AFX_ERRORSCOMBOBOX_H__511DD370_0B26_45A9_93DF_BAE87420F911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorsComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CErrorsComboBox window

class CErrorsComboBox : public CXTPControlComboBox
{
// Construction
public:
	CErrorsComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CErrorsComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CErrorsComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CErrorsComboBox)
	//}}AFX_MSG
	HBRUSH OnCtlColor(CDC* pDC, UINT nCtlColor);
	void Draw(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORSCOMBOBOX_H__511DD370_0B26_45A9_93DF_BAE87420F911__INCLUDED_)
