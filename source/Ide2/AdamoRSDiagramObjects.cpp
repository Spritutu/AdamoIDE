/*
** AdamoRSDiagramObjects.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "AdamoRSDiagramObjects.h"

/*
** SetRect :
*/
void CAdamoRSDiagramObjects::SetRect (CRect rect )           {
    if (m_pRSCtrl)   {
        m_pRSCtrl->GetObjStyle().SetRect (rect);
        UpdateSelected ();
    }
    CDiagramEntity::SetRect (rect);
}

/*
** SetRect :
*/
void CAdamoRSDiagramObjects::SetRect (double left, double top, double right, double bottom)
{
    CRect rc ((int)left, (int)top, (int)right, (int)bottom);
    if (m_pRSCtrl)   {
        m_pRSCtrl->GetObjStyle().SetRect (rc);
        UpdateSelected ();
    }
    CDiagramEntity::SetRect (left, top, right, bottom);
}

/*
** MoveRect :
*/
void CAdamoRSDiagramObjects::MoveRect (double x, double y)
{
    CRect rc;
    if (m_pRSCtrl)   {
        rc=m_pRSCtrl->GetObjStyle().GetRect ();
        rc.OffsetRect ((int)x-rc.left, (int)y-rc.top);
        m_pRSCtrl->GetObjStyle().SetRect (rc);
        UpdateSelected ();
    }
    CDiagramEntity::MoveRect (x, y);
}

/*
** SetLeft :
*/
void CAdamoRSDiagramObjects::SetLeft (double left)   {
    CRect rc;
    if (m_pRSCtrl)   {
        rc=m_pRSCtrl->GetObjStyle().GetRect ();
        rc.left=(int)left;
        m_pRSCtrl->GetObjStyle().SetRect (rc);
        UpdateSelected ();
    }
    CDiagramEntity::SetLeft (left);
}

/*
** SetRight :
*/
void CAdamoRSDiagramObjects::SetRight (double right)   {
    CRect rc;
    if (m_pRSCtrl)   {
        rc=m_pRSCtrl->GetObjStyle().GetRect ();
        rc.right=(int)right;
        m_pRSCtrl->GetObjStyle().SetRect (rc);
        UpdateSelected ();
    }
    CDiagramEntity::SetRight (right);
}

/*
** SetTop :
*/
void CAdamoRSDiagramObjects::SetTop (double top)   {
    CRect rc;
    if (m_pRSCtrl)   {
        rc=m_pRSCtrl->GetObjStyle().GetRect ();
        rc.top=(int)top;
        m_pRSCtrl->GetObjStyle().SetRect (rc);
        UpdateSelected ();
    }
    CDiagramEntity::SetTop (top);
}

/*
** SetBottom :
*/
void CAdamoRSDiagramObjects::SetBottom (double bottom)   {
    CRect rc;
    if (m_pRSCtrl)   {
        rc=m_pRSCtrl->GetObjStyle().GetRect ();
        rc.bottom=(int)bottom;
        m_pRSCtrl->GetObjStyle().SetRect (rc);
        UpdateSelected ();
    }
    CDiagramEntity::SetBottom (bottom);
}

/*
** UpdateSelected :
*/
void CAdamoRSDiagramObjects::UpdateSelected ()   {
    CAdamoRSProperties* pRSProp=((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ();
    if (pRSProp)
        pRSProp->UpdateSelected ();
}

/*
** DrawSelectionMarkers :
*/
void CAdamoRSDiagramObjects::DrawSelectionMarkers ( CDC* dc, CRect rect ) const
{
    UINT uStyle;

    if ((CDiagramEntity*)this==((CAdamoRSContainer*)GetParent())->GetSelectedObj ())
        uStyle=CRectTracker::hatchedBorder|CRectTracker::resizeInside;
    else
        uStyle=CRectTracker::resizeInside;
    CRectTracker rt (rect, uStyle);
    rt.Draw (dc);    
}
