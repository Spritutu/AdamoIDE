#if !defined(AFX_ADAMOETHERCATIOANALOGICO_H__98B31E6B_28F2_4986_8D7E_0EB7DC535E79__INCLUDED_)
#define AFX_ADAMOETHERCATIOANALOGICO_H__98B31E6B_28F2_4986_8D7E_0EB7DC535E79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoEthercatIOAnalogico.h : header file
//

/* include files */
#include "ConfigHWGrid.h"
#include "ConfigDoc.h"

/* forward reference */
class CAdamoEthercatIOAnalogico;

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatIOAnalogico dialog

class CAdamoEthercatIOAnalogicoGrid : public CConfigHWGrid
{
	public :
		CAdamoEthercatIOAnalogicoGrid ();
		void SetEthIOAnalogico (CAdamoEthercatIOAnalogico *pEth)   { m_pEth=pEth; }
	private :
		void FillGridCaption ();
		int  OnConnected (int nRow, int nCol, const char *szHWAddress);
		int  OnRemoveConnection (int nRow, int nCol, const char *szHWAddress);
		void OnEditCell (int nRow, int nCol, CPoint point, UINT nChar);
		void OnEndEditCell (int nRow, int nCol, CString str);
	private :
		CAdamoEthercatIOAnalogico *m_pEth;
};

class CAdamoEthercatIOAnalogico : public CDialog
{
	DECLARE_DYNAMIC(CAdamoEthercatIOAnalogico)
	friend CAdamoEthercatIOAnalogicoGrid;
// Construction
public:
	CAdamoEthercatIOAnalogico(CWnd* pParent = NULL);   // standard constructor
    void SetConfigDoc (CConfigDoc *pDoc)        { m_pDoc=pDoc; }
	void SetEthercatNode (int nNode, int nTab)	{ m_nNode=nNode; m_nTab=nTab; }
    void Find (const char *szBuffer);
	void RemoveCurrentConnection ();
	void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }
// Dialog Data
	//{{AFX_DATA(CAdamoEthercatIOAnalogico)
	enum { IDD = IDD_DLG_ETHERCAT_IO_ANALOGICO };
	CStatic	m_stOutAnag;
	CStatic	m_stInpAnag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoEthercatIOAnalogico)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoEthercatIOAnalogico)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoEthercatIOAnalogicoGrid m_GridInpAna;
    CAdamoEthercatIOAnalogicoGrid m_GridOutAna;
	CConfigDoc      *m_pDoc;
	CProject		*m_pPrj;
	stAdamoEthSlave *m_pSlave;
	int				m_nNode;
	int				m_nTab;
	bool			m_bInitialized;
    ePswLevel		m_ePswLevel;
private :
	void InitializeTitles ();
	void InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode);
	void InitializeGridData ();
	void LoadSlaveInfo ();
	void InitPsw ();
	int  GetInpAnalogType (int nHWAddress);
	int  GetOutAnalogType (int nHWAddress);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOETHERCATIOANALOGICO_H__98B31E6B_28F2_4986_8D7E_0EB7DC535E79__INCLUDED_)
