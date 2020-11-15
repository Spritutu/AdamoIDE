// TransparentCheckBox.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "TransparentCheckBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransparentCheckBox

CTransparentCheckBox::CTransparentCheckBox() : m_pFnt (NULL)
{
}

CTransparentCheckBox::~CTransparentCheckBox()
{
}


BEGIN_MESSAGE_MAP(CTransparentCheckBox, CButton)
	//{{AFX_MSG_MAP(CTransparentCheckBox)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransparentCheckBox message handlers

void CTransparentCheckBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    CRect rc, rcText;

    if (!m_pFnt)
        m_pFnt=((CMainFrame*)AfxGetMainWnd())->CreateFont ("Tahoma", 14, FW_BOLD);
    GetClientRect (&rc);
    rc.right=16;
    ::DrawFrameControl (dc.GetSafeHdc (), &rc, DFC_BUTTON, DFCS_BUTTONCHECK|DFCS_FLAT);
    CFont* pFnt=dc.SelectObject (m_pFnt);
    dc.SetBkMode (TRANSPARENT);
    dc.TextOut (24, rc.top, CString ("Non visualizzare questa finestra al prossimo avvio di Thyra"));
    dc.SelectObject (pFnt);
}

void CTransparentCheckBox::OnDestroy() 
{
	CButton::OnDestroy();
	
    if (m_pFnt)   {
	    m_pFnt->DeleteObject ();
        _delete (m_pFnt);
    }
}

