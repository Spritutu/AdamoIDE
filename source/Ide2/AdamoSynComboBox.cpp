#include "stdafx.h"
#include "MainFrame.h"
#include "AdamoSynContainer.h"
#include "AdamoSynPage.h"
#include "AdamoSynBaseCtrl.h"

IMPLEMENT_DYNAMIC(CAdamoSynComboBox, CComboBox)

CAdamoSynComboBox::CAdamoSynComboBox() : m_pParent (NULL), m_pFont (NULL), m_nOptions (0)
{

}

CAdamoSynComboBox::~CAdamoSynComboBox()
{
}


BEGIN_MESSAGE_MAP(CAdamoSynComboBox, CComboBox)
END_MESSAGE_MAP()

/*
** Create :
*/
BOOL CAdamoSynComboBox::Create ()
{
	BOOL b = CComboBox::Create (WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, GetObjStyle ().GetRect (), m_pParent, GetID ());
	if (b)
		CreateFont ();
	return TRUE;
}

/*
** Destroy :
*/
BOOL CAdamoSynComboBox::Destroy ()
{
	return FALSE;
}

/*
** CreateFont :
*/
void CAdamoSynComboBox::CreateFont ()
{
	m_pFont = GETSYNCONT ()->CreateFont (m_objStyle.GetRSFont().strFaceName, m_objStyle.GetRSFont().nSize, m_objStyle.GetRSFont().bBold);
	if (m_pFont)
		SetFont (m_pFont);
}

/*
** OnSave :
*/
int CAdamoSynComboBox::OnSave ()
{
	CADORecordset pRSGrid;
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CResourceFile *pRF = pSynContainer->GetResourceFile ();
	CADORecordset *pRS = pRF->GetRSCtrlSyn ();
    COleSafeArray sa;
    CString strField;
    _variant_t v;
	int nListSel = 0;
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
	nListSel = GetCurSel ();
	pRS->m_pRecordset->PutIndex (CString (RESINDEX_RESCONTROLLISINOTTICO).AllocSysString ());
    pRS->m_pRecordset->Seek (sa, adSeekFirstEQ);
    if (!pRS->IsEof())   {
		/* record trovato, scriviamo il valore */
        pRS->GetFieldValue ("NomePagina", strField);
		if (strResourceName == strField)   {
            pRS->Edit ();
			pRS->SetFieldValue ("ValoreDouble", (double)nListSel);
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
		pRS->SetFieldValue ("ValoreDouble", (double) nListSel);
		pRS->SetFieldValue ("Tipo", (int)LUA_TNUMBER);
		pRS->AddNew ();
		pRS->Update ();
	}
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynComboBox::OnLoad ()
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
	SetCurSel ((int) fp);	
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynComboBox::OnDownload ()
{
	int nListSel = GetCurSel ();

	stRSVariabili& var = GetObjVar ().GetObjVar ();
	if (var.m_strName != "")
		GETSYNCONT ()->OnUpdateDownload (var.m_strName, var.m_nRow, var.m_strCol, var.m_nType, "", (double) nListSel);
	return 0;
}

/*
** AddRSOption :
*/
void CAdamoSynComboBox::AddRSOption (stRSOpzioni* pOpt, int nPos)
{
    if (m_nOptions<MAX_RSOPTIONS)   {
        if (nPos!=-1)   {
            memmove (&m_stOpzioni[nPos], &m_stOpzioni[nPos+1], MAX_RSOPTIONS-nPos-1);
            m_stOpzioni[nPos]=*pOpt;
        }
        else
            m_stOpzioni[m_nOptions]=*pOpt;
        m_nOptions++;
    }    
}

/*
** GetRSOptions :
*/
void CAdamoSynComboBox::GetRSOptions (stRSOpzioni* pOptions, int* nOptions)
{
    for (int i=0; i<m_nOptions; i++)
        pOptions[i]=m_stOpzioni[i];
    *nOptions=m_nOptions;
}

/*
** FillOptions :
*/
void CAdamoSynComboBox::FillOptions ()
{
	for (int i = 0; i < m_nOptions; i++)   {
		InsertString (-1, m_stOpzioni[i].strValue);
	}
}
