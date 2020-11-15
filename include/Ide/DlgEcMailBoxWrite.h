#if !defined(AFX_DLGECMAILBOXWRITE_H__3EF88A8F_059F_463D_9F6D_B28316BDC3DA__INCLUDED_)
#define AFX_DLGECMAILBOXWRITE_H__3EF88A8F_059F_463D_9F6D_B28316BDC3DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEcMailBoxWrite.h : header file
//

#include "gridctrl.h"

#define MAXECMAILBOXWRITEROWS  128
#define MAXECMAILBOXWRITECOLS    6
#define EC_MAILBOX_TYPE_WRITE	 2
#define ID_ECMAILBOXWRITETIMER 500

/////////////////////////////////////////////////////////////////////////////
// CDlgEcMailBoxWrite dialog

class CEcMailBoxWriteGrid : public CGridCtrl
{
    public :
        CEcMailBoxWriteGrid ();
        void InitControl (CPtrArray &slaveArray);
        void CloseControl ();
        void SetColumnSize ();
		void InitStrings (CPtrArray &slaveArray);
    private :
        void InitColumnCaptions ();
        void InitGrid ();
        void InitGridLine (int n);
        void CloseStrings ();
        void OnEditCell (int nRow, int nCol, CPoint point, UINT nChar);
    private :
        CImageList m_ImageList;
        CStringArray m_slCanaleCan;
        CStringArray m_slRicevitore;
        CStringArray m_slTipoDato;
};

class CDlgEcMailBoxWrite : public CDialog
{
// Construction
public:
	CDlgEcMailBoxWrite(CWnd* pParent = NULL);   // standard constructor
	void LoadEthercatXMLFile (CPtrArray &slaveArray);
// Dialog Data
	//{{AFX_DATA(CDlgEcMailBoxWrite)
	enum { IDD = IDD_DLG_EC_MAILBOX_WRITE };
	CButton	m_btnEcMailBoxReset;
	CButton	m_btnEcMailBoxRead;
	CButton	m_btnEcMailBoxDelete;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEcMailBoxWrite)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEcMailBoxWrite)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnEcResetWrite();
	afx_msg void OnBtnEcDeleteWrite();
	afx_msg void OnBtnEcReadWrite();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	void InitButtons ();
	int  GetSlaveIndex (CString str);
	bool GetEcMalBoxData (int nRow, int *nSlave, int *nIndex, int *nSubIndex, int *nLen, BYTE *pData);
	int  GetWriteData (int nRow, CString &str, int *l, int *t, BYTE *p);
	CAdamoMachine *GetMachine();
	void ShowResults(LONG lResult);
	void OnOK ();
	void OnCancel ();
private :
	CEcMailBoxWriteGrid m_ctrlEcMailBoxWrite;
	CPtrArray		   *m_slaveArray;
	bool				m_bInitialized;
	bool				m_bEcMonitoring;
	int					m_nMonitoredRow;
	UINT				m_nTimer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGECMAILBOXWRITE_H__3EF88A8F_059F_463D_9F6D_B28316BDC3DA__INCLUDED_)
