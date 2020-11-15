#if !defined(AFX_GLOBALARR_H__95BA7514_0916_48BF_A1C1_12946C0D1BAA__INCLUDED_)
#define AFX_GLOBALARR_H__95BA7514_0916_48BF_A1C1_12946C0D1BAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GlobalArr.h : header file
//

#include "gridctrl.h"
#include "gridcellcheck.h"
#include "adamoint.h"
#include "lobject.h"
#include "adamomachine.h"

#define NMAX_GRIDELEMENTS 16

#define ROW_HEIGTH_W7   22
#define ROW_HEIGTH_XP	19

/////////////////////////////////////////////////////////////////////////////
// CGlobalArr dialog

class CGlobalArr : public CAdamoDialog
{
// Construction
public:
	CGlobalArr(CWnd* pParent = NULL);   // standard constructor
    void SetData (const char* szArrayName, int nType, int nElements);
    void OnTimer ();
// Dialog Data
	//{{AFX_DATA(CGlobalArr)
	enum { IDD = IDD_GLOBALARRAY };
	CButton	m_ctrlButOk;
	CButton	m_ctrlButFreeze;
	CButton	m_ctrlButUpdate;
	CStatic	m_ctrlName;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlobalArr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGlobalArr)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnArrayFreeze();
	afx_msg void OnClose();
	//}}AFX_MSG
    afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private :
    void FillCaption ();
    void FillElements ();
    void ResizeDialog (int nElements);
    void OnOK ();
    void OnCancel ();
private :
    CAdamoMachine* m_pMachine;
    CImageList m_ImageList;
    CGridCtrl m_Grid;
    CString m_strArrayName;
    int  m_nType;
    int  m_nElements;
    bool m_bFreeze;
    int  m_nRow;
    int  m_nColumn;
public:
	afx_msg void OnBnClickedArrayaggiorna();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBALARR_H__95BA7514_0916_48BF_A1C1_12946C0D1BAA__INCLUDED_)
