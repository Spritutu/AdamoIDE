#if !defined(AFX_DLGHWSSCNETSTATION_H__1C142D91_D085_495A_B1E1_8BBD7A74D797__INCLUDED_)
#define AFX_DLGHWSSCNETSTATION_H__1C142D91_D085_495A_B1E1_8BBD7A74D797__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlghwsscnetstation.h : header file
//

#include "ConfigDoc.h"
#include "ConfigHWGrid.h"
#include "SSCNETParameters.h"
#include "SSCNETIO.h"

#define SSCNET_BASEPAR          0
#define SSCNET_GAINFILTERPAR    1
#define SSCNET_EXTENSIONPAR     2
#define SSCNET_IOPAR            3
#define SSCNET_IOEXT2           4
#define SSCNET_IOEXT3           5
#define SSCNET_IOEXTLINEARMOTOR 6

/////////////////////////////////////////////////////////////////////////////
// CDlgHWSSCNETStation dialog

class CDlgHWSSCNETStation : public CDialog
{
    DECLARE_DYNAMIC (CDlgHWSSCNETStation)
// Construction
public:
	CDlgHWSSCNETStation(CWnd* pParent = NULL);   // standard constructor
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    int  GetKinemaAddress ()                    { return m_nKinemaAddress; }
    void SetModuleNumber (int nModule)          { m_nModule=nModule; }
    int  GetModuleNumber ()                     { return m_nModule; }
    void SetSSCNETStation (int nKinemaAddress)  { m_nStation=nKinemaAddress; }
    int  GetSSCNETStation ()                    { return m_nStation; }
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }		
	CProject *GetProject ()						{ return m_pPrj; }		
    void RemoveCurrentConnection ();
    void Find (const char *szBuffer);
    void Save ();
    void UploadSSCNETParams ();
    void DownloadSSCNETParams();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);

// Dialog Data
	//{{AFX_DATA(CDlgHWSSCNETStation)
	enum { IDD = IDD_DLG_HW_SSCNETSTATIONS };
	CStatic	m_stEncTitle;
	CButton	m_EnableAbsEncoder;
	CStatic	m_stStationTitle;
	CStatic	m_stAxisTitle;
	CStatic	m_stServoTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHWSSCNETStation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHWSSCNETStation)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnCheckEnableAbsEncoder();
	afx_msg void OnChangeEditHomePosition();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    void SetConfigDoc ();
    void OnOK ();
    void OnCancel ();
    void InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nMode);
    void InitializeGridData ();
    void InitializeTitles ();
    void CreateTab ();
    void CreateFont ();
    void InitBaseParData ();
    void InitGainFilterParData ();
    void InitExtensionParData ();
    void InitIOParData ();
    void InitIOData ();
    void InitExtension2ParData ();
    void InitExtension3ParData ();
    void InitLinearMotorParData ();
    void LoadParValues ();
    void SaveParValues ();
    void InitializeMachine ();
    void DefineType (int nType, WORD *pW, int nNumParams);
    void InitData ();
    void SaveData ();
    void InitPsw ();
private :
    CConfigHWGrid   m_GridSSCNETAxis;
    CConfigDoc     *m_pConfigDoc;
	CProject       *m_pPrj;
    int             m_nKinemaAddress;
	int				m_nModule;
    int             m_nStation;
    bool            m_bInitialized;
    CXTFlatTabCtrl *m_pFlatTabCtrl;
    CFont          *m_pFontSSCNET;
    CSSCNETParameters *m_pBasePar;
    CSSCNETParameters *m_pGainFilterPar;
    CSSCNETParameters *m_pExtensionPar;
    CSSCNETParameters *m_pIOPar;
    CSSCNETParameters *m_pIOExt2;
    CSSCNETParameters *m_pIOExt3;
    CSSCNETParameters *m_LinearMotorPar;
    CSSCNETIO      *m_pIO;
    CAdamoMachine  *m_pMachine;
    CParsedEdit	    m_EditHomePosition;
    ePswLevel       m_ePswLevel;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHWSSCNETSTATION_H__1C142D91_D085_495A_B1E1_8BBD7A74D797__INCLUDED_)
