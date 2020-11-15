#if !defined(AFX_CREATESLOT_H__191E3C59_A8E9_49EE_9B04_BC672CD1D143__INCLUDED_)
#define AFX_CREATESLOT_H__191E3C59_A8E9_49EE_9B04_BC672CD1D143__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CreateSlot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateSlot dialog

class CCreateSlot : public CDialog
{
// Construction
public:
	CCreateSlot(CWnd* pParent = NULL);   // standard constructor
    void SetBrowserContainer (CBrowserContainer *pbc)   { m_bc=pbc; }
// Dialog Data
	//{{AFX_DATA(CCreateSlot)
	enum { IDD = IDD_DLG_CREATESLOT };
	CListCtrl	m_ctrlCreateSlotList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateSlot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateSlot)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSlotCreate();
	afx_msg void OnSlotGoto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void InitTitles();
    void InitList();
    void LoadFunctions();
    void AddFunction (int nIndex, CString strFunction, CString strModule, int nLine);
    int  GetIndex(CString strInsert);
private :
    CBrowserContainer *m_bc;
    CImageList *m_iml;
    CBitmap    *m_bmpSlot;
    CBitmap    *m_bmpFunction;
    CBitmap    *m_bmpFile;
    CBitmap    *m_bmpLine;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATESLOT_H__191E3C59_A8E9_49EE_9B04_BC672CD1D143__INCLUDED_)
