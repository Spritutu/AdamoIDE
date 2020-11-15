#pragma once

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "afxcmn.h"

// CAdamoRSEditGrid dialog

class CAdamoRSEditGrid : public CDialog
{
	DECLARE_DYNAMIC(CAdamoRSEditGrid)

public:
	CAdamoRSEditGrid(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdamoRSEditGrid();
	
// Dialog Data
	enum { IDD = IDD_DLG_RS_GRID };
	void SetGridData (int nRows, int nColumns);
	void SetColumnData (int nColumn, stGridColumnData *pgcd);
	void GetGridData (int *pnRow, int *pnColumns);
	void GetColumnData (int nColumn, stGridColumnData *pgcd);
	void SetImagesList (CStringList *images);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
private :
	CListCtrl m_listColumns;
	CStringList *m_slImages;
	CParsedEdit	m_editRows;
	CParsedEdit	m_editCols;
	stGridColumnData m_gcd[MAXCOLUMNS];
	int m_nRows;
	int m_nCols;
	DECLARE_MESSAGE_MAP()
private :
	int  FillColumns ();
	int  FillRows ();
	int  AddItem (int i, CString strName, int nType, CString strImage);
	void GetItem (int nIndex, CString& strName, int& nType, CString& strImage);
	void UpdateColumnsText ();
	void OnOK ();
public:
	afx_msg void OnBnClickedBtnRsAggiungi();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRsModifica();
	afx_msg void OnBnClickedBtnRsRimuovi();
	afx_msg void OnBnClickedBtnRsSpostaSu();
	afx_msg void OnBnClickedBtnRsSpostaGiu();
};
