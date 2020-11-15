#pragma once

#include "paredit.h"
// CAdamoRenameMachine dialog

class CAdamoRenameMachine : public CDialog
{
	DECLARE_DYNAMIC(CAdamoRenameMachine)

public:
	CAdamoRenameMachine(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdamoRenameMachine();
	void	SetActualMachine (CString strActualMachine)		{ m_strActualMachine = strActualMachine; }
	CString GetNewMachine ()								{ return m_strNewMachine; }
// Dialog Data
	enum { IDD = IDD_DLG_RENAME_MACHINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private :
	CString m_strActualMachine;
	CString m_strNewMachine;
	CParsedEdit m_editMachine;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
