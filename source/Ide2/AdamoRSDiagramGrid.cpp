/* ==========================================================================
	CAdamoRSDiagramGrid

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

IMPLEMENT_SERIAL(CAdamoRSDiagramGrid, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramGrid

CAdamoRSDiagramGrid::CAdamoRSDiagramGrid()
/* ============================================================
	Function :		CAdamoRSDiagramGrid::CAdamoRSDiagramGrid
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle( LOADSTRING (IDS_ADAMORSDIAGRAM_3) );
	SetType( "grid" );
	SetName( "grid" );
}

CDiagramEntity* CAdamoRSDiagramGrid::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramGrid::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramGrid* obj = new CAdamoRSDiagramGrid;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramGrid::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CAdamoRSDiagramGrid::Draw
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{
	CString str;
	stGridColumnData gcd;
	CRect clientRect( rect );
	CRect rectCell;
	int nRows, nCols;
	int nSingleCelWidth, nSingleCelHeigh, i;
	bool bDrawAndExit = false;

	CStdGrfx::drawdoublesunken3dFrame( dc, rect );
	((CAdamoRSGrid *)GetRSControl ())->GetGridData (&nRows, &nCols);
	nSingleCelWidth = (rect.Width ()-2) / (nCols+1);
	nSingleCelHeigh = 20;
	rectCell.left = rect.left + 2;
	rectCell.right = rect.left + nSingleCelWidth + 2;
	rectCell.top = rect.top + 2;
	rectCell.bottom = rect.top + 20 + 2;
	if (rectCell.bottom > rect.bottom)
		rectCell.bottom = rect.bottom;

	LOGFONT lf;
	CFont font;
	GetFont( lf );
	// MS Sans Serif will not scale below 8 pts
	font.CreateFontIndirect( &lf );
	dc->SelectObject( &font );
	dc->SetBkMode (TRANSPARENT);
	for (i = 0; i<nCols + 1; i++)   {
		CStdGrfx::draw3dFrame( dc, rectCell );
		if (i>0)   {
			((CAdamoRSGrid *)GetRSControl ())->GetColumnData (i-1, &gcd);
			dc->DrawText (gcd.m_strName, rectCell, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}
		rectCell.left += nSingleCelWidth;
		rectCell.right += nSingleCelWidth;
	}
	rectCell.left = rect.left + 2;
	rectCell.right = rect.left + nSingleCelWidth + 2;
	rectCell.top = rectCell.top + 20;
	rectCell.bottom = rectCell.top + 20;
	if (rectCell.top < rect.bottom - 1)   {
		if (rectCell.bottom > rect.bottom)
			rectCell.bottom = rect.bottom;
		for (i = 0; i<nRows; i++)   {
			CStdGrfx::draw3dFrame( dc, rectCell );
			str.Format ("%d", i+1);
			dc->DrawText (str, rectCell, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
			if (bDrawAndExit) break;
			rectCell.top += 20;
			rectCell.bottom += 20;
			if (rectCell.top >= rect.bottom - 1)
				break;
			else
			if (rectCell.bottom >= rect.bottom - 1)   {
				rectCell.bottom = rect.bottom - 1;
				bDrawAndExit = true;
			}
		}
	}
}

CDiagramEntity* CAdamoRSDiagramGrid::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramGrid::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramGrid* obj = new CAdamoRSDiagramGrid;
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
void CAdamoRSDiagramGrid::Select (BOOL selected)
{
    CAdamoRSDiagramGrid* pSelectedObj=NULL;
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
int CAdamoRSDiagramGrid::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSLabel*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramGrid::SaveTabIndex ()
{
    ((CAdamoRSLabel*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}
