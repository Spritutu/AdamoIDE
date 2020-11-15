// DockDevice.cpp: implementation of the CDockDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DockDevice.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDockDevice::CDockDevice():m_pDevListView(NULL)
{

}

CDockDevice::~CDockDevice()
{

}

void CDockDevice::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pDevListView = (CDockDevListView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CDockDevListView));
}
