#pragma once
#include "afxcmn.h"

#include "resource.h"

#define NMAX_CORR_XYZCB_IDE 3
#define NMAX_CORR_TRT_IDE 6
// finestra di dialogo CDlgKC
class CDlgKC;

class CKCGrid : public CGridCtrl
{
        CDlgKC* m_pDlg;
        virtual void OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);
        virtual void OnEndEditCell(int nRow, int nCol, CString str);
    public :
        CKCGrid ();
        void SetKCDlg (CDlgKC* pDlg)      { m_pDlg=pDlg; }
};

class CDlgKC : public CDialog
{
	DECLARE_DYNAMIC(CDlgKC)

public:
	CDlgKC(CWnd* pParent = NULL);   // costruttore standard
	virtual ~CDlgKC();
	void SetKCXYZBC (double* afpGeometry_XYZCB)   {
		memcpy (m_afpGeometry_XYZCB, afpGeometry_XYZCB, sizeof (m_afpGeometry_XYZCB));
	}
	void SetKCTRT (double* afpGeometry_TRT)   {
		memcpy (m_afpGeometry_TRT, afpGeometry_TRT, sizeof (m_afpGeometry_TRT));
	}
	void GetKCXYZBC (double* afpGeometry_XYZCB)   {
		memcpy (afpGeometry_XYZCB, m_afpGeometry_XYZCB, sizeof (m_afpGeometry_XYZCB));
	}
	void GetKCTRT (double* afpGeometry_TRT)   {
		memcpy (afpGeometry_TRT, m_afpGeometry_TRT, sizeof (m_afpGeometry_TRT));
	}
// Dati della finestra di dialogo
	enum { IDD = IDD_DLG_KINEMATIC_COMPENSATION };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTabKC(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl	m_ctrlTabKC;
	CKCGrid		m_grid;
    CImageList* m_tbiml;
    CImageList* m_gridiml;
    CBitmap*    m_bmpKCXYZBC;
    CBitmap*    m_bmpKCTRT;
    CBitmap*    m_bmpKCComment;
    CBitmap*    m_bmpKCNumber;
	double		m_afpGeometry_XYZCB[NMAX_CORR_XYZCB_IDE];
	double		m_afpGeometry_TRT[NMAX_CORR_TRT_IDE];
	int			m_nCurSel;
	bool		m_bFirstTime;
private :
	void SelectGrid (int nCurSel);
	void SelectXYZBCGrid ();
	void SelectTRTGrid ();
	int  FillData ();
private :
	void GetTRTData ();
	void GetXYZBCData (); 
	void SetTRTData ();
	void SetXYZBCData ();
};
