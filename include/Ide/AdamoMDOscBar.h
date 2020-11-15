#if !defined(AFX_ADAMOMDOSCBAR_H__220BD7DF_424E_4B02_8C9C_28939ECC6B72__INCLUDED_)
#define AFX_ADAMOMDOSCBAR_H__220BD7DF_424E_4B02_8C9C_28939ECC6B72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoMDOscBar.h : header file
//

/* constants definitions */
#define NMAXOSCBARBUTTONS 9

/* forward reference */
class CDlgOscilloscope;

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDOscBar dialog

class CAdamoMDOscBar : public CDialog
{
// Construction
public:
	CAdamoMDOscBar(CWnd* pParent = NULL);   // standard constructor
    void OnOscStarted ();
    void OnOscStopped ();
    void OnOscStatus  (const char* s);
// Dialog Data
	//{{AFX_DATA(CAdamoMDOscBar)
	enum { IDD = IDD_DLGOSCBAR };
	CStatic	m_ctrlOscStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDOscBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoMDOscBar)
	virtual BOOL OnInitDialog();
	afx_msg void OnOscStart();
	afx_msg void OnDestroy();
	afx_msg void OnOscStartPretrigger();
	afx_msg void OnOscStop();
	afx_msg void OnOscClear();
	afx_msg void OnOscOptions();
	afx_msg void OnOscCursor();
	afx_msg void OnOscLoad();
	afx_msg void OnOscSave();
	afx_msg void OnOscPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CDlgOscilloscope* m_pOsc;
	CBitmapButton	  m_ctrlOscPrint;
	CBitmapButton	  m_ctrlOscSave;
	CBitmapButton	  m_ctrlOscLoad;
	CBitmapButton	  m_ctrlOscOptions;
	CBitmapButton	  m_ctrlOscCursor;
	CBitmapButton	  m_ctrlOscClear;
	CBitmapButton	  m_ctrlOscStop;
	CBitmapButton	  m_ctrlOscStartPrTrigger;
	CBitmapButton	  m_ctrlOscStart;
    CBitmapButton*    m_pOscButtons[NMAXOSCBARBUTTONS];
private :
    void EnableButtons (DWORD dwEnable);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDOSCBAR_H__220BD7DF_424E_4B02_8C9C_28939ECC6B72__INCLUDED_)
