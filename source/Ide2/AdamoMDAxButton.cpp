/************ FILE : mpaxbutt.cpp *******************************************
 *                                                                          *
 * PROGETTO : Albatros                                                      *
 *                                                                          *
 * SCOPO :  application file del bottone subclassato degli asse             *
 *                                                                          *
 * PROGETTISTA : Mottarella Maurizio                                        *
 *                                                                          *
 * DATA : 2 / 1 / 1996                                                      *
 ****************************************************************************/

/* include files */
#include "stdafx.h" 
#include "AdamoMDAxButton.h"

/* debug memory directive */
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CMDAxButton, CxSkinButton)
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

CMDAxButton::CMDAxButton () : m_nPushed (0), m_nKeyboardPushed (0), m_bCtrlOn (false)
{
}

CMDAxButton::~CMDAxButton ()
{                           
}

void CMDAxButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_bCtrlOn)   {
        int nKeyState=GetKeyState (VK_CONTROL);
        if (nKeyState&0x8000)   {
            m_nPushed=TRUE;                             
            m_nKeyboardPushed=1;
            CxSkinButton::OnLButtonDown(nFlags, point);
            m_pParent->SendMessage (WM_AXBUTTONDOWN, 0, (LPARAM) this);
        }
    }
    else   {
        m_nPushed=TRUE;
        m_nKeyboardPushed=1;
        CxSkinButton::OnLButtonDown(nFlags, point);
        m_pParent->SendMessage (WM_AXBUTTONDOWN, 0, (LPARAM) this);
    }
}

void CMDAxButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_nPushed)   {
        m_nKeyboardPushed=2;
        CxSkinButton::OnLButtonUp(nFlags, point);
        m_pParent->SendMessage (WM_AXBUTTONUP, 0, (LPARAM) this);
    }    
    m_nPushed=FALSE;
}
                                 
void CMDAxButton::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{                         
    if (m_nKeyboardPushed==1)
        lpDIS->itemState|=ODS_SELECTED;
    if (m_nKeyboardPushed==2)
        lpDIS->itemState&=(~ODS_SELECTED);
    CxSkinButton::DrawItem (lpDIS);
    m_nKeyboardPushed=0;
}
