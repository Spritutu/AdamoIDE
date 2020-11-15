// AdamoSEFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoSEFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEFrame

IMPLEMENT_DYNCREATE(CAdamoSEFrame, CMDIChildWnd)

CAdamoSEFrame::CAdamoSEFrame()
{
}

CAdamoSEFrame::~CAdamoSEFrame()
{
}


BEGIN_MESSAGE_MAP(CAdamoSEFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAdamoSEFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoSEFrame message handlers
void CAdamoSEFrame::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::ActivateFrame(SW_MAXIMIZE);	
}

