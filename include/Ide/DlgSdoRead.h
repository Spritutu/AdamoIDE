#if !defined(AFX_DLGSDOREAD_H__1FF3F4DC_25B4_4BA4_85D0_A8451C913C1F__INCLUDED_)
#define AFX_DLGSDOREAD_H__1FF3F4DC_25B4_4BA4_85D0_A8451C913C1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSdoRead.h : header file
//

#include "SdoReadGrid.h"

#define ID_SDOREADTIMER 400
#define TYPE_VETTORE_CARATTERI  0
#define TYPE_VETTORE_STRINGA    1
#define TYPE_VETTORE_FLOAT      2
#define TYPE_VETTORE_DOUBLE     3
#define TYPE_VETTORE_CHAR       4
#define TYPE_VETTORE_BYTE       5
#define TYPE_VETTORE_SHORT      6
#define TYPE_VETTORE_WORD       7
#define TYPE_VETTORE_INT        8
#define TYPE_VETTORE_DWORD      9
#define TYPE_VETTORE_40BIT      10
#define TYPE_VETTORE_48BIT      11

/////////////////////////////////////////////////////////////////////////////
// CDlgSdoRead dialog

class CDlgSdoRead : public CDialog
{
    enum eTVSL   {
        evtHex,
        evtDec,
        evtOct
    };
    struct stSDOGridInfo   {
        int nRow;
        int nType;
    };
// Construction
public:
	CDlgSdoRead(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSdoRead)
	enum { IDD = IDD_DLG_SDO_READ };
	CButton	m_btnResetReadSdo;
	CButton	m_btnDeleteSdo;
	CButton	m_btnReadSdo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSdoRead)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSdoRead)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBtnSdoReadDelete();
	afx_msg void OnBtnSdoRead();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnSdoResetRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoMachine *GetMachine();
    int  GetSDORequests (stSDORequest *pRequests);
    bool GetRowData (int nRow, stSDORequest *pRequests);
    int  GetReadData (int nRow, CString &str, int *l, int *t);
    void StartSDOMonitoring ();
    void ShowResults(stSDOResults *pResults);
    eTVSL GetTipoDiVisualizzazione (int nRow);
    void OnOK ();
    void OnCancel ();
private :
    CSdoReadGrid    m_ctrlSdoRead;
    stSDOGridInfo   m_stSDOGridInfo[NMAX_READ_WRITE_SDO];
    int             m_nCount;
    UINT            m_nTimer;
    bool            m_bInitialized;
    bool            m_bSDOMonitoring;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSDOREAD_H__1FF3F4DC_25B4_4BA4_85D0_A8451C913C1F__INCLUDED_)
