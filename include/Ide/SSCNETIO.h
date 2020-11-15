#if !defined(AFX_SSCNETIO_H__B16313FC_7529_402C_AEBA_C7B4EEC032A6__INCLUDED_)
#define AFX_SSCNETIO_H__B16313FC_7529_402C_AEBA_C7B4EEC032A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SSCNETIO.h : header file
//

#include "ConfigDoc.h"
#include "ConfigHWGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CSSCNETIO dialog

class CSSCNETIO : public CDialog
{
// Construction
public:
	CSSCNETIO(CWnd* pParent = NULL);   // standard constructor
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    int  GetKinemaAddress ()                    { return m_nKinemaAddress; }
    void SetSSCNETStation (int nKinemaAddress)  { m_nStation=nKinemaAddress; }
    int  GetSSCNETStation ()                    { return m_nStation; }
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }
    bool Find (const char *szBuffer);
    void RemoveCurrentConnection ();
// Dialog Data
	//{{AFX_DATA(CSSCNETIO)
	enum { IDD = IDD_DLG_SSCNETIO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSCNETIO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private :
    CConfigHWGrid   m_GridInpDig;
    CConfigHWGrid   m_GridOutDig;
    CConfigDoc     *m_pConfigDoc;
	CProject       *m_pPrj;
	CStatic	        m_stInpDigTitle;
	CStatic	        m_stOutDigTitle;
    bool            m_bInitialized;
    int             m_nKinemaAddress;
    int             m_nStation;
private :
    void InitializeTitles ();
    void InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nMode);
    void SetConfigDoc ();
    void InitializeGridData ();
protected:
	// Generated message map functions
	//{{AFX_MSG(CSSCNETIO)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSCNETIO_H__B16313FC_7529_402C_AEBA_C7B4EEC032A6__INCLUDED_)
