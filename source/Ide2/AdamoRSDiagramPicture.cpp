/* ==========================================================================
	CAdamoRSDiagramPicture

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

IMPLEMENT_SERIAL(CAdamoRSDiagramPicture, CObject, 1)

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDiagramPicture

CAdamoRSDiagramPicture::CAdamoRSDiagramPicture() : m_pImage (NULL), m_bIsTransparent (false), m_rgbTransparentColor (0)
/* ============================================================
	Function :		CAdamoRSDiagramPicture::CAdamoRSDiagramPicture
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{

	SetMinimumSize( CSize( 1, 1 ) );
	SetTitle("");
	SetType("picture");
	SetName( "picture" );
}

CAdamoRSDiagramPicture::~CAdamoRSDiagramPicture()
/* ============================================================
	Function :		CAdamoRSDiagramPicture::CAdamoRSDiagramPicture
	Description :	constructor
					
	Return :		void
	Parameters :	none

	Usage :			

   ============================================================*/
{
    _delete (m_pImage);
}

CDiagramEntity* CAdamoRSDiagramPicture::Clone()
/* ============================================================
	Function :		CAdamoRSDiagramPicture::Clone
	Description :	Clones this object to a new one.
					
	Return :		CDiagramEntity*	-	The clone of the object.
	Parameters :	none

	Usage :			

   ============================================================*/
{
	CAdamoRSDiagramPicture* obj = new CAdamoRSDiagramPicture;
	obj->Copy( this );
	return obj;
}

void CAdamoRSDiagramPicture::Draw( CDC* dc, CRect rect )
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
        if (((CAdamoRSIPicture*)GetRSControl())->IsStretched ())
            m_pImage->Draw(dc->GetSafeHdc(), rect, 0, false);
        else   {
            if (((CAdamoRSIPicture*)GetRSControl())->IsTransparent ())   {
                COLORREF clr = ((CAdamoRSIPicture*)GetRSControl())->GetTransparentColor ();
                CBitmap bmp;
                bmp.Attach (m_pImage->MakeBitmap (dc->GetSafeHdc ()));
                ((CMainFrame *) AfxGetMainWnd ())->DrawTransparentBitmap (dc->GetSafeHdc (), (HBITMAP)bmp, (short)rect.left, (short)rect.top, clr);
            }
            else
                m_pImage->Draw(dc->GetSafeHdc(), rect.left, rect.top, m_pImage->GetWidth(), m_pImage->GetHeight(), 0, false);
        }
}

CDiagramEntity* CAdamoRSDiagramPicture::CreateFromString( const CString& str )
/* ============================================================
	Function :		CAdamoRSDiagramPicture::CreateFromString
	Description :	Static factory function
					
	Return :		CDiagramEntity*		-	An object of this 
											type if str is a 
											proper representation 
	Parameters :	const CString& str	-	The string to test
					
	Usage :			See DiagramControlFactory.cpp

   ============================================================*/
{

	CAdamoRSDiagramPicture* obj = new CAdamoRSDiagramPicture;
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
void CAdamoRSDiagramPicture::Select (BOOL selected)
{
    CAdamoRSDiagramPicture* pSelectedObj=NULL;
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
int CAdamoRSDiagramPicture::LoadTabIndex ()
{
    m_nTabIndex=((CAdamoRSIPicture*)GetRSControl())->GetTabIndex ()+1;
    return m_nTabIndex;
}

/*
** SaveTabIndex :
*/
void CAdamoRSDiagramPicture::SaveTabIndex ()
{
    ((CAdamoRSIPicture*)GetRSControl())->SetTabIndex (m_nTabIndex-1);
}

