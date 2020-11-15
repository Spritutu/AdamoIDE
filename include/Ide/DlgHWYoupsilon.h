#if !defined(AFX_DLGHWYOUPSILON_H__6FFA527B_2EDE_4A99_9485_B420325EDA9B__INCLUDED_)
#define AFX_DLGHWYOUPSILON_H__6FFA527B_2EDE_4A99_9485_B420325EDA9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHWYoupsilon.h : header file
//

#include "ConfigDoc.h"
#include "ConfigHWGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgHWYoupsilon dialog

class CDlgHWYoupsilon : public CDialog
{
    DECLARE_DYNAMIC (CDlgHWYoupsilon)
// Construction
public:
	CDlgHWYoupsilon(CWnd* pParent = NULL);   // standard constructor
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    int  GetKinemaAddress ()                    { return m_nKinemaAddress; }
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }
	CProject *GetProject ()						{ return m_pPrj; }
    void RemoveCurrentConnection ();
    void Find (const char *szBuffer);
// Dialog Data
	//{{AFX_DATA(CDlgHWYoupsilon)
	enum { IDD = IDD_DLG_HW_YOUPSILON };
	CStatic	m_stOutDigTitle;
	CStatic	m_stInpDigTitle;
	CStatic	m_stOutAnaTitle;
	CStatic	m_stInpAnaTitle;
	CStatic	m_stAxisTitle;
	CStatic	m_bmpYoupsilon;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHWYoupsilon)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHWYoupsilon)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CConfigHWGrid   m_GridAssi;
    CConfigHWGrid   m_GridInpAna;
    CConfigHWGrid   m_GridOutAna;
    CConfigHWGrid   m_GridInpDig;
    CConfigHWGrid   m_GridOutDig;
    bool            m_bInitialized;
    CConfigDoc     *m_pConfigDoc;
    int             m_nKinemaAddress;
	CProject	   *m_pPrj;
private :
    void InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nMode);
    void InitializeTitles ();
    void InitializeGridData ();
    void SetConfigDoc ();
    void OnCancel ();
    void OnOK ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHWYOUPSILON_H__6FFA527B_2EDE_4A99_9485_B420325EDA9B__INCLUDED_)
