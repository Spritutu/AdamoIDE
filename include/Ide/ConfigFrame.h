#if !defined(AFX_CONFIGFRAME_H__BF0BDFCD_ADFE_485C_946E_12DC6D18F607__INCLUDED_)
#define AFX_CONFIGFRAME_H__BF0BDFCD_ADFE_485C_946E_12DC6D18F607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigFrame.h : header file
//

#include "ConfTreeView.h"
#include "AdamoConfHWView.h"

/////////////////////////////////////////////////////////////////////////////
// CConfigFrame frame

class CConfigFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CConfigFrame)
protected:
	CConfigFrame();           // protected constructor used by dynamic creation

// Attributes
public:
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigFrame)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CConfigFrame();

	// Generated message map functions
	//{{AFX_MSG(CConfigFrame)
	afx_msg void OnDestroy();
	afx_msg void OnModeLogic();
	afx_msg void OnModeHardware();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CConfTreeView *m_pLogicConf;
    CAdamoConfHWView *m_pHWConf;
private :
    int CreateConfHWView();
    CView *SwitchView ();
    void GestisciMappaEId ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGFRAME_H__BF0BDFCD_ADFE_485C_946E_12DC6D18F607__INCLUDED_)
