#pragma once

#include "AdamoShowMatrixSupergrid.h"

// CShowGlobalMatrix dialog

class CShowGlobalMatrix : public CDialog
{
public:
	CShowGlobalMatrix(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShowGlobalMatrix();
    void SetResults (CString  strName);
    void GetResults (CString* strName);

// Dialog Data
	enum { IDD = IDD_DLG_SHOWMATRIX };
	CStatic	m_stFrame;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private :
    void OnOK ();
private :
    CShowMatrixSuperGrid m_sg;
    CString m_strName;
};
