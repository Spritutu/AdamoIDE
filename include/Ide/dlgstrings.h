#if !defined(AFX_DLGSTRINGS_H__3D6E13CB_674A_4378_AB94_8C7D5DC405B2__INCLUDED_)
#define AFX_DLGSTRINGS_H__3D6E13CB_674A_4378_AB94_8C7D5DC405B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgstrings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStrings dialog

class CDlgStrings : public CDialog
{
// Construction
public:
	CDlgStrings(CWnd* pParent = NULL);   // standard constructo
	~CDlgStrings ();
    void SetData (CPtrList* pData)   { m_pData=pData; }
    void FillList ();

// Dialog Data
	//{{AFX_DATA(CDlgStrings)
	enum { IDD = IDD_LISTSTRINGS };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStrings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStrings)
	virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CImageList* m_iml;
    CBitmap*    m_bmpString;
    CBitmap*    m_bmpPriority;
    CBitmap*    m_bmpDateTime;
    CPtrList*   m_pData;
	HD_ITEM*		m_hd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTRINGS_H__3D6E13CB_674A_4378_AB94_8C7D5DC405B2__INCLUDED_)
