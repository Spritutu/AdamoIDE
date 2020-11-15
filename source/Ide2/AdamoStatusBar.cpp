/*
** AdamoStatusBar.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "AdvancedStatusBar.h"

/* debug memory directive */
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static UINT indicators[] =
{
	ID_SEPARATOR,           
    ID_INDICATOR_LINE,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


BEGIN_MESSAGE_MAP(CAdamoStatusBar, CXTPStatusBar)
	//{{AFX_MSG_MAP(CControlBar)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CAdamoStatusBar::CAdamoStatusBar () : m_bLedOn (false), m_nCounter (0), m_nCounterErrors (0), m_nCounterMessages (0)
{
}

/*
** InitializeStatusBar : inizializza la status bar
*/
int CAdamoStatusBar::InitializeStatusBar ()
{
    int nB=FALSE;

	SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	GetPane(0)->SetBeginGroup(FALSE);
	SetRibbonDividerIndex(GetPaneCount() - 1);
	if (CreaLeddini ())   {
        EndCmd ();
		nB=TRUE;
    }
	CreaBottoncini ();
    return nB;
}

/*
** CreaLeddini : crea il led che segnala se le connessioni sono ok.
*/
int CAdamoStatusBar::CreaLeddini ()
{
    GetImageManager()->SetIcon(ID_ICO_NOCOMM, ID_ICO_NOCOMM);
    GetImageManager()->SetIcon(ID_ICO_OKCOMM, ID_ICO_OKCOMM);
    GetImageManager()->SetIcon(ID_ICO_NO_CMD, ID_ICO_NO_CMD);
    GetImageManager()->SetIcon(ID_ICO_CMD,    ID_ICO_CMD);

	CXTPStatusBarPane* pPane1 = AddIndicator(ID_INDICATOR_PSW_LEVEL);
    int iIndex = CommandToIndex(ID_INDICATOR_PSW_LEVEL);
	ASSERT(iIndex != -1);
	SetPaneWidth(iIndex, XTP_DPI_X(196));
	SetPaneStyle(iIndex, GetPaneStyle(iIndex) | SBPS_POPOUT);
	pPane1->SetText(_T("Password Level "));

	AddIconCommunication ();
	AddIconConnection ();
	EnableCustomization();
    return TRUE;
}

/*
** Connect : Accende il led di connesso.
*/
void CAdamoStatusBar::Connect (bool bConnect)
{
	if (bConnect)
		m_wndImageConnection.SetActualColor (false);
	else
		m_wndImageConnection.SetActualColor (true);
    m_wndImageConnection.Invalidate ();
}

/*
** InitCmd : Inizia un comando.
*/
void CAdamoStatusBar::InitCmd ()
{
	m_wndImageCommunication.SetActualColor (false);
	m_wndImageCommunication.Invalidate ();
    m_bLedOn = true;
}

/*
** EndCmd : termina un comando.
*/
void CAdamoStatusBar::EndCmd ()
{
}

/*
** OnTimer : chiamata dal timer di sistema.
*/
void CAdamoStatusBar::OnTimer ()
{
	CMyCXTPStatusBarPane* pPane;
	int nIndex;
    /* se il led e' acceso spegnamolo */
    if (m_bLedOn)   {
        if (m_nCounter >= 2)   {
			m_wndImageCommunication.SetActualColor (true);
			m_wndImageCommunication.Invalidate ();
            m_bLedOn = false;
            m_nCounter = 0;
        }
        m_nCounter++;
    }
	/* verifichiamo se sono accesi i pane degli errori */
	nIndex = CommandToIndex(ID_INDICATOR_SYSTEM_ERROR);
	pPane = (CMyCXTPStatusBarPane *)GetPane (nIndex);
	if (pPane && pPane->m_bSignaled)   {
		m_nCounterErrors++;
		if (m_nCounterErrors > 250)   {
			ShowAlarm (false);
		}
	}
	else
		m_nCounterErrors = 0;
	/* verifichiamo se sono accesi i pane degli errori */
	nIndex = CommandToIndex(ID_INDICATOR_SYSTEM_MESSAGE);
	pPane = (CMyCXTPStatusBarPane *)GetPane (nIndex);
	if (pPane && pPane->m_bSignaled)   {
		m_nCounterMessages++;
		if (m_nCounterMessages > 250)   {
			ShowMessage (false);
		}
	}
	else
		m_nCounterErrors = 0;
}

/*
** AddIconConnection :
*/
void CAdamoStatusBar::AddIconConnection ()
{
	if (!m_wndImageConnection.Create (NULL,WS_CHILD | WS_VISIBLE/* | SS_LEFT | SS_OWNERDRAW*/,
					CRect(0, 0, 16, 9), this, 100))
	{
		TRACE0("Failed to create edit control.\n");
		return;
	}
	m_wndImageConnection.SetColor1 (RGB (0, 255, 0));
	m_wndImageConnection.SetColor2 (RGB (255, 0, 0));
	m_wndImageConnection.SetActualColor (true);
	// add the indicator to the status bar.
	CXTPStatusBarPane* pPane = AddIndicator(ID_INDICATOR_CMD);

	// Initialize the pane info and add the control.
	int nIndex = CommandToIndex(ID_INDICATOR_CMD);
	ASSERT(nIndex != -1);

	SetPaneWidth(nIndex, XTP_DPI_X(16));
	SetPaneStyle(nIndex, GetPaneStyle(nIndex) | SBPS_OWNERDRAW);
	AddControl(&m_wndImageConnection, ID_INDICATOR_CMD, FALSE);
	m_wndImageConnection.Invalidate ();
}

/*
** AddIconCommunication :
*/
void CAdamoStatusBar::AddIconCommunication ()
{
	if (!m_wndImageCommunication.Create (NULL,WS_CHILD | WS_VISIBLE/* | SS_LEFT | SS_OWNERDRAW*/,
					CRect(0, 0, 16, 9), this, 100))
	{
		TRACE0("Failed to create edit control.\n");
		return;
	}
	m_wndImageCommunication.SetColor1 (RGB (255, 255, 0));
	m_wndImageCommunication.SetColor2 (RGB (255, 128, 64));
	m_wndImageCommunication.SetActualColor (true);
	// add the indicator to the status bar.
	CXTPStatusBarPane* pPane = AddIndicator(ID_INDICATOR_COMM);

	// Initialize the pane info and add the control.
	int nIndex = CommandToIndex(ID_INDICATOR_COMM);
	ASSERT(nIndex != -1);

	SetPaneWidth(nIndex, XTP_DPI_X(16));
	SetPaneStyle(nIndex, GetPaneStyle(nIndex) | SBPS_OWNERDRAW);
	AddControl(&m_wndImageCommunication, ID_INDICATOR_COMM, FALSE);
	m_wndImageCommunication.Invalidate ();
}

void CAdamoStatusBar::CreaBottoncini ()
{
	CMyCXTPStatusBarPane* pMyPane_1 = new CMyCXTPStatusBarPane (IDB_CAPALARM);
	AddIndicator(pMyPane_1, ID_INDICATOR_SYSTEM_ERROR, 0);

	// Initialize the pane info and add the control.
	int nIndex = CommandToIndex(ID_INDICATOR_SYSTEM_ERROR);
	ASSERT(nIndex != -1);

	SetPaneWidth(nIndex, XTP_DPI_X(64));

	CMyCXTPStatusBarPane* pMyPane_2 = new CMyCXTPStatusBarPane (IDB_CAPSTRING);
	AddIndicator(pMyPane_2, ID_INDICATOR_SYSTEM_MESSAGE, 1);

	// Initialize the pane info and add the control.
	nIndex = CommandToIndex(ID_INDICATOR_SYSTEM_MESSAGE);
	ASSERT(nIndex != -1);

	SetPaneWidth(nIndex, XTP_DPI_X(64));

	CMyCXTPStatusBarPane* pMyPane_3 = new CMyCXTPStatusBarPane (IDB_CAPGOMMA);
	AddIndicator(pMyPane_3, ID_INDICATOR_DELETE_ERRORS, 2);

	// Initialize the pane info and add the control.
	nIndex = CommandToIndex(ID_INDICATOR_DELETE_ERRORS);
	ASSERT(nIndex != -1);

	SetPaneWidth(nIndex, XTP_DPI_X(64));
}

void CAdamoStatusBar::ShowAlarm (bool bShow)
{
	int nIndex = CommandToIndex(ID_INDICATOR_SYSTEM_ERROR);
	CMyCXTPStatusBarPane* pPane = (CMyCXTPStatusBarPane *)GetPane (nIndex);
	if (bShow)
		pPane->m_bSignaled = true;
	else
		pPane->m_bSignaled = false;
	m_nCounterErrors = 0;
	Invalidate ();
}

void CAdamoStatusBar::ShowMessage (bool bShow)
{
	int nIndex = CommandToIndex(ID_INDICATOR_SYSTEM_MESSAGE);
	CMyCXTPStatusBarPane* pPane = (CMyCXTPStatusBarPane *)GetPane (nIndex);
	if (bShow)
		pPane->m_bSignaled = true;
	else
		pPane->m_bSignaled = false;
	m_nCounterMessages = 0;
	Invalidate ();
}

BEGIN_MESSAGE_MAP(CTransparentImage, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/** construction / destruction **/
CTransparentImage::CTransparentImage ()
{
}

CTransparentImage::~CTransparentImage ()
{
}

void CTransparentImage::OnPaint ()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	CBitmap *pOldBitmap = MemDC.SelectObject(&MemBitmap);

	CBrush brBkGnd;
	brBkGnd.CreateSolidBrush(m_clrActual);
	MemDC.FillRect(&rc ,&brBkGnd);
	MemDC.SetBkMode(TRANSPARENT);
	

	// Render
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBitmap);
}

/*
** OnDraw :
*/
void CMyCXTPStatusBarPane::OnDraw (CDC* pDC, CRect rcItem)
{
	if (m_bSignaled)   {
		CBrush *pBR;
		if (GetID () == ID_INDICATOR_SYSTEM_ERROR)
			pBR = new CBrush (RGB (255, 0, 0));
		else
		if (GetID () == ID_INDICATOR_SYSTEM_MESSAGE)
			pBR = new CBrush (RGB (255, 201, 14));
		pDC->FillRect (rcItem, pBR);
		delete pBR;
	}
	((CMainFrame*) AfxGetMainWnd ())->DrawTransparentBitmap (pDC->GetSafeHdc (), *m_pBitmap, (short)rcItem.left + 24, (short)rcItem.top, RGB (0, 128, 128));
}

/*
** OnLButtonDown :
*/
void CMyCXTPStatusBarPane::OnLButtonDown (CPoint point)
{
	((CMainFrame*) AfxGetMainWnd ())->SendMessage (WM_OPENDIALOGBARS, ID_STATIC_STATIC, 0L);
}

/*
** OnLButtonDown :
*/
void CAdamoStatusBar::OnLButtonDown (UINT nFlags, CPoint pt)
{
	int nIndex = CommandToIndex(ID_INDICATOR_SYSTEM_ERROR);
	CMyCXTPStatusBarPane* pPane = (CMyCXTPStatusBarPane *)GetPane (nIndex);
	CRect rc = pPane->GetRect ();
	if (rc.PtInRect (pt))   {
		((CMainFrame*) AfxGetMainWnd ())->SendMessage (WM_OPENDIALOGBARS, ID_STATIC_STATIC, 0L);
	}
	else   {
		nIndex = CommandToIndex(ID_INDICATOR_SYSTEM_MESSAGE);
		pPane = (CMyCXTPStatusBarPane *)GetPane (nIndex);
		rc = pPane->GetRect ();
		if (rc.PtInRect (pt))   {
			((CMainFrame*) AfxGetMainWnd ())->SendMessage (WM_OPENDIALOGBARS, ID_STATIC_STRINGS, 0L);
		}
		else   {
			nIndex = CommandToIndex(ID_INDICATOR_DELETE_ERRORS);
			pPane = (CMyCXTPStatusBarPane *)GetPane (nIndex);
			rc = pPane->GetRect ();
			if (rc.PtInRect (pt))   {
				((CMainFrame*) AfxGetMainWnd ())->SendMessage (WM_DELETEERRORS, 0, 0L);
			}
		}
	}
	Invalidate ();
	__super::OnLButtonDown (nFlags, pt);
}
