#if !defined(AFX_ADAMOCONFIGHWVIEW_H__F75BA34A_0DFE_47C4_AF57_C97D38DAD490__INCLUDED_)
#define AFX_ADAMOCONFIGHWVIEW_H__F75BA34A_0DFE_47C4_AF57_C97D38DAD490__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// adamoconfighwview.h : header file
//

/* constants definitions */
#define PCI_YOUPSILON 0
#define PCI_OMIKRON   1
#define PCI_ETHERCAT  2
#define NET_EPSILON   3
#define NETWORK_TYPE_SSCNETIII 0
#define NETWORK_TYPE_CANOPEN   1
#define NETWORK_TYPE_ETHERCAT  2

#define ID_TREECTRL_CONFHW	1100

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfigHWView view

class CAdamoXTPTreeCtrl : public CXTPTreeCtrl
{
	public :
		CAdamoXTPTreeCtrl ()   {
		}
	protected :
		afx_msg void OnLButtonDblClk(UINT u, CPoint pt);
		afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
		DECLARE_MESSAGE_MAP()
};

class CAdamoConfigHWView : public CPaneView
{
protected:
	CAdamoConfigHWView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoConfigHWView)

// Attributes
public:

// Operations
public:
	void SaveHWConfiguration (CProject *pPrj);
	void LoadEthercatXMLFile (CProject* pPrj, CPtrArray &pSlaves);
	void UpdateTree (CString strCurrentAdapter);
	void OnAdamoTimer ();
	void OnLButtonDblClk (UINT u, CPoint pt);
	void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoConfigHWView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoConfigHWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected :
	void RefreshMetrics(AppTheme nTheme);
	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoConfigHWView)
	afx_msg void OnAdapters ();
	afx_msg void OnScanNetwork ();
	afx_msg void OnAddXMLFile();
	afx_msg void OnRemoveXMLFile();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CXTPImageList m_il;
    bool       m_bInitialized;
	bool	   m_bScanNetwork;	
	HTREEITEM  m_htItemEthercat;
	CAdamoXTPTreeCtrl m_treeCtrl;
private :
    void SetTreeStyle () ;
    void FillTree ();
    DWORD MakeConfigHWItemData (BYTE bCardType, BYTE bPciSlot, BYTE bNetworkType, BYTE bNode);
    void InsertYoupsilonTree (HTREEITEM htItemYoupsilon, int nCardType, int nNode);
    void InsertOmikronTree (HTREEITEM htItemOmikron, int nCardType, int nNode);
    void InsertCanOpenTree (HTREEITEM htItem, int nCardType, int nNode);
	void InsertModuleTree (HTREEITEM htItem, int nCardType, int nNode);
    void OpenHWDialog (int nCardType, int nCard, int i, int n);
    void InitView ();
	CString CreateEthercatDir ();
	void DeleteEthercatNodes ();
	void FillEthercatNodes (CPtrArray &pSlaves);
	void CleanInfo (CProject *pPrj);
	void DestroyAnyDlg ();
	CAdamoMachine *GetMachine ();
	CAdamoHWConfigFile *GetHWConfigFile ();
	void SetHWConfigModified ();
	void LoadEcData (int nSlave);
	void SetObjectsData (CAdamoMachine* pMachine, int nSlave);
	CProject *GetEthercatPrj (int nCardType, int nCard);
	CAdamoXTPTreeCtrl& GetTreeCtrl ()	{ return m_treeCtrl; }
	int CreateTree ();
	void InsertEthercatTree (HTREEITEM htItemRemoteModule	, int nCardType, int nNode);
	void FindEthercatNode (CProject* pPrj);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOCONFIGHWVIEW_H__F75BA34A_0DFE_47C4_AF57_C97D38DAD490__INCLUDED_)
