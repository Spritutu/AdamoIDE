// AdamoMDCommandBar.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "ide2.h"
#include "AdamoMDCommandBar.h"
#include "mainframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAXRISOL_POS   140737488355328.0
#define MAXRISOL_NEG  -140737488355328.0
#define MAXSTEPVALUES 5

char* VettStr[] = {
    "10.0",
    "1.0",
    "0.1",
    "0.01",
    "0.001"
};
  
/////////////////////////////////////////////////////////////////////////////
// CAdamoMDCommandBar dialog


CAdamoMDCommandBar::CAdamoMDCommandBar(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoMDCommandBar::IDD, pParent), m_pAx (NULL), m_pMachine (NULL),
      m_pSettings (NULL), m_nMode (0), m_ePswLevel (ePswUser), m_stFont (NULL)
{
	//{{AFX_DATA_INIT(CAdamoMDCommandBar)
	//}}AFX_DATA_INIT
}


void CAdamoMDCommandBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoMDCommandBar)
    DDX_Control(pDX, IDC_BUTMINUS, m_butMeno);
    DDX_Control(pDX, IDC_BUTPLUS, m_butPiu);
    DDX_Control(pDX, IDC_BUTSTOP, m_butStop);
    DDX_Control(pDX, IDC_BUTOPENLOOP, m_butOpenLoop);
    DDX_Control(pDX, IDC_BUTCLOSELOOP, m_butCloseLoop);
	DDX_Control(pDX, IDC_IKNOBSPEED, m_knobSpeed);
	DDX_Control(pDX, IDC_RADIOJOG, m_butRadioJog);
	DDX_Control(pDX, IDC_RADIOSTEP, m_butRadioStep);
	DDX_Control(pDX, IDC_RADIOABS, m_butRadioAbs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoMDCommandBar, CDialog)
	//{{AFX_MSG_MAP(CAdamoMDCommandBar)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_RADIOABS, OnRadioAbs)
	ON_BN_CLICKED(IDC_RADIOJOG, OnRadioJog)
	ON_BN_CLICKED(IDC_RADIOSTEP, OnRadioStep)
	ON_WM_DESTROY()
    ON_MESSAGE (WM_AXBUTTONDOWN, OnMDButtonDown)
    ON_MESSAGE (WM_AXBUTTONUP, OnMDButtonUp)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDCommandBar message handlers

BOOL CAdamoMDCommandBar::OnInitDialog() 
{
    int i;
	CDialog::OnInitDialog();
	
    /* assegnamo il livello corrente di password */
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    InitButtons ();
	m_butMeno.SetParent (this);
	m_butStop.SetParent (this);
	m_butPiu.SetParent (this);
	m_butOpenLoop.SetParent (this);
	m_butCloseLoop.SetParent (this);
    m_butMeno.SetCtrlOn ();
    m_butPiu.SetCtrlOn ();
    m_butOpenLoop.SetCtrlOn ();
    m_butCloseLoop.SetCtrlOn ();
    m_editAbs.SubclassEdit(IDC_EDITABS, this, PES_NUMBERS|PES_FLOATS);
    m_editVel.SubclassEdit(IDC_EDITSPEED, this, PES_NUMBERS|PES_FLOATS);
    for (i=0; i<MAXSTEPVALUES; i++)
        ((CComboBox*)GetDlgItem (IDC_COMBOVALUES))->AddString (VettStr[i]);
    /* carichiamo il bitmap di sfondo */
    m_bkg.CreateSolidBrush (RGB (128, 128, 128));
	m_stFont = ((CMainFrame *)AfxGetMainWnd ())->CreateFont ("Segoe UI", 18, FW_BOLD);
	GetDlgItem (IDC_ST_JOG)->SetFont (m_stFont);
	GetDlgItem (IDC_ST_STEP)->SetFont (m_stFont);
	GetDlgItem (IDC_ST_ABS)->SetFont (m_stFont);
	GetDlgItem (IDC_ST_LOOP_APERTO)->SetFont (m_stFont);
	GetDlgItem (IDC_ST_LOOP_CHIUSO)->SetFont (m_stFont);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}                                                           

LONG CAdamoMDCommandBar::OnMDButtonDown(UINT wParam, LONG lParam)
{
    int nId = ((CWnd*)lParam)->GetDlgCtrlID();
    int nOp = GetCheckedRadioButton (IDC_RADIOJOG, IDC_RADIOABS);
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
    return 0;
}

LONG CAdamoMDCommandBar::OnMDButtonUp(UINT wParam, LONG lParam)
{
    int nOp = GetCheckedRadioButton (IDC_RADIOJOG, IDC_RADIOABS);
    if (nOp==IDC_RADIOJOG)
        ManageStopButton ();
    AfxGetMainWnd ()->SetFocus ();
    return 0;
}

void CAdamoMDCommandBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	AfxGetMainWnd ()->SetFocus ();
	CDialog::OnLButtonUp(nFlags, point);
}

void CAdamoMDCommandBar::ManageMinusButton (int nOp)
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
    fpRisoluzione=m_pAx->GetResolution ();
    switch (nOp)   {
        case IDC_RADIOJOG :
            if (bLimitNeg)
                if (fpQuotaTarget<fpLimiteNegativo)
                    return;
                else
                    fpQuotaDaRaggiungere=fpLimiteNegativo;
            else
                fpQuotaDaRaggiungere=ceil(MAXRISOL_NEG / fpRisoluzione);
			if (m_nMode == 0 || m_nMode == 1)
				m_pMachine->GotoPosition (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eJogJog);
			else
				m_pMachine->GotoLinear (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eJogJog);
            break;
        case IDC_RADIOSTEP :
            if (GetStep(&fpQuotaDaRaggiungere))
                return;
            fpQuotaDaRaggiungere=-fpQuotaDaRaggiungere;
            if (bLimitNeg && fpQuotaTarget+fpQuotaDaRaggiungere<fpLimiteNegativo)
                return;
			if (m_nMode == 0 || m_nMode == 1)
				m_pMachine->GotoPositionInc (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eStepJog);
			else
				m_pMachine->GotoLinearInc (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eStepJog);
            break;
        case IDC_RADIOABS :
            GetDlgItemText (IDC_EDITABS, szBuffer, 16);
            fpQuotaDaRaggiungere = atof(szBuffer);
            if ((fpQuotaDaRaggiungere < fpLimiteNegativo)&&bLimitNeg)   {
                AfxMessageBox (LOADSTRING (IDS_ADAMOMDCOMMANDBAR_1));
                GetDlgItem(IDC_EDITABS)->SetFocus();
                return;
            }
            if ((fpQuotaDaRaggiungere > fpLimitePositivo)&&bLimitPos)   {
                AfxMessageBox (LOADSTRING (IDS_ADAMOMDCOMMANDBAR_2));
                GetDlgItem(IDC_EDITABS)->SetFocus();
                return;
            }
			if (m_nMode == 0 || m_nMode == 1)
				m_pMachine->GotoPosition (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eAbsJog);
			else
				m_pMachine->GotoLinear (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eAbsJog);
            break;
    }
}

void CAdamoMDCommandBar::ManageStopButton ()
{
	m_pMachine->AbortMovement (m_pAx->GetDeviceID());
}

void CAdamoMDCommandBar::ManagePlusButton (int nOp)
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
    fpRisoluzione=m_pAx->GetResolution ();
    switch (nOp)   {
        case IDC_RADIOJOG :
            if (bLimitPos)
                if (fpQuotaTarget>fpLimitePositivo)
                    return;
                else
                    fpQuotaDaRaggiungere=fpLimitePositivo;
            else
                fpQuotaDaRaggiungere=floor(MAXRISOL_POS / fpRisoluzione);
			if (m_nMode == 0 || m_nMode == 1)
				m_pMachine->GotoPosition (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eJogJog);
			else
				m_pMachine->GotoLinear (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eJogJog);
            break;
        case IDC_RADIOSTEP :
            if (GetStep(&fpQuotaDaRaggiungere))
                return;
            if (bLimitPos && fpQuotaTarget+fpQuotaDaRaggiungere>fpLimitePositivo)
                return;
			if (m_nMode == 0 || m_nMode == 1)
				m_pMachine->GotoPositionInc (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eStepJog);
			else
				m_pMachine->GotoLinearInc (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eStepJog);
            break;
        case IDC_RADIOABS :
            GetDlgItemText (IDC_EDITABS, szBuffer, 16);
            fpQuotaDaRaggiungere = atof(szBuffer);
            if ((fpQuotaDaRaggiungere < fpLimiteNegativo)&&bLimitNeg)   {
                AfxMessageBox (LOADSTRING (IDS_ADAMOMDCOMMANDBAR_1));
                GetDlgItem(IDC_EDITABS)->SetFocus();
                return;
            }
            if ((fpQuotaDaRaggiungere > fpLimitePositivo)&&bLimitPos)   {
                AfxMessageBox (LOADSTRING (IDS_ADAMOMDCOMMANDBAR_2));
                GetDlgItem(IDC_EDITABS)->SetFocus();
                return;
            }
			if (m_nMode == 0 || m_nMode == 1)
				m_pMachine->GotoPosition (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eAbsJog);
			else
				m_pMachine->GotoLinear (m_pAx->GetDeviceID(), fpQuotaDaRaggiungere, fpSpeed, eAbsJog);
            break;
    }
}

void CAdamoMDCommandBar::ManageOpenLoop ()
{
        m_pMachine->OpenPositionLoop (m_pAx->GetDeviceID());
}

void CAdamoMDCommandBar::ManageCloseLoop ()
{
        m_pMachine->ClosePositionLoop (m_pAx->GetDeviceID());
}

int CAdamoMDCommandBar::GetSpeed (double* fpVel, int nShow)
{
    int nBack = 0;
    float fpVelMax;
    char szBuffer[128], *p;

    if (m_pAx->GetAxisType()!=enAsseConteggio)   {
        GetDlgItemText(IDC_EDITSPEED,szBuffer,sizeof(szBuffer)-1);
        *fpVel = (float)strtod(szBuffer, &p);
        if ((*p!=NULL) || (*fpVel <= 0.0))   {
            if (nShow)
                AfxMessageBox (LOADSTRING (IDS_ADAMOMDCOMMANDBAR_3));
            if (*p==NULL)   {
                if (*fpVel < 0.0)   {
                    if (nShow)   {
                        sprintf (szBuffer, "%.2f", 0.1);
                        SetDlgItemText(IDC_EDITSPEED,szBuffer);
                    }
                    *fpVel=0.0;
                }
            }
            else
                *fpVel=0.0;
            if (nShow)
                GetDlgItem(IDC_EDITSPEED)->SetFocus();
            nBack = -1;
        }
        else   {
            switch (m_nMode)   {
                case 0 :
                    fpVelMax=(float)m_pAx->GetSpeedMan ();
                    break;
                case 1 :
                    fpVelMax=(float)m_pAx->GetSpeed ();
                    break;
                case 2 :
                    fpVelMax=(float)m_pAx->GetInterSpeed ();
                    break;
            }
            if (*fpVel > fpVelMax)
                *fpVel=fpVelMax;
        }
    }
    return nBack;
}

int CAdamoMDCommandBar::GetLimits (bool* bLimitPos, bool* bLimitNeg, 
    double* fpLimitePositivo, double* fpLimiteNegativo)
{
    CDeviceData dd;
    CAdamoParametri ap;
    CAdamoStatoAsse asa;

    int nB=-1;
    dd.m_eDev=enAsse;
    dd.m_nAddress=m_pAx->GetDeviceID();
    if (m_pMachine->GetAdamoAxes (&dd, &ap, &asa)==S_OK)   {
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

int CAdamoMDCommandBar::GetTargetPosition (double* fpTargetPosition)
{
    CDeviceData dd;
    struct stDatiAsse da;
    int nB=-1;

    dd.m_eDev=enAsse;
    dd.m_nAddress=m_pAx->GetDeviceID();
    if (m_pMachine->GetAxes (1, &dd, &da)==S_OK)   {
        *fpTargetPosition=da.fpQuotaTeorica;
        nB=0;        
    }
    return nB;
}

void CAdamoMDCommandBar::OnRadioAbs() 
{
    GetDlgItem(IDC_COMBOVALUES)->EnableWindow (FALSE);
    GetDlgItem(IDC_EDITABS)->EnableWindow (TRUE);
    GetDlgItem(IDC_EDITABS)->SetFocus ();
	((CEdit*)GetDlgItem (IDC_EDITABS))->SetSel (MAKELONG (0, -1));
    Invalidate ();
}

void CAdamoMDCommandBar::OnRadioJog() 
{
    GetDlgItem(IDC_COMBOVALUES)->EnableWindow (FALSE);
    GetDlgItem(IDC_EDITABS)->EnableWindow (FALSE);
    AfxGetMainWnd ()->SetFocus ();	
    Invalidate ();
}

void CAdamoMDCommandBar::OnRadioStep() 
{
    GetDlgItem(IDC_COMBOVALUES)->SetFocus ();
    GetDlgItem(IDC_COMBOVALUES)->EnableWindow (TRUE);
    GetDlgItem(IDC_EDITABS)->EnableWindow (FALSE);
	((CComboBox*)GetDlgItem (IDC_COMBOVALUES))->SetEditSel (0, -1);
	HWND pEdit = ::GetDlgItem(GetDlgItem (IDC_COMBOVALUES)->GetSafeHwnd (), 0x3e9 );
	::SetFocus (pEdit);
    Invalidate ();
}

int CAdamoMDCommandBar::GetStep (double* fpStep)
{
    int nBack=0;
    char szBuffer[128], *p;

    GetDlgItemText(IDC_COMBOVALUES,szBuffer,sizeof(szBuffer)-1);
    *fpStep = strtod(szBuffer, &p);
    return nBack;
}

void CAdamoMDCommandBar::OnOK ()
{
}

void CAdamoMDCommandBar::OnCancel ()
{
}

BOOL CAdamoMDCommandBar::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message==WM_KEYDOWN)   {
        short sh=GetKeyState (VK_RETURN);
        if (sh&0x8000)   {
            double fp;
            GetSpeed (&fp, TRUE);
            m_knobSpeed.SetPosition (fp);
            AfxGetMainWnd ()->SetFocus ();
            return TRUE;
        }
    }
	return CDialog::PreTranslateMessage(pMsg);
}

void CAdamoMDCommandBar::PushPlusButton ()
{
    m_butPiu.OnLButtonDown (1, CPoint (0, 0));
    m_butPiu.SetCapture ();
    m_butPiu.Invalidate ();
}

void CAdamoMDCommandBar::ReleasePlusButton ()
{
    m_butPiu.OnLButtonUp (0, CPoint (0, 0));
    ReleaseCapture ();
    m_butPiu.Invalidate ();
}

void CAdamoMDCommandBar::PushMinusButton ()
{
    m_butMeno.OnLButtonDown (1, CPoint (0, 0));
    m_butMeno.SetCapture ();
    m_butMeno.Invalidate ();
}

void CAdamoMDCommandBar::ReleaseMinusButton ()
{
    m_butMeno.OnLButtonUp (0, CPoint (0, 0));
    ReleaseCapture ();
    m_butMeno.Invalidate ();
}

void CAdamoMDCommandBar::PushStopButton ()
{
    m_butStop.OnLButtonDown (1, CPoint (0, 0));
    m_butStop.SetCapture ();
    m_butStop.Invalidate ();
}

void CAdamoMDCommandBar::ReleaseStopButton ()
{
    m_butStop.OnLButtonUp (0, CPoint (0, 0));
    ReleaseCapture ();
    m_butStop.Invalidate ();
}

void CAdamoMDCommandBar::SetVel ()
{
    GetDlgItem (IDC_EDITSPEED)->SetFocus ();
    ((CEdit*)GetDlgItem (IDC_EDITSPEED))->SetSel (MAKELONG (0, -1));
}

void CAdamoMDCommandBar::SetJogMode ()
{
    CheckRadioButton (IDC_RADIOJOG, IDC_RADIOABS, IDC_RADIOJOG);
    OnRadioJog ();
}

void CAdamoMDCommandBar::SetAbsMode ()
{
    CheckRadioButton (IDC_RADIOJOG, IDC_RADIOABS, IDC_RADIOABS);
    OnRadioAbs ();
}

void CAdamoMDCommandBar::SetStepMode ()
{
    CheckRadioButton (IDC_RADIOJOG, IDC_RADIOABS, IDC_RADIOSTEP);
    OnRadioStep ();
}

void CAdamoMDCommandBar::PushOpenLoopButton ()
{
    m_butOpenLoop.OnLButtonDown (1, CPoint (0, 0));
    m_butOpenLoop.SetCapture ();
    m_butOpenLoop.Invalidate ();
}

void CAdamoMDCommandBar::PushCloseLoopButton ()
{
    m_butCloseLoop.OnLButtonDown (1, CPoint (0, 0));
    m_butCloseLoop.SetCapture ();
    m_butCloseLoop.Invalidate ();
}

void CAdamoMDCommandBar::ReleaseOpenLoopButton ()
{
    m_butOpenLoop.OnLButtonUp (0, CPoint (0, 0));
    ReleaseCapture ();
    m_butOpenLoop.Invalidate ();
}

void CAdamoMDCommandBar::ReleaseCloseLoopButton ()
{
    m_butCloseLoop.OnLButtonUp (0, CPoint (0, 0));
    ReleaseCapture ();
    m_butCloseLoop.Invalidate ();
}

void CAdamoMDCommandBar::UpdateData (CAdamoAxis* pAx, int nMode)
{
    double fpVel;
    char szBuffer[256];

    if (m_pSettings)
        if (m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nCmdBar)   {
            sprintf (szBuffer, "%.2f", m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].fpVMan);
            GetDlgItem (IDC_EDITSPEED)->SetWindowText (szBuffer);
            if (m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nTypeMove<IDC_RADIOJOG || m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nTypeMove>IDC_RADIOABS)
                m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nTypeMove=IDC_RADIOJOG;
            CheckRadioButton (IDC_RADIOJOG, IDC_RADIOABS, m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nTypeMove);
            switch (m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nTypeMove)   {
            case IDC_RADIOJOG :
                OnRadioJog ();
                break;
            case IDC_RADIOSTEP :
                OnRadioStep ();
                break;
            case IDC_RADIOABS :
                OnRadioAbs ();
                break;
            }
            ((CComboBox*)GetDlgItem (IDC_COMBOVALUES))->SetCurSel(m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nStepComboIndex);
            sprintf (szBuffer, "%.2f", m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].fpQuotaAbs);
            GetDlgItem (IDC_EDITABS)->SetWindowText (szBuffer);
            fpVel=m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].fpVMan;
            switch (nMode)   {
                case 0 :
                    m_knobSpeed.SetPositionMax (m_pAx->GetSpeedMan ());
                    if (fpVel>m_pAx->GetSpeedMan ())
                        fpVel=m_pAx->GetSpeedMan ();
                    break;
                case 1 :
                    m_knobSpeed.SetPositionMax (m_pAx->GetSpeed ());
                    if (fpVel>m_pAx->GetSpeed ())
                        fpVel=m_pAx->GetSpeed ();
                    break;
                case 2 :
                    m_knobSpeed.SetPositionMax (m_pAx->GetInterSpeed ());
                    if (fpVel>m_pAx->GetInterSpeed ())
                        fpVel=m_pAx->GetInterSpeed ();
                    break;
            }
            m_knobSpeed.SetPosition (fpVel);
        }
        else   {
            sprintf (szBuffer, "%.2f", pAx->GetSpeedMan ());
            GetDlgItem (IDC_EDITSPEED)->SetWindowText (szBuffer);
            CheckRadioButton (IDC_RADIOJOG, IDC_RADIOABS, IDC_RADIOJOG);
            ((CComboBox*)GetDlgItem (IDC_COMBOVALUES))->SetCurSel(0);
            m_knobSpeed.SetPositionMax (m_pAx->GetSpeedMan ());
            m_knobSpeed.SetPosition (m_pAx->GetSpeedMan ());
            OnRadioJog ();
        }
}

void CAdamoMDCommandBar::SetAx (CAdamoAxis *pAx, int nMode)
{
    if (m_pAx)
        StoreData (m_pAx);
    m_pAx=pAx;
    UpdateData (m_pAx, nMode);
    GestionePassword ();
    m_nMode=nMode;
}

void CAdamoMDCommandBar::OnViewSettings (stMDSettings* MDS)
{
    if (!m_pSettings)   {
        m_pSettings=new stMDSettings;
        memset (m_pSettings, 0, sizeof (stMDSettings));
        if (m_pSettings)   {
            *m_pSettings=*MDS;
            UpdateData (m_pAx, 0);
        }
    }
}

void CAdamoMDCommandBar::GetViewSettings (stMDSettings* MDS)
{
    int n;
    if (m_pSettings)   {
        for (n=0; n<NMAXAXIS; n++)   {
            MDS->stAxSettings[n].nCmdBar=m_pSettings->stAxSettings[n].nCmdBar;
            MDS->stAxSettings[n].nAxSettings=m_pSettings->stAxSettings[n].nAxSettings;
            MDS->stAxSettings[n].nTypeMove=m_pSettings->stAxSettings[n].nTypeMove;
            MDS->stAxSettings[n].nStepComboIndex=m_pSettings->stAxSettings[n].nStepComboIndex;
            MDS->stAxSettings[n].fpVMan=m_pSettings->stAxSettings[n].fpVMan;
            MDS->stAxSettings[n].fpQuotaAbs=m_pSettings->stAxSettings[n].fpQuotaAbs;
            MDS->stAxSettings[n].nCurAxIndex=m_pSettings->stAxSettings[n].nCurAxIndex;
        }
        _delete (m_pSettings);
    }
}

void CAdamoMDCommandBar::OnDestroy() 
{
	CDialog::OnDestroy();
	
	DeleteObject (m_stFont->GetSafeHandle ());
	delete (m_stFont);
    _delete (m_pSettings);
    m_bkg.DeleteObject ();
}

void CAdamoMDCommandBar::StoreData (CAdamoAxis* pAx)
{
    double fp;
    int n;
    char szBuffer[256];
    if (m_pSettings)   {
        m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nCmdBar=1;
        GetSpeed (&fp, FALSE);
        m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].fpVMan=fp;
        n=GetCheckedRadioButton (IDC_RADIOJOG, IDC_RADIOABS);
        m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nTypeMove=n;
        n=((CComboBox*)GetDlgItem (IDC_COMBOVALUES))->GetCurSel();
        m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].nStepComboIndex=n;
        GetDlgItemText (IDC_EDITABS, szBuffer, 256);
        m_pSettings->stAxSettings[m_pAx->GetDeviceID ()].fpQuotaAbs= atof(szBuffer);
    }
}

HBRUSH CAdamoMDCommandBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	if (nCtlColor==CTLCOLOR_DLG)
        return m_bkg;
	else   {
		if (nCtlColor==CTLCOLOR_STATIC)   {
			int nID = ((CStatic *) pWnd)->GetDlgCtrlID ();
			if (nID == IDC_ST_JOG || nID == IDC_ST_STEP || nID == IDC_ST_ABS || nID == IDC_ST_LOOP_APERTO || nID == IDC_ST_LOOP_CHIUSO)   {
				pDC->SetBkMode (TRANSPARENT);
				pDC->SetTextColor (RGB (255, 255, 255));
				return m_bkg;
			}
		}
	}
	return hbr;
}

BEGIN_EVENTSINK_MAP(CAdamoMDCommandBar, CDialog)
    //{{AFX_EVENTSINK_MAP(CAdamoMDCommandBar)
	ON_EVENT(CAdamoMDCommandBar, IDC_IKNOBSPEED, 4 /* OnPositionChange */, OnPositionChangeIknobSpeed, VTS_NONE)
	ON_EVENT(CAdamoMDCommandBar, IDC_IKNOBSPEED, 11 /* OnMouseUp */, OnMouseUpIknobSpeed, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAdamoMDCommandBar::OnPositionChangeIknobSpeed() 
{
    char szBuffer[256];
	// TODO: Add your control notification handler code here
    sprintf (szBuffer, "%.2f", (double)m_knobSpeed.GetPosition());
    GetDlgItem (IDC_EDITSPEED)->SetWindowText (szBuffer);
}

void CAdamoMDCommandBar::InitButtons ()
{
	m_butMeno.SetSkin(IDB_B2NORM,IDB_B2DOWN,IDB_B2OVER,IDB_B2GRAY,0,IDB_B2MASK,0,0,0);
	m_butMeno.SetToolTipText(LOADSTRING (IDS_ADAMOMDCOMMANDBAR_5));
	m_butPiu.SetSkin(IDB_B1NORM,IDB_B1DOWN,IDB_B1OVER,IDB_B1GRAY,0,IDB_B1MASK,0,0,0);
	m_butPiu.SetToolTipText(LOADSTRING (IDS_ADAMOMDCOMMANDBAR_6));
	m_butStop.SetSkin(IDB_B5NORM,IDB_B5DOWN,IDB_B5OVER,IDB_B5GRAY,0,IDB_B5MASK,0,0,0);
	m_butStop.SetToolTipText(LOADSTRING (IDS_ADAMOMDCOMMANDBAR_7));
	m_butOpenLoop.SetSkin(IDB_B4NORM,IDB_B4DOWN,IDB_B4OVER,IDB_B4GRAY,0,IDB_B4MASK,0,0,0);
	m_butOpenLoop.SetToolTipText(LOADSTRING (IDS_ADAMOMDCOMMANDBAR_8));
	m_butCloseLoop.SetSkin(IDB_B3NORM,IDB_B3DOWN,IDB_B3OVER,IDB_B3GRAY,0,IDB_B3MASK,0,0,0);
	m_butCloseLoop.SetToolTipText(LOADSTRING (IDS_ADAMOMDCOMMANDBAR_9));
	m_butRadioJog.SetSkin(IDB_RADIODISABLED,IDB_RADIOJOG,IDB_RADIOOVER,IDB_RADIODISABLED,IDB_RADIOFOCUS,IDB_RADIOMASK,0,0,0);
	m_butRadioStep.SetSkin(IDB_RADIODISABLED,IDB_RADIOSTEP,IDB_RADIOOVER,IDB_RADIODISABLED,IDB_RADIOFOCUS,IDB_RADIOMASK,0,0,0);
	m_butRadioAbs.SetSkin(IDB_RADIODISABLED,IDB_RADIOABS,IDB_RADIOOVER,IDB_RADIODISABLED,IDB_RADIOFOCUS,IDB_RADIOMASK,0,0,0);
	m_butRadioJog.SetToolTipText(LOADSTRING (IDS_ADAMOMDCOMMANDBAR_10));
	m_butRadioStep.SetToolTipText(LOADSTRING (IDS_ADAMOMDCOMMANDBAR_11));
	m_butRadioAbs.SetToolTipText(LOADSTRING (IDS_ADAMOMDCOMMANDBAR_12));
}

void CAdamoMDCommandBar::SendAxDataDouble (int nType, double fpValue)
{
    CDeviceData Data;

    Data.m_eDev=enAsse;
    Data.m_nAddress=m_pAx->GetDeviceID ();
    Data.m_nType=nType;
    Data.m_fpDati=fpValue;
    m_pMachine->SetDevice (1, &Data);
}

void CAdamoMDCommandBar::OnMouseUpIknobSpeed(short Button, short Shift, long X, long Y) 
{
	// TODO: Add your control notification handler code here
	AfxGetMainWnd ()->SetFocus ();
}

void CAdamoMDCommandBar::GestionePassword ()
{
    bool bEnable=false;
    if (m_ePswLevel<=m_pAx->GetWAcces ())
        bEnable=true;
    m_butMeno.EnableWindow (bEnable);
    m_butPiu.EnableWindow (bEnable);
    m_butStop.EnableWindow (bEnable);
    m_butOpenLoop.EnableWindow (bEnable);
    m_butCloseLoop.EnableWindow (bEnable);
}

void CAdamoMDCommandBar::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    GestionePassword ();    
}

void CAdamoMDCommandBar::UpdateKnob (int nMode)
{
	double fpSpeed;

	if (!GetSpeed (&fpSpeed, false))   {
		switch (nMode)   {
			case 0 :
				if (fpSpeed > m_pAx->GetSpeedMan ())
					fpSpeed = m_pAx->GetSpeedMan ();
				m_knobSpeed.SetPositionMax (m_pAx->GetSpeedMan ());
				m_knobSpeed.SetPosition (fpSpeed);
				break;
			case 1 :
				if (fpSpeed > m_pAx->GetSpeed ())
					fpSpeed = m_pAx->GetSpeed ();
				m_knobSpeed.SetPositionMax (m_pAx->GetSpeed ());
				m_knobSpeed.SetPosition (fpSpeed);
				break;
			case 2 :
				if (fpSpeed > m_pAx->GetInterSpeed ())
					fpSpeed = m_pAx->GetInterSpeed ();
				m_knobSpeed.SetPositionMax (m_pAx->GetInterSpeed ());
				m_knobSpeed.SetPosition (fpSpeed);
				break;
		}
		m_nMode = nMode;
	}
}