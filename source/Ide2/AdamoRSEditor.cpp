// DEditor.cpp : implementation file
//

#include "stdafx.h"
#include "StdGrfx.h"
#include "MainFrame.h"
#include "AdamoRSEditor.h"
#include "AdamoRSDiagramObjects.h"
#include "AdamoRSProperties.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDEditor
void CAdamoRSEditor::DrawBackground( CDC* dc, CRect rect, double /*zoom*/ ) const
{

	CStdGrfx::drawframed3dBox( dc, rect );
}

void CAdamoRSEditor::DrawGrid( CDC* dc, CRect /*rect*/, double zoom ) const
{

	COLORREF gridcol = GetGridColor();

	dc->SelectStockObject( BLACK_PEN );

	int stepx = GetVirtualSize().cx / GetGridSize().cx;
	int stepy = GetVirtualSize().cy / GetGridSize().cy;

	for( int x = 0 ; x <= stepx ; x++ )
		for( int y = 0; y <= stepy ; y++ )
			dc->SetPixel( round( ( double ) ( GetGridSize().cx * x ) * zoom ), round( ( double ) ( GetGridSize().cy * y ) * zoom ), gridcol );
}

CDiagramEntity* CAdamoRSEditor::GetSelectedObject() const   {
    CAdamoRSContainer* pEC=(CAdamoRSContainer*) GetDiagramEntityContainer();
    if (pEC)   {
        CDiagramEntity* pE=pEC->GetSelectedObj ();
        if (pE)
            return pE;
    }
    return CDiagramEditor::GetSelectedObject ();
}

void CAdamoRSEditor::DrawSelectionMarkers( CDC* dc ) const
{
    CAdamoRSContainer* pEC=(CAdamoRSContainer*) GetDiagramEntityContainer();
    CAdamoRSForm* pForm=pEC->GetCtrlList ()->GetFormCtrl ();
    CAdamoRSProperties* pRSProp=((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ();
    if (pRSProp)   {
        CSize size=GetVirtualSize ();
        pForm->SetPositionAbs (0, 0, size.cx, size.cy);
        pRSProp->Select (pForm);
        pRSProp->UpdateSelected ();
    }
    CDiagramEditor::DrawSelectionMarkers (dc);
}

/*
** Add :
*/
void CAdamoRSContainer::Add (CDiagramEntity* obj)
{
    CAdamoRSControl* pCtrl=NULL;

    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramButton)))   {
        pCtrl=new CAdamoRSButton;
        ((CAdamoRSDiagramButton*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramEdit)))   {
        pCtrl=new CAdamoRSInputText;
        ((CAdamoRSDiagramEdit*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramRadioButton)))   {
        pCtrl=new CAdamoRSInputRadio;
        ((CAdamoRSDiagramRadioButton*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramCheckBox)))   {
        pCtrl=new CAdamoRSInputCheck;
        ((CAdamoRSDiagramCheckBox*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramGroupBox)))   {
        pCtrl=new CAdamoRSGroup;
        ((CAdamoRSDiagramGroupBox*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramLabel)))   {
        pCtrl=new CAdamoRSLabel;
        ((CAdamoRSDiagramLabel*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramList)))   {
        pCtrl=new CAdamoRSList;
        ((CAdamoRSDiagramList*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramCombo)))   {
        pCtrl=new CAdamoRSCombo;
        ((CAdamoRSDiagramCombo*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramImage)))   {
        pCtrl=new CAdamoRSImage;
        ((CAdamoRSDiagramImage*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramGrid)))   {
        pCtrl=new CAdamoRSGrid;
        ((CAdamoRSDiagramGrid*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramAx)))   {
        pCtrl=new CAdamoRSAx;
        ((CAdamoRSDiagramAx*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramLed)))   {
        pCtrl=new CAdamoRSILed;
        ((CAdamoRSDiagramLed*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramIAngularGauge)))   {
        pCtrl=new CAdamoRSIAngularGauge;
        ((CAdamoRSDiagramIAngularGauge*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramILinearGauge)))   {
        pCtrl=new CAdamoRSILinearGauge;
        ((CAdamoRSDiagramILinearGauge*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramISevenSegment)))   {
        pCtrl=new CAdamoRSIDisplay;
        ((CAdamoRSDiagramISevenSegment*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramILabel)))   {
        pCtrl=new CAdamoRSILabel;
        ((CAdamoRSDiagramILabel*)obj)->SetRSControl (pCtrl);
    }
    if (obj->IsKindOf (RUNTIME_CLASS (CAdamoRSDiagramPicture)))   {
        pCtrl=new CAdamoRSIPicture;
        ((CAdamoRSDiagramPicture*)obj)->SetRSControl (pCtrl);
    }
    if (pCtrl)   {
        pCtrl->SetID (m_pctrlList->GetValidID ());
        pCtrl->SetTabIndex (pCtrl->GetID ()-DEFAULT_RSCTRL_ID);
        pCtrl->SetCtrlList (m_pctrlList);
        m_pctrlList->AddCtrl (pCtrl);
        CAdamoRSProperties* pRSProp=((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ();
        if (pRSProp)
            pRSProp->Select (pCtrl);
    }
    CDiagramEntityContainer::Add (obj);
}

/*
** TabModified :
*/
void CAdamoRSEditor::TabModified ()
{
    GetDiagramEntityContainer ()->SetModified (true);
}

/*
** RemoveAt :
*/
void CAdamoRSContainer::RemoveAt (int index)
{
    CObject*obj=GetData()->GetAt (index);
    if (obj)   {
        CAdamoRSControl* pCtrl=((CAdamoRSDiagramObjects*)obj)->GetRSControl ();
        if (m_pctrlList)
            if (pCtrl)   {
                m_pctrlList->RemoveCtrl (pCtrl);
                CAdamoRSDiagramObjects* pDiagramObj=(CAdamoRSDiagramObjects*)GetSelectedObj ();
                if (pDiagramObj)
                    if (pCtrl==pDiagramObj->GetRSControl ())
                        ClearSelection ();
                    CAdamoRSProperties* pRSProp=((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ();
                    if (pRSProp)
                        pRSProp->ClearRSObject (pCtrl);
            }
    }
    CDiagramEntityContainer::RemoveAt (index);
}

/*
** RemoveAll :
*/
void CAdamoRSContainer::RemoveAll ()
{
    if (m_pctrlList)
        m_pctrlList->RemoveAllCtrl ();
    ClearSelection ();
    CDiagramEntityContainer::RemoveAll ();
}

/*
** SetCtrlList :
*/
void CAdamoRSContainer::SetCtrlList (CAdamoRSCtrlList* ctrlList)
{
    m_pctrlList=ctrlList;
}

/*
** GetCtrlList :
*/
CAdamoRSCtrlList* CAdamoRSContainer::GetCtrlList ()
{
    return m_pctrlList;
}

/*
** Load :
*/
void CAdamoRSContainer::Load ()
{
    for (POSITION pos=m_pctrlList->GetHeadPosition (); pos;)   {
        CAdamoRSControl* p=(CAdamoRSControl*) m_pctrlList->GetNext (pos);
        switch (p->KindOf ())   {
            case RSForm :
                break;
            case RSLabel :
                AddDiagramLabel (p);
                break;
            case RSInputText :
                AddDiagramEdit (p);
                break;
            case RSInputRadio :
                AddDiagramRadioButton (p);
                break;
            case RSInputCheck :
                AddDiagramCheckBox (p);
                break;
            case RSGroup :
                AddDiagramGroup (p);
                break;
            case RSButton :
                AddDiagramButton (p);
                break;
            case RSList :
                AddDiagramList (p);
                break;
            case RSCombo :
                AddDiagramCombo (p);
                break;
            case RSImage :
                AddDiagramImage (p);
                break;
            case RSGrid :
                AddDiagramGrid (p);
                break;
            case RSAx :
                AddDiagramAx (p);
                break;
            case RSILed :
                AddDiagramLed (p);
                break;
            case RSIAngularGauge :
                AddDiagramIAngularGauge (p);
                break;
            case RSILinearGauge :
                AddDiagramILinearGauge (p);
                break;
            case RSIDisplay :
                AddDiagramIDisplay (p);
                break;
            case RSILabel :
                AddDiagramILabel (p);
                break;
            case RSIPicture :
                AddDiagramIPicture (p);
                break;
        }
    }
}

/*
** AddDiagramButton :
*/
void CAdamoRSContainer::AddDiagramButton (CAdamoRSControl* p)
{
    CAdamoRSDiagramButton *pDB=new CAdamoRSDiagramButton;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetTitle (((CAdamoRSButton*)p)->GetValue ());
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
}

/*
** AddDiagramEdit :
*/
void CAdamoRSContainer::AddDiagramEdit (CAdamoRSControl* p)
{
    CAdamoRSDiagramEdit *pDB=new CAdamoRSDiagramEdit;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetTitle (((CAdamoRSInputText*)p)->GetValue ());
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
}

/*
** AddDiagramRadioButton :
*/
void CAdamoRSContainer::AddDiagramRadioButton (CAdamoRSControl* p)
{
    CAdamoRSDiagramRadioButton *pDB=new CAdamoRSDiagramRadioButton;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetTitle (((CAdamoRSInputRadio*)p)->GetValue ());
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
}

/*
** AddDiagramCheckBox :
*/
void CAdamoRSContainer::AddDiagramCheckBox (CAdamoRSControl* p)
{
    CAdamoRSDiagramCheckBox *pDB=new CAdamoRSDiagramCheckBox;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetTitle (((CAdamoRSInputCheck*)p)->GetValue ());
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
}

/*
** AddDiagramLabel :
*/
void CAdamoRSContainer::AddDiagramLabel (CAdamoRSControl* p)
{
    CAdamoRSDiagramLabel *pDB=new CAdamoRSDiagramLabel;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetTitle (((CAdamoRSLabel*)p)->GetValue ());
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
}

/*
** AddDiagramImage :
*/
void CAdamoRSContainer::AddDiagramImage (CAdamoRSControl* p)
{
    CAdamoRSDiagramImage *pDB=new CAdamoRSDiagramImage;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetTitle ("");
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    if (pTmpl)   {
        CAdamoRSDoc* pDoc=(CAdamoRSDoc*) pTmpl->GetDocument ("");
        if (pDoc)   {
            if (((CAdamoRSImage*)p)->GetIDImage ()!="")   {
                CxImage* pImage = pDoc->CaricaCxImage (((CAdamoRSImage*)p)->GetIDImage ());
                if (pImage)
                    pDB->SetImage (pImage);
                else
                    ((CAdamoRSImage*)p)->SetIDImage ("");
            }
        }
    }
}

/*
** AddDiagramGroup :
*/
void CAdamoRSContainer::AddDiagramGroup (CAdamoRSControl* p)
{
    CAdamoRSDiagramGroupBox *pDB=new CAdamoRSDiagramGroupBox;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetTitle (((CAdamoRSGroup*)p)->GetValue ());
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
}

/*
** AddDiagramCombo :
*/
void CAdamoRSContainer::AddDiagramCombo (CAdamoRSControl* p)
{
    CAdamoRSDiagramCombo *pDB=new CAdamoRSDiagramCombo;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
}

/*
** AddDiagramList :
*/
void CAdamoRSContainer::AddDiagramList (CAdamoRSControl* p)
{
    CAdamoRSDiagramList *pDB=new CAdamoRSDiagramList;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
}

/*
** AddDiagramGrid :
*/
void CAdamoRSContainer::AddDiagramGrid (CAdamoRSControl* p)
{
    CAdamoRSDiagramGrid *pDB=new CAdamoRSDiagramGrid;
    pDB->SetRect (p->GetObjStyle().GetRect());
    pDB->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDB);
    pDB->SetRSControl (p);
}

/*
** AddDiagramAx :
*/
void CAdamoRSContainer::AddDiagramAx (CAdamoRSControl* p)
{
    CAdamoRSDiagramAx *pDiagramAx=new CAdamoRSDiagramAx;
    pDiagramAx->SetRect (p->GetObjStyle().GetRect());
    pDiagramAx->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDiagramAx);
    pDiagramAx->SetRSControl (p);
}

/*
** AddDiagramLed :
*/
void CAdamoRSContainer::AddDiagramLed (CAdamoRSControl* p)
{
    CAdamoRSDiagramLed *pDiagramLed=new CAdamoRSDiagramLed;
    pDiagramLed->SetRect (p->GetObjStyle().GetRect());
    pDiagramLed->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDiagramLed);
    pDiagramLed->SetRSControl (p);
}

/*
** AddDiagramIAngularGauge :
*/
void CAdamoRSContainer::AddDiagramIAngularGauge (CAdamoRSControl* p)
{
    CAdamoRSDiagramIAngularGauge *pDiagramIAngularGauge=new CAdamoRSDiagramIAngularGauge;
    pDiagramIAngularGauge->SetRect (p->GetObjStyle().GetRect());
    pDiagramIAngularGauge->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDiagramIAngularGauge);
    pDiagramIAngularGauge->SetRSControl (p);
}

/*
** AddDiagramOutDig :
*/
void CAdamoRSContainer::AddDiagramILinearGauge (CAdamoRSControl* p)
{
    CAdamoRSDiagramILinearGauge *pDiagramILinearGauge=new CAdamoRSDiagramILinearGauge;
    pDiagramILinearGauge->SetRect (p->GetObjStyle().GetRect());
    pDiagramILinearGauge->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDiagramILinearGauge);
    pDiagramILinearGauge->SetRSControl (p);
}

/*
** AddDiagramIDisplay :
*/
void CAdamoRSContainer::AddDiagramIDisplay (CAdamoRSControl* p)
{
    CAdamoRSDiagramISevenSegment *pDiagramIDisplay=new CAdamoRSDiagramISevenSegment;
    pDiagramIDisplay->SetRect (p->GetObjStyle().GetRect());
    pDiagramIDisplay->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDiagramIDisplay);
    pDiagramIDisplay->SetRSControl (p);
}

/*
** AddDiagramILabel :
*/
void CAdamoRSContainer::AddDiagramILabel (CAdamoRSControl* p)
{
    CAdamoRSDiagramILabel *pDiagramILabel=new CAdamoRSDiagramILabel;
    pDiagramILabel->SetRect (p->GetObjStyle().GetRect());
    pDiagramILabel->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDiagramILabel);
    pDiagramILabel->SetRSControl (p);
}

/*
** AddDiagramIPicture :
*/
void CAdamoRSContainer::AddDiagramIPicture (CAdamoRSControl* p)
{
    CAdamoRSDiagramPicture *pDiagramPicture=new CAdamoRSDiagramPicture;
    pDiagramPicture->SetRect (p->GetObjStyle().GetRect());
    pDiagramPicture->SetTitle ("");
    pDiagramPicture->SetName (p->GetName ());
    CDiagramEntityContainer::Add (pDiagramPicture);
    pDiagramPicture->SetRSControl (p);
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    if (pTmpl)   {
        CAdamoRSDoc* pDoc=(CAdamoRSDoc*) pTmpl->GetDocument ("");
        if (pDoc)   {
            CString strImage = ((CAdamoRSIPicture*)p)->GetFirstIDImage ();
            if (strImage != "")   {
                CxImage* pImage = pDoc->CaricaCxImage (strImage);
                if (pImage)
                    pDiagramPicture->SetImage (pImage);
            }
            else
                ((CAdamoRSIPicture*)p)->ClearFirstImage ();
        }
    }
}

/*
** Front :
*/
void CAdamoRSContainer::Front (CDiagramEntity* obj)
{
    CAdamoRSControl *pRS=((CAdamoRSDiagramObjects*)obj)->GetRSControl ();
    
    if (DeleteControl (pRS))   {
        CDiagramEntityContainer::Front (obj);
        m_pctrlList->AddTail (pRS);
    }
}

/*
** Bottom :
*/
void CAdamoRSContainer::Bottom (CDiagramEntity* obj)
{
    CAdamoRSControl *pRS=((CAdamoRSDiagramObjects*)obj)->GetRSControl ();
    
    if (DeleteControl (pRS))   {
        CDiagramEntityContainer::Bottom (obj);
        m_pctrlList->AddHead (pRS);
    }
}

/*
** DeleteControl :
*/
bool CAdamoRSContainer::DeleteControl (CAdamoRSControl *pRS)
{
    bool bFound=false;

    for (POSITION pos=m_pctrlList->GetHeadPosition (), pos2; pos2=pos; )   {
        CAdamoRSControl* p=(CAdamoRSControl*) m_pctrlList->GetNext (pos);
        if (p==pRS)   {
            m_pctrlList->RemoveAt (pos2); bFound=true; break;
        }
    }
    return bFound;
}
