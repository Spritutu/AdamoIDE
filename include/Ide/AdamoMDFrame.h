#if !defined(AFX_ADAMOMDFRAME_H__D428A6B8_CB33_4453_B6A3_0F8D9FDBC930__INCLUDED_)
#define AFX_ADAMOMDFRAME_H__D428A6B8_CB33_4453_B6A3_0F8D9FDBC930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoMDFrame.h : header file
//

class CAdamoMDFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CAdamoMDFrame)
protected:
	CAdamoMDFrame();           // protected constructor used by dynamic creation

// Operations
public:
    static CAdamoMDFrame* GetMDFrame ()   { return m_pThis; }
    void OnTimer ();
    void OnClose ();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDFrame)
	protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdamoMDFrame();

	// Generated message map functions
	//{{AFX_MSG(CAdamoMDFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
    afx_msg void OnDestroy();
    afx_msg void OnButtonPlus();
    afx_msg void OnButtonMinus();
    afx_msg void OnButtonStop();
    afx_msg void OnSetVel();
    afx_msg void OnSetFeed();
    afx_msg void OnJog();
    afx_msg void OnAbs();
    afx_msg void OnStep();
    afx_msg void OnOpenLoop();
    afx_msg void OnCloseLoop();
    afx_msg void OnAxArrowUp();
    afx_msg void OnAxArrowDown();
    afx_msg void OnRunPlc ();
    afx_msg void OnStopPlc ();
    afx_msg void OnForceAllInputsOff ();
	afx_msg void OnUpdateRunPlc (CCmdUI* pCmdUI);
	afx_msg void OnUpdateStopPlc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateForceAllInputsOff(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected :
    BOOL PreTranslateMessage(MSG* pMsg);
private :
    static CAdamoMDFrame* m_pThis;
    HACCEL m_hAccel;
    int    m_btnPressed;
};

/////////////////////////////////////////////////////////////////////////////


#endif // !defined(AFX_ADAMOMDFRAME_H__D428A6B8_CB33_4453_B6A3_0F8D9FDBC930__INCLUDED_)
