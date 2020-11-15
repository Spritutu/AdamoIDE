/* ==========================================================================
	CAdamoRSDiagramILinearGauge

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

IMPLEMENT_SERIAL(CAdamoRSDiagramILinearGauge, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramILinearGauge

CAdamoRSDiagramILinearGauge::CAdamoRSDiagramILinearGauge() : m_pBmp (NULL)
/* ============================================================
	Function :		CAdamoRSDiagramILinearGauge::CAdamoRSDiagramILinearGauge
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
	m_pBmp = new CBitmap;
	m_pBmp->LoadBitmap (IDB_RS_DEVICE_LINEARGAUGE);
	SetMinimumSize( CSize( 16, 16 ) );
	SetTitle ("");
	SetType ("Linear Gauge" );
	SetName ("Linear Gauge" );
}

CAdamoRSDiagramILinearGauge::~CAdamoRSDiagramILinearGauge()
{
	_delete (m_pBmp);
}

CDiagramEntity* CAdamoRSDiagramILinearGauge::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramILinearGauge::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramILinearGauge* obj = new CAdamoRSDiagramILinearGauge;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramILinearGauge::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CAdamoRSDiagramILinearGauge::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{
	BITMAP bmp;
	if (m_pBmp && m_pBmp->GetSafeHandle () != NULL)   {
		m_pBmp->GetBitmap (&bmp);
		((CMainFrame *)AfxGetMainWnd ())->StretchBitmap (dc->GetSafeHdc (), *m_pBmp, (short)rect.left, (short)rect.top, (short)rect.Width (), (short) rect.Height (),
			0, 0, (short)bmp.bmWidth, (short)bmp.bmHeight);
	}
}

CDiagramEntity* CAdamoRSDiagramILinearGauge::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramILinearGauge::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramILinearGauge* obj = new CAdamoRSDiagramILinearGauge;
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
void CAdamoRSDiagramILinearGauge::Select (BOOL selected)
{
    CAdamoRSDiagramILinearGauge* pSelectedObj=NULL;
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
int CAdamoRSDiagramILinearGauge::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSDiagramILinearGauge*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramILinearGauge::SaveTabIndex ()
{
    ((CAdamoRSDiagramILinearGauge*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}
