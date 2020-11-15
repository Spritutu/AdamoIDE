/* ==========================================================================
	CAdamoRSDiagramCheckBox

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-03-31

	Purpose :		Encapsulates a "command button" as a CDiagramEntity 
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

IMPLEMENT_SERIAL(CAdamoRSDiagramCheckBox, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramCheckBox

CAdamoRSDiagramCheckBox::CAdamoRSDiagramCheckBox()
/* ============================================================
	Function :		CAdamoRSDiagramCheckBox::CAdamoRSDiagramCheckBox
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( "" );
	SetType( "checkbox" );
	SetName( "checkbox" );
}

CDiagramEntity* CAdamoRSDiagramCheckBox::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramCheckBox::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramCheckBox* obj = new CAdamoRSDiagramCheckBox;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramCheckBox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CAdamoRSDiagramCheckBox::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	dc->SelectObject( CStdGrfx::dialogBrush() );
	dc->SelectObject( CStdGrfx::dialogPen() );

	LOGFONT lf;
	CFont chk;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );
	font.CreateFontIndirect( &lf );

	// Marlett is used for the checkbox
	chk.CreateFont( ( int ) ( ( double ) lf.lfHeight * 1.25 ), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SelectObject( &chk );

	dc->Rectangle( rect );

	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor( ::GetSysColor( COLOR_WINDOW ) );
	dc->TextOut( rect.left, rect.top, "g" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DSHADOW ) );
	dc->TextOut( rect.left, rect.top, "d" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DHIGHLIGHT ) );
	dc->TextOut( rect.left, rect.top, "d" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DDKSHADOW ) );
	dc->TextOut( rect.left, rect.top, "e" );

	dc->SetTextColor( ::GetSysColor( COLOR_3DLIGHT ) );
	dc->TextOut( rect.left, rect.top, "f" );

	dc->SelectObject( &font );
	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );
	rect.left += ( int ) ( ( double ) abs( lf.lfHeight ) * 1.5 );
	dc->DrawText( GetTitle(), rect, DT_SINGLELINE );

	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SelectStockObject( NULL_PEN );
	dc->SelectStockObject( LTGRAY_BRUSH );

}

CDiagramEntity* CAdamoRSDiagramCheckBox::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramCheckBox::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramCheckBox* obj = new CAdamoRSDiagramCheckBox;
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
void CAdamoRSDiagramCheckBox::Select (BOOL selected)
{
    CAdamoRSDiagramCheckBox* pSelectedObj=NULL;
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
int CAdamoRSDiagramCheckBox::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSInputCheck*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramCheckBox::SaveTabIndex ()
{
    ((CAdamoRSInputCheck*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}
