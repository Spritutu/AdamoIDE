#pragma once

#include "ShowModulesSuperGrid.h"

// finestra di dialogo CAdamoDlgShowModules

class CAdamoDlgShowModules : public CDialog
{
	DECLARE_DYNAMIC(CAdamoDlgShowModules)

public:
	CAdamoDlgShowModules(CWnd* pParent = NULL);   // costruttore standard
	virtual ~CAdamoDlgShowModules();

	// Dati della finestra di dialogo
	enum { IDD = IDD_DLG_SHOW_MODULES };

private :
	void InizializzaBottoni ();
private :
	CShowModulesSuperGrid m_sg;
	CStatic	m_stFrame;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnReconnect();
	afx_msg void OnBnClickedBtnActivate();
    afx_msg LRESULT OnActivateMachine (WPARAM wParam, LPARAM lParam);
};
