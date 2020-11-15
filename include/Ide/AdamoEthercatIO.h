#if !defined(AFX_ADAMOETHERCATIO_H__A608CDFF_1506_495E_AC75_3EC048F04AFD__INCLUDED_)
#define AFX_ADAMOETHERCATIO_H__A608CDFF_1506_495E_AC75_3EC048F04AFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoEthercatIO.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatIO dialog

#include "ConfigHWGrid.h"

class CAdamoEthercatIOGrid : public CConfigHWGrid
{
	public :
		CAdamoEthercatIOGrid ();
	private :
		void FillGridCaption ();
};


class CAdamoEthercatIO : public CDialog
{
	DECLARE_DYNAMIC(CAdamoEthercatIO)
	// Construction
public:
	CAdamoEthercatIO(CWnd* pParent = NULL);   // standard constructor
    void SetConfigDoc (CConfigDoc *pDoc)        { m_pDoc=pDoc; }
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }
	void SetEthercatNode (int nNode, int nTab)	{ m_nNode=nNode; m_nTab=nTab; }
    void Find (const char *szBuffer);
	void RemoveCurrentConnection ();

// Dialog Data
	//{{AFX_DATA(CAdamoEthercatIO)
	enum { IDD = IDD_DLG_ETHERCAT_IO };
	CStatic	m_stOutDig;
	CStatic	m_stInpDig;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoEthercatIO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoEthercatIO)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	int				m_nNode;
	int				m_nTab;
	CConfigDoc      *m_pDoc;
	CProject        *m_pPrj;
	stAdamoEthSlave *m_pSlave;
    CAdamoEthercatIOGrid   m_GridInpDig;
    CAdamoEthercatIOGrid   m_GridOutDig;
    bool            m_bInitialized;
private :
	void InitializeGridData ();
	void InitializeTitles ();
	void InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode);
	void LoadSlaveInfo ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOETHERCATIO_H__A608CDFF_1506_495E_AC75_3EC048F04AFD__INCLUDED_)
