#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "DockDbgObjectView.h"
#include "AdamoDbgObjectView.h"

CDockDbgObjectView::CDockDbgObjectView(void) : m_pDbgObjectView (NULL), m_pDockingPane (NULL)
{
}

CDockDbgObjectView::~CDockDbgObjectView(void)
{
}

/*
** Init : inizializza la docking che contiene le variabili.
*/
void CDockDbgObjectView::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pDbgObjectView = (CAdamoDbgObjectView*) pPane->AttachView (pWnd, RUNTIME_CLASS(CAdamoDbgObjectView));
}
