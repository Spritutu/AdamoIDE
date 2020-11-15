/* ==========================================================================
	CAdamoRSDiagramCombo

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		Encapsulates a "combobox" as a CDiagramEntity 
					object.

	Description :	An example of a CDiagramEntity-derived class. Includes 
					an example of the Export function.

					The class uses a static class with GDI-helpers, 
					CStdGrfx, for drawing.

					The also class demonstrates custom size constraints.

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

IMPLEMENT_SERIAL(CAdamoRSDiagramCombo, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramCombo

CAdamoRSDiagramCombo::CAdamoRSDiagramCombo()
/* ============================================================
	Function :		CAdamoRSDiagramCombo::CAdamoRSDiagramCombo
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	// Note the constraints
	SetConstraints( CSize( 20, 20 ), CSize( -1, 20 ) );

	SetTitle( "" );
	SetType( "combobox" );
	SetName( "combobox" );
}

CDiagramEntity* CAdamoRSDiagramCombo::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramCombo::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramCombo* obj = new CAdamoRSDiagramCombo;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramCombo::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CAdamoRSDiagramCombo::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	CStdGrfx::drawsunkenframed3dWindow( dc, rect );

	LOGFONT lf;
	GetFont( lf );
	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

	CRect rectArrow( rect );
	rectArrow.left = rectArrow.right - ( int ) ( ( double ) GetMinimumSize().cy * GetZoom() );
	rectArrow.InflateRect( -2, -2 );

	CStdGrfx::drawframed3dBox( dc, rectArrow );

	CFont chk;
	chk.CreateFont( lf.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SelectObject( &chk );
	dc->DrawText( "u", rectArrow, DT_VCENTER | DT_SINGLELINE | DT_CENTER );

	dc->SelectStockObject( DEFAULT_GUI_FONT );

}

CDiagramEntity* CAdamoRSDiagramCombo::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramCombo::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramCombo* obj = new CAdamoRSDiagramCombo;
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
void CAdamoRSDiagramCombo::Select (BOOL selected)
{
    CAdamoRSDiagramCombo* pSelectedObj=NULL;
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
int CAdamoRSDiagramCombo::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSCombo*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramCombo::SaveTabIndex ()
{
    ((CAdamoRSCombo*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}
