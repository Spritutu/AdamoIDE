// ImageResource.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoTemplates.h"
#include "ParEdit.h"
#include "imageresource.h"
#include "dlgimportimage.h"
#include "ximage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageResource

IMPLEMENT_DYNCREATE(CImageResource, CView)

CImageResource::CImageResource() : m_pDoc (NULL)
{
}

CImageResource::~CImageResource()
{
}


BEGIN_MESSAGE_MAP(CImageResource, CView)
	//{{AFX_MSG_MAP(CImageResource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageResource drawing

void CImageResource::OnDraw(CDC* pDC)
{
    CRect rc;

	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
    if (m_pDoc)   {
	    CxImage* ima = m_pDoc->GetImage(); 
	    if (ima) {
            GetWindowRect (rc);
            CenterImage (rc);
			ima->Draw(pDC->GetSafeHdc(), rc, 0, false);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// CImageResource diagnostics

#ifdef _DEBUG
void CImageResource::AssertValid() const
{
	CView::AssertValid();
}

void CImageResource::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

CString CImageResource::AddRsBimap ()
{
    CxImage pImg;

    CString str;
    DWORD dwLength;
    char* p;

    CDlgImportImage dlg;
    if (dlg.DoModal ()==IDOK)   {
        /* verificare se l'id inserito non esista gia nel data base */
        if (true)   {
            /* qui dobbiamo salvare nella base dati e visualizzare l'immagine */
            if (m_pDoc)   {
                if (m_pDoc->EsisteImmagine (dlg.GetID ()))
                    AfxMessageBox (LOADSTRING (IDS_ADAMOIMAGERESOURCE_1));
                else   {
                    if (LeggiFile (dlg.GetPath (), &p, &dwLength))   {
                        BeginWaitCursor ();
                        /* salviamo nel data base le immagini */
                        str=dlg.GetID ();
                        m_pDoc->AggiungiImmagine (str, p, dwLength);
                        /* memorizziamo nel documento l'immagine */
                        m_pDoc->SetImage (new CxImage ((BYTE*)p, dwLength, CXIMAGE_FORMAT_JPG));
                        /* rinfreschiamo il video */
                        delete p;
                        EndWaitCursor ();
                    }
                    else
                        AfxMessageBox (LOADSTRING (IDS_ADAMOIMAGERESOURCE_2));
                }
            }
        }
    }
    return str;
}

/*
** LeggiFile :
*/
bool CImageResource::LeggiFile (CPath strPath, char** pBuf, DWORD* dwLength)
{
    CFile f;
    char* p;
    DWORD dw;
    bool b=false;

    if (f.Open (strPath, CFile::typeBinary|CFile::modeRead))   {
        dw=(DWORD)f.GetLength ();
        p=new char [dw];
        if (p)   {
            f.Read (p, (UINT)dw);
            *dwLength=dw; *pBuf=p;
            b=true;
        }
        f.Close ();
    }
    return b;
}

/*
** DeleteRsBimap :
*/
bool CImageResource::DeleteRsBimap (CString strID)
{
    bool b=false;

    if (m_pDoc)   {
        if (m_pDoc->EsisteImmagine (strID))   {
            m_pDoc->CancellaImmagine (strID);
            b=true;
        }
    }
    return b;
}

/////////////////////////////////////////////////////////////////////////////
// CImageResource message handlers

void CImageResource::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    m_pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
}

/*
**
*/
void CImageResource::CenterImage (CRect& rcCentered)
{
    CRect rc;
    double fp;

    /* primo caso, l'immagine e' piu' piccola dell'area */
    CxImage* pImg=m_pDoc->GetImage ();
    if (pImg->GetWidth()<=(DWORD)rcCentered.Width()&&pImg->GetHeight()<=(DWORD)rcCentered.Height())   {
        rc.left=(int)((rcCentered.Width()-pImg->GetWidth())/2.0);
        rc.right=rc.left+pImg->GetWidth();
        rc.top=(int)((rcCentered.Height()-pImg->GetHeight())/2.0);
        rc.bottom=rc.top+pImg->GetHeight();
    }
    else
        /* secondo caso, sia le dimensioni orizzontali che verticali sono piu' grandi della client area */
        if (pImg->GetWidth()>(DWORD)rcCentered.Width()||pImg->GetHeight()>(DWORD)rcCentered.Height())   {
            double fpImgWidth=pImg->GetWidth(), fpImgHeigth=pImg->GetHeight ();
            double fpRCWidth=rcCentered.Width(), fpRCHeight=rcCentered.Height();
            double fpAsp1=fpImgWidth/fpImgHeigth;
            double fpAsp2=fpRCWidth/fpRCHeight;

            if (fpAsp1>fpAsp2)   {
                fp=fpRCWidth/fpImgWidth;
                rc.left=0;
                rc.right=rcCentered.Width();
                rc.top=(int)(((double)rcCentered.Height()-(fpImgHeigth*fp))/2.0);
                rc.bottom=(int)((double)rc.top+(fpImgHeigth*fp));
            }
            else   {
                fp=fpRCHeight/fpImgHeigth;
                rc.left=(int)(((double)rcCentered.Width()-(fpImgWidth*fp))/2.0);
                rc.right=(int)((double)rc.left+(fpImgWidth*fp));
                rc.top=0;
                rc.bottom=rcCentered.Height ();
            }
        }
    rcCentered=rc;
}

/*
** SelectRSBitmap :
*/
void CImageResource::SelectRSBitmap (CString strID)
{
    if (m_pDoc)   {
        CxImage* pImage=m_pDoc->CaricaCxImage (strID);
        if (pImage)
            m_pDoc->SetImage (pImage);
        else
            AfxMessageBox (LOADSTRING (IDS_ADAMOIMAGERESOURCE_2));
    }
}
