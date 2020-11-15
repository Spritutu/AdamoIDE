/* ==========================================================================
	CAdamoRSDiagramGroupBox

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

IMPLEMENT_SERIAL(CAdamoRSDiagramGroupBox, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramGroupBox

CAdamoRSDiagramGroupBox::CAdamoRSDiagramGroupBox()
/* ============================================================
	Function :		CAdamoRSDiagramGroupBox::CAdamoRSDiagramGroupBox
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( LOADSTRING (IDS_ADAMORSDIAGRAM_2) );
	SetType( "groupbox" );
	SetName( "groupbox" );
}

CDiagramEntity* CAdamoRSDiagramGroupBox::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramGroupBox::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramGroupBox* obj = new CAdamoRSDiagramGroupBox;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramGroupBox::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CAdamoRSDiagramGroupBox::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	CRect rect3d( rect );
	dc->SelectObject( CStdGrfx::dialogBrush() );
	dc->SelectObject( CStdGrfx::dialogPen() );

	rect.InflateRect( -2, 0 );

	LOGFONT lf;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	if( GetZoom() < 1 )
		lstrcpy( lf.lfFaceName, _T( "Arial" ) );

	rect3d.top += abs( lf.lfHeight ) / 2;

	CStdGrfx::drawsunken3dFrame( dc, rect3d );

	rect3d.InflateRect( -1, -1 );
	CStdGrfx::draw3dFrame( dc, rect3d );

    if (GetRSControl ())   {
        stRSFont& rf=GetRSControl ()->GetObjStyle().GetRSFont();
        strcpy (lf.lfFaceName, rf.strFaceName);
        lf.lfHeight=rf.nSize;
        lf.lfStrikeOut=rf.bStrikeOut;
        lf.lfUnderline=rf.bUnderLine;
        lf.lfWeight=rf.bBold ? FW_BOLD : FW_NORMAL;
        lf.lfItalic=rf.bItalic;
    }

	font.CreateFontIndirect( &lf );

	dc->SelectObject( &font );

	dc->SetBkMode( OPAQUE );
    COLORREF crf=GetSysColor (COLOR_3DFACE);
	dc->SetBkColor( crf );
	rect.left += abs( lf.lfHeight ) / 2;
	dc->DrawText( " " + GetTitle() + " ", rect, DT_SINGLELINE );

	dc->SelectStockObject( ANSI_VAR_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDiagramEntity* CAdamoRSDiagramGroupBox::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramGroupBox::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramGroupBox* obj = new CAdamoRSDiagramGroupBox;
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
void CAdamoRSDiagramGroupBox::Select (BOOL selected)
{
    CAdamoRSDiagramGroupBox* pSelectedObj=NULL;
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

