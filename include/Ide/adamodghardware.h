#if !defined(AFX_ADAMODGHARDWARE_H__0B424381_6D82_4142_9926_694D9A3BE9E8__INCLUDED_)
#define AFX_ADAMODGHARDWARE_H__0B424381_6D82_4142_9926_694D9A3BE9E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoDGHardware.h : header file
//

/* forward reference */
class CAdamoDGCards;
class CAdamoDGDevice;
class CAdamoDGSoftware;

/////////////////////////////////////////////////////////////////////////////
// CAdamoDGHardware view

class CAdamoDGHardware : public CView
{
protected:
	CAdamoDGHardware();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoDGHardware)

// Attributes
public:
    void OnAdamoTimer ();                       
    int  GetCurTabSel ()        { return m_nCurSel; }
    void SetDGMode (int nMode)  { m_nMode=nMode; }
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
	virtual ~CAdamoDGHardware();
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
	afx_msg void OnDgRefresh();
	afx_msg void OnDgSetOperational();
	afx_msg void OnDgSetPreoperational();
	afx_msg void OnUpdateDgRefresh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDgSetOperational(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDgSetPreoperational(CCmdUI* pCmdUI);
	afx_msg void OnDgSetInitialize();
	afx_msg void OnUpdateDgSetInitialize(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDgSetStart(CCmdUI* pCmdUI);
	afx_msg void OnDgSetStart();
	afx_msg void OnDgSetStop();
	afx_msg void OnUpdateDgSetStop(CCmdUI* pCmdUI);
	afx_msg void OnBtnSetpointAbs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CXTPTabControl*			 m_pFlatTabCtrl;
    CAdamoDGCards*           m_pDGCards;
    CAdamoDGDevice*          m_pDGDevice;
    CAdamoDGSoftware*        m_pDGSoftware;
    CAdamoMachine*           m_pMachine;
    int                      m_nCurSel;
    int                      m_nMode;
    int                      m_nNumCards;
    ePswLevel				 m_ePswLevel;
private :
    void CreateViews ();
    void SetMachine ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMODGHARDWARE_H__0B424381_6D82_4142_9926_694D9A3BE9E8__INCLUDED_)
