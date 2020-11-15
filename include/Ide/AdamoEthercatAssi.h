#if !defined(AFX_ADAMOETHERCATASSI_H__C645387B_1EBF_4204_8D35_9744B3C6AA4A__INCLUDED_)
#define AFX_ADAMOETHERCATASSI_H__C645387B_1EBF_4204_8D35_9744B3C6AA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoEthercatAssi.h : header file
//

#include "ConfigHWGrid.h"
#include "ConfigDoc.h"

#define AX_PER_SLAVE 8

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatAssi dialog

class CAdamoEthercatAssi;

class CAdamoEthercatAssiGrid : public CConfigHWGrid
{
	public :
		CAdamoEthercatAssiGrid ();
		void SetEthAssi (CAdamoEthercatAssi *pEth)   { m_pEth=pEth; }
	private :
		void FillGridCaption ();
		int  OnConnected (int nRow, int nCol, const char *szHWAddress);
		int  OnRemoveConnection (int nRow, int nCol, const char *szHWAddress);
	private :
		CAdamoEthercatAssi *m_pEth;
		bool m_bOutput;
};

class CAdamoEthercatVariabiliAssiGrid : public CConfigHWGrid
{
	public :
		CAdamoEthercatVariabiliAssiGrid ();
		void SetOutputGrid ()						 { m_bOutput=true; }
		void SetEthAssi (CAdamoEthercatAssi *pEth)   { m_pEth=pEth; }
		int  OnConnected (int nRow, int nCol, const char *szHWAddress);
		void UpdateData ();
	private :
		void FillGridCaption ();
		void OnEditCell (int nRow, int nCol, CPoint point, UINT nChar);
		void OnEndEditCell (int nRow, int nCol, CString str);
		void StoreEthercatInfo (stAdamoEthSlaveVariable *pVar, int nBase, DWORD dw);
	private :
		CAdamoEthercatAssi *m_pEth;
		bool m_bOutput;
};

class CAdamoEthercatAssi : public CDialog
{
	DECLARE_DYNAMIC(CAdamoEthercatAssi)
// Construction
public:
	CAdamoEthercatAssi(CWnd* pParent = NULL);   // standard constructor
	void SetConfigDoc (CConfigDoc *pDoc)	{ m_pDoc=pDoc; }
	void SetProject (CProject *pPrj)		{ m_pPrj = pPrj; }		
	void SetEthercatNode (int nNode, int nTab)	{ m_nNode=nNode; m_nTab=nTab; }
	void Find (const char *szBuffer);
	void RemoveCurrentConnection ();
	CStringArray &GetSLVarInput ()			{ return m_slVarInput; }
	CStringArray &GetSLVarOutput ()			{ return m_slVarOutput; }
	void ConnectAx (int nAx, bool b)		{ bAxConnected[nAx]=b; }
	bool IsAxConnected (int nAx)			{ return bAxConnected[nAx]; }
	stAdamoEthSlave *GetSlave ()			{ return m_pSlave; }
	CAdamoEthercatAssiGrid *GetGridAssi ()	{ return &m_GridAssi; }
	int  OnConnected (int nRow, int nCol, const char *szHWAddress);
	int  OnRemoveConnection (int nRow, int nID);
	void StoreEthercatInfo (stAdamoEthSlaveVariable *pVar, int nBase, DWORD dw);
	CAdamoHWConfigFile *GetHWConfigFile()	{ return m_pPrj->GetHWConfigFile (); }
	CConfigDoc *GetDocument ()				{ return m_pDoc; }
	CProject   *GetProject ()				{ return m_pPrj; }
	void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Dialog Data
	//{{AFX_DATA(CAdamoEthercatAssi)
	enum { IDD = IDD_DLG_ETHERCAT_ASSI };
	CStatic	m_stAssi;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoEthercatAssi)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoEthercatAssi)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CAdamoEthercatAssiGrid m_GridAssi;
	CAdamoEthercatVariabiliAssiGrid m_GridVarInput;
	CAdamoEthercatVariabiliAssiGrid m_GridVarOutput;
	stAdamoEthSlave *m_pSlave;
	bool		m_bInitialized;
	int		    m_nNode;
	int         m_nTab;
	CConfigDoc *m_pDoc;
	CProject   *m_pPrj;
	CStringArray m_slVarInput;
	CStringArray m_slVarOutput;
	bool bAxConnected[AX_PER_SLAVE];
    ePswLevel	m_ePswLevel;
private :
	void InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode);
	void InitializeGridVariabili (CConfigHWGrid& m_Grid, int nID, int nRow, int nCol, int nMode);
	void InitializeGridVariabiliData (CConfigHWGrid& m_Grid);
	void InitializeTitles ();
	void InitializeGridData ();
	void InitializeStrings ();
	void LoadSlaveInfo ();
	void InitPsw ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOETHERCATASSI_H__C645387B_1EBF_4204_8D35_9744B3C6AA4A__INCLUDED_)
