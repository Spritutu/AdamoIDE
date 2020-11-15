// DockLangView.cpp: implementation of the CDockLangView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mainframe.h"
#include "ListViewErrorsCustomColor.h"
#include "AdamoListErrors.h"
#include "DockListErrors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDockTabListErrors

/** CDockListAllErrors construction / destruction **/
CDockListAllErrors::CDockListAllErrors() : m_pListAllErrors (NULL)
{
}

CDockListAllErrors::~CDockListAllErrors()
{
}

void CDockListAllErrors::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)
		m_pListAllErrors = (CAdamoListAllErrors*)pPane->AttachView(pWnd,RUNTIME_CLASS(CAdamoListAllErrors));
}

CAdamoListAllErrors *CDockListAllErrors::GetView()
{ 
	return m_pListAllErrors;
}

/** CDockListInterpreterErrors construction / destruction **/
CDockListInterpreterErrors::CDockListInterpreterErrors() : m_pListInterpreterErrors (NULL)
{
}

CDockListInterpreterErrors::~CDockListInterpreterErrors()
{
}

void CDockListInterpreterErrors::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)
		m_pListInterpreterErrors = (CAdamoListInterpreterErrors*)pPane->AttachView(pWnd,RUNTIME_CLASS(CAdamoListInterpreterErrors));
}

CAdamoListInterpreterErrors *CDockListInterpreterErrors::GetView()
{ 
	return m_pListInterpreterErrors;
}

/** CDockListPlcErrors construction / destruction **/
CDockListPlcErrors::CDockListPlcErrors() : m_pListPlcErrors (NULL)
{
}

CDockListPlcErrors::~CDockListPlcErrors()
{
}

void CDockListPlcErrors::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)
		m_pListPlcErrors = (CAdamoListPLCErrors*)pPane->AttachView(pWnd,RUNTIME_CLASS(CAdamoListPLCErrors));
}

CAdamoListPLCErrors *CDockListPlcErrors::GetView()
{ 
	return m_pListPlcErrors;
}

/** CDockListISOErrors construction / destruction **/
CDockListISOErrors::CDockListISOErrors() : m_pListISOErrors (NULL)
{
}

CDockListISOErrors::~CDockListISOErrors()
{
}

void CDockListISOErrors::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)
		m_pListISOErrors = (CAdamoListISOErrors*)pPane->AttachView(pWnd,RUNTIME_CLASS(CAdamoListISOErrors));
}

CAdamoListISOErrors *CDockListISOErrors::GetView()
{ 
	return m_pListISOErrors;
}

/** CDockListISOErrors construction / destruction **/
CDockListHardwareErrors::CDockListHardwareErrors() : m_pListHardwareErrors (NULL)
{
}

CDockListHardwareErrors::~CDockListHardwareErrors()
{
}

void CDockListHardwareErrors::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)
		m_pListHardwareErrors = (CAdamoListHardwareErrors*)pPane->AttachView(pWnd,RUNTIME_CLASS(CAdamoListHardwareErrors));
}

CAdamoListHardwareErrors *CDockListHardwareErrors::GetView()
{ 
	return m_pListHardwareErrors;
}

/** CDockListMessages construction / destruction **/
CDockListMessages::CDockListMessages() : m_pListMessages (NULL)
{
}

CDockListMessages::~CDockListMessages()
{
}

void CDockListMessages::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
	if (pPane)
		m_pListMessages = (CAdamoListMessages*)pPane->AttachView(pWnd,RUNTIME_CLASS(CAdamoListMessages));
}

CAdamoListMessages *CDockListMessages::GetView()
{ 
	return m_pListMessages;
}
