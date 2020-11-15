#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynRTAx.h"
#ifdef ADAMO_KOS
#define IDS_RTSYNAX_0					291
#define IDS_RTSYNAX_1					292
#define IDS_RTSYNAX_2					293
#define IDS_RTSYNAX_3					294
#define IDS_RTSYNAX_4					295
#define IDS_RTSYNAX_5					296
#define IDS_RTSYNAX_6					297
#define IDS_RTSYNAX_7					298
#define IDS_RTSYNAX_8					299
#define IDS_RTSYNAX_9					300
#define IDS_RTSYNAX_10					301
extern CString LoadString (UINT nId);
#endif
#define MAXRISOL_POS   140737488355328.0
#define MAXRISOL_NEG  -140737488355328.0

IMPLEMENT_DYNAMIC(CAdamoSynRTAx, CDialog)

void CAdamoSynRTAx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoMDCommandBar)
    DDX_Control(pDX, IDC_RS_AX_DATA, m_stData);
    DDX_Control(pDX, IDC_BUTMINUS, m_butMeno);
    DDX_Control(pDX, IDC_BUTPLUS, m_butPiu);
    DDX_Control(pDX, IDC_BUTSTOP, m_butStop);
    DDX_Control(pDX, IDC_BUTOPENLOOP, m_butOpenLoop);
    DDX_Control(pDX, IDC_BUTCLOSELOOP, m_butCloseLoop);
	DDX_Control(pDX, IDC_RS_AX_RADIOJOG, m_butRadioJog);
	DDX_Control(pDX, IDC_RS_AX_RADIOSTEP, m_butRadioStep);
	DDX_Control(pDX, IDC_RS_AX_RADIOABS, m_butRadioAbs);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CAdamoSynRTAx, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RS_AX_RADIOABS, OnRadioAbs)
	ON_BN_CLICKED(IDC_RS_AX_RADIOJOG, OnRadioJog)
	ON_BN_CLICKED(IDC_RS_AX_RADIOSTEP, OnRadioStep)
	ON_WM_LBUTTONUP()
    ON_MESSAGE (WM_AXBUTTONDOWN, OnRSAxButtonDown)
    ON_MESSAGE (WM_AXBUTTONUP, OnRSAxButtonUp)
END_MESSAGE_MAP()

/* construction / destruction */
CAdamoSynRTAx::CAdamoSynRTAx () : m_pParent (NULL), m_pFont (NULL), m_bmp (NULL), m_bkg (NULL), m_bInitialized (false),
	m_fpQuotaTeorica (0.0), m_fpErroreAnello (0.0), m_fpCoppia (0.0), m_fpVelocita (0.0), m_fpAccelerazione (0.0), m_bLetto (false)
{
	SetDeviceType (enAsse);
}

CAdamoSynRTAx::~CAdamoSynRTAx ()
{
	_delete (m_pFont);
	_delete (m_bmp);
	_delete (m_bkg);
}

/*
** Create :
*/
BOOL CAdamoSynRTAx::Create ()
{
	BOOL b = __super::Create (IDD_DLG_RS_AX, m_pParent);
	CDialog::MoveWindow (GetObjStyle ().GetRect (), true);
	if (b)
		CreateFont ();
	return b;
}

/*
** Destroy :
*/
BOOL CAdamoSynRTAx::Destroy ()
{
	return FALSE;
}

/*
** OnSave :
*/
int CAdamoSynRTAx::OnSave ()
{
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynRTAx::OnLoad ()
{
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynRTAx::OnDownload ()
{
	return 0;
}

/*
** CreateFont :
*/
void CAdamoSynRTAx::CreateFont ()
{
	m_pFont = GETSYNCONT ()->CreateFont (m_objStyle.GetRSFont().strFaceName, m_objStyle.GetRSFont().nSize, m_objStyle.GetRSFont().bBold);
	if (m_pFont)
		SetFont (m_pFont);
}

/*
** OnCtlColor :
*/
HBRUSH CAdamoSynRTAx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Return a different brush if the default is not desired
	if (nCtlColor==CTLCOLOR_DLG)
        return (HBRUSH) m_bkg->GetSafeHandle ();
	return hbr;
}

/*
** OnInitDialog :
*/
BOOL CAdamoSynRTAx::OnInitDialog()
{
	CString str;
	__super::OnInitDialog();

	// TODO:  Add extra initialization here
	if (!m_bInitialized)   {
		InitButtons ();
		m_bmp = new CBitmap;
		m_bkg = new CBrush;
		m_bmp->LoadBitmap (IDB_RS_DEVICE_AX_BCK);
		m_bkg->CreatePatternBrush (m_bmp);
		m_editAbs.SubclassEdit(IDC_RS_AX_EDITABS, this, PES_NUMBERS|PES_FLOATS);
		m_editVel.SubclassEdit(IDC_RS_AX_EDITSPEED, this, PES_NUMBERS|PES_FLOATS);
		m_stData.SetRTAx (this);
		if (GetID () != -1)
			str.Format ("%.2f", ((CAdamoAxis* ) GetDevice ())->GetSpeedMan ());
		m_editVel.SetWindowText (str);
		m_bInitialized = true;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/*
** InitButtons :
*/
void CAdamoSynRTAx::InitButtons ()
{
	m_butPiu.SetSkin(IDB_RS_AX1_UP,IDB_RS_AX1_DOWN,IDB_RS_AX1_HOVER,IDB_RS_AX1_UP,0,IDB_RS_AX1_MASK,0,0,0);
	m_butPiu.SetToolTipText(GETSYNCONT()->GetString (IDS_SYNCONT_4));
	m_butMeno.SetSkin(IDB_RS_AX2_UP,IDB_RS_AX2_DOWN,IDB_RS_AX2_HOVER,IDB_RS_AX2_UP,0,IDB_RS_AX2_MASK,0,0,0);
	m_butMeno.SetToolTipText(GETSYNCONT()->GetString (IDS_SYNCONT_3));
	m_butStop.SetSkin(IDB_RS_AX3_UP,IDB_RS_AX3_DOWN,IDB_RS_AX3_HOVER,IDB_RS_AX3_UP,0,IDB_RS_AX3_MASK,0,0,0);
	m_butStop.SetToolTipText(GETSYNCONT()->GetString (IDS_SYNCONT_5));
	m_butOpenLoop.SetSkin(IDB_RS_AX4_UP,IDB_RS_AX4_DOWN,IDB_RS_AX4_HOVER,IDB_RS_AX4_UP,0,IDB_RS_AX4_MASK,0,0,0);
	m_butOpenLoop.SetToolTipText(GETSYNCONT()->GetString (IDS_SYNCONT_6));
	m_butCloseLoop.SetSkin(IDB_RS_AX5_UP,IDB_RS_AX5_DOWN,IDB_RS_AX5_HOVER,IDB_RS_AX5_UP,0,IDB_RS_AX5_MASK,0,0,0);
	m_butCloseLoop.SetToolTipText(GETSYNCONT()->GetString (IDS_SYNCONT_7));
	m_butRadioJog.SetSkin(IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_SELECTED,IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_MASK,0,0,0);
	m_butRadioAbs.SetSkin(IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_SELECTED,IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_MASK,0,0,0);
	m_butRadioStep.SetSkin(IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_SELECTED,IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_NORMAL,IDB_RS_RADIO_MASK,0,0,0);
	m_butRadioJog.SetToolTipText(GETSYNCONT()->GetString (IDS_SYNCONT_8));
	m_butRadioStep.SetToolTipText(GETSYNCONT()->GetString (IDS_SYNCONT_9));
	m_butRadioAbs.SetToolTipText(GETSYNCONT()->GetString (IDS_SYNCONT_10));
	m_butMeno.SetParent (this);
	m_butStop.SetParent (this);
	m_butPiu.SetParent (this);
	m_butOpenLoop.SetParent (this);
	m_butCloseLoop.SetParent (this);
    m_butMeno.SetCtrlOn ();
    m_butPiu.SetCtrlOn ();
    m_butOpenLoop.SetCtrlOn ();
    m_butCloseLoop.SetCtrlOn ();
    CheckRadioButton (IDC_RS_AX_RADIOJOG, IDC_RS_AX_RADIOABS, IDC_RS_AX_RADIOJOG);
}

/*
** OnRadioAbs :
*/
void CAdamoSynRTAx::OnRadioAbs() 
{
    CheckRadioButton (IDC_RS_AX_RADIOJOG, IDC_RS_AX_RADIOABS, IDC_RS_AX_RADIOABS);
}

/*
** OnRadioJog :
*/
void CAdamoSynRTAx::OnRadioJog() 
{
    CheckRadioButton (IDC_RS_AX_RADIOJOG, IDC_RS_AX_RADIOABS, IDC_RS_AX_RADIOJOG);
}

/*
** OnRadioStep :
*/
void CAdamoSynRTAx::OnRadioStep() 
{
    CheckRadioButton (IDC_RS_AX_RADIOJOG, IDC_RS_AX_RADIOSTEP, IDC_RS_AX_RADIOSTEP);
}

/*
** OnTimer :
*/
int CAdamoSynRTAx::OnTimer ()							   
{
    CDeviceData dd;
    struct stDatiAsse da;
	CAdamoMachine *pMachine = GetMachine ();
	int nID = GetID ();

	if (pMachine && nID != -1)   {
		dd.m_eDev=enAsse;
		dd.m_nAddress=nID;
		m_fpQuotaTeoricaVecchia = m_fpQuotaTeorica;
		m_fpCoppiaVecchia = m_fpCoppia;
		m_fpVelocitaVecchia = m_fpVelocita;
		m_fpAccelerazioneVecchia = m_fpAccelerazione;
		if (pMachine->GetAxes (1, &dd, &da)==S_OK)   {
			m_fpQuotaTeorica = da.fpQuotaTeorica;
			m_fpCoppia = da.fpTorqueReale;
			m_fpVelocita = da.fpVelAttuale;
			m_fpAccelerazione = da.fpAccInstInt;
			m_fpErroreAnello = da.fpErroreAnello;
			m_bLetto = true;
			m_stData.Invalidate ();
			m_butRadioJog.Invalidate ();
			m_butRadioStep.Invalidate ();
			m_butRadioAbs.Invalidate ();
		}
        else   {
			m_fpQuotaTeorica = 0.0;
			m_fpCoppia = 0.0;
			m_fpVelocita = 0.0;
			m_fpAccelerazione = 0.0;
			m_fpErroreAnello = 0.0;
			m_bLetto = true;
			m_stData.Invalidate ();
			m_butRadioJog.Invalidate ();
			m_butRadioStep.Invalidate ();
			m_butRadioAbs.Invalidate ();
        }
	}
	return 0;
}

/*
** OnLButtonUp :
*/ 
void CAdamoSynRTAx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	AfxGetMainWnd ()->SetFocus ();
	__super::OnLButtonUp(nFlags, point);
}

/*
** OnRSAxButtonDown :
*/
LONG CAdamoSynRTAx::OnRSAxButtonDown(UINT wParam, LONG lParam)
{
	if (GetID () != -1)   {
		int nId = ((CWnd*)lParam)->GetDlgCtrlID();
		int nOp = GetCheckedRadioButton (IDC_RS_AX_RADIOJOG, IDC_RS_AX_RADIOABS);
		switch (nId)   {
			case IDC_BUTMINUS :
				ManageMinusButton (nOp);
				break;
			case IDC_BUTSTOP :
				ManageStopButton ();
				break;
			case IDC_BUTPLUS :
				ManagePlusButton (nOp);
				break;
			case IDC_BUTOPENLOOP :
				ManageOpenLoop ();
				break;
			case IDC_BUTCLOSELOOP :
				ManageCloseLoop ();
				break;
		}
	}
    return 0;
}

/*
** OnRSAxButtonUp :
*/
LONG CAdamoSynRTAx::OnRSAxButtonUp(UINT wParam, LONG lParam)
{
	if (GetID () != -1)   {
		int nOp = GetCheckedRadioButton (IDC_RS_AX_RADIOJOG, IDC_RS_AX_RADIOABS);
		if (nOp==IDC_RS_AX_RADIOJOG)
			ManageStopButton ();
		AfxGetMainWnd ()->SetFocus ();
	}
    return 0;
}

/*
** ManageMinusButton :
*/
void CAdamoSynRTAx::ManageMinusButton (int nOp)
{
    bool bLimitPos, bLimitNeg;
    double fpSpeed;
    double fpLimitePositivo, fpLimiteNegativo;
    double fpQuotaTarget;
    double fpQuotaDaRaggiungere;
    double fpRisoluzione;
    char szBuffer[16];

    if (GetSpeed (&fpSpeed, TRUE))
        return;
    if (GetLimits (&bLimitPos, &bLimitNeg, &fpLimitePositivo, &fpLimiteNegativo))
        return;
    if (GetTargetPosition (&fpQuotaTarget))
        return;
    fpRisoluzione=((CAdamoAxis *)GetDevice ())->GetResolution ();
    switch (nOp)   {
        case IDC_RS_AX_RADIOJOG :
            if (bLimitNeg)
                if (fpQuotaTarget<fpLimiteNegativo)
                    return;
                else
                    fpQuotaDaRaggiungere=fpLimiteNegativo;
            else
                fpQuotaDaRaggiungere=ceil(MAXRISOL_NEG / fpRisoluzione);
			GetMachine ()->GotoPosition (GetID (), fpQuotaDaRaggiungere, fpSpeed, eJogJog);
            break;
        case IDC_RS_AX_RADIOSTEP :
            if (GetStep(&fpQuotaDaRaggiungere))
                return;
            fpQuotaDaRaggiungere=-fpQuotaDaRaggiungere;
            if (bLimitNeg && fpQuotaTarget+fpQuotaDaRaggiungere<fpLimiteNegativo)
                return;
			GetMachine ()->GotoPositionInc (GetID (), fpQuotaDaRaggiungere, fpSpeed, eStepJog);
            break;
        case IDC_RS_AX_RADIOABS :
            GetDlgItemText (IDC_RS_AX_EDITABS, szBuffer, 16);
            fpQuotaDaRaggiungere = atof(szBuffer);
            if ((fpQuotaDaRaggiungere < fpLimiteNegativo)&&bLimitNeg)   {
                AfxMessageBox (GETSYNCONT()->GetString (IDS_SYNCONT_0));
                GetDlgItem(IDC_RS_AX_EDITABS)->SetFocus();
                return;
            }
            if ((fpQuotaDaRaggiungere > fpLimitePositivo)&&bLimitPos)   {
                AfxMessageBox (GETSYNCONT()->GetString (IDS_SYNCONT_1));
                GetDlgItem(IDC_RS_AX_EDITABS)->SetFocus();
                return;
            }
			GetMachine ()->GotoPosition (GetID (), fpQuotaDaRaggiungere, fpSpeed, eAbsJog);
            break;
    }
}

/*
** ManageStopButton :
*/
void CAdamoSynRTAx::ManageStopButton ()
{
	GetMachine ()->AbortMovement (GetID ());
}

/*
** ManagePlusButton :
*/
void CAdamoSynRTAx::ManagePlusButton (int nOp)
{
    bool bLimitPos, bLimitNeg;
    double fpSpeed;
    double fpLimitePositivo, fpLimiteNegativo;
    double fpQuotaTarget;
    double fpQuotaDaRaggiungere;
    double fpRisoluzione;
    char szBuffer[16];

    if (GetSpeed (&fpSpeed, TRUE))
        return;
    if (GetLimits (&bLimitPos, &bLimitNeg, &fpLimitePositivo, &fpLimiteNegativo))
        return;
    if (GetTargetPosition (&fpQuotaTarget))
        return;
    fpRisoluzione=((CAdamoAxis *)GetDevice ())->GetResolution ();
    switch (nOp)   {
        case IDC_RS_AX_RADIOJOG :
            if (bLimitPos)
                if (fpQuotaTarget>fpLimitePositivo)
                    return;
                else
                    fpQuotaDaRaggiungere=fpLimitePositivo;
            else
                fpQuotaDaRaggiungere=floor(MAXRISOL_POS / fpRisoluzione);
			GetMachine ()->GotoPosition (GetID (), fpQuotaDaRaggiungere, fpSpeed, eJogJog);
            break;
        case IDC_RS_AX_RADIOSTEP :
            if (GetStep(&fpQuotaDaRaggiungere))
                return;
            if (bLimitPos && fpQuotaTarget+fpQuotaDaRaggiungere>fpLimitePositivo)
                return;
			GetMachine ()->GotoPositionInc (GetID (), fpQuotaDaRaggiungere, fpSpeed, eStepJog);
            break;
        case IDC_RS_AX_RADIOABS :
            GetDlgItemText (IDC_RS_AX_EDITABS, szBuffer, 16);
            fpQuotaDaRaggiungere = atof(szBuffer);
            if ((fpQuotaDaRaggiungere < fpLimiteNegativo)&&bLimitNeg)   {
                AfxMessageBox (GETSYNCONT()->GetString (IDS_SYNCONT_0));
                GetDlgItem(IDC_RS_AX_EDITABS)->SetFocus();
                return;
            }
            if ((fpQuotaDaRaggiungere > fpLimitePositivo)&&bLimitPos)   {
                AfxMessageBox (GETSYNCONT()->GetString (IDS_SYNCONT_1));
                GetDlgItem(IDC_RS_AX_EDITABS)->SetFocus();
                return;
            }
			GetMachine ()->GotoPosition (GetID (), fpQuotaDaRaggiungere, fpSpeed, eAbsJog);
            break;
    }
}

/*
** ManageOpenLoop :
*/
void CAdamoSynRTAx::ManageOpenLoop ()
{
	GetMachine ()->OpenPositionLoop (GetID ());
}

/*
** ManageCloseLoop :
*/
void CAdamoSynRTAx::ManageCloseLoop ()
{
	GetMachine ()->ClosePositionLoop (GetID ());
}

/*
** GetSpeed :
*/
int CAdamoSynRTAx::GetSpeed (double* fpVel, int nShow)
{
    int nBack = 0;
    float fpVelMax;
    char szBuffer[128], *p;

    if (((CAdamoAxis *)GetDevice ())->GetAxisType()!=enAsseConteggio)   {
        GetDlgItemText(IDC_RS_AX_EDITSPEED,szBuffer,sizeof(szBuffer)-1);
        *fpVel = (float)strtod(szBuffer, &p);
        if ((*p!=NULL) || (*fpVel <= 0.0))   {
            if (nShow)
                AfxMessageBox (GETSYNCONT()->GetString (IDS_SYNCONT_2));
            if (*p==NULL)   {
                if (*fpVel < 0.0)   {
                    if (nShow)   {
                        sprintf (szBuffer, "%.2f", 0.1);
                        SetDlgItemText(IDC_RS_AX_EDITSPEED,szBuffer);
                    }
                    *fpVel=0.0;
                }
            }
            else
                *fpVel=0.0;
            if (nShow)
                GetDlgItem(IDC_RS_AX_EDITSPEED)->SetFocus();
            nBack = -1;
        }
        else   {
            fpVelMax=(float)((CAdamoAxis *)GetDevice ())->GetSpeedMan ();
            if (*fpVel > fpVelMax)
                *fpVel=fpVelMax;
        }
    }
    return nBack;
}

/*
** GetLimits :
*/
int CAdamoSynRTAx::GetLimits (bool* bLimitPos, bool* bLimitNeg, 
    double* fpLimitePositivo, double* fpLimiteNegativo)
{
    CDeviceData dd;
    CAdamoParametri ap;
    CAdamoStatoAsse asa;

    int nB=-1;
    dd.m_eDev=enAsse;
    dd.m_nAddress=GetID ();
    if (GetMachine ()->GetAdamoAxes (&dd, &ap, &asa)==S_OK)   {
        *bLimitPos=ap.nEnableLimitPos!=0;
        *bLimitNeg=ap.nEnableLimitNeg!=0;
        if (*bLimitPos)
            *fpLimitePositivo=ap.qLimitePos;
        else
            *fpLimitePositivo=0.0;
        if (*bLimitNeg)
            *fpLimiteNegativo=ap.qLimiteNeg;
        else
            *fpLimiteNegativo=0.0;
        nB=0;        
    }
    else   {
        *bLimitPos = *bLimitNeg = false;
        *fpLimitePositivo = *fpLimiteNegativo = 0.0;
    }
    return nB;
}

/*
** GetTargetPosition :
*/
int CAdamoSynRTAx::GetTargetPosition (double* fpTargetPosition)
{
    CDeviceData dd;
    struct stDatiAsse da;
    int nB=-1;

    dd.m_eDev=enAsse;
    dd.m_nAddress=GetID ();
    if (GetMachine ()->GetAxes (1, &dd, &da)==S_OK)   {
        *fpTargetPosition=da.fpQuotaTeorica;
        nB=0;        
    }
    else
        *fpTargetPosition = 0.0;
    return nB;
}

/*
** GetStep :
*/
int CAdamoSynRTAx::GetStep (double* fpStep)
{
    int nBack=0;
    char szBuffer[128], *p;

    GetDlgItemText (IDC_RS_AX_EDITABS, szBuffer, sizeof(szBuffer)-1);
    *fpStep = strtod(szBuffer, &p);
    return nBack;
}
