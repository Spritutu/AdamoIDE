/************ FILE : mpaxbutt.h *********************************************
 *                                                                          *
 * PROGETTO : Albatros                                                      *
 *                                                                          *
 * SCOPO :  interface file del bottone subclassato degli asse               *
 *                                                                          *
 * PROGETTISTA : Mottarella Maurizio                                        *
 *                                                                          *
 * DATA : 2 / 1 / 1996                                                      *
 ****************************************************************************/

#pragma once

#include "stdafx.h" 
#include "xskinbutton.h" 
#include "AdamoGlobal.h"

class CMDAxButton : public CxSkinButton
{
        int     m_nPushed;   
        int     m_nKeyboardPushed;
        CWnd*   m_pParent;
        bool    m_bCtrlOn;
    public :     
        CMDAxButton ();
        ~CMDAxButton ();
        inline void SetParent (CWnd* pParent); 
        inline void SetCtrlOn (bool b=true)   { m_bCtrlOn=b; }
        afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
        afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    protected :
        virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
        DECLARE_MESSAGE_MAP()
};

inline void CMDAxButton::SetParent (CWnd* pParent)
{
    m_pParent=pParent;
}


