#if !defined(AFX_ADAMOCONFHWVIEW_H__D9AB8576_9B86_4E00_AD5F_133499A8EEC7__INCLUDED_)
#define AFX_ADAMOCONFHWVIEW_H__D9AB8576_9B86_4E00_AD5F_133499A8EEC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// adamoconfhwview.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfHWView view

class CAdamoConfHWView : public CView
{
public:
	CAdamoConfHWView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoConfHWView)

// Attributes
public:

// Operations
public:
    void CreateDlgHW (int nCardType, int nCard, int i, int n);
    void DestroyDlgHW ();
    void FlushData ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoConfHWView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdamoConfHWView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoConfHWView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditClear();
	afx_msg void OnUpdateEditClear(CCmdUI* pCmdUI);
	afx_msg void OnEditFind();
	afx_msg void OnUpdateEditFind(CCmdUI* pCmdUI);
	afx_msg void OnDestroy();
	afx_msg void OnDownloadSSCNETParams();
	afx_msg void OnUpdateDownloadSSCNETParams(CCmdUI* pCmdUI);
	afx_msg void OnUploadSSCNETParams();
	afx_msg void OnUpdateUploadSSCNETParams(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CDialog   *m_pHWDlg;
    ePswLevel  m_ePswLevel;
private :
    void FindInDialog (CProject *pPrj, char *szBuffer);
	void FindModule (CProject *pPrj);
    bool CloseCurrentDialog (CProject *pPrj, char *szBuffer);
    int  GetStationFromHWAddress (const char *szBuffer);
    void GestionePsw (CXTPPropertyGridItem* pItem, ePswLevel e);
	CProject *GetAvailablePrj (int nCardType, int nCard);
	eEthercatDeviceType IsSlaveConfigured (CProject *pPrj, int nNode);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOCONFHWVIEW_H__D9AB8576_9B86_4E00_AD5F_133499A8EEC7__INCLUDED_)
