#if !defined(AFX_SSCNETPARAMETERS_H__37D517FE_142F_4431_A060_6B5649A52EAD__INCLUDED_)
#define AFX_SSCNETPARAMETERS_H__37D517FE_142F_4431_A060_6B5649A52EAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sscnetparameters.h : header file
//

#include "ConfigDoc.h"
#include "SSCNETParGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CSSCNETParameters dialog

class CSSCNETParameters : public CDialog
{
// Construction
public:
	CSSCNETParameters(CWnd* pParent = NULL);   // standard constructor
    void SetGridData (int nDataNumber, CString strParNumber, CString strParSymbol, CString strParUnit, CString strParName, bool bIsEditable, bool bIsHex);
    void InitParData (int nPar);
    void SetParValue (int nPar, WORD w);
    void GetParValue (int nPar, WORD *pw);
    void GetSelectedParams (WORD *pW, int *nNumPar);
    void EnableGrid (bool b=true);
	void SetProject (CProject *pPrj)	{ m_pPrj = pPrj; }

// Dialog Data
	//{{AFX_DATA(CSSCNETParameters)
	enum { IDD = IDD_DLG_SSCNETPAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSCNETParameters)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSSCNETParameters)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CSSCNETParGrid  m_grid;
    CConfigDoc     *m_pConfigDoc;
	CProject	   *m_pPrj;
    bool            m_bInitialized;
private :
    void InitializeGrid (int nID);
    void InitializeData ();
    void SetConfigDoc ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSCNETPARAMETERS_H__37D517FE_142F_4431_A060_6B5649A52EAD__INCLUDED_)
