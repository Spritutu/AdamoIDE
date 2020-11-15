#if !defined(AFX_ADAMOMDIOVIEWDLG_H__19ADAAAA_E9A4_47CA_AE4F_F5EA84A4C675__INCLUDED_)
#define AFX_ADAMOMDIOVIEWDLG_H__19ADAAAA_E9A4_47CA_AE4F_F5EA84A4C675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoMDIOViewDlg.h : header file
//

#include "IOSuperGrid.h"
#include "gridctrl.h"
#include "gridcellcheck.h"
#include "gridcelltogglebutton.h"
#include "AdamoMachine.h"

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDIOViewDlg dialog

class CAdamoMDIOViewDlg : public CDialog
{
// Construction
public:
	CAdamoMDIOViewDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAdamoMDIOViewDlg)
	enum { IDD = IDD_DLGIOVIEW };
	CIOSuperGrid m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDIOViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public :
    void OnAdamoTimer();
    void OnViewSettings (stMDSettings* MDS);
    void GetViewSettings (stMDSettings* MDS);
    void SetMachine (CAdamoMachine* pMachine) { m_pMachine=pMachine; }
    void UpdateData ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoMDIOViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
    afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridButtonPressed(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private :
    CGridCtrl  m_Grid;
    CImageList m_ImageList;
    CAdamoMachine* m_pMachine;
    int m_nCurRow;
    int m_nCurColumn;
    int m_nIniRow;
    int m_nGridRow;
    ePswLevel m_ePswLevel;
private :
    void FillColumnsCaption ();
    void FillVariables ();
    void InitializeGrid ();
    void Clear ();
    int  InsertRow (CString str);
    void SpecilizeRow (stGlobalDescriptor* gd, int nRow);
    void OnOK ();
    void OnCancel ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMOMDIOVIEWDLG_H__19ADAAAA_E9A4_47CA_AE4F_F5EA84A4C675__INCLUDED_)
