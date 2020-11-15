// AdamoRSFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoRSFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSFrame

IMPLEMENT_DYNCREATE(CAdamoRSFrame, CMDIChildWnd)

CAdamoRSFrame::CAdamoRSFrame()
{
}

CAdamoRSFrame::~CAdamoRSFrame()
{
}


BEGIN_MESSAGE_MAP(CAdamoRSFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAdamoRSFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSFrame message handlers
void CAdamoRSFrame::ActivateFrame(int nCmdShow) 
{
	CMDIChildWnd::ActivateFrame(SW_MAXIMIZE);	
}
