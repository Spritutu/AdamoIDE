// OscChannelData.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "ide2.h"
#include "samplesgrid.h"
#include "OscChannelData.h"
#include "mainframe.h"
#include "AdamoGlobal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* static global variables */
DWORD COscChannelData::m_colors[MAXSAMPLESNUMBER] = { RGB (255, 255, 0),
                                                RGB (255, 0, 0),
                                                RGB (0, 0, 255),
                                                RGB (0, 255, 0),
                                                RGB (0, 255, 255),
                                                RGB (255, 0, 255),
                                                RGB (192, 192, 192),
                                                RGB (255, 255, 255) };

/////////////////////////////////////////////////////////////////////////////
// COscChannelData

COscChannelData::COscChannelData() : m_bSelected (false), m_pAD (NULL), m_nChannel (-1), m_pDataFont (NULL), m_pCaptionFont (NULL)
{
	m_pDataFont = ((CMainFrame *)AfxGetMainWnd ())->CreateFont ("Segoe UI", 20, FW_BOLD);
	m_pCaptionFont = ((CMainFrame *)AfxGetMainWnd ())->CreateFont ("Segoe UI", 16, FW_BOLD);
}

COscChannelData::~COscChannelData()
{
	if (m_pDataFont)
		_delete (m_pDataFont);
	if (m_pCaptionFont)
		_delete (m_pCaptionFont);
}


BEGIN_MESSAGE_MAP(COscChannelData, CStatic)
	//{{AFX_MSG_MAP(COscChannelData)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COscChannelData message handlers
void COscChannelData::OnPaint() 
{
    if (m_nChannel!=-1)   {
	    CPaintDC dc(this); // device context for painting
        DrawBackGround (&dc, m_bSelected);
        DrawVariousText (&dc);
        DrawSymbol (&dc);
        DrawCaption (&dc);
    }
}

void COscChannelData::DrawFrameSelected (CPaintDC* pDC)
{
    RECT rc;

    GetClientRect (&rc);

    CPen pWhite, pBlack, pGray;
    pWhite.CreatePen (PS_SOLID, 1, RGB (255, 255, 255));    
    pBlack.CreatePen (PS_SOLID, 1, RGB (128, 128, 128));    
    pGray.CreatePen (PS_SOLID, 1, RGB (192, 192, 192));    
    CPen* pOld=pDC->SelectObject (&pWhite);
    pDC->MoveTo (0, 1);
    pDC->LineTo (0, rc.bottom-1);
    pDC->MoveTo (0, 1);
    pDC->LineTo (rc.right-1, 1);
    pDC->SelectObject (&pBlack);
    pDC->MoveTo (0, rc.bottom-1);
    pDC->LineTo (rc.right-1, rc.bottom-1);
    pDC->LineTo (rc.right-1, 0);
    pDC->SelectObject (&pGray);
    pDC->MoveTo (1, rc.bottom-2);
    pDC->LineTo (rc.right-2, rc.bottom-2);
    pDC->LineTo (rc.right-2, 1);
    pDC->SelectObject (&pOld);
}

void COscChannelData::DrawLittleBox (CPaintDC* pDC, int x, int y, int cx, int cy)
{
    CPen pWhite, pGray;
    pWhite.CreatePen (PS_SOLID, 1, RGB (255, 255, 255));    
    pGray.CreatePen (PS_SOLID, 1, RGB (192, 192, 192));    
    CPen* pOld=pDC->SelectObject (&pWhite);
    pDC->MoveTo (x, y);
    pDC->LineTo (x, y+cy-1);
    pDC->MoveTo (x, y);
    pDC->LineTo (x+cx, y);
    pDC->SelectObject (&pGray);
    pDC->MoveTo (x, y+cy-1);
    pDC->LineTo (x+cx-1, y+cy-1);
    pDC->LineTo (x+cx-1, y);
    pDC->SelectObject (&pOld);
}

void COscChannelData::DrawVariousText (CPaintDC* pDC)
{
    RECT rc;
    char szBuffer[256];
    CFont* pOldFont=pDC->SelectObject (((CMainFrame*)AfxGetMainWnd())->GetProgramFont ());
    sprintf (szBuffer, "%d", m_nChannel);
    pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (RGB (64, 64, 64));
    pDC->SelectObject (m_pCaptionFont);
    pDC->TextOut (10, 6, szBuffer, strlen (szBuffer));
    pDC->TextOut (10, 28, m_strType);
    rc.left=10, rc.right=174, rc.top=42, rc.bottom=76;
	pDC->SetTextColor (RGB (255, 255, 255));
	pDC->SelectObject (m_pDataFont);
    pDC->DrawText (m_strCurrentValue.Trim (), &rc, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
    pDC->SelectObject (pOldFont);
}

void COscChannelData::DrawSymbol (CPaintDC* pDC)
{
    CBrush br;
	CPen pen;
    RECT rc;
    br.CreateSolidBrush (COscChannelData::m_colors[m_nChannel]);
	pen.CreatePen (PS_SOLID, 1, COscChannelData::m_colors[m_nChannel]);
    CBrush* pOld = pDC->SelectObject (&br);
    CPen* pOldPen = pDC->SelectObject (&pen);
    rc.left=26, rc.right=46, rc.top=4, rc.bottom=24;
    pDC->Ellipse (&rc);
    pDC->SelectObject (pOld);
    pDC->SelectObject (pOldPen);
}

void COscChannelData::DrawCaption (CPaintDC* pDC)
{
    RECT rc;
    CFont* pOld=pDC->SelectObject (m_pCaptionFont);
    pDC->SetBkMode (TRANSPARENT);
    pDC->SetTextColor (RGB (255, 255, 255));
    rc.left=51, rc.right=178, rc.top=5, rc.bottom=24;
    pDC->DrawText (m_strName, &rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
    pDC->SelectObject (pOld);
}

void COscChannelData::DrawFrame (CPaintDC* pDC)
{
    RECT rc;

    GetClientRect (&rc);

    CPen pBlack;
    pBlack.CreatePen (PS_SOLID, 1, RGB (128, 128, 128));    
    CPen* pOld=pDC->SelectObject (&pBlack);
    pDC->MoveTo (0, 1);
    pDC->LineTo (0, rc.bottom-1);
    pDC->MoveTo (0, 1);
    pDC->LineTo (rc.right-1, 1);
    pDC->MoveTo (0, rc.bottom-1);
    pDC->LineTo (rc.right-1, rc.bottom-1);
    pDC->LineTo (rc.right-1, 0);
    pDC->MoveTo (1, rc.bottom-2);
    pDC->LineTo (rc.right-2, rc.bottom-2);
    pDC->LineTo (rc.right-2, 1);
    pDC->SelectObject (&pOld);
}

void COscChannelData::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	GetParent()->SendMessage (IDMSG_SELOSCCHAANNEL, m_nChannel, (LPARAM)NULL);
	CStatic::OnLButtonDown(nFlags, point);
}

void COscChannelData::SetCurrentValue (const char* szCurrentValue)
{
    RECT rc;
    m_strCurrentValue=szCurrentValue;
    rc.left=10, rc.right=174, rc.top=42, rc.bottom=76;
    InvalidateRect (&rc, FALSE);
}

void COscChannelData::DrawBackGround (CPaintDC* pDC, bool bSelected)
{
    CBrush br;
	CPen pen;
    RECT rc; 

    GetClientRect (&rc);
	if (bSelected)    {
		br.CreateSolidBrush (RGB (192, 192, 192));
		pen.CreatePen (PS_SOLID, 1, RGB (192, 192, 192));
	}
	else   {
		br.CreateSolidBrush (RGB (128, 128, 128));
		pen.CreatePen (PS_SOLID, 1, RGB (128, 128, 128));
	}
    CBrush* pOldBrush = pDC->SelectObject (&br);
    CPen* pOldPen = pDC->SelectObject (&pen);
    pDC->Rectangle (&rc);
	pDC->SelectObject (pOldBrush);
	pDC->SelectObject (pOldPen);
    br.DeleteObject ();
	pen.DeleteObject ();
}
