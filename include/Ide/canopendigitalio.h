#if !defined(AFX_CANOPENDIGITALIO_H__9796E483_0E7C_4943_B8C7_CBA33F0DBE59__INCLUDED_)
#define AFX_CANOPENDIGITALIO_H__9796E483_0E7C_4943_B8C7_CBA33F0DBE59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// canopendigitalio.h : header file
//

#include "ConfigDoc.h"
#include "ConfigHWGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCanOpenDigitalIO dialog

class CCanOpenDigitalIO : public CDialog
{
// Construction
public:
	CCanOpenDigitalIO(CWnd* pParent = NULL);   // standard constructor
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    void SetCanOpenNode (int nNode)             { m_nNode=nNode; }
    void SetConfigDoc (CConfigDoc *pDoc)        { m_pConfigDoc=pDoc; }
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }
    void RemoveCurrentConnection ();
    void Find (const char *szBuffer);
// Dialog Data
	//{{AFX_DATA(CCanOpenDigitalIO)
	enum { IDD = IDD_DLG_HW_CANOPEN_DIGITALIO };
	CStatic	m_stOutDig;
	CStatic	m_stInpDig;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanOpenDigitalIO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCanOpenDigitalIO)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CConfigDoc     *m_pConfigDoc;
	CProject	   *m_pPrj;
    CConfigHWGrid   m_GridInpDig;
    CConfigHWGrid   m_GridOutDig;
    bool            m_bInitialized;
    int             m_nKinemaAddress;
    int             m_nNode;
private :
    void InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode);
    void InitializeGridData ();
    void InitializeTitles ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANOPENDIGITALIO_H__9796E483_0E7C_4943_B8C7_CBA33F0DBE59__INCLUDED_)
