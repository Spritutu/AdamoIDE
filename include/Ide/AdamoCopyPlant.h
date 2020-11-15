#pragma once

#include "paredit.h"
// CAdamoCopyPlant dialog

class CAdamoCopyPlant : public CDialog
{
	DECLARE_DYNAMIC(CAdamoCopyPlant)

public:
	CAdamoCopyPlant(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdamoCopyPlant();
	void	SetMachine (CString strMachine)		{ m_strMachine = strMachine; }
	void	SetPlant   (CString strPlant)		{ m_strPlant = strPlant; }
	CString GetMachine ()						{ return m_strMachine; }
	CString GetPlant ()							{ return m_strPlant; }
// Dialog Data
	enum { IDD = IDD_DLG_COPY_PLANT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private :
	CString m_strPlant;
	CString m_strMachine;
	CParsedEdit m_editPlant;
	CParsedEdit m_editMachine;
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
