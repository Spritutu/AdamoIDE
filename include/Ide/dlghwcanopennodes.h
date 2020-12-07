#if !defined(AFX_DLGHWCANOPENNODES_H__6E1A3294_70CE_4A2B_9D78_9D614C89E12A__INCLUDED_)
#define AFX_DLGHWCANOPENNODES_H__6E1A3294_70CE_4A2B_9D78_9D614C89E12A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlghwcanopennodes.h : header file
//

#include "ConfigDoc.h"
#include "ConfigHWGrid.h"
#include "CanOpenAnalogIO.h"
#include "CanOpenDigitalIO.h"
#include "CanOpenData.h"
#include "CanOpenAxes.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgHWCanOpenNodes dialog

class CDlgHWCanOpenNodes : public CDialog
{
    DECLARE_DYNAMIC (CDlgHWCanOpenNodes)
// Construction
public:
	CDlgHWCanOpenNodes(CWnd* pParent = NULL);   // standard constructor
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    int  GetKinemaAddress ()                    { return m_nKinemaAddress; }
    void SetModuleNumber (int nModule)          { m_nModule=nModule; }
    int  GetModuleNumber ()                     { return m_nModule; }
    void SetCanOpenNode (int nNode)             { m_nNode=nNode; }
    int  GetCanOpenNode ()                      { return m_nNode; }
    void Find (const char *szBuffer);
    void Save ();
    void RemoveCurrentConnection ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }
	CProject *GetProject ()						{ return m_pPrj; }

// Dialog Data
	//{{AFX_DATA(CDlgHWCanOpenNodes)
	enum { IDD = IDD_DLG_HW_CANOPENNODES };
	CStatic	m_stCanTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHWCanOpenNodes)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHWCanOpenNodes)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void InitializeTitles ();
    void SetConfigDoc ();
    void CreateTab ();
    void CreateFont ();
	void OnOK() {
	}
	void OnCancel() {
	}
private :
    CConfigDoc     *m_pConfigDoc;
    int             m_nKinemaAddress;
    int             m_nNode;
	int				m_nModule;
    bool            m_bInitialized;
    CXTFlatTabCtrl *m_pFlatTabCtrl;
    CCanOpenDigitalIO   *m_pDigitalIO;
    CCanOpenAnalogIO    *m_pAnalogIO;
    CCanOpenData        *m_pCanData;
	CCanOpenAxes		*m_pCanAxes;
    CFont           *m_pFont;
	CProject	    *m_pPrj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHWCANOPENNODES_H__6E1A3294_70CE_4A2B_9D78_9D614C89E12A__INCLUDED_)
