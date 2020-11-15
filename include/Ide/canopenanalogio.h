#if !defined(AFX_CANOPENANALOGIO_H__2E93DC39_4364_449C_BE6B_BBE3EFC02045__INCLUDED_)
#define AFX_CANOPENANALOGIO_H__2E93DC39_4364_449C_BE6B_BBE3EFC02045__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// canopenanalogio.h : header file
//

#include "ConfigDoc.h"
#include "ConfigHWGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCanOpenAnalogIO dialog

class CCanOpenAnalogIO : public CDialog
{
// Construction
public:
	CCanOpenAnalogIO(CWnd* pParent = NULL);   // standard constructor
    void SetConfigDoc (CConfigDoc *pDoc)        { m_pConfigDoc=pDoc; }
    void SetProject (CProject *pPrj)			{ m_pPrj=pPrj; }
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    void SetCanOpenNode (int nNode)             { m_nNode=nNode; }
    void RemoveCurrentConnection ();
    void Find (const char *szBuffer);
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);

    // Dialog Data
	//{{AFX_DATA(CCanOpenAnalogIO)
	enum { IDD = IDD_DLG_HW_CANOPEN_ANALOGIO };
	CStatic	m_stOutAnag;
	CStatic	m_stInpAnag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanOpenAnalogIO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCanOpenAnalogIO)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CConfigHWGrid   m_GridInpAna;
    CConfigHWGrid   m_GridOutAna;
    CConfigDoc     *m_pConfigDoc;
	CProject	   *m_pPrj;
    bool            m_bInitialized;
    int             m_nKinemaAddress;
    int             m_nNode;
    ePswLevel       m_ePswLevel;
private :
    void InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode);
    void InitializeGridData ();
    void InitializeTitles ();
    void InitPsw ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANOPENANALOGIO_H__2E93DC39_4364_449C_BE6B_BBE3EFC02045__INCLUDED_)
