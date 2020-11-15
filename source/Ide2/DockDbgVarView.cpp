/*
** DockOscilloscope.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "DockDbgVarView.h"
#include "AdamoDbgVarView.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* construction / destruction */
CDockDbgVarView::CDockDbgVarView () : m_pDbgVarView (NULL)
{
}

CDockDbgVarView::~CDockDbgVarView ()
{
}

/*
** Init : inizializza la docking che contiene le variabili.
*/
void CDockDbgVarView::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pDbgVarView = (CAdamoDbgVarView*) pPane->AttachView (pWnd, RUNTIME_CLASS(CAdamoDbgVarView));
}
