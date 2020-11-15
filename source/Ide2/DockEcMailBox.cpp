/*
** DockEcmailBox.cpp
*/

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "resource.h"
#include "DGEcMailBoxView.h"
#include "DockEcMailBox.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* construction / destruction */
CDockEcMailBox::CDockEcMailBox () : m_pDGEcMailBoxView (NULL)
{
}

CDockEcMailBox::~CDockEcMailBox ()
{
}

/*
** Init : inizializza la docking che contiene le finestre di read/write SDO
*/
void CDockEcMailBox::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pDGEcMailBoxView = (CDGEcMailBoxView*) pPane->AttachView (pWnd, RUNTIME_CLASS(CDGEcMailBoxView));
}
