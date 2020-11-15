// AdamoMDAxSettings.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "AdamoMDAxSettings.h"
#include "DlgOscilloscope.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxSettings dialog

/* nome asse */
class CXTPAxSettingsRecordItemName : public CXTPReportRecordItemText
{
	public:
		CXTPAxSettingsRecordItemName (CString str) : CXTPReportRecordItemText (str)    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};
/* visualizzazione quote */
class CXTPAxSettingsRecordItemPositions : public CXTPReportRecordItemText
{
	public:
		CXTPAxSettingsRecordItemPositions () : CXTPReportRecordItemText ("")    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};

/* visualizzazione stato */
class CXTPAxSettingsRecordItemStatus : public CXTPReportRecordItemText
{
	public:
		CXTPAxSettingsRecordItemStatus () : CXTPReportRecordItemText ("")    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};

/* visualizzazione parametri dinamici */
class CXTPAxSettingsRecordItemDynamic : public CXTPReportRecordItemText
{
	public:
		CXTPAxSettingsRecordItemDynamic () : CXTPReportRecordItemText ("")    {
		}
		virtual void OnDrawCaption(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics);
};

CAdamoMDAxSettings::CAdamoMDAxSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoMDAxSettings::IDD, pParent), m_pAx (NULL), m_bSingle (true), m_nEditFocused (0),
      m_pMachine (NULL), m_bModified (false), m_bInitialized (false), m_pCmdBar (NULL), m_ePswLevel (ePswUser),
	  m_pDigitFont (NULL), m_bTimerOk (false), m_bAnalogic (false)
{
	//{{AFX_DATA_INIT(CAdamoMDAxSettings)
	//}}AFX_DATA_INIT
    memset (m_aszConfrontoAssi, 0, 5*sizeof (CString *));
    memset (m_aszTipoMovimento, 0, 11*sizeof (CString *));
}

CAdamoMDAxSettings::~CAdamoMDAxSettings()
{
	DestroyStrings ();
}

void CAdamoMDAxSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoMDAxSettings)
	DDX_Control(pDX, IDC_AXSETTINGS_REPORT, m_wndReport);
	DDX_Control(pDX, IIDC_BTN_STOPTARATURA, m_btnStopTaratura);
	DDX_Control(pDX, IDC_BTN_STARTTARATURA, m_btnStartTaratura);
	DDX_Control(pDX, IDC_SLIDER_MP_ACC, m_sliderAcc);
	DDX_Control(pDX, IDC_SLIDER_MP_DEC, m_sliderDec);
	DDX_Control(pDX, IDC_SLIDER_MP_PROP, m_sliderProp);
	DDX_Control(pDX, IDC_SLIDER_MP_DER, m_sliderDer);
	DDX_Control(pDX, IDC_SLIDER_MP_INTEG, m_sliderInteg);
	DDX_Control(pDX, IDC_SLIDER_MP_FFW, m_sliderFF);
	DDX_Control(pDX, IDC_SLIDER_MP_FFWACC, m_sliderFFA);
	DDX_Control(pDX, IDC_STQP, m_lblQP);
	DDX_Control(pDX, IDC_STQN, m_lblQN);
	DDX_Control(pDX, IDC_STSETRITARDO, m_lblRitardo);
	DDX_Control(pDX, IDC_ST_EA, m_lblEA);
	DDX_Control(pDX, IDC_STSETACC, m_lblAcc);
	DDX_Control(pDX, IDC_STSETDEC, m_lblDec);
	DDX_Control(pDX, IDC_STSETP, m_lblK);
	DDX_Control(pDX, IDC_STSETD, m_lblD);
	DDX_Control(pDX, IDC_STSETI, m_lblI);
	DDX_Control(pDX, IDC_STSETFFW, m_lblFFW);
	DDX_Control(pDX, IDC_STSETFFWA, m_lblFFWA);
	DDX_Control(pDX, IDC_STSETLIMEAPOS, m_lblQLPos);
	DDX_Control(pDX, IDC_STSETLIMEANEG, m_lblQLNeg);
	DDX_Control(pDX, IDC_STSETVIEWK, m_lblViewK);
	DDX_Control(pDX, IDC_STSETVIEWD, m_lblViewD);
	DDX_Control(pDX, IDC_STSETVIEWI, m_lblViewI);
	DDX_Control(pDX, IDC_STSETVIEWFFW, m_lblViewFFW);
	DDX_Control(pDX, IDC_STSETVIEWFFWA, m_lblViewFFWA);
	DDX_Control(pDX, IDC_SETGAUGEACCELERAZIONE, m_gaugeAccelerazione);
	DDX_Control(pDX, IDC_SETGAUGECOPPIA, m_gaugeCoppia);
	DDX_Control(pDX, IDC_SETGAUGEVELTEORICA, m_gaugeVelTeorica);
	DDX_Control(pDX, IDC_ILEDBARD, m_ledD);
	DDX_Control(pDX, IDC_ILEDBARK, m_ledK);
	DDX_Control(pDX, IDC_ILEDBARI, m_ledI);
	DDX_Control(pDX, IDC_ILEDBARFFA, m_ledFFWA);
	DDX_Control(pDX, IDC_ILEDBARFF, m_ledFFW);
	DDX_Control(pDX, IDC_ILEDBAREA, m_ledEA);
	DDX_Control(pDX, IDC_ISWITCHPANELX, m_plx);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAdamoMDAxSettings, CDialog)
	//{{AFX_MSG_MAP(CAdamoMDAxSettings)
	ON_WM_CREATE ()
	ON_WM_LBUTTONUP()
	ON_EN_SETFOCUS(IDC_EDIT_ACC, OnSetFocusEditAcc)
	ON_EN_SETFOCUS(IDC_EDIT_DEC, OnSetFocusEditDec)
	ON_EN_SETFOCUS(IDC_EDIT_DER, OnSetFocusEditDer)
	ON_EN_SETFOCUS(IDC_EDIT_FFW, OnSetFocusEditFFW)
	ON_EN_SETFOCUS(IDC_EDIT_FFWA, OnSetFocusEditFFWA)
	ON_EN_SETFOCUS(IDC_EDIT_INTEG, OnSetFocusEditInteg)
	ON_EN_SETFOCUS(IDC_EDIT_LIMEANEG, OnSetFocusEditLimEANeg)
	ON_EN_SETFOCUS(IDC_EDIT_LIMEAPOS, OnSetFocusEditLimEAPos)
	ON_EN_SETFOCUS(IDC_EDIT_PROP, OnSetFocusEditProp)
	ON_EN_KILLFOCUS(IDC_EDIT_ACC, OnKillFocusEditAcc)
	ON_EN_KILLFOCUS(IDC_EDIT_DEC, OnKillFocusEditDec)
	ON_EN_KILLFOCUS(IDC_EDIT_DER, OnKillFocusEditDer)
	ON_EN_KILLFOCUS(IDC_EDIT_FFW, OnKillFocusEditFFW)
	ON_EN_KILLFOCUS(IDC_EDIT_FFWA, OnKillFocusEditFFWA)
	ON_EN_KILLFOCUS(IDC_EDIT_INTEG, OnKillFocusEditInteg)
	ON_EN_KILLFOCUS(IDC_EDIT_LIMEANEG, OnKillFocusEditLimEANeg)
	ON_EN_KILLFOCUS(IDC_EDIT_LIMEAPOS, OnKillFocusEditLimEAPos)
	ON_EN_KILLFOCUS(IDC_EDIT_PROP, OnKillFocusEditProp)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_STARTTARATURA, OnBtnStartTaratura)
	ON_BN_CLICKED(IIDC_BTN_STOPTARATURA, OnBtnStopTaratura)
	ON_BN_CLICKED(IDC_BTNSETQUOTA, OnBtnSetQuota)
	ON_BN_CLICKED(IDC_BTNSETDAC, OnBtnSetDac)
	ON_BN_CLICKED(IDC_BTNSTORE, OnBtnStore)
	ON_BN_CLICKED(IDC_BTNTRACE, OnBtnTrace)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVED, OnBitmapSliderMoved)
	ON_MESSAGE(WM_BITMAPSLIDER_MOVING, OnBitmapSliderMoving)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxSettings message handlers

/*
** OnCreate :
*/
int CAdamoMDAxSettings::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	CXTPAxSettinsPaintManager* pPaintManager = new CXTPAxSettinsPaintManager();
	m_wndReport.SetPaintManager(pPaintManager);
	m_br.CreateSolidBrush (RGB (192, 192, 192));
	return 0;
}

BOOL CAdamoMDAxSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
    /* assegnamo il livello corrente di password */
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	InitReportControl ();
    InitStrings ();
    /* subclassiamo gli edit controls */
	m_editQP.SubclassEdit(IDC_EDIT_QP, this, PES_NUMBERS|PES_FLOATS);
	m_editQN.SubclassEdit(IDC_EDIT_QN, this, PES_NUMBERS|PES_FLOATS);
	m_editProp.SubclassEdit(IDC_EDIT_PROP, this, PES_NUMBERS|PES_FLOATS);
	m_editLimEAPos.SubclassEdit(IDC_EDIT_LIMEAPOS, this, PES_NUMBERS|PES_FLOATS);
	m_editInteg.SubclassEdit(IDC_EDIT_INTEG, this, PES_NUMBERS|PES_FLOATS);
	m_editLimEANeg.SubclassEdit(IDC_EDIT_LIMEANEG, this, PES_NUMBERS|PES_FLOATS);
	m_editFFWA.SubclassEdit(IDC_EDIT_FFWA, this, PES_NUMBERS|PES_FLOATS);
	m_editFFW.SubclassEdit(IDC_EDIT_FFW, this, PES_NUMBERS|PES_FLOATS);
	m_editDer.SubclassEdit(IDC_EDIT_DER, this, PES_NUMBERS|PES_FLOATS);
	m_editDec.SubclassEdit(IDC_EDIT_DEC, this, PES_NUMBERS|PES_NUMBERS);
	m_editDelay.SubclassEdit(IDC_EDIT_DELAY, this, PES_NUMBERS|PES_FLOATS);
	m_editAcc.SubclassEdit(IDC_EDIT_ACC, this, PES_NUMBERS|PES_NUMBERS);
	m_editSetQuota.SubclassEdit(IDC_EDIT_SETQUOTA, this, PES_NUMBERS|PES_FLOATS);
	m_editSetDAC.SubclassEdit(IDC_EDIT_SETDAC, this, PES_NUMBERS|PES_FLOATS);
	/* accelerazione */
	m_sliderAcc.SetBitmapChannel( IDB_MPSHORT_CHANNEL, IDB_MPSHORT_CHANNEL_ACTIVE );
	m_sliderAcc.SetBitmapThumb( IDB_MPSHORT_THUMB, IDB_MPSHORT_THUMB_ACTIVE, TRUE );
	m_sliderAcc.DrawFocusRect( FALSE );
	m_sliderAcc.SetRange( 0, 200 );
	m_sliderAcc.SetMargin( 2, 3, 2, 0 );
    /* decelerazione */
	m_sliderDec.SetBitmapChannel( IDB_MPSHORT_CHANNEL, IDB_MPSHORT_CHANNEL_ACTIVE );
	m_sliderDec.SetBitmapThumb( IDB_MPSHORT_THUMB, IDB_MPSHORT_THUMB_ACTIVE, TRUE );
	m_sliderDec.DrawFocusRect( FALSE );
	m_sliderDec.SetRange( 0, 200 );
	m_sliderDec.SetMargin( 2, 3, 2, 0 );
    /* proporzionale */
	m_sliderProp.SetBitmapChannel( IDB_MPSHORT_CHANNEL, IDB_MPSHORT_CHANNEL_ACTIVE );
	m_sliderProp.SetBitmapThumb( IDB_MPSHORT_THUMB, IDB_MPSHORT_THUMB_ACTIVE, TRUE );
	m_sliderProp.DrawFocusRect( FALSE );
	m_sliderProp.SetRange( 0, 100 );
	m_sliderProp.SetMargin( 2, 3, 2, 0 );
    /* derivativo */
	m_sliderDer.SetBitmapChannel( IDB_MPSHORT_CHANNEL, IDB_MPSHORT_CHANNEL_ACTIVE );
	m_sliderDer.SetBitmapThumb( IDB_MPSHORT_THUMB, IDB_MPSHORT_THUMB_ACTIVE, TRUE );
	m_sliderDer.DrawFocusRect( FALSE );
	m_sliderDer.SetRange( 0, 100 );
	m_sliderDer.SetMargin( 2, 3, 2, 0 );
    /* integrale */
	m_sliderInteg.SetBitmapChannel( IDB_MPSHORT_CHANNEL, IDB_MPSHORT_CHANNEL_ACTIVE );
	m_sliderInteg.SetBitmapThumb( IDB_MPSHORT_THUMB, IDB_MPSHORT_THUMB_ACTIVE, TRUE );
	m_sliderInteg.DrawFocusRect( FALSE );
	m_sliderInteg.SetRange( 0, 100 );
	m_sliderInteg.SetMargin( 2, 3, 2, 0 );
    /* FF */
	m_sliderFF.SetBitmapChannel( IDB_MPSHORT_CHANNEL, IDB_MPSHORT_CHANNEL_ACTIVE );
	m_sliderFF.SetBitmapThumb( IDB_MPSHORT_THUMB, IDB_MPSHORT_THUMB_ACTIVE, TRUE );
	m_sliderFF.DrawFocusRect( FALSE );
	m_sliderFF.SetRange( 0, 100 );
	m_sliderFF.SetMargin( 2, 3, 2, 0 );
    /* FF Accelerazione */
	m_sliderFFA.SetBitmapChannel( IDB_MPSHORT_CHANNEL, IDB_MPSHORT_CHANNEL_ACTIVE );
	m_sliderFFA.SetBitmapThumb( IDB_MPSHORT_THUMB, IDB_MPSHORT_THUMB_ACTIVE, TRUE );
	m_sliderFFA.DrawFocusRect( FALSE );
	m_sliderFFA.SetRange( 0, 100 );
	m_sliderFFA.SetMargin( 2, 3, 2, 0 );
    /* cambiamo lo sfondo */
	m_bkg.CreateSolidBrush (RGB (192, 192, 192));
    /* inizializziamo tutte le label */
    InitLabels ();
    /* inizializziamo i bottoni per la gestione della taratura */
	m_btnStopTaratura.SetParent (this);
	m_btnStartTaratura.SetParent (this);
	m_btnStopTaratura.SetSkin(IDB_STOPTU,IDB_STOPTD,0,0,0,IDB_STOPTM,0,0,0);
	m_btnStartTaratura.SetSkin(IDB_STU,IDB_STD,0,0,0,IDB_STM,0,0,0);
	m_btnStopTaratura.SetToolTipText(LOADSTRING (IDS_ADAMOMDAXSETTINGS_1));
	m_btnStartTaratura.SetToolTipText(LOADSTRING (IDS_ADAMOMDAXSETTINGS_2));
    if (GestionePassword ())
		GestioneTipoAsse ();
    m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoMDAxSettings::UpdateAxData ()
{
	int nMode;
    char szBuffer[256];
    if (m_bSingle)   {
        m_sliderAcc.SetPos  ((int)Ax.GetAcc()/10);
        m_sliderDec.SetPos  ((int)Ax.GetDec()/10);
        m_sliderProp.SetPos ((int)Ax.GetProp()*10);
        m_sliderDer.SetPos  ((int)Ax.GetDer());
        m_sliderInteg.SetPos((int)Ax.GetInt());
        m_sliderFF.SetPos   ((int)Ax.GetFeedForward());
        m_sliderFFA.SetPos  ((int)Ax.GetFeedForwardAcc());
        sprintf (szBuffer, "%d", (int)Ax.GetAcc());
        m_editAcc.SetWindowText (szBuffer);
        sprintf (szBuffer, "%d", (int)Ax.GetDec());
        m_editDec.SetWindowText (szBuffer);
        sprintf (szBuffer, "%.2f", Ax.GetProp());
        m_editProp.SetWindowText (szBuffer);
        sprintf (szBuffer, "%.2f", Ax.GetDer());
        m_editDer.SetWindowText (szBuffer);
        sprintf (szBuffer, "%.2f", Ax.GetInt());
        m_editInteg.SetWindowText (szBuffer);
		if (m_bAnalogic)
			sprintf (szBuffer, "%.2f", Ax.GetFeedForward());
		else
			sprintf (szBuffer, "%.2f", Ax.GetCompensazioneEA ());
        m_editFFW.SetWindowText (szBuffer);
		if (m_bAnalogic)
	        sprintf (szBuffer, "%.2f", Ax.GetFeedForwardAcc());
		else
			sprintf (szBuffer, "%.2f", Ax.GetCompensazioneEAA ());
        m_editFFWA.SetWindowText (szBuffer);
		nMode = 1;
    }
    else   {
        m_sliderAcc.SetPos  ((int)Ax.GetInterAcc()/10);
        m_sliderDec.SetPos  ((int)Ax.GetInterDec()/10);
        m_sliderProp.SetPos ((int)(Ax.GetInterProp()*10.0));
        m_sliderDer.SetPos  ((int)Ax.GetInterDer());
        m_sliderInteg.SetPos((int)Ax.GetInterInt());
        m_sliderFF.SetPos   ((int)Ax.GetInterFeedForward());
        m_sliderFFA.SetPos  ((int)Ax.GetInterFeedForwardAcc());
        sprintf (szBuffer, "%d", (int)Ax.GetInterAcc());
        m_editAcc.SetWindowText (szBuffer);
        sprintf (szBuffer, "%d", (int)Ax.GetInterDec());
        m_editDec.SetWindowText (szBuffer);
        sprintf (szBuffer, "%.2f", Ax.GetInterProp());
        m_editProp.SetWindowText (szBuffer);
        sprintf (szBuffer, "%.2f", Ax.GetInterDer());
        m_editDer.SetWindowText (szBuffer);
        sprintf (szBuffer, "%.2f", Ax.GetInterInt());
        m_editInteg.SetWindowText (szBuffer);
        sprintf (szBuffer, "%.2f", Ax.GetInterFeedForward());
        m_editFFW.SetWindowText (szBuffer);
        sprintf (szBuffer, "%.2f", Ax.GetInterFeedForwardAcc());
        m_editFFWA.SetWindowText (szBuffer);
		nMode = 2;
    }
    sprintf (szBuffer, "%.2f", Ax.GetLimEAConc());
    m_editLimEAPos.SetWindowText (szBuffer);
    sprintf (szBuffer, "%.2f", Ax.GetLimEADisc());
    m_editLimEANeg.SetWindowText (szBuffer);
    InitAngularGauges ();
	m_pCmdBar->UpdateKnob (nMode);
	UpdateLabelUM ();
}

LRESULT CAdamoMDAxSettings::OnBitmapSliderMoving(WPARAM wParam, LPARAM lParam)
{
    double fp;
    char szBuffer[256];
	switch (wParam)  {
	    case IDC_SLIDER_MP_ACC :
            sprintf (szBuffer, "%d", lParam*10);
            m_editAcc.SetWindowText (szBuffer);
		    break;
	    case IDC_SLIDER_MP_DEC :
            sprintf (szBuffer, "%d", lParam*10);
            m_editDec.SetWindowText (szBuffer);
		    break;
	    case IDC_SLIDER_MP_PROP :
            fp=(double)lParam;
            fp/=10.0;
            sprintf (szBuffer, "%.2f", fp);
            m_editProp.SetWindowText (szBuffer);
		    break;
	    case IDC_SLIDER_MP_DER :
            sprintf (szBuffer, "%d", lParam);
            m_editDer.SetWindowText (szBuffer);
		    break;
	    case IDC_SLIDER_MP_INTEG :
            sprintf (szBuffer, "%d", lParam);
            m_editInteg.SetWindowText (szBuffer);
		    break;
	    case IDC_SLIDER_MP_FFW :
            sprintf (szBuffer, "%d", lParam);
            m_editFFW.SetWindowText (szBuffer);
		    break;
	    case IDC_SLIDER_MP_FFWACC :
            sprintf (szBuffer, "%d", lParam);
            m_editFFWA.SetWindowText (szBuffer);
		    break;
	}
	return 0;
}

LRESULT CAdamoMDAxSettings::OnBitmapSliderMoved(WPARAM wParam, LPARAM lParam)
{
    double fp;
    char szBuffer[256];
	switch (wParam)  {
	    case IDC_SLIDER_MP_ACC :
            sprintf (szBuffer, "%d", lParam*10);
            m_editAcc.SetWindowText (szBuffer);
            SetAcceleration (lParam*10);
		    break;
	    case IDC_SLIDER_MP_DEC :
            sprintf (szBuffer, "%d", lParam*10);
            m_editDec.SetWindowText (szBuffer);
            SetDeceleration (lParam*10);
            break;
	    case IDC_SLIDER_MP_PROP :
            fp=(double)lParam;
            fp/=10.0;
            sprintf (szBuffer, "%.2f", fp);
            m_editProp.SetWindowText (szBuffer);
            SetProp (fp);
            break;
	    case IDC_SLIDER_MP_DER :
            sprintf (szBuffer, "%d", lParam);
            m_editDer.SetWindowText (szBuffer);
            SetDer (lParam);
		    break;
	    case IDC_SLIDER_MP_INTEG :
            sprintf (szBuffer, "%d", lParam);
            m_editInteg.SetWindowText (szBuffer);
            SetInteg (lParam);
		    break;
	    case IDC_SLIDER_MP_FFW :
            sprintf (szBuffer, "%d", lParam);
            m_editFFW.SetWindowText (szBuffer);
            SetFFW (lParam);
            break;
	    case IDC_SLIDER_MP_FFWACC :
            sprintf (szBuffer, "%d", lParam);
            m_editFFWA.SetWindowText (szBuffer);
            SetFFWA (lParam);
		    break;
	}
	return 0;
}

void CAdamoMDAxSettings::OnLButtonUp(UINT nFlags, CPoint point) 
{
	AfxGetMainWnd ()->SetFocus ();
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CAdamoMDAxSettings::PreTranslateMessage(MSG* pMsg) 
{
    double fp;
    int n;

    if (pMsg->message==WM_KEYDOWN)   {
        short sh=GetKeyState (VK_RETURN);
        if (sh&0x8000)   {
            switch (m_nEditFocused)   {
	            case IDC_EDIT_ACC :
                    if (GetEditValueInt (IDC_EDIT_ACC, 0, 1000000, &n))   {
                        m_sliderAcc.SetPos (n/10);
                        SetAcceleration (n);
                    }
		            break;
	            case IDC_EDIT_DEC :
                    if (GetEditValueInt (IDC_EDIT_DEC, 0, 1000000, &n))   {
                        m_sliderDec.SetPos (n/10);
                        SetDeceleration (n);
                    }
		            break;
	            case IDC_EDIT_PROP :
                    if (GetEditValueDouble (IDC_EDIT_PROP, 0, 100.0, &fp))   {
                        m_sliderProp.SetPos ((int)(fp*10.0));
                        SetProp (fp);
                    }
		            break;
	            case IDC_EDIT_DER :
                    if (GetEditValueDouble (IDC_EDIT_DER, 0, 100.0, &fp))   {
                        m_sliderDer.SetPos ((int)fp);
                        SetDer (fp);
                    }
		            break;
	            case IDC_EDIT_INTEG :
                    if (GetEditValueDouble (IDC_EDIT_INTEG, 0, 100.0, &fp))   {
                        m_sliderInteg.SetPos ((int)fp);
                        SetInteg (fp);
                    }
		            break;
	            case IDC_EDIT_FFW :
                    if (GetEditValueDouble (IDC_EDIT_FFW, 0, 100.0, &fp))   {
                        m_sliderFF.SetPos ((int)fp);
                        SetFFW (fp);
                    }
		            break;
	            case IDC_EDIT_FFWA :
                    if (GetEditValueDouble (IDC_EDIT_FFWA, 0, 100.0, &fp))   {
                        m_sliderFFA.SetPos ((int)fp);
                        SetFFWA (fp);
                    }
		            break;
	            case IDC_EDIT_LIMEAPOS :
                    if (GetEditValueDouble (IDC_EDIT_LIMEAPOS, 0, 1000000.0, &fp))
                        SetLimEAPos (fp);
		            break;
	            case IDC_EDIT_LIMEANEG :
                    if (GetEditValueDouble (IDC_EDIT_LIMEANEG, 0, 1000000.0, &fp))
                        SetLimEANeg (fp);
		            break;
            }
            AfxGetMainWnd ()->SetFocus ();
            return TRUE;
        }
    }
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CAdamoMDAxSettings::GetEditValueInt (int nId, int nRangeMin, int nRangeMax, int* n)
{
    BOOL bB=TRUE;
    char szBuffer[256], *p;

    GetDlgItemText(nId,szBuffer,sizeof(szBuffer)-1);
    *n = (int)strtod(szBuffer, &p);
    if ((*p!=NULL) || (*n < nRangeMin) || (*n > nRangeMax))   {
        GetDlgItem(nId)->SetFocus();
        bB = FALSE;
    }
    return bB;
}

BOOL CAdamoMDAxSettings::GetEditValueDouble (int nId, double fpRangeMin, double fpRangeMax, double* fp)
{
    BOOL bB=TRUE;
    char szBuffer[256], *p;

    GetDlgItemText(nId,szBuffer,sizeof(szBuffer)-1);
    *fp = (double)strtod(szBuffer, &p);
    if ((*p!=NULL) || (*fp < fpRangeMin) || (*fp > fpRangeMax))   {
        GetDlgItem(nId)->SetFocus();
        bB = FALSE;
    }
    return bB;
}

void CAdamoMDAxSettings::OnSetFocusEditAcc() 
{
    m_nEditFocused=IDC_EDIT_ACC;    	
}

void CAdamoMDAxSettings::OnSetFocusEditDec() 
{
	m_nEditFocused=IDC_EDIT_DEC;
}

void CAdamoMDAxSettings::OnSetFocusEditDer() 
{
    m_nEditFocused=IDC_EDIT_DER;
}

void CAdamoMDAxSettings::OnSetFocusEditFFW() 
{
    m_nEditFocused=IDC_EDIT_FFW;    		
}

void CAdamoMDAxSettings::OnSetFocusEditFFWA() 
{
    m_nEditFocused=IDC_EDIT_FFWA;	
}

void CAdamoMDAxSettings::OnSetFocusEditInteg() 
{
    m_nEditFocused=IDC_EDIT_INTEG;
}

void CAdamoMDAxSettings::OnSetFocusEditLimEANeg() 
{
    m_nEditFocused=IDC_EDIT_LIMEANEG;
}

void CAdamoMDAxSettings::OnSetFocusEditLimEAPos() 
{
    m_nEditFocused=IDC_EDIT_LIMEAPOS;
}

void CAdamoMDAxSettings::OnSetFocusEditProp() 
{
    m_nEditFocused=IDC_EDIT_PROP;
}

void CAdamoMDAxSettings::OnKillFocusEditAcc() 
{
    m_nEditFocused=0;
}

void CAdamoMDAxSettings::OnKillFocusEditDec() 
{
    m_nEditFocused=0;
}

void CAdamoMDAxSettings::OnKillFocusEditDer() 
{
    m_nEditFocused=0;
}

void CAdamoMDAxSettings::OnKillFocusEditFFW() 
{
    m_nEditFocused=0;
}

void CAdamoMDAxSettings::OnKillFocusEditFFWA() 
{
    m_nEditFocused=0;
}

void CAdamoMDAxSettings::OnKillFocusEditInteg() 
{
    m_nEditFocused=0;
}

void CAdamoMDAxSettings::OnKillFocusEditLimEANeg() 
{
    m_nEditFocused=0;
}

void CAdamoMDAxSettings::OnKillFocusEditLimEAPos() 
{
    m_nEditFocused=0;
}

void CAdamoMDAxSettings::OnKillFocusEditProp() 
{
    m_nEditFocused=0;
}

void CAdamoMDAxSettings::OnOK ()
{
}

void CAdamoMDAxSettings::OnCancel ()
{
}

void CAdamoMDAxSettings::SendAxDataInt (int nType, int nValue)
{
    CDeviceData Data;

    Data.m_eDev=enAsse;
    Data.m_nAddress=Ax.GetDeviceID ();
    Data.m_nType=nType;
    Data.m_nDati=nValue;
    m_pMachine->SetDevice (1, &Data);
}

void CAdamoMDAxSettings::SendAxDataDouble (int nType, double fpValue)
{
    CDeviceData Data;

    Data.m_eDev=enAsse;
    Data.m_nAddress=Ax.GetDeviceID ();
    Data.m_nType=nType;
    Data.m_fpDati=fpValue;
    m_pMachine->SetDevice (1, &Data);
}

void CAdamoMDAxSettings::SetAcceleration (int n)
{
    int nAcc;
    if (m_bSingle)   {
        nAcc=(int)Ax.GetAcc ();
        if (nAcc!=n)   {
            Ax.SetAcc (n);
            SendAxDataInt (SAD_ACC, n);
            m_bModified=true;
        }
    }
    else   {
        nAcc=(int)Ax.GetInterAcc ();
        if (nAcc!=n)   {
            Ax.SetInterAcc (n);
            SendAxDataInt (SAD_ACC_I, n);
            m_bModified=true;
        }
    }
}

void CAdamoMDAxSettings::SetDeceleration (int n)
{
    int nDec;
    if (m_bSingle)   {
        nDec=(int)Ax.GetDec();
        if (nDec!=n)   {
            Ax.SetDec (n);
            SendAxDataInt (SAD_DEC, n);
            m_bModified=true;
        }
    }
    else   {
        nDec=(int)Ax.GetInterDec();
        if (nDec!=n)   {
            Ax.SetInterDec (n);
            SendAxDataInt (SAD_DEC_I, n);
            m_bModified=true;
        }
    }
}

void CAdamoMDAxSettings::SetProp (double fp)
{
    double fpProp;
    if (m_bSingle)   {
        fpProp=Ax.GetProp ();
        if (!ISEQUAL (fp, fpProp))   {
            Ax.SetProp (fp);
            SendAxDataDouble (SAD_PROP, fp);
            m_bModified=true;
        }
    }
    else   {
        fpProp=Ax.GetInterProp ();
        if (!ISEQUAL (fp, fpProp))   {
            Ax.SetInterProp (fp);
            SendAxDataDouble (SAD_PROP_I, fp);
            m_bModified=true;
        }
    }
}

void CAdamoMDAxSettings::SetDer (double fp)
{
    double fpDer;
    if (m_bSingle)   {
        fpDer=Ax.GetDer ();
        if (!ISEQUAL (fp, fpDer))   {
            Ax.SetDer (fp);
            SendAxDataDouble (SAD_DER, fp);
            m_bModified=true;
        }
    }
    else   {
        fpDer=Ax.GetInterDer ();
        if (!ISEQUAL (fp, fpDer))   {
            Ax.SetInterDer (fp);
            SendAxDataDouble (SAD_DER_I, fp);
            m_bModified=true;
        }
    }
}

void CAdamoMDAxSettings::SetInteg (double fp)
{
    double fpInteg;
    if (m_bSingle)   {
        fpInteg=Ax.GetInt ();
        if (!ISEQUAL (fp, fpInteg))   {
            Ax.SetInt (fp);
            SendAxDataDouble (SAD_INTEG, fp);
            m_bModified=true;
        }
    }
    else   {
        fpInteg=Ax.GetInterInt ();
        if (!ISEQUAL (fp, fpInteg))   {
            Ax.SetInterInt (fp);
            SendAxDataDouble (SAD_INTEG_I, fp);
            m_bModified=true;
        }
    }
}

void CAdamoMDAxSettings::SetFFW (double fp)
{
    double fpFFW;
    if (m_bSingle)   {
        fpFFW=Ax.GetFeedForward ();
        if (!ISEQUAL (fp, fpFFW))   {
			if (m_bAnalogic)
				Ax.SetFeedForward (fp);
			else
				Ax.SetCompensazioneEA (fp);
            SendAxDataDouble (SAD_FFW, fp);
            m_bModified=true;
        }
    }
    else   {
        fpFFW=Ax.GetInterFeedForward ();
        if (!ISEQUAL (fp, fpFFW))   {
			if (m_bAnalogic)
	            Ax.SetInterFeedForward (fp);
			else
				Ax.SetCompensazioneEA (fp);
            SendAxDataDouble (SAD_FFW_I, fp);
            m_bModified=true;
        }
    }
}

void CAdamoMDAxSettings::SetFFWA (double fp)
{
    double fpFFWA;
    if (m_bSingle)   {
        fpFFWA=Ax.GetFeedForward ();
        if (!ISEQUAL (fp, fpFFWA))   {
			if (m_bAnalogic)
				Ax.SetFeedForwardAcc (fp);
			else
				Ax.SetCompensazioneEAA (fp);
            SendAxDataDouble (SAD_FFWA, fp);
            m_bModified=true;
        }
    }
    else   {
        fpFFWA=Ax.GetInterFeedForward ();
        if (!ISEQUAL (fp, fpFFWA))   {
			if (m_bAnalogic)
	            Ax.SetInterFeedForwardAcc (fp);
			else
				Ax.SetCompensazioneEAA (fp);
            SendAxDataDouble (SAD_FFWA_I, fp);
            m_bModified=true;
        }
    }
}

void CAdamoMDAxSettings::SetLimEAPos (double fp)
{
    double fpEA;
    fpEA=Ax.GetLimEAConc();
    if (!ISEQUAL (fp, fpEA))   {
        Ax.SetLimEAConc (fp);
        SendAxDataDouble (SAD_LIMEAPOS, fp);
        m_bModified=true;
    }
}

void CAdamoMDAxSettings::SetLimEANeg (double fp)
{
    double fpEA;
    fpEA=Ax.GetLimEADisc();
    if (!ISEQUAL (fp, fpEA))   {
        Ax.SetLimEADisc (fp);
        SendAxDataDouble (SAD_LIMEANEG, fp);
        m_bModified=true;
    }
}

void CAdamoMDAxSettings::OnViewSettings (stMDSettings* MDS)
{
    char szBuffer[256];

    if (MDS->stAxSettings[Ax.GetDeviceID()].nAxSettings)    {
        m_bSingle=MDS->stAxSettings[Ax.GetDeviceID()].nModeSingleInterp!=0;
        m_plx.SetPosition (m_bSingle ? 0 : 1);
        /* inizializziamo i dati di movimento per il momento a zero */
        sprintf (szBuffer,"%8.2f", MDS->stAxSettings[Ax.GetDeviceID()].fpQuotaPos);
        m_editQP.SetWindowText (szBuffer);
        sprintf (szBuffer,"%8.2f", MDS->stAxSettings[Ax.GetDeviceID()].fpQuotaNeg);
        m_editQN.SetWindowText (szBuffer);
        sprintf (szBuffer,"%8.2f", MDS->stAxSettings[Ax.GetDeviceID()].fpDelay);
        m_editDelay.SetWindowText (szBuffer);
    }
    else   {
        /* cecchiamo lo switch del movimento singolo */
        m_plx.SetPosition (0);
        /* inizializziamo i dati di movimento per il momento a zero */
        sprintf (szBuffer,"%8.2f", 0.0);
        m_editQP.SetWindowText (szBuffer);
        m_editQN.SetWindowText (szBuffer);
        m_editDelay.SetWindowText (szBuffer);
    }
    /* settiamo le posizioni correnti */
    UpdateAxData ();
}

void CAdamoMDAxSettings::GetViewSettings (stMDSettings* MDS)
{
    char szBuffer[256];

    /* switch interpolazione o singolo */
    MDS->stAxSettings[Ax.GetDeviceID()].nModeSingleInterp=m_bSingle;
    /* quota positiva */
    m_editQP.GetWindowText (szBuffer, sizeof (szBuffer));
    MDS->stAxSettings[Ax.GetDeviceID()].fpQuotaPos=atof (szBuffer);
    /* quota negativa */
    m_editQN.GetWindowText (szBuffer, sizeof (szBuffer));
    MDS->stAxSettings[Ax.GetDeviceID()].fpQuotaNeg=atof (szBuffer);
    /* ritardo */        
    m_editDelay.GetWindowText (szBuffer, sizeof (szBuffer));
    MDS->stAxSettings[Ax.GetDeviceID()].fpDelay=atof (szBuffer);
    /* settiamo a uno il flag di prima inizializzazione */
    MDS->stAxSettings[Ax.GetDeviceID()].nAxSettings=1;
	CDialog::OnDestroy();
	/* chiediamo se salvare all'uscita */
    if (m_bModified)   {
        if (AfxMessageBox (LOADSTRING (IDS_ADAMOMDAXSETTINGS_3), MB_ICONQUESTION|MB_YESNO)==IDYES)   {
            OnBtnStore ();
        }
    }
}

HBRUSH CAdamoMDAxSettings::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	if (nCtlColor==CTLCOLOR_DLG || nCtlColor==CTLCOLOR_STATIC)
        return m_bkg;
	return hbr;
}

void CAdamoMDAxSettings::OnAdamoTimer ()
{
    CDeviceData dd;
    struct stDatiAsse da;
    char szBuffer[256];

    if (m_bInitialized)   {
        dd.m_eDev=enAsse;
        dd.m_nAddress=Ax.GetDeviceID();
        if (m_pMachine->GetAxes (1, &dd, &da)==S_OK)   {
            /* quota reale */
            if (!ISEQUAL (da.fpQuotaReale, m_fpBackQuotaReale))   {
                sprintf (szBuffer, "%8.3f", da.fpQuotaReale);
                m_fpBackQuotaReale=da.fpQuotaReale;
            }
            /* quota  teorica */
            if (!ISEQUAL (da.fpQuotaTeorica, m_fpBackQuotaTeorica))   {
                sprintf (szBuffer, "%8.3f", da.fpQuotaTeorica);
                m_fpBackQuotaTeorica=da.fpQuotaTeorica;
            }
            /* velocita teorica */
            if (!ISEQUAL (da.fpVelAttuale, m_fpBackVelocitaTeorica))   {
                sprintf (szBuffer, "%8.3f", da.fpVelAttuale);
                m_fpBackVelocitaTeorica=da.fpVelAttuale;
                m_gaugeVelTeorica.SetPosition (fabs (m_fpBackVelocitaTeorica));
				m_gaugeVelTeorica.SetLabel2Text (szBuffer);
            }
            /* accelerazione */
            if (!ISEQUAL (da.fpAccInstInt, m_fpBackAccelerazione))   {
                sprintf (szBuffer, "%8.3f", da.fpAccInstInt);
                m_fpBackAccelerazione=da.fpAccInstInt;
                m_gaugeAccelerazione.SetPosition (fabs (m_fpBackAccelerazione));
				m_gaugeAccelerazione.SetLabel2Text (szBuffer);
            }
            /* coppia */
			if (!ISEQUAL (da.fpTorqueReale, m_fpBackCoppia))   {
				sprintf (szBuffer, "%8.3f", da.fpTorqueReale);
                m_fpBackCoppia=da.fpTorqueReale;
                m_gaugeCoppia.SetPosition (m_fpBackCoppia);
                m_gaugeCoppia.SetLabel2Text (szBuffer);
            }
            /* stato */
            if (da.nStato!=m_nBackStStato)   {
                if (!da.nStato)
                    da.nStato=QUOTA;    
                m_nBackStStato=da.nStato;
            }
            /* tipo movimento */
            if (da.nTipo!=m_nBackStTipoMovimento)   {
                m_nBackStTipoMovimento=da.nTipo;
            }
            /* DAC */
            if (da.nDACAsse!=m_nBackStDAC)   {
                m_nBackStDAC=da.nDACAsse;
            }
            /* errore d'anello */
            if (!ISEQUAL (da.fpErroreAnello, m_fpBackEA))   {
				m_ledEA.SetPosition (da.fpErroreAnello);
                m_fpBackEA=da.fpErroreAnello;
            }
            /* K */
            if (da.nProporzionale!=m_nBackStK)   {
                m_ledK.SetPosition (abs (da.nProporzionale));
                m_nBackStK=da.nProporzionale;
            }
            /* D */
            if (da.nDerivativa!=m_nBackStD)   {
                m_ledD.SetPosition (abs (da.nDerivativa));
                m_nBackStD=da.nDerivativa;
            }
            /* I */
            if (da.nIntegrativa!=m_nBackStI)   {
                m_ledI.SetPosition (abs (da.nIntegrativa));
                m_nBackStI=da.nIntegrativa;
            }
            /* FFW */
            if (da.nFeedForward!=m_nBackStFFW)   {
                m_ledFFW.SetPosition (abs (da.nFeedForward));
                m_nBackStFFW=da.nFeedForward;
            }
            /* FFWA */
            if (da.nFeedForwardA!=m_nBackStFFWA)   {
                m_ledFFWA.SetPosition (abs (da.nFeedForwardA));
                m_nBackStFFWA=da.nFeedForwardA;
            }
			m_wndReport.RedrawControl ();
			m_bTimerOk = true;
        }
    }
}


BEGIN_EVENTSINK_MAP(CAdamoMDAxSettings, CDialog)
    //{{AFX_EVENTSINK_MAP(CAdamoMDAxSettings)
	ON_EVENT(CAdamoMDAxSettings, IDC_ISWITCHPANELX, 1 /* OnPositionChange */, OnPositionChange, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAdamoMDAxSettings::InitLabels ()
{
    CString str;

    m_lblQP.SetTransparent (true);
    m_lblQP.SetFont3D (true);
    m_lblQP.SetTextColor (RGB (255, 255, 255));
    m_lblQP.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblQP.SetFontName ("Segoe UI");
    m_lblQP.SetFontSize (10);
    m_lblQP.SetFontBold (true);
    m_lblQP.SetFontItalic (false);
	str = LOADSTRING (IDS_ADAMOMDAXSETTINGS_9);
    m_lblQP.SetText (str);

    m_lblQN.SetTransparent (true);
    m_lblQN.SetFont3D (true);
    m_lblQN.SetTextColor (RGB (255, 255, 255));
    m_lblQN.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblQN.SetFontName ("Segoe UI");
    m_lblQN.SetFontSize (10);
    m_lblQN.SetFontBold (true);
    m_lblQN.SetFontItalic (false);
	str = LOADSTRING (IDS_ADAMOMDAXSETTINGS_8);
    m_lblQN.SetText (str);

    m_lblRitardo.SetTransparent (true);
    m_lblRitardo.SetFont3D (true);
    m_lblRitardo.SetTextColor (RGB (255, 255, 255));
    m_lblRitardo.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblRitardo.SetFontName ("Segoe UI");
    m_lblRitardo.SetFontSize (10);
    m_lblRitardo.SetFontBold (true);
    m_lblRitardo.SetFontItalic (false);
	str = LOADSTRING (IDS_ADAMOMDAXSETTINGS_10);
    m_lblRitardo.SetText (str);

    m_lblEA.SetTransparent (true);
    m_lblEA.SetFont3D (true);
    m_lblEA.SetTextColor (RGB (255, 255, 255));
    m_lblEA.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblEA.SetFontName ("Segoe UI");
    m_lblEA.SetFontSize (10);
    m_lblEA.SetFontBold (true);
    m_lblEA.SetFontItalic (false);
	str = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_34);
    m_lblEA.SetText (str);

	m_lblAcc.SetTransparent (true);
    m_lblAcc.SetFont3D (true);
    m_lblAcc.SetTextColor (RGB (255, 255, 255));
    m_lblAcc.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblAcc.SetFontName ("Segoe UI");
    m_lblAcc.SetFontSize (10);
    m_lblAcc.SetFontBold (true);
    m_lblAcc.SetFontItalic (false);
    m_lblAcc.GetWindowText (str);
    m_lblAcc.SetText (str);

    m_lblDec.SetTransparent (true);
    m_lblDec.SetFont3D (true);
    m_lblDec.SetTextColor (RGB (255, 255, 255));
    m_lblDec.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblDec.SetFontName ("Segoe UI");
    m_lblDec.SetFontSize (10);
    m_lblDec.SetFontBold (true);
    m_lblDec.SetFontItalic (false);
    m_lblDec.GetWindowText (str);
    m_lblDec.SetText (str);

    m_lblK.SetTransparent (true);
    m_lblK.SetFont3D (true);
    m_lblK.SetTextColor (RGB (255, 255, 255));
    m_lblK.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblK.SetFontName ("Segoe UI");
    m_lblK.SetFontSize (10);
    m_lblK.SetFontBold (true);
    m_lblK.SetFontItalic (false);
    m_lblK.GetWindowText (str);
    m_lblK.SetText (str);

    m_lblD.SetTransparent (true);
    m_lblD.SetFont3D (true);
    m_lblD.SetTextColor (RGB (255, 255, 255));
    m_lblD.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblD.SetFontName ("Segoe UI");
    m_lblD.SetFontSize (10);
    m_lblD.SetFontBold (true);
    m_lblD.SetFontItalic (false);
    m_lblD.GetWindowText (str);
    m_lblD.SetText (str);

    m_lblI.SetTransparent (true);
    m_lblI.SetFont3D (true);
    m_lblI.SetTextColor (RGB (255, 255, 255));
    m_lblI.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblI.SetFontName ("Segoe UI");
    m_lblI.SetFontSize (10);
    m_lblI.SetFontBold (true);
    m_lblI.SetFontItalic (false);
    m_lblI.GetWindowText (str);
    m_lblI.SetText (str);

    m_lblFFW.SetTransparent (true);
    m_lblFFW.SetFont3D (true);
    m_lblFFW.SetTextColor (RGB (255, 255, 255));
    m_lblFFW.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblFFW.SetFontName ("Segoe UI");
    m_lblFFW.SetFontSize (10);
    m_lblFFW.SetFontBold (true);
    m_lblFFW.SetFontItalic (false);
    m_lblFFW.GetWindowText (str);
    m_lblFFW.SetText (str);

    m_lblFFWA.SetTransparent (true);
    m_lblFFWA.SetFont3D (true);
    m_lblFFWA.SetTextColor (RGB (255, 255, 255));
    m_lblFFWA.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblFFWA.SetFontName ("Segoe UI");
    m_lblFFWA.SetFontSize (10);
    m_lblFFWA.SetFontBold (true);
    m_lblFFWA.SetFontItalic (false);
    m_lblFFWA.GetWindowText (str);
    m_lblFFWA.SetText (str);

    m_lblQLPos.SetTransparent (true);
    m_lblQLPos.SetFont3D (true);
    m_lblQLPos.SetTextColor (RGB (255, 255, 255));
    m_lblQLPos.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblQLPos.SetFontName ("Segoe UI");
    m_lblQLPos.SetFontSize (10);
    m_lblQLPos.SetFontBold (true);
    m_lblQLPos.SetFontItalic (false);
    m_lblQLPos.GetWindowText (str);
    m_lblQLPos.SetText (str);

    m_lblQLNeg.SetTransparent (true);
    m_lblQLNeg.SetFont3D (true);
    m_lblQLNeg.SetTextColor (RGB (255, 255, 255));
    m_lblQLNeg.SetText3DHiliteColor (RGB (255, 0, 0));
    m_lblQLNeg.SetFontName ("Segoe UI");
    m_lblQLNeg.SetFontSize (10);
    m_lblQLNeg.SetFontBold (true);
    m_lblQLNeg.SetFontItalic (false);
    m_lblQLNeg.GetWindowText (str);
    m_lblQLNeg.SetText (str);

    m_lblViewK.SetTransparent (true);
    m_lblViewK.SetTextColor (RGB (255, 0, 0));
    m_lblViewK.SetFontName ("Segoe UI");
    m_lblViewK.SetFontSize (12);
    m_lblViewK.SetFontBold (true);
    m_lblViewK.SetFontItalic (false);
    m_lblViewK.GetWindowText (str);
    m_lblViewK.SetText (str);

    m_lblViewD.SetTransparent (true);
    m_lblViewD.SetTextColor (RGB (255, 0, 0));
    m_lblViewD.SetFontName ("Segoe UI");
    m_lblViewD.SetFontSize (12);
    m_lblViewD.SetFontBold (true);
    m_lblViewD.SetFontItalic (false);
    m_lblViewD.GetWindowText (str);
    m_lblViewD.SetText (str);

    m_lblViewI.SetTransparent (true);
    m_lblViewI.SetTextColor (RGB (255, 0, 0));
    m_lblViewI.SetFontName ("Segoe UI");
    m_lblViewI.SetFontSize (12);
    m_lblViewI.SetFontBold (true);
    m_lblViewI.SetFontItalic (false);
    m_lblViewI.GetWindowText (str);
    m_lblViewI.SetText (str);

    m_lblViewFFW.SetTransparent (true);
    m_lblViewFFW.SetTextColor (RGB (255, 0, 0));
    m_lblViewFFW.SetFontName ("Segoe UI");
    m_lblViewFFW.SetFontSize (12);
    m_lblViewFFW.SetFontBold (true);
    m_lblViewFFW.SetFontItalic (false);
    m_lblViewFFW.GetWindowText (str);
    m_lblViewFFW.SetText (str);

    m_lblViewFFWA.SetTransparent (true);
    m_lblViewFFWA.SetTextColor (RGB (255, 0, 0));
    m_lblViewFFWA.SetFontName ("Segoe UI");
    m_lblViewFFWA.SetFontSize (12);
    m_lblViewFFWA.SetFontBold (true);
    m_lblViewFFWA.SetFontItalic (false);
    m_lblViewFFWA.GetWindowText (str);
    m_lblViewFFWA.SetText (str);
}

void CAdamoMDAxSettings::InitAngularGauges ()
{
    double fpAcc, fpDec;
    if (m_bSingle)   {
        m_gaugeVelTeorica.SetPositionMin (0.0);
        m_gaugeVelTeorica.SetPositionMax (Ax.GetSpeed ());
        fpAcc=(Ax.GetSpeed()/60.0)/(Ax.GetAcc()/1000.0);
        fpDec=(Ax.GetSpeed()/60.0)/(Ax.GetDec()/1000.0);
        if (fpDec>fpAcc)
            fpAcc=fpDec;
    }
    else   {
        m_gaugeVelTeorica.SetPositionMin (0.0);
        m_gaugeVelTeorica.SetPositionMax (Ax.GetInterSpeed ());
        fpAcc=(Ax.GetInterSpeed ()/60.0)/(Ax.GetInterAcc()/1000.0);
        fpDec=(Ax.GetInterSpeed ()/60.0)/(Ax.GetInterDec()/1000.0);
        if (fpDec>fpAcc)
            fpAcc=fpDec;
        }    
    m_gaugeCoppia.SetPositionMin (-Ax.GetPercTorque ());
    m_gaugeCoppia.SetPositionMax (Ax.GetPercTorque ());
    m_gaugeAccelerazione.SetPositionMin (0.0);
    m_gaugeAccelerazione.SetPositionMax (fpAcc);
}

void CAdamoMDAxSettings::OnPositionChange() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	switch (m_plx.GetPosition ())   {
        case 0 :
            m_bSingle=true;	
            break;
        case 1 :
            m_bSingle=false;	
            break;
    }
    UpdateAxData ();
}

void CAdamoMDAxSettings::OnBtnStartTaratura() 
{
    double fpQuotaNeg, fpQuotaPos, fpRitardo, fpSpeed;
	bool bLancia = false;

    if (m_pMachine->IsCncRunning ())   {
        if (GetMoveParameters (&fpQuotaNeg, &fpQuotaPos, &fpRitardo, &fpSpeed))   {
            /* componiamo il testo della function */
			if (m_bSingle)   
				if (ComponiTemporary (fpQuotaNeg, fpQuotaPos, fpRitardo, fpSpeed))
					bLancia = true;
				else;
			else 
				if (ComponiInterpTemporary (fpQuotaNeg, fpQuotaPos, fpRitardo, fpSpeed))
					bLancia = true;
				else;
			if (bLancia)   {
                /* compiliamola */
                LanciaTemporary ();
                /* un po di pulizia */
                EliminaTemporary ();
            }
        }
    }
    else
        AfxMessageBox (LOADSTRING (IDS_ADAMOMDAXSETTINGS_4));
}

void CAdamoMDAxSettings::OnBtnStopTaratura() 
{
    /* prendiamo innanzitutto il numero dello slot */
    TerminaTemporary ();	
}

bool CAdamoMDAxSettings::GetMoveParameters (double* pfpQuotaNeg, double* pfpQuotaPos, double* pfpRitardo, double *fpSpeed)
{
    double fpQuotaNeg, fpQuotaPos, fpRitardo;
    bool bStringValid = false, b=false;
    char* p;

    CString str;
    m_editQN.GetWindowText (str);
    str.TrimLeft(); str.TrimRight();
    fpQuotaNeg=strtod (str, &p);
    if (*p=='\0')   {
        m_editQP.GetWindowText (str);
        str.TrimLeft(); str.TrimRight();
        fpQuotaPos=strtod (str, &p);
        if (*p=='\0')   {
            m_editDelay.GetWindowText (str);
            str.TrimLeft(); str.TrimRight();
			if (str != "")   {
            fpRitardo=strtod (str, &p);
				bStringValid = true;
			}
			else 
				fpRitardo = 0.0;
            if ((bStringValid && *p == '\0') || !bStringValid)   {
                if (fpRitardo>=0.0)   {
                        /* tutto ok */
                        *pfpQuotaNeg=fpQuotaNeg; *pfpQuotaPos=fpQuotaPos; *pfpRitardo=fpRitardo;
                        m_pCmdBar->GetSpeed (fpSpeed, TRUE);
                        b=true;
                    }
                    else   {
                        /* errore */
                    m_editDelay.SetFocus ();
                    m_editDelay.SetSel (0, -1);
                    AfxMessageBox (LOADSTRING (IDS_ADAMOMDAXSETTINGS_6));
                }
            }
            else   {
                /* errore */
                m_editDelay.SetFocus ();
                m_editDelay.SetSel (0, -1);
                AfxMessageBox (LOADSTRING (IDS_ADAMOMDAXSETTINGS_7));
            }
        }    
        else   {
            /* errore */
            m_editQP.SetFocus ();
            m_editQP.SetSel (0, -1);
            AfxMessageBox (LOADSTRING (IDS_ADAMOMDAXSETTINGS_7));
        }
    }
    else   {
        /* errore */
        m_editQN.SetFocus ();
        m_editQN.SetSel (0, -1);
        AfxMessageBox (LOADSTRING (IDS_ADAMOMDAXSETTINGS_7));
    }
    return b;
}

bool CAdamoMDAxSettings::ComponiTemporary (double fpQuotaPos, double fpQuotaNeg, double fpRitardo, double fpSpeed)
{
    bool b=false;
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        CString strFile = pPrj->GetProjectDir() + "\\" + TEMPORARY_TUNING, strText;
        FILE* f=fopen (strFile, "w");
        if (f)   {
            /* intestazione della funzione */
            strText.Format ("while (true) do\r\n");
            fputs (strText, f);
            /* andiamo avanti */
            strText.Format ("axes.Speed (%d, %.2f)\r\n", m_pAx->GetDeviceID(), fpSpeed);
            fputs (strText, f);
            strText.Format ("axes.Goto (%d, %.3f)\r\n", m_pAx->GetDeviceID(), fpQuotaPos);
            fputs (strText, f);
            strText.Format ("axes.WaitMovement (%d)\r\n", m_pAx->GetDeviceID());
            fputs (strText, f);
            strText.Format ("misc.Delay (%.3f)\r\n", fpRitardo);
            fputs (strText, f);
            /* e ora indietro */
            strText.Format ("axes.Speed (%d, %.2f)\r\n", m_pAx->GetDeviceID(), fpSpeed);
            fputs (strText, f);
            strText.Format ("axes.Goto (%d, %.3f)\r\n", m_pAx->GetDeviceID(), fpQuotaNeg);
            fputs (strText, f);
            strText.Format ("axes.WaitMovement (%d)\r\n", m_pAx->GetDeviceID());
            fputs (strText, f);
            strText.Format ("misc.Delay (%.3f)\r\n", fpRitardo);
            fputs (strText, f);
            /* chiusura */
            strText.Format ("end\r\n");
            fputs (strText, f);
            fclose (f);
            b=true;
        }
    }
    return b;
}

bool CAdamoMDAxSettings::ComponiInterpTemporary (double fpQuotaPos, double fpQuotaNeg, double fpRitardo, double fpSpeed)
{
    bool b=false;
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        CString strFile = pPrj->GetProjectDir() + "\\" + TEMPORARY_TUNING, strText;
        FILE* f=fopen (strFile, "w");
        if (f)   {
            /* intestazione della funzione */
			strText.Format ("local nCanale\r\n");
            fputs (strText, f);
            /* apriamo il canale di interpolazione */
			strText.Format ("nCanale = interp.OpenChannel (%d)\r\n", m_pAx->GetDeviceID(), fpSpeed);
			fputs (strText, f);
			/* cicliamo i movimenti */
            strText.Format ("while (true) do\r\n");
            fputs (strText, f);
            strText.Format ("interp.Feed (nCanale, %.2f)\r\n", fpSpeed);
            fputs (strText, f);
            strText.Format ("interp.Goto (nCanale, 0, %.3f)\r\n", fpQuotaPos);
            fputs (strText, f);
            strText.Format ("interp.WaitProfile (nCanale)\r\n");
            fputs (strText, f);
            strText.Format ("misc.Delay (%.3f)\r\n", fpRitardo);
            fputs (strText, f);
            /* e ora indietro */
            strText.Format ("interp.Feed (nCanale, %.2f)\r\n", fpSpeed);
            fputs (strText, f);
            strText.Format ("interp.Goto (nCanale, 0, %.3f)\r\n", fpQuotaNeg);
            fputs (strText, f);
            strText.Format ("interp.WaitProfile (nCanale)\r\n");
            fputs (strText, f);
            strText.Format ("misc.Delay (%.3f)\r\n", fpRitardo);
            fputs (strText, f);
            /* chiusura */
            strText.Format ("end\r\n");
            fputs (strText, f);
            fclose (f);
            b=true;
        }
    }
    return b;
}

void CAdamoMDAxSettings::LanciaTemporary ()
{
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        CString strFile = pPrj->GetProjectDir() + "\\" + TEMPORARY_TUNING;
        pPrj->LanciaTemporary (strFile, "_TuningSlot");
    }
}

void CAdamoMDAxSettings::EliminaTemporary ()
{
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        CString strFile = pPrj->GetProjectDir() + "\\" + TEMPORARY_TUNING;
        CFile::Remove (strFile);
    }
}

void CAdamoMDAxSettings::TerminaTemporary ()
{
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        pPrj->TerminaTemporary ("_TuningSlot");
    }
}


void CAdamoMDAxSettings::OnBtnSetQuota() 
{
    double fp;
    BOOL bB=TRUE;
    char szBuffer[256], *p;

    GetDlgItemText(IDC_EDIT_SETQUOTA, szBuffer, sizeof(szBuffer)-1);
    fp = (double)strtod (szBuffer, &p);
    if (*p=='\0')
        SendAxDataDouble (SAD_SETQUOTA, fp);    
}

void CAdamoMDAxSettings::OnBtnSetDac() 
{
    int n;
    BOOL bB=TRUE;
    char szBuffer[256];

    GetDlgItemText(IDC_EDIT_SETDAC, szBuffer, sizeof(szBuffer)-1);
    n = atoi (szBuffer);
    SendAxDataInt (SAD_SETDAC, n);
}

void CAdamoMDAxSettings::OnBtnStore() 
{
	// TODO: Add your control notification handler code here
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        if (m_bModified)   {
            /* assegnamo alla lista della configurazione l'oggetto modificato */
            *m_pAx=Ax;
            /* memorizziamo i dati */
            pPrj->StoreConfigData ();
            /* dati salvati */
            m_bModified=false;
        }
    }
}

void CAdamoMDAxSettings::OnBtnTrace() 
{
    CDlgOscilloscope* pOsc=CDlgOscilloscope::GetOscilloscope ();
    if (pOsc)
        pOsc->TraceFromAxSettings (Ax.GetPathName()+"."+Ax.GetName());
}

/*
** InitStrings : inizializza le stringhe della dialog
*/
void CAdamoMDAxSettings::InitStrings ()
{
    int i;

    for (i=0; i<5; i++)   {
        if (m_aszConfrontoAssi[i]==NULL)   {
            m_aszConfrontoAssi[i]=new CString;
            *m_aszConfrontoAssi[i]=LOADSTRING (IDS_ADAMOTRIGGERGRID_11+i);
        }
    }
    for (i=0; i<11; i++)   {
        if (m_aszTipoMovimento[i]==NULL)   {
            m_aszTipoMovimento[i]=new CString;
            *m_aszTipoMovimento[i]=LOADSTRING (IDS_ADAMO_AXIS_STATUS_0+i);
        }
    }
}

/*
** DestroyStrings : distrugge le stringhe della dialog
*/
void CAdamoMDAxSettings::DestroyStrings ()
{
    int i;

    for (i=0; i<5; i++)
        _delete (m_aszConfrontoAssi[i]);
    for (i=0; i<11; i++)
        _delete (m_aszTipoMovimento[i]);
}

/*
** GestionePassword :
*/
bool CAdamoMDAxSettings::GestionePassword ()
{
    bool bEnable=false;
    if (m_ePswLevel<=m_pAx->GetWAcces ())
        bEnable=true;
	m_editQP.EnableWindow (bEnable);
	m_editQN.EnableWindow (bEnable);
	m_editProp.EnableWindow (bEnable);
	m_editLimEAPos.EnableWindow (bEnable);
	m_editInteg.EnableWindow (bEnable);
	m_editLimEANeg.EnableWindow (bEnable);
	m_editFFWA.EnableWindow (bEnable);
	m_editFFW.EnableWindow (bEnable);
	m_editDer.EnableWindow (bEnable);
	m_editDec.EnableWindow (bEnable);
	m_editDelay.EnableWindow (bEnable);
	m_editAcc.EnableWindow (bEnable);
    m_editSetQuota.EnableWindow (bEnable);
    m_editSetDAC.EnableWindow (bEnable);
	m_btnStopTaratura.EnableWindow (bEnable);
	m_btnStartTaratura.EnableWindow (bEnable);
	m_sliderAcc.EnableWindow (bEnable);
	m_sliderDec.EnableWindow (bEnable);
	m_sliderProp.EnableWindow (bEnable);
	m_sliderDer.EnableWindow (bEnable);
	m_sliderInteg.EnableWindow (bEnable);
	m_sliderFF.EnableWindow (bEnable);
	m_sliderFFA.EnableWindow (bEnable);
	m_plx.EnableWindow (bEnable);
    GetDlgItem(IDC_BTNSETQUOTA)->EnableWindow (bEnable);
    GetDlgItem(IDC_BTNSETDAC)->EnableWindow (bEnable);
    GetDlgItem(IDC_BTNSTORE)->EnableWindow (bEnable);
    GetDlgItem(IDC_BTNTRACE)->EnableWindow (bEnable);
	return bEnable;
}

/*
** OnChangePsw :
*/
void CAdamoMDAxSettings::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    if (GestionePassword ())
		GestioneTipoAsse ();
}

/*
** UpdateLabelUM :
*/
void CAdamoMDAxSettings::UpdateLabelUM ()
{
	CString strSpazio, strVel, str;

	switch (m_pAx->GetUnit ())   {
		case enUMMmin :
			strSpazio = LOADSTRING (IDS_ADAMOGRIDVIEW_139);
			strVel = LOADSTRING (IDS_ADAMOGRIDVIEW_140);
			break;
		case enUMinch :
			strSpazio = LOADSTRING (IDS_ADAMOGRIDVIEW_142);
			strVel = LOADSTRING (IDS_ADAMOGRIDVIEW_143);
			break;
		case enUMgradisec :
			strSpazio = LOADSTRING (IDS_ADAMOGRIDVIEW_145);
			strVel = LOADSTRING (IDS_ADAMOGRIDVIEW_146);
			break;
		case enUMgiri :
			strSpazio = LOADSTRING (IDS_ADAMOGRIDVIEW_148);
			strVel = LOADSTRING (IDS_ADAMOGRIDVIEW_149);
			break;
		case enUMMMmin :
			strSpazio = LOADSTRING (IDS_ADAMOGRIDVIEW_139);
			strVel = LOADSTRING (IDS_ADAMOGRIDVIEW_178);
			break;
		case enUMgradimin :
			strSpazio = LOADSTRING (IDS_ADAMOGRIDVIEW_145);
			strVel = LOADSTRING (IDS_ADAMOGRIDVIEW_179);
			break;
	}
	str.Format ("%s (%s)", LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_8), strSpazio);
	//m_lblQuotaReale.SetText (str);

	str.Format ("%s (%s)", LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_7), strSpazio);
	//m_lblQuotaTeorica.SetText (str);
}

/*
** OnSize :
*/
void CAdamoMDAxSettings::OnSize (UINT nType, int cx, int cy)
{
	if (m_bInitialized)   {
		CDialog::OnSize(nType, cx, cy);
		CRect rc;
		GetClientRect (&rc);
		int yReportControl = cy;
		int nRow = yReportControl / AXISROWHEIGHT;
		if (nRow > m_wndReport.GetRecords ()->GetCount ())
			nRow = m_wndReport.GetRecords ()->GetCount ();
		yReportControl = nRow * AXISROWHEIGHT;
		int nHeaderHeight = m_wndReport.GetPaintManager ()->GetHeaderHeight ();
		m_wndReport.SetWindowPos(NULL, 0, 0, 450, yReportControl + nHeaderHeight + 2, 0);
		rc.left = cx-219; rc.top = 0; rc.right = rc.left + 218; rc.bottom = rc.top + 218;
		m_gaugeVelTeorica.MoveWindow (&rc);
		rc.left = cx-219; rc.top = 220; rc.right = rc.left + 218; rc.bottom = rc.top + 218;
		m_gaugeCoppia.MoveWindow (&rc);
		rc.left = cx-219; rc.top = 440; rc.right = rc.left + 218; rc.bottom = rc.top + 218;
		m_gaugeAccelerazione.MoveWindow (&rc);
	}
}

/*
** InitReportControl :
*/
void CAdamoMDAxSettings::InitReportControl ()
{
	/* per prima cosa settiamo le colonne */
	FillColumns ();
	/* poi inseriamo i record */
	FillGrid ();
	/* scateniamo l'inferno */
    m_wndReport.SetFocus ();
	m_wndReport.Populate ();
	/* esplodiamo tutti i rami */
	m_wndReport.ExpandAll (TRUE);
	m_wndReport.SetMultipleSelection (FALSE);
	/* settiamo il font */
	m_pDigitFont = ((CMainFrame *)AfxGetMainWnd ())->CreateFont ("Segoe UI", 40, FW_BOLD);
}

/*
** FillColumns :
*/
void CAdamoMDAxSettings::FillColumns ()
{
	CXTPReportColumn* pColumn;
	int nItemIndex = 0;

	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMOMDAXSETTINGS_11), 300);
	m_wndReport.AddColumn (pColumn);
	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMOMDAXSETTINGS_12), 300);
	m_wndReport.AddColumn (pColumn);
}

/*
** FillGrid :
*/
void CAdamoMDAxSettings::FillGrid ()
{
	CXTPReportRecord* pRecord = NULL;
	CXTPReportRecordItemText* pItem;
	CString str;
	int i = 0;

	while (i < 5)   {
		pRecord = new CXTPReportRecord();
		switch (i)   {
			case 0 :
				str = LOADSTRING (IDS_ADAMO_AXIS_STATUS_12);
				break;
			case 1 :
				str = LOADSTRING (IDS_ADAMO_AXIS_STATUS_14);
				break;
			case 2 :
				str = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_10);
				break;
			case 3 :
				str = LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_5);
				break;
			case 4 :
				str = LOADSTRING (IDS_ADAMOMDAXSETTINGS_13);
				break;
		}
		pItem = (CXTPReportRecordItemName *) new CXTPAxSettingsRecordItemName (str);
		pRecord->AddItem (pItem);
		switch (i)   {
			case 0 :
			case 1 :
			case 2 :
				pItem = (CXTPAxSettingsRecordItemPositions *) new CXTPAxSettingsRecordItemPositions ();
				break;
			case 3 :
				pItem = (CXTPAxSettingsRecordItemStatus *) new CXTPAxSettingsRecordItemStatus ();
				break;
			case 4 :
				pItem = (CXTPAxSettingsRecordItemDynamic *) new CXTPAxSettingsRecordItemDynamic ();
				break;
		}
		pRecord->AddItem (pItem);
		m_wndReport.AddRecord (pRecord);
		i++;
	}
}

/*
** GestioneTipoAsse :
*/
void CAdamoMDAxSettings::GestioneTipoAsse ()
{
	enCategoriaAsse eCategory = (enCategoriaAsse) m_pAx->GetAxisType ();
	m_bAnalogic = eCategory == enAsseContinua || eCategory == enAssePassopasso || eCategory == enAsseFrequenza /*|| eCategory == enAsseVirtuale*/;
	if (m_bAnalogic)   {
		m_ledD.SetEnabled (TRUE);
		m_ledK.SetEnabled (TRUE);
		m_ledI.SetEnabled (TRUE);
		m_editDer.EnableWindow (TRUE);
		m_editProp.EnableWindow (TRUE);
		m_editInteg.EnableWindow (TRUE);
		m_lblFFW.SetText ("FFW");
		m_lblFFWA.SetText ("FFWA");
	}
	else   {
		m_ledD.SetEnabled (FALSE);
		m_ledK.SetEnabled (FALSE);
		m_ledI.SetEnabled (FALSE);
		m_editDer.EnableWindow (FALSE);
		m_editProp.EnableWindow (FALSE);
		m_editInteg.EnableWindow (FALSE);
		m_lblFFW.SetText ("KFFW");
		m_lblFFWA.SetText ("KFFWA");
	}
}

/*
** OnDrawCaption :
*/
void CXTPAxSettingsRecordItemName::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CXTPReportRecord* pRecord = pDrawArgs->pRow->GetRecord ();
	CDC* pDC = pDrawArgs->pDC;
	COLORREF crf, oldcrf;

	CBrush* pBrush = new CBrush ();
	CFont* pFont = ((CMainFrame *)AfxGetMainWnd ())->CreateFont ("Segoe UI", 20, FW_BOLD);
	if (pDrawArgs->pRow->IsSelected ())   {
		pBrush->CreateSolidBrush (RGB (255, 0, 0));
		crf = RGB (255, 255, 255);
	}
	else	{
		pBrush->CreateSolidBrush (RGB (96, 96, 96));
		crf = RGB (255, 255, 255);
	}
	CBrush* pOldBrush = pDC->SelectObject (pBrush);
	CFont* pOldFont = pDC->SelectObject (pFont);
	FillRect (pDC->GetSafeHdc (), pDrawArgs->rcItem, (HBRUSH) pBrush);
	oldcrf = pDC->SetTextColor (crf);
 	::DrawText (pDC->GetSafeHdc (), GetValue (), -1, &pDrawArgs->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	pDC->SelectObject (pOldBrush);
	pDC->SetTextColor (oldcrf);
	pDC->SelectObject (pOldFont);
	delete pBrush;
	delete pFont;
}

/*
** OnDrawCaption :
*/
void CXTPAxSettingsRecordItemPositions::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CXTPReportRecord* pRecord = pDrawArgs->pRow->GetRecord ();
	CAdamoMDAxSettings* pAxSettings = (CAdamoMDAxSettings *) pDrawArgs->pControl->GetParent ();
	if (pAxSettings->m_bTimerOk == true)   {
		CString strValue;
		double fp;
		COLORREF crf, oldcrf;
		int nRow = (int) pDrawArgs->pRow->GetIndex ();
		switch (nRow)   {
			case 0 :
				fp = pAxSettings->m_fpBackQuotaTeorica;
				break;
			case 1 :
				fp = pAxSettings->m_fpBackQuotaReale;
				break;
			case 2 :
				fp = pAxSettings->m_fpBackEA;
				break;
		}
		crf = RGB (0, 0, 0);
		oldcrf = pDrawArgs->pDC->SetTextColor (crf);
		strValue.Format ("%.3f", fp);
		HGDIOBJ hObj = pDrawArgs->pDC->SelectObject (pAxSettings->m_pDigitFont);
		::DrawText (pDrawArgs->pDC->GetSafeHdc (), strValue, -1, &pDrawArgs->rcItem, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		pDrawArgs->pDC->SelectObject (hObj);
		pDrawArgs->pDC->SetTextColor (oldcrf);
	}
}

/*
** OnDrawCaption :
*/
void CXTPAxSettingsRecordItemStatus::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CAdamoMDAxSettings* pAxSettings = (CAdamoMDAxSettings *) pDrawArgs->pControl->GetParent ();
	if (pAxSettings->m_bTimerOk == true)   {
		CDC* pDC = pDrawArgs->pDC;
		COLORREF crf, oldcrf;
		CBrush* pBrush = new CBrush ();
		pBrush->CreateSolidBrush (RGB (96, 96, 96));
		crf = RGB (255, 255, 255);
		CBrush* pOldBrush = pDC->SelectObject (pBrush);
		oldcrf = pDC->SetTextColor (crf);
		FillRect (pDC->GetSafeHdc (), pDrawArgs->rcItem, (HBRUSH) pBrush);
		if (pAxSettings->m_bTimerOk == true)   {
			CXTPReportRecord* pRecord = pDrawArgs->pRow->GetRecord ();
			int nAxesIndex = (int) pRecord->GetItem (0)->GetItemData ();

			/* calcoliamo la Y corretta */
			int yBriek = pDrawArgs->rcItem.Height () / 5;
			int yStatus = yBriek;
			int yMovementType = yBriek * 3;
			CRect rcStatusLabel (pDrawArgs->rcItem.left + 2,
				pDrawArgs->rcItem.top + yStatus, 
				pDrawArgs->rcItem.left + 50, 
				pDrawArgs->rcItem.top + yStatus + yBriek);
			CRect rcStatus (pDrawArgs->rcItem.left + 51, 
				pDrawArgs->rcItem.top + yStatus, 
				pDrawArgs->rcItem.right, 
				pDrawArgs->rcItem.top + yStatus + yBriek);
			CRect rcMovementTypeLabel (pDrawArgs->rcItem.left + 2, 
				pDrawArgs->rcItem.top + yMovementType, 
				pDrawArgs->rcItem.left + 50, 
				pDrawArgs->rcItem.top + yMovementType + yBriek);
			CRect rcMovementType (pDrawArgs->rcItem.left + 51, 
				pDrawArgs->rcItem.top + yMovementType, 
				pDrawArgs->rcItem.right, 
				pDrawArgs->rcItem.top + yMovementType + yBriek);
			::DrawText (pDrawArgs->pDC->GetSafeHdc (), LOADSTRING (IDS_ADAMO_AXIS_STATUS_21), -1, &rcStatusLabel, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			::DrawText (pDrawArgs->pDC->GetSafeHdc (), *pAxSettings->m_aszConfrontoAssi[pAxSettings->m_nBackStStato-1], -1, &rcStatus, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			::DrawText (pDrawArgs->pDC->GetSafeHdc (), LOADSTRING (IDS_ADAMO_AXIS_STATUS_22), -1, &rcMovementTypeLabel, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
			::DrawText (pDrawArgs->pDC->GetSafeHdc (), *pAxSettings->m_aszTipoMovimento[pAxSettings->m_nBackStTipoMovimento], -1, &rcMovementType, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		}
		pDC->SetTextColor (oldcrf);
		pDC->SelectObject (pOldBrush);
		delete (pBrush);
	}
}

/*
** OnDrawCaption :
*/
void CXTPAxSettingsRecordItemDynamic::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CXTPReportRecord* pRecord = pDrawArgs->pRow->GetRecord ();
	CDC* pDC = pDrawArgs->pDC;
	CBrush* pBrush = new CBrush ();
	pBrush->CreateSolidBrush (RGB (96, 96, 96));
	CBrush* pOldBrush = pDC->SelectObject (pBrush);
	FillRect (pDC->GetSafeHdc (), pDrawArgs->rcItem, (HBRUSH) pBrush);
	pDC->SelectObject (pOldBrush);
	delete (pBrush);
}

/* paint manager destructor */
CXTPAxSettinsPaintManager::CXTPAxSettinsPaintManager()
{
	m_bHideSelection = true;
	m_clrHighlight.SetCustomValue (m_clrControlBack.GetStandardColor ());
	SetGridStyle (FALSE, xtpReportLineStyleNone);
}

CXTPAxSettinsPaintManager::~CXTPAxSettinsPaintManager()
{
}

/*
** GetRowHeight :
*/
int CXTPAxSettinsPaintManager::GetRowHeight(CDC* pDC, CXTPReportRow* pRow)
{
	return XTP_DPI_Y(AXISROWHEIGHT);
}

/*
** DrawFocusedRow :
*/
void CXTPAxSettinsPaintManager::DrawFocusedRow (CDC* pDC, CRect rcRow)
{
}
