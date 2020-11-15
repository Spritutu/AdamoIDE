#pragma once

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"

// CAdamoRSColumns dialog

class CAdamoRSColumns : public CDialog
{
	DECLARE_DYNAMIC(CAdamoRSColumns)

public:
	CAdamoRSColumns(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdamoRSColumns();
	void SetColumnsData (stGridColumnData *pgcd);
	void GetColumnsData (stGridColumnData *pgcd);
	void SetImagesList (CStringList *images);
// Dialog Data
	enum { IDD = IDD_DLG_RS_COLS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
private :
	stGridColumnData m_gcd;
	CParsedEdit	m_editColName;
	CStringList *m_slImages;
private :
	void FillImageCombo ();
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK ();
};
