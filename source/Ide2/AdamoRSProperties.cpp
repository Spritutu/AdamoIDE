// AdamoRSProperties.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "MainFrame.h"
#include "adamorsproperties.h"
#include "propertygriditemex.h"
#include "AdamoRSOptions.h"
#include "ShowGlobalVars.h"
#include "AdamoRSEditTableIndex.h"
#include "AdamoRSEditGrid.h"
#include "ShowGlobalMatrix.h"
#include "IdeMisc.h"
#include "adamoselectdevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSProperties

IMPLEMENT_DYNCREATE(CAdamoRSProperties, CPaneView)

CAdamoRSProperties::CAdamoRSProperties() : m_pRSObject (NULL)
{
}

CAdamoRSProperties::~CAdamoRSProperties()
{
}

BEGIN_MESSAGE_MAP(CAdamoRSProperties, CPaneView)
	//{{AFX_MSG_MAP(CAdamoRSProperties)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CREATE()
    ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSProperties drawing

void CAdamoRSProperties::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSProperties diagnostics

#ifdef _DEBUG
void CAdamoRSProperties::AssertValid() const
{
	CPaneView::AssertValid();
}

void CAdamoRSProperties::Dump(CDumpContext& dc) const
{
	CPaneView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSProperties message handlers
/*
** OnCreate :
*/
int CAdamoRSProperties::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    /* normale creazione della finestra */
	if (CPaneView::OnCreate(lpCreateStruct) == -1)
		return -1;
    /* creazione del grid */
	m_wndRSPropGrid.Create( CRect(0,0, 0, 0), this, ID_PROPERTY_GRID);
	m_wndRSPropGrid.SetOwner(this);		
    m_wndRSPropGrid.ShowToolBar();
    m_wndRSPropGrid.SetTheme(((CIdeApp* ) AfxGetApp ())->GetXTPThemePropertyGrid ());
	m_wndRSPropGrid.HighlightChangedItems(TRUE);
    /* inizializzazione filtro caratteri */
    InitCharCheck ();
	return 0;
}

/*
** OnSize :
*/
void CAdamoRSProperties::OnSize(UINT nType, int cx, int cy) 
{
	CPaneView::OnSize(nType, cx, cy);
	/* eseguiamo il resize del grid delle proprieta */
    m_wndRSPropGrid.SetWindowPos(NULL, 0, 0, cx, cy, 0);
}

/*
** Select :
*/
void CAdamoRSProperties::Select (CAdamoRSBase* pObj)
{
    if (pObj)   {
        if (m_pRSObject!=pObj)   {
            m_pRSObject=pObj;
            CreateGrid ();
			SelectActiveXPreview ();
        }
    }
}

/*
** Clear :
*/
void CAdamoRSProperties::Clear ()
{
    m_pRSObject=NULL;
    m_wndRSPropGrid.ResetContent();
}

/*
** ClearRSObject :
*/
void CAdamoRSProperties::ClearRSObject (CAdamoRSBase* pObj)
{
    if (m_pRSObject==pObj)
        Clear ();
}

/*
** CreateGrid :
*/
void CAdamoRSProperties::CreateGrid ()
{
    /* creaimo le voci di base */
    m_wndRSPropGrid.ResetContent();
    /* prendiamo i dati del controllo */
    if (m_pRSObject)   {
        if (m_pRSObject->KindOf ()==RSForm)
            SelectRSForm ();
        else   {
            CXTPPropertyGridItem* pItem;
            /* settiamo i dati generali del grid */
            CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_1));
            CString str;
            AddGridItem(ID_RS_NAME, LOADSTRING (IDS_ADAMORSPROPERTIES_2), ((CAdamoRSControl*)m_pRSObject)->GetName(), pGridItem);
            str.Format ("%d", ((CAdamoRSControl*)m_pRSObject)->GetID());
            pItem=AddGridItem(ID_RS_ID, LOADSTRING (IDS_ADAMORSPROPERTIES_3),str, pGridItem);
            pItem->SetReadOnly (true);
            str.Format ("%d", ((CAdamoRSControl*)m_pRSObject)->GetTabIndex());
            AddGridItem(ID_RS_TABINDEX,LOADSTRING (IDS_ADAMORSPROPERTIES_4),str, pGridItem);
            str.Format ("%d", ((CAdamoRSControl*)m_pRSObject)->GetZOrder());
            AddGridItem(ID_RS_ZORDER,LOADSTRING (IDS_ADAMORSPROPERTIES_5),str, pGridItem);
            SelectRSObject ();
        }
    }
}

/*
** AddGridItem :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridItem (int iId,CString strCaption, CString strValue, CXTPPropertyGridItem *pItemNode)
{
    CXTPPropertyGridItem *pItem = new CXTPPropertyGridItem(iId,strValue);
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
    return pItem;
}

/*
** AddGridItemCheckBox :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridItemCheckBox (int iId, CString strCaption, BOOL bValue, CXTPPropertyGridItem *pItemNode)
{
	CXTPPropertyGridItemBool* pItemBool = (CXTPPropertyGridItemBool*)pItemNode->AddChildItem(
		new CXTPPropertyGridItemBool(strCaption, FALSE));
	pItemBool->SetBool(bValue);
	pItemBool->SetID(iId);
	pItemBool->SetCheckBoxStyle();
    return pItemBool;
}

/*
** AddGridComboVarType :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboVarType (int nType, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

    switch (nType)   {
        case LUA_TNUMBER :
            strValue = "number";
            break;
        case LUA_TSTRING :
            strValue = "string";
            break;
        case LUA_TBOOLEAN :
            strValue = "boolean";
            break;
    }
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (ID_RS_VAR_TYPE, strValue));
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMORSPROPERTIES_11));
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
	pList->AddConstraint(_T("number"));
	pList->AddConstraint(_T("string"));
	pList->AddConstraint(_T("boolean"));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboDBFields :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboDBFields (CString strTable, CString strField, CXTPPropertyGridItem* pGridItem)
{
    CXTPPropertyGridItem* pItemCombo;
    CStringList sl;
    CString str;

    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
    if (pDoc)   {
        pItemCombo = pGridItem->AddChildItem (new CXTPPropertyGridItem (ID_RS_DB_FIELDS, strField));
        pItemCombo->SetCaption(LOADSTRING (IDS_ADAMORSPROPERTIES_21));
        if (strField!="")   {
            CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
            pDoc->GetFieldsList (strTable, sl);
            for (POSITION pos=sl.GetHeadPosition (); pos; )   {
                str=sl.GetNext (pos);
	            pList->AddConstraint(str);
            }
	        pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
            pItemCombo->SetConstraintEdit(TRUE);
        }
        else
            pItemCombo->SetReadOnly (true);
    }
    return pItemCombo;
}

/*
** AddGridComboDBTable :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboDBTable (CString strTable, CXTPPropertyGridItem* pGridItem)
{
    CXTPPropertyGridItem* pItemCombo=NULL;
    CStringList sl;
    CString str;

    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
    if (pDoc)   {
        pItemCombo = pGridItem->AddChildItem (new CXTPPropertyGridItem (ID_RS_DB_TABLE, strTable));
        pItemCombo->SetCaption(LOADSTRING (IDS_ADAMORSPROPERTIES_22));
        CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
        pDoc->GetTableList (sl);
        for (POSITION pos=sl.GetHeadPosition (); pos; )   {
            str=sl.GetNext (pos);
	        pList->AddConstraint(str);
        }
	    pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
        pItemCombo->SetConstraintEdit(TRUE);
    }
    return pItemCombo;
}

/*
** AddGridComboDBIndex :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboDBIndex (CString strTable, CString strIndex, CXTPPropertyGridItem* pGridItem)
{
    CXTPPropertyGridItem* pItemCombo=NULL;
    CStringList sl;
    CString str;

    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
    if (pDoc)   {
        pItemCombo = pGridItem->AddChildItem (new CXTPPropertyGridItem (ID_RS_DB_INDEX, strIndex));
        pItemCombo->SetCaption(LOADSTRING (IDS_ADAMORSPROPERTIES_23));
        if (strIndex!="")   {
            CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
            pDoc->GetIndexList (strTable, sl);
            for (POSITION pos=sl.GetHeadPosition (); pos; )   {
                str=sl.GetNext (pos);
	            pList->AddConstraint(str);
            }
	        pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
            pItemCombo->SetConstraintEdit(TRUE);
        }
        else
            pItemCombo->SetReadOnly (true);
    }
    return pItemCombo;
}

/*
** AddSpinBorders : 
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddSpinBorders (int nID, CString strText, int nValue, CXTPPropertyGridItem *pItemNode)
{
    CString str;

    CCustomItemSpin* pSpin=new CCustomItemSpin (nID);
    pSpin->SetRange (1, 16);
    CXTPPropertyGridItem* pItemSpin = pItemNode->AddChildItem(pSpin);
    str.Format ("%d", nValue);
    pItemSpin->SetCaption (strText);
    pItemSpin->SetValue (str);
    return pItemSpin;
}

/*
** SelectRSObject :
*/
void CAdamoRSProperties::SelectRSObject ()
{
    if (m_pRSObject->KindOf()==RSLabel)
        SelectRSLabel ();
    else  
    if (m_pRSObject->KindOf()==RSInputText)
        SelectRSInputText ();
    else  
    if (m_pRSObject->KindOf()==RSInputRadio)
        SelectRSRadioButton ();
    else  
    if (m_pRSObject->KindOf()==RSInputCheck)
        SelectRSCheckBox ();
    else  
    if (m_pRSObject->KindOf()==RSGroup)
        SelectRSGroup ();
    else  
    if (m_pRSObject->KindOf()==RSButton)
        SelectRSButton ();
    else  
    if (m_pRSObject->KindOf()==RSList)
        SelectRSList ();
    else  
    if (m_pRSObject->KindOf()==RSCombo)
        SelectRSCombo ();
    else  
    if (m_pRSObject->KindOf()==RSImage)
        SelectRSImage ();
    else  
    if (m_pRSObject->KindOf()==RSGrid)
		SelectRSGrid ();
	else
    if (m_pRSObject->KindOf()==RSAx)
		SelectRSAx ();
	else
    if (m_pRSObject->KindOf()==RSILed)
		SelectRSLed ();
	else
    if (m_pRSObject->KindOf()==RSIAngularGauge)
		SelectRSIAngularGauge ();
	else
    if (m_pRSObject->KindOf()==RSILinearGauge)
		SelectRSILinearGauge ();
	else
    if (m_pRSObject->KindOf()==RSIDisplay)
		SelectRSIDisplay ();
	else
    if (m_pRSObject->KindOf()==RSILabel)
		SelectRSILabel ();
	else
    if (m_pRSObject->KindOf()==RSIPicture)
		SelectRSIPicture ();
}

/*
** SelectRSForm :
*/
void CAdamoRSProperties::SelectRSForm ()
{
    CXTPPropertyGridItem* pItem, *pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_85)), *pGrid;
    pItem = AddGridItem(ID_RS_NAME, LOADSTRING (IDS_ADAMORSPROPERTIES_2),((CAdamoRSControl*)m_pRSObject)->GetName(), pGridItem);
	pItem->SetReadOnly (true);
	AddGridItemCheckBox	(ID_RS_FORM_ENABLE_BCKGR, LOADSTRING (IDS_ADAMORSPROPERTIES_86), ((CAdamoRSForm*)m_pRSObject)->IsBackgroundEnabled (), pGridItem);
	pGrid = AddGridComboImageID (((CAdamoRSForm*)m_pRSObject)->GetBackgroundImage (), ID_RS_FORM_IMAGE, pGridItem, LOADSTRING (IDS_ADAMORSPROPERTIES_74));
	pGrid->SetReadOnly (!((CAdamoRSForm*)m_pRSObject)->IsBackgroundEnabled ());
	AddGridItemCheckBox	(ID_RS_FORM_MAIN_SYN, LOADSTRING (IDS_ADAMORSPROPERTIES_95), ((CAdamoRSForm*)m_pRSObject)->GetMainSynoptic (), pGridItem);
    pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_1));
    CRect rc=((CAdamoRSForm*)m_pRSObject)->GetPosition ();
    CString str;
    str.Format ("%d", rc.top);
    AddGridItem(ID_RS_TOP, LOADSTRING (IDS_ADAMORSPROPERTIES_24), str, pGridItem);
    str.Format ("%d", rc.left);
    AddGridItem(ID_RS_LEFT, LOADSTRING (IDS_ADAMORSPROPERTIES_25), str, pGridItem);
    str.Format ("%d", rc.bottom);
    AddGridItem(ID_RS_BOTTOM, LOADSTRING (IDS_ADAMORSPROPERTIES_26), str, pGridItem);
    str.Format ("%d", rc.right);
    AddGridItem(ID_RS_RIGHT, LOADSTRING (IDS_ADAMORSPROPERTIES_27), str, pGridItem);
}

/*
** SelectRSButton :
*/
void CAdamoRSProperties::SelectRSButton ()
{
    CString str;
    str=((CAdamoRSButton*)m_pRSObject)->GetValue();
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_28));
    AddGridItem(ID_RS_BUTTON_VALUE,LOADSTRING (IDS_ADAMORSPROPERTIES_29),str, pGridItem);
 	AddGridComboButtonType (((CAdamoRSButton*)m_pRSObject)->GetButtonType (), ID_RS_BUTTON_TYPE, pGridItem);
	CXTPPropertyGridItem* pGridImage = AddGridComboImageID (((CAdamoRSButton*)m_pRSObject)->GetButtonImage (), ID_RS_BUTTON_IMAGE, pGridItem, LOADSTRING (IDS_ADAMORSPROPERTIES_74));
	CXTPPropertyGridItem* pGridTransparency = AddGridItemCheckBox	(ID_RS_BUTTON_TRANSPARENCY, LOADSTRING (IDS_ADAMORSPROPERTIES_115), ((CAdamoRSButton*)m_pRSObject)->IsTransparent (), pGridItem);
	if (((CAdamoRSButton*)m_pRSObject)->GetButtonType () == eStandard)   {
		pGridImage->SetReadOnly (true);
		pGridTransparency->SetReadOnly (true);
	}
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_89));
	AddGridItemCheckBox	(ID_RS_BUTTON_ACTION_DOWNLOAD,   LOADSTRING (IDS_ADAMORSPROPERTIES_90), ((CAdamoRSButton*)m_pRSObject)->IsDownloadActionEnabled (), pGridItem);
	AddGridItemCheckBox	(ID_RS_BUTTON_ACTION_LOAD,       LOADSTRING (IDS_ADAMORSPROPERTIES_93), ((CAdamoRSButton*)m_pRSObject)->IsLoadActionEnabled (), pGridItem);
	AddGridItemCheckBox	(ID_RS_BUTTON_ACTION_SAVE,	     LOADSTRING (IDS_ADAMORSPROPERTIES_91), ((CAdamoRSButton*)m_pRSObject)->IsSaveActionEnabled (), pGridItem);
	AddGridItemCheckBox	(ID_RS_BUTTON_ACTION_CHANGEPAGE, LOADSTRING (IDS_ADAMORSPROPERTIES_92), ((CAdamoRSButton*)m_pRSObject)->IsChangePageActionEnabled (), pGridItem);
	AddGridComboNextPage (((CAdamoRSButton*)m_pRSObject)->GetNextPage (), ID_RS_BUTTON_NEXT_PAGE, pGridItem);
    SelectRSStyle ();
}

/*
** SelectRSList :
*/
void CAdamoRSProperties::SelectRSList ()
{
    CString strVar;
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_30));
    CCustomItemRSList *pItem= new CCustomItemRSList (ID_RS_LIST_OPTIONS, "");
    pGridItem->AddChildItem (pItem)->SetCaption (LOADSTRING (IDS_ADAMORSPROPERTIES_31));
    AddGridItemCheckBox (ID_RS_LIST_MULTISEL, LOADSTRING (IDS_ADAMORSPROPERTIES_32), ((CAdamoRSList*)m_pRSObject)->IsMultipleSelection(), pGridItem);
    AddSpinBorders (ID_RS_LIST_SIZE, LOADSTRING (IDS_ADAMORSPROPERTIES_33), ((CAdamoRSList*)m_pRSObject)->GetSize (), pGridItem);
    SelectRSVars ();
    SelectRSDB ();
    SelectRSStyle ();
}

/*
** SelectRSCombo :
*/
void CAdamoRSProperties::SelectRSCombo ()
{
    CString strVar;
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_34));
    CCustomItemRSCombo *pItem= new CCustomItemRSCombo (ID_RS_COMBO_OPTIONS, "");
    pGridItem->AddChildItem (pItem)->SetCaption (LOADSTRING (IDS_ADAMORSPROPERTIES_35));
    SelectRSVars ();
    SelectRSDB ();
    SelectRSStyle ();
}

/*
** SelectRSImage :
*/
void CAdamoRSProperties::SelectRSImage ()
{
    CString strVar;
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_36));
    CString strIDImage=((CAdamoRSImage*)m_pRSObject)->GetIDImage ();
    AddGridComboImageID (strIDImage, ID_RS_IMAGE, pGridItem, LOADSTRING (IDS_ADAMORSPROPERTIES_74));
    AddGridItemCheckBox (ID_RS_IMAGE_STRETCH, LOADSTRING (IDS_ADAMORSPROPERTIES_37), ((CAdamoRSImage*)m_pRSObject)->IsStretched(), pGridItem);
    AddGridItemCheckBox (ID_RS_IMAGE_TRANSPARENCY, LOADSTRING (IDS_ADAMORSPROPERTIES_226), ((CAdamoRSImage*)m_pRSObject)->IsTransparent (), pGridItem);
	AddGridColor (ID_RS_IMAGE_TRANSPARENT_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_53), ((CAdamoRSImage *) m_pRSObject)->GetTransparentColor (), pGridItem);
    SelectRSStyle ();
}

/*
** SelectRSDB :
*/
void CAdamoRSProperties::SelectRSDB ()
{
    stRSVariabili& var=((CAdamoRSControl*)m_pRSObject)->GetObjVar();
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_38));
    AddGridComboDBTable  (var.m_strTable, pGridItem);
    AddGridComboDBFields (var.m_strTable, var.m_strField, pGridItem);
    AddGridComboDBIndex  (var.m_strTable, var.m_strKeyName, pGridItem);
    CCustomItemRSTableIndex *pItem= new CCustomItemRSTableIndex (ID_RS_DB_INDEXVALUE, var.m_strKeyValue);
    pItem->SetFlags(xtpGridItemHasExpandButton);
    pItem->SetDBData (&var);
    pGridItem->AddChildItem (pItem)->SetCaption (LOADSTRING (IDS_ADAMORSPROPERTIES_39));
    if (var.m_strKeyValue=="")
        pItem->SetReadOnly (true);
}

/*
** SelectRSDBMatrix :
*/
void CAdamoRSProperties::SelectRSDBMatrix ()
{
	CString str;
    stRSVariabili& var=((CAdamoRSControl*)m_pRSObject)->GetObjVar();
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_38));
	str = var.m_strTable;
    AddGridItem(ID_RS_MATRIX_DB, LOADSTRING (IDS_ADAMORSPROPERTIES_22), str, pGridItem);
}

/*
** SelectRSInputText :
*/
void CAdamoRSProperties::SelectRSInputText ()
{
    CString str;
    str=((CAdamoRSInputText*)m_pRSObject)->GetValue();
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_40));
    AddGridItem(ID_RS_EDIT_VALUE, LOADSTRING (IDS_ADAMORSPROPERTIES_41), str, pGridItem);
    str.Format ("%d", ((CAdamoRSInputText*)m_pRSObject)->GetSize ());
    AddGridItem(ID_RS_EDIT_SIZE, LOADSTRING (IDS_ADAMORSPROPERTIES_42), str, pGridItem);
    AddGridItemCheckBox (ID_RS_EDIT_PSW, LOADSTRING (IDS_ADAMORSPROPERTIES_43), ((CAdamoRSInputText*)m_pRSObject)->GetPsw(), pGridItem);
    AddGridItemCheckBox (ID_RS_EDIT_UPDATE, LOADSTRING (IDS_ADAMORSPROPERTIES_97), ((CAdamoRSInputText*)m_pRSObject)->IsAutoUpdate(), pGridItem);
 	AddGridComboEditType (((CAdamoRSInputText*)m_pRSObject)->GetType (), ID_RS_EDIT_TYPE, pGridItem);
    SelectRSVars ();
    SelectRSDB ();
    SelectRSStyle ();
}

/*
** SelectRSStyle :
*/
void CAdamoRSProperties::SelectRSStyle ()
{
    SelectRSStylePosition ();
    SelectRSStyleFont ();
}

/*
** SelectRSStylePosition :
*/
void CAdamoRSProperties::SelectRSStylePosition ()
{
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING(IDS_ADAMORSPROPERTIES_44));
    CRect rc=((CAdamoRSControl*)m_pRSObject)->GetObjStyle ().GetRect ();
    CString str;
    str.Format ("%d", rc.top);
    AddGridItem(ID_RS_TOP,LOADSTRING(IDS_ADAMORSPROPERTIES_24), str, pGridItem);
    str.Format ("%d", rc.left);
    AddGridItem(ID_RS_LEFT,LOADSTRING(IDS_ADAMORSPROPERTIES_25), str, pGridItem);
    str.Format ("%d", rc.bottom);
    AddGridItem(ID_RS_BOTTOM,LOADSTRING(IDS_ADAMORSPROPERTIES_26), str, pGridItem);
    str.Format ("%d", rc.right);
    AddGridItem(ID_RS_RIGHT,LOADSTRING(IDS_ADAMORSPROPERTIES_27), str, pGridItem);
}

/*
** SelectRSStyleFont :
*/
void CAdamoRSProperties::SelectRSStyleFont ()
{
    LOGFONT lf;

    CFont* pFont=((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ();
    pFont->GetLogFont (&lf);
    stRSFont& RSFont=((CAdamoRSControl*)m_pRSObject)->GetObjStyle().GetRSFont();
    strcpy (lf.lfFaceName, RSFont.strFaceName);
    lf.lfHeight=-RSFont.nSize;
    lf.lfWeight=RSFont.bBold ? FW_BOLD : FW_NORMAL;
    lf.lfItalic=RSFont.bItalic;
    lf.lfStrikeOut=RSFont.bStrikeOut;
    lf.lfUnderline=RSFont.bUnderLine;
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_45));
    CXTPPropertyGridItem* pItem=pGridItem->AddChildItem(new CXTPPropertyGridItemFont(ID_RS_FONT, lf));
    pItem->SetCaption (LOADSTRING (IDS_ADAMORSPROPERTIES_46));
}

/*
** OnGridNotify :
*/
LRESULT CAdamoRSProperties::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
    int n;
    bool bAssigned=false;

	if (wParam==XTP_PGN_ITEMVALUE_CHANGED)   {
        CXTPPropertyGridItem *pItem = (CXTPPropertyGridItem*)lParam;
		if (pItem)   {
            /* assegnamo prima gli elementi di base */
            switch (pItem->GetID())   {
                case ID_RS_NAME :
					 m_pRSObject->CAdamoRSBase::SetName (pItem->GetValue()); bAssigned=true;
                     break;
                case ID_RS_TABINDEX :
                     n=atoi (pItem->GetValue());
                     if (CheckRange (n, 0, MAX_RSCONTROLS-1))   {
                         m_pRSObject->SetTabIndex (n); bAssigned=true;
                     }
                     else
                         ReloadField (pItem);
                     break;
                case ID_RS_ZORDER :
                     n=atoi (pItem->GetValue());
                     if (CheckRange (n, 0, MAX_RSCONTROLS-1))   {
                         m_pRSObject->SetZOrder (n); bAssigned=true;
                     }
                     else
                         ReloadField (pItem);
                     break;
		    }
            /* ora differenziamo a seconda del tipo */
            if (!bAssigned)
                AssignRSObject (pItem);
            UpdateDiagramSelected ();
		}
	}
	else
        if (wParam == XTP_PGN_EDIT_CHANGED)  {
            CXTPPropertyGridInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPPropertyGridInplaceEdit, (CWnd*)lParam);
            CXTPPropertyGridItem *pItem = pEdit->GetItem ();
            CheckInput (pItem->GetID(), pEdit);
        }
        else   {
	        if (wParam == XTP_PGN_INPLACEBUTTONDOWN)	{
		        CXTPPropertyGridInplaceButton* pButton = (CXTPPropertyGridInplaceButton*)lParam;
                if ((pButton->GetItem()->GetID() == ID_RS_VAR_NAME) && pButton->GetID() == XTP_ID_PROPERTYGRID_EXPANDBUTTON)   {
                    CShowGlobalVars sg;
                    CString strName, strCol;
                    int nRow, nType;

                    ((CCustomItemRSVar *)pButton->GetItem())->GetResults (&strName, &nRow, &strCol, &nType);
                    sg.SetResults (strName, nRow, strCol, nType);
                    sg.DoModal ();
                    sg.GetResults (&strName, &nRow, &strCol, &nType);
                    ((CCustomItemRSVar *)pButton->GetItem())->SetResults (strName, nRow, strCol, nType);
                    ((CCustomItemRSVar *)pButton->GetItem())->OnValueChanged(strName);
                }
                if ((pButton->GetItem()->GetID() == ID_RS_DB_INDEXVALUE) && pButton->GetID() == XTP_ID_PROPERTYGRID_EXPANDBUTTON)   {
                    CAdamoRSEditTableIndex eti;

                    stRSVariabili *var=((CCustomItemRSTableIndex *)pButton->GetItem())->GetDBData ();
                    if (var->m_strTable!=""&&var->m_strKeyName!="")   {
                        eti.SetDBData (var->m_strTable, var->m_strKeyName, var->m_strKeyValue);
                        if (eti.DoModal ()==IDOK)   {
                            CString str=eti.GetResult ();
                            if (str!="")
                                ((CCustomItemRSTableIndex *)pButton->GetItem())->OnValueChanged(str);
                        }
                    }
                }
            }
        }
    return 0;
}

/*
** InitCharCheck :
*/
void CAdamoRSProperties::InitCharCheck ()
{
	m_objCheckKeys.AddRange( 'a', 'z' );
	m_objCheckKeys.AddRange( 'A', 'Z' );
	m_objCheckKeys.AddChar( '_' );
	m_objCheckKeys.AddChar( '|' );
    m_objCheckKeys.AddRange ('0', '9');
	m_objCheckIdentifier.AddRange( 'a', 'z' );
	m_objCheckIdentifier.AddRange( 'A', 'Z' );
	m_objCheckIdentifier.AddChar( '_' );
    m_objCheckNumbers.AddRange ('0', '9');
    m_objCheckHex.AddRange ('0', '9');
    m_objCheckHex.AddRange ('a', 'f');
    m_objCheckHex.AddRange ('A', 'F');
	m_objCheckCaption.AddRange( 'a', 'z' );
	m_objCheckCaption.AddRange( 'A', 'Z' );
	m_objCheckCaption.AddChar( '!' );
	m_objCheckCaption.AddChar( '\"' );
	m_objCheckCaption.AddChar( '#' );
	m_objCheckCaption.AddChar( '$' );
	m_objCheckCaption.AddChar( '%' );
	m_objCheckCaption.AddChar( '&' );
	m_objCheckCaption.AddChar( ',' );
	m_objCheckCaption.AddChar( '(' );
	m_objCheckCaption.AddChar( ')' );
	m_objCheckCaption.AddChar( '*' );
	m_objCheckCaption.AddChar( '+' );
	m_objCheckCaption.AddChar( '\'' );
	m_objCheckCaption.AddChar( '-' );
	m_objCheckCaption.AddChar( '.' );
	m_objCheckCaption.AddChar( '/' );
	m_objCheckCaption.AddChar( ':' );
	m_objCheckCaption.AddChar( ';' );
	m_objCheckCaption.AddChar( '<' );
	m_objCheckCaption.AddChar( '=' );
	m_objCheckCaption.AddChar( '>' );
	m_objCheckCaption.AddChar( '@' );
	m_objCheckCaption.AddChar( '[' );
	m_objCheckCaption.AddChar( '\\' );
	m_objCheckCaption.AddChar( ']' );
	m_objCheckCaption.AddChar( '^' );
	m_objCheckCaption.AddChar( '{' );
	m_objCheckCaption.AddChar( '|' );
	m_objCheckCaption.AddChar( '}' );
	m_objCheckCaption.AddChar( '~' );
	m_objCheckCaption.AddChar( ' ' );
}

/*
** StringControl :
*/
void CAdamoRSProperties::StringControl (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength)
{
        
}

/*
** AssignRSObject :
*/
void CAdamoRSProperties::AssignRSObject (CXTPPropertyGridItem *pItem)
{
    switch (m_pRSObject->KindOf ())   {
        case RSForm :
            AssignRSForm (pItem);
            break;
        case RSLabel :
            AssignRSLabel (pItem);
            break;
        case RSInputText :
            AssignRSInputText (pItem);
            break;
        case RSInputRadio :
            AssignRSRadioButton (pItem);
            break;
        case RSInputCheck :
            AssignRSCheckBox (pItem);
            break;
        case RSGroup :
            AssignRSGroup (pItem);
            break;
        case RSButton :
            AssignRSButton (pItem);
            break;
        case RSList :
            AssignRSList (pItem);
            break;
        case RSCombo :
            AssignRSCombo (pItem);
            break;
        case RSImage :
            AssignRSImage (pItem);
            break;
        case RSGrid :
            AssignRSGrid (pItem);
            break;
        case RSAx :
            AssignRSAx (pItem);
            break;
        case RSILed :
            AssignRSLed (pItem);
            break;
        case RSIAngularGauge :
            AssignRSIAngularGauge (pItem);
            break;
        case RSILinearGauge :
            AssignRSILinearGauge (pItem);
            break;
		case RSIDisplay :
			AssignRSIDisplay (pItem);
			break;
		case RSILabel :
			AssignRSILabel (pItem);
			break;
		case RSIPicture :
			AssignRSIPicture (pItem);
			break;
    }
}

/*
** AssignRSButton :
*/
void CAdamoRSProperties::AssignRSButton (CXTPPropertyGridItem *pItem)
{
    CXTPPropertyGridItem* pFindItem;
	eRSButtonType e;
	CString str;
    bool bAssigned=false, b;

    switch (pItem->GetID ())   {
        case ID_RS_BUTTON_VALUE :
            ((CAdamoRSButton*)m_pRSObject)->SetValue (pItem->GetValue ()); bAssigned = true;
            UpdateDiagramValue ();
            break;
        case ID_RS_BUTTON_TYPE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_87))
				e = eStandard;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_88))
				e = eBitmap;
			((CAdamoRSButton*)m_pRSObject)->SetButtonType (e);
			((CAdamoRSButton*)m_pRSObject)->SetButtonImage (""); 
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_BUTTON_IMAGE);
			e == eStandard ?  pFindItem->SetReadOnly (true) : pFindItem->SetReadOnly (false);
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_BUTTON_TRANSPARENCY);
			e == eStandard ?  pFindItem->SetReadOnly (true) : pFindItem->SetReadOnly (false);
			pFindItem->SetValue ("");
			bAssigned = true;
			break;
        case ID_RS_BUTTON_IMAGE :
			str=pItem->GetValue ();
			((CAdamoRSButton*)m_pRSObject)->SetButtonImage (str); 
			bAssigned = true;
			break;
        case ID_RS_BUTTON_TRANSPARENCY :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
			((CAdamoRSButton*)m_pRSObject)->SetTransparency (b); 
			bAssigned = true;
			break;
        case ID_RS_BUTTON_ACTION_DOWNLOAD :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
			((CAdamoRSButton*)m_pRSObject)->EnableDownloadAction (b);
			bAssigned = true;
			break;
        case ID_RS_BUTTON_ACTION_LOAD :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
			((CAdamoRSButton*)m_pRSObject)->EnableLoadAction (b);
			bAssigned = true;
			break;
        case ID_RS_BUTTON_ACTION_SAVE :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
			((CAdamoRSButton*)m_pRSObject)->EnableSaveAction (b);
			bAssigned = true;
			break;
        case ID_RS_BUTTON_ACTION_CHANGEPAGE :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
			((CAdamoRSButton*)m_pRSObject)->EnableChangePageAction (b);
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_BUTTON_NEXT_PAGE);
			pFindItem->SetReadOnly (!b);
			pFindItem->SetValue ("");
			bAssigned = true;
        case ID_RS_BUTTON_NEXT_PAGE :
			str=pItem->GetValue ();
			((CAdamoRSButton*)m_pRSObject)->SetNextPage (str);
			bAssigned = true;
			break;
    }
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** AssignRSButton :
*/
void CAdamoRSProperties::AssignRSInputText (CXTPPropertyGridItem *pItem)
{
	CString str;
	eEditType e;
    bool bAssigned=false;

    switch (pItem->GetID ())   {
        case ID_RS_EDIT_VALUE :
            ((CAdamoRSInputText*)m_pRSObject)->SetValue (pItem->GetValue ()); bAssigned=true;
            break;
        case ID_RS_EDIT_SIZE :
            ((CAdamoRSInputText*)m_pRSObject)->SetSize (atoi (pItem->GetValue ())); bAssigned=true;
            break;
        case ID_RS_EDIT_PSW :
            ((CAdamoRSInputText*)m_pRSObject)->SetPsw (((CXTPPropertyGridItemBool*) pItem)->GetBool ()!=0); bAssigned=true;
            break;
        case ID_RS_EDIT_UPDATE :
            ((CAdamoRSInputText*)m_pRSObject)->SetAutoUpdate (((CXTPPropertyGridItemBool*) pItem)->GetBool ()!=0); bAssigned=true;
            break;
        case ID_RS_EDIT_TYPE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_98))
				e = eNumbers;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_99))
				e = eFloat;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_100))
				e = eHex;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_101))
				e = eLetters;
			((CAdamoRSInputText*)m_pRSObject)->SetType (e);
			bAssigned = true;
			break;
    }
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        bAssigned=AssignRSDB (pItem);
    if (!bAssigned)   {
        AssignRSStyle (pItem);
    }
}

/*
** AssignRSStyle
*/
void CAdamoRSProperties::AssignRSStyle (CXTPPropertyGridItem *pItem)
{
    CAdamoRSStyle& RSStyle=((CAdamoRSButton*)m_pRSObject)->GetObjStyle ();
    UINT nID=pItem->GetID ();
    CSize size;

    if (nID==ID_RS_TOP||nID==ID_RS_LEFT||nID==ID_RS_BOTTOM||nID==ID_RS_RIGHT)   {
        CRect rc;
        rc=RSStyle.GetRect ();
        switch (pItem->GetID ())   {
            case ID_RS_TOP :
                size=((CMainFrame*)AfxGetMainWnd ())->GetScreenResolution ();
                AssignRange (pItem, (int*)&rc.top, 0, size.cy);
                break;
            case ID_RS_LEFT :
                size=((CMainFrame*)AfxGetMainWnd ())->GetScreenResolution ();
                AssignRange (pItem, (int*)&rc.left, 0, size.cx);
                break;
            case ID_RS_BOTTOM :
                size=((CMainFrame*)AfxGetMainWnd ())->GetScreenResolution ();
                AssignRange (pItem, (int*)&rc.bottom, 0, size.cy);
                break;
            case ID_RS_RIGHT :
                size=((CMainFrame*)AfxGetMainWnd ())->GetScreenResolution ();
                AssignRange (pItem, (int*)&rc.right, 0, size.cx);
                break;
        }
        RSStyle.SetRect (rc);
        UpdateDiagramEditor ();
    }
    if (nID==ID_RS_FONT)   {
        LOGFONT lf;
        stRSFont& RSFont=RSStyle.GetRSFont();
        ((CXTPPropertyGridItemFont*) pItem)->GetFont (&lf);
        RSFont.strFaceName=lf.lfFaceName;
        RSFont.nSize=abs (lf.lfHeight);
        RSFont.bBold=lf.lfWeight==FW_BOLD;
        RSFont.bItalic=lf.lfItalic!=0;
        RSFont.bStrikeOut=lf.lfStrikeOut!=0;
        RSFont.bUnderLine=lf.lfUnderline!=0;
    }
}

/*
** UpdateDiagramEditor :
*/
void CAdamoRSProperties::UpdateDiagramEditor ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)
        pView->UpdateDiagramEntitySize ();
}

/*
** UpdateDiagramSize :
*/
void CAdamoRSProperties::UpdateDiagramSize ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)
        pView->UpdateDiagramSize ();
}

/*
** UpdateDiagramSize :
*/
void CAdamoRSProperties::UpdateDiagramValue ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)
        pView->UpdateDiagramValue ();
}

/*
** UpdateDiagramSelected :
*/
void CAdamoRSProperties::UpdateDiagramSelected ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)
        pView->UpdateDiagramSelected ();
}

/*
** UpdateSelected :
*/
void CAdamoRSProperties::UpdateSelected ()
{
	CString str;
	CRect rc;

    if (m_pRSObject)   {
        if (m_pRSObject->KindOf ()!=RSForm)   {
			rc = ((CAdamoRSControl*)m_pRSObject)->GetObjStyle().GetRect();
            CXTPPropertyGridItem* pSelectedItem;
            str.Format ("%d", rc.top);
            pSelectedItem=m_wndRSPropGrid.FindItem (ID_RS_TOP);
            pSelectedItem->SetValue (str);
            str.Format ("%d", rc.left);
            pSelectedItem=m_wndRSPropGrid.FindItem (ID_RS_LEFT);
            pSelectedItem->SetValue (str);
            str.Format ("%d", rc.bottom);
            pSelectedItem=m_wndRSPropGrid.FindItem (ID_RS_BOTTOM);
            pSelectedItem->SetValue (str);
            str.Format ("%d", rc.right);
            pSelectedItem=m_wndRSPropGrid.FindItem (ID_RS_RIGHT);
            pSelectedItem->SetValue (str);
			UpdateActiveXPreviewSize (rc);
        }
        else   {
			rc=((CAdamoRSForm*)m_pRSObject)->GetPosition ();
            CXTPPropertyGridItem* pSelectedItem;
            str.Format ("%d", rc.top);
            pSelectedItem=m_wndRSPropGrid.FindItem (ID_RS_TOP);
            pSelectedItem->SetValue (str);
            str.Format ("%d", rc.left);
            pSelectedItem=m_wndRSPropGrid.FindItem (ID_RS_LEFT);
            pSelectedItem->SetValue (str);
            str.Format ("%d", rc.bottom);
            pSelectedItem=m_wndRSPropGrid.FindItem (ID_RS_BOTTOM);
            pSelectedItem->SetValue (str);
            str.Format ("%d", rc.right);
            pSelectedItem=m_wndRSPropGrid.FindItem (ID_RS_RIGHT);
            pSelectedItem->SetValue (str);
        }
    }
}

/*
** AssignRSForm :
*/
void CAdamoRSProperties::AssignRSForm (CXTPPropertyGridItem *pItem)
{
    UINT nID=pItem->GetID ();    
    CXTPPropertyGridItem* pFindItem;
    CString str;
    CSize size;
	BOOL b;

    if (nID==ID_RS_TOP||nID==ID_RS_LEFT||nID==ID_RS_BOTTOM||nID==ID_RS_RIGHT||nID==ID_RS_FORM_STORE_LOCATION||nID==ID_RS_FORM_STORE_MODE
		||nID==ID_RS_FORM_STORE_FILE||nID==ID_RS_FORM_OPEN_FILE||nID==ID_RS_FORM_ENABLE_BCKGR||nID==ID_RS_FORM_IMAGE||nID==ID_RS_FORM_MAIN_SYN)   {
        CRect rc;
        rc=((CAdamoRSForm*)m_pRSObject)->GetPosition ();
        switch (pItem->GetID ())   {
            case ID_RS_TOP :
                size=((CMainFrame*)AfxGetMainWnd ())->GetScreenResolution ();
                AssignRange (pItem, (int*)&rc.top, 0, size.cy);
                break;
            case ID_RS_LEFT :
                size=((CMainFrame*)AfxGetMainWnd ())->GetScreenResolution ();
                AssignRange (pItem, (int*)&rc.left, 0, size.cx);
                break;
            case ID_RS_BOTTOM :
                size=((CMainFrame*)AfxGetMainWnd ())->GetScreenResolution ();
                AssignRange (pItem, (int*)&rc.bottom, 0, size.cy);
                break;
            case ID_RS_RIGHT :
                size=((CMainFrame*)AfxGetMainWnd ())->GetScreenResolution ();
                AssignRange (pItem, (int*)&rc.right, 0, size.cx);
                break;
			case ID_RS_FORM_ENABLE_BCKGR :
                b=((CXTPPropertyGridItemBool*)pItem)->GetBool ();
				((CAdamoRSForm*)m_pRSObject)->EnableBackground (b!=0);
				pFindItem = m_wndRSPropGrid.FindItem(ID_RS_FORM_IMAGE);
				pFindItem->SetReadOnly(!b);
				if (!b)
					pFindItem->SetValue ("");
				break;
			case ID_RS_FORM_IMAGE :
                str=pItem->GetValue ();
				((CAdamoRSForm*)m_pRSObject)->SetBackgroundImage (str);
				break;
			case ID_RS_FORM_MAIN_SYN :
				b=((CXTPPropertyGridItemBool*)pItem)->GetBool ();
				if (b)   {
					if (CheckExistMainSyn ())   {
						AfxMessageBox (LOADSTRING (IDS_ADAMORSPROPERTIES_96), MB_ICONWARNING);
						((CXTPPropertyGridItemBool*)pItem)->SetBool (FALSE);
					}
					else
						((CAdamoRSForm*)m_pRSObject)->SetMainSynoptic ();
				}
				else
					((CAdamoRSForm*)m_pRSObject)->SetMainSynoptic (b!=0);
				break;
        }
        ((CAdamoRSForm*)m_pRSObject)->SetPositionAbs (rc.top, rc.left, rc.right, rc.bottom);
        UpdateDiagramSize ();
    }
}

/*
** SelectRSVars :
*/
void CAdamoRSProperties::SelectRSVars ()
{
    CString strVar;
    stRSVariabili& var=((CAdamoRSControl*)m_pRSObject)->GetObjVar();
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_58));
	CCustomItemRSVar *pItem= new CCustomItemRSVar (ID_RS_VAR_NAME, var.m_strName);
	pItem->SetFlags(xtpGridItemHasExpandButton);
	pItem->SetResults (var.m_strName, var.m_nRow, var.m_strCol, var.m_nType);
	pGridItem->AddChildItem (pItem)->SetCaption (LOADSTRING (IDS_ADAMORSPROPERTIES_59));
	AddGridComboVarType (var.m_nType, pGridItem);
	strVar.Format ("%d", var.m_nRow); 
	AddGridItem(ID_RS_VAR_ROW, LOADSTRING (IDS_ADAMORSPROPERTIES_60),strVar, pGridItem);
	AddGridItem(ID_RS_VAR_COLUMN, LOADSTRING (IDS_ADAMORSPROPERTIES_61),var.m_strCol, pGridItem);
}

/*
** SelectRSMatrix :
*/
void CAdamoRSProperties::SelectRSMatrix ()
{
    CString strVar;
    stRSVariabili& var=((CAdamoRSControl*)m_pRSObject)->GetObjVar();
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_58));
	CCustomItemRSMatrix *pItem= new CCustomItemRSMatrix (ID_RS_MATRIX_NAME, var.m_strName);
	pItem->SetFlags(xtpGridItemHasExpandButton);
	pItem->SetResults (var.m_strName);
	pGridItem->AddChildItem (pItem)->SetCaption (LOADSTRING (IDS_ADAMORSPROPERTIES_84));
}

/*
** AssignRSVar : 
*/
bool CAdamoRSProperties::AssignRSVar (CXTPPropertyGridItem *pItem)
{
    CString strName, strRow, strColumn, strType;
    int nRow, nType;
    bool bAssigned=false;
    stRSVariabili& var=((CAdamoRSControl*)m_pRSObject)->GetObjVar();
    stRSVariabili  v=var;

    UINT nID=pItem->GetID ();
    if (nID==ID_RS_VAR_NAME)   {
        var.m_strName=pItem->GetValue ();
        ((CCustomItemRSVar *)pItem)->GetResults (&strName, &nRow, &strColumn, &nType);
        var.m_nRow=nRow;
        var.m_strCol=strColumn;
        var.m_nType=nType;
        CXTPPropertyGridItem* pItemRow=m_wndRSPropGrid.FindItem(ID_RS_VAR_ROW);
        if (pItemRow)   {
            strRow.Format ("%d", var.m_nRow);
            pItemRow->SetValue (strRow);
        }
        CXTPPropertyGridItem* pItemCol=m_wndRSPropGrid.FindItem(ID_RS_VAR_COLUMN);
        if (pItemCol)
            pItemCol->SetValue (var.m_strCol);
        CXTPPropertyGridItem* pItemType=m_wndRSPropGrid.FindItem(ID_RS_VAR_TYPE);
        if (pItemType)   {
            switch (var.m_nType)   {
                case LUA_TNUMBER :
                    strType = "number";
                    break;
                case LUA_TSTRING :
                    strType = "string";
                    break;
                case LUA_TBOOLEAN :
                    strType = "boolean";
                    break;
            }
            pItemType->SetValue (strType);
        }
        bAssigned=true;
    }
    if (nID==ID_RS_VAR_TYPE)   {
        CString strType=pItem->GetValue ();
        if (strType)   {
            if (strType=="number")
                var.m_nType=LUA_TNUMBER;
            if (strType=="string")
                var.m_nType=LUA_TSTRING;
            if (strType=="boolean")
                var.m_nType=LUA_TBOOLEAN;
        }
        bAssigned=true;
    }
    if (nID==ID_RS_VAR_ROW)   {
        AssignRange (pItem, &var.m_nRow, 0, 100000);
        bAssigned=true;
    }
    if (nID==ID_RS_VAR_COLUMN)   {
        var.m_strCol=pItem->GetValue ();
        bAssigned=true;
    }
    if (bAssigned)
        RemoveVariableValue (v);
    return bAssigned;
}

/*
** AssignRSMatrix : 
*/
bool CAdamoRSProperties::AssignRSMatrix (CXTPPropertyGridItem *pItem)
{
    stRSVariabili& var=((CAdamoRSControl*)m_pRSObject)->GetObjVar();
	stRSVariabili& v = var;
	bool bAssigned = false;

    UINT nID=pItem->GetID ();
    if (nID==ID_RS_MATRIX_NAME)   {
        ((CCustomItemRSMatrix *)pItem)->GetResults (&var.m_strName);
		bAssigned = true;
	}
    return bAssigned;
}

/*
** PopulateFieldCombo :
*/
void CAdamoRSProperties::PopulateFieldCombo (CString strTable)
{
    CStringList sl;
    CString str;

    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
    if (pDoc)   {
        CXTPPropertyGridItem *pItemCombo=m_wndRSPropGrid.FindItem (ID_RS_DB_FIELDS);
        if (pItemCombo)   {
            pItemCombo->SetValue ("");
            CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
            pList->RemoveAll ();
            pDoc->GetFieldsList (strTable, sl);
            for (POSITION pos=sl.GetHeadPosition (); pos; )   {
                str=sl.GetNext (pos);
	            pList->AddConstraint(str);
            }
	        pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
            pItemCombo->SetConstraintEdit(TRUE);
            pItemCombo->SetReadOnly (false);
        }
    }
}

/*
** PopulateIndexCombo :
*/
void CAdamoRSProperties::PopulateIndexCombo (CString strTable)
{
    CStringList sl;
    CString str;

    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
    if (pDoc)   {
        CXTPPropertyGridItem *pItemCombo=m_wndRSPropGrid.FindItem (ID_RS_DB_INDEX);
        if (pItemCombo)   {
            pItemCombo->SetValue ("");
            CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
            pList->RemoveAll ();
            pDoc->GetIndexList (strTable, sl);
            for (POSITION pos=sl.GetHeadPosition (); pos; )   {
                str=sl.GetNext (pos);
	            pList->AddConstraint(str);
            }
	        pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
            pItemCombo->SetConstraintEdit(TRUE);
            pItemCombo->SetReadOnly (false);
        }
    }
}

/*
** AssignRSDB : 
*/
bool CAdamoRSProperties::AssignRSDB (CXTPPropertyGridItem *pItem)
{
    bool bAssigned=false;
    stRSVariabili& var=((CAdamoRSControl*)m_pRSObject)->GetObjVar();
    UINT nID=pItem->GetID ();
    if (nID==ID_RS_DB_TABLE)   {
        var.m_strTable=pItem->GetValue ();
        if (var.m_strTable!="")   {
            PopulateFieldCombo (var.m_strTable);
            PopulateIndexCombo (var.m_strTable);
            PopulateIndexValues ();
        }
        bAssigned=true;
    }
    if (nID==ID_RS_DB_FIELDS)   {
        var.m_strField=pItem->GetValue ();
        bAssigned=true;
    }
    if (nID==ID_RS_DB_INDEX)   {
        var.m_strKeyName=pItem->GetValue ();
        bAssigned=true;
    }
    if (nID==ID_RS_DB_INDEXVALUE)   {
        var.m_strKeyValue=pItem->GetValue ();
        bAssigned=true;
    }
    return bAssigned;
}

/*
** AssignRSDBMatrix : 
*/
bool CAdamoRSProperties::AssignRSDBMatrix (CXTPPropertyGridItem *pItem)
{
	int nRow, nCols, i = 0;
    bool bAssigned=false;
    stRSVariabili& var=((CAdamoRSControl*)m_pRSObject)->GetObjVar();
	stGridColumnData gcd[MAXCOLUMNS];

    UINT nID = pItem->GetID ();
    if (nID == ID_RS_MATRIX_DB)   {
		CString strPValue = var.m_strTable;
		CString strValue = pItem->GetValue ();
        var.m_strTable=strValue;

		CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
		CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
		if (pDoc)   {
			CResourceFile *pRF = pDoc->GetRSFile ();
			if (pRF)   {
				if (strPValue != strValue)   {
					if (strPValue != "")
						pRF->DeleteTable (strPValue);
					if (strValue != "")   {
						((CAdamoRSGrid *) m_pRSObject)->GetGridData (&nRow, &nCols);
						pRF->CreateTable (strValue);
						while (i < nCols)   {
							((CAdamoRSGrid *) m_pRSObject)->GetColumnData (i, &gcd[i]);
							i++;
						}
						pRF->AddGridColumns (strValue, gcd, nCols);
					}
				}
			}
		}
        bAssigned=true;
    }
    return bAssigned;
}

/*
** SelectRSRadioButton :
*/
void CAdamoRSProperties::SelectRSRadioButton ()
{
    CString str;
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_62));
    str=((CAdamoRSInputRadio*)m_pRSObject)->GetGroup();
    AddGridItem(ID_RS_RADIO_GROUP, LOADSTRING (IDS_ADAMORSPROPERTIES_63), str, pGridItem);
    str=((CAdamoRSInputRadio*)m_pRSObject)->GetValue();
    AddGridItem(ID_RS_RADIO_VALUE, LOADSTRING (IDS_ADAMORSPROPERTIES_64), str, pGridItem);
    AddGridItemCheckBox (ID_RS_RADIO_CHECKED, LOADSTRING (IDS_ADAMORSPROPERTIES_65), ((CAdamoRSInputRadio*)m_pRSObject)->IsChecked(), pGridItem);
    AddGridItemCheckBox (ID_RS_RADIO_UPDATE, LOADSTRING (IDS_ADAMORSPROPERTIES_97), ((CAdamoRSInputCheck*)m_pRSObject)->IsAutoUpdate(), pGridItem);
    SelectRSVars ();
    SelectRSDB ();
    SelectRSStyle ();
}

/*
** SelectRSCheckBox :
*/
void CAdamoRSProperties::SelectRSCheckBox ()
{
    CString str;
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_66));
    str=((CAdamoRSInputCheck*)m_pRSObject)->GetValue();
    AddGridItem(ID_RS_CHECKBOX_VALUE, LOADSTRING (IDS_ADAMORSPROPERTIES_67), str, pGridItem);
    AddGridItemCheckBox (ID_RS_CHECKBOX_CHECKED, LOADSTRING (IDS_ADAMORSPROPERTIES_68), ((CAdamoRSInputCheck*)m_pRSObject)->IsChecked(), pGridItem);
    AddGridItemCheckBox (ID_RS_CHECKBOX_UPDATE, LOADSTRING (IDS_ADAMORSPROPERTIES_97), ((CAdamoRSInputCheck*)m_pRSObject)->IsAutoUpdate(), pGridItem);
    SelectRSVars ();
    SelectRSDB ();
    SelectRSStyle ();
}

/*
** AssignRSRadioButton :
*/
void CAdamoRSProperties::AssignRSRadioButton (CXTPPropertyGridItem *pItem)
{
    bool bAssigned=false;

    switch (pItem->GetID ())   {
        case ID_RS_RADIO_GROUP :
            ((CAdamoRSInputRadio*)m_pRSObject)->SetGroup (pItem->GetValue ()); bAssigned=true;
            break;
        case ID_RS_RADIO_VALUE :
            ((CAdamoRSInputRadio*)m_pRSObject)->SetValue (pItem->GetValue ()); bAssigned=true;
            break;
        case ID_RS_RADIO_CHECKED :
            ((CAdamoRSInputRadio*)m_pRSObject)->SetChecked (((CXTPPropertyGridItemBool*) pItem)->GetBool ()!=0); bAssigned=true;
            break;
        case ID_RS_RADIO_UPDATE :
            ((CAdamoRSInputCheck*)m_pRSObject)->SetAutoUpdate (((CXTPPropertyGridItemBool*) pItem)->GetBool ()!=0); bAssigned=true;
            break;
    }
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        bAssigned=AssignRSDB (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** AssignRSCheckBox :
*/
void CAdamoRSProperties::AssignRSCheckBox (CXTPPropertyGridItem *pItem)
{
    bool bAssigned=false;

    switch (pItem->GetID ())   {
        case ID_RS_CHECKBOX_VALUE :
            ((CAdamoRSInputCheck*)m_pRSObject)->SetValue (pItem->GetValue ()); bAssigned=true;
            break;
        case ID_RS_CHECKBOX_CHECKED :
            ((CAdamoRSInputCheck*)m_pRSObject)->SetChecked (((CXTPPropertyGridItemBool*) pItem)->GetBool ()!=0); bAssigned=true;
            break;
        case ID_RS_CHECKBOX_UPDATE :
            ((CAdamoRSInputCheck*)m_pRSObject)->SetAutoUpdate (((CXTPPropertyGridItemBool*) pItem)->GetBool ()!=0); bAssigned=true;
            break;
    }
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        bAssigned=AssignRSDB (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** SelectRSGroup :
*/
void CAdamoRSProperties::SelectRSGroup ()
{
    CString str;
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_69));
    str=((CAdamoRSGroup*)m_pRSObject)->GetValue();
    AddGridItem(ID_RS_GROUPBOX_VALUE, LOADSTRING (IDS_ADAMORSPROPERTIES_70), str, pGridItem);
    SelectRSStyle ();
}

/*
** SelectRSLabel :
*/
void CAdamoRSProperties::SelectRSLabel ()
{
    CString str;
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_71));
    str=((CAdamoRSLabel*)m_pRSObject)->GetValue();
    AddGridItem(ID_RS_LABEL_VALUE, LOADSTRING (IDS_ADAMORSPROPERTIES_72), str, pGridItem);
    SelectRSStyle ();
}

/*
** AssignRSGroup :
*/
void CAdamoRSProperties::AssignRSGroup (CXTPPropertyGridItem *pItem)
{
    bool bAssigned=false;

    switch (pItem->GetID ())   {
        case ID_RS_GROUPBOX_VALUE :
            ((CAdamoRSGroup*)m_pRSObject)->SetValue (pItem->GetValue ()); bAssigned=true;
            UpdateDiagramValue ();
            break;
    }
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** AssignRSLabel :
*/
void CAdamoRSProperties::AssignRSLabel (CXTPPropertyGridItem *pItem)
{
    bool bAssigned=false;

    switch (pItem->GetID ())   {
        case ID_RS_LABEL_VALUE :
            ((CAdamoRSLabel*)m_pRSObject)->SetValue (pItem->GetValue ()); bAssigned=true;
            UpdateDiagramValue ();
            break;
    }
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** AssignRSCombo :
*/
void CAdamoRSProperties::AssignRSCombo (CXTPPropertyGridItem *pItem)
{
    bool bAssigned;

    bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        bAssigned=AssignRSDB (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** AssignRSList :
*/
void CAdamoRSProperties::AssignRSList (CXTPPropertyGridItem *pItem)
{
    bool bAssigned=false;

    switch (pItem->GetID ())   {
        case ID_RS_LIST_MULTISEL :
            ((CAdamoRSList*)m_pRSObject)->SetMultipleSelection (((CXTPPropertyGridItemBool*) pItem)->GetBool ()!=0); bAssigned=true;
            break;
        case ID_RS_LIST_SIZE :
            ((CAdamoRSList*)m_pRSObject)->SetSize (atoi (pItem->GetValue())); bAssigned=true;
            break;
    }
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        bAssigned=AssignRSDB (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** AssignRSGrid :
*/
void CAdamoRSProperties::AssignRSGrid (CXTPPropertyGridItem *pItem)
{
    bool bAssigned;

    bAssigned=AssignRSMatrix (pItem);
    if (!bAssigned)
        bAssigned=AssignRSDBMatrix (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** OnEditListOptions :
*/
void CAdamoRSProperties::OnEditListOptions ()
{
    CAdamoRSOptions rsOpt;
    stRSOpzioni stOpzioni[MAX_RSOPTIONS];
    int nNumOptions;
    
    ((CAdamoRSList*)m_pRSObject)->GetRSOptions (stOpzioni, &nNumOptions);
    rsOpt.SetOptions (stOpzioni, nNumOptions);
    if (rsOpt.DoModal ()==IDOK)   {
        rsOpt.GetOptions (&nNumOptions);
        ((CAdamoRSList*)m_pRSObject)->DeleteAllRSOption ();
        for (int i=0; i<nNumOptions; i++)
            ((CAdamoRSList*)m_pRSObject)->AddRSOption (&stOpzioni[i], -1);
    }
}

/*
** OnEditComboOptions :
*/
void CAdamoRSProperties::OnEditComboOptions ()
{
    CAdamoRSOptions rsOpt;
    stRSOpzioni stOpzioni[MAX_RSOPTIONS];
    int nNumOptions;
    
    ((CAdamoRSCombo*)m_pRSObject)->GetRSOptions (stOpzioni, &nNumOptions);
    rsOpt.SetOptions (stOpzioni, nNumOptions);
    if (rsOpt.DoModal ()==IDOK)   {
        rsOpt.GetOptions (&nNumOptions);
        ((CAdamoRSCombo*)m_pRSObject)->DeleteAllRSOption ();
        for (int i=0; i<nNumOptions; i++)
            ((CAdamoRSCombo*)m_pRSObject)->AddRSOption (&stOpzioni[i], -1);
    }
}

/*
** CheckChars :
*/
void CAdamoRSProperties::CheckChars (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength)
{
    ((CMainFrame *) AfxGetMainWnd ())->CheckChars (pEdit, objCheck, nMaxLength);
}

/*
** CheckInput :
*/
void CAdamoRSProperties::CheckInput (int nID, CXTPPropertyGridInplaceEdit* pEdit)
{
    switch (nID)   {
        case ID_RS_NAME :
            CheckChars (pEdit, m_objCheckIdentifier, 32);
            break;
        case ID_RS_TABINDEX :
            CheckChars (pEdit, m_objCheckNumbers, 3);
            break;
        case ID_RS_ZORDER :
            CheckChars (pEdit, m_objCheckNumbers, 3);
            break;
        case ID_RS_TOP :
            CheckChars (pEdit, m_objCheckNumbers, 4);
            break;
        case ID_RS_LEFT :
            CheckChars (pEdit, m_objCheckNumbers, 4);
            break;
        case ID_RS_BOTTOM :
            CheckChars (pEdit, m_objCheckNumbers, 4);
            break;
        case ID_RS_RIGHT :
            CheckChars (pEdit, m_objCheckNumbers, 4);
            break;
        case ID_RS_BUTTON_VALUE :
            CheckChars (pEdit, m_objCheckCaption, 32);
            break;
        case ID_RS_STYLE_FORE_COLOR :
            CheckChars (pEdit, m_objCheckHex, 6);
            break;
        case ID_RS_STYLE_BACK_COLOR :
            CheckChars (pEdit, m_objCheckHex, 6);
            break;
        case ID_RS_BORDER_COLOR :
            CheckChars (pEdit, m_objCheckHex, 6);
            break;
        case ID_RS_EDIT_VALUE :
            CheckChars (pEdit, m_objCheckCaption, 32);
            break;
        case ID_RS_EDIT_SIZE :
            CheckChars (pEdit, m_objCheckNumbers, 4);
            break;
        case ID_RS_VAR_NAME :
            CheckChars (pEdit, m_objCheckIdentifier, 32);
            break;
        case ID_RS_VAR_ROW :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_VAR_COLUMN :
            CheckChars (pEdit, m_objCheckIdentifier, 32);
            break;
        case ID_RS_RADIO_GROUP :
            CheckChars (pEdit, m_objCheckIdentifier, 32);
            break;
        case ID_RS_RADIO_VALUE :
            CheckChars (pEdit, m_objCheckCaption, 32);
            break;
        case ID_RS_CHECKBOX_VALUE :
            CheckChars (pEdit, m_objCheckCaption, 32);
            break;
        case ID_RS_GROUPBOX_VALUE :
            CheckChars (pEdit, m_objCheckCaption, 32);
            break;
        case ID_RS_LABEL_VALUE :
            CheckChars (pEdit, m_objCheckCaption, 32);
            break;
        case ID_RS_DB_INDEXVALUE :
            CheckChars (pEdit, m_objCheckKeys, 32);
            break;
		case ID_RS_AG_POSMIN :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_POSMAX :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_POINTER_SIZE :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_POINTER_MARGIN :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_HUB_SIZE :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_ARC_RADIUS :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_ARC_START :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_ARC_RANGE :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_BEVEL_SIZE :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_TICK_MARGIN :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_TICK_LABEL_MARGIN :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_TMJ_NUMBER :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_TMJ_LENGHT :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_TMM_NUMBER :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_TMM_LENGHT :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_SEC_NUMBER :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_SEC5_LIMIT1 :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_SEC5_LIMIT2 :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_SEC5_LIMIT3 :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
        case ID_RS_AG_SEC5_LIMIT4 :
            CheckChars (pEdit, m_objCheckNumbers, 6);
            break;
    }    
}

/*
** CheckRange :
*/
bool CAdamoRSProperties::CheckRange (int nValue, int nRangeMin, int nRangeMax)
{
    bool b=false;

    if (nValue>=nRangeMin&&nValue<=nRangeMax)
        b=true;
    else   {
        AfxMessageBox (LOADSTRING (IDS_ADAMORSPROPERTIES_73), MB_ICONSTOP);
        m_wndRSPropGrid.SetFocus ();
    }
    return b;
}

/*
** ReloadField :
*/
void CAdamoRSProperties::ReloadField (CXTPPropertyGridItem *pItem)
{
    CString str;
    CRect rc;
    int n;

    switch (pItem->GetID())   {
        case ID_RS_TABINDEX :
            str.Format ("%d", m_pRSObject->GetTabIndex());
            pItem->SetValue (str);
            pItem->Select ();
            break;
        case ID_RS_ZORDER :
            str.Format ("%d", m_pRSObject->GetZOrder());
            pItem->SetValue (str);
            pItem->Select ();
            break;
        case ID_RS_TOP :
            if (m_pRSObject->KindOf ()==RSForm)
                rc=((CAdamoRSForm*)m_pRSObject)->GetPosition ();
            else
                rc=((CAdamoRSControl*)m_pRSObject)->GetObjStyle().GetRect();
            str.Format ("%d", rc.top);
            pItem->SetValue (str);
            pItem->Select ();
            break;
        case ID_RS_LEFT :
            if (m_pRSObject->KindOf ()==RSForm)
                rc=((CAdamoRSForm*)m_pRSObject)->GetPosition ();
            else
                rc=((CAdamoRSControl*)m_pRSObject)->GetObjStyle().GetRect();
            str.Format ("%d", rc.left);
            pItem->SetValue (str);
            pItem->Select ();
            break;
        case ID_RS_BOTTOM :
            if (m_pRSObject->KindOf ()==RSForm)
                rc=((CAdamoRSForm*)m_pRSObject)->GetPosition ();
            else
                rc=((CAdamoRSControl*)m_pRSObject)->GetObjStyle().GetRect();
            str.Format ("%d", rc.bottom);
            pItem->SetValue (str);
            pItem->Select ();
            break;
        case ID_RS_RIGHT :
            if (m_pRSObject->KindOf ()==RSForm)
                rc=((CAdamoRSForm*)m_pRSObject)->GetPosition ();
            else
                rc=((CAdamoRSControl*)m_pRSObject)->GetObjStyle().GetRect();
            str.Format ("%d", rc.right);
            pItem->SetValue (str);
            pItem->Select ();
            break;
        case ID_RS_EDIT_SIZE :
            n=((CAdamoRSInputText*) m_pRSObject)->GetSize ();
            str.Format ("%d", n);
            pItem->SetValue (str);
            pItem->Select ();
            break;
        case ID_RS_VAR_ROW :
            stRSVariabili& var=((CAdamoRSControl*)m_pRSObject)->GetObjVar();
            n=var.m_nRow;
            str.Format ("%d", n);
            pItem->SetValue (str);
            pItem->Select ();
            break;
    }
}

/*
** AssignRange :
*/
void CAdamoRSProperties::AssignRange (CXTPPropertyGridItem* pItem, int* nVar, int nRangeMin, int nRangeMax)
{
    int n;

    n=atoi (pItem->GetValue ());
    if (CheckRange (n, nRangeMin, nRangeMax))
        *nVar=atoi (pItem->GetValue ());
    else
        ReloadField (pItem);
}

/*
** AddGridComboImageID :
*/
CXTPPropertyGridItem* CAdamoRSProperties::AddGridComboImageID (CString strID, int nID, CXTPPropertyGridItem* pGridItem, CString strCaption)
{
    CXTPPropertyGridItem* pItemCombo = pGridItem->AddChildItem (new CXTPPropertyGridItem (nID, strID));
    pItemCombo->SetCaption(strCaption);
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();
    if (pView)   {
        CString strImage;
        CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
        /* leggiamo il primo record */
        if (!pView->GetFirstRSImage (strImage))   {
            do   {
	            pList->AddConstraint(strImage);
                /* andiamo a leggere i record successivi */
            } while (!pView->GetNextRSImage (strImage));
        }
    }
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboButtonType :
*/
CXTPPropertyGridItem* CAdamoRSProperties::AddGridComboButtonType (eRSButtonType e, int nID, CXTPPropertyGridItem* pGridItem)
{
	CString strValue;

	switch (e)   {
		case eStandard :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_87);
			break;
		case eBitmap :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_88);
			break;
	};
    CXTPPropertyGridItem* pItemCombo = pGridItem->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMORSPROPERTIES_11));
    CString strImage;
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    /* leggiamo il primo record */
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_87));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_88));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboEditType :
*/
CXTPPropertyGridItem* CAdamoRSProperties::AddGridComboEditType (eEditType e, int nID, CXTPPropertyGridItem* pGridItem)
{
	CString strValue;

	switch (e)   {
		case eNumbers :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_98);
			break;
		case eFloat :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_99);
			break;
		case eHex :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_100);
			break;
		case eLetters :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_101);
			break;
	};
    CXTPPropertyGridItem* pItemCombo = pGridItem->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMORSPROPERTIES_11));
    CString strImage;
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    /* leggiamo il primo record */
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_98));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_99));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_100));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_101));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AssignRSImage :
*/
void CAdamoRSProperties::AssignRSImage (CXTPPropertyGridItem *pItem)
{
    CXTPPropertyGridItem* pFindItem;
    COLORREF rgb;
    bool bAssigned=false, b;

    switch (pItem->GetID ())   {
        case ID_RS_IMAGE :
            ((CAdamoRSImage*)m_pRSObject)->SetIDImage (pItem->GetValue ()); bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_IMAGE_STRETCH :
            ((CAdamoRSImage*)m_pRSObject)->SetStretch (((CXTPPropertyGridItemBool*)pItem)->GetBool()!=0); bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_IMAGE_TRANSPARENCY :
            b = ((CXTPPropertyGridItemBool*)pItem)->GetBool()!=0;
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_IMAGE_TRANSPARENT_COLOR);
            ((CAdamoRSImage*)m_pRSObject)->SetTransparent (b); bAssigned=true;
            if (pFindItem)
			    pFindItem->SetReadOnly (!b);
            UpdateDiagramValue ();
            break;
        case ID_RS_IMAGE_TRANSPARENT_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
            ((CAdamoRSImage*)m_pRSObject)->SetTransparentColor (rgb);
            break;
    }
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** RemoveVariableValue :
*/
void CAdamoRSProperties::RemoveVariableValue (stRSVariabili& v)
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
    if (pDoc)   {
        /* cerchiamo se c'e' un record con il nome della variabile */
        if (v.m_strName!="")
            pDoc->CancellaValore (v);
    }
}

/*
** PopulateIndexValues :
*/
void CAdamoRSProperties::PopulateIndexValues ()
{
    CXTPPropertyGridItem* pEnableItem;
    pEnableItem=m_wndRSPropGrid.FindItem (ID_RS_DB_INDEXVALUE);
    if (pEnableItem)
        pEnableItem->SetReadOnly (false);
}

CXTPPropertyGridItem *CAdamoRSProperties::AddGridItemButtonText (int iId, CString strCaption, bool bOpen, CString strValue, CXTPPropertyGridItem *pItemNode)
{
    CCustomItemButtonText *pItem;
    pItem = new CCustomItemButtonText(iId, strValue, bOpen);
    pItem->SetGridView (this);
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
    return pItem;
}

/*
** SelectRSGrid :
*/
void CAdamoRSProperties::SelectRSGrid ()
{
    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_77));
    CCustomItemRSGrid *pItem= new CCustomItemRSGrid (ID_RS_GRID_DATA, "");
    pItem->SetFlags(xtpGridItemHasExpandButton);
    pGridItem->AddChildItem (pItem)->SetCaption (LOADSTRING (IDS_ADAMORSPROPERTIES_78));
    SelectRSMatrix ();
    SelectRSDBMatrix ();
    SelectRSStyle ();
}

/*
** SelectRSAx :
*/
void CAdamoRSProperties::SelectRSAx ()
{
	CString str, strAxisPath;

    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_102));
    str = ((CAdamoRSAx*)m_pRSObject)->GetName();
    strAxisPath = ((CAdamoRSAx*)m_pRSObject)->GetData ();
	AddGridComboAsse (strAxisPath, pGridItem);
    SelectRSStyle ();
}

/*
** SelectRSLed :
*/
void CAdamoRSProperties::SelectRSLed ()
{
	CString str, strAxisPath;

    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_104));
    str = ((CAdamoRSILed*)m_pRSObject)->GetName();
	/* dispositivo fisico a cui agganciarsi */
	SelectDevice (pGridItem);
	/* variabile eventuale da leggere */
    SelectRSVars ();
	/* dati generici */
    pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_127));
	AddGridComboLedStyle (ID_RS_LD_STYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_166), ((CAdamoRSILed *) m_pRSObject)->GetLedType (), pGridItem);
	AddGridItemCheckBox (ID_RS_LD_TRANSPARENT, LOADSTRING (IDS_ADAMORSPROPERTIES_115), ((CAdamoRSILed *) m_pRSObject)->GetTransparent (), pGridItem);
	AddGridItemCheckBox (ID_RS_LD_SHOWREFLECTION, LOADSTRING (IDS_ADAMORSPROPERTIES_170), ((CAdamoRSILed *) m_pRSObject)->GetShowReflection (), pGridItem);
	AddGridColor (ID_RS_LD_ACTIVE_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_171), ((CAdamoRSILed *) m_pRSObject)->GetActiveColor (), pGridItem);
	AddGridComboLedBevelStyle (ID_RS_LD_BEVEL_STYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_146), ((CAdamoRSILed *) m_pRSObject)->GetBevelStyle (), pGridItem);
	str.Format ("%d", ((CAdamoRSILed *) m_pRSObject)->GetHorizontalSpacing ());
	AddGridItem (ID_RS_LD_HORZ_SPACING, LOADSTRING (IDS_ADAMORSPROPERTIES_172), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILed *) m_pRSObject)->GetVerticalSpacing ());
	AddGridItem (ID_RS_LD_VERT_SPACING, LOADSTRING (IDS_ADAMORSPROPERTIES_173), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILed *) m_pRSObject)->GetRowCount ());
	AddGridItem (ID_RS_LD_ROW_COUNT, LOADSTRING (IDS_ADAMORSPROPERTIES_174), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILed *) m_pRSObject)->GetColCount ());
	AddGridItem (ID_RS_LD_COL_COUNT, LOADSTRING (IDS_ADAMORSPROPERTIES_175), str, pGridItem);
	AddGridComboLedBevelStyle (ID_RS_LD_BORDER_STYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_121), ((CAdamoRSILed *) m_pRSObject)->GetBorderStyle (), pGridItem);
	AddGridColor (ID_RS_LD_BACK_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_122), ((CAdamoRSILed *) m_pRSObject)->GetBackgroundColor (), pGridItem);
    SelectRSStyle ();
}

/*
** SelectRSIAngularGauge :
*/
void CAdamoRSProperties::SelectRSIAngularGauge ()
{
	CString str, strAxisPath;

    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_106));
    str = ((CAdamoRSAx*)m_pRSObject)->GetName();
	/* dispositivo fisico a cui agganciarsi */
	SelectDevice (pGridItem);
	/* variabile eventuale da leggere */
    SelectRSVars ();
	/* generic */
    pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_127));
	AddGridItemCheckBox (ID_RS_AG_TRANSPARENT, LOADSTRING (IDS_ADAMORSPROPERTIES_115), ((CAdamoRSIAngularGauge *) m_pRSObject)->IsTransparent(), pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetPosMin ());
	AddGridItem (ID_RS_AG_POSMIN, LOADSTRING (IDS_ADAMORSPROPERTIES_116), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetPosMax ());
	AddGridItem (ID_RS_AG_POSMAX, LOADSTRING (IDS_ADAMORSPROPERTIES_117), str, pGridItem);
	AddGridItemCheckBox (ID_RS_AG_SHOW_INNER_ARC, LOADSTRING (IDS_ADAMORSPROPERTIES_118), ((CAdamoRSIAngularGauge *) m_pRSObject)->IsInnerRadius(), pGridItem);
	AddGridItemCheckBox (ID_RS_AG_SHOW_OUTER_ARC, LOADSTRING (IDS_ADAMORSPROPERTIES_119), ((CAdamoRSIAngularGauge *) m_pRSObject)->IsOuterRadius(), pGridItem);
	AddGridItemCheckBox (ID_RS_AG_REVERSE_SCALE, LOADSTRING (IDS_ADAMORSPROPERTIES_120), ((CAdamoRSIAngularGauge *) m_pRSObject)->IsReverseScale(), pGridItem);
	AddGridComboBorderStyle (ID_RS_AG_BORDER_STYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_121), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetBorderStyle (), pGridItem);
	AddGridColor (ID_RS_AG_BACK_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_122), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetBackGroundColor (), pGridItem);
	/* pointers */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_128));
	AddGridComboPointerStyle (ID_RS_AG_POINTER_STYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_12), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetPointerStyle (), pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetPointerSize ());
	AddGridItem (ID_RS_AG_POINTER_SIZE, LOADSTRING (IDS_ADAMORSPROPERTIES_134), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetPointerMargin ());
	AddGridItem (ID_RS_AG_POINTER_MARGIN, LOADSTRING (IDS_ADAMORSPROPERTIES_135), str, pGridItem);
	AddGridColor (ID_RS_AG_POINTER_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_136), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetPointerColor (), pGridItem);
	/* arc */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_142));
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetArcRadius ());
	AddGridItem (ID_RS_AG_ARC_RADIUS, LOADSTRING (IDS_ADAMORSPROPERTIES_139), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetArcStartAngle ());
	AddGridItem (ID_RS_AG_ARC_START, LOADSTRING (IDS_ADAMORSPROPERTIES_140), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetArcAngle ());
	AddGridItem (ID_RS_AG_ARC_RANGE, LOADSTRING (IDS_ADAMORSPROPERTIES_141), str, pGridItem);
	/* hub */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_143));
	AddGridItemCheckBox (ID_RS_AG_HUB_SHOW, LOADSTRING (IDS_ADAMORSPROPERTIES_137), ((CAdamoRSIAngularGauge *) m_pRSObject)->IsHub(), pGridItem);
	AddGridItemCheckBox (ID_RS_AG_HUB_SHOW_3D, LOADSTRING (IDS_ADAMORSPROPERTIES_138), ((CAdamoRSIAngularGauge *) m_pRSObject)->IsHub3D (), pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetHubSize ());
	AddGridItem (ID_RS_AG_HUB_SIZE, LOADSTRING (IDS_ADAMORSPROPERTIES_134), str, pGridItem);
	AddGridColor (ID_RS_AG_HUB_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_136), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetHubColor (), pGridItem);
	/* face */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_144));
	AddGridItemCheckBox (ID_RS_AG_FACE_SHOW, LOADSTRING (IDS_ADAMORSPROPERTIES_137), ((CAdamoRSIAngularGauge *) m_pRSObject)->IsFace(), pGridItem);
	AddGridComboStyle (ID_RS_AG_FACE_STYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_145), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetFaceStyle (), pGridItem);
	AddGridComboStyle (ID_RS_AG_BEVEL_STYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_146), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetBevelStyle (), pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetBevelSize ());
	AddGridItem (ID_RS_AG_BEVEL_SIZE, LOADSTRING (IDS_ADAMORSPROPERTIES_134), str, pGridItem);
	AddGridColor (ID_RS_AG_FACE_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_136), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetFaceColor (), pGridItem);
	/* tick */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_150));
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetTickMargin ());
	AddGridItem (ID_RS_AG_TICK_MARGIN, LOADSTRING (IDS_ADAMORSPROPERTIES_135), str, pGridItem);
	/* tick label */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_151));
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetTickLabelMargin ());
	AddGridItem (ID_RS_AG_TICK_LABEL_MARGIN, LOADSTRING (IDS_ADAMORSPROPERTIES_135), str, pGridItem);
	/* tick major */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_152));
	AddGridItemCheckBox (ID_RS_AG_TMJ_SHOW, LOADSTRING (IDS_ADAMORSPROPERTIES_137), ((CAdamoRSIAngularGauge *) m_pRSObject)->IsTickMajor(), pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetTickMajorNumber ());
	AddGridItem (ID_RS_AG_TMJ_NUMBER, LOADSTRING (IDS_ADAMORSPROPERTIES_153), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetTickMajorLenght ());
	AddGridItem (ID_RS_AG_TMJ_LENGHT, LOADSTRING (IDS_ADAMORSPROPERTIES_154), str, pGridItem);
	AddGridColor (ID_RS_AG_TMJ_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_136), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetTickMajorColor (), pGridItem);
	/* tick minor */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_155));
	AddGridItemCheckBox (ID_RS_AG_TMM_SHOW, LOADSTRING (IDS_ADAMORSPROPERTIES_137), ((CAdamoRSIAngularGauge *) m_pRSObject)->IsTickMinor(), pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetTickMinorNumber ());
	AddGridItem (ID_RS_AG_TMM_NUMBER, LOADSTRING (IDS_ADAMORSPROPERTIES_153), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetTickMinorLenght ());
	AddGridItem (ID_RS_AG_TMM_LENGHT, LOADSTRING (IDS_ADAMORSPROPERTIES_154), str, pGridItem);
	AddGridColor (ID_RS_AG_TMM_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_136), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetTickMinorColor (), pGridItem);
	/* sezioni */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_156));
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetSectionsNumber ());
	AddGridColor (ID_RS_AG_SEC1_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_157), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetSection1Color (), pGridItem);
	AddGridColor (ID_RS_AG_SEC2_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_158), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetSection2Color (), pGridItem);
	AddGridColor (ID_RS_AG_SEC3_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_159), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetSection3Color (), pGridItem);
	AddGridColor (ID_RS_AG_SEC4_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_160), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetSection4Color (), pGridItem);
	AddGridColor (ID_RS_AG_SEC5_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_161), ((CAdamoRSIAngularGauge *) m_pRSObject)->GetSection5Color (), pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetEndSection1 ());
	AddGridItem (ID_RS_AG_SEC5_LIMIT1, LOADSTRING (IDS_ADAMORSPROPERTIES_162), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetEndSection2 ());
	AddGridItem (ID_RS_AG_SEC5_LIMIT2, LOADSTRING (IDS_ADAMORSPROPERTIES_163), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetEndSection3 ());
	AddGridItem (ID_RS_AG_SEC5_LIMIT3, LOADSTRING (IDS_ADAMORSPROPERTIES_164), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIAngularGauge *) m_pRSObject)->GetEndSection4 ());
	AddGridItem (ID_RS_AG_SEC5_LIMIT4, LOADSTRING (IDS_ADAMORSPROPERTIES_165), str, pGridItem);
	/* style */
	SelectRSStyle ();
}

/*
** SelectRSILinearGauge :
*/
void CAdamoRSProperties::SelectRSILinearGauge ()
{
	CString str, strAxisPath;

    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_176));
    str = ((CAdamoRSAx*)m_pRSObject)->GetName();
	/* dispositivo fisico a cui agganciarsi */
	SelectDevice (pGridItem);
	/* variabile eventuale da leggere */
    SelectRSVars ();
	/* generic */
    pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_127));
	AddGridItemCheckBox (ID_RS_LG_TRANSPARENT, LOADSTRING (IDS_ADAMORSPROPERTIES_115), ((CAdamoRSILinearGauge *) m_pRSObject)->IsTransparent(), pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetPosMin ());
	AddGridItem (ID_RS_LG_POSMIN, LOADSTRING (IDS_ADAMORSPROPERTIES_116), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetPosMax ());
	AddGridItem (ID_RS_LG_POSMAX, LOADSTRING (IDS_ADAMORSPROPERTIES_117), str, pGridItem);
	AddGridItemCheckBox (ID_RS_LG_REVERSE_SCALE, LOADSTRING (IDS_ADAMORSPROPERTIES_120), ((CAdamoRSILinearGauge *) m_pRSObject)->IsReverseScale(), pGridItem);
	AddGridComboOrientation (ID_RS_LG_ORIENTATION, LOADSTRING (IDS_ADAMORSPROPERTIES_177), ((CAdamoRSILinearGauge *) m_pRSObject)->GetOrientation (), pGridItem);
	AddGridComboOrientationTicks (ID_RS_LG_ORIENTATION_TICKS, LOADSTRING (IDS_ADAMORSPROPERTIES_178), ((CAdamoRSILinearGauge *) m_pRSObject)->GetOrientationTicks (), pGridItem);
	AddGridComboLGStyle (ID_RS_LG_STYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_183), ((CAdamoRSILinearGauge *) m_pRSObject)->GetBorderStyle (), pGridItem);
	AddGridColor (ID_RS_LG_BACK_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_122), ((CAdamoRSILinearGauge *) m_pRSObject)->GetBackGroundColor (), pGridItem);
	/* pointers */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_128));
	AddGridComboLGPointerStyle (ID_RS_LG_POINTER_STYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_128), ((CAdamoRSILinearGauge *) m_pRSObject)->GetPointerStyle (), pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetPointerSize ());
	AddGridItem (ID_RS_LG_POINTER_SIZE, LOADSTRING (IDS_ADAMORSPROPERTIES_134), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetPointerOffset ());
	AddGridItem (ID_RS_LG_POINTER_MARGIN, LOADSTRING (IDS_ADAMORSPROPERTIES_188), str, pGridItem);
	AddGridColor (ID_RS_LG_POINTER_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_136), ((CAdamoRSILinearGauge *) m_pRSObject)->GetPointerColor (), pGridItem);
	AddGridItemCheckBox (ID_RS_LG_POINTER_3D, LOADSTRING (IDS_ADAMORSPROPERTIES_189), ((CAdamoRSILinearGauge *) m_pRSObject)->GetPointer3D (), pGridItem);
	AddGridItemCheckBox (ID_RS_LG_POINTER_SCALESIDE, LOADSTRING (IDS_ADAMORSPROPERTIES_190), ((CAdamoRSILinearGauge *) m_pRSObject)->GetDrawScaleSide (), pGridItem);
	/* ticks */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_150));
	AddGridItemCheckBox (ID_RS_LG_TA_SHOW, LOADSTRING (IDS_ADAMORSPROPERTIES_191), ((CAdamoRSILinearGauge *) m_pRSObject)->IsTickAxes(), pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetTickMargin ());
	AddGridItem (ID_RS_LG_TA_MARGIN, LOADSTRING (IDS_ADAMORSPROPERTIES_135), str, pGridItem);
	/* labels */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_192));
	AddGridItemCheckBox (ID_RS_LG_LBL_SHOW, LOADSTRING (IDS_ADAMORSPROPERTIES_193), ((CAdamoRSILinearGauge *) m_pRSObject)->IsShowLabels(), pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetLabelMargin  ());
	AddGridItem (ID_RS_LG_LBL_MARGIN, LOADSTRING (IDS_ADAMORSPROPERTIES_135), str, pGridItem);
	/* tick major */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_152));
	AddGridItemCheckBox (ID_RS_LG_TMJ_SHOW, LOADSTRING (IDS_ADAMORSPROPERTIES_137), ((CAdamoRSILinearGauge *) m_pRSObject)->IsTickMajor(), pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetTickMajorNumber ());
	AddGridItem (ID_RS_LG_TMJ_NUMBER, LOADSTRING (IDS_ADAMORSPROPERTIES_153), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetTickMajorLenght ());
	AddGridItem (ID_RS_LG_TMJ_LENGHT, LOADSTRING (IDS_ADAMORSPROPERTIES_154), str, pGridItem);
	AddGridColor (ID_RS_LG_TMJ_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_136), ((CAdamoRSILinearGauge *) m_pRSObject)->GetTickMajorColor (), pGridItem);
	/* tick minor */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_155));
	AddGridItemCheckBox (ID_RS_LG_TMM_SHOW, LOADSTRING (IDS_ADAMORSPROPERTIES_137), ((CAdamoRSILinearGauge *) m_pRSObject)->IsTickMinor(), pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetTickMinorNumber ());
	AddGridItem (ID_RS_LG_TMM_NUMBER, LOADSTRING (IDS_ADAMORSPROPERTIES_153), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetTickMinorLenght ());
	AddGridItem (ID_RS_LG_TMM_LENGHT, LOADSTRING (IDS_ADAMORSPROPERTIES_154), str, pGridItem);
	AddGridColor (ID_RS_LG_TMM_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_136), ((CAdamoRSILinearGauge *) m_pRSObject)->GetTickMinorColor (), pGridItem);
	/* sezioni */
	pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_156));
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetSectionsNumber ());
	AddGridColor (ID_RS_LG_SEC1_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_157), ((CAdamoRSILinearGauge *) m_pRSObject)->GetSection1Color (), pGridItem);
	AddGridColor (ID_RS_LG_SEC2_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_158), ((CAdamoRSILinearGauge *) m_pRSObject)->GetSection2Color (), pGridItem);
	AddGridColor (ID_RS_LG_SEC3_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_159), ((CAdamoRSILinearGauge *) m_pRSObject)->GetSection3Color (), pGridItem);
	AddGridColor (ID_RS_LG_SEC4_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_160), ((CAdamoRSILinearGauge *) m_pRSObject)->GetSection4Color (), pGridItem);
	AddGridColor (ID_RS_LG_SEC5_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_161), ((CAdamoRSILinearGauge *) m_pRSObject)->GetSection5Color (), pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetEndSection1 ());
	AddGridItem (ID_RS_LG_SEC5_LIMIT1, LOADSTRING (IDS_ADAMORSPROPERTIES_162), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetEndSection2 ());
	AddGridItem (ID_RS_LG_SEC5_LIMIT2, LOADSTRING (IDS_ADAMORSPROPERTIES_163), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetEndSection3 ());
	AddGridItem (ID_RS_LG_SEC5_LIMIT3, LOADSTRING (IDS_ADAMORSPROPERTIES_164), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILinearGauge *) m_pRSObject)->GetEndSection4 ());
	AddGridItem (ID_RS_LG_SEC5_LIMIT4, LOADSTRING (IDS_ADAMORSPROPERTIES_165), str, pGridItem);
	/* style */
	SelectRSStyle ();
}

/*
** SelectRSIDisplay :
*/
void CAdamoRSProperties::SelectRSIDisplay ()
{
	CString str, strAxisPath;

    CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_194));
    str = ((CAdamoRSAx*)m_pRSObject)->GetName();
	/* dispositivo fisico a cui agganciarsi */
	SelectDevice (pGridItem);
	/* variabile eventuale da leggere */
    SelectRSVars ();
	/* generic */
    pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_127));
	AddGridItemCheckBox (ID_RS_DG_OFFSEGMENTS, LOADSTRING (IDS_ADAMORSPROPERTIES_195), ((CAdamoRSIDisplay *) m_pRSObject)->IsOffSegment (), pGridItem);
	AddGridItemCheckBox (ID_RS_DG_TRANSPARENT, LOADSTRING (IDS_ADAMORSPROPERTIES_115), ((CAdamoRSIDisplay *) m_pRSObject)->IsTransparent(), pGridItem);
	AddGridItemCheckBox (ID_RS_DG_SIGN, LOADSTRING (IDS_ADAMORSPROPERTIES_196), ((CAdamoRSIDisplay *) m_pRSObject)->IsSign (), pGridItem);
	AddGridItemCheckBox (ID_RS_DG_AUTOOFFSEGCOL, LOADSTRING (IDS_ADAMORSPROPERTIES_197), ((CAdamoRSIDisplay *) m_pRSObject)->IsAutoSegmentOffColor (), pGridItem);
	str.Format ("%d", ((CAdamoRSIDisplay *) m_pRSObject)->GetPrecision ());
	AddGridItem (ID_RS_DG_PRECISION, LOADSTRING (IDS_ADAMORSPROPERTIES_198), str, pGridItem);
	AddGridComboDisplayStyle (ID_RS_DG_BORDERSTYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_121), ((CAdamoRSIDisplay *) m_pRSObject)->GetDisplayStyle (), pGridItem);
	AddGridComboDisplayLeadingStyle (ID_RS_DG_LEADINGSTYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_199), ((CAdamoRSIDisplay *) m_pRSObject)->GetLeadingStyle (), pGridItem);
	AddGridColor (ID_RS_DG_BACK_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_122), ((CAdamoRSIDisplay *) m_pRSObject)->GetBackGroundColor (), pGridItem);
	/* display */
	str.Format ("%d", ((CAdamoRSIDisplay *) m_pRSObject)->GetDisplayCount ());
	AddGridItem (ID_RS_DD_COUNT, LOADSTRING (IDS_ADAMORSPROPERTIES_203), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIDisplay *) m_pRSObject)->GetDisplaySpacing ());
	AddGridItem (ID_RS_DD_SPACING, LOADSTRING (IDS_ADAMORSPROPERTIES_204), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIDisplay *) m_pRSObject)->GetDisplaySize ());
	AddGridItem (ID_RS_DD_SEGSIZE, LOADSTRING (IDS_ADAMORSPROPERTIES_205), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIDisplay *) m_pRSObject)->GetDisplaySeparation ());
	AddGridItem (ID_RS_DD_SEGSEPARATION, LOADSTRING (IDS_ADAMORSPROPERTIES_206), str, pGridItem);
	str.Format ("%d", ((CAdamoRSIDisplay *) m_pRSObject)->GetDisplayMargin ());
	AddGridItem (ID_RS_DD_SEGMARGIN, LOADSTRING (IDS_ADAMORSPROPERTIES_207), str, pGridItem);
	AddGridColor (ID_RS_DD_SEGON_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_208), ((CAdamoRSIDisplay *) m_pRSObject)->GetDisplayColor (), pGridItem);
	AddGridColor (ID_RS_DD_SEGOFF_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_209), ((CAdamoRSIDisplay *) m_pRSObject)->GetDisplayOffColor (), pGridItem);
	/* style */
	SelectRSStyle ();
}

/*
** SelectRSILabel :
*/
void CAdamoRSProperties::SelectRSILabel ()
{
	CString str, strAxisPath;

	CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_228));
    str = ((CAdamoRSILabel*)m_pRSObject)->GetName();
	/* dispositivo fisico a cui agganciarsi */
	SelectDevice (pGridItem);
	/* variabile eventuale da leggere */
    SelectRSVars ();
	/* generic */
    pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_127));
    AddGridItem(ID_RS_LBLG_VALUE, LOADSTRING (IDS_ADAMORSPROPERTIES_72), str, pGridItem);
	AddGridItemCheckBox (ID_RS_LBLG_TRANSPARENT, LOADSTRING (IDS_ADAMORSPROPERTIES_115), ((CAdamoRSILabel *) m_pRSObject)->IsTransparent(), pGridItem);
	AddGridItemCheckBox (ID_RS_LBLG_AUTOSIZE, LOADSTRING (IDS_ADAMORSPROPERTIES_225), ((CAdamoRSILabel *) m_pRSObject)->IsAutoSize(), pGridItem);
	AddGridComboLabelStyle (ID_RS_LBLG_BORDERSTYLE, LOADSTRING (IDS_ADAMORSPROPERTIES_183), ((CAdamoRSILabel *) m_pRSObject)->GetStyle (), pGridItem);
	AddGridComboLabelAlignment (ID_RS_LBLG_ALIGNMENT, LOADSTRING (IDS_ADAMORSPROPERTIES_215), ((CAdamoRSILabel *) m_pRSObject)->GetAlignment (), pGridItem);
	AddGridItemCheckBox (ID_RS_LBLG_SHADOW, LOADSTRING (IDS_ADAMORSPROPERTIES_219), ((CAdamoRSILabel *) m_pRSObject)->IsShadow(), pGridItem);
	str.Format ("%d", ((CAdamoRSILabel *) m_pRSObject)->GetOffsetX ());
	AddGridItem (ID_RS_LBLG_OFFX, LOADSTRING (IDS_ADAMORSPROPERTIES_220), str, pGridItem);
	str.Format ("%d", ((CAdamoRSILabel *) m_pRSObject)->GetOffsetY ());
	AddGridItem (ID_RS_LBLG_OFFY, LOADSTRING (IDS_ADAMORSPROPERTIES_221), str, pGridItem);
	AddGridColor (ID_RS_LBLG_FORECOLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_222), ((CAdamoRSILabel *) m_pRSObject)->GetColor (), pGridItem);
	AddGridColor (ID_RS_LBLG_BACKCOLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_223), ((CAdamoRSILabel *) m_pRSObject)->GetBackColor (), pGridItem);
	AddGridColor (ID_RS_LBLG_SHADOWCOLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_224), ((CAdamoRSILabel *) m_pRSObject)->GetShadowColor (), pGridItem);
	/* style */
	SelectRSStyle ();
}

/*
** SelectRSImage :
*/
void CAdamoRSProperties::SelectRSIPicture ()
{
	CXTPPropertyGridItem* pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_227));
    CString strIDImage, str;

    str = ((CAdamoRSILabel*)m_pRSObject)->GetName();
	/* dispositivo fisico a cui agganciarsi */
	SelectDevice (pGridItem);
	/* variabile eventuale da leggere */
    SelectRSVars ();
	/* generic */
    pGridItem = m_wndRSPropGrid.AddCategory(LOADSTRING (IDS_ADAMORSPROPERTIES_127));
    AddGridItemCheckBox (ID_RS_PICTURE_STRETCH, LOADSTRING (IDS_ADAMORSPROPERTIES_37), ((CAdamoRSIPicture*)m_pRSObject)->IsStretched(), pGridItem);
    AddGridItemCheckBox (ID_RS_PICTURE_TRANSPARENCY, LOADSTRING (IDS_ADAMORSPROPERTIES_226), ((CAdamoRSIPicture*)m_pRSObject)->IsTransparent (), pGridItem);
	AddGridColor (ID_RS_PICTURE_TRANSPARENT_COLOR, LOADSTRING (IDS_ADAMORSPROPERTIES_53), ((CAdamoRSIPicture *) m_pRSObject)->GetTransparentColor (), pGridItem);
    /* immagini */
    strIDImage=((CAdamoRSIPicture*)m_pRSObject)->GetImageID (0);
    str.Format ("%s %d", LOADSTRING (IDS_ADAMORSPROPERTIES_229), 0);
    AddGridComboImageID (strIDImage, ID_RS_PICTURE_1, pGridItem, str);
    strIDImage=((CAdamoRSIPicture*)m_pRSObject)->GetImageID (1);
    str.Format ("%s %d", LOADSTRING (IDS_ADAMORSPROPERTIES_229), 1);
    AddGridComboImageID (strIDImage, ID_RS_PICTURE_2, pGridItem, str);
    strIDImage=((CAdamoRSIPicture*)m_pRSObject)->GetImageID (2);
    str.Format ("%s %d", LOADSTRING (IDS_ADAMORSPROPERTIES_229), 2);
    AddGridComboImageID (strIDImage, ID_RS_PICTURE_3, pGridItem, str);
    strIDImage=((CAdamoRSIPicture*)m_pRSObject)->GetImageID (3);
    str.Format ("%s %d", LOADSTRING (IDS_ADAMORSPROPERTIES_229), 3);
    AddGridComboImageID (strIDImage, ID_RS_PICTURE_4, pGridItem, str);
    strIDImage=((CAdamoRSIPicture*)m_pRSObject)->GetImageID (4);
    str.Format ("%s %d", LOADSTRING (IDS_ADAMORSPROPERTIES_229), 4);
    AddGridComboImageID (strIDImage, ID_RS_PICTURE_5, pGridItem, str);
    strIDImage=((CAdamoRSIPicture*)m_pRSObject)->GetImageID (5);
    str.Format ("%s %d", LOADSTRING (IDS_ADAMORSPROPERTIES_229), 5);
    AddGridComboImageID (strIDImage, ID_RS_PICTURE_6, pGridItem, str);
    strIDImage=((CAdamoRSIPicture*)m_pRSObject)->GetImageID (6);
    str.Format ("%s %d", LOADSTRING (IDS_ADAMORSPROPERTIES_229), 6);
    AddGridComboImageID (strIDImage, ID_RS_PICTURE_7, pGridItem, str);
    strIDImage=((CAdamoRSIPicture*)m_pRSObject)->GetImageID (7);
    str.Format ("%s %d", LOADSTRING (IDS_ADAMORSPROPERTIES_229), 7);
    AddGridComboImageID (strIDImage, ID_RS_PICTURE_8, pGridItem, str);
    SelectRSStyle ();
}

/*
** OnEditGrid :
*/
void CAdamoRSProperties::OnEditGrid ()
{
    CAdamoRSEditGrid rsGrid;
	CStringList sl;
	CString strImage;
    int nRows, nCols, i;
	stGridColumnData gcd[MAXCOLUMNS];
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSView* pView=(CAdamoRSView*)pTmpl->GetActiveView ();

	((CAdamoRSGrid*)m_pRSObject)->GetGridData (&nRows, &nCols);
	for (i = 0; i<nCols; i++)   {
		((CAdamoRSGrid*)m_pRSObject)->GetColumnData (i, &gcd[i]);
	}
	rsGrid.SetGridData (nRows, nCols);
	for (i = 0; i<nCols; i++)
		rsGrid.SetColumnData (i, &gcd[i]);
	if (pView && !pView->GetFirstRSImage (strImage))   {
		do   {
			sl.AddTail (strImage);
			/* andiamo a leggere i record successivi */
		} while (!pView->GetNextRSImage (strImage));
	}
	rsGrid.SetImagesList (&sl);
    if (rsGrid.DoModal ()==IDOK)   {
		/* prendiamo i dati e carichiamoli nell'oggetto */
		rsGrid.GetGridData (&nRows, &nCols);
		for (i = 0; i<nCols; i++)   {
			rsGrid.GetColumnData (i, &gcd[i]);
		}
		/* gestiamo i risultati */
		ManageGridData (nRows, nCols, gcd, (CAdamoRSGrid*)m_pRSObject);
		/* aggiorniamo la vista */
		UpdateDiagramSelected ();
    }
}

/*
** OnEditMatrix :
*/
void CAdamoRSProperties::OnEditMatrix (CXTPPropertyGridInplaceButton* pButton)
{
	CShowGlobalMatrix gm;
	CString strName;

	CAdamoRSGrid *pGrid = (CAdamoRSGrid*)m_pRSObject;
	stRSVariabili &var = pGrid->GetObjVar ();
    gm.SetResults (var.m_strName);
	if (gm.DoModal () == IDOK)   {
		gm.GetResults (&var.m_strName);
		((CCustomItemRSMatrix *)pButton->GetItem ())->SetResults (var.m_strName);
		pButton->GetItem ()->OnValueChanged(var.m_strName);
	}
}

/*
** OnEditDevice :
*/
void CAdamoRSProperties::OnEditDevice (CXTPPropertyGridInplaceButton* pButton)
{
	CAdamoSelectDevice sd;
	CString strName;
	
	sd.SetPath (((CAdamoRSIAngularGauge*)m_pRSObject)->GetPath ());
	sd.SetDevice (((CAdamoRSIAngularGauge*)m_pRSObject)->GetDeviceType ());
	sd.SetID (((CAdamoRSIAngularGauge*)m_pRSObject)->GetDeviceID ());
	if (sd.DoModal () == IDOK)   {
		((CAdamoRSIAngularGauge*)m_pRSObject)->SetDevice (sd.GetPath (), sd.GetDevice (), sd.GetID(), 0);
		pButton->GetItem ()->OnValueChanged(sd.GetPath ());
	}
}

/*
** ManageGridData :
*/
void CAdamoRSProperties::ManageGridData (int nRows, int nCols, stGridColumnData *pgcd, CAdamoRSGrid *pGrid)
{
	CXTPPropertyGridItem *pItem;
	CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
	CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
	stRSVariabili& var=pGrid->GetObjVar();
	int i;

	if (pDoc)   {
		CResourceFile *pRF = pDoc->GetRSFile ();
		if (pRF)   {
			if (IsValidGrid (nRows, nCols))   {
				((CAdamoRSGrid*)m_pRSObject)->SetGridData (nRows, nCols);
				for (i = 0; i<nCols; i++)   {
					pGrid->SetColumnData (i, &pgcd[i]);
				}
				if (var.m_strTable != "")
					pRF->ManageGridColumns (var.m_strTable, pgcd, nCols);
			}
			else   {
				/* se c'e' una tabella cancelliamola */
				if (var.m_strTable != "")   {
					pRF->DeleteTable (var.m_strTable);
					pItem=m_wndRSPropGrid.FindItem (ID_RS_MATRIX_DB);
					if (pItem)
						pItem->SetValue ("");
					var.m_strTable = "";
				}
				/* se c'e' assegnata una variabile PLC, cancelliamola */
				if (var.m_strName != "")   {
					pItem=m_wndRSPropGrid.FindItem (ID_RS_MATRIX_NAME);
					if (pItem)
						pItem->SetValue ("");
					var.m_strName = "";
				}
			}
		}
	}
}

/*
** IsValidGrid :
*/
bool CAdamoRSProperties::IsValidGrid (int nRows, int nCols)
{
	return nRows > 0 && nCols > 0;
}

/*
** AddGridComboNextPage :
*/
CXTPPropertyGridItem* CAdamoRSProperties::AddGridComboNextPage (CString strPage, int nID, CXTPPropertyGridItem* pGridItem)
{
    CXTPPropertyGridItem* pItemCombo;
    CStringList sl;
    CString str;

    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
    if (pDoc)   {
        pItemCombo = pGridItem->AddChildItem (new CXTPPropertyGridItem (ID_RS_BUTTON_NEXT_PAGE, strPage));
        pItemCombo->SetCaption(LOADSTRING (IDS_ADAMORSPROPERTIES_94));
        CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
        pDoc->GetSynList (sl);
        for (POSITION pos=sl.GetHeadPosition (); pos; )   {
            str=sl.GetNext (pos);
            pList->AddConstraint(str);
        }
        pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
        pItemCombo->SetConstraintEdit(TRUE);
    }
    return pItemCombo;
}

/*
** CheckExistMainSyn :
*/
bool CAdamoRSProperties::CheckExistMainSyn ()
{
    COleSafeArray sa;
    _variant_t v;
    long i=0;
	int nTipo;
	bool b = false;

	CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    CAdamoRSDoc* pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
    if (pDoc)   {
		CResourceFile *pRF = pDoc->GetRSFile ();
		if (pRF)   {
			CADORecordset *pRS = pRF->GetRSDB ();
			if (pRS)   {
				v=(long)eDialogs;
				sa.CreateOneDim (VT_VARIANT, 1);
				sa.PutElement (&i, &v);
				pRS->m_pRecordset->PutIndex (RESINDEX_RESTIPORISORSA);
				pRS->m_pRecordset->Seek (sa, adSeekFirstEQ);
				if (!pRS->IsEOF())   {
					do   {
						pRS->GetFieldValue (FIELD_MAIN_SYN, b);
						if (b)
							break;
						pRS->MoveNext ();
						if (pRS->IsEOF())
							break;
						pRS->GetFieldValue (FIELD_TIPO, nTipo);
					}   while (nTipo==eDialogs);
				}
				else
					pRS->m_pRecordset->Requery (-1);

			}
		}
	}
	return b;
}

/*
** AddGridComboAsse :
*/
CXTPPropertyGridItem * CAdamoRSProperties::AddGridComboAsse (CString strAsse, CXTPPropertyGridItem* pGridItem)
{
	CAdamoVMachine *pMachine = (CAdamoVMachine *) GETWS ()->GetActiveProject ()->GetMachine ();
	CXTPPropertyGridItem* pItemCombo = pGridItem ->AddChildItem (new CXTPPropertyGridItem (ID_RS_AXIS_NAME, strAsse));
	CString strAxesName;

	pItemCombo->SetCaption(LOADSTRING (IDS_ADAMORSPROPERTIES_103));
	CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
	CPtrList &ptrAxesList = pMachine->GetAxesList ();
	pList->AddConstraint(_T(""));
	for (POSITION pos = ptrAxesList.GetHeadPosition (); pos;)   {
		CAdamoAxis *pAxis = (CAdamoAxis *) ptrAxesList.GetNext (pos);
		strAxesName = pAxis->GetPathName () + "." + pAxis->GetName ();
		pList->AddConstraint (strAxesName);
	}
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
	pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AssignRSAx :
*/
void CAdamoRSProperties::AssignRSAx (CXTPPropertyGridItem *pItem)
{
	CAdamoVMachine *pMachine = (CAdamoVMachine *) GETWS ()->GetActiveProject ()->GetMachine ();
	CString str, strAxesName;
	int nID = -1;
    bool bAssigned=false;

	CPtrList &ptrAxesList = pMachine->GetAxesList ();
    switch (pItem->GetID ())   {
        case ID_RS_AXIS_NAME :
			str=pItem->GetValue ();
			for (POSITION pos = ptrAxesList.GetHeadPosition (); pos;)   {
				CAdamoAxis *pAxis = (CAdamoAxis *) ptrAxesList.GetNext (pos);
				strAxesName = pAxis->GetPathName () + "." + pAxis->GetName ();
				if (strAxesName == str)   {
					nID = pAxis->GetDeviceID (); break;
				}
			}
			((CAdamoRSAx*)m_pRSObject)->SetData (str, nID);
			bAssigned = true;
			break;
    }
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** AssignRSLed :
*/
void CAdamoRSProperties::AssignRSLed (CXTPPropertyGridItem *pItem)
{
    CXTPPropertyGridItem* pFindItem;
	CString str, strAxesName;
	eLedType eLT;
	eLedStyle eLS;
	COLORREF rgb;
	int i, nID = -1;
    bool b, bFound, bAssigned=false;

	switch (pItem->GetID ())   {
		case ID_RS_DEVICE_NAME :
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_DEVICE_DATA);
			pFindItem->SetReadOnly (((CAdamoRSILed*)m_pRSObject)->GetDeviceType () != enAsse);
			pFindItem->SetValue ("");
				bAssigned = true;
			break;
		case ID_RS_DEVICE_DATA :
			str=pItem->GetValue ();
			bFound = false;
			for (i=0; i<MAXAXESDATA; i++)   {
				if (str == LOADSTRING (IDS_OSC_SAMPLES_1+i))   {
					bFound = true; break;
				}
			}
			if (!bFound)
				i = -1;
			else
				i = ((CMainFrame *) AfxGetMainWnd ())->GetStringaCodiceDatiAsse (i);
			((CAdamoRSILinearGauge*)m_pRSObject)->SetDeviceData (i);
			bAssigned=true;
		case ID_RS_LD_STYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_167))
				eLT = eRectangular;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_168))
				eLT = eDiamond;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_169))
				eLT = eCircular;
			((CAdamoRSILed*)m_pRSObject)->SetLedType (eLT);
			bAssigned=true;
			break;
		case ID_RS_LD_SHOWREFLECTION :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILed*)m_pRSObject)->SetShowReflection (b); 
			bAssigned=true;
			break;
		case ID_RS_LD_TRANSPARENT :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILed*)m_pRSObject)->SetTransparent (b); 
			bAssigned=true;
			break;
		case ID_RS_LD_ACTIVE_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILed*)m_pRSObject)->SetActiveColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_LD_BEVEL_STYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_123))
				eLS = eNoStyle;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_125))
				eLS = eLedStyleRaised;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_126))
				eLS = eLedStyleLowered;
			((CAdamoRSILed*)m_pRSObject)->SetBevelStyle (eLS);
				bAssigned = true;
			break;
		case ID_RS_LD_HORZ_SPACING :
            ((CAdamoRSILed*)m_pRSObject)->SetHorizontalSpacing (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LD_VERT_SPACING :
            ((CAdamoRSILed*)m_pRSObject)->SetVerticalSpacing (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LD_ROW_COUNT :
            ((CAdamoRSILed*)m_pRSObject)->SetRowCount (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LD_COL_COUNT :
            ((CAdamoRSILed*)m_pRSObject)->SetColCount (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LD_BORDER_STYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_123))
				eLS = eNoStyle;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_125))
				eLS = eLedStyleRaised;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_126))
				eLS = eLedStyleLowered;
			((CAdamoRSILed*)m_pRSObject)->SetBorderStyle (eLS);
			bAssigned=true;
			break;
		case ID_RS_LD_BACK_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILed*)m_pRSObject)->SetBackgroundColor (rgb);
			bAssigned=true;
			break;
	}
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** AssignRSIAngularGauge :
*/
void CAdamoRSProperties::AssignRSIAngularGauge (CXTPPropertyGridItem *pItem)
{
    CXTPPropertyGridItem* pFindItem;
	COLORREF rgb;
	CString str;
	eBorderStyle eBS;
	ePointerStyle ePS;
	eStyle eST;
	int i;
	bool b, bFound, bAssigned = false;

	switch (pItem->GetID ())   {
		case ID_RS_DEVICE_NAME :
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_DEVICE_DATA);
			pFindItem->SetReadOnly (((CAdamoRSIAngularGauge*)m_pRSObject)->GetDeviceType () != enAsse);
			pFindItem->SetValue ("");
			bAssigned=true;
			break;	
		case ID_RS_DEVICE_DATA :
			str=pItem->GetValue ();
			bFound = false;
			for (i=0; i<MAXAXESDATA; i++)   {
				if (str == LOADSTRING (IDS_OSC_SAMPLES_1+i))   {
					bFound = true; break;
				}
			}
			if (!bFound)
				i = -1;
			else
				i = ((CMainFrame *) AfxGetMainWnd ())->GetStringaCodiceDatiAsse (i);
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetDeviceData (i);
			bAssigned=true;
			break;	
		case ID_RS_AG_TRANSPARENT :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetTransparent (b); 
			bAssigned=true;
			break;
		case ID_RS_AG_POSMIN :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetPosMin (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_POSMAX :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetPosMax (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_SHOW_INNER_ARC :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIAngularGauge*)m_pRSObject)->ShowInnerRadius (b); 
			bAssigned=true;
			break;
		case ID_RS_AG_SHOW_OUTER_ARC :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIAngularGauge*)m_pRSObject)->ShowOuterRadius (b); 
			bAssigned=true;
			break;
		case ID_RS_AG_REVERSE_SCALE :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetReverseScale (b); 
			bAssigned=true;
			break;
		case ID_RS_AG_BORDER_STYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_123))
				eBS = eNoBorder;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_124))
				eBS = eBorderSunken;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_125))
				eBS = eBorderRaised;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_126))
				eBS = eBorderLowered;
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetBorderStyle (eBS);
			bAssigned=true;
			break;
		case ID_RS_AG_BACK_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetBackGroundColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_POINTER_STYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_130))
				ePS = eArrowLine;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_131))
				ePS = eArrow;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_132))
				ePS = eLine;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_133))
				ePS = eTriangle;
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetPointerStyle (ePS);
			bAssigned=true;
			break;
		case ID_RS_AG_POINTER_SIZE :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetPointerSize (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_POINTER_MARGIN :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetPointerMargin (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_POINTER_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetPointerColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_ARC_RADIUS :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetArcRadius (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_ARC_START :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetArcStartAngle (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_ARC_RANGE :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetArcAngle (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_HUB_SHOW :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIAngularGauge*)m_pRSObject)->ShowHub (b); 
			bAssigned=true;
			break;
		case ID_RS_AG_HUB_SHOW_3D :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIAngularGauge*)m_pRSObject)->ShowHub3D (b); 
			bAssigned=true;
			break;
		case ID_RS_AG_HUB_SIZE :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetHubSize (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_HUB_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetHubColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_FACE_SHOW	:
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIAngularGauge*)m_pRSObject)->ShowFace (b); 
			bAssigned=true;
			break;
		case ID_RS_AG_FACE_STYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_147))
				eST = eStyleSunken;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_148))
				eST = eStyleRaised;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_149))
				eST = eStyleFlat;
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetFaceStyle (eST);
			bAssigned=true;
			break;
		case ID_RS_AG_BEVEL_STYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_147))
				eST = eStyleSunken;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_148))
				eST = eStyleRaised;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_149))
				eST = eStyleFlat;
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetBevelStyle (eST);
			bAssigned=true;
			break;
		case ID_RS_AG_BEVEL_SIZE :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetBevelSize (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_FACE_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetFaceColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_TICK_MARGIN :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetTickMargin (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_TICK_LABEL_MARGIN :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetTickLabelMargin (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_TMJ_SHOW :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIAngularGauge*)m_pRSObject)->ShowTickMajor (b); 
			bAssigned=true;
			break;
		case ID_RS_AG_TMJ_NUMBER :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetTickMajorNumber (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_TMJ_LENGHT :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetTickMajorLenght (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_TMJ_COLOR	:
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetTickMajorColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_TMM_SHOW :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIAngularGauge*)m_pRSObject)->ShowTickMinor (b); 
			bAssigned=true;
			break;
		case ID_RS_AG_TMM_NUMBER :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetTickMinorNumber (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_TMM_LENGHT :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetTickMinorLenght (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_TMM_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetTickMinorColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_SEC_NUMBER :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetSectionsNumber (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_SEC1_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetSection1Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_SEC2_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetSection2Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_SEC3_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetSection3Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_SEC4_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetSection4Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_SEC5_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIAngularGauge*)m_pRSObject)->SetSection5Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_AG_SEC5_LIMIT1 :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetEndSection1 (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_AG_SEC5_LIMIT2 :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetEndSection2 (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_AG_SEC5_LIMIT3 :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetEndSection3 (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_AG_SEC5_LIMIT4 :
            ((CAdamoRSIAngularGauge*)m_pRSObject)->SetEndSection4 (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
	}
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
	UpdateActiveXPreview ();
}

/*
** AssignRSILinearGauge :
*/
void CAdamoRSProperties::AssignRSILinearGauge (CXTPPropertyGridItem *pItem)
{
    CXTPPropertyGridItem* pFindItem;
	eOrientation eO;
	eOrientationTicks eOT;
	eLinearGaugeStyle eST;
	eLinearGaugePointerStyle ePT;
	CString str;
	COLORREF rgb;
	int i;
	bool bFound, b, bAssigned = false;

	switch (pItem->GetID ())   {
		case ID_RS_DEVICE_NAME :
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_DEVICE_DATA);
			pFindItem->SetReadOnly (((CAdamoRSILinearGauge*)m_pRSObject)->GetDeviceType () != enAsse);
			pFindItem->SetValue ("");
			bAssigned=true;
			break;	
		case ID_RS_DEVICE_DATA :
			str=pItem->GetValue ();
			bFound = false;
			for (i=0; i<MAXAXESDATA; i++)   {
				if (str == LOADSTRING (IDS_OSC_SAMPLES_1+i))   {
					bFound = true; break;
				}
			}
			if (!bFound)
				i = -1;
			else
				i = ((CMainFrame *) AfxGetMainWnd ())->GetStringaCodiceDatiAsse (i);
			((CAdamoRSILinearGauge*)m_pRSObject)->SetDeviceData (i);
			bAssigned=true;
			break;	
		case ID_RS_LG_TRANSPARENT :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetTransparent (b); 
			bAssigned=true;
			break;
		case ID_RS_LG_POSMIN :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetPosMin (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_POSMAX :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetPosMax (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_REVERSE_SCALE :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetReverseScale (b); 
			bAssigned=true;
			break;
		case ID_RS_LG_ORIENTATION :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_179))
				eO = eHorizontal;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_180))
				eO = eVertical;
			((CAdamoRSILinearGauge*)m_pRSObject)->SetOrientation (eO);
			bAssigned=true;
			break;
		case ID_RS_LG_ORIENTATION_TICKS	:
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_181))
				eOT = eBottomLeft;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_182))
				eOT = eTopRight;
			((CAdamoRSILinearGauge*)m_pRSObject)->SetOrientationTicks (eOT);
			bAssigned=true;
			break;
		case ID_RS_LG_STYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_129))
				eST = eNoLinearGaugeStyle;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_148))
				eST = eLinearGaugeStyleRaised;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_126))
				eST = eLinearGaugeStyleLowered;
			((CAdamoRSILinearGauge*)m_pRSObject)->SetBorderStyle (eST);
			bAssigned=true;
			break;
		case ID_RS_LG_BACK_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILinearGauge*)m_pRSObject)->SetBackGroundColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_LG_POINTER_STYLE	:
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_128))
				ePT = eLinearGaugePointer;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_133))
				ePT = eLinearGaugeTriangle;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_132))
				ePT = eLinearGaugeTLine;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_184))
				ePT = eLinearGaugeColorBar;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_185))
				ePT = eLinearGaugeTube;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_186))
				ePT = eLinearGaugeBarSunken;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_187))
				ePT = eLinearGaugeBarRaised;
			((CAdamoRSILinearGauge*)m_pRSObject)->SetPointerStyle (ePT);
			bAssigned=true;
			break;
		case ID_RS_LG_POINTER_SIZE :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetPointerSize (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_POINTER_MARGIN :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetPointerOffset (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_POINTER_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILinearGauge*)m_pRSObject)->SetPointerColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_LG_POINTER_3D :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetPointer3D (b); 
			bAssigned=true;
			break;
		case ID_RS_LG_POINTER_SCALESIDE :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetDrawScaleSide (b); 
			bAssigned=true;
			break;
		case ID_RS_LG_TA_SHOW :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILinearGauge*)m_pRSObject)->ShowTickAxes (b); 
			bAssigned=true;
			break;
		case ID_RS_LG_TA_MARGIN :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetTickMargin (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_LBL_SHOW :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILinearGauge*)m_pRSObject)->ShowLabels (b); 
			bAssigned=true;
		case ID_RS_LG_LBL_MARGIN :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetLabelMargin (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_TMJ_SHOW :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILinearGauge*)m_pRSObject)->ShowTickMajor (b); 
			bAssigned=true;
			break;
		case ID_RS_LG_TMJ_NUMBER :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetTickMajorNumber (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_TMJ_LENGHT :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetTickMajorLenght (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_TMJ_COLOR	:
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILinearGauge*)m_pRSObject)->SetTickMajorColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_LG_TMM_SHOW :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILinearGauge*)m_pRSObject)->ShowTickMinor (b); 
			bAssigned=true;
			break;
		case ID_RS_LG_TMM_NUMBER :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetTickMinorNumber (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_TMM_LENGHT :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetTickMinorLenght (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_TMM_COLOR	:
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILinearGauge*)m_pRSObject)->SetTickMinorColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_LG_SEC_NUMBER :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetSectionsNumber (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_SEC1_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILinearGauge*)m_pRSObject)->SetSection1Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_LG_SEC2_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILinearGauge*)m_pRSObject)->SetSection2Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_LG_SEC3_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILinearGauge*)m_pRSObject)->SetSection3Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_LG_SEC4_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILinearGauge*)m_pRSObject)->SetSection4Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_LG_SEC5_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILinearGauge*)m_pRSObject)->SetSection5Color (rgb);
			bAssigned=true;
			break;
		case ID_RS_LG_SEC5_LIMIT1 :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetEndSection1 (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_LG_SEC5_LIMIT2 :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetEndSection2 (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_LG_SEC5_LIMIT3 :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetEndSection3 (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
		case ID_RS_LG_SEC5_LIMIT4 :
            ((CAdamoRSILinearGauge*)m_pRSObject)->SetEndSection4 (atoi (pItem->GetValue ())); 
			bAssigned=true;
			break;
	}
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
	UpdateActiveXPreview ();
}

/*
** AssignRSIDisplay :
*/
void CAdamoRSProperties::AssignRSIDisplay (CXTPPropertyGridItem *pItem)
{
    CXTPPropertyGridItem* pFindItem;
	eDisplayStyle eDS;
	eDisplayLeadingStyle eLS;
	CString str;
	COLORREF rgb;
	int i;
	bool bFound, b, bAssigned = false;

	switch (pItem->GetID ())   {
		case ID_RS_DEVICE_NAME :
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_DEVICE_DATA);
			pFindItem->SetReadOnly (((CAdamoRSIDisplay*)m_pRSObject)->GetDeviceType () != enAsse);
			pFindItem->SetValue ("");
			bAssigned=true;
			break;	
		case ID_RS_DEVICE_DATA :
			str=pItem->GetValue ();
			bFound = false;
			for (i=0; i<MAXAXESDATA; i++)   {
				if (str == LOADSTRING (IDS_OSC_SAMPLES_1+i))   {
					bFound = true; break;
				}
			}
			if (!bFound)
				i = -1;
			else
				i = ((CMainFrame *) AfxGetMainWnd ())->GetStringaCodiceDatiAsse (i);
			((CAdamoRSIDisplay*)m_pRSObject)->SetDeviceData (i);
			bAssigned=true;
			break;
		case ID_RS_DG_TRANSPARENT :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIDisplay*)m_pRSObject)->SetTransparent (b); 
			bAssigned=true;
			break;
		case ID_RS_DG_OFFSEGMENTS :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIDisplay*)m_pRSObject)->ShowOffSegment (b); 
			bAssigned=true;
			break;
		case ID_RS_DG_SIGN :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIDisplay*)m_pRSObject)->ShowSign (b); 
			bAssigned=true;
			break;
		case ID_RS_DG_AUTOOFFSEGCOL :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSIDisplay*)m_pRSObject)->SetAutoSegmentOffColor (b); 
			bAssigned=true;
			break;
		case ID_RS_DG_PRECISION	:
            ((CAdamoRSIDisplay*)m_pRSObject)->SetPrecision (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_DG_BORDERSTYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_129))
				eDS = eNoDisplayStyle;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_148))
				eDS = eDisplayStyleRaised;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_126))
				eDS = eDisplayStyleLowered;
			((CAdamoRSIDisplay*)m_pRSObject)->SetDisplayStyle (eDS);
			bAssigned=true;
			break;
		case ID_RS_DG_LEADINGSTYLE :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_200))
				eLS = eNoDisplayLeading;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_201))
				eLS = eDisplayZeroes;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_202))
				eLS = eDisplaySpaces;
			((CAdamoRSIDisplay*)m_pRSObject)->SetLeadingStyle (eLS);
			bAssigned=true;
			break;
		case ID_RS_DG_BACK_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIDisplay*)m_pRSObject)->SetBackGroundColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_DD_COUNT :
            ((CAdamoRSIDisplay*)m_pRSObject)->SetDisplayCount (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_DD_SPACING :
            ((CAdamoRSIDisplay*)m_pRSObject)->SetDisplaySpacing (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_DD_SEGSIZE :
            ((CAdamoRSIDisplay*)m_pRSObject)->SetDisplaySize (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_DD_SEGSEPARATION	:
            ((CAdamoRSIDisplay*)m_pRSObject)->SetDisplaySeparation (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_DD_SEGMARGIN	:
            ((CAdamoRSIDisplay*)m_pRSObject)->SetDisplayMargin (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_DD_SEGON_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIDisplay*)m_pRSObject)->SetDisplayColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_DD_SEGOFF_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSIDisplay*)m_pRSObject)->SetDisplayOffColor (rgb);
			bAssigned=true;
			break;
	}
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
	UpdateActiveXPreview ();
}

/*
** AssignRSILabel :
*/
void CAdamoRSProperties::AssignRSILabel (CXTPPropertyGridItem *pItem)
{
    CXTPPropertyGridItem* pFindItem;
	eLabelStyle eLS;
	eLabelAlignment eLA;
	CString str;
	COLORREF rgb;
	int i;
	bool bFound, b, bAssigned = false;

	switch (pItem->GetID ())   {
		case ID_RS_DEVICE_NAME :
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_DEVICE_DATA);
			pFindItem->SetReadOnly (((CAdamoRSILabel*)m_pRSObject)->GetDeviceType () != enAsse);
			pFindItem->SetValue ("");
			bAssigned=true;
			break;	
		case ID_RS_DEVICE_DATA :
			str=pItem->GetValue ();
			bFound = false;
			for (i=0; i<MAXAXESDATA; i++)   {
				if (str == LOADSTRING (IDS_OSC_SAMPLES_1+i))   {
					bFound = true; break;
				}
			}
			if (!bFound)
				i = -1;
			else
				i = ((CMainFrame *) AfxGetMainWnd ())->GetStringaCodiceDatiAsse (i);
			((CAdamoRSILabel*)m_pRSObject)->SetDeviceData (i);
			bAssigned=true;
			break;
		case ID_RS_LBLG_VALUE	:
            ((CAdamoRSILabel*)m_pRSObject)->SetValue (pItem->GetValue ());
			bAssigned=true;
			break;
		case ID_RS_LBLG_TRANSPARENT	:
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILabel*)m_pRSObject)->SetTransparent (b); 
			bAssigned=true;
			break;
		case ID_RS_LBLG_AUTOSIZE :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILabel*)m_pRSObject)->SetAutoSize (b); 
			bAssigned=true;
			break;
		case ID_RS_LBLG_BORDERSTYLE	:
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_211))
				eLS = eLabelNone;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_212))
				eLS = eLabelSingle;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_213))
				eLS = eLabelLowered;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_214))
				eLS = eLabelRaised;
			((CAdamoRSILabel*)m_pRSObject)->SetStyle (eLS);
			bAssigned=true;
			break;
		case ID_RS_LBLG_ALIGNMENT :
			str=pItem->GetValue ();
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_216))
				eLA = eLabelCentered;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_217))
				eLA = eLabelLeft;
			else
			if (str == LOADSTRING (IDS_ADAMORSPROPERTIES_218))
				eLA = eLabelRight;
			((CAdamoRSILabel*)m_pRSObject)->SetAlignment (eLA);
			bAssigned=true;
			break;
		case ID_RS_LBLG_SHADOW :
			b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
            ((CAdamoRSILabel*)m_pRSObject)->ShowShadow (b); 
			bAssigned=true;
			break;
		case ID_RS_LBLG_OFFX :
            ((CAdamoRSILabel*)m_pRSObject)->SetOffsetX (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_LBLG_OFFY :
            ((CAdamoRSILabel*)m_pRSObject)->SetOffsetY (atoi (pItem->GetValue ()));
			bAssigned=true;
			break;
		case ID_RS_LBLG_FORECOLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILabel*)m_pRSObject)->SetColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_LBLG_BACKCOLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILabel*)m_pRSObject)->SetBackColor (rgb);
			bAssigned=true;
			break;
		case ID_RS_LBLG_SHADOWCOLOR	:
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
			((CAdamoRSILabel*)m_pRSObject)->SetShadowColor (rgb);
			bAssigned=true;
			break;
	}
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
	UpdateActiveXPreview ();
}

/*
** AssignRSILabel :
*/
void CAdamoRSProperties::AssignRSIPicture (CXTPPropertyGridItem *pItem)
{
    CXTPPropertyGridItem* pFindItem;
    COLORREF rgb;
    CString str;
	int i;
	bool bFound, b, bAssigned = false;

    switch (pItem->GetID ())   {
		case ID_RS_DEVICE_NAME :
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_DEVICE_DATA);
			pFindItem->SetReadOnly (((CAdamoRSIPicture*)m_pRSObject)->GetDeviceType () != enAsse);
			pFindItem->SetValue ("");
			bAssigned=true;
			break;	
		case ID_RS_DEVICE_DATA :
			str=pItem->GetValue ();
			bFound = false;
			for (i=0; i<MAXAXESDATA; i++)   {
				if (str == LOADSTRING (IDS_OSC_SAMPLES_1+i))   {
					bFound = true; break;
				}
			}
			if (!bFound)
				i = -1;
			else
				i = ((CMainFrame *) AfxGetMainWnd ())->GetStringaCodiceDatiAsse (i);
			((CAdamoRSIPicture*)m_pRSObject)->SetDeviceData (i);
			bAssigned=true;
            break;
        case ID_RS_PICTURE_STRETCH:
            ((CAdamoRSIPicture*)m_pRSObject)->SetStretch (((CXTPPropertyGridItemBool*)pItem)->GetBool()!=0); bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_PICTURE_TRANSPARENCY :
            b = ((CXTPPropertyGridItemBool*)pItem)->GetBool()!=0;
			pFindItem = m_wndRSPropGrid.FindItem (ID_RS_PICTURE_TRANSPARENT_COLOR);
            ((CAdamoRSIPicture*)m_pRSObject)->SetTransparent (b); bAssigned=true;
            if (pFindItem)
			    pFindItem->SetReadOnly (!b);
            UpdateDiagramValue ();
            break;
        case ID_RS_PICTURE_TRANSPARENT_COLOR :
			rgb=((CXTPPropertyGridItemColor*) pItem)->GetColor ();
            ((CAdamoRSIPicture*)m_pRSObject)->SetTransparentColor (rgb);
            break;
        case ID_RS_PICTURE_1 :
            ((CAdamoRSIPicture*)m_pRSObject)->SetImageID (0, pItem->GetValue ()); 
            bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_PICTURE_2 :
            ((CAdamoRSIPicture*)m_pRSObject)->SetImageID (1, pItem->GetValue ()); 
            bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_PICTURE_3 :
            ((CAdamoRSIPicture*)m_pRSObject)->SetImageID (2, pItem->GetValue ()); 
            bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_PICTURE_4 :
            ((CAdamoRSIPicture*)m_pRSObject)->SetImageID (3, pItem->GetValue ()); 
            bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_PICTURE_5 :
            ((CAdamoRSIPicture*)m_pRSObject)->SetImageID (4, pItem->GetValue ()); 
            bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_PICTURE_6 :
            ((CAdamoRSIPicture*)m_pRSObject)->SetImageID (5, pItem->GetValue ()); 
            bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_PICTURE_7 :
            ((CAdamoRSIPicture*)m_pRSObject)->SetImageID (6, pItem->GetValue ()); 
            bAssigned=true;
            UpdateDiagramValue ();
            break;
        case ID_RS_PICTURE_8 :
            ((CAdamoRSIPicture*)m_pRSObject)->SetImageID (7, pItem->GetValue ()); 
            bAssigned=true;
            UpdateDiagramValue ();
            break;
    }
    if (!bAssigned)
        bAssigned=AssignRSVar (pItem);
    if (!bAssigned)
        AssignRSStyle (pItem);
}

/*
** FindDevice :
*/
bool CAdamoRSProperties::FindDevice (CString strPath, int *nID)
{
	bool b = false;

	*nID = -1;
	CAdamoCfgMachine *pMachine = (CAdamoVMachine *) GETWS ()->GetActiveProject ()->GetMachine ();
	CAdamoBase *pDevice = (CAdamoBase *)pMachine->GetItem (strPath);
	if (pDevice)   {
		*nID = pDevice->GetDeviceID ();
		b = true;
	}
	return b;
}

/*
** SelectDevice :
*/
void CAdamoRSProperties::SelectDevice (CXTPPropertyGridItem* pGridItem)
{
	CCustomItemRSDevice *pItem;
    CString strDevPath;
	enDispositivi eDev;
	int nID, nDeviceData;

	switch (m_pRSObject->KindOf ())   {
		case RSIAngularGauge :
			strDevPath = ((CAdamoRSIAngularGauge *) m_pRSObject)->GetPath ();
			eDev = ((CAdamoRSIAngularGauge *) m_pRSObject)->GetDeviceType ();
			nID = ((CAdamoRSIAngularGauge *) m_pRSObject)->GetID ();
			nDeviceData = ((CAdamoRSIAngularGauge *) m_pRSObject)->GetDeviceData ();
			break;
		case RSILinearGauge :
			strDevPath = ((CAdamoRSILinearGauge *) m_pRSObject)->GetPath ();
			eDev = ((CAdamoRSILinearGauge *) m_pRSObject)->GetDeviceType ();
			nID = ((CAdamoRSILinearGauge *) m_pRSObject)->GetID ();
			nDeviceData = ((CAdamoRSILinearGauge *) m_pRSObject)->GetDeviceData ();
			break;
		case RSILed :
			strDevPath = ((CAdamoRSILed *) m_pRSObject)->GetPath ();
			eDev = ((CAdamoRSILed *) m_pRSObject)->GetDeviceType ();
			nID = ((CAdamoRSILed *) m_pRSObject)->GetID ();
			nDeviceData = ((CAdamoRSILed *) m_pRSObject)->GetDeviceData ();
			break;
		case RSIDisplay :
			strDevPath = ((CAdamoRSIDisplay *) m_pRSObject)->GetPath ();
			eDev = ((CAdamoRSIDisplay *) m_pRSObject)->GetDeviceType ();
			nID = ((CAdamoRSIDisplay *) m_pRSObject)->GetID ();
			nDeviceData = ((CAdamoRSIDisplay *) m_pRSObject)->GetDeviceData ();
			break;
		case RSILabel :
			strDevPath = ((CAdamoRSILabel *) m_pRSObject)->GetPath ();
			eDev = ((CAdamoRSILabel *) m_pRSObject)->GetDeviceType ();
			nID = ((CAdamoRSILabel *) m_pRSObject)->GetID ();
			nDeviceData = ((CAdamoRSILabel *) m_pRSObject)->GetDeviceData ();
			break;
		case RSIPicture :
			strDevPath = ((CAdamoRSIPicture *) m_pRSObject)->GetPath ();
			eDev = ((CAdamoRSIPicture *) m_pRSObject)->GetDeviceType ();
			nID = ((CAdamoRSIPicture *) m_pRSObject)->GetID ();
			nDeviceData = ((CAdamoRSIPicture *) m_pRSObject)->GetDeviceData ();
			break;
	}
	pItem= new CCustomItemRSDevice (ID_RS_DEVICE_NAME, strDevPath);
	pItem->SetFlags(xtpGridItemHasExpandButton);
	pGridItem->AddChildItem (pItem)->SetCaption (LOADSTRING (IDS_ADAMORSPROPERTIES_103));
	CXTPPropertyGridItem *pComboItem = AddComboDeviceData (nDeviceData, pGridItem);
	if (eDev != enAsse)
		pComboItem->SetReadOnly (true);
}

/*
** AddComboDeviceData :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddComboDeviceData (int nDeviceData, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	int nIndex = ((CMainFrame *)AfxGetMainWnd ())->GetCodiceStringaDatiAsse (nDeviceData);
	strValue = LOADSTRING (IDS_OSC_SAMPLES_1+nIndex);
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (ID_RS_DEVICE_DATA, strValue));
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMORSPROPERTIES_114));
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    for (int i=0; i<MAXAXESDATA; i++)
        pList->AddConstraint(LOADSTRING (IDS_OSC_SAMPLES_1+i));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboBorderStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboBorderStyle (int nID, CString strCaption, eBorderStyle e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eNone :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_123);
			break;
		case eBorderSunken :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_124);
			break;
		case eBorderRaised :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_125);
			break;
		case eBorderLowered :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_126);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_123));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_124));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_125));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_126));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboPointerStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboPointerStyle (int nID, CString strCaption, ePointerStyle e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eArrowLine :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_130);
			break;
		case eArrow :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_131);
			break;
		case eLine :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_132);
			break;
		case eTriangle :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_133);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_130));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_131));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_132));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_133));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboStyle (int nID, CString strCaption, eStyle e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eStyleSunken :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_147);
			break;
		case eStyleRaised :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_148);
			break;
		case eStyleFlat :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_149);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_147));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_148));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_149));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboLedBevelStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboLedBevelStyle (int nID, CString strCaption, eLedStyle e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eNoStyle :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_123);
			break;
		case eLedStyleRaised :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_125);
			break;
		case eLedStyleLowered :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_126);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_123));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_125));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_126));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboLedStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboLedStyle (int nID, CString strCaption, eLedType e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eRectangular :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_167);
			break;
		case eDiamond :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_168);
			break;
		case eCircular :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_169);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_167));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_168));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_169));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboOrientation :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboOrientation (int nID, CString strCaption, eOrientation e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eHorizontal :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_179);
			break;
		case eVertical :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_180);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_179));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_180));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboOrientationTicks :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboOrientationTicks (int nID, CString strCaption, eOrientationTicks e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eBottomLeft :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_181);
			break;
		case eTopRight :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_182);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_181));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_182));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboLGStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboLGStyle (int nID, CString strCaption, eLinearGaugeStyle e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eNoLinearGaugeStyle :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_129);
			break;
		case eLinearGaugeStyleRaised :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_148);
			break;
		case eLinearGaugeStyleLowered :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_126);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_129));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_148));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_126));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboLGPointerStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboLGPointerStyle (int nID, CString strCaption, eLinearGaugePointerStyle e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eLinearGaugePointer :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_128);
			break;
		case eLinearGaugeTriangle :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_133);
			break;
		case eLinearGaugeTLine :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_132);
			break;
		case eLinearGaugeColorBar :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_184);
			break;
		case eLinearGaugeTube :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_185);
			break;
		case eLinearGaugeBarSunken :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_186);
			break;
		case eLinearGaugeBarRaised :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_187);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_128));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_133));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_132));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_184));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_185));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_186));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_187));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboDisplayStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboDisplayStyle (int nID, CString strCaption, eDisplayStyle e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eNoDisplayStyle :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_129);
			break;
		case eDisplayStyleRaised :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_148);
			break;
		case eDisplayStyleLowered :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_126);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_129));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_148));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_126));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboDisplayLeadingStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboDisplayLeadingStyle (int nID, CString strCaption, eDisplayLeadingStyle e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eNoDisplayStyle :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_200);
			break;
		case eDisplayStyleRaised :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_201);
			break;
		case eDisplayStyleLowered :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_202);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_200));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_201));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_202));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboLabelStyle :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboLabelStyle (int nID, CString strCaption, eLabelStyle e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eLabelNone :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_211);
			break;
		case eLabelSingle :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_212);
			break;
		case eLabelLowered :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_213);
			break;
		case eLabelRaised :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_214);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_211));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_212));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_213));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_214));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridComboLabelAlignment :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridComboLabelAlignment (int nID, CString strCaption, eLabelAlignment e, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

	switch (e)   {
		case eLabelCentered :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_216);
			break;
		case eLabelLeft :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_217);
			break;
		case eLabelRight :
			strValue = LOADSTRING (IDS_ADAMORSPROPERTIES_218);
			break;
	}
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (nID, strValue));
    pItemCombo->SetCaption(strCaption);
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_216));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_217));
    pList->AddConstraint(LOADSTRING (IDS_ADAMORSPROPERTIES_218));
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridColor :
*/
CXTPPropertyGridItem *CAdamoRSProperties::AddGridColor (int nID, CString strCaption, COLORREF rgb, CXTPPropertyGridItem *pItemNode)
{
	CXTPPropertyGridItem* pItem=pItemNode->AddChildItem(new CCustomItemColor(nID, rgb));
    pItem->SetCaption (strCaption);
	return pItem;
}

/*
** SetActiveXPreview :
*/
void CAdamoRSProperties::SelectActiveXPreview ()
{
	CAdamoRSActiveXPreview *pAP = ((CMainFrame *) AfxGetMainWnd ())->GetDockRSActiveXPreview ()->GetView ();
	if (pAP)
		pAP->SetRSControl (m_pRSObject);
}

/*
** UpdateActiveXPreview :
*/
void CAdamoRSProperties::UpdateActiveXPreview ()
{
	CAdamoRSActiveXPreview *pAP = ((CMainFrame *) AfxGetMainWnd ())->GetDockRSActiveXPreview ()->GetView ();
	if (pAP)
		pAP->UpdateProperties ();
}

/*
** UpdateActiveXPreviewSize :
*/
void CAdamoRSProperties::UpdateActiveXPreviewSize (CRect &rc)
{
	CAdamoRSActiveXPreview *pAP = ((CMainFrame *) AfxGetMainWnd ())->GetDockRSActiveXPreview ()->GetView ();
	if (pAP)
		pAP->UpdateSize ();
}

/*
** SelectRSForm :
*/
void CAdamoRSProperties::RenameRSForm (CString strFormName)
{
	if (m_pRSObject && m_pRSObject->KindOf ()==RSForm)   {
		CXTPPropertyGridItem* pFindItem= m_wndRSPropGrid.FindItem (ID_RS_NAME);;
		if (pFindItem)   {
			pFindItem->SetValue (strFormName);
		}
	}
}

/*
** RefreshMetrics :
*/
void CAdamoRSProperties::RefreshMetrics(AppTheme nTheme)
{
	CPaneView::RefreshMetrics(nTheme);

	switch (nTheme)
	{
		case themeVS6: m_wndRSPropGrid.SetTheme(xtpGridThemeDefault); break;
		case themeVS2003: m_wndRSPropGrid.SetTheme(xtpGridThemeOfficeXP); break;
		case themeVS2005: m_wndRSPropGrid.SetTheme(xtpGridThemeVisualStudio2005); break;
		case themeVS2008: m_wndRSPropGrid.SetTheme(xtpGridThemeVisualStudio2005); break;
		case themeVS2010: m_wndRSPropGrid.SetTheme(xtpGridThemeVisualStudio2010); break;
		case themeVS2012Light: m_wndRSPropGrid.SetTheme(xtpGridThemeVisualStudio2012Light); break;
		case themeVS2012Dark: m_wndRSPropGrid.SetTheme(xtpGridThemeVisualStudio2012Dark); break;
		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue: m_wndRSPropGrid.SetTheme(xtpGridThemeVisualStudio2015); break;
	}
}
