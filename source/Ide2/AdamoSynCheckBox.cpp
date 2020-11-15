// CAdamoSynCheckBox.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynPage.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynCheckBox.h"

#pragma comment(lib, "UxTheme.lib")

// CAdamoSynCheckBox

IMPLEMENT_DYNAMIC(CAdamoSynCheckBox, CWnd)

CAdamoSynCheckBox::CAdamoSynCheckBox() : m_bIsChecked (false), m_bIsAutoUpdate (true), m_pFont (NULL), m_pParent (NULL)
{
}

CAdamoSynCheckBox::~CAdamoSynCheckBox()
{
}

BEGIN_MESSAGE_MAP(CAdamoSynCheckBox, CWnd)
ON_WM_CTLCOLOR_REFLECT()
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CAdamoSynCheckBox message handlers
/*
** Create :
*/
BOOL CAdamoSynCheckBox::Create ()
{
	BOOL b = CButton::Create (GetValue (), WS_CHILD | WS_VISIBLE | BS_CHECKBOX, GetObjStyle ().GetRect (), m_pParent, GetID ());
	if (b)
		CreateFont ();
	return b;
}

/*
** Destroy :
*/
BOOL CAdamoSynCheckBox::Destroy ()
{
	return FALSE;
}

/*
** OnSave :
*/
int CAdamoSynCheckBox::OnSave ()
{
	CADORecordset pRSGrid;
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CResourceFile *pRF = pSynContainer->GetResourceFile ();
	CADORecordset *pRS = pRF->GetRSCtrlSyn ();
    COleSafeArray sa;
    CString strField;
    _variant_t v;
	int nCheck = 0;
    int nB=-1;
    long i;

	CString strResourceName = ((CAdamoSynPage *) m_pParent)->GetName ();
    v=strResourceName.AllocSysString ();
    i=0;
    sa.CreateOneDim (VT_VARIANT, 2);
    sa.PutElement (&i, &v);
	i++;
	v=GetID ();
    sa.PutElement (&i, &v);
	nCheck = GetCheck ();
	pRS->m_pRecordset->PutIndex (CString (RESINDEX_RESCONTROLLISINOTTICO).AllocSysString ());
    pRS->m_pRecordset->Seek (sa, adSeekFirstEQ);
    if (!pRS->IsEof())   {
		/* record trovato, scriviamo il valore */
        pRS->GetFieldValue ("NomePagina", strField);
		if (strResourceName == strField)   {
            pRS->Edit ();
			pRS->SetFieldValue ("ValoreDouble", (double)nCheck);
			pRS->Update ();
		}
    }
	else   {
		/* record non trovato, aggiungiamolo */
		pRS->m_pRecordset->Requery (-1);
		pRS->SetFieldValue ("ID", -1);
		pRS->SetFieldValue ("NomePagina", strResourceName);
		pRS->SetFieldValue ("IDControllo", GetID ());
		pRS->SetFieldValue ("TipoControllo", KindOf ());
		pRS->SetFieldValue ("NomeVariabile", GetObjVar().GetObjVar().m_strName);
		pRS->SetFieldValue ("Riga", GetObjVar().GetObjVar().m_nRow);
		pRS->SetFieldValue ("Colonna", GetObjVar().GetObjVar().m_strCol);
		pRS->SetFieldValue ("ValoreStringa", "");
		pRS->SetFieldValue ("ValoreDouble", (double) nCheck);
		pRS->SetFieldValue ("Tipo", (int)LUA_TBOOLEAN);
		pRS->AddNew ();
		pRS->Update ();
	}
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynCheckBox::OnLoad ()
{
	CADORecordset pRSGrid;
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CResourceFile *pRF = pSynContainer->GetResourceFile ();
	CADORecordset *pRS = pRF->GetRSCtrlSyn ();
    COleSafeArray sa;
    CString strField;
    _variant_t v;
	double fp = 0.0;
    int nB=-1;
    long i;

	CString strResourceName = ((CAdamoSynPage *) m_pParent)->GetName ();
    v=strResourceName.AllocSysString ();
    i=0;
    sa.CreateOneDim (VT_VARIANT, 2);
    sa.PutElement (&i, &v);
	i++;
	v=GetID ();
    sa.PutElement (&i, &v);
	pRS->m_pRecordset->PutIndex (CString (RESINDEX_RESCONTROLLISINOTTICO).AllocSysString ());
    pRS->m_pRecordset->Seek (sa, adSeekFirstEQ);
    if (!pRS->IsEof())   {
        pRS->GetFieldValue ("NomePagina", strField);
		if (strResourceName == strField)
			pRS->GetFieldValue ("ValoreDouble", fp);
    }
	else
		pRS->m_pRecordset->Requery (-1);
	SetCheck ((int) fp);	
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynCheckBox::OnDownload ()
{
	stRSVariabili& var = GetObjVar ().GetObjVar ();
	int nCheck = GetCheck ();
	if (var.m_strName != "")
		GETSYNCONT ()->OnUpdateDownload (var.m_strName, var.m_nRow, var.m_strCol, var.m_nType, "", (double)nCheck);
	return 0;
}

/*
** CreateFont :
*/
void CAdamoSynCheckBox::CreateFont ()
{
	m_pFont = GETSYNCONT ()->CreateFont (m_objStyle.GetRSFont().strFaceName, m_objStyle.GetRSFont().nSize, m_objStyle.GetRSFont().bBold);
	if (m_pFont)
		SetFont (m_pFont);
}

/*
** CtlColor :
*/
HBRUSH CAdamoSynCheckBox::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

/*
** OnEraseBkgnd :
*/
BOOL CAdamoSynCheckBox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

/*
** PreSubclassWindow :
*/
void CAdamoSynCheckBox::PreSubclassWindow() 
{
	SetWindowTheme (*this, L"", L"");
}

