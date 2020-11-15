// TestDlgCounterDlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "ide2.h"
#include "mainframe.h"
#include "AdamoMDAxStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxStatus dialog

CAdamoMDAxStatus::CAdamoMDAxStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoMDAxStatus::IDD, pParent), m_nCount (0),
      m_pCmdBar (NULL), m_pMachine (NULL),
      m_pSettings (NULL), m_bInitialized (false), m_nSelectedAx (0), m_pDigitFont (NULL),
	  m_bTimerOk (false)
{
	//{{AFX_DATA_INIT(CAdamoMDAxStatus)
	//}}AFX_DATA_INIT
    memset (m_aszConfrontoAssi, 0, 5*sizeof (CString *));
    memset (m_aszTipoMovimento, 0, 11*sizeof (CString *));
}

CAdamoMDAxStatus::~CAdamoMDAxStatus ()
{
    _delete (m_pSettings);
	DestroyStrings ();
	m_pDigitFont->DeleteObject ();
	delete m_pDigitFont;
}

void CAdamoMDAxStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoMDAxStatus)
	DDX_Control(pDX, IDC_AXSTATUS_REPORT, m_wndReport);
	DDX_Control(pDX, IDC_GAUGEVELTEORICA, m_gaugeVelTeorica);
	DDX_Control(pDX, IDC_GAUGEACCELERAZIONE, m_gaugeAccelerazione);
	DDX_Control(pDX, IDC_GAUGECOPPIA, m_gaugeCoppia);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAdamoMDAxStatus, CDialog)
	//{{AFX_MSG_MAP(CAdamoMDAxStatus)
	ON_WM_CTLCOLOR()
    ON_WM_SIZE()
	ON_WM_CREATE ()
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, IDC_AXSTATUS_REPORT, OnReportSelChanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoMDAxStatus message handlers

/*
** OnCreate :
*/
int CAdamoMDAxStatus::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_wndReport.SetParent (this);
	CXTPCustomDrawReportPaintManager* pPaintManager = new CXTPCustomDrawReportPaintManager(this);
	m_wndReport.SetPaintManager(pPaintManager);
	m_br.CreateSolidBrush (RGB (192, 192, 192));
	return 0;
}

/*
** OnInitDialog :
*/
BOOL CAdamoMDAxStatus::OnInitDialog()
{
	CDialog::OnInitDialog();

    InitStrings ();
    for (POSITION pos=m_la->GetHeadPosition (); pos;)
        m_apAxis[m_nCount++]=(CAdamoAxis*)m_la->GetNext (pos);

	InitReportControl ();
    InitBackTimerData ();
	UpdateLabelUM ();

    m_bInitialized=true;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAdamoMDAxStatus::UpdateAxesData ()
{
    m_pCmdBar->SetAx (m_apAxis[m_nSelectedAx], 0);
    InitAngularGauges ();
	UpdateLabelUM ();
}

void CAdamoMDAxStatus::OnAdamoTimer ()
{
    CDeviceData dd;
    struct stDatiAsse da;
    char szBuffer[256];

    if (m_bInitialized)   {
        /* per tutti gli assi */
        for (int i=0, n=0; i<m_nCount; i++, n++)    {
            /* rinfreschiamo prima di tutto la quota reale */
            dd.m_eDev=enAsse;
            dd.m_nAddress=m_apAxis[i]->GetDeviceID();
            if (m_pMachine->GetAxes (1, &dd, &da)==S_OK)   {
                /* quota reale */
                if (!ISEQUAL (da.fpQuotaReale, m_fpBackQuotaReale[i]))   {
                    sprintf (szBuffer, "%8.3f", da.fpQuotaReale);
                    m_fpBackQuotaReale[i]=da.fpQuotaReale;
                }
                /* quota  teorica */
                if (!ISEQUAL (da.fpQuotaTeorica, m_fpBackQuotaTeorica[i]))   {
                    sprintf (szBuffer, "%8.3f", da.fpQuotaTeorica);
                    m_fpBackQuotaTeorica[i]=da.fpQuotaTeorica;
                }
                /* errore d'anello */
                if (!ISEQUAL (da.fpErroreAnello, m_fpBackEA[i]))   {
                    sprintf (szBuffer, "%8.3f", da.fpErroreAnello);
                    m_fpBackEA[i]=da.fpErroreAnello;
                }
                /* stato */
                if (da.nStato!=m_nBackStato[i])   {
                    if (!da.nStato)
                        da.nStato=QUOTA;    
                    m_nBackStato[i]=da.nStato;
                }
                /* tipo movimento  */
                if (da.nTipo!=m_nBackTM[i])   {
                    m_nBackTM[i]=da.nTipo;
                }
                /* ora le spie di stato */
                DrawBitmapStatus (n, &da);
                if (i==m_nSelectedAx)   {
                    /* velocita teorica */
                    if (!ISEQUAL (da.fpVelAttuale, m_fpBackVelocitaTeorica))   {
                        sprintf (szBuffer, "%8.2f", da.fpVelAttuale);
                        //m_stVelTeorica.SetText (szBuffer);
                        m_fpBackVelocitaTeorica=da.fpVelAttuale;
                        m_gaugeVelTeorica.SetPosition (fabs (m_fpBackVelocitaTeorica));
						m_gaugeVelTeorica.SetLabel2Text (szBuffer);
                    }
                    /* accelerazione */
                    if (!ISEQUAL (da.fpAccInstInt, m_fpBackAccelerazione))   {
                        sprintf (szBuffer, "%8.2f", da.fpAccInstInt);
                        //m_stAccelerazione.SetText (szBuffer);
                        m_fpBackAccelerazione=da.fpAccInstInt;
                        m_gaugeAccelerazione.SetPosition (fabs (m_fpBackAccelerazione));
                        m_gaugeAccelerazione.SetLabel2Text (szBuffer);
                    }
                    /* coppia */
                    if (!ISEQUAL (da.fpTorqueReale, m_fpBackCoppia))   {
                        sprintf (szBuffer, "%8.2f", da.fpTorqueReale);
                        //m_stCoppia.SetText (szBuffer);
                        m_fpBackCoppia=da.fpTorqueReale;
                        m_gaugeCoppia.SetPosition (m_fpBackCoppia);
                        m_gaugeCoppia.SetLabel2Text (szBuffer);
                    }
                }
            }
        }
		m_bTimerOk = true;
		m_wndReport.RedrawControl ();
    }
}

void CAdamoMDAxStatus::OnViewSettings (stMDSettings* MDS)
{
    if (!m_pSettings)   {
        m_pSettings=new stMDSettings;
        memset (m_pSettings, 0, sizeof (stMDSettings));
        if (m_pSettings)
            if (MDS->nAxStatus)   {
                *m_pSettings=*MDS;
                UpdateData ();	// verificare
            }
            else
                MDS->nAxStatus=1;
    }
}

void CAdamoMDAxStatus::GetViewSettings (stMDSettings* MDS)
{
    if (m_pSettings)   {
        MDS->nCurAx=m_apAxis[m_nSelectedAx]->GetDeviceID();
        SAFE_DELETE (m_pSettings);
    }
}

void CAdamoMDAxStatus::UpdateData ()
{
    InitAngularGauges ();
}

void CAdamoMDAxStatus::OnSize (UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if ( ::IsWindow(m_wndReport.m_hWnd) )   {
		CRect rc;
		GetClientRect (&rc);
		int yReportControl = cy;
		int nRow = yReportControl / AXISROWHEIGHT;
		if (nRow > m_wndReport.GetRecords ()->GetCount ())
			nRow = m_wndReport.GetRecords ()->GetCount ();
		yReportControl = nRow * AXISROWHEIGHT;
		int nHeaderHeight = m_wndReport.GetPaintManager ()->GetHeaderHeight ();
		m_wndReport.SetWindowPos(NULL, 0, 0, cx-220, yReportControl + nHeaderHeight + 2, 0);
		rc.left = cx-219; rc.top = 0; rc.right = rc.left + 218; rc.bottom = rc.top + 218;
		m_gaugeVelTeorica.MoveWindow (&rc);
		rc.left = cx-219; rc.top = 220; rc.right = rc.left + 218; rc.bottom = rc.top + 218;
		m_gaugeCoppia.MoveWindow (&rc);
		rc.left = cx-219; rc.top = 440; rc.right = rc.left + 218; rc.bottom = rc.top + 218;
		m_gaugeAccelerazione.MoveWindow (&rc);
	}
}

HBRUSH CAdamoMDAxStatus::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	if (nCtlColor==CTLCOLOR_DLG)
		return (HBRUSH) m_br.GetSafeHandle ();
	return hbr;
}

void CAdamoMDAxStatus::InitBackTimerData ()
{
    memset (m_fpBackQuotaReale, 0, sizeof (m_fpBackQuotaReale));
    memset (m_fpBackQuotaTeorica, 0, sizeof (m_fpBackQuotaTeorica));
    memset (m_fpBackEA, 0, sizeof (m_fpBackEA));
    memset (m_nBackStato, 0, sizeof (m_nBackStato));
    m_fpBackVelocitaTeorica=0.0;
    m_fpBackAccelerazione=0.0;
    m_fpBackCoppia=0.0;
}

void CAdamoMDAxStatus::InitAngularGauges ()
{
    double fpAcc, fpDec;
    m_gaugeVelTeorica.SetPositionMin (0.0);
    m_gaugeVelTeorica.SetPositionMax (m_apAxis[m_nSelectedAx]->GetSpeedMan ());
    m_gaugeCoppia.SetPositionMax (m_apAxis[m_nSelectedAx]->GetPercTorque ());
    m_gaugeCoppia.SetPositionMin (-m_apAxis[m_nSelectedAx]->GetPercTorque ());
    fpAcc=(m_apAxis[m_nSelectedAx]->GetSpeed()/60.0)/(m_apAxis[m_nSelectedAx]->GetAcc()/1000.0);
    fpDec=(m_apAxis[m_nSelectedAx]->GetSpeed()/60.0)/(m_apAxis[m_nSelectedAx]->GetDec()/1000.0);
    if (fpDec>fpAcc)
        fpAcc=fpDec;
    m_gaugeAccelerazione.SetPositionMin (0.0);
    m_gaugeAccelerazione.SetPositionMax (fpAcc);
}

/*
** DrawBitmapStatus :
*/
void CAdamoMDAxStatus::DrawBitmapStatus (int nProg, struct stDatiAsse* da)
{
}

/*
** InitStrings : inizializza le stringhe della dialog
*/
void CAdamoMDAxStatus::InitStrings ()
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
void CAdamoMDAxStatus::DestroyStrings ()
{
    int i;

    for (i=0; i<5; i++)
        _delete (m_aszConfrontoAssi[i]);
    for (i=0; i<11; i++)
        _delete (m_aszTipoMovimento[i]);
}

/*
** OnChangePsw :
*/
void CAdamoMDAxStatus::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_pCmdBar->SetAx (m_apAxis[m_nSelectedAx], 0);
    m_pCmdBar->OnChangePsw (oldPsw, newPsw);
}

/*
** UpdateLabelUM :
*/
void CAdamoMDAxStatus::UpdateLabelUM ()
{
	CString strSpazio, strVel, str;

	CAdamoAxis* pAx = m_apAxis[m_nSelectedAx];
	switch (pAx->GetUnit ())   {
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
}

/*
** InitReportControl :
*/
void CAdamoMDAxStatus::InitReportControl ()
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
void CAdamoMDAxStatus::FillColumns ()
{
	CXTPReportColumn* pColumn;
	int nItemIndex = 0;

	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMO_AXIS_STATUS_11), 300);
	m_wndReport.AddColumn (pColumn);
	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMO_AXIS_STATUS_12), 300);
	m_wndReport.AddColumn (pColumn);
	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMO_AXIS_STATUS_13), 300);
	m_wndReport.AddColumn (pColumn);
	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMO_AXIS_STATUS_14), 300);
	m_wndReport.AddColumn (pColumn);
	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMO_AXIS_STATUS_15), 300);
	m_wndReport.AddColumn (pColumn);
	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMO_AXIS_STATUS_16), 300);
	m_wndReport.AddColumn (pColumn);
}

/*
** FillGrid :
*/
void CAdamoMDAxStatus::FillGrid ()
{
	CXTPReportRecord* pRecord = NULL;
	CXTPReportRecordItemText* pItem;
	int i = 0;

	while (i < m_nCount)   {
		pRecord = new CXTPReportRecord();
		pItem = (CXTPReportRecordItemName *) new CXTPReportRecordItemName (m_apAxis[i]->GetName ());
		pItem->SetItemData ((DWORD_PTR) i);
		pRecord->AddItem (pItem);
		pItem = (CXTPReportRecordItemPositions *) new CXTPReportRecordItemPositions ();
		pRecord->AddItem (pItem);
		pItem = (CXTPReportRecordItemPositions *) new CXTPReportRecordItemPositions ();
		pRecord->AddItem (pItem);
		pItem = (CXTPReportRecordItemPositions *) new CXTPReportRecordItemPositions ();
		pRecord->AddItem (pItem);
		pItem = (CXTPReportRecordItemStatus *) new CXTPReportRecordItemStatus ();
		pRecord->AddItem (pItem);
		pItem = (CXTPReportRecordItemDynamic *) new CXTPReportRecordItemDynamic ();
		pRecord->AddItem (pItem);
		m_wndReport.AddRecord (pRecord);
		i++;
	}
}

/*
** OnReportSelChanged :
*/
void CAdamoMDAxStatus::OnReportSelChanged (NMHDR* pNotifyStruct, LRESULT* pResult	)
{
	CXTPReportRecord* pRecord = NULL;
	CXTPReportRow* pRow = NULL;
	CObject *pSelect = NULL;
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows ();

	if (pSelectedRows->GetCount () > 0)   {
		pRow = pSelectedRows->GetAt (0);
		if (pRow)   {
			pRecord = pRow->GetRecord ();
			CXTPReportRecordItem* pItem = pRecord->GetItem (0);
			if (pItem)   {
				m_nSelectedAx = (int) pItem->GetItemData ();
				m_pCmdBar->SetAx (m_apAxis[m_nSelectedAx], 0);
			}
		}
	}
}

/*
** OnFrecciaGiu :
*/
void CAdamoMDAxStatus::OnFrecciaGiu ()
{
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows ();
	if (pSelectedRows->GetCount () > 0)   {
		CXTPReportRow* pSelectedRow = pSelectedRows->GetAt (0);
		if (pSelectedRow)   {
			CXTPReportRows* pRows = m_wndReport.GetRows();
			for (int i = 0; i<pRows->GetCount (); i++)   {
				CXTPReportRow* pRow = pRows->GetAt (i);
				if (pRow == pSelectedRow)   {
					if (i < pRows->GetCount () - 1)   {
						pRow = pRows->GetAt (i + 1);
						m_wndReport.SetFocusedRow (pRow);
						m_wndReport.Invalidate ();
						AfxGetMainWnd ()->SetFocus ();
						break;
					}
				}
			}
		}
	}
}

/*
** OnFrecciaSu :
*/
void CAdamoMDAxStatus::OnFrecciaSu ()
{
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows ();
	if (pSelectedRows->GetCount () > 0)   {
		CXTPReportRow* pSelectedRow = pSelectedRows->GetAt (0);
		if (pSelectedRow)   {
			CXTPReportRows* pRows = m_wndReport.GetRows();
			for (int i = 0; i<pRows->GetCount (); i++)   {
				CXTPReportRow* pRow = pRows->GetAt (i);
				if (pRow == pSelectedRow)   {
					if (i > 0)   {
						pRow = pRows->GetAt (i - 1);
						m_wndReport.SetFocusedRow (pRow);
						m_wndReport.Invalidate ();
						AfxGetMainWnd ()->SetFocus ();
						break;
					}
				}
			}
		}
	}
}

/*
** OnDrawCaption :
*/
void CXTPReportRecordItemName::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CXTPReportRecord* pRecord = pDrawArgs->pRow->GetRecord ();
	CDC* pDC = pDrawArgs->pDC;
	COLORREF crf, oldcrf;

	CBrush* pBrush = new CBrush ();
	CFont* pFont = ((CMainFrame *)AfxGetMainWnd ())->CreateFont ("Segoe UI", 40, FW_BOLD);
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
void CXTPReportRecordItemPositions::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CXTPReportRecord* pRecord = pDrawArgs->pRow->GetRecord ();
	CAdamoMDAxStatus* pAxStatus = (CAdamoMDAxStatus *) pDrawArgs->pControl->GetParent ();
	CString strValue;
	double fp;
	int nAxesIndex = (int) pRecord->GetItem (0)->GetItemData ();
	COLORREF crf, oldcrf;

	switch (pDrawArgs->pColumn->GetIndex ())   {
		case 1 :
			fp = pAxStatus->m_fpBackQuotaTeorica[nAxesIndex];
			break;
		case 2 :
			fp = 0.0;
			break;
		case 3 :
			fp = pAxStatus->m_fpBackQuotaReale[nAxesIndex];
			break;
	}
	crf = RGB (0, 0, 0);
	oldcrf = pDrawArgs->pDC->SetTextColor (crf);
	strValue.Format ("%.3f", fp);
	HGDIOBJ hObj = pDrawArgs->pDC->SelectObject (pAxStatus->m_pDigitFont);
	::DrawText (pDrawArgs->pDC->GetSafeHdc (), strValue, -1, &pDrawArgs->rcItem, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	pDrawArgs->pDC->SelectObject (hObj);
	pDrawArgs->pDC->SetTextColor (oldcrf);
}

/*
** OnDrawCaption :
*/
void CXTPReportRecordItemStatus::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CAdamoMDAxStatus* pAxStatus = (CAdamoMDAxStatus *) pDrawArgs->pControl->GetParent ();
	CDC* pDC = pDrawArgs->pDC;
	COLORREF crf, oldcrf;
	CBrush* pBrush = new CBrush ();
	pBrush->CreateSolidBrush (RGB (96, 96, 96));
	crf = RGB (255, 255, 255);
	CBrush* pOldBrush = pDC->SelectObject (pBrush);
	oldcrf = pDC->SetTextColor (crf);
	FillRect (pDC->GetSafeHdc (), pDrawArgs->rcItem, (HBRUSH) pBrush);
	if (pAxStatus->m_bTimerOk == true)   {
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
		::DrawText (pDrawArgs->pDC->GetSafeHdc (), *pAxStatus->m_aszConfrontoAssi[pAxStatus->m_nBackStato[nAxesIndex]-1], -1, &rcStatus, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		::DrawText (pDrawArgs->pDC->GetSafeHdc (), LOADSTRING (IDS_ADAMO_AXIS_STATUS_22), -1, &rcMovementTypeLabel, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
		::DrawText (pDrawArgs->pDC->GetSafeHdc (), *pAxStatus->m_aszTipoMovimento[pAxStatus->m_nBackTM[nAxesIndex]], -1, &rcMovementType, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	}
	pDC->SetTextColor (oldcrf);
	pDC->SelectObject (pOldBrush);
	delete (pBrush);
}

/*
** OnDrawCaption :
*/
void CXTPReportRecordItemDynamic::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
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
CXTPCustomDrawReportPaintManager::CXTPCustomDrawReportPaintManager(CAdamoMDAxStatus* pParent) : m_pParent (pParent)
{
	m_bHideSelection = true;
	m_clrHighlight.SetCustomValue (m_clrControlBack.GetStandardColor ());
	SetGridStyle (FALSE, xtpReportLineStyleNone);
}

CXTPCustomDrawReportPaintManager::~CXTPCustomDrawReportPaintManager()
{
}

/*
** GetRowHeight :
*/
int CXTPCustomDrawReportPaintManager::GetRowHeight(CDC* pDC, CXTPReportRow* pRow)
{
	return XTP_DPI_Y(AXISROWHEIGHT);
}

/*
** DrawFocusedRow :
*/
void CXTPCustomDrawReportPaintManager::DrawFocusedRow (CDC* pDC, CRect rcRow)
{
}

