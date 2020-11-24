#if !defined(AFX_ADAMOABOUTBOX_H__A6FA24EE_4FCC_49E1_AB53_913927CE3616__INCLUDED_)
#define AFX_ADAMOABOUTBOX_H__A6FA24EE_4FCC_49E1_AB53_913927CE3616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoAboutBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoAboutBox dialog

class CAdamoAboutBox : public CDialog
{
// Construction
public:
	CAdamoAboutBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdamoAboutBox)
	enum { IDD = IDD_DLG_ABOUT };
	CTransparentStatic	m_stProductID;
	CTransparentStatic	m_stProductVersion;
	CTransparentStatic	m_stProductCompany;
	CTransparentStatic	m_stRightsReserved;
	CTransparentStatic	m_stKinemaSuite;
	CTransparentStatic	m_stCommInterface;
	CTransparentStatic	m_stCommModule;
	CTransparentStatic	m_stTranslationModule;
	CTransparentStatic	m_stFirmware;
	CTransparentStatic	m_stCommInterfaceVer;
	CTransparentStatic	m_stCommModuleVer;
	CTransparentStatic	m_stTranslationModuleVer;
	CTransparentStatic	m_stFirmwareVer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoAboutBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoAboutBox)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	void InitVersionLabel();
	void InitCommInterfaceLabel();
	void InitCommModuleLabel();
	void InitTranslationModuleLabel();
	void InitFirmwareLabel();
private :
    CBrush  m_bkg;
    CFont*  m_fnt;
	CFont*  m_fntBig;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOABOUTBOX_H__A6FA24EE_4FCC_49E1_AB53_913927CE3616__INCLUDED_)
