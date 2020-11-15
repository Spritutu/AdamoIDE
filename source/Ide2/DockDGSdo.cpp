/*
** DockDGSdo.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "DGSdoView.h"
#include "DockDGSdo.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* construction / destruction */
CDockSGSDO::CDockSGSDO () : m_pDGSdoView (NULL)
{
}

CDockSGSDO::~CDockSGSDO ()
{
}

/*
** Init : inizializza la docking che contiene le finestre di read/write SDO
*/
void CDockSGSDO::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pDGSdoView = (CDGSdoView*) pPane->AttachView (pWnd, RUNTIME_CLASS(CDGSdoView));
}
