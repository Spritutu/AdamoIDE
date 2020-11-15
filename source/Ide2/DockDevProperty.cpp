// DockDevProperty.cpp: implementation of the CDockDevProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DockDevProperty.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDockDevProperty::CDockDevProperty():m_pGridView(NULL)
{

}

CDockDevProperty::~CDockDevProperty()
{

}

void CDockDevProperty::Init(CWnd *pWnd, int nID)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nID);
    if (pPane)
        m_pGridView = (CGridView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CGridView));
}

void CDockDevProperty::SetObject(CObject *pObj)
{
    if(m_pGridView)
        m_pGridView->SetObject(pObj);
}

