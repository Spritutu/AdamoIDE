#if !defined(AFX_DIALOGSLOT_H__E2CC7810_F01B_480E_A93B_7AF0B326BD95__INCLUDED_)
#define AFX_DIALOGSLOT_H__E2CC7810_F01B_480E_A93B_7AF0B326BD95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSlot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSlot dialog

class CDialogSlot : public CDialog
{
// Construction
public:
	CDialogSlot(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSlot)
	enum { IDD = IDD_DLGSELECTSLOT };
	CString	m_szSlotName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSlot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSlot)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSLOT_H__E2CC7810_F01B_480E_A93B_7AF0B326BD95__INCLUDED_)
