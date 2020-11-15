/*
** DockOscilloscope.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "OscView.h"
#include "DockOscilloscope.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* construction / destruction */
CDockOscilloscope::CDockOscilloscope () : m_pOscView (NULL), m_pDockingPane (NULL)
{
}

CDockOscilloscope::~CDockOscilloscope ()
{
}

/*
** Init : inizializza la docking che contiene l'oscilloscopio
*/
void CDockOscilloscope::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pOscView = (COscView*) pPane->AttachView (pWnd, RUNTIME_CLASS(COscView));
}
