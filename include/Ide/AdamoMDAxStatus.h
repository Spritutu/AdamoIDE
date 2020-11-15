// TestDlgCounterDlg.h : header file
//

#if !defined(AFX_TESTDLGCOUNTERDLG_H__E6473713_1D66_48F8_BD59_35E5BD1168BF__INCLUDED_)
#define AFX_TESTDLGCOUNTERDLG_H__E6473713_1D66_48F8_BD59_35E5BD1168BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* include files */
#include "digistatic.h"
#include "bitmapslider.h"
#include "adamobase.h"
#include "adamoaxis.h"
#include "AdamoMDCommandBar.h"
#include "label.h"
#include "iangulargaugex.h"

/* constants definitions */
#define AXISROWHEIGHT 60

/* forward references */
class CAdamoMDAxStatus;

/* nome asse */
class CXTPReportRecordItemName : public CXTPReportRecordItemText
{
	public:
		CXTPReportRecordItemName (CString str) : CXTPReportRecordItemText (str)    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};

/* visualizzazione quote */
class CXTPReportRecordItemPositions : public CXTPReportRecordItemText
{
	public:
		CXTPReportRecordItemPositions () : CXTPReportRecordItemText ("")    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};

/* visualizzazione stato */
class CXTPReportRecordItemStatus : public CXTPReportRecordItemText
{
	public:
		CXTPReportRecordItemStatus () : CXTPReportRecordItemText ("")    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};

/* visualizzazione parametri dinamici */
class CXTPReportRecordItemDynamic : public CXTPReportRecordItemText
{
	public:
		CXTPReportRecordItemDynamic () : CXTPReportRecordItemText ("")    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};

/* report control paint manager */
class CXTPCustomDrawReportPaintManager : public CXTPReportPaintManager
{
public:
	CXTPCustomDrawReportPaintManager(CAdamoMDAxStatus* pParent);
	virtual ~CXTPCustomDrawReportPaintManager();
	virtual int GetRowHeight(CDC* pDC, CXTPReportRow* pRow);
	virtual void DrawFocusedRow(CDC* pDC, CRect rcRow);
protected:
	CAdamoMDAxStatus* m_pParent;
};

/* Report Control */
class CAdamoMDStatusReportCtrl : public CXTPReportControl
{
	public :
		CAdamoMDStatusReportCtrl () : m_pParent (NULL)  {
		}
		void SetParent  (CWnd *pParent)   {
			m_pParent = pParent;
		}
		CWnd *GetParent  ()   {
			return m_pParent;
		}
	private :
		CWnd* m_pParent;
};


/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxStatus dialog

class CAdamoMDAxStatus : public CDialog
{
	friend class CAdamoMDStatusReportCtrl;
	friend class CXTPReportRecordItemPositions;
	friend class CXTPReportRecordItemStatus;
// Construction
public:
	CAdamoMDAxStatus(CWnd* pParent = NULL);	// standard constructor
    ~CAdamoMDAxStatus ();
    void SettaCommandBar (CAdamoMDCommandBar* pCmdBar)   { m_pCmdBar=pCmdBar; }
    void SettaListaAssi (CPtrList* ListaAssi)            { m_la=ListaAssi; }
    void SetMachine (CAdamoMachine* pMachine)            { m_pMachine=pMachine; }
    void OnViewSettings (stMDSettings* MDS);
    void GetViewSettings (stMDSettings* MDS);
    void OnAdamoTimer ();
    void UpdateAxesData ();
    void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
	void OnFrecciaGiu ();
	void OnFrecciaSu ();

// Dialog Data
	//{{AFX_DATA(CAdamoMDAxStatus)
	enum { IDD = IDD_DLG_AXSTATUS };
    CiAngularGaugeX m_gaugeVelTeorica;
    CiAngularGaugeX m_gaugeAccelerazione;
    CiAngularGaugeX m_gaugeCoppia;
	CStatic m_stPanelKnobs;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoMDAxStatus)
	protected:
	    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdamoMDAxStatus)
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnReportSelChanged(NMHDR* pNotifyStruct, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CAdamoMDStatusReportCtrl m_wndReport;
    CPtrList* m_la;
    int m_nCount;
    CAdamoAxis*    m_apAxis[NMAXAXIS];
    CAdamoMDCommandBar* m_pCmdBar;
    CAdamoMachine* m_pMachine;
    stMDSettings*  m_pSettings;
    double         m_fpBackQuotaReale[NMAXAXIS];
    double         m_fpBackQuotaTeorica[NMAXAXIS];
    double         m_fpBackEA[NMAXAXIS];
    int            m_nBackStato[NMAXAXIS];
    int            m_nBackTM[NMAXAXIS];
    double         m_fpBackVelocitaTeorica;
    double         m_fpBackAccelerazione;
    double         m_fpBackCoppia;
    int            m_nBackCurPageAx;
    bool           m_bInitialized;
	bool		   m_bTimerOk;
    CString        *m_aszConfrontoAssi[5];
    CString        *m_aszTipoMovimento[11];
	int			   m_nSelectedAx;
	CFont          *m_pDigitFont;
	CBrush         m_br;
private :
    void UpdateData ();
    void InitBackTimerData ();
    void InitAngularGauges ();
    void DrawBitmapStatus (int nProg, struct stDatiAsse* da);
    void InitStrings ();
    void DestroyStrings ();
	void UpdateLabelUM ();
	void InitReportControl ();
	void FillColumns ();
	void FillGrid ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDLGCOUNTERDLG_H__E6473713_1D66_48F8_BD59_35E5BD1168BF__INCLUDED_)
