/*
** DockRS.h
*/

/* include files */
#include <stdafx.h>
#include "mainframe.h"
#include "DockRS.h"

/* implementation */
CDockRSCatalogue::CDockRSCatalogue() : m_pRSCatalogue (NULL)
{
}

CDockRSCatalogue::~CDockRSCatalogue()
{
}

void CDockRSCatalogue::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pRSCatalogue = (CAdamoRSCatalogue*)pPane->AttachView(pWnd,RUNTIME_CLASS(CAdamoRSCatalogue));
}

CDockRSProperties::CDockRSProperties() : m_pRSProperties (NULL)
{
}

CDockRSProperties::~CDockRSProperties()
{
}

void CDockRSProperties::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pRSProperties = (CAdamoRSProperties*)pPane->AttachView (pWnd,RUNTIME_CLASS(CAdamoRSProperties));
}

CDockRSActiveXPreview::CDockRSActiveXPreview() : m_pRSActiveXPreview (NULL)
{
}

CDockRSActiveXPreview::~CDockRSActiveXPreview()
{
}

void CDockRSActiveXPreview::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pRSActiveXPreview = (CAdamoRSActiveXPreview*)pPane->AttachView (pWnd,RUNTIME_CLASS(CAdamoRSActiveXPreview));
}
