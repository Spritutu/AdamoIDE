// GridView.cpp : implementation file
//

#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "GridView.h"
#include "MainFrame.h"
#include "ConfTreeView.h"
#include "DeviceDefine.h"
#include "AdamoCC.h"
#include "AdamoCPDlg.h"
#include "DlgKC.h"
#include "Adamo.h"
#include <math.h>
#include "PropertyGridItemEx.h"

#define NESSUN_ASSE_MASTER      LOADSTRING (IDS_ADAMOGRIDVIEW_1)
#define NESSUN_CANALE_G         LOADSTRING (IDS_ADAMOGRIDVIEW_100)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char vettGAxes[MAX_ISO_AXES] = { 'A', 'B', 'C', 'U', 'V', 'W', 'X', 'Y', 'Z' };

//
/////////////////////////////////////////////////////////////////////////////
// CGridView
#define EQUAL_PRECISION         1e-06

IMPLEMENT_DYNCREATE(CGridView, CPaneView)

CGridView::CGridView():m_pCurrentObj(NULL), m_ePswLevel (ePswUser)
{
	m_objCheckAlphaNum.AddRange( 'a', 'z' );
	m_objCheckAlphaNum.AddRange( 'A', 'Z' );
    m_objCheckAlphaNum.AddRange ('0', '9');
	m_objCheckAlphaNum.AddChar( '_' );
	m_objCheckAlphaNum.AddChar( ' ' );
	m_objCheckAlphaNum.AddChar( '!' );
	m_objCheckAlphaNum.AddChar( '\"' );
	m_objCheckAlphaNum.AddChar( '#' );
	m_objCheckAlphaNum.AddChar( '$' );
	m_objCheckAlphaNum.AddChar( '%' );
	m_objCheckAlphaNum.AddChar( '&' );
	m_objCheckAlphaNum.AddChar( ',' );
	m_objCheckAlphaNum.AddChar( '(' );
	m_objCheckAlphaNum.AddChar( ')' );
	m_objCheckAlphaNum.AddChar( '*' );
	m_objCheckAlphaNum.AddChar( '+' );
	m_objCheckAlphaNum.AddChar( '\'');
	m_objCheckAlphaNum.AddChar( '-' );
	m_objCheckAlphaNum.AddChar( '.' );
	m_objCheckAlphaNum.AddChar( '/' );
	m_objCheckAlphaNum.AddChar( ':' );
	m_objCheckAlphaNum.AddChar( ';' );
	m_objCheckAlphaNum.AddChar( '<' );
	m_objCheckAlphaNum.AddChar( '=' );
	m_objCheckAlphaNum.AddChar( '>' );
	m_objCheckAlphaNum.AddChar( '@' );
	m_objCheckAlphaNum.AddChar( '[' );
	m_objCheckAlphaNum.AddChar( '\\' );
	m_objCheckAlphaNum.AddChar( ']' );
	m_objCheckAlphaNum.AddChar( '^' );
	m_objCheckAlphaNum.AddChar( '{' );
	m_objCheckAlphaNum.AddChar( '|' );
	m_objCheckAlphaNum.AddChar( '}' );
	m_objCheckAlphaNum.AddChar( '~' );

	m_objCheckName.AddRange( 'a', 'z' );
	m_objCheckName.AddRange( 'A', 'Z' );
    m_objCheckName.AddRange ('0', '9');
	m_objCheckName.AddChar( '_' );
	m_objCheckName.AddChar( '(' );
	m_objCheckName.AddChar( ')' );

    m_objCheckNumbers.AddRange ('0', '9');
    m_objCheckReal.AddRange ('0', '9');
	m_objCheckReal.AddChar( '+' );
	m_objCheckReal.AddChar( '-' );
	m_objCheckReal.AddChar( '.' );

}

CGridView::~CGridView()
{
}


BEGIN_MESSAGE_MAP(CGridView, CPaneView)
	//{{AFX_MSG_MAP(CGridView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridView drawing

void CGridView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CGridView diagnostics

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CView::AssertValid();
}

void CGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridView message handlers

int CGridView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	/* tool bar gridview */
	VERIFY(m_wndToolBar.CreateToolBar(WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_PANE_PROPERTIES));
	/* grid view */
	if (m_wndPropertyGrid.GetSafeHwnd() == 0){
		m_wndPropertyGrid.Create( CRect(0,0, 0, 0), this, ID_PROPERTY_GRID);
		m_wndPropertyGrid.SetOwner(this);		
        m_wndPropertyGrid.ShowToolBar();
        m_wndPropertyGrid.SetTheme(((CIdeApp* ) AfxGetApp ())->GetXTPThemePropertyGrid ());
		m_wndPropertyGrid.HighlightChangedItems(TRUE);
	}
	return 0;
}

void CGridView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_wndPropertyGrid.m_hWnd) )   {
		MoveChildren(&m_wndToolBar, &m_wndPropertyGrid, cx, cy);
		m_wndPropertyGrid.SetWindowPos(NULL, 0, 0, cx, cy, 0);
	}
}

void CGridView::SetObject(CObject *pObj)
{
    m_wndPropertyGrid.ResetContent();

    if (pObj)  {
        CProject *pPrj;
        CXTPPropertyGridItem* pDevice = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_2));
    
        if(pObj->IsKindOf(RUNTIME_CLASS(CProject)))  {
            pPrj=(CProject*)pObj;
            pObj = (CObject*)((CProject*)pObj)->GetMachineModule();
        }
        CXTPPropertyGridItem *pItem, *pItemName;
        pItemName=AddGridItem(ID_NAME,LOADSTRING (IDS_ADAMOGRIDVIEW_3),((CAdamoBase*)pObj)->GetName(),pDevice);
        GestionePsw (pItemName, ePswBuilder);
        pItem=AddGridItem(ID_DESCRIPTION,LOADSTRING (IDS_ADAMOGRIDVIEW_4),((CAdamoBase*)pObj)->GetDesc(),pDevice);
        GestionePsw (pItem, ePswBuilder);
		if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoLogicModule)))   {
            SetForModule ((CProject*) pPrj, pDevice);
			pItemName->SetReadOnly ();
		}
        else if (!pObj->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))   {
            AddGridComboPswLevel (((CAdamoBase*)pObj)->GetRAcces (), pDevice, ID_PSWLEVELR);
            AddGridComboPswLevel (((CAdamoBase*)pObj)->GetWAcces (), pDevice, ID_PSWLEVELW);
            if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoAxis)))
			SetForAxis((CAdamoAxis*)pObj,pDevice);
            else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoAnalogIn)))
                SetForAnalogIn((CAdamoAnalogIn*)pObj,pDevice);
                else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoAnalogOut)))
                    SetForAnalogOut((CAdamoAnalogOut*)pObj,pDevice);
	                else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoDgtIn)))
		                SetForDgtIn((CAdamoDgtIn*)pObj,pDevice);
		                else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoDgtOut)))
			                SetForDgtOut((CAdamoDgtOut*)pObj,pDevice);
			                else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoCounter)))
				                SetForCounter((CAdamoCounter*)pObj,pDevice);
	                            else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoTimer)))
		                            SetForTimer((CAdamoTimer*)pObj,pDevice);
        }
        pDevice->Expand();
    }
    m_pCurrentObj = pObj;
}

void CGridView::SetForAxis(CAdamoAxis *pAxis,CXTPPropertyGridItem *pRoot)
{
    CXTPPropertyGridItem* pItem;
    CXTPPropertyGridItem* pSimParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_5));
    CXTPPropertyGridItem* pBaseParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_6));
    CXTPPropertyGridItem* pIsoParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_7));
    CXTPPropertyGridItem* pMoveParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_8));
    CXTPPropertyGridItem* pInterpParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_9));
    CXTPPropertyGridItem* pOtherParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_10));
    CXTPPropertyGridItem* pAxisGantry = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_11));
    CXTPPropertyGridItem* pSplineParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_127));
    CXTPPropertyGridItem* pAxisLink = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_12));
    CXTPPropertyGridItem* pAxisEAComp = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_160));
    CXTPPropertyGridItem* pAxisCD = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_13));
    CXTPPropertyGridItem* pDisplayParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_107));
	CString str;

	pItem=AddGridItem(ID_ALIAS,LOADSTRING (IDS_ADAMOGRIDVIEW_14),pAxis->GetAlias(),pSimParam);
    GestionePsw (pItem, ePswBuilder);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_15), (enum enUnitaMisura) pAxis->GetUnit (), 2);
    pItem=AddGridItemdHighRes(ID_RESOLUTION,str,pAxis->GetResolution(),pBaseParam, FALSE, _T("%0.6f"));
    GestionePsw (pItem, ePswService);
    pItem=AddGridItem(ID_GNAME,LOADSTRING (IDS_ADAMOGRIDVIEW_16),pAxis->GetGName(),pIsoParam);
    GestionePsw (pItem, ePswBuilder);
    pItem = AddGridComboItemAxisType(pAxis->GetAxisType(),pBaseParam);
    if (GetMachineType()==enVirtual)   {
		pAxis->SetAxisType (enAsseVirtuale);
		pItem->SetValue (LOADSTRING (IDS_ADAMOGRIDVIEW_71));
		pItem->SetReadOnly ();
    }
    pItem = AddGridComboItemAxisControlType(pAxis->GetTIC (),pBaseParam);
	switch (pAxis->GetAxisType ())   {
		case enAsseContinua :
            pAxis->SetTIC (enTICVelocita);
			pItem->SetValue (LOADSTRING (IDS_ADAMOGRIDVIEW_111));
			pItem->SetReadOnly (true);
			break;
		case enAsseVirtuale :
			pAxis->SetTIC (enTICSimulato);
			pItem->SetValue (LOADSTRING (IDS_ADAMOGRIDVIEW_138));
			pItem->SetReadOnly (true);
			break;
	}
    if (pAxis->GetAxisType ()==enAsseCia402)
        AddGridComboItemAxisMovementCategory(pAxis->GetMovementCategory (),pBaseParam);
    else   {
        CXTPPropertyGridItem *pItem=AddGridItem (ID_AXISMOVETYPE, LOADSTRING (IDS_ADAMOGRIDVIEW_113), LOADSTRING (IDS_ADAMOGRIDVIEW_114), pBaseParam);
        pItem->SetReadOnly (true);
        pAxis->SetMovementCategory (enTMInterpolato);
    }
    AddGridComboItemAxesCategory(pAxis->GetAxesCategory(),pBaseParam);
    AddGridComboItemAxesUnit(pAxis->GetUnit(),pBaseParam);
    pItem=AddGridItemCheckBox(ID_INVPHASE,LOADSTRING (IDS_ADAMOGRIDVIEW_19),pAxis->GetInvPhase(),pBaseParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_INVRIF,LOADSTRING (IDS_ADAMOGRIDVIEW_20),pAxis->GetInvRif(),pBaseParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_WAITSTOPAXIS,LOADSTRING (IDS_ADAMOGRIDVIEW_21),pAxis->GetWaitStopAxis(),pBaseParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemCheckBox(ID_ENABLENICK,LOADSTRING (IDS_ADAMOGRIDVIEW_22),pAxis->GetEnableNick(),pBaseParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_ENABLEJERK,LOADSTRING (IDS_ADAMOGRIDVIEW_23),pAxis->GetEnableJerk(),pBaseParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemi(ID_RIF,LOADSTRING (IDS_ADAMOGRIDVIEW_24),(int)pAxis->GetRif(),pMoveParam);
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_25), (enUnitaMisura) pAxis->GetUnit (), 1);
    pItem=AddGridItemd(ID_SPEED,str,pAxis->GetSpeed(), pMoveParam);
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_26), (enUnitaMisura) pAxis->GetUnit (), 1);
    pItem=AddGridItemd(ID_SPEEDMAN,str,pAxis->GetSpeedMan(), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemi(ID_ACC,LOADSTRING (IDS_ADAMOGRIDVIEW_27),(int)pAxis->GetAcc(), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemi(ID_DECC,LOADSTRING (IDS_ADAMOGRIDVIEW_28),(int)pAxis->GetDec(), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_PROP,LOADSTRING (IDS_ADAMOGRIDVIEW_29),pAxis->GetProp(), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_INT,LOADSTRING (IDS_ADAMOGRIDVIEW_30),pAxis->GetInt(), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_INTKT,LOADSTRING (IDS_ADAMOGRIDVIEW_193),pAxis->GetIntKT (), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemCheckBox(ID_ENABLESWITCHEDINT,LOADSTRING (IDS_ADAMOGRIDVIEW_194),pAxis->GetEnableSwitchedInt (), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_SWITCHEDINTSPEED,LOADSTRING (IDS_ADAMOGRIDVIEW_195),pAxis->GetSwitchedIntSpeed (), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_DER,LOADSTRING (IDS_ADAMOGRIDVIEW_31),pAxis->GetDer(), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_DERN,LOADSTRING (IDS_ADAMOGRIDVIEW_196),pAxis->GetDerN (), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_FEEDFORWARD,LOADSTRING (IDS_ADAMOGRIDVIEW_32),pAxis->GetFeedForward(), pMoveParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_FEEDFORWARDACC,LOADSTRING (IDS_ADAMOGRIDVIEW_33),pAxis->GetFeedForwardAcc(), pMoveParam);
    GestionePsw (pItem, ePswService);
    AddGridComboItemAxesRampType(ID_RAMPTYPE,LOADSTRING (IDS_ADAMOGRIDVIEW_34),pAxis->GetRampType(),pMoveParam);
    CXTPPropertyGridItem *pItem_PERCRT=AddGridItemd(ID_PERCRT,LOADSTRING (IDS_ADAMOGRIDVIEW_35),pAxis->GetPercRT(), pMoveParam);
    if (pAxis->GetRampType ()!=enEsseParziale)
        pItem_PERCRT->SetReadOnly (true);
    GestionePsw (pItem_PERCRT, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_36), (enUnitaMisura) pAxis->GetUnit (), 1);
    pItem=AddGridItemd(ID_INTERSPEED,str,pAxis->GetInterSpeed(), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemi(ID_INTERACC,LOADSTRING (IDS_ADAMOGRIDVIEW_37),(int)pAxis->GetInterAcc(), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemi(ID_INTERDEC,LOADSTRING (IDS_ADAMOGRIDVIEW_38),(int)pAxis->GetInterDec(), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_INTERPROP,LOADSTRING (IDS_ADAMOGRIDVIEW_39),pAxis->GetInterProp(), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_INTERINT,LOADSTRING (IDS_ADAMOGRIDVIEW_40),pAxis->GetInterInt(), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_INTERPINTKT,LOADSTRING (IDS_ADAMOGRIDVIEW_193),pAxis->GetInterIntKT (), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemCheckBox(ID_INTERENABLESWITCHEDINT,LOADSTRING (IDS_ADAMOGRIDVIEW_194),pAxis->GetInterEnableSwitchedInt (), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_INTERSWITCHEDINTSPEED,LOADSTRING (IDS_ADAMOGRIDVIEW_195),pAxis->GetInterSwitchedIntSpeed (), pInterpParam);
    GestionePsw (pItem, ePswService);
	pItem=AddGridItemd(ID_INTERDER,LOADSTRING (IDS_ADAMOGRIDVIEW_41),pAxis->GetInterDer(), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_INTERDERN,LOADSTRING (IDS_ADAMOGRIDVIEW_196),pAxis->GetInterDerN (), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_INTERFEEDFORWARD,LOADSTRING (IDS_ADAMOGRIDVIEW_42),pAxis->GetInterFeedForward(), pInterpParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_INTERFEEDFORWARDACC,LOADSTRING (IDS_ADAMOGRIDVIEW_43),pAxis->GetInterFeedForwardAcc(), pInterpParam);
    GestionePsw (pItem, ePswService);
    AddGridComboItemAxesRampType(ID_INTERRAMPTYPE,LOADSTRING (IDS_ADAMOGRIDVIEW_44),pAxis->GetInterRampType(),pInterpParam);
    CXTPPropertyGridItem *pItem_INTERPERCRT=AddGridItemd(ID_INTERPERCRT,LOADSTRING (IDS_ADAMOGRIDVIEW_45),pAxis->GetInterPercRT(), pInterpParam);
    if (pAxis->GetInterRampType ()!=enEsseParziale)
        pItem_INTERPERCRT->SetReadOnly (true);
    GestionePsw (pItem_INTERPERCRT, ePswService);
    AddGridComboItemServoLimitType(pAxis->GetServoLimitType(),pOtherParam);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_46), (enUnitaMisura) pAxis->GetUnit (), 0);
    pItem=AddGridItemd(ID_LIMEACONC,str,pAxis->GetLimEAConc(), pOtherParam);
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_47), (enUnitaMisura) pAxis->GetUnit (), 0);
    pItem=AddGridItemd(ID_LIMEADISC,str,pAxis->GetLimEADisc(), pOtherParam);
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_137), (enUnitaMisura) pAxis->GetUnit (), 0);
    pItem=AddGridItemd(ID_LIMEAMIN,str,pAxis->GetLimEAMin(), pOtherParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemCheckBox(ID_ENABLELIMAXESPOS,LOADSTRING (IDS_ADAMOGRIDVIEW_48),pAxis->GetEnableLimAxesPos(),pOtherParam);
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_49), (enUnitaMisura) pAxis->GetUnit (), 0);
    CXTPPropertyGridItem *pItem_LIMAXISPOS = AddGridItemd(ID_LIMAXISPOS,str,pAxis->GetLimAxisPos(), pOtherParam);
    if(pItem_LIMAXISPOS)
        pItem_LIMAXISPOS->SetReadOnly(!pAxis->GetEnableLimAxesPos());
    GestionePsw (pItem_LIMAXISPOS, ePswService);

    pItem=AddGridItemCheckBox(ID_ENABLELIMAXESNEG,LOADSTRING (IDS_ADAMOGRIDVIEW_50),pAxis->GetEnableLimAxesNeg(),pOtherParam);
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_51), (enUnitaMisura) pAxis->GetUnit (), 0);
    CXTPPropertyGridItem *pItem_LIMAXISNEG = AddGridItemd(ID_LIMAXISNEG,str,pAxis->GetLimAxisNeg(), pOtherParam);
    if(pItem_LIMAXISNEG)
        pItem_LIMAXISNEG->SetReadOnly(!pAxis->GetEnableLimAxesNeg());
    GestionePsw (pItem_LIMAXISNEG, ePswService);

	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_52), (enUnitaMisura) pAxis->GetUnit (), 0);
    pItem=AddGridItemdHighRes(ID_WINPOS,str,pAxis->GetWinPos(), pOtherParam, FALSE, _T("%0.3f"));
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_53), (enUnitaMisura) pAxis->GetUnit (), 0);
    pItem=AddGridItemdHighRes(ID_WINNEG,str,pAxis->GetWinNeg(), pOtherParam, FALSE, _T("%0.3f"));
    GestionePsw (pItem, ePswService);

	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_124), (enUnitaMisura) pAxis->GetUnit (), 0);
    pItem=AddGridItemdHighRes(ID_RANGEQUOTAPOS,str,pAxis->GetRangeQuotaPos (), pOtherParam, FALSE, _T("%0.3f"));
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_125), (enUnitaMisura) pAxis->GetUnit (), 0);
    pItem=AddGridItemdHighRes(ID_RANGEQUOTANEG,str,pAxis->GetRangeQuotaNeg (), pOtherParam, FALSE, _T("%0.3f"));
    GestionePsw (pItem, ePswService);

    pItem=AddGridItemi(ID_DIMBUGINTEG,LOADSTRING (IDS_ADAMOGRIDVIEW_54),pAxis->GetDimBufferIntegrale(), pOtherParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemCheckBox(ID_WAITONACCCHANGE,LOADSTRING (IDS_ADAMOGRIDVIEW_118),pAxis->GetStopOnAccDecChange (),pOtherParam);
    GestionePsw (pItem, ePswBuilder);

	pItem = AddGridItemd(ID_KLC, LOADSTRING (IDS_ADAMOGRIDVIEW_136),pAxis->GetCostanteDiTempoAzionamento (), pOtherParam);
    GestionePsw (pItem, ePswBuilder);
	
    pItem=AddGridItemi (ID_TEMPOARRIVOINQUOTA,LOADSTRING (IDS_ADAMOGRIDVIEW_131),pAxis->GetTempoArrivoInQuota(),pOtherParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemi (ID_TEMPOASSEFERMO,LOADSTRING (IDS_ADAMOGRIDVIEW_132),pAxis->GetTempoAsseFermo (),pOtherParam);
    GestionePsw (pItem, ePswBuilder);

    pItem=AddGridItemi (ID_FILTROCOLLENCODER,LOADSTRING (IDS_ADAMOGRIDVIEW_197),pAxis->GetFiltroCollegamentoEncoder (), pOtherParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemi (ID_FILTROSERVOERROR,LOADSTRING (IDS_ADAMOGRIDVIEW_198),pAxis->GetFiltroServoError (), pOtherParam);
    GestionePsw (pItem, ePswBuilder);

	AddGridComboItemAxisMaster (pAxis, GetGantryMasterName (pAxis->GetGantryAxes ()), pAxisGantry);
    CXTPPropertyGridItem *pItem_GantryProp = AddGridItemd(ID_GANTRYPROP,LOADSTRING (IDS_ADAMOGRIDVIEW_55),pAxis->GetPropGantry (), pAxisGantry);
    CXTPPropertyGridItem *pItem_GantryDer  = AddGridItemd(ID_GANTRYDER,LOADSTRING (IDS_ADAMOGRIDVIEW_56), pAxis->GetDerGantry (), pAxisGantry);
    CXTPPropertyGridItem *pItem_GantryInt  = AddGridItemd(ID_GANTRYINT,LOADSTRING (IDS_ADAMOGRIDVIEW_57),pAxis->GetIntGantry (), pAxisGantry);
    if (pAxis->GetGantryAxes ()==_NO_GANTRY)   {
        pItem_GantryProp->SetReadOnly (false);
        pItem_GantryDer->SetReadOnly (false);
        pItem_GantryInt->SetReadOnly (false);
    }
    GestionePsw (pItem_GantryProp , ePswService);
    GestionePsw (pItem_GantryDer, ePswService);
    GestionePsw (pItem_GantryInt, ePswService);
    
    pItem=AddGridItemd(ID_ROTATIONFILTER,LOADSTRING (IDS_ADAMOGRIDVIEW_128),pAxis->GetFiltroRotanti (), pSplineParam);
    GestionePsw (pItem, ePswService);

	pItem=AddGridItemButtonText (ID_CC, LOADSTRING (IDS_ADAMOGRIDVIEW_58), pAxis, pAxisLink);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemCheckBox(ID_ENABLE_COMPENSAZIONE_GIOCO,LOADSTRING (IDS_ADAMOGRIDVIEW_121),pAxis->GetEnableRecuperoGioco(),pAxisLink);
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_122), (enUnitaMisura) pAxis->GetUnit (), 0);
    pItem = AddGridItemd(ID_COMPENSAZIONE_GIOCO, str, pAxis->GetRecuperoGioco(), pAxisLink);
    if(pItem)
        pItem->SetReadOnly(!pAxis->GetEnableRecuperoGioco());
    pItem = AddGridItemd(ID_TEMPO_COMPENSAZIONE, LOADSTRING (IDS_ADAMOGRIDVIEW_123),pAxis->GetTempoDiRecupero(), pAxisLink);
    if(pItem)
        pItem->SetReadOnly(!pAxis->GetEnableRecuperoGioco());

    pItem=AddGridItemCheckBox(ID_ENABLE_COMPENSAZIONE_EA,LOADSTRING (IDS_ADAMOGRIDVIEW_158),pAxis->GetEnableCompensazioneEA (),pAxisEAComp);
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_159), (enUnitaMisura) pAxis->GetUnit (), 0);
    pItem = AddGridItemld(ID_COMPENSAZIONE_EA, str, pAxis->GetCompensazioneEA (), pAxisEAComp);
    if(pItem)
        pItem->SetReadOnly(!pAxis->GetEnableCompensazioneEA());
    pItem = AddGridItemld(ID_COMPENSAZIONE_EAA, str, pAxis->GetCompensazioneEAA (), pAxisEAComp);
    if(pItem)
        pItem->SetReadOnly(!pAxis->GetEnableCompensazioneEA());
	
    pItem=AddGridItemCheckBox(ID_CORREZIONEDINAMICA,LOADSTRING (IDS_ADAMOGRIDVIEW_172),pAxis->GetCorrezioneDinamica (),pAxisLink);
    GestionePsw (pItem, ePswService);
	str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_135), (enUnitaMisura) pAxis->GetUnit (), 1);
	pItem = AddGridItemd(ID_VELOCITADICORREZIONE, str, pAxis->GetVelocitaDiCorrezione (), pAxisLink);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemButtonText (ID_CP, LOADSTRING (IDS_ADAMOGRIDVIEW_59), pAxis, pAxisCD);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemCheckBox(ID_DISPLAY, LOADSTRING (IDS_ADAMOGRIDVIEW_108), pAxis->GetDisplay (), pDisplayParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemi (ID_MAXPERCTORQUE, LOADSTRING (IDS_ADAMOGRIDVIEW_126), pAxis->GetPercTorque (), pDisplayParam);
    GestionePsw (pItem, ePswBuilder);

    AddGridComboItemGSlot (pAxis->GetGSlot (), pIsoParam);

    pSimParam->Expand ();
    pBaseParam->Expand ();
    pIsoParam->Expand ();
    pMoveParam->Expand ();
    pInterpParam->Expand ();
    pOtherParam->Expand ();
    pAxisGantry->Expand ();
	pSplineParam->Expand ();
    pAxisLink->Expand ();
	pAxisEAComp->Expand ();
    pAxisCD->Expand ();
    pDisplayParam->Expand ();
}

void CGridView::SetForAnalogIn(CAdamoAnalogIn *pAnalogIn,CXTPPropertyGridItem *pRoot)
{
    CXTPPropertyGridItem *pParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_61));
    CXTPPropertyGridItem *pItem;

    pItem=AddGridItemd (ID_LOGICRANGEMIN,LOADSTRING (IDS_ADAMOGRIDVIEW_105),pAnalogIn->GetLLInf (),pParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd (ID_LOGICRANGEMAX,LOADSTRING (IDS_ADAMOGRIDVIEW_106),pAnalogIn->GetLLSup (),pParam);
    GestionePsw (pItem, ePswBuilder);
    pParam->Expand ();
}

void CGridView::SetForAnalogOut(CAdamoAnalogOut *pAnalogOut,CXTPPropertyGridItem *pRoot)
{
    CXTPPropertyGridItem *pParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_61));
    CXTPPropertyGridItem *pItem;

    pItem=AddGridItemd (ID_LOGICRANGEMIN,LOADSTRING (IDS_ADAMOGRIDVIEW_105),pAnalogOut->GetLLInf (),pParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd (ID_LOGICRANGEMAX,LOADSTRING (IDS_ADAMOGRIDVIEW_106),pAnalogOut->GetLLSup (),pParam);
    GestionePsw (pItem, ePswBuilder);
    pParam->Expand ();
}

void CGridView::SetForDgtIn(CAdamoDgtIn *pDgtIn,CXTPPropertyGridItem *pRoot)
{
    CXTPPropertyGridItem *pParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_61));
    CXTPPropertyGridItem *pItem;

    pItem=AddGridItemi(ID_OUTNUMBER, LOADSTRING (IDS_ADAMOGRIDVIEW_62), pDgtIn->GetNumInput(),pParam);
    GestionePsw (pItem, ePswBuilder);
    pParam->Expand();
}

void CGridView::SetForDgtOut(CAdamoDgtOut *pDgtOut,CXTPPropertyGridItem *pRoot)
{
    CXTPPropertyGridItem *pParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_61));
    CXTPPropertyGridItem *pItem;

    pItem=AddGridItemCheckBox(ID_ISMONOSTABLE,LOADSTRING (IDS_ADAMOGRIDVIEW_63),pDgtOut->GetIsMonoStable()!=0,pParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemi(ID_OUTTIMEOUT,LOADSTRING (IDS_ADAMOGRIDVIEW_64) ,pDgtOut->GetTimeOut(),pParam);
    if (!pDgtOut->GetIsMonoStable())
        pItem->SetReadOnly (true);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemi(ID_OUTNUMBER,LOADSTRING (IDS_ADAMOGRIDVIEW_65) ,pDgtOut->GetNumOutput(),pParam);
    GestionePsw (pItem, ePswBuilder);
    pParam->Expand();
}

void CGridView::SetForCounter(CAdamoCounter *pCounter,CXTPPropertyGridItem *pRoot)
{
    CXTPPropertyGridItem *pParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_61));
    AddGridComboItemCounterType(pCounter->GetCounterType(), pParam);
    pParam->Expand();
}

void CGridView::SetForTimer(CAdamoTimer *pTimer,CXTPPropertyGridItem *pRoot)
{
    CXTPPropertyGridItem *pParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_61));
    CXTPPropertyGridItem *pItem;

    pItem=AddGridItemi(ID_PERIOD,LOADSTRING (IDS_ADAMOGRIDVIEW_66) ,pTimer->GetTime(),pParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_UPDOWN,LOADSTRING (IDS_ADAMOGRIDVIEW_67),pTimer->GetUpDown(),pParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_ENABLED,LOADSTRING (IDS_ADAMOGRIDVIEW_68),pTimer->GetEnabled(),pParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_STORED,LOADSTRING (IDS_ADAMOGRIDVIEW_69),pTimer->GetStored(),pParam);
    GestionePsw (pItem, ePswBuilder);
    pParam->Expand();
}

CXTPPropertyGridItem *CGridView::AddGridItemCheckBox(int iId,CString strCaption, BOOL bValue,CXTPPropertyGridItem *pItemNode)
{
	CXTPPropertyGridItemBool* pItemBool = (CXTPPropertyGridItemBool*)pItemNode->AddChildItem(
		new CXTPPropertyGridItemBool(strCaption, FALSE));
	pItemBool->SetBool(bValue);
	pItemBool->SetID(iId);
	pItemBool->SetCheckBoxStyle();
    return pItemBool;
}

CXTPPropertyGridItem *CGridView::AddGridItemButtonText (int iId,CString strCaption, void* p, CXTPPropertyGridItem *pItemNode)
{
    CXTPPropertyGridItem *pItem = new CXTPPropertyGridItem(iId);
    pItem->SetFlags(xtpGridItemHasExpandButton);
    pItem->SetItemData ((unsigned long) p);
    if (iId==ID_CC)
        pItem->SetValue (((CAdamoAxis*)p)->GetCCName());
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
    return pItem;
}

CXTPPropertyGridItem *CGridView::AddGridItem(int iId,CString strCaption, CString strValue, CXTPPropertyGridItem *pItemNode)
{    
    CXTPPropertyGridItem *pItem = new CXTPPropertyGridItem(iId,strValue);
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
    return pItem;
}

CXTPPropertyGridItem *CGridView::AddGridItemi(int iId,CString strCaption, int iValue,CXTPPropertyGridItem *pItemNode)
{
    CXTPPropertyGridItemNumber *pItem = new CXTPPropertyGridItemNumber(iId,iValue);
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
    return pItem;
}

CXTPPropertyGridItem *CGridView::AddGridItemd(int iId,CString strCaption, double dValue,
                                              CXTPPropertyGridItem *pItemNode,BOOL bReadOnly)
{
    CXTPPropertyGridItemDouble *pItem = new CXTPPropertyGridItemDouble(iId,dValue, _T("%0.3f"));
    pItem->SetReadOnly(bReadOnly);
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
    return pItem;
}

CXTPPropertyGridItem *CGridView::AddGridItemld(int iId,CString strCaption, double dValue,
                                              CXTPPropertyGridItem *pItemNode,BOOL bReadOnly)
{
    CXTPPropertyGridItemDouble *pItem = new CXTPPropertyGridItemDouble(iId,dValue, _T("%0.6f"));
    pItem->SetReadOnly(bReadOnly);
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
    return pItem;
}

CXTPPropertyGridItem *CGridView::AddGridItemdHighRes(int iId,CString strCaption, double dValue,
                                              CXTPPropertyGridItem *pItemNode,BOOL bReadOnly, CString strRes)
{
    CXTPPropertyGridItemDouble *pItem = new CXTPPropertyGridItemDouble(iId,dValue, strRes);
    pItem->SetReadOnly(bReadOnly);
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
    return pItem;
}

CXTPPropertyGridItem *CGridView::AddGridComboItemAxisType (int iValue,CXTPPropertyGridItem *pItemNode)
{
    CString strValue("");

    switch (iValue)   {
        case enAsseContinua:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_70);
            break;
        case enAsseVirtuale:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_71);
            break;
        case enAsseDigitale:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_104);
            break;
        case enAsseCia402:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_112);
            break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_AXISTYPE,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_17));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_70));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_71));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_104));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
	return pItemCombo;
}

CXTPPropertyGridItem* CGridView::AddGridComboItemAxisControlType (int iValue,CXTPPropertyGridItem *pItemNode)
{
    CString strValue("");

    switch (iValue)   {
		case enTICSimulato :
			strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_138);
			break;
        case enTICPosizione:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_110);
            break;
        case enTICVelocita:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_111);
            break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_AXISTIC,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_109));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_110));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_111));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_138));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
	return pItemCombo;
}

void CGridView::AddGridComboItemAxisMovementCategory (int iValue,CXTPPropertyGridItem *pItemNode)
{
    CString strValue("");

    switch (iValue)   {
        case enTMInterpolato:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_114);
            break;
        case enTMPosizionato:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_115);
            break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_AXISMOVETYPE,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_113));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_114));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_115));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}

void CGridView::AddGridComboItemGSlot (int nGSlot, CXTPPropertyGridItem *pItemNode)
{
    int i;
    char szBuffer[256];

    if (nGSlot==_NO_GSLOT)
        sprintf (szBuffer, "%s", (const char *) NESSUN_CANALE_G);
    else
        sprintf (szBuffer, "%d", nGSlot);
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_GSLOT,CString (szBuffer)));
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_99));
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    for (i=0; i<NMAX_GSLOT+1; i++)   {
        if (i==0)
            sprintf (szBuffer, "%s", (const char*) LOADSTRING (IDS_ADAMOGRIDVIEW_100));
        else
            sprintf (szBuffer, "%d", i);
	    pList->AddConstraint(szBuffer);
    }
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}

void CGridView::AddGridComboItemCounterType(int iValue,CXTPPropertyGridItem *pItemNode)
{
    CString strValue("");

    switch(iValue){
    case LUA_TNUMBER :
        strValue = "Number";
        break;
    case LUA_TBOOLEAN :
        strValue = "Boolean";
        break;
    case LUA_TSTRING :
        strValue = "String";
        break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_COUNTERTYPE,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_72));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(_T("Number"));
	pList->AddConstraint(_T("Boolean"));
	pList->AddConstraint(_T("String"));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}

void CGridView::AddGridComboItemAxesCategory(int iValue,CXTPPropertyGridItem *pItemNode)
{
    CString strValue("");

    switch(iValue){
    case enAsseLineare:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_74);
        break;
    case enAsseRotante:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_75);
        break;
	case enAsseBrandeggio :
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_130);
		break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_AXESCATEGORY,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_73));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_74));
	pList->AddConstraint(_T(LOADSTRING (IDS_ADAMOGRIDVIEW_75)));
	pList->AddConstraint(_T(LOADSTRING (IDS_ADAMOGRIDVIEW_130)));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}

void CGridView::AddGridComboItemAxesUnit(int iValue,CXTPPropertyGridItem *pItemNode)
{
    CString strValue("");

    switch(iValue){
    case enUMMmin:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_77);
		break;
    case enUMMMmin:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_180);
        break;
    case enUMinch:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_78);
        break;
    case enUMgiri:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_79);
        break;
    case enUMgradisec:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_80);
		break;
    case enUMgradimin:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_181);
        break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_UNIT,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_76));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_77));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_78));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_79));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_80));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_180));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_181));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}

void CGridView::AddGridComboItemAxesRampType(int iId,CString strCaption,int iValue,CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

    switch(iValue){
    case enLineare:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_81);
        break;
    case enEssePura:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_82);;
        break;
    case enEsseParziale:
        strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_83);;
        break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(iId,strValue));
    
    pItemCombo->SetCaption(strCaption);

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_81));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_82));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_83));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswService);
}

void CGridView::AddGridComboItemServoLimitType(int iValue,CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

    switch(iValue){
        case enSTDisabled:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_84);
            break;
        case enSTLineare:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_85);
            break;
        case enSTDinamico:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_86);
            break;
        case enSTProporzionale:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_87);
            break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_SERVOLIMITTYPE,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_88));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_84));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_85));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_86));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_87));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswService);
}

bool CGridView::IsValidName(CString strName)
{
    CAdamoGroup *pNode = (CAdamoGroup*)((CAdamoBase*)m_pCurrentObj)->GetParentObj();
	/* vediamo prima i gruppi */
	pNode->Init ();
    while(pNode->More()){
        CAdamoBase *pDev = (CAdamoBase*)pNode->GetNext();
        if(pDev->GetName() == strName)
            return false;
    }
	/* vediamo ora i dispositivi */
    pNode->DevInit();
    while(pNode->DevMore()){
        CAdamoBase *pDev = (CAdamoBase*)pNode->DevGetNext();
        if(pDev->GetName() == strName)
            return false;
    }
    return true;
}

LRESULT CGridView::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_PGN_ITEMVALUE_CHANGED)  {
        bool isValid=true;

        CXTPPropertyGridItem *pItem = (CXTPPropertyGridItem*)lParam;
        ((CXTPPropertyGridItem*)pItem)->SetValue(Trim(((CXTPPropertyGridItem*)pItem)->GetValue()));
		if (pItem && m_pCurrentObj)   {
            switch(pItem->GetID())   {
            case ID_NAME:
                {
                CString strValue = ((CXTPPropertyGridItem*)pItem)->GetValue();
				if (strValue != "")   {
					if(m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoAxis)))
						isValid = IsValidNameAxis (strValue, 0);
					else
						isValid = IsValidName (strValue);
					if (!isValid)
						AfxMessageBox(LOADSTRING (IDS_ADAMOGRIDVIEW_89));
					if(isValid)   {
						if (m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoAxis)))
							/* rinominiamo se necessario il file dei correttori */
							RinominaFileDeiCorrettori ((CAdamoAxis *)m_pCurrentObj, strValue);
						((CAdamoBase*)m_pCurrentObj)->SetName(strValue);
					}
					else   {
						MessageBeep((UINT)-1);
						((CXTPPropertyGridItem*)pItem)->SetValue(((CAdamoBase*)m_pCurrentObj)->GetName());
					}
				}
				else
					((CXTPPropertyGridItem*)pItem)->SetValue(((CAdamoBase*)m_pCurrentObj)->GetName());
                break;
                }
            case ID_DESCRIPTION:
                {
                CString strValue = ((CXTPPropertyGridItem*)pItem)->GetValue();
                ((CAdamoBase*)m_pCurrentObj)->SetDesc(strValue);
                break;
                }
            case ID_PSWLEVELR:
                {
                CString strValue = ((CXTPPropertyGridItem*)pItem)->GetValue();
                if(strValue == LOADSTRING (IDS_PSWLEVEL_UTENTE))
                    ((CAdamoBase*)m_pCurrentObj)->SetRAcces((int)ePswUser);
                else if(strValue == LOADSTRING (IDS_PSWLEVEL_MANUTENTORE))
                    ((CAdamoBase*)m_pCurrentObj)->SetRAcces((int)ePswService);
                else if(strValue == LOADSTRING (IDS_PSWLEVEL_COSTRUTTORE))
                    ((CAdamoBase*)m_pCurrentObj)->SetRAcces((int)ePswBuilder);
                else if(strValue == LOADSTRING (IDS_PSWLEVEL_KINEMA))
                    ((CAdamoBase*)m_pCurrentObj)->SetRAcces((int)ePswKinema);
                break;
                }
            case ID_PSWLEVELW:
                {
                CString strValue = ((CXTPPropertyGridItem*)pItem)->GetValue();
                if(strValue == LOADSTRING (IDS_PSWLEVEL_UTENTE))
                    ((CAdamoBase*)m_pCurrentObj)->SetWAcces((int)ePswUser);
                else if(strValue == LOADSTRING (IDS_PSWLEVEL_MANUTENTORE))
                    ((CAdamoBase*)m_pCurrentObj)->SetWAcces((int)ePswService);
                else if(strValue == LOADSTRING (IDS_PSWLEVEL_COSTRUTTORE))
                    ((CAdamoBase*)m_pCurrentObj)->SetWAcces((int)ePswBuilder);
                else if(strValue == LOADSTRING (IDS_PSWLEVEL_KINEMA))
                    ((CAdamoBase*)m_pCurrentObj)->SetWAcces((int)ePswKinema);
                break;
                }
            }        
            if(m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoLogicModule)))
                SwitchForModule(pItem);
            else if(m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoAxis)))
                SwitchForAxis(pItem);
                else if(m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoAnalogIn)))
                    SwitchForAnalogIn(pItem);
                    else if(m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoAnalogOut)))
                        SwitchForAnalogOut(pItem);
                        else if(m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoDgtIn)))
                            SwitchForDgtIn(pItem);
                            else if(m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoDgtOut)))
                                SwitchForDgtOut(pItem);
                                else if(m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoCounter)))
                                    SwitchForCounter(pItem);
                                    else if(m_pCurrentObj->IsKindOf(RUNTIME_CLASS(CAdamoTimer)))
                                        SwitchForTimer(pItem);
            if (isValid)  {
                RefreshFrame();
                SetModifiedFlag();
            }
		}
	}
	
    if (wParam == XTP_PGN_EDIT_CHANGED)  {
        CXTPPropertyGridInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPPropertyGridInplaceEdit, (CWnd*)lParam);
		if(pEdit)
            CheckInput (pEdit);
    }

	if (wParam == XTP_PGN_INPLACEBUTTONDOWN)	{
		CXTPPropertyGridInplaceButton* pButton = (CXTPPropertyGridInplaceButton*)lParam;
        if ((pButton->GetItem()->GetID() == ID_CC || 
			pButton->GetItem()->GetID() == ID_KYNEMATIC_COMPENSATIONS ||
			pButton->GetItem()->GetID() == ID_CP) && pButton->GetID() == XTP_ID_PROPERTYGRID_EXPANDBUTTON)   {
            switch (pButton->GetItem()->GetID())   {
                case ID_CC :   {
                    CCavStrzDlg dlg;
					CAdamoAxis *pAx=(CAdamoAxis*) pButton->GetItem ()->GetItemData ();
                    dlg.SetActiveAxis (pAx);
                    if (pAx->GetName ())   {
                        pAx->SetCCName (pAx->GetName ());
                        dlg.SetPathName (pAx->GetCCName ());
                        dlg.DoModal ();
		                pButton->GetItem ()->OnValueChanged( dlg.GetPathName());
		                Invalidate( FALSE );
                    }
                    else
                        AfxMessageBox (LOADSTRING (IDS_ADAMOPROPERTYGRIDITEMEX_1));
		            }
                break;
                case ID_KYNEMATIC_COMPENSATIONS :   {
					CDlgKC dlg;
					CProject* pPrj=(CProject*) pButton->GetItem ()->GetItemData ();
					double afpGeometry_XYZCB[NMAX_CORR_XYZCB];
					double afpGeometry_TRT[NMAX_CORR_TRT];
					pPrj->GetXYZCBCompensation (afpGeometry_XYZCB);
					pPrj->GetTRTCompensation (afpGeometry_TRT);
					dlg.SetKCXYZBC (afpGeometry_XYZCB);
					dlg.SetKCTRT (afpGeometry_TRT);
					if (dlg.DoModal () == IDOK)   {
						dlg.GetKCXYZBC (afpGeometry_XYZCB);
						dlg.GetKCTRT (afpGeometry_TRT);
						pPrj->SetXYZCBCompensation (afpGeometry_XYZCB);
						pPrj->SetTRTCompensation (afpGeometry_TRT);
                        SetModifiedFlag ();
					}
					break;
					}
                case ID_CP :   {
                    CAdamoCPDlg dlg;
					CAdamoAxis *pAx=(CAdamoAxis*) pButton->GetItem ()->GetItemData ();

                    dlg.SetActiveAxis (pAx);
                    dlg.DoModal ();
                    if (dlg.IsModified ())
                        SetModifiedFlag ();
                }
	        }
	    }
	}
    return 0;
}

void CGridView::SetModifiedFlag()
{
    CObject *pObj=NULL;
    CObject *pParent=((CAdamoBase*)m_pCurrentObj)->GetParentObj();

    if(!pParent)
        pObj = m_pCurrentObj;

    while(pParent){
        pObj = pParent;
        pParent=((CAdamoBase*)pParent)->GetParentObj();
    }

    if(pObj){
        CProject *pPrj = (CProject*)((CAdamoLogicModule*)pObj)->GetProject();
        pPrj->SetMachineModifiedFlag (true);
    }
}

void CGridView::CheckInput (CXTPPropertyGridInplaceEdit* pEdit)
{
    switch(pEdit->GetItem()->GetID())   {
        case ID_NAME:
            StringControlName(pEdit,m_objCheckName,MAXNAMELENGTH);
            break;
        case ID_DESCRIPTION:
            CheckChars(pEdit, m_objCheckAlphaNum, MAXDESCLENGTH);
            break;
        case ID_ALIAS:
            CheckChars(pEdit, m_objCheckName, MAXNAMELENGTH);
            break;
        case ID_GNAME:
            IsoAxisControl(pEdit);
            break;
        case ID_RESOLUTION:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_SPEED:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_SPEEDMAN:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_ACC:
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
        case ID_DECC:
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
        case ID_PROP:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_INT:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_DER:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_FEEDFORWARD:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_FEEDFORWARDACC:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_RIF:
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
        case ID_INTERSPEED:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_INTERACC:
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
        case ID_INTERDEC:
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
        case ID_INTERPROP:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_INTERINT:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_INTERDER:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_INTERFEEDFORWARD:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_INTERFEEDFORWARDACC:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_LIMEACONC:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_LIMEADISC:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_LIMEAMIN:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_LIMAXISPOS:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_LIMAXISNEG:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_WINPOS:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_WINNEG:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_RANGEQUOTAPOS:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_RANGEQUOTANEG:
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_OUTNUMBER:
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
        case ID_OUTTIMEOUT:
            CheckChars (pEdit, m_objCheckNumbers, 5);
            break;
        case ID_PERIOD:
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
        case ID_PERCRT :
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_INTERPERCRT :
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
        case ID_DIMBUGINTEG :
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
		case ID_5ASSI_SPEED :
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
		case ID_5ASSI_ACC :
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
		case ID_5ASSI_DEC :
            CheckChars (pEdit, m_objCheckNumbers, 12);
            break;
		case ID_5ASSI_BOW_THICKENING :
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
		case ID_5ASSI_SINGLESPIKESMOOTHERWEIGHT :
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
		case ID_5ASSI_SINGLESPIKEFACTOR :
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
		case ID_5ASSI_GEOMETRYSPIKEFACTOR :
            CheckChars (pEdit, m_objCheckReal, 12);
            break;
		case ID_RATE :
            CheckChars (pEdit, m_objCheckNumbers, 12);
			break;
		case ID_PRECISION :
            CheckChars (pEdit, m_objCheckReal, 12);
			break;
        case ID_RADIUSPRECISION :
            CheckChars (pEdit, m_objCheckReal, 12);
			break;
		case ID_KAC :
            CheckChars (pEdit, m_objCheckReal, 12);
			break;
		case ID_CONTOURANGLE :
            CheckChars (pEdit, m_objCheckReal, 12);
			break;
		case ID_5ASSI_MINSTOPANGLE :
            CheckChars (pEdit, m_objCheckReal, 12);
			break;
		case ID_5ASSI_SIP :
            CheckChars (pEdit, m_objCheckReal, 12);
			break;
		case ID_MAXJITTER :
            CheckChars (pEdit, m_objCheckNumbers, 12);
			break;
		case ID_G0RAGGIATA_RAGGIO :
            CheckChars (pEdit, m_objCheckReal, 12);
			break;
		case ID_G0RAGGIATA_INFITTIMENTO :
            CheckChars (pEdit, m_objCheckReal, 12);
			break;
    }
}

void CGridView::StringControlName(CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength)
{
   	CString str;
	pEdit->CEdit::GetWindowText(str);		
    BOOL bError = FALSE;
    int nRange = 0;    

    if(m_pCurrentObj){
        CString strPath = ((CAdamoBase*)m_pCurrentObj)->GetPathName();
        if(strPath.GetLength()+str.GetLength() > MAXPATH){
            bError = TRUE;
            nRange = MAXPATH-strPath.GetLength();
        }
    }

    CheckChars (pEdit,objCheck,nMaxLength);
}

void CGridView::IsoAxisControl(CXTPPropertyGridInplaceEdit* pEdit)
{
   	CString str;
    int nError = 0;

	pEdit->CEdit::GetWindowText(str);		
    if (str=="")
        return;
    if (str.GetLength ()>1)
        nError=1;
    else
        if (!IsValidIsoAxis (str[0]))
            nError=2;
    if (nError)   {
		MessageBeep((UINT)-1);
		pEdit->SetSel(0, -1);
        if (nError==1)
            pEdit->ReplaceSel(str.Left(1));
        else
            pEdit->ReplaceSel("");
    }
}

bool CGridView::IsValidIsoAxis (char chIsoAxisName)
{
    bool nB=false;

    for (int i=0; i<sizeof (vettGAxes); i++)
        if (toupper (vettGAxes[i])==chIsoAxisName)   {
            nB=true; break;
    }
    return nB;
}

void CGridView::SwitchForAxis(CXTPPropertyGridItem *pItem)
{
    CString strValue = pItem->GetValue();
    double fp;
    int n;
    BOOL isValid=TRUE, bNoSlot;
    char *p;

    switch(pItem->GetID())   {
        case ID_AXISVIRTUAL :
        case ID_TICVELOCITA :
            pItem->SetReadOnly (true);
            break;
        case ID_ALIAS:
            isValid = IsValidNameAxis (strValue, 2);
            if (!isValid)   {
                AfxMessageBox(LOADSTRING (IDS_ADAMOGRIDVIEW_90));
                pItem->SetValue(((CAdamoAxis*)m_pCurrentObj)->GetAlias());
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetAlias(strValue);
            break;                           
        case ID_GNAME:
			isValid=true; bNoSlot=false;
			if (strValue!="")
				isValid = IsValidNameAxis (strValue, 1, ((CAdamoAxis*)m_pCurrentObj)->GetGSlot ());
			else
				bNoSlot=true;
            if (!isValid)   {
                AfxMessageBox(LOADSTRING (IDS_ADAMOGRIDVIEW_91));
                pItem->SetValue(((CAdamoAxis*)m_pCurrentObj)->GetGName());
            }
			else   {
                ((CAdamoAxis*)m_pCurrentObj)->SetGName(strValue);
				if (bNoSlot)   {
					((CAdamoAxis*)m_pCurrentObj)->SetGSlot(-1);
					CXTPPropertyGridItem *pGSlotItem = m_wndPropertyGrid.FindItem(ID_GSLOT);
					if (pGSlotItem)
						pGSlotItem->SetValue(LOADSTRING (IDS_ADAMOGRIDVIEW_100));
				}
			}
            break;
        case ID_GSLOT:
            if (strValue==LOADSTRING (IDS_ADAMOGRIDVIEW_100))
                n=_NO_GSLOT;
            else
                n=strtol (strValue, &p, 10);
            isValid = IsValidNameAxis (((CAdamoAxis*)m_pCurrentObj)->GetGName (), 1, n);
            if (!isValid)   {
                AfxMessageBox(LOADSTRING (IDS_ADAMOGRIDVIEW_91));
                n=((CAdamoAxis*)m_pCurrentObj)->GetGSlot();
                if (n==_NO_GSLOT)
                    pItem->SetValue(LOADSTRING (IDS_ADAMOGRIDVIEW_100));
                else   {
                    strValue.Format ("%d", n);
                    pItem->SetValue(strValue);
                }
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetGSlot(n);
            break;
        case ID_RESOLUTION:
            isValid = IsValidDouble (strValue, ZERO, MAXVALUERESOLUTION, &fp);
            if (isValid)
                isValid = fp>0.01 ;
            if (!isValid)   {
                MessageBeep((UINT)-1);
                strValue.Format ("%8.6f", ((CAdamoAxis*)m_pCurrentObj)->GetResolution());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetResolution(fp);
            break;
        case ID_AXISTYPE:
            if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_70))
                ((CAdamoAxis*)m_pCurrentObj)->SetAxisType(enAsseContinua);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_71))
                ((CAdamoAxis*)m_pCurrentObj)->SetAxisType(enAsseVirtuale);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_104))
                ((CAdamoAxis*)m_pCurrentObj)->SetAxisType(enAsseDigitale);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_112))
                ((CAdamoAxis*)m_pCurrentObj)->SetAxisType(enAsseCia402);
			switch (((CAdamoAxis*)m_pCurrentObj)->GetAxisType ())   {
				case enAsseDigitale :   {
					CXTPPropertyGridItem *pAxisTic = m_wndPropertyGrid.FindItem(ID_AXISTIC);
					if (pAxisTic)
						pAxisTic->SetReadOnly (false);
					break;
				}
				case enAsseContinua :	{
					CXTPPropertyGridItem *pAxisTic = m_wndPropertyGrid.FindItem(ID_AXISTIC);
					if (pAxisTic)   {
						pAxisTic->SetValue (LOADSTRING (IDS_ADAMOGRIDVIEW_111));
						pAxisTic->SetReadOnly ();
					}
					break;
				}
				case enAsseVirtuale :	{
					CXTPPropertyGridItem *pAxisTic = m_wndPropertyGrid.FindItem(ID_AXISTIC);
					if (pAxisTic)   {
						pAxisTic->SetValue (LOADSTRING (IDS_ADAMOGRIDVIEW_138));
						pAxisTic->SetReadOnly ();
					}
					break;
				}
			}
            break;
        case ID_AXISTIC:
            if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_110))
                ((CAdamoAxis*)m_pCurrentObj)->SetTIC(enTICPosizione);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_111))
                ((CAdamoAxis*)m_pCurrentObj)->SetTIC(enTICVelocita);
            break;
        case ID_AXESCATEGORY:
            if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_74))
                ((CAdamoAxis*)m_pCurrentObj)->SetAxesCategory(enAsseLineare);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_75))
                ((CAdamoAxis*)m_pCurrentObj)->SetAxesCategory(enAsseRotante);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_130))
                ((CAdamoAxis*)m_pCurrentObj)->SetAxesCategory(enAsseBrandeggio);
            break;
        case ID_UNIT:
            if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_77))
                ((CAdamoAxis*)m_pCurrentObj)->SetUnit(enUMMmin);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_78))
                ((CAdamoAxis*)m_pCurrentObj)->SetUnit(enUMinch);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_79))
                ((CAdamoAxis*)m_pCurrentObj)->SetUnit(enUMgiri);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_80))
                ((CAdamoAxis*)m_pCurrentObj)->SetUnit(enUMgradisec);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_180))
                ((CAdamoAxis*)m_pCurrentObj)->SetUnit(enUMMMmin);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_181))
                ((CAdamoAxis*)m_pCurrentObj)->SetUnit(enUMgradimin);
			CorreggiUnitaDiMisura ((enUnitaMisura)((CAdamoAxis*)m_pCurrentObj)->GetUnit ());
            break;
        case ID_INVPHASE:   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoAxis*)m_pCurrentObj)->SetInvPhase(bValue);
            break;
        }
        case ID_INVRIF:     {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoAxis*)m_pCurrentObj)->SetInvRif(bValue);
            break;
        }
        case ID_ENABLENICK: {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoAxis*)m_pCurrentObj)->SetEnableNick(bValue);
            break;
        }
        case ID_ENABLEJERK: {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoAxis*)m_pCurrentObj)->SetEnableJerk(bValue);
            break;
        }
        case ID_WAITONACCCHANGE :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoAxis*)m_pCurrentObj)->SetEnableJerk(bValue);
            break;
        }
        case ID_SPEED:
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetSpeed());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetSpeed(fp);
            break;
        case ID_SPEEDMAN:
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetSpeedMan());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetSpeedMan(fp);
            break;
        case ID_ACC:
            isValid = IsValidInteger (strValue, (int)ZERO, (int)MAXVALUE, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("d", (int)((CAdamoAxis*)m_pCurrentObj)->GetAcc());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetAcc(n);
            break;
        case ID_DECC :
            isValid = IsValidInteger (strValue, (int)ZERO, (int)MAXVALUE, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetDec());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetDec(n);
            break;
        case ID_PROP :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetProp());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetProp(fp);
            break;
        case ID_INT :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetInt());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInt(fp);
            break;
        case ID_DER :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetDer());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetDer(fp);
            break;
        case ID_FEEDFORWARD :
            isValid = IsValidDouble (strValue, ZERO, 100, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetFeedForward());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetFeedForward (fp);
            break;
        case ID_FEEDFORWARDACC :
            isValid = IsValidDouble (strValue, ZERO, 100, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetFeedForwardAcc());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetFeedForwardAcc (fp);
            break;
        case ID_RIF :
            isValid = IsValidInteger (strValue, 0, 10, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetRif());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetRif (n);
            break;
        case ID_RAMPTYPE :   {
            if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_81))
                ((CAdamoAxis*)m_pCurrentObj)->SetRampType(enLineare);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_82))
                ((CAdamoAxis*)m_pCurrentObj)->SetRampType(enEssePura);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_83))
                ((CAdamoAxis*)m_pCurrentObj)->SetRampType(enEsseParziale);
            int etr=((CAdamoAxis*)m_pCurrentObj)->GetRampType ();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_PERCRT);
            pItem->SetReadOnly(etr!=enEsseParziale);
            break;
        }
        case ID_INTERSPEED :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetInterSpeed());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterSpeed (fp);
            break;
        case ID_INTERACC :
            isValid = IsValidInteger (strValue, (int)ZERO, (int)MAXVALUE, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetInterAcc());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterAcc (n);
            break;
        case ID_INTERDEC :
            isValid = IsValidInteger (strValue, (int)ZERO, (int)MAXVALUE, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetInterDec());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterDec (n);
            break;
        case ID_INTERPROP :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetInterProp());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterProp (fp);
            break;
        case ID_INTERINT :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetInterInt());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterInt (fp);
            break;
        case ID_INTERDER :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetInterDer());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterDer (fp);
            break;
        case ID_INTERFEEDFORWARD :
            isValid = IsValidDouble (strValue, ZERO, 100, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetInterFeedForward());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterFeedForward (fp);
            break;
        case ID_INTERFEEDFORWARDACC :
            isValid = IsValidDouble (strValue, ZERO, 100, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetInterFeedForwardAcc());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterFeedForwardAcc (fp);
            break;
        case ID_INTERRAMPTYPE :   {
            if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_81))
                ((CAdamoAxis*)m_pCurrentObj)->SetInterRampType(enLineare);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_82))
                ((CAdamoAxis*)m_pCurrentObj)->SetInterRampType(enEssePura);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_83))
                ((CAdamoAxis*)m_pCurrentObj)->SetInterRampType(enEsseParziale);
            int etr=((CAdamoAxis*)m_pCurrentObj)->GetInterRampType ();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_INTERPERCRT);
            pItem->SetReadOnly(etr!=enEsseParziale);
            break;
        }
        case ID_WAITSTOPAXIS :
            {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoAxis*)m_pCurrentObj)->SetWaitStopAxis(bValue);
            break;
            }
        case ID_LIMEACONC :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetLimEAConc());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetLimEAConc(fp);
            break;
        case ID_LIMEADISC :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetLimEADisc());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetLimEADisc(fp);
            break;
        case ID_LIMEAMIN :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetLimEAMin());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetLimEAMin(fp);
            break;
        case ID_ENABLELIMAXESPOS :
            {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_LIMAXISPOS);
            pItem->SetReadOnly(!bValue);
            ((CAdamoAxis*)m_pCurrentObj)->SetEnableLimAxesPos(bValue);
            break;
            }
        case ID_ENABLELIMAXESNEG :
            {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_LIMAXISNEG);
            pItem->SetReadOnly(!bValue);
            ((CAdamoAxis*)m_pCurrentObj)->SetEnableLimAxesNeg(bValue);
            break;
            }
        case ID_LIMAXISPOS :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetLimAxisPos());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetLimAxisPos(fp);
            break;
        case ID_LIMAXISNEG :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetLimAxisNeg());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetLimAxisNeg(fp);
            break;
        case ID_WINPOS :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetWinPos());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetWinPos(fp);
            break;
        case ID_WINNEG :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetWinNeg());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetWinNeg(fp);
            break;
        case ID_SERVOLIMITTYPE :
            if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_84))
                ((CAdamoAxis*)m_pCurrentObj)->SetServoLimitType(enSTDisabled);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_85))
                ((CAdamoAxis*)m_pCurrentObj)->SetServoLimitType(enSTLineare);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_86))
                ((CAdamoAxis*)m_pCurrentObj)->SetServoLimitType(enSTDinamico);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_87))
                ((CAdamoAxis*)m_pCurrentObj)->SetServoLimitType(enSTProporzionale);
            break;
        case ID_CC :
            ((CAdamoAxis*)m_pCurrentObj)->SetCCName (strValue);
            break;
        case ID_PERCRT :
            isValid = IsValidDouble (strValue, ZERO, 100, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetPercRT());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetPercRT(fp);
            break;
        case ID_INTERPERCRT :
            isValid = IsValidDouble (strValue, ZERO, 100, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetInterPercRT());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterPercRT(fp);
            break;
        case ID_DIMBUGINTEG :
            isValid = IsValidInteger (strValue, (int)ZERO, 1024, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetInterPercRT());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetDimBufferIntegrale(n);
            break;
        case ID_GANTRYMASTER :
            n=GetGantryMasterName (strValue);
            ((CAdamoAxis*)m_pCurrentObj)->SetGantryAxes (n);
            break;
        case ID_GANTRYPROP :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetPropGantry());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetPropGantry(fp);
            break;
        case ID_GANTRYDER :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetDerGantry());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetDerGantry(fp);
            break;
        case ID_GANTRYINT :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetIntGantry());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetIntGantry(fp);
            break;
        case ID_DISPLAY :
            {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_DISPLAY);
            pItem->SetReadOnly(!bValue);
            ((CAdamoAxis*)m_pCurrentObj)->SetDisplay (bValue);
            break;
            }
        case ID_MAXPERCTORQUE :
            isValid = IsValidInteger (strValue, (int)ZERO, 500, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetPercTorque ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetPercTorque (n);
			break;
        case ID_ENABLE_COMPENSAZIONE_GIOCO :
            {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_COMPENSAZIONE_GIOCO);
            pItem->SetReadOnly(!bValue);
            pItem = m_wndPropertyGrid.FindItem(ID_TEMPO_COMPENSAZIONE);
            pItem->SetReadOnly(!bValue);
            ((CAdamoAxis*)m_pCurrentObj)->SetEnableRecuperoGioco(bValue);
            break;
            }
        case ID_COMPENSAZIONE_GIOCO :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetRecuperoGioco());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetRecuperoGioco(fp);
            break;
        case ID_VELOCITADICORREZIONE :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetVelocitaDiCorrezione ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetVelocitaDiCorrezione (fp);
            break;
		case ID_CORREZIONEDINAMICA :
			{
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoAxis*)m_pCurrentObj)->SetCorrezioneDinamica (bValue);
			break;
			}
        case ID_TEMPO_COMPENSAZIONE :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetTempoDiRecupero());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetTempoDiRecupero(fp);
            break;
        case ID_KLC :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetCostanteDiTempoAzionamento ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetCostanteDiTempoAzionamento(fp);
            break;
        case ID_RANGEQUOTANEG :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetRangeQuotaNeg ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetRangeQuotaNeg (fp);
            break;
        case ID_RANGEQUOTAPOS :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetRangeQuotaPos ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetRangeQuotaPos (fp);
            break;
        case ID_ROTATIONFILTER :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAxis*)m_pCurrentObj)->GetFiltroRotanti ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetFiltroRotanti(fp);
            break;
        case ID_TEMPOARRIVOINQUOTA :
            isValid = IsValidInteger (strValue, (int)0, 32, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetTempoArrivoInQuota ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetTempoArrivoInQuota (n);
        case ID_TEMPOASSEFERMO :
            isValid = IsValidInteger (strValue, (int)0, 32, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetTempoAsseFermo ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetTempoAsseFermo (n);
        case ID_ENABLE_COMPENSAZIONE_EA :
            {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_COMPENSAZIONE_EA);
            pItem->SetReadOnly(!bValue);
            ((CAdamoAxis*)m_pCurrentObj)->SetEnableCompensazioneEA (bValue);
            break;
            }
        case ID_COMPENSAZIONE_EA :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.6f", ((CAdamoAxis*)m_pCurrentObj)->GetCompensazioneEA ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetCompensazioneEA(fp);
            break;
        case ID_COMPENSAZIONE_EAA :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.6f", ((CAdamoAxis*)m_pCurrentObj)->GetCompensazioneEAA ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetCompensazioneEAA(fp);
            break;
		case ID_INTKT :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.6f", ((CAdamoAxis*)m_pCurrentObj)->GetIntKT ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetIntKT (fp);
			break;
		case ID_ENABLESWITCHEDINT :
            {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_SWITCHEDINTSPEED);
            pItem->SetReadOnly(!bValue);
            ((CAdamoAxis*)m_pCurrentObj)->SetEnableSwitchedInt (bValue);
            break;
            }
		case ID_SWITCHEDINTSPEED :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.6f", ((CAdamoAxis*)m_pCurrentObj)->GetSwitchedIntSpeed ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetSwitchedIntSpeed (fp);
			break;
		case ID_DERN :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.6f", ((CAdamoAxis*)m_pCurrentObj)->GetDerN ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetDerN (fp);
			break;
		case ID_INTERPINTKT	:
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.6f", ((CAdamoAxis*)m_pCurrentObj)->GetInterIntKT ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterIntKT (fp);
			break;
		case ID_INTERENABLESWITCHEDINT :
            {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_INTERSWITCHEDINTSPEED);
            pItem->SetReadOnly(!bValue);
            ((CAdamoAxis*)m_pCurrentObj)->SetInterEnableSwitchedInt (bValue);
            break;
            }
			break;
		case ID_INTERSWITCHEDINTSPEED :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.6f", ((CAdamoAxis*)m_pCurrentObj)->GetInterSwitchedIntSpeed ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterSwitchedIntSpeed (fp);
			break;
		case ID_INTERDERN :
            isValid = IsValidDouble (strValue, MINVALUE, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.6f", ((CAdamoAxis*)m_pCurrentObj)->GetInterDerN ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetInterDerN (fp);
			break;

        case ID_FILTROCOLLENCODER :
            isValid = IsValidInteger (strValue, (int)0, 32, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetFiltroCollegamentoEncoder ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetFiltroCollegamentoEncoder (n);
        case ID_FILTROSERVOERROR :
            isValid = IsValidInteger (strValue, (int)0, 32, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", (int)((CAdamoAxis*)m_pCurrentObj)->GetFiltroServoError ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAxis*)m_pCurrentObj)->SetFiltroServoError ((WORD) n);
    }
}

void CGridView::SwitchForAnalogIn(CXTPPropertyGridItem *pItem)
{
    CString strValue = pItem->GetValue();
    double fp;
    bool isValid;

    switch (pItem->GetID())  {
        case ID_LOGICRANGEMIN :
            isValid = IsValidDouble (strValue, -DBL_MAX, DBL_MAX, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAnalogIn*)m_pCurrentObj)->GetLLInf ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAnalogIn*)m_pCurrentObj)->SetLLInf (fp);
            break;
        case ID_LOGICRANGEMAX :
            isValid = IsValidDouble (strValue, -DBL_MAX, DBL_MAX, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAnalogIn*)m_pCurrentObj)->GetLLSup ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAnalogIn*)m_pCurrentObj)->SetLLSup (fp);
            break;
    }
}

void CGridView::SwitchForAnalogOut(CXTPPropertyGridItem *pItem)
{
    CString strValue = pItem->GetValue();
    double fp;
    bool isValid;

    switch (pItem->GetID())  {
        case ID_LOGICRANGEMIN :
            isValid = IsValidDouble (strValue, -DBL_MAX, DBL_MAX, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAnalogOut*)m_pCurrentObj)->GetLLInf ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAnalogOut*)m_pCurrentObj)->SetLLInf (fp);
            break;
        case ID_LOGICRANGEMAX :
            isValid = IsValidDouble (strValue, -DBL_MAX, DBL_MAX, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CAdamoAnalogOut*)m_pCurrentObj)->GetLLSup ());
                pItem->SetValue(strValue);
            }
            else
                ((CAdamoAnalogOut*)m_pCurrentObj)->SetLLSup (fp);
            break;
    }
}

void CGridView::SwitchForDgtIn(CXTPPropertyGridItem *pItem)
{
    int n;
    bool isValid;
    char szBuffer[256];

    CString strValue = pItem->GetValue();
    switch (pItem->GetID())   {
        case ID_OUTNUMBER :
            isValid = IsValidInteger (strValue, (int)ZERO, 16, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                sprintf (szBuffer, "%d", ((CAdamoDgtIn*)m_pCurrentObj)->GetNumInput());
                pItem->SetValue(szBuffer);
            }
            else
                ((CAdamoDgtIn*)m_pCurrentObj)->SetNumInput(n);
            break;
    }
}

void CGridView::SwitchForDgtOut(CXTPPropertyGridItem *pItem)
{
    int n; 
    bool isValid;
    char szBuffer[256];

    CString strValue = pItem->GetValue();
    switch (pItem->GetID())   {
        case ID_ISMONOSTABLE:   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_OUTTIMEOUT);
            pItem->SetReadOnly(!bValue);
            ((CAdamoDgtOut*)m_pCurrentObj)->SetIsMonoStable(bValue);
            break;
        }
        case ID_OUTNUMBER:
            isValid = IsValidInteger (strValue, (int)ZERO, 16, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                sprintf (szBuffer, "%d", ((CAdamoDgtOut*)m_pCurrentObj)->GetNumOutput());
                pItem->SetValue(szBuffer);
            }
            else
                ((CAdamoDgtOut*)m_pCurrentObj)->SetNumOutput (n);
            break;
        case ID_OUTTIMEOUT: {
            isValid = IsValidInteger (strValue, (int)ZERO, 10000, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                sprintf (szBuffer, "%d", ((CAdamoDgtOut*)m_pCurrentObj)->GetTimeOut ());
                pItem->SetValue(szBuffer);
            }
            else
                ((CAdamoDgtOut*)m_pCurrentObj)->SetTimeOut (n);
            break;
            }
    }
}

void CGridView::SwitchForTimer(CXTPPropertyGridItem *pItem)
{
    int n; 
    bool isValid;
    char szBuffer[256];

    CString strValue = pItem->GetValue();
    switch(pItem->GetID()){
        case ID_PERIOD :
            isValid = IsValidInteger (strValue, (int)ZERO, 100000, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                sprintf (szBuffer, "%d", ((CAdamoTimer*)m_pCurrentObj)->GetTime ());
                pItem->SetValue(szBuffer);
            }
            else
                ((CAdamoTimer*)m_pCurrentObj)->SetTime (n);
            break;
        case ID_UPDOWN :    {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoTimer*)m_pCurrentObj)->SetUpDown(bValue);
            break;
        }
        case ID_ENABLED :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoTimer*)m_pCurrentObj)->SetEnabled(bValue);
            break;
        }
        case ID_STORED :    {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CAdamoTimer*)m_pCurrentObj)->SetStored(bValue);
            break;
        }
    }
}

void CGridView::SwitchForCounter(CXTPPropertyGridItem *pItem)
{
    switch(pItem->GetID()){
    case ID_COUNTERTYPE:
        {
        CString strValue = pItem->GetValue();
        if(strValue == "Number")
            ((CAdamoCounter*)m_pCurrentObj)->SetCounterType(LUA_TNUMBER);
        else if(strValue == "Boolean")
            ((CAdamoCounter*)m_pCurrentObj)->SetCounterType(LUA_TBOOLEAN);
        else if(strValue == "String")
            ((CAdamoCounter*)m_pCurrentObj)->SetCounterType(LUA_TSTRING);
        break;
        }
    }
}

void CGridView::RefreshFrame()
{
   CView *pView = NULL;
   CMDIChildWnd *pChild = ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->MDIGetActive();
   if (pChild != NULL && m_pCurrentObj)  {
      pView = pChild->GetActiveView();
      if (pView != NULL)
          if(pView->IsKindOf(RUNTIME_CLASS(CConfTreeView)))
              ((CConfTreeView*)pView)->Update(m_pCurrentObj);
   }    
}

CString CGridView::Trim(CString strString)
{
    strString.TrimRight();
    strString.TrimLeft();

    return strString;
}

void CGridView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
    CMainFrame *pFrame = (CMainFrame *)(AfxGetApp()->GetMainWnd());
    if(pFrame)
        m_wndPropertyGrid.SetFont(pFrame->GetProgramFont());	
}

void CGridView::SetForModule (CProject *pPrj, CXTPPropertyGridItem *pRoot)
{
	CAdamoData ad;
    CXTPPropertyGridItem* pModuleParam = m_wndPropertyGrid.AddCategory(_T("Module Data"));
    AddGridComboModule (pPrj, pModuleParam);
    AddGridComboNumber (pPrj, pModuleParam);
    AddGridComboFrequency (pPrj, pModuleParam);
    CXTPPropertyGridItem* pItem=AddGridItemi(ID_RATE,LOADSTRING (IDS_ADAMOGRIDVIEW_103),pPrj->GetRate (),pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemi(ID_MAXJITTER,LOADSTRING (IDS_ADAMOGRIDVIEW_185),(int)pPrj->GetMaxJitter (), pModuleParam);
    GestionePsw (pItem, ePswService);

	pModuleParam->Expand ();

    pModuleParam = m_wndPropertyGrid.AddCategory(_T("Generico"));

    pItem=AddGridItemd(ID_CONTOURANGLE,LOADSTRING (IDS_ADAMOGRIDVIEW_188),pPrj->GetContoureAngle (),pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_MESSAGEFILTERINGONSTART,LOADSTRING (IDS_ADAMOGRIDVIEW_119),pPrj->GetMessageFilteringOnStart(),pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_STOPAXISONERROR,LOADSTRING (IDS_ADAMOGRIDVIEW_120),pPrj->GetStopAxisOnError(),pModuleParam);
    GestionePsw (pItem, ePswBuilder);

	pModuleParam->Expand ();

	pModuleParam = m_wndPropertyGrid.AddCategory(_T("Interpolazioni"));

    pItem=AddGridItemd(ID_PRECISION,LOADSTRING (IDS_ADAMOGRIDVIEW_116),pPrj->GetPrecision (),pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_RADIUSPRECISION,LOADSTRING (IDS_ADAMOGRIDVIEW_117),pPrj->GetEpsilonRaggi (),pModuleParam);
    GestionePsw (pItem, ePswBuilder);
	ad.nType = LUA_TNUMBER;
	ad.AdamoData.fp = pPrj->GetKAC ();
	FilterValue (ID_KAC, ad);
    pItem=AddGridItemd(ID_KAC, LOADSTRING (IDS_ADAMOGRIDVIEW_133), ad.AdamoData.fp, pModuleParam);
    GestionePsw (pItem, ePswBuilder);

	pModuleParam->Expand ();

	pModuleParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_152));
    pItem=AddGridItemCheckBox(ID_5ASSI_USE_AS_DEFAULT, LOADSTRING (IDS_ADAMOGRIDVIEW_157), pPrj->GetAInterpDefault (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_5ASSI_SPEED,LOADSTRING (IDS_ADAMOGRIDVIEW_153),pPrj->GetVelProfile (), pModuleParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemi(ID_5ASSI_ACC,LOADSTRING (IDS_ADAMOGRIDVIEW_154),(int)pPrj->GetAccProfile (), pModuleParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemi(ID_5ASSI_DEC,LOADSTRING (IDS_ADAMOGRIDVIEW_155),(int)pPrj->GetDecProfile (), pModuleParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_5ASSI_SMOOTHING, LOADSTRING (IDS_ADAMOGRIDVIEW_129), pPrj->GetSmoothing (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_5ASSI_BOW_THICKENING,LOADSTRING (IDS_ADAMOGRIDVIEW_156),pPrj->GetBowThickening (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_5ASSI_LINEAR_THICKENING,LOADSTRING (IDS_ADAMOGRIDVIEW_174),pPrj->GetLinearThickening (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_5ASSI_SPIKEDETECTOR, LOADSTRING (IDS_ADAMOGRIDVIEW_161), pPrj->GetSpikeDetector (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_5ASSI_MAXUTURNPRECISION, LOADSTRING (IDS_ADAMOGRIDVIEW_162), pPrj->GetMaxUTurnPrecision (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
	pItem=AddGridComboSmootherType (pPrj->GetSmootherType (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_5ASSI_SINGLESPIKESMOOTHERWEIGHT,LOADSTRING (IDS_ADAMOGRIDVIEW_169),pPrj->GetSingleSpikeSmootherWeight (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_5ASSI_SINGLESPIKEFACTOR,LOADSTRING (IDS_ADAMOGRIDVIEW_168),pPrj->GetSingleSpikeFactor (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_5ASSI_GEOMETRYSPIKEFACTOR,LOADSTRING (IDS_ADAMOGRIDVIEW_171),pPrj->GetGeometrySpikeFactor (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_5ASSI_DETECTJOIUNTUTURN, LOADSTRING (IDS_ADAMOGRIDVIEW_190), pPrj->GetDetectJointUTurn (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_5ASSI_STOPONANGLE, LOADSTRING (IDS_ADAMOGRIDVIEW_191), pPrj->GetStopOnAngle (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_5ASSI_MINSTOPANGLE, LOADSTRING (IDS_ADAMOGRIDVIEW_192), pPrj->GetMinStopAngle (), pModuleParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemd(ID_5ASSI_SIP, LOADSTRING (IDS_ADAMOGRIDVIEW_200), pPrj->GetSIP (), pModuleParam);
    GestionePsw (pItem, ePswService);
	pItem=AddGridItemButtonText (ID_KYNEMATIC_COMPENSATIONS, LOADSTRING (IDS_ADAMOGRIDVIEW_201), pPrj, pModuleParam);
    GestionePsw (pItem, ePswService);
    pModuleParam->Expand ();

	pModuleParam = m_wndPropertyGrid.AddCategory(_T("G Code"));

    pItem=AddGridItemCheckBox(ID_INVERTIG2G3, LOADSTRING (IDS_ADAMOGRIDVIEW_134), pPrj->GetSensoRotazioneCircolari (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_G0RAGGIATA_ENABLE, LOADSTRING (IDS_ADAMOGRIDVIEW_182),pPrj->GetG0Raggiata (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_G0RAGGIATA_INFITTIMENTO, LOADSTRING (IDS_ADAMOGRIDVIEW_183),pPrj->GetG0RaggiataInfittimento (), pModuleParam);
	pItem->SetReadOnly (!(pPrj->GetG0Raggiata () != 0));
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemd(ID_G0RAGGIATA_RAGGIO, LOADSTRING (IDS_ADAMOGRIDVIEW_184),pPrj->GetG0RaggiataRaggio (), pModuleParam);
	pItem->SetReadOnly (!(pPrj->GetG0Raggiata () != 0));
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemCheckBox(ID_CENTROINCREMENTALE, LOADSTRING (IDS_ADAMOGRIDVIEW_189), pPrj->GetCentroIncrementale (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pModuleParam->Expand ();

	pModuleParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_175));
    pItem=AddGridItemCheckBox(ID_RAM_DISK, LOADSTRING (IDS_ADAMOGRIDVIEW_176),pPrj->GetRamDisk (),pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemi(ID_RAM_DISK_BUFFER_SIZE,LOADSTRING (IDS_ADAMOGRIDVIEW_177),pPrj->GetRamDiskBufferSize (),pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pModuleParam->Expand ();

	pModuleParam = m_wndPropertyGrid.AddCategory(LOADSTRING (IDS_ADAMOGRIDVIEW_186));
    pItem=AddGridItemCheckBox(ID_SIMULATIONMODE, LOADSTRING (IDS_ADAMOGRIDVIEW_187),pPrj->GetSimulationMode (),pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pModuleParam->Expand ();

    pModuleParam = m_wndPropertyGrid.AddCategory(_T("SSCNET"));
    pItem=AddGridItemi(ID_BOOSTPLC,LOADSTRING (IDS_ADAMOGRIDVIEW_199),(int)pPrj->GetPlcBoost (), pModuleParam);
    GestionePsw (pItem, ePswService);
    pItem=AddGridItemCheckBox(ID_STRICTABSPOSCONTROL, LOADSTRING (IDS_ADAMOGRIDVIEW_202), pPrj->GetStrictAbsPosControl (), pModuleParam);
    GestionePsw (pItem, ePswBuilder);
    pItem=AddGridItemi(ID_STRICTABSPOSENCSTEPS,LOADSTRING (IDS_ADAMOGRIDVIEW_203),(int)pPrj->GetStrictAbsPosEncoderSteps (), pModuleParam);
    GestionePsw (pItem, ePswService);

    pModuleParam->Expand ();
}

void CGridView::AddGridComboModule (CProject *pPrj, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

    switch (pPrj->GetMachineType())   {
        case enVirtual :
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_92);
            break;
        case enPhysical:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_93);
            break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_MACHINE_TYPE,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_94));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_92));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_93));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}

void CGridView::AddGridComboNumber (CProject *pPrj, CXTPPropertyGridItem *pItemNode)
{
    int i;
    char szBuffer[256];

    sprintf (szBuffer, "%d", pPrj->GetModuleNumber());
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_MACHINE_NUMBER,CString (szBuffer)));
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_95));
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    for (i=0; i<NMAXMACHINES; i++)
        if (!GETWS()->ExistModule (i) && pPrj->GetModuleNumber()!=i)   {
            sprintf (szBuffer, "%d", i);
	        pList->AddConstraint(szBuffer);
        }
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}

void CGridView::SwitchForModule (CXTPPropertyGridItem *pItem)
{
    double fp;
    bool bIsChanged=false;
    CString strValue = pItem->GetValue();
	int n;
    BOOL isValid;

    switch (pItem->GetID())   {
        case ID_MACHINE_TYPE :   {
            if (strValue==LOADSTRING (IDS_ADAMOGRIDVIEW_92))   {
                if (((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetMachineType()!=enVirtual)   {
                    ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetMachineType(enVirtual);
                    ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetMachineModifiedFlag (true);
                    bIsChanged=true;
                }
            }
            else   {
                if (((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetMachineType()!=enPhysical)   {
                    ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetMachineType(enPhysical);
                    ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetMachineModifiedFlag (true);
                    bIsChanged=true;
                }
            }
            if (bIsChanged)   {
                CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_MACHINE_FREQUENCY);
                if (pItem)
                    pItem->SetReadOnly (((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetMachineType()==enVirtual);
                SwitchMachine ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject());
            }
            break;
        }
        case ID_MACHINE_FREQUENCY :   {
            int nValue=atol (strValue);
            if (((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetFrequency()!=nValue)   {
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetFrequency(nValue);
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetMachineModifiedFlag (true);
            }
            break;
        }
        case ID_MACHINE_NUMBER :   {
            int nValue=atol (strValue);
            if (((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetModuleNumber()!=nValue)   {
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetModuleNumber(nValue);
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetMachineModifiedFlag (true);
            }
            break;
        }
        case ID_RATE :    {
            int nValue=atol (strValue);
            if (((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetRate()!=nValue)   {
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetRate(nValue);
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetMachineModifiedFlag (true);
            }
            break;
        }
        case ID_PRECISION :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetPrecision());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetPrecision (fp);
            break;
        case ID_RADIUSPRECISION :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetEpsilonRaggi());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetEpsilonRaggi (fp);
            break;
        case ID_MESSAGEFILTERINGONSTART :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetMessageFilteringOnStart(bValue);
            break;
        }
        case ID_STOPAXISONERROR :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetStopAxisOnError(bValue);
            break;
        }
		case ID_5ASSI_SMOOTHING :   {
            isValid = IsValidDouble (strValue, 0.0, 100.0, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetSmoothing());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetSmoothing (fp);
			break;
    }
		case ID_INVERTIG2G3 :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetSensoRotazioneCircolari (bValue);
			break;
		}
		case ID_KAC :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetKAC ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetKAC (fp);
			break;
		case ID_5ASSI_USE_AS_DEFAULT :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetAInterpDefault (bValue);
			break;
		}
        case ID_5ASSI_SPEED :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetVelProfile ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetVelProfile (fp);;
            break;
        case ID_5ASSI_ACC :
            isValid = IsValidInteger (strValue, (int)ZERO, (int)MAXVALUE, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetAccProfile ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetAccProfile (n);
            break;
        case ID_5ASSI_DEC :
            isValid = IsValidInteger (strValue, (int)ZERO, (int)MAXVALUE, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetDecProfile ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetDecProfile (n);
            break;
        case ID_5ASSI_BOW_THICKENING :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetBowThickening ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetBowThickening (fp);;
            break;
        case ID_5ASSI_LINEAR_THICKENING :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetLinearThickening ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetLinearThickening (fp);;
            break;
        case ID_5ASSI_SINGLESPIKEFACTOR :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetSingleSpikeFactor ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetSingleSpikeFactor (fp);;
            break;
        case ID_5ASSI_GEOMETRYSPIKEFACTOR :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetGeometrySpikeFactor ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetGeometrySpikeFactor (fp);
            break;
		case ID_5ASSI_SPIKEDETECTOR :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetSpikeDetector (bValue != 0);
			break;
		}
		case ID_5ASSI_MAXUTURNPRECISION : {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetMaxUTurnPrecision (bValue != 0);
			break;
		}
		case ID_5ASSI_SMOOTHERTYPE :
            if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_166))
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetSmootherType (enSmootherParabolic);
            else if(strValue == LOADSTRING (IDS_ADAMOGRIDVIEW_167))
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetSmootherType (enSmootherSinosuidal);
			break;
		case ID_5ASSI_SINGLESPIKESMOOTHERWEIGHT :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetSingleSpikeSmootherWeight ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetSingleSpikeSmootherWeight (fp);
            break;
		case ID_RAM_DISK :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetRamDisk (bValue != 0);
			break;
		}
		case ID_RAM_DISK_BUFFER_SIZE :
            isValid = IsValidInteger (strValue, (int)ZERO, (int)10240, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetRamDiskBufferSize ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetRamDiskBufferSize (n);
			break;
		case ID_G0RAGGIATA_ENABLE :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetG0Raggiata (bValue != 0);
			CXTPPropertyGridItem *pG0RInfittimento = m_wndPropertyGrid.FindItem(ID_G0RAGGIATA_INFITTIMENTO);
			if (pG0RInfittimento)
				pG0RInfittimento->SetReadOnly (!(bValue != 0));
			CXTPPropertyGridItem *pG0RRaggio = m_wndPropertyGrid.FindItem(ID_G0RAGGIATA_RAGGIO);
			if (pG0RRaggio)
				pG0RRaggio->SetReadOnly (!(bValue != 0));
			break;
		}
		case ID_G0RAGGIATA_INFITTIMENTO :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetG0RaggiataInfittimento ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetG0RaggiataInfittimento (fp);
			break;
		case ID_G0RAGGIATA_RAGGIO :
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetG0RaggiataRaggio ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetG0RaggiataRaggio (fp);
			break;
		case ID_SIMULATIONMODE :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetSimulationMode (bValue != 0);
			break;
		}
		case ID_MAXJITTER :   {
            isValid = IsValidInteger (strValue, (int)ZERO, (int)500, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetMaxJitter ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetMaxJitter (n);
			break;
		}
        case ID_CONTOURANGLE :
            isValid = IsValidDouble (strValue, ZERO, 180.0, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetContoureAngle ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetContureAngle (fp);
            break;
		case ID_CENTROINCREMENTALE :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetCentroIncrementale (bValue != 0);
			break;
		}
		case ID_5ASSI_DETECTJOIUNTUTURN :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetDetectJointUTurn (bValue != 0);
			break;
		}
		case ID_5ASSI_STOPONANGLE :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetStopOnAngle (bValue != 0);
			break;
		}
		case ID_5ASSI_MINSTOPANGLE :   {
            isValid = IsValidDouble (strValue, ZERO, 180.0, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetMinStopAngle ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetMinStopAngle (fp);
            break;
		}
		case ID_5ASSI_SIP :   {
            isValid = IsValidDouble (strValue, ZERO, MAXVALUE, &fp);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%8.2f", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetSIP ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetSIP (fp);
            break;
		}
		case ID_BOOSTPLC :   {
            isValid = IsValidInteger (strValue, (int)ZERO, (int)500, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetPlcBoost ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetPlcBoost (n);
			break;
		}
		case ID_STRICTABSPOSCONTROL :   {
            BOOL bValue = ((CXTPPropertyGridItemBool*)pItem)->GetBool();
            ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject ())->SetStrictAbsPosControl (bValue != 0);
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_STRICTABSPOSENCSTEPS);
            pItem->SetReadOnly(!bValue);
			break;
		}
		case ID_STRICTABSPOSENCSTEPS :   {
			int n;
            isValid = IsValidInteger (strValue, (int)ZERO, (int)0xffff, &n);
            if (!isValid)   {  
                MessageBeep((UINT)-1);
                strValue.Format ("%d", ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->GetStrictAbsPosEncoderSteps ());
                pItem->SetValue(strValue);
            }
            else
                ((CProject*)((CAdamoLogicModule*)m_pCurrentObj)->GetProject())->SetStrictAbsPosEncoderSteps (n);
			break;
		}
    }
}

void CGridView::AddGridComboFrequency (CProject *pPrj, CXTPPropertyGridItem *pItemNode)
{
    CString strValue;

    switch (pPrj->GetFrequency ())   {
        case FREQ2000 :
            strValue = "2000";
            break;
        case FREQ1136 :
            strValue = "1126";
            break;
        case FREQ1000 :
            strValue = "1000";
            break;
        case FREQ500 :
            strValue = "500";
            break;
        case FREQ100 :
            strValue = "100";
            break;
        case FREQ50 :
            strValue = "50";
            break;
    }
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_MACHINE_FREQUENCY,strValue));
    if (pPrj->GetMachineType()==enVirtual)
        pItemCombo->SetReadOnly (true);
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_96));
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
	pList->AddConstraint("50");
	pList->AddConstraint("100");
	pList->AddConstraint("500");
	pList->AddConstraint("1000");
	pList->AddConstraint("1126");
	pList->AddConstraint("2000");
	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}

void CGridView::SwitchMachine (CProject* pPrj)
{
    if (pPrj->GetMachine ()->IsCncOnline ())
        if (AfxMessageBox (LOADSTRING (IDS_ADAMOGRIDVIEW_97), MB_YESNO | MB_ICONQUESTION)==IDYES)
            ((CAdamoVMachine*)pPrj->GetMachine ())->ReplaceMachine ();            
}

CProject* CGridView::GetRootPrj ()
{
    CProject* pPrj=NULL;
    CObject *pObj=NULL;
    CObject *pParent=((CAdamoBase*)m_pCurrentObj)->GetParentObj();

    if (!pParent)
        pObj = m_pCurrentObj;
    while(pParent)   {
        pObj = pParent;
        pParent=((CAdamoBase*)pParent)->GetParentObj();
    }
    if (pObj)
        pPrj = (CProject*)((CAdamoLogicModule*)pObj)->GetProject();
    return pPrj;
}    

eTypeMachine CGridView::GetMachineType()
{
    CProject* pPrj=NULL;
    eTypeMachine el=enVirtual;

    pPrj=GetRootPrj ();
    if (pPrj)
        el=pPrj->GetMachineType ();
    return el;
}

bool CGridView::Iterator(CAdamoGroup *pGrp, CString strValue, int nType, int nGSlot)
{
    bool b=true;

    pGrp->Init();
    pGrp->DevInit();
    while(pGrp->DevMore())   {
        CAdamoBase *pDev = (CAdamoBase *)pGrp->DevGetNext();
        if (pDev->GetType ()== enAsse)
            if (nType==0)
                if (pDev->GetName()==strValue)
                    return false;
                else;
            else
                if (nType==1)
                    if (((CAdamoAxis*)pDev)->GetGName()==strValue&&((CAdamoAxis*)pDev)->GetGSlot ()==nGSlot)
                        return false;
                    else;
                else
                    if (nType==2)
                        if (((CAdamoAxis*)pDev)->GetAlias()==strValue)
                            return false;
                        else;
    }
    while (pGrp->More())   {
        CObject *obj = (CObject*)pGrp->GetNext();
        if (!(b=Iterator((CAdamoGroup*)obj, strValue, nType, nGSlot)))
            break;
    }
    return b;
}

bool CGridView::IsValidNameAxis (CString strValue, int nType, int nGSlot)
{
    bool b;
    BeginWaitCursor ();
    CProject* pPrj=GetRootPrj ();
    if (pPrj)
        b=Iterator ((CAdamoGroup*)pPrj->GetMachineModule(), strValue, nType, nGSlot);
    EndWaitCursor ();
    return b;
}

bool CGridView::IsValidDouble (CString strValue, double fpMinRange, double fpMaxRange, double *fp)
{
    char *c;
    bool b=false;
    strValue.TrimLeft (); strValue.TrimRight ();
    *fp=strtod (strValue, &c);
    if (*c=='\0')   {
        if (*fp>=fpMinRange&&*fp<=fpMaxRange)
            b=true;
    }
    return b;
}

bool CGridView::IsValidInteger (CString strValue, int fpMinRange, int fpMaxRange, int *n)
{
    char *c;
    bool b=false;
    strValue.TrimLeft (); strValue.TrimRight ();
    *n=strtol (strValue, &c, 10);
    if (*c=='\0')   {
        if (*n>=fpMinRange&&*n<=fpMaxRange)
            b=true;
    }
    return b;
}

void CGridView::CheckChars (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength)
{
    ((CMainFrame *) AfxGetMainWnd ())->CheckChars (pEdit, objCheck, nMaxLength);
}

CString CGridView::GetGantryMasterName (int nAxis)   
{
    CPtrList AxesList;
    CString strGantryAxis (NESSUN_ASSE_MASTER);

    ((CMainFrame *) AfxGetMainWnd ())->CreateAxesList (AxesList);
    if (AxesList.GetCount ())   {
        for (POSITION pos=AxesList.GetHeadPosition (); pos; )   {
            CAdamoAxis *pAxis=(CAdamoAxis *)AxesList.GetNext (pos);
            if (pAxis->GetDeviceID ()==nAxis)    {
                strGantryAxis=pAxis->GetName (); break;
            }
        }
    }
    return strGantryAxis;
}

int CGridView::GetGantryMasterName (CString str)
{
    CPtrList AxesList;
    int nId=_NO_GANTRY;

    ((CMainFrame *) AfxGetMainWnd ())->CreateAxesList (AxesList);
    if (AxesList.GetCount ())   {
        for (POSITION pos=AxesList.GetHeadPosition (); pos; )   {
            CAdamoAxis *pAxis=(CAdamoAxis *)AxesList.GetNext (pos);
            if (pAxis->GetName ()==str)   {
                nId=pAxis->GetDeviceID (); break;
            }
        }
    }
    return nId;
}

void CGridView::AddGridComboItemAxisMaster (CAdamoAxis *pAxis, CString strMasterName, CXTPPropertyGridItem *pItemNode)
{
    CPtrList AxesList;
    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem (new CXTPPropertyGridItem (ID_GANTRYMASTER,strMasterName));
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_98));
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
	pList->AddConstraint(_T(NESSUN_ASSE_MASTER));
    ((CMainFrame *) AfxGetMainWnd ())->CreateAxesList (AxesList);
    if (AxesList.GetCount ())   {
        for (POSITION pos=AxesList.GetHeadPosition (); pos; )   {
            CAdamoAxis *pOtherAxis=(CAdamoAxis *)AxesList.GetNext (pos);
            if (pAxis->GetName ()!=pOtherAxis->GetName ())
	            pList->AddConstraint(pOtherAxis->GetName ());
        }
    }
    pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}
    
void CGridView::RinominaFileDeiCorrettori (CAdamoAxis *pAxis, CString strNewName)
{
    CString str, strPath, strNewPath;
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        if (pAxis->GetCCName ()!="")   {
            pAxis->SetCCName (strNewName);
            str=pPrj->GetCCPathName ();
            strPath.Format ("%s\\%s", str, pAxis->GetName ());
            strNewPath.Format ("%s\\%s", str, strNewName);
            if (!_access (strPath, 00))
                CFile::Rename (strPath, strNewPath);
            CXTPPropertyGridItem *pItem = m_wndPropertyGrid.FindItem(ID_CC);
            if (pItem)
                pItem->SetValue (strNewName);
        }
    }
}

void CGridView::AddGridComboPswLevel (int iValue,CXTPPropertyGridItem *pItemNode, int nRW)
{
    CString strValue("");
    ePswLevel eLevel=(ePswLevel)iValue;

    switch (eLevel)   {
        case ePswKinema:
            strValue = LOADSTRING (IDS_PSWLEVEL_KINEMA);
            break;
        case ePswBuilder:
            strValue = LOADSTRING (IDS_PSWLEVEL_COSTRUTTORE);
            break;
        case ePswService:
            strValue = LOADSTRING (IDS_PSWLEVEL_MANUTENTORE);
            break;
        case ePswUser:
            strValue = LOADSTRING (IDS_PSWLEVEL_UTENTE);
            break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(nRW,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (nRW==ID_PSWLEVELR ? IDS_PSWLEVEL_CAPTION_READACCESS : IDS_PSWLEVEL_CAPTION_WRITEACCESS));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_PSWLEVEL_UTENTE));
	pList->AddConstraint(LOADSTRING (IDS_PSWLEVEL_MANUTENTORE));
	pList->AddConstraint(LOADSTRING (IDS_PSWLEVEL_COSTRUTTORE));
	pList->AddConstraint(LOADSTRING (IDS_PSWLEVEL_KINEMA));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    GestionePsw (pItemCombo, ePswBuilder);
}

/*
** OnChangePsw :
*/
void CGridView::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    CObject *pObj=m_pCurrentObj;
    m_ePswLevel=newPsw;
    if (m_pCurrentObj->IsKindOf (RUNTIME_CLASS(CAdamoLogicModule)))
        pObj=(CObject *)GetRootPrj ();
    SetObject (pObj);
}

/*
** GestionePsw :
*/
void CGridView::GestionePsw (CXTPPropertyGridItem* pItem, ePswLevel e)
{
    if (m_ePswLevel>e)
        pItem->SetReadOnly ();
}

/*
** AggiungiUnitaDiMisura :
**							0 spazio
**							1 velocita
**							2 impulsi
*/ 
CString CGridView::AggiungiUnitaDiMisura (CString str, enum enUnitaMisura um, int nTipo)
{
	CString strCorretta;

	switch (um)   {
		case enUMMmin :
			switch (nTipo)   {
				case 0 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_139) + "]";
					break;
				case 1 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_140) + "]";
					break;
				case 2 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_141) + "]";
					break;
			}
			break;
		case enUMMMmin :
			switch (nTipo)   {
				case 0 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_139) + "]";
					break;
				case 1 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_180) + "]";
					break;
				case 2 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_141) + "]";
					break;
			}
			break;
		case enUMinch :
			switch (nTipo)   {
				case 0 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_142) + "]";
					break;
				case 1 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_143) + "]";
					break;
				case 2 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_144) + "]";
					break;
			}
			break;
		case enUMgradisec :
			switch (nTipo)   {
				case 0 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_145) + "]";
					break;
				case 1 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_146) + "]";
					break;
				case 2 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_147) + "]";
					break;
			}
			break;
		case enUMgradimin :
			switch (nTipo)   {
				case 0 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_145) + "]";
					break;
				case 1 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_179) + "]";
					break;
				case 2 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_147) + "]";
					break;
			}
			break;
		case enUMgiri :
			switch (nTipo)   {
				case 0 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_148) + "]";
					break;
				case 1 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_149) + "]";
					break;
				case 2 :
					strCorretta = str + " [" + LOADSTRING (IDS_ADAMOGRIDVIEW_150) + "]";
					break;
			}
			break;
	}
	return strCorretta;
}

/*
** CorreggiUnitaDiMisura :
**							0 spazio
**							1 velocita
**							2 impulsi
*/ 
CString CGridView::CorreggiUnitaDiMisura (enum enUnitaMisura um)
{
	CXTPPropertyGridItem *pItem;
	CString str;

	/* risoluzione */
	pItem = m_wndPropertyGrid.FindItem(ID_RESOLUTION);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_15), um, 2);
		pItem->SetCaption (str);
	}
	/* velocita */
	pItem = m_wndPropertyGrid.FindItem(ID_SPEED);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_25), um, 1);
		pItem->SetCaption (str);
	}
	/* velocita manuale */
	pItem = m_wndPropertyGrid.FindItem(ID_SPEEDMAN);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_26), um, 1);
		pItem->SetCaption (str);
	}
	/* velocita interpolazione */
	pItem = m_wndPropertyGrid.FindItem(ID_INTERSPEED);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_36), um, 1);
		pItem->SetCaption (str);
	}
	/* errore di inseguimento concordante */
	pItem = m_wndPropertyGrid.FindItem(ID_LIMEACONC);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_46), um, 0);
		pItem->SetCaption (str);
	}
	/* errore di inseguimento discordante */
	pItem = m_wndPropertyGrid.FindItem(ID_LIMEADISC);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_47), um, 0);
		pItem->SetCaption (str);
	}
	/* errore di inseguimento minimo */
	pItem = m_wndPropertyGrid.FindItem(ID_LIMEAMIN);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_137), um, 0);
		pItem->SetCaption (str);
	}
	/* limite positivo */
	pItem = m_wndPropertyGrid.FindItem(ID_LIMAXISPOS);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_49), um, 0);
		pItem->SetCaption (str);
	}
	/* limite negativo */
	pItem = m_wndPropertyGrid.FindItem(ID_LIMAXISNEG);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_51), um, 0);
		pItem->SetCaption (str);
	}
	/* finestra positiva */
	pItem = m_wndPropertyGrid.FindItem(ID_WINPOS);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_52), um, 0);
		pItem->SetCaption (str);
	}
	/* finetsra negativa */
	pItem = m_wndPropertyGrid.FindItem(ID_WINNEG);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_53), um, 0);
		pItem->SetCaption (str);
	}
	/* finestra quota positiva */
	pItem = m_wndPropertyGrid.FindItem(ID_RANGEQUOTAPOS);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_124), um, 0);
		pItem->SetCaption (str);
	}
	/* finestra quota negativa */
	pItem = m_wndPropertyGrid.FindItem(ID_RANGEQUOTANEG);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_125), um, 0);
		pItem->SetCaption (str);
	}
	/* recupero gioco */
	pItem = m_wndPropertyGrid.FindItem(ID_COMPENSAZIONE_GIOCO);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_122), um, 0);
		pItem->SetCaption (str);
	}
	/* velocita di recupero gioco */
	pItem = m_wndPropertyGrid.FindItem(ID_VELOCITADICORREZIONE);
	if (pItem)   {
		str = AggiungiUnitaDiMisura (LOADSTRING (IDS_ADAMOGRIDVIEW_135), um, 0);
		pItem->SetCaption (str);
	}
	return str;
}

/*
** FilterValue :
*/
void CGridView::FilterValue (int nID, CAdamoData& ad)
{
	switch (nID)   {
		case ID_KAC :
			if (ISEQUAL (ad.AdamoData.fp, 0.0))
				ad.AdamoData.fp = 0.5;
			break;
	}
}

/*
** AddGridComboSmootherType:
*/
CXTPPropertyGridItem *CGridView::AddGridComboSmootherType (int iValue,CXTPPropertyGridItem *pItemNode)
{
    CString strValue("");

    switch (iValue)   {
        case enSmootherParabolic:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_166);
            break;
        case enSmootherSinosuidal:
            strValue = LOADSTRING (IDS_ADAMOGRIDVIEW_167);
            break;
    }

    CXTPPropertyGridItem* pItemCombo = pItemNode->AddChildItem(new CXTPPropertyGridItem(ID_5ASSI_SMOOTHERTYPE,strValue));
    
    pItemCombo->SetCaption(LOADSTRING (IDS_ADAMOGRIDVIEW_163));

    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();

	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_166));
	pList->AddConstraint(LOADSTRING (IDS_ADAMOGRIDVIEW_167));

	pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
	return pItemCombo;
}

/*
** RefreshMetrics :
*/
void CGridView::RefreshMetrics(AppTheme nTheme)
{
	CPaneView::RefreshMetrics(nTheme);

	switch (nTheme)
	{
		case themeVS6: m_wndPropertyGrid.SetTheme(xtpGridThemeDefault); break;
		case themeVS2003: m_wndPropertyGrid.SetTheme(xtpGridThemeOfficeXP); break;
		case themeVS2005: m_wndPropertyGrid.SetTheme(xtpGridThemeVisualStudio2005); break;
		case themeVS2008: m_wndPropertyGrid.SetTheme(xtpGridThemeVisualStudio2005); break;
		case themeVS2010: m_wndPropertyGrid.SetTheme(xtpGridThemeVisualStudio2010); break;
		case themeVS2012Light: m_wndPropertyGrid.SetTheme(xtpGridThemeVisualStudio2012Light); break;
		case themeVS2012Dark: m_wndPropertyGrid.SetTheme(xtpGridThemeVisualStudio2012Dark); break;
		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue: m_wndPropertyGrid.SetTheme(xtpGridThemeVisualStudio2015); break;
	}

	CXTPClientRect rClient(this);
	MoveChildren(&m_wndToolBar, &m_wndPropertyGrid, rClient.Width(), rClient.Height());
}
