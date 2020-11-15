#if !defined(AFX_DLGECMAILBOXREAD_H__F19B79BE_486E_406B_A45B_3CB327FDC69E__INCLUDED_)
#define AFX_DLGECMAILBOXREAD_H__F19B79BE_486E_406B_A45B_3CB327FDC69E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEcMailBoxRead.h : header file
//

#include "GridCtrl.h"

#define MAXECMAILBOXREADROWS  128
#define MAXECMAILBOXREADCOLS    7
#define EC_MAILBOX_TYPE_READ	1
#define ID_ECMAILBOXREADTIMER 400

/////////////////////////////////////////////////////////////////////////////
// CDlgEcMailBoxRead dialog

class CEcMailBoxReadGrid : public CGridCtrl
{
    public :
        CEcMailBoxReadGrid ();
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
        CStringArray m_slRicevitore;
        CStringArray m_slTipoDato;
        CStringArray m_slTipoDiVisualizzazione;
};

class CDlgEcMailBoxRead : public CDialog
{
    enum eTVSL   {
        evtHex,
        evtDec,
        evtOct
    };
// Construction
public:
	CDlgEcMailBoxRead(CWnd* pParent = NULL);   // standard constructor
	void LoadEthercatXMLFile (CPtrArray &slaveArray);
// Dialog Data
	//{{AFX_DATA(CDlgEcMailBoxRead)
	enum { IDD = IDD_DLG_EC_MAILBOX_READ };
	CButton	m_btnEcMailBoxReset;
	CButton	m_btnEcMailBoxRead;
	CButton	m_btnEcMailBoxDelete;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEcMailBoxRead)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgEcMailBoxRead)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBtnEcDeleteRead();
	afx_msg void OnBtnEcResetRead();
	afx_msg void OnBtnEcReadRead();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	void InitButtons ();
	CAdamoMachine *GetMachine();
	bool GetEcMalBoxData (int nRow, int *nSlave, int *nIndex, int *nSubIndex, int *nLen);
	int  GetReadData (int nRow, CString &str, int *l, int *t);
	int  GetSlaveIndex (CString str);
	void ShowResults(LONG lResult, int nLen, BYTE *pData);
	eTVSL GetTipoDiVisualizzazione (int nRow);
	void OnOK ();
	void OnCancel ();
private :
    CEcMailBoxReadGrid m_ctrlEcMailBoxRead;
	CPtrArray	   *m_slaveArray;
	bool			m_bInitialized;
	bool			m_bEcMonitoring;
	int				m_nMonitoredRow;
	UINT			m_nTimer;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGECMAILBOXREAD_H__F19B79BE_486E_406B_A45B_3CB327FDC69E__INCLUDED_)
