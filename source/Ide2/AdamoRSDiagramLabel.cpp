/* ==========================================================================
	CAdamoRSDiagramLabel

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

IMPLEMENT_SERIAL(CAdamoRSDiagramLabel, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramLabel

CAdamoRSDiagramLabel::CAdamoRSDiagramLabel()
/* ============================================================
	Function :		CAdamoRSDiagramLabel::CAdamoRSDiagramLabel
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( LOADSTRING (IDS_ADAMORSDIAGRAM_3) );
	SetType( "static" );
	SetName( "static" );
}

CDiagramEntity* CAdamoRSDiagramLabel::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramLabel::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramLabel* obj = new CAdamoRSDiagramLabel;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramLabel::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CAdamoRSDiagramLabel::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{
    UINT uStyle;

	dc->SelectObject( CStdGrfx::dialogBrush() );
	dc->SelectObject( CStdGrfx::dialogPen() );

	dc->Rectangle( rect );

	LOGFONT lf;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts

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

	dc->SetBkMode( TRANSPARENT );

	if (GetRSControl ())   {
        switch (GetRSControl ()->GetObjStyle().GetAlignment())   {
            case leftAlignment :
                uStyle=DT_SINGLELINE;
                break;
            case centerAlignment :
                uStyle=DT_SINGLELINE | DT_CENTER;
                break;
            case rigthAlignment :
                uStyle=DT_SINGLELINE | DT_RIGHT;
                break;
            default :
                uStyle=DT_SINGLELINE;
        };
    }
    else
        uStyle=DT_SINGLELINE;

    dc->DrawText( GetTitle(), rect, uStyle );

	dc->SelectStockObject( ANSI_VAR_FONT );
	dc->SelectStockObject( BLACK_PEN );
	dc->SelectStockObject( WHITE_BRUSH );

}

CDiagramEntity* CAdamoRSDiagramLabel::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramLabel::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramLabel* obj = new CAdamoRSDiagramLabel;
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
void CAdamoRSDiagramLabel::Select (BOOL selected)
{
    CAdamoRSDiagramLabel* pSelectedObj=NULL;
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
int CAdamoRSDiagramLabel::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSLabel*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramLabel::SaveTabIndex ()
{
    ((CAdamoRSLabel*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}
