#if !defined(AFX_DLGSDOWRITE_H__B0BE7E85_7E41_40B6_9E21_C2E9A338F676__INCLUDED_)
#define AFX_DLGSDOWRITE_H__B0BE7E85_7E41_40B6_9E21_C2E9A338F676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSdoWrite.h : header file
//

#include "SdoWriteGrid.h"

#define ID_SDOWRITETIMER 400

/////////////////////////////////////////////////////////////////////////////
// CDlgSdoWrite dialog

class CDlgSdoWrite : public CDialog
{
    struct stSDOGridInfo   {
        int nRow;
        int nType;
    };
// Construction
public:
	CDlgSdoWrite(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSdoWrite)
	enum { IDD = IDD_DLG_SDO_WRITE };
	CButton	m_btnResetWriteSdo;
	CButton	m_btnDeleteSdo;
	CButton	m_btnWriteSdo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSdoWrite)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSdoWrite)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnSdoWriteDelete();
	afx_msg void OnBtnSdoWrite();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnSdoResetWrite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoMachine *GetMachine();
    int  GetSDORequests (stSDORequest *pRequests);
    void StartSDOMonitoring ();
    bool GetRowData (int nRow, stSDORequest *pRequests);
    int  GetWriteData (int nRow, CString &str, int *l, int *t, BYTE *p);
    void ShowResults(stSDOResults *pResults);
    void OnOK ();
    void OnCancel ();
private :
    CSdoWriteGrid m_ctrlSdoWrite;
    stSDOGridInfo m_stSDOGridInfo[NMAX_READ_WRITE_SDO];
    bool          m_bInitialized;
    bool          m_bSDOMonitoring;
    UINT          m_nTimer;
    int           m_nCount;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSDOWRITE_H__B0BE7E85_7E41_40B6_9E21_C2E9A338F676__INCLUDED_)
