#if !defined(AFX_ADAMOETHERCATNODE_H__AA29DF2D_AB90_4D5C_9C98_D6AB5A165FB4__INCLUDED_)
#define AFX_ADAMOETHERCATNODE_H__AA29DF2D_AB90_4D5C_9C98_D6AB5A165FB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoEthercatNode.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatNode dialog

#include "AdamoIdeEthercat.h"

class CAdamoEthercatNode : public CDialog
{
    DECLARE_DYNAMIC (CAdamoEthercatNode)
// Construction
public:
	CAdamoEthercatNode(CWnd* pParent = NULL);   // standard constructor
	void SetEthercatNode (int nNode)	{ m_nNode=nNode; }
	int  GetEthercatNode ()				{ return m_nNode; }
    void Find (const char *szBuffer);
    void RemoveCurrentConnection ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void SetProject (CProject *pPrj)	{ m_pPrj = pPrj; }		

// Dialog Data
	//{{AFX_DATA(CAdamoEthercatNode)
	enum { IDD = IDD_DLG_ETHERCAT_NODE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoEthercatNode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoEthercatNode)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CXTFlatTabCtrl *m_pFlatTabCtrl;
	int m_nNode;
	bool m_bInitialized;
	CConfigDoc *m_pConfigDoc;
	CProject	*m_pPrj;
private :
	void CreateTab ();
	void SetConfigDoc ();
	eEthercatDeviceType GetDeviceType (int nIndex);
	void OnOK() {
	}
	void OnCancel() {
	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOETHERCATNODE_H__AA29DF2D_AB90_4D5C_9C98_D6AB5A165FB4__INCLUDED_)
