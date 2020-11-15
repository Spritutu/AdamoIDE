/*
** DockOscilloscope.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "DockDbgIOView.h"
#include "AdamoDbgIOView.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* construction / destruction */
CDockDbgIOView::CDockDbgIOView () : m_pDbgIOView (NULL)
{
}

CDockDbgIOView::~CDockDbgIOView ()
{
}

/*
** Init : inizializza la docking che contiene l'oscilloscopio
*/
void CDockDbgIOView::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)
        m_pDbgIOView = (CAdamoDbgIOView*) pPane->AttachView (pWnd, RUNTIME_CLASS(CAdamoDbgIOView));
}
