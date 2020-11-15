#if !defined(AFX_GLOBALMATRIX_H__A0FDCF61_9935_4822_8D9E_7AC37F3C5B5F__INCLUDED_)
#define AFX_GLOBALMATRIX_H__A0FDCF61_9935_4822_8D9E_7AC37F3C5B5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GlobalMatrix.h : header file
//

#include "gridctrl.h"
#include "gridcellcheck.h"
#include "adamoint.h"
#include "lobject.h"
#include "lexxer.h"
#include "slotview.h"
#include "adamomachine.h"

#define NMAX_GRIDROWS   14
#define NMAX_GRIDCOLS   7
#define ROW_HEIGTH_XP   19
#define ROW_HEIGTH_W7   22
#define COL_WIDTH_XP    78
#define COL_WIDTH_W7    78

/////////////////////////////////////////////////////////////////////////////
// CGlobalMatrix dialog

struct stColumnsData   {
    int  nType;
    char szName[NMAXIDENTIFIER-1];
};

class CMatrixListener : public cooLexxerListener
{
    friend class CGlobalMatrix;
    // Attributes
    public:
        CMatrixListener () : m_nCurrentColumn (0)   {
        }
    // Operations
    public:
	    virtual	bool fCheckValid() const	{ return true; };
	    virtual bool fShouldDelete() const { return false; };
	    virtual	void vRegisterToken(const std::tstring& strTokenText, const cooLexxerTokenRule* pptrRule);
        int  ParseMatrixString (char* szBuffer, int i);
        void ResetResults (int nColumns);
        void SetColumnsTypes (int nColumns, int* nColumnsType);
    private :
        int m_nVettTypes[MAXCOLUMNS];
        int m_nColumn; 
        int m_nCurrentColumn;
        CString strResults[MAXCOLUMNS];
};

class CGlobalMatrix : public CAdamoDialog
{
// Construction
public:
	CGlobalMatrix(CWnd* pParent = NULL);   // standard constructor
    int  SetData (const char* szMatrixName, int nRows, int nColumns, stColumnsData* aColumnsTypes);
    void OnTimer ();
// Dialog Data
	//{{AFX_DATA(CGlobalMatrix)
	enum { IDD = IDD_GLOBALMATRIX };
	CButton	m_ctrlButOk;
	CStatic	m_ctrlName;
	CButton	m_butLoadMatrix;
	CButton	m_butSaveMatrix;
	CButton	m_butFreezeMatrix;
	CButton	m_butUpdateMatrix;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlobalMatrix)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGlobalMatrix)
	virtual BOOL OnInitDialog();
	afx_msg void OnLoadMatrix();
	afx_msg void OnSaveMatrix();
	afx_msg void OnFreezeMatrix();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	//}}AFX_MSG
    afx_msg void OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* pResult);
    afx_msg void OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private :
    int  m_nColumns, m_nCurColumn;
    int  m_nRows, m_nCurRow;
    stColumnsData m_astColumnsTypes[MAXCOLUMNS];
    CImageList m_ImageList;
   	CGridCtrl m_Grid;
    bool m_bFreeze;
	bool m_bInitialized;
	CRect m_rcInitialPosition;
	CRect m_rcGridInitialPosition;
	CRect m_rcLoadMatrixInitialPosition;
	CRect m_rcSaveMatrixInitialPosition;
	CRect m_rcFreezeMatrixInitialPosition;
	CRect m_rcUpdateMatrixInitialPosition;
	CRect m_rcOkInitialPosition;
private :
    void FillColumnsCaption ();
    void FillRows ();
    int  FillMatrix (CStdioFile* pF);
    void FillMatrixFile (CStdioFile* fMatrix);
    void PrepareBufKeys (char** pBufKeys, int* nBufKeysLen);
    void ResizeDialog();
    void OnOK ();
    void OnCancel ();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedAggiornamatrix();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBALMATRIX_H__A0FDCF61_9935_4822_8D9E_7AC37F3C5B5F__INCLUDED_)
