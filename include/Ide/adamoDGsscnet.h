#if !defined(AFX_ADAMOMDSSCNET_H__DFA6AB4B_9D9B_4DD9_B2FE_7E5E87C62324__INCLUDED_)
#define AFX_ADAMOMDSSCNET_H__DFA6AB4B_9D9B_4DD9_B2FE_7E5E87C62324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// adamomdsscnet.h : header file
//

#include "AdamoSSCNETGrid.h"

class CAdamoDGSSCNET : public CView
{
public:
	CAdamoDGSSCNET();           // protected constructor used by dynamic creation
	~CAdamoDGSSCNET();

// Attributes
public:

// Operations
public:
    void OnAdamoTimer ();
    void InizializzaInformazioni ();
    void SetCard (int nCard)        { m_nCard = nCard; }
	void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoDGSSCNET)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoDGSSCNET)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public :
	void OnBtnSSCNETReadyOn();
	void OnBtnSSCNETReadyOff();
	void OnBtnSSCNETServoOn();
	void OnBtnSSCNETServoOff();
    void OnUpdateBtnSSCNETReadyOn(CCmdUI* pCmdUI);
	void OnUpdateBtnSSCNETReadyOff(CCmdUI* pCmdUI);
	void OnUpdateBtnSSCNETServoOff(CCmdUI* pCmdUI);
	void OnUpdateBtnSSCNETServoOn(CCmdUI* pCmdUI);
	void OnBtnResetAlarm();
	void OnUpdateBtnSetpointAbs(CCmdUI* pCmdUI);
	void OnUpdateBtnInitSSCNET(CCmdUI* pCmdUI);
	void OnUpdateBtnDownloadAbsEncoder(CCmdUI* pCmdUI);
	void OnUpdateBtnUploadAbsEncoder(CCmdUI* pCmdUI);
	void OnUpdateBtnResetAlarm(CCmdUI* pCmdUI);
	void OnBtnSetpointAbs();
	void OnBtnInitSSCNET();
	void OnBtnDownloadAbsEncoder();
	void OnBtnUploadAbsEncoder();
    void OnSetAllSSCNETStations(NMHDR* pNMHDR, LRESULT* pResult);
	void OnUpdateSetAllSSCNETStations(CCmdUI* pCmdUI);
private :
    CAdamoMachine    *m_pMachine;
    CAdamoSSCNETGrid *m_pSSCNETGrid;
    bool              m_bAllNetwork;
    int               m_nCard;
    ePswLevel		  m_ePswLevel;
private :
    int  GetCurrentStation ();
    void SetMachine ();
    int  ReadAbsPosition (int nStation, WORD *pwMultiRevCounter, DWORD *pdwOneRevPos);
    int  WriteAbsPosition (int nStation, WORD wMultiRevCounter, DWORD dwOneRevPos);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDSSCNET_H__DFA6AB4B_9D9B_4DD9_B2FE_7E5E87C62324__INCLUDED_)
