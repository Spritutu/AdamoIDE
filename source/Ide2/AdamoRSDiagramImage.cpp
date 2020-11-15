/* ==========================================================================
	CAdamoRSDiagramImage

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

IMPLEMENT_SERIAL(CAdamoRSDiagramImage, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramImage

CAdamoRSDiagramImage::CAdamoRSDiagramImage() : m_pImage (NULL), m_bIsTransparent (false), m_rgbTransparentColor (0)
/* ============================================================
	Function :		CAdamoRSDiagramImage::CAdamoRSDiagramImage
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle("");
	SetType("image");
	SetName( "image" );
}

CAdamoRSDiagramImage::~CAdamoRSDiagramImage()
/* ============================================================
	Function :		CAdamoRSDiagramImage::CAdamoRSDiagramImage
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
    _delete (m_pImage);
}

CDiagramEntity* CAdamoRSDiagramImage::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramImage::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramImage* obj = new CAdamoRSDiagramImage;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramImage::Draw( CDC* dc, CRect rect )
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
    if (m_pImage)
        if (((CAdamoRSImage*)GetRSControl())->IsStretched ())
            m_pImage->Draw(dc->GetSafeHdc(), rect, 0, false);
        else   {
            if (((CAdamoRSImage*)GetRSControl())->IsTransparent ())   {
                COLORREF clr = ((CAdamoRSImage*)GetRSControl())->GetTransparentColor ();
                CBitmap bmp;
                bmp.Attach (m_pImage->MakeBitmap (dc->GetSafeHdc ()));
                ((CMainFrame *) AfxGetMainWnd ())->DrawTransparentBitmap (dc->GetSafeHdc (), (HBITMAP)bmp, rect.left, rect.top, clr);
            }
            else
                m_pImage->Draw(dc->GetSafeHdc(), rect.left, rect.top, m_pImage->GetWidth(), m_pImage->GetHeight(), 0, false);
        }
}

CDiagramEntity* CAdamoRSDiagramImage::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramImage::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramImage* obj = new CAdamoRSDiagramImage;
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
void CAdamoRSDiagramImage::Select (BOOL selected)
{
    CAdamoRSDiagramImage* pSelectedObj=NULL;
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
int CAdamoRSDiagramImage::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSImage*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramImage::SaveTabIndex ()
{
    ((CAdamoRSImage*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}

