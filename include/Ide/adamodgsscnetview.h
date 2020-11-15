#pragma once

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGSSCNETView view

class CAdamoDGSSCNETView : public CView
{
protected:
	CAdamoDGSSCNETView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoDGSSCNETView)

// Attributes
public:
    void OnAdamoTimer ();                       
    int  GetCurTabSel ()        { return m_nCurSel; }
    void InizializzaInformazioni ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGHardware)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoDGSSCNETView ();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoDGHardware)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnSSCNETReadyOn();
	afx_msg void OnBtnSSCNETReadyOff();
	afx_msg void OnBtnSSCNETServoOn();
	afx_msg void OnBtnSSCNETServoOff();
    afx_msg void OnUpdateBtnSSCNETReadyOn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnSSCNETReadyOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnSSCNETServoOff(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnSSCNETServoOn(CCmdUI* pCmdUI);
	afx_msg void OnBtnResetAlarm();
	afx_msg void OnUpdateBtnSetpointAbs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnInitSSCNET(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnDownloadAbsEncoder(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnUploadAbsEncoder(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBtnResetAlarm(CCmdUI* pCmdUI);
	afx_msg void OnBtnSetpointAbs();
	afx_msg void OnBtnInitSSCNET();
	afx_msg void OnBtnDownloadAbsEncoder();
	afx_msg void OnBtnUploadAbsEncoder();
    afx_msg void OnSetAllSSCNETStations(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateSetAllSSCNETStations(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CXTPTabControl*			 m_pFlatTabCtrl;
    CAdamoDGSSCNET*          m_pDGSSCNET[NMAX_SCHEDE];
    CAdamoMachine           *m_pMachine;
    ePswLevel		         m_ePswLevel;
    int                      m_nNumCards;
    int                      m_nCurSel;
private :
    void CreateViews ();
    void SetMachine ();
};

/////////////////////////////////////////////////////////////////////////////

