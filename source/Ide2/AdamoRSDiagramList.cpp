/* ==========================================================================
	CAdamoRSDiagramList

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

IMPLEMENT_SERIAL(CAdamoRSDiagramList, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramList

CAdamoRSDiagramList::CAdamoRSDiagramList()
/* ============================================================
	Function :		CAdamoRSDiagramList::CAdamoRSDiagramList
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 20, 20 ) );
	SetTitle( "" );
	SetType( "listbox" );
	SetName( "listbox" );
}

CDiagramEntity* CAdamoRSDiagramList::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramList::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramList* obj = new CAdamoRSDiagramList;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramList::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CAdamoRSDiagramList::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{
	CRect clientRect( rect );

	CStdGrfx::drawsunkenframed3dWindow( dc, rect );

	LOGFONT lf;
	GetFont( lf );
	dc->SetBkMode( TRANSPARENT );
	dc->SetTextColor( ::GetSysColor( COLOR_BTNTEXT ) );

	// Sidebar
	dc->SelectObject( CStdGrfx::scrollBrush() );
	dc->SelectObject( CStdGrfx::scrollPen() );
	int side = ( int ) ( ( double ) GetMinimumSize().cx * GetZoom() );

	rect.left = rect.right - side;
	rect.InflateRect( -2, -2 );
	dc->Rectangle( rect );

	CFont chk;
	chk.CreateFont( lf.lfHeight, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DECORATIVE, "Marlett" );
	dc->SelectObject( &chk );

	CRect rectTop( rect );
	CRect rectBottom( rect );
	rectTop.bottom = rectTop.top + rect.Width();
	rectBottom.top = rectBottom.bottom - rect.Width();

	int clientHeight = clientRect.Height() - 4;
	if( clientHeight < rectTop.Height() + rectBottom.Height() )
	{
		rectTop.bottom = rectTop.top + clientHeight / 2;
		rectBottom.top = rectTop.bottom;
	}


	CStdGrfx::drawframed3dBox( dc, rectTop );
	CStdGrfx::drawframed3dBox( dc, rectBottom );

	dc->DrawText( "u", rectTop, DT_VCENTER | DT_SINGLELINE | DT_CENTER );
	dc->DrawText( "u", rectBottom, DT_VCENTER | DT_SINGLELINE | DT_CENTER );

	dc->SelectStockObject( DEFAULT_GUI_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDiagramEntity* CAdamoRSDiagramList::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramList::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramList* obj = new CAdamoRSDiagramList;
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
void CAdamoRSDiagramList::Select (BOOL selected)
{
    CAdamoRSDiagramList* pSelectedObj=NULL;
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
int CAdamoRSDiagramList::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSList*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramList::SaveTabIndex ()
{
    ((CAdamoRSList*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}
