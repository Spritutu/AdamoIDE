// AdamoSynage.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynPage.h"
#include "StdGrfx.h"

// CAdamoSynPage
extern CString GetXMLProperty (const char* sz, const char* szProp);
extern COLORREF TranslateRGB (char* p);

IMPLEMENT_DYNAMIC(CAdamoSynPage, CWnd)

CAdamoSynPage::CAdamoSynPage() : m_bBackgroundEnabled (false), m_bKilling (false)
{

}

CAdamoSynPage::~CAdamoSynPage()
{
	m_bKilling = true;
	ClearCtrls ();
}

BEGIN_MESSAGE_MAP(CAdamoSynPage, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_CONTROL_RANGE (BN_CLICKED, 1000, 5000, OnClick)
	ON_CONTROL_RANGE (LBN_SELCHANGE, 1000, 5000, OnSelChange)
END_MESSAGE_MAP()

// CAdamoSynPage message handlers
int CAdamoSynPage::Load (CString strXML)
{
	if (!LoadXMLCtrls (strXML))
		;
	return 0;
}

int CAdamoSynPage::LoadXMLCtrls (CString strXML)
{
    HRESULT hr = m_pDocXml.CreateInstance( __uuidof(MSXML2::DOMDocument60));
    if (SUCCEEDED(hr))   {
        m_pDocXml->async = false;
        if (m_pDocXml->loadXML(strXML.AllocSysString ()))   {
            ParseXMLString ();
            hr=S_OK;
        }
        m_pDocXml.Release ();
    }
    return hr == S_OK ? 0 : -1;
}

int CAdamoSynPage::ParseXMLString ()
{
    ElementXmlPtr  p=m_pDocXml->documentElement;
	NodeListXmlPtr pRootChild;

    if (p->hasChildNodes())	  {
        pRootChild = p->childNodes;
        COleVariant v=pRootChild->Getitem (0)->nodeName;
        if (CString (v.bstrVal) == "form")   {
            v=((ElementXmlPtr)pRootChild->Getitem(0))->getAttribute("name");
            if (v.vt==VT_BSTR)   {
                CString strFormName=CString (v.bstrVal);
                CreateFormXML (pRootChild->Getitem (0));
                CreateXMLControls (pRootChild->Getitem (0));
				CreateWndCtrls ();
            }
        }
    }
	return 0;
}

/*
** CreateFormXML :
*/
int CAdamoSynPage::CreateFormXML (ElementXmlPtr p)
{
	COleVariant v;
	CString strFormName, strFormPosition, strImageName;

    v=p->getAttribute("name");
    if (v.vt==VT_BSTR)   {
        strFormPosition=CString (v.bstrVal);
		SetFormPosition (strFormPosition);
	}
    v=p->getAttribute("formname");
    if (v.vt==VT_BSTR)   {
        strFormName=CString (v.bstrVal);
		SetName (strFormName);
	}
    v=p->getAttribute("src");
    if (v.vt==VT_BSTR)   {
        strImageName = CString (v.bstrVal);
		SetBackgroundImage (strImageName);
	}
    return 0;
}

/*
** CreateXMLControls :
*/
int CAdamoSynPage::CreateXMLControls (ElementXmlPtr p)
{
	NodeListXmlPtr pRootChild;
    int nB=-1;

    if (p->hasChildNodes())	  {
        pRootChild = p->childNodes;
        int n=pRootChild->length, i=0;
        while (n>0)   {
            COleVariant v=pRootChild->Getitem (i)->nodeName;
            CString str=CString (v.bstrVal);
            if (str=="input")
                nB=CreateXMLInput (pRootChild->Getitem (i));
            else
            if (str=="select")
                nB=CreateXMLComboList (pRootChild->Getitem (i));
            else
            if (str=="fieldset")
                nB=CreateXMLGroupBox (pRootChild->Getitem (i));
            else
            if (str=="button")
                nB=CreateXMLButton (pRootChild->Getitem (i));
            else
            if (str=="label")
                nB=CreateXMLLabel (pRootChild->Getitem (i));
            else
            if (str=="img")
                nB=CreateXMLInputImage (pRootChild->Getitem (i));
			else
            if (str=="grid")
                nB=CreateXMLGrid (pRootChild->Getitem (i));
			else
            if (str=="ax")
                nB=CreateXMLAx (pRootChild->Getitem (i));
			else
            if (str=="led")
                nB=CreateXMLLed (pRootChild->Getitem (i));
			else
            if (str=="angulargauge")
                nB=CreateXMLAngularGauge (pRootChild->Getitem (i));
			else
            if (str=="lineargauge")
                nB=CreateXMLLinearGauge (pRootChild->Getitem (i));
			else
            if (str=="display")
                nB=CreateXMLDisplay (pRootChild->Getitem (i));
			else
            if (str=="static")
                nB=CreateXMLILabel (pRootChild->Getitem (i));
            else
            if (str=="picture")
                nB=CreateXMLIPicture (pRootChild->Getitem (i));
            n--; i++;
        }
    }
    return nB;
}

/*
** CreateXMLButton :
*/
int CAdamoSynPage::CreateXMLButton (ElementXmlPtr p)
{
    CAdamoSynButton *pBtn=new CAdamoSynButton;
    COleVariant v=p->getAttribute("name");
    pBtn->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pBtn->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pBtn->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pBtn->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pBtn->GetObjStyle().Load (CString (v.bstrVal));
    v=p->getAttribute("src");
    pBtn->SetButtonImage (CString (v.bstrVal));
	v=p->getAttribute("type");
	CString str = CString (v.bstrVal);
	if (str == "standard")
		pBtn->SetButtonType (eStandard);
	else
	if (str == "bitmap")
		pBtn->SetButtonType (eBitmap);
	v=p->getAttribute("transparency");
	str = CString (v.bstrVal);
	pBtn->SetTransparency (atoi ((const char *)str) != 0);
	v=p->getAttribute("action");
	str = CString (v.bstrVal);
	DWORD dwAction = atoi ((const char *)str);
	if (bittest (dwAction, 0))
		pBtn->EnableDownloadAction ();
	if (bittest (dwAction, 1))
		pBtn->EnableLoadAction ();
	if (bittest (dwAction, 2))
		pBtn->EnableSaveAction ();
	if (bittest (dwAction, 3))
		pBtn->EnableChangePageAction ();
	v=p->getAttribute("nextpage");
	pBtn->SetNextPage (CString (v.bstrVal));
	pBtn->SetParentWnd (this);
    AddCtrl (pBtn);
    return 0;
}

/*
** CreateXMLGroupBox :
*/
int CAdamoSynPage::CreateXMLGroupBox (ElementXmlPtr p)
{
    CAdamoSynGroupBox *pGB=new CAdamoSynGroupBox;
    COleVariant v=p->getAttribute("id");
    pGB->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pGB->GetObjStyle().Load (CString (v.bstrVal));
    if (p->hasChildNodes())   {
        ElementXmlPtr pLegend = p->childNodes->Getitem (0);
        COleVariant v=pLegend->nodeName;
        if (CString (v.bstrVal)=="legend")   {
            v=pLegend->text;
            pGB->SetValue (CString (v.bstrVal));
        }
    }
	pGB->SetParentWnd (this);
    AddCtrl (pGB);
    return 0;
}

/*
** CreateXMLLabel :
*/
int CAdamoSynPage::CreateXMLLabel (ElementXmlPtr p)
{
    CAdamoSynLabel *pIT=new CAdamoSynLabel;
    COleVariant v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pIT->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
	pIT->SetParentWnd (this);
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLGrid :
*/
int CAdamoSynPage::CreateXMLGrid (ElementXmlPtr p)
{
	int nRows, nCols;
	char *pC;

    CAdamoSynGrid *pGrid=new CAdamoSynGrid;
    COleVariant v=p->getAttribute("name");
    pGrid->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pGrid->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pGrid->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("rows");
	nRows = strtol (CString (v.bstrVal), &pC, 10);
    v=p->getAttribute("cols");
	nCols = strtol (CString (v.bstrVal), &pC, 10);
	pGrid->SetGridData (nRows, nCols);
    v=p->getAttribute("style");
    pGrid->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLColumns (p, pGrid);
    LoadXMLVariable (p, &pGrid->GetObjVar ());
	pGrid->SetParentWnd (this);
    AddCtrl (pGrid);
	return 0;
}

/*
** CreateXMLAx :
*/
int CAdamoSynPage::CreateXMLAx (ElementXmlPtr p)
{
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CAdamoMachine *pMachine;
    CAdamoSynRTAx *pAx=new CAdamoSynRTAx;
    COleVariant v=p->getAttribute("name");
    pAx->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pAx->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pAx->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pAx->GetObjStyle().Load (CString (v.bstrVal));
    v=p->getAttribute("path");
    pAx->SetPath (CString (v.bstrVal));
    v=p->getAttribute("address");
    pAx->SetID (atoi (CString (v.bstrVal)));
	pAx->SetParentWnd (this);
	pMachine = pSynContainer->GetMachine ();
	pAx->SetMachine (pMachine);
    AddCtrl (pAx);
	return 0;
}

/*
** CreateXMLLed :
*/
int CAdamoSynPage::CreateXMLLed (ElementXmlPtr p)
{
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CAdamoMachine *pMachine;
    CAdamoSynILed *pLed=new CAdamoSynILed;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nData, nLogicAddress;

    COleVariant v=p->getAttribute("name");
    pLed->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pLed->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pLed->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
	pLed->SetPath (strPath);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
	pLed->SetDeviceType (eDev);
    v=p->getAttribute("address");
    nLogicAddress = atoi (CString (v.bstrVal));
	pLed->SetLogicAddress (nLogicAddress);
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pLed->SetData (nData);
	/* general */
    v=p->getAttribute("L_G_Type");
	pLed->SetLedType ((eLedType)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_Transparent");
    pLed->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("L_G_ShowReflection");
    pLed->SetShowReflection (atoi (CString (v.bstrVal)) != 0);
    v=p->getAttribute("L_G_ActiveColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLed->SetActiveColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("L_G_BevelStyle");
	pLed->SetBevelStyle ((eLedStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_A_SpacingHorizontal");
    pLed->SetHorizontalSpacing (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_A_SpacingVertical");
    pLed->SetVerticalSpacing (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_A_RowCount");
    pLed->SetRowCount (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_A_ColCount");
    pLed->SetColCount (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_A_BorderStyle");
	pLed->SetBorderStyle ((eLedStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_A_BackgroundColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLed->SetBackgroundColor (TranslateRGB (pColor + 1));
	v=p->getAttribute("style");
    pLed->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, &pLed->GetObjVar ());
	pLed->SetParentWnd (this);
	pMachine = pSynContainer->GetMachine ();
	pLed->SetMachine (pMachine);
    AddCtrl (pLed);
	return 0;
}

/*
** CreateXMLAngularGauge :
*/
int CAdamoSynPage::CreateXMLAngularGauge (ElementXmlPtr p)
{
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CAdamoMachine *pMachine;
    CAdamoSynIAngularGauge *pAngGauge=new CAdamoSynIAngularGauge;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nLogicAddress, nData;

    COleVariant v=p->getAttribute("name");
    pAngGauge->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pAngGauge->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pAngGauge->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
	pAngGauge->SetPath (strPath);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
	pAngGauge->SetDeviceType (eDev);
    v=p->getAttribute("address");
    nLogicAddress = atoi (CString (v.bstrVal));
	pAngGauge->SetLogicAddress (nLogicAddress);
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pAngGauge->SetData (nData);
	/* caratteristiche generali */
    v=p->getAttribute("G_G_Transparent");
	pAngGauge->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_G_PosMin");
	pAngGauge->SetPosMin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_G_PosMax");
	pAngGauge->SetPosMax (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_G_ShowInArcR");
	pAngGauge->ShowInnerRadius (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_G_ShowOutArcR");
	pAngGauge->ShowOuterRadius (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_G_RevScale");
	pAngGauge->SetReverseScale (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_G_BorderStyle");
	pAngGauge->SetBorderStyle ((eBorderStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_G_BackColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetBackGroundColor (TranslateRGB (pColor + 1));
	/* pointers */
    v=p->getAttribute("G_P_Type");
	pAngGauge->SetPointerStyle ((ePointerStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_P_Size");
	pAngGauge->SetPointerSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_P_Margin");
	pAngGauge->SetPointerMargin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_P_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetPointerColor (TranslateRGB (pColor + 1));
	/* arc */
    v=p->getAttribute("G_A_Radius");
	pAngGauge->SetArcRadius (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_A_Angle");
	pAngGauge->SetArcAngle (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_A_StartAngle");
	pAngGauge->SetArcStartAngle (atoi (CString (v.bstrVal)));
	/* hub */
    v=p->getAttribute("G_H_Show");
	pAngGauge->ShowHub (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_H_Show3D");
	pAngGauge->ShowHub3D (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_H_Size");
	pAngGauge->SetHubSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_H_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetHubColor (TranslateRGB (pColor + 1));
	/* face */
    v=p->getAttribute("G_F_Show");
	pAngGauge->ShowFace (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_F_Type");
	pAngGauge->SetFaceStyle ((eStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_B_Type");
	pAngGauge->SetBevelStyle ((eStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_B_Size");
	pAngGauge->SetBevelSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_F_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetFaceColor (TranslateRGB (pColor + 1));
	/* tick */
    v=p->getAttribute("G_T_Margin");
	pAngGauge->SetTickMargin (atoi (CString (v.bstrVal)));
	/* tick label */
    v=p->getAttribute("G_TL_Margin");
	pAngGauge->SetTickLabelMargin (atoi (CString (v.bstrVal)));
	/* tick major */
    v=p->getAttribute("G_TMA_Show");
	pAngGauge->ShowTickMajor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_TMA_Number");
	pAngGauge->SetTickMajorNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_TMA_Lenght");
	pAngGauge->SetTickMajorLenght (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_TMA_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetTickMajorColor (TranslateRGB (pColor + 1));
	/* tick minor */
    v=p->getAttribute("G_TMI_Show");
	pAngGauge->ShowTickMinor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("G_TMI_Number");
	pAngGauge->SetTickMinorNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_TMI_Lenght");
	pAngGauge->SetTickMinorLenght (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_TMI_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetTickMinorColor (TranslateRGB (pColor + 1));
	/* sections */
    v=p->getAttribute("G_S_Count");
	pAngGauge->SetSectionsNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_S_Color1");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection1Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_Color2");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection2Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_Color3");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection3Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_Color4");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection4Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_Color5");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pAngGauge->SetSection5Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("G_S_End1");
	pAngGauge->SetEndSection1 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_S_End2");
	pAngGauge->SetEndSection2 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_S_End3");
	pAngGauge->SetEndSection3 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("G_S_End4");
	pAngGauge->SetEndSection4 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pAngGauge->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, &pAngGauge->GetObjVar ());
	pAngGauge->SetParentWnd (this);
	pMachine = pSynContainer->GetMachine ();
	pAngGauge->SetMachine (pMachine);
    AddCtrl (pAngGauge);
	return 0;
}

/*
** CreateXMLLinearGauge :
*/
int CAdamoSynPage::CreateXMLLinearGauge (ElementXmlPtr p)
{
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CAdamoMachine *pMachine;
    CAdamoSynILinearGauge *pLinGauge=new CAdamoSynILinearGauge;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nData, nLogicAddress;

    COleVariant v=p->getAttribute("name");
    pLinGauge->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pLinGauge->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pLinGauge->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    pLinGauge->SetPath (strPath);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
	pLinGauge->SetDeviceType (eDev);
    v=p->getAttribute("address");
    nLogicAddress = atoi (CString (v.bstrVal));
	pLinGauge->SetLogicAddress (nLogicAddress);
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pLinGauge->SetData (nData);
	/* general */
    v=p->getAttribute("LI_G_Transparent");
	pLinGauge->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_G_PosMin");
	pLinGauge->SetPosMin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_PosMax");
	pLinGauge->SetPosMax (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_RevScale");
	pLinGauge->SetReverseScale (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_G_Orientation");
	pLinGauge->SetOrientation ((eOrientation)atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_OrientationTicks");
	pLinGauge->SetOrientationTicks ((eOrientationTicks)atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_BorderStyle");
	pLinGauge->SetBorderStyle ((eLinearGaugeStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_G_BackColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetBackGroundColor (TranslateRGB (pColor + 1));
	/* pointers */
    v=p->getAttribute("LI_P_Style");
	pLinGauge->SetPointerStyle ((eLinearGaugePointerStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_P_Size");
	pLinGauge->SetPointerSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_P_Margin");
	pLinGauge->SetPointerOffset (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_P_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetPointerColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_P_3D");
	pLinGauge->SetPointer3D (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_P_DrawScaleSide");
	pLinGauge->SetDrawScaleSide (atoi (CString (v.bstrVal))!=0);
	/* ticks */
    v=p->getAttribute("LI_T_Show");
	pLinGauge->ShowTickAxes (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_T_Margin");
	pLinGauge->SetTickMargin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_T_ShowLabels");
	pLinGauge->ShowLabels (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_T_LabelMargin");
	pLinGauge->SetLabelMargin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMA_Show");
	pLinGauge->ShowTickMajor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_TMA_Number");
	pLinGauge->SetTickMajorNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMA_Lenght");
	pLinGauge->SetTickMajorLenght (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMA_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetTickMajorColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_TMI_Show");
	pLinGauge->ShowTickMinor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("LI_TMI_Number");
	pLinGauge->SetTickMinorNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMI_Lenght");
	pLinGauge->SetTickMinorLenght (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_TMI_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetTickMinorColor (TranslateRGB (pColor + 1));
	/* sections */
    v=p->getAttribute("LI_S_Count");
	pLinGauge->SetSectionsNumber (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_S_Color1");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection1Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_Color2");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection2Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_Color3");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection3Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_Color4");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection4Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_Color5");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLinGauge->SetSection5Color (TranslateRGB (pColor + 1));
    v=p->getAttribute("LI_S_End1");
	pLinGauge->SetEndSection1 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_S_End2");
	pLinGauge->SetEndSection2 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_S_End3");
	pLinGauge->SetEndSection3 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("LI_S_End4");
	pLinGauge->SetEndSection4 (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pLinGauge->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, &pLinGauge->GetObjVar ());
	pLinGauge->SetParentWnd (this);
	pMachine = pSynContainer->GetMachine ();
	pLinGauge->SetMachine (pMachine);
    AddCtrl (pLinGauge);
	return 0;
}

/*
** CreateXMLDisplay :
*/
int CAdamoSynPage::CreateXMLDisplay (ElementXmlPtr p)
{
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CAdamoMachine *pMachine;
    CAdamoSynIDisplay *pDisplay=new CAdamoSynIDisplay;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nData, nLogicAddress;

    COleVariant v=p->getAttribute("name");
    pDisplay->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pDisplay->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pDisplay->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    pDisplay->SetPath (strPath);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
	pDisplay->SetDeviceType (eDev);
    v=p->getAttribute("address");
    nLogicAddress = atoi (CString (v.bstrVal));
	pDisplay->SetLogicAddress (nLogicAddress);
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pDisplay->SetData (nData);
	/* general */
    v=p->getAttribute("D_G_ShowOffsement");
	pDisplay->ShowOffSegment (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("D_G_Transparent");
	pDisplay->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("D_G_ShowSign");
	pDisplay->ShowSign (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("D_G_AutosegmentColor");
	pDisplay->SetAutoSegmentOffColor (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("D_G_Precision");
	pDisplay->SetPrecision (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_G_BorderStyle");
	pDisplay->SetDisplayStyle ((eDisplayStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_G_LeadingStyle");
	pDisplay->SetLeadingStyle ((eDisplayLeadingStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_G_BackColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pDisplay->SetBackGroundColor (TranslateRGB (pColor + 1));
	/* display */
    v=p->getAttribute("D_D_Count");
	pDisplay->SetDisplayCount (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Spacing");
	pDisplay->SetDisplaySpacing (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Size");
	pDisplay->SetDisplaySize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Separation");
	pDisplay->SetDisplaySeparation (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Margin");
	pDisplay->SetDisplayMargin (atoi (CString (v.bstrVal)));
    v=p->getAttribute("D_D_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pDisplay->SetDisplayColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("D_D_OffColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pDisplay->SetDisplayOffColor (TranslateRGB (pColor + 1));
	v=p->getAttribute("style");
    pDisplay->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, &pDisplay->GetObjVar ());
	pDisplay->SetParentWnd (this);
	pMachine = pSynContainer->GetMachine ();
	pDisplay->SetMachine (pMachine);
    AddCtrl (pDisplay);
	return 0;
}

/*
** CreateXMLILabel :
*/
int CAdamoSynPage::CreateXMLILabel (ElementXmlPtr p)
{
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CAdamoMachine *pMachine;
    CAdamoSynILabel *pLabel=new CAdamoSynILabel;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nData, nLogicAddress;

    COleVariant v=p->getAttribute("name");
    pLabel->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pLabel->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pLabel->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    pLabel->SetPath (strPath);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
	pLabel->SetDeviceType (eDev);
    v=p->getAttribute("address");
    nLogicAddress = atoi (CString (v.bstrVal));
	pLabel->SetLogicAddress (nLogicAddress);
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pLabel->SetData (nData);
    v=p->getAttribute("value");
    pLabel->SetValue (CString (v.bstrVal));
	/* general */
    v=p->getAttribute("L_G_Transparent");
	pLabel->SetTransparent (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("L_G_AutoSize");
	pLabel->SetAutoSize (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("L_G_BorderStyle");
	pLabel->SetStyle ((eLabelStyle)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_Alignment");
	pLabel->SetAlignment ((eLabelAlignment)atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_Shadow");
	pLabel->ShowShadow (atoi (CString (v.bstrVal))!=0);
    v=p->getAttribute("L_G_OffsetX");
	pLabel->SetOffsetX (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_OffsetY");
	pLabel->SetOffsetY (atoi (CString (v.bstrVal)));
    v=p->getAttribute("L_G_Color");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLabel->SetColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("L_G_BackColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLabel->SetBackColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("L_G_ShadowColor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pLabel->SetShadowColor (TranslateRGB (pColor + 1));
	v=p->getAttribute("style");
    pLabel->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, &pLabel->GetObjVar ());
	pLabel->SetParentWnd (this);
	pMachine = pSynContainer->GetMachine ();
	pLabel->SetMachine (pMachine);
    AddCtrl (pLabel);
	return 0;
}

/*
** CreateXMLILabel :
*/
int CAdamoSynPage::CreateXMLIPicture (ElementXmlPtr p)
{
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CAdamoMachine *pMachine;
    CAdamoSynIPicture *pPicture=new CAdamoSynIPicture;
	CString strPath, str;
	enDispositivi eDev;
	char *pColor;
	int nData, nLogicAddress;

    COleVariant v=p->getAttribute("name");
    pPicture->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pPicture->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pPicture->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("path");
    strPath = CString (v.bstrVal);
    pPicture->SetPath (strPath);
    v=p->getAttribute("device");
    eDev = (enDispositivi)atoi (CString (v.bstrVal));
	pPicture->SetDeviceType (eDev);
    v=p->getAttribute("address");
    nLogicAddress = atoi (CString (v.bstrVal));
	pPicture->SetLogicAddress (nLogicAddress);
    v=p->getAttribute("data");
    nData = atoi (CString (v.bstrVal));
	pPicture->SetData (nData);
    v=p->getAttribute("checked");
    pPicture->SetStretch (CString (v.bstrVal)=="true");
    v=p->getAttribute("transparent");
    pPicture->SetTransparent (CString (v.bstrVal)=="true");
    v=p->getAttribute("transparentcolor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pPicture->SetTransparentColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("picture1");
    pPicture->SetImageID (0, CString (v.bstrVal));
    v=p->getAttribute("picture2");
    pPicture->SetImageID (1, CString (v.bstrVal));
    v=p->getAttribute("picture3");
    pPicture->SetImageID (2, CString (v.bstrVal));
    v=p->getAttribute("picture4");
    pPicture->SetImageID (3, CString (v.bstrVal));
    v=p->getAttribute("picture5");
    pPicture->SetImageID (4, CString (v.bstrVal));
    v=p->getAttribute("picture6");
    pPicture->SetImageID (5, CString (v.bstrVal));
    v=p->getAttribute("picture7");
    pPicture->SetImageID (6, CString (v.bstrVal));
    v=p->getAttribute("picture8");
    pPicture->SetImageID (7, CString (v.bstrVal));
    v=p->getAttribute("style");
    pPicture->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLVariable (p, &pPicture->GetObjVar ());
	pPicture->SetParentWnd (this);
	pMachine = pSynContainer->GetMachine ();
	pPicture->SetMachine (pMachine);
    AddCtrl (pPicture);
	return 0;
}

/*
** AddCtrl :
*/
void CAdamoSynPage::AddCtrl (CAdamoSynCtrl* pCtrl)
{
    /* se il controllo che dobbiamo inserire e' un gruppo, inseriamolo per primo */
    if (pCtrl->KindOf()==RSGroup)
        m_listCtrls.AddHead (pCtrl);
    else
        m_listCtrls.AddTail (pCtrl);
}

/*
** LoadXMLColumns :
*/
int CAdamoSynPage::LoadXMLColumns (ElementXmlPtr p, CAdamoSynGrid *pCtrl)
{
	NodeListXmlPtr pRootChild;
    ElementXmlPtr  pChild;
    COleVariant v;
	CString strType, strImage;
	stGridColumnData gcd;

    if (p->hasChildNodes())   {
        pRootChild = p->childNodes;
        int n=pRootChild->length, i=0;
        while (n>0)   {
            pChild=pRootChild->Getitem (i);
            v=pChild->nodeName;
            if (CString (v.bstrVal)=="column")   {
                v=pChild->getAttribute("type");
				strType = CString (v.bstrVal);
				if (strType == "number")
					gcd.m_nType = LUA_TNUMBER;
				else
				if (strType == "boolean")
					gcd.m_nType = LUA_TBOOLEAN;
				else
				if (strType == "string")
					gcd.m_nType = LUA_TSTRING;
                v=pChild->getAttribute("image");
				strImage = CString (v.bstrVal);
                if (v.vt!=VT_NULL)
                    gcd.m_strImage = CString (v.bstrVal);
                v=pChild->text;
                gcd.m_strName=CString (v.bstrVal);
				pCtrl->SetColumnData (i, &gcd);
            }
            n--; i++;
        }
    }
    return 0;
}

/*
** LoadXMLVariable :
*/
int CAdamoSynPage::LoadXMLVariable (ElementXmlPtr p, CAdamoRSVarControl *pCtrl)
{
	NodeListXmlPtr pRootChild;

    if (p->hasChildNodes())   {
        pRootChild = p->childNodes;
        int n=pRootChild->length, i=0;
        while (n>0)   {
            ElementXmlPtr pVar = p->childNodes->Getitem (i);
            COleVariant v=pVar->nodeName;
            if (CString (v.bstrVal)=="Variable")   {
                v=pVar->getAttribute("VariableName");
                if (v.vt==VT_BSTR)   {
                    /* e' una variabile semplice */
                    pCtrl->GetObjVar ().m_strName=CString (v.bstrVal);
                    v=pVar->getAttribute("VariableIndex");
                    pCtrl->GetObjVar ().Load (CString (v.bstrVal));
                }
                v=pVar->getAttribute("Table");
                if (v.vt==VT_BSTR)   {
                    /* e' una tabella */
                    pCtrl->GetObjVar ().m_strTable=CString (v.bstrVal);
                    v=pVar->getAttribute("Field");
					if (v.vt==VT_BSTR)
						pCtrl->GetObjVar ().m_strField=CString (v.bstrVal);
                    v=pVar->getAttribute("KeyName");
					if (v.vt==VT_BSTR)
						pCtrl->GetObjVar ().m_strKeyName=CString (v.bstrVal);
                    v=pVar->getAttribute("KeyValue");
					if (v.vt==VT_BSTR)
						pCtrl->GetObjVar ().m_strKeyValue=CString (v.bstrVal);
                }
            }
            n--; i++;
        }
    }
    return 0;
}

/*
** SetFormPosition :
*/
void CAdamoSynPage::SetFormPosition (CString strFormPosition)
{
    CString str=GetXMLProperty (strFormPosition, "left");
    str.TrimRight ("px");
    m_rc.left=atoi (str);
    str=GetXMLProperty (strFormPosition, "top");
    str.TrimRight ("px");
    m_rc.top=atoi (str);
    str=GetXMLProperty (strFormPosition, "width");
    str.TrimRight ("px");
    m_rc.right=m_rc.left+atoi (str);
    str=GetXMLProperty (strFormPosition, "height");
    str.TrimRight ("px");
    m_rc.bottom=m_rc.top+atoi (str);
}

/*
** SetBackgroundImage :
*/
void CAdamoSynPage::SetBackgroundImage (CString strImageName)
{
	m_strImageName = strImageName;
	if (m_strImageName != "")
		m_bBackgroundEnabled = true;
}

/*
** Resize :
*/
void CAdamoSynPage::Resize ()
{
	MoveWindow (m_rc.left, m_rc.top, m_rc.Width (), m_rc.Height (), true);
	CreateBackground ();
}

/*
** OnPaint :
*/
void CAdamoSynPage::OnPaint() 
{
	CPaintDC dc (this);

	CRect rect;
	GetClientRect( &rect );
	CStdGrfx::draw3dFrame( &dc, rect );
}

/*
** CreateBackground :
*/
int CAdamoSynPage::CreateBackground ()
{
	COLORREF m_nonClientBkgndCol = ::GetSysColor( COLOR_3DFACE );
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	BITMAP	csBitmapSize;

	if (pSynContainer && m_bBackgroundEnabled)   {
		pSynContainer->CaricaImmagine (m_strImageName, &m_hBckGround);
		::GetObject(m_hBckGround, sizeof(csBitmapSize), &csBitmapSize);
		m_dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_dwHeight = (DWORD)csBitmapSize.bmHeight;
	}
	return 0;
}

/*
** CreateWndCtrls :
*/
int CAdamoSynPage::CreateWndCtrls ()
{
	for (POSITION pos = m_listCtrls.GetHeadPosition (); pos; )   {
		CAdamoSynCtrl *pCtrl = (CAdamoSynCtrl *) m_listCtrls.GetNext (pos);
		switch (pCtrl->KindOf ())   {
			case RSButton :
				((CAdamoSynButton *) pCtrl)->Create ();
				break;
			case RSInputText :
				((CAdamoSynEdit *) pCtrl)->Create ();
				break;
			case RSInputCheck :
				((CAdamoSynCheckBox *) pCtrl)->Create ();
				break;
			case RSInputRadio :
				((CAdamoSynRadioButton *) pCtrl)->Create ();
				break;
			case RSGrid :
				((CAdamoSynGrid *) pCtrl)->Create ();
				break;
			case RSGroup :
				((CAdamoSynGroupBox *) pCtrl)->Create ();
				break;
			case RSLabel :
				((CAdamoSynLabel *) pCtrl)->Create ();
				break;
			case RSList :
				((CAdamoSynListBox *) pCtrl)->Create ();
				break;
			case RSCombo :
				((CAdamoSynComboBox *) pCtrl)->Create ();
				break;
			case RSImage :
				((CAdamoSynImage *) pCtrl)->Create ();
                break;
			case RSAx :
				((CAdamoSynRTAx *) pCtrl)->Create ();
				break;
			case RSILed :
				((CAdamoSynILed *) pCtrl)->Create ();
				break;
			case RSIAngularGauge :
				((CAdamoSynIAngularGauge *) pCtrl)->Create ();
				break;
			case RSILinearGauge :
				((CAdamoSynILinearGauge *) pCtrl)->Create ();
				break;
			case RSIDisplay :
				((CAdamoSynIDisplay *) pCtrl)->Create ();
				break;
			case RSILabel :
				((CAdamoSynILabel *) pCtrl)->Create ();
				break;
			case RSIPicture :
				((CAdamoSynIPicture *) pCtrl)->Create ();
				break;
		}
	}
	return 0;
}

/*
** OnClick :
*/
void CAdamoSynPage::OnClick (UINT nID)
{
	POSITION pos;
	bool bReturn = false;

	/* andiamo a cercare il bottone che ha cliccato */
	for (pos = m_listCtrls.GetHeadPosition (); pos; )   {
		CAdamoSynCtrl *pCtrl = (CAdamoSynCtrl *) m_listCtrls.GetNext (pos);
		if (pCtrl->GetID () == nID)   {
			switch (pCtrl->KindOf ())   {
				case RSButton :
					if (GestioneButtonClick (pCtrl))
						bReturn = true;
					break;
				case RSInputCheck :
					GestioneCheckBoxClick (pCtrl);
					break;
				case RSInputRadio :
					GestioneRadioButtonClick (pCtrl);
					break;
			}
		}
		if (bReturn)
			break;
	}
}

/*
** OnSelChange :
*/
void CAdamoSynPage::OnSelChange (UINT nID)
{
	POSITION pos;
	bool bReturn = false;

	/* andiamo a cercare il bottone che ha cliccato */
	for (pos = m_listCtrls.GetHeadPosition (); pos; )   {
		CAdamoSynCtrl *pCtrl = (CAdamoSynCtrl *) m_listCtrls.GetNext (pos);
		if (pCtrl->GetID () == nID)   {
			switch (pCtrl->KindOf ())   {
				case RSList :
					GestioneListBoxSelection (pCtrl);
					break;
				case RSCombo :
					GestioneComboBoxSelection (pCtrl);
					break;
			}
		}
		if (bReturn)
			break;
	}
}

/*
** GestioneListBoxSelection :
*/
void CAdamoSynPage::GestioneListBoxSelection (CAdamoSynCtrl* pCtrl)
{
	CAdamoSynListBox *pListBox = (CAdamoSynListBox *) pCtrl;
	if (pListBox->IsAutoUpdate ())
		pListBox->OnDownload ();
}

/*
** GestioneComboBoxSelection :
*/
void CAdamoSynPage::GestioneComboBoxSelection (CAdamoSynCtrl* pCtrl)
{
	CAdamoSynComboBox *pComboBox = (CAdamoSynComboBox *) pCtrl;
	if (pComboBox->IsAutoUpdate ())
		pComboBox->OnDownload ();
}

/*
** GestioneButtonClick :
*/
int CAdamoSynPage::GestioneButtonClick (CAdamoSynCtrl *pCtrl)
{
	CAdamoSynButton *pBtn = (CAdamoSynButton *) pCtrl;
	int nR = 0;
	for (POSITION pos = m_listCtrls.GetHeadPosition (); pos; )   {
		CAdamoSynCtrl *pCtrl = (CAdamoSynCtrl *) m_listCtrls.GetNext (pos);
		if (pBtn->IsLoadActionEnabled ())
			pCtrl->OnLoad ();
		if (pBtn->IsSaveActionEnabled ())
 			pCtrl->OnSave ();
	}
	if (pBtn->IsDownloadActionEnabled ())
		OnDownload ();
	if (pBtn->IsChangePageActionEnabled ())   {
		OnChangePage (pBtn->GetNextPage ());
		nR = -1;
	}
	return nR;
}

/*
** GestioneCheckBoxClick :
*/
int CAdamoSynPage::GestioneCheckBoxClick (CAdamoSynCtrl *pCtrl)
{
	CAdamoSynCheckBox *pCheck = (CAdamoSynCheckBox *) pCtrl;
	pCheck->SetCheck (!pCheck->GetCheck ());
	if (pCheck->IsAutoUpdate ())
		pCheck->OnDownload ();
	return 0;
}

/*
** GestioneCheckBoxClick :
*/
int CAdamoSynPage::GestioneRadioButtonClick (CAdamoSynCtrl *pCtrl)
{
	CAdamoSynRadioButton *pRadio = (CAdamoSynRadioButton *) pCtrl;
	pRadio->SetCheck (TRUE);
	if (pRadio->IsAutoUpdate ())
		pRadio->OnDownload ();
	UnCheckRadioButton (pRadio);
	return 0;
}

/*
** UnCheckRadioButton :
*/
void CAdamoSynPage::UnCheckRadioButton (CAdamoSynCtrl *pRadioChecked)
{
	for (POSITION pos = m_listCtrls.GetHeadPosition (); pos; )   {
		CAdamoSynCtrl *pCtrl = (CAdamoSynCtrl *) m_listCtrls.GetNext (pos);
		if (pCtrl->KindOf () == RSInputRadio)   {
			CAdamoSynRadioButton *pRadio = (CAdamoSynRadioButton *) pCtrl;
			if (pRadio->GetID () != pRadioChecked->GetID ())   {
				if (pRadio->GetName () == pRadioChecked->GetName ())   {
					pRadio->SetCheck (FALSE);
					if (pRadio->IsAutoUpdate ())
						pRadio->OnDownload ();
				}
			}
		}
	}
}

/*
** OnDownload :
*/
void CAdamoSynPage::OnDownload ()
{
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	pSynContainer->OnDownload ();
}

/*
** CreateXMLInput :
*/
int CAdamoSynPage::CreateXMLInput (ElementXmlPtr p)
{
    COleVariant v=p->getAttribute("type");
    if (v.vt==VT_BSTR)   {
        CString strType=CString (v.bstrVal);
		/*
        if (strType=="password")
            return CreateXMLInputText (p);
        else
		*/
        if (strType=="text")
            return CreateXMLInputText (p);
        else
        if (strType=="radio")
            return CreateXMLInputRadio (p);
        else
        if (strType=="checkbox")
            return CreateXMLInputCheckBox (p);
    }
	return 0;
}

/*
** CreateXMLInputCheckBox :
*/
int CAdamoSynPage::CreateXMLInputCheckBox (ElementXmlPtr p)
{
    CAdamoSynCheckBox *pCheck=new CAdamoSynCheckBox;
    COleVariant v=p->getAttribute("name");
    pCheck->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pCheck->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pCheck->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pCheck->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pCheck->GetObjStyle().Load (CString (v.bstrVal));
    v=p->getAttribute("checked");
    pCheck->SetChecked (CString (v.bstrVal)=="true");
    v=p->getAttribute("autoupdate");
    pCheck->SetAutoUpdate(CString (v.bstrVal)=="true");
    LoadXMLVariable (p, &pCheck->GetObjVar ());
	pCheck->SetParentWnd (this);
    AddCtrl (pCheck);
	return 0;
}

/*
** CreateXMLInputImage :
*/
int CAdamoSynPage::CreateXMLInputImage (ElementXmlPtr p)
{
    CString str;
	char *pColor;

    CAdamoSynImage *pIT=new CAdamoSynImage;
    COleVariant v=p->getAttribute("src");
    pIT->SetIDImage (CString (v.bstrVal));
    v=p->getAttribute("checked");
    pIT->SetStretch (CString (v.bstrVal)=="true");
    v=p->getAttribute("transparent");
    pIT->SetTransparent (CString (v.bstrVal)=="true");
    v=p->getAttribute("transparentcolor");
	str = CString (v.bstrVal);
	pColor = str.GetBuffer ();
	if (pColor[0] == '#')
		pIT->SetTransparentColor (TranslateRGB (pColor + 1));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
	pIT->SetParentWnd (this);
    AddCtrl (pIT);
	return 0;
}

/*
** CreateXMLInputRadio :
*/
int CAdamoSynPage::CreateXMLInputRadio (ElementXmlPtr p)
{
    CAdamoSynRadioButton *pRadio=new CAdamoSynRadioButton;
    COleVariant v=p->getAttribute("name");
    pRadio->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pRadio->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pRadio->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pRadio->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pRadio->GetObjStyle().Load (CString (v.bstrVal));
    v=p->getAttribute("group");
    pRadio->SetGroup (CString (v.bstrVal));
    v=p->getAttribute("checked");
    pRadio->SetChecked (CString (v.bstrVal)=="true");
    v=p->getAttribute("autoupdate");
    pRadio->SetAutoUpdate(CString (v.bstrVal)=="true");
    LoadXMLVariable (p, &pRadio->GetObjVar ());
	pRadio->SetParentWnd (this);
    AddCtrl (pRadio);
	return 0;
}

/*
** ClearCtrls :
*/
void CAdamoSynPage::ClearCtrls ()
{
	CAdamoSynCtrl* pCtrl;

	for (POSITION pos = m_listCtrls.GetHeadPosition (); pos; )   {
		pCtrl = (CAdamoSynCtrl*) m_listCtrls.GetNext (pos);
		delete pCtrl;
	}
	m_listCtrls.RemoveAll ();
}

/*
** OnChangePage :
*/
void CAdamoSynPage::OnChangePage (CString strPage)
{
	if (strPage != "" && strPage != m_strName)   {
		CAdamoSynContainer *pSynContainer = GETSYNCONT();
		pSynContainer->LoadPage (strPage);
	}
}

/*
** InitialLoad :
*/
void CAdamoSynPage::OnLoad ()
{
	for (POSITION pos = m_listCtrls.GetHeadPosition (); pos; )   {
		CAdamoSynCtrl *pCtrl = (CAdamoSynCtrl *) m_listCtrls.GetNext (pos);
		pCtrl->OnLoad ();
	}
}

/*
** CreateXMLInputText :
*/
int CAdamoSynPage::CreateXMLInputText (ElementXmlPtr p)
{
	eEditType e = eLetters;
    CAdamoSynEdit *pEdit=new CAdamoSynEdit;
    COleVariant v=p->getAttribute("name");
    pEdit->SetName (CString (v.bstrVal));
    v=p->getAttribute("value");
    pEdit->SetValue (CString (v.bstrVal));
    v=p->getAttribute("id");
    pEdit->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pEdit->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pEdit->GetObjStyle().Load (CString (v.bstrVal));
    v=p->getAttribute("autoupdate");
    pEdit->SetAutoUpdate(CString (v.bstrVal)=="true");
    LoadXMLVariable (p, &pEdit->GetObjVar ());
	v=p->getAttribute("mode");
	CString str = CString (v.bstrVal);
	if (str == "numbers")
		e = eNumbers;
	else
	if (str == "float")
		e = eFloat;
	else
	if (str == "hex")
		e = eHex;
	else
	if (str == "letters")
		e = eLetters;
	pEdit->SetType (e);
	pEdit->SetParentWnd (this);
    AddCtrl (pEdit);
	return 0;
}

/*
** CreateXMLComboList :
*/
int CAdamoSynPage::CreateXMLComboList (ElementXmlPtr p)
{
    COleVariant v=p->getAttribute("size");
    int nSize=atoi (CString (v.bstrVal));
    if (nSize==1)
        CreateXMLCombo (p);
    else
        CreateXMLList (p);
    return 0;
}

/*
** CreateXMLCombo :
*/
int CAdamoSynPage::CreateXMLCombo (ElementXmlPtr p)
{
    CAdamoSynComboBox *pIT=new CAdamoSynComboBox;
    COleVariant v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLOptions (p, pIT, true);
    LoadXMLVariable (p, &pIT->GetObjVar ());
	pIT->SetParentWnd (this);
    AddCtrl (pIT);
    return 0;
}

/*
** CreateXMLList :
*/
int CAdamoSynPage::CreateXMLList (ElementXmlPtr p)
{
    CAdamoSynListBox *pIT=new CAdamoSynListBox;
    COleVariant v=p->getAttribute("name");
    pIT->SetName (CString (v.bstrVal));
    v=p->getAttribute("id");
    pIT->SetID (atoi (CString (v.bstrVal)));
    v=p->getAttribute("tabindex");
    pIT->SetTabIndex (atoi (CString (v.bstrVal)));
    v=p->getAttribute("size");
    pIT->SetSize (atoi (CString (v.bstrVal)));
    v=p->getAttribute("style");
    pIT->GetObjStyle().Load (CString (v.bstrVal));
    LoadXMLOptions (p, pIT, false);
    LoadXMLVariable (p, &pIT->GetObjVar ());
	pIT->SetParentWnd (this);
    AddCtrl (pIT);
    return 0;
}

/*
** LoadXMLOptions :
*/
int CAdamoSynPage::LoadXMLOptions (ElementXmlPtr p, CAdamoSynCtrl *pCtrl, bool bIsCombo)
{
	NodeListXmlPtr pRootChild;
    ElementXmlPtr  pChild;
    struct stRSOpzioni stOpzioni;
    COleVariant v;

    if (p->hasChildNodes())   {
        pRootChild = p->childNodes;
        int n=pRootChild->length, i=0;
        while (n>0)   {
            pChild=pRootChild->Getitem (i);
            v=pChild->nodeName;
            if (CString (v.bstrVal)=="option")   {
                v=pChild->getAttribute("selected");
                stOpzioni.m_bSelected=CString (v.bstrVal)=="true";
                v=pChild->getAttribute("value");
                if (v.vt!=VT_NULL)
                    stOpzioni.strValue=CString (v.bstrVal);
                v=pChild->text;
                stOpzioni.strOpzione=CString (v.bstrVal);
                bIsCombo ? ((CAdamoSynComboBox *)pCtrl)->AddRSOption (&stOpzioni, -1) : ((CAdamoSynListBox *)pCtrl)->AddRSOption (&stOpzioni, -1);
            }
            n--; i++;
        }
    }
    return 0;
}

/*
** OnTimer :
*/
int CAdamoSynPage::OnTimer ()
{
	for (POSITION pos = m_listCtrls.GetHeadPosition (); pos; )   {
		CAdamoSynCtrl *pCtrl = (CAdamoSynCtrl *) m_listCtrls.GetNext (pos);
		pCtrl->OnTimer ();
	}
	return 0;
}

/*
** OnEraseBkgnd :
*/
BOOL CAdamoSynPage::OnEraseBkgnd(CDC* pDC)
{
	CRect		rWnd;
	int			nX			= 0;
	int			nY			= 0;

	BOOL	bRetValue = CWnd::OnEraseBkgnd(pDC);

	// If there is a bitmap loaded
	if (m_hBckGround.GetSafeHandle ())
	{
		GetClientRect(rWnd);

		CDC			dcMemoryDC;
		CBitmap		bmpMemoryBitmap;
		CBitmap*	pbmpOldMemoryBitmap = NULL;

		dcMemoryDC.CreateCompatibleDC(pDC);
		bmpMemoryBitmap.CreateCompatibleBitmap(pDC, rWnd.Width(), rWnd.Height());
		pbmpOldMemoryBitmap = (CBitmap*)dcMemoryDC.SelectObject(&bmpMemoryBitmap);

		// Fill background 
		dcMemoryDC.FillSolidRect(rWnd, pDC->GetBkColor());

		CDC			dcTempDC;
		HBITMAP		hbmpOldTempBitmap = NULL;

		dcTempDC.CreateCompatibleDC(pDC);
		hbmpOldTempBitmap = (HBITMAP)::SelectObject(dcTempDC.m_hDC, m_hBckGround);
		dcMemoryDC.BitBlt(0, 0, m_dwWidth, m_dwHeight, &dcTempDC, 0, 0, SRCCOPY);

		if (!m_bKilling)
			OnEraseBackGround (&dcMemoryDC);
		
		pDC->BitBlt(0, 0, rWnd.Width(), rWnd.Height(), &dcMemoryDC, 0, 0, SRCCOPY);
		::SelectObject(dcTempDC.m_hDC, hbmpOldTempBitmap);
		dcMemoryDC.SelectObject(pbmpOldMemoryBitmap);
	} // if

	return bRetValue;
}

/*
** OnEraseBackGround :
*/
void CAdamoSynPage::OnEraseBackGround (CDC* pDC)
{
	POSITION pos;
	bool bReturn = false;

	/* andiamo a cercare il bottone che ha cliccato */
	for (pos = m_listCtrls.GetHeadPosition (); pos; )   {
		CAdamoSynCtrl *pCtrl = (CAdamoSynCtrl *) m_listCtrls.GetNext (pos);
		switch (pCtrl->KindOf ())   {
			case RSButton :
				((CAdamoSynButton *)pCtrl)->OnEraseBackGround (pDC);
				break;
			case RSImage :
				((CAdamoSynImage *)pCtrl)->OnEraseBackGround (pDC);
				break;
			case RSIPicture :
				((CAdamoSynIPicture *)pCtrl)->OnEraseBackGround (pDC);
                break;
		}
	}
}

