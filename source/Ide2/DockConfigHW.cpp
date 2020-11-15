/*
** DockConfigHW.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "AdamoConfigHWView.h"
#include "DockConfigHW.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* construction / destruction */
CDockConfigHW::CDockConfigHW () : m_pConfigHWView (NULL)
{
}

CDockConfigHW::~CDockConfigHW ()
{
}

/*
** Init : inizializza la docking che contiene le finestre di read/write SDO
*/
void CDockConfigHW::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pConfigHWView = (CAdamoConfigHWView*) pPane->AttachView (pWnd, RUNTIME_CLASS(CAdamoConfigHWView));
}
