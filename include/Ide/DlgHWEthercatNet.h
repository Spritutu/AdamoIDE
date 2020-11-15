#if !defined(AFX_DLGHWETHERCATNET_H__44EAA025_FE44_4463_93D5_6528A4821B6B__INCLUDED_)
#define AFX_DLGHWETHERCATNET_H__44EAA025_FE44_4463_93D5_6528A4821B6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgHWEthercatNet.h : header file
//

/* include files */
#include "gridcelltogglebutton.h"

/* constants definitions */
#define ID_DEVICE_GROUP_ID 100

/////////////////////////////////////////////////////////////////////////////
// CDlgHWEthercatNet dialog

class CDlgHWEthercatNet;

class CHWEthercatNetGrid : public CGridCtrl  {
	public :
		CHWEthercatNetGrid ()   { }
		void OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)   {
			if (nCol==2)
				CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
		}
};

class CDlgHWEthercatNet : public CDialog
{
// Construction
	DECLARE_DYNAMIC(CDlgHWEthercatNet)
public:
	CDlgHWEthercatNet(CWnd* pParent = NULL);   // standard constructor
	void Save ();
	void SaveData ();
	void SetHWModified ();
	void RemoveCurrentConnection ();
	void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }		

// Dialog Data
	//{{AFX_DATA(CDlgHWEthercatNet)
	enum { IDD = IDD_DLG_ETHERCAT_NETWORK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgHWEthercatNet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgHWEthercatNet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
    afx_msg void OnGridButtonPressed(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
	afx_msg void OnEcInit();
	afx_msg void OnEcPreoperational();
	afx_msg void OnEcSafeoperational();
	afx_msg void OnEcOperational();
	afx_msg void OnChangeDCStart();
	afx_msg void OnChangeFrequency();
	afx_msg void OnChangeDCOffset();

	afx_msg void OnCbnSelchangeComboDriverHpe();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CHWEthercatNetGrid m_grid;
	bool m_bInitialized;
	CAdamoConfigHWView *m_pHWView;
	CImageList m_ImageList;
	int m_nCurRow;
    ePswLevel m_ePswLevel;
	CConfigDoc     *m_pConfigDoc;
	CParsedEdit	m_ctrlEditDCStartTime;
	CParsedEdit	m_ctrlEditDCOffsetTime;
	CParsedEdit	m_ctrlEditFrequency;
	CProject       *m_pPrj;
private :
	void InitializeGrid ();
	void InitializeCaption ();
	void InitializeTitles ();
	void InitializeControls ();
	void LoadDevices ();
	void ClearSlaveInfo (int nIndex);
	void InitPsw ();
	void SetConfigDoc ();
public:
	afx_msg void OnBnClickedCheckContinousMeasuring();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGHWETHERCATNET_H__44EAA025_FE44_4463_93D5_6528A4821B6B__INCLUDED_)
