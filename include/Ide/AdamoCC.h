// CavStrzDlg.h : header file
//

#if !defined(AFX_CAVSTRZDLG_H__B7BAB6F2_67A0_4E03_B55D_FB578E17FE4E__INCLUDED_)
#define AFX_CAVSTRZDLG_H__B7BAB6F2_67A0_4E03_B55D_FB578E17FE4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCavStrzDlg dialog
#include "paredit.h"
#include "GridCtrl.h"
#include "Adamobase.h"
#include "AdamoAxis.h"
#include "AdamoFileDati.h"

#define NUMINTERVALLI_64     64
#define NUMINTERVALLI_128    128
#define NUMINTERVALLI_256    256

class CCavStrzDlg;

class CCavStrzGrid : public CGridCtrl
{
        CCavStrzDlg* m_pDlg;
        virtual void OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);
        virtual void OnEndEditCell(int nRow, int nCol, CString str);
    public :
        CCavStrzGrid () : m_pDlg (NULL)             { }
        void SetCavStrzDlg (CCavStrzDlg* pDlg)      { m_pDlg=pDlg; }
};

class CCavStrzDlg : public CDialog
{
// Construction
public:
	CCavStrzDlg(CWnd* pParent = NULL);	// standard constructor
    void SetActiveAxis (CAdamoAxis* pAx)        { m_pAx=pAx; }
    bool IsColumnChecked (int n);
    void    SetPathName (CString strPathName)   { m_strNomeFile=strPathName; }
    CString GetPathName ()                      { return m_strNomeFile; }
    CParsedEdit m_editIntervals;
    CParsedEdit m_editRange;
// Dialog Data
	//{{AFX_DATA(CCavStrzDlg)
	enum { IDD = IDD_DLG_CC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCavStrzDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCavStrzDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnChkAx1();
	afx_msg void OnChkAx2();
	afx_msg void OnChkAx3();
	afx_msg void OnChkAx4();
	afx_msg void OnChkAx5();
    afx_msg void OnSelAsse1 ();
    afx_msg void OnSelAsse2 ();
    afx_msg void OnSelAsse3 ();
    afx_msg void OnSelAsse4 ();
    afx_msg void OnSelAsse5 ();
	afx_msg void OnBtnAx1();
	afx_msg void OnBtnAx2();
	afx_msg void OnBtnAx3();
	afx_msg void OnBtnAx4();
	afx_msg void OnBtnAx5();
	virtual void OnOK();
	afx_msg void OnBtnApply();
	afx_msg void OnBtnDeleteCCFile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    bool InitGrid ();
    void CreateAxesList ();
    void TutteLeRigheA (const char* sz, int n);
    bool   GetLimits (double* fpPos, double* fpNeg);
    void   CalcNumCorrectors (double fpStep, double fpLimPos, double fpLimNeg, int* nNumPos, int* nNumNeg);
    double CalcInitialStep (double fpLimPos, double fpLimNeg);
    void   FillRows (double fpStep, double fpLimPos, double fpLimNeg, int nNumPos, int nNumNeg, double* fpNegInterval, double* fpPosInterval);
    void   FillButtons ();
    void   InitialSettings ();
    int  FirstFullRow ();
    int  Accorcia (int nRows);
    int  Accorcia (double fpStep);
    CString GetPath ();
    void FillGridFromFile ();
    void EnableAxes ();
    bool IsLimitChanged (double fpFileLimit, double fpAxLimit, double fpStep, bool bPositive);
    void InsertAx (CAdamoAxis *pDev);
    void Iterator (CAdamoGroup *pGrp);
    void SettaAsseOccupato (int n, CString strNomeAsse);
    void AzzeraColonneAbilitate ();
private :
    double m_fpStep, m_fpLimPos, m_fpLimNeg;
    CCavStrzFile    m_file;
    CCavStrzGrid    m_grid;
    CPtrList        m_listAxes;
    int             m_nRows;
    int             m_nNumIntervals;
    CAdamoAxis*     m_anAsseOccupato[NMAXAXESCORRETTED];
    CAdamoAxis*     m_pAx;
    int             m_bChecked[NMAXAXESCORRETTED];
    CString         m_strNomeFile;
    double          m_fpInitialStep;
    int             m_nInitialNumIntervals;
    CString         m_strRange;
    CString         m_strIntervals;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAVSTRZDLG_H__B7BAB6F2_67A0_4E03_B55D_FB578E17FE4E__INCLUDED_)
