// AllAxesView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AllAxesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllAxesView

CAllAxesView::CAllAxesView()
{
}

CAllAxesView::~CAllAxesView()
{
}


BEGIN_MESSAGE_MAP(CAllAxesView, CWnd)
	//{{AFX_MSG_MAP(CAllAxesView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAllAxesView message handlers

void CAllAxesView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    CBrush br;
	
	// TODO: Add your message handler code here
    RECT rc;
    br.CreateSolidBrush (RGB (255, 0, 0));
    GetClientRect (&rc);
    CBrush* obr=dc.SelectObject (&br);
    dc.Rectangle (&rc);
    dc.SelectObject (obr);
    br.DeleteObject ();
    //dc.DrawText ("Hy, I am the All Axes View !!!", &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	// Do not call CWnd::OnPaint() for painting messages
}
