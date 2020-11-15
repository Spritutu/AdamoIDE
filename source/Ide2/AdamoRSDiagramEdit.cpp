/* ==========================================================================
	CAdamoRSDiagramEdit

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

IMPLEMENT_SERIAL(CAdamoRSDiagramEdit, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramEdit

CAdamoRSDiagramEdit::CAdamoRSDiagramEdit()
/* ============================================================
	Function :		CAdamoRSDiagramEdit::CAdamoRSDiagramEdit
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle("");
	SetType("editbox");
	SetName( "editbox" );
}

CDiagramEntity* CAdamoRSDiagramEdit::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramEdit::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramEdit* obj = new CAdamoRSDiagramEdit;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramEdit::Draw( CDC* dc, CRect rect )
/* ============================================================
	Function :		CDiagramEdit::Draw
	Description :	
					
	Description :	Draws the "control"
					
	Return :		void
	Parameters :	CDC* dc		-	CDC to draw to
					CRect rect	-	Total object rect (zoomed)
					
	Usage :			

   ============================================================*/
{

	CStdGrfx::drawsunkenframed3dWindow( dc, rect );

}

CDiagramEntity* CAdamoRSDiagramEdit::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramEdit::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramEdit* obj = new CAdamoRSDiagramEdit;
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
void CAdamoRSDiagramEdit::Select (BOOL selected)
{
    CAdamoRSDiagramEdit* pSelectedObj=NULL;
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
int CAdamoRSDiagramEdit::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSInputText*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramEdit::SaveTabIndex ()
{
    ((CAdamoRSInputText*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}
