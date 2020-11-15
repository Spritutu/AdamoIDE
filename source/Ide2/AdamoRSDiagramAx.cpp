/* ==========================================================================
	CAdamoRSDiagramAx

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		Encapsulates a "static control" as a CDiagramEntity 
					object.

	Description :	An example of a CDiagramEntity-derived class. Includes 
					an example of the Export function.

					The class uses a static class with GDI-helpers, 
					CStdGrfx, for drawing.

	Usage :			

	Changes :		15/5  2004	Corrected Export to be const

   ========================================================================*/

#include "stdafx.h"
#include "mainFrame.h"
#include "DiagramEntity.h"
#include "DiagramEntityContainer.h"
#include "AdamoRSDiagramObjects.h"
#include "AdamoRSEditor.h"
#include "AdamoRSProperties.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_SERIAL(CAdamoRSDiagramAx, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramAx

CAdamoRSDiagramAx::CAdamoRSDiagramAx() : m_pBmp (NULL)
/* ============================================================
	Function :		CAdamoRSDiagramAx::CAdamoRSDiagramAx
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	BITMAP bmp;
	m_pBmp = new CBitmap;
	m_pBmp->LoadBitmap (IDB_RS_DEVICE_AX);
	m_pBmp->GetBitmap (&bmp);
	SetMinimumSize (CSize( bmp.bmWidth, bmp.bmHeight));
	SetMaximumSize (CSize( bmp.bmWidth, bmp.bmHeight));
	SetTitle ("");
	SetType ("Asse" );
	SetName ("Asse" );

}

CAdamoRSDiagramAx::~CAdamoRSDiagramAx()
{
	_delete (m_pBmp);
}

CDiagramEntity* CAdamoRSDiagramAx::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramAx::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramAx* obj = new CAdamoRSDiagramAx;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramAx::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CAdamoRSDiagramAx::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{
	if (m_pBmp && m_pBmp->GetSafeHandle () != NULL)
		((CMainFrame *)AfxGetMainWnd ())->DrawTransparentBitmap (dc->GetSafeHdc (), *m_pBmp, (short)rect.left, (short)rect.top, RGB (255, 0, 0));
}

CDiagramEntity* CAdamoRSDiagramAx::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramAx::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramAx* obj = new CAdamoRSDiagramAx;
	if(!obj->FromString( str ) )
	{
		delete obj;
		obj = NULL;
	}

	return obj;

}

/*
** Select :
*/
void CAdamoRSDiagramAx::Select (BOOL selected)
{
    CAdamoRSDiagramAx* pSelectedObj=NULL;
    bool bSettato=false;
    
    CAdamoRSContainer* pRSContainer=(CAdamoRSContainer*)GetParent ();
    if (pRSContainer)   {
        if (selected==TRUE)   {
            pSelectedObj=this; bSettato=true;
        }
        else
            if (pRSContainer->GetSelectedObj ()==this)   {
                pSelectedObj=NULL; bSettato=true;
            }
    }
    if (bSettato)   {
        pRSContainer->SetSelectedObj (pSelectedObj);
        CAdamoRSProperties* pRSProp=((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ();
        pRSProp->Select (pSelectedObj ? pSelectedObj->GetRSControl() : NULL);
    }
    CDiagramEntity::Select (selected);
}

/*
** LoadTabIndex :
*/
int CAdamoRSDiagramAx::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSDiagramAx*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramAx::SaveTabIndex ()
{
    ((CAdamoRSDiagramAx*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}
