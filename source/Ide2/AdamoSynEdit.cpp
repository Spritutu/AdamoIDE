// AdamoSynEdit.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "AdamoSynContainer.h"
#include "AdamoSynPage.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynEdit.h"

// CAdamoSynEdit
IMPLEMENT_DYNAMIC(CAdamoSynEdit, CEdit)

CAdamoSynEdit::CAdamoSynEdit()
{

}

CAdamoSynEdit::~CAdamoSynEdit()
{
}


BEGIN_MESSAGE_MAP(CAdamoSynEdit, CParsedEdit)
END_MESSAGE_MAP()

/*
** Create :
*/
BOOL CAdamoSynEdit::Create ()
{
	DWORD dw = 0;
	switch (m_eType)   {
		case eNumbers :
			dw = PES_NUMBERS;
			break;
		case eFloat :
			dw = PES_NUMBERS | PES_FLOATS;
			break;
		case eHex :
			dw = PES_NUMBERS | PES_FLOATS | PES_HEXNUMBERS;
			break;
		case eLetters :
			dw = PES_NUMBERS | PES_LETTERS | PES_UNDERLINE | PES_HEXNUMBERS;
			break;
	}
	BOOL b = CParsedEdit::Create (WS_CHILD | WS_VISIBLE | dw, GetObjStyle ().GetRect (), m_pParent, GetID ());
	if (b)
		CreateFont ();
	return TRUE;
}

/*
** Destroy :
*/
BOOL CAdamoSynEdit::Destroy ()
{
	return FALSE;
}

/*
** OnSave :
*/
int CAdamoSynEdit::OnSave ()
{
	CADORecordset pRSGrid;
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CResourceFile *pRF = pSynContainer->GetResourceFile ();
	CADORecordset *pRS = pRF->GetRSCtrlSyn ();
    COleSafeArray sa;
    CString strField;
    _variant_t v;
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
		/* record trovato, scriviamo il valore */
        pRS->GetFieldValue ("NomePagina", strField);
		if (strResourceName == strField)   {
            pRS->Edit ();
			switch (m_ad.nType)   {
				case LUA_TNUMBER :
					pRS->SetFieldValue ("ValoreDouble", (double)m_ad.AdamoData.fp);
					pRS->SetFieldValue ("ValoreStringa", "");
					pRS->SetFieldValue ("Tipo", (int)LUA_TNUMBER);
					break;
				case LUA_TSTRING :
					pRS->SetFieldValue ("ValoreDouble", 0.0);
					pRS->SetFieldValue ("ValoreStringa", CString (m_ad.AdamoData.str));
					pRS->SetFieldValue ("Tipo", (int)LUA_TSTRING);
					break;
			}
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
		switch (m_ad.nType)   {
			case LUA_TNUMBER :
				pRS->SetFieldValue ("ValoreDouble", (double)m_ad.AdamoData.fp);
				pRS->SetFieldValue ("ValoreStringa", "");
				pRS->SetFieldValue ("Tipo", (int)LUA_TNUMBER);
				break;
			case LUA_TSTRING :
				pRS->SetFieldValue ("ValoreDouble", 0.0);
				pRS->SetFieldValue ("ValoreStringa", CString (m_ad.AdamoData.str));
				pRS->SetFieldValue ("Tipo", (int)LUA_TSTRING);
				break;
		}
		pRS->AddNew ();
		pRS->Update ();
	}
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynEdit::OnLoad ()
{
	CADORecordset pRSGrid;
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CResourceFile *pRF = pSynContainer->GetResourceFile ();
	CADORecordset *pRS = pRF->GetRSCtrlSyn ();
    COleSafeArray sa;
    CString strField, str;
    _variant_t v;
	double fp = 0.0;
    int nID, nTipo, nB=-1;
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
		pRS->GetFieldValue ("IDControllo", nID);
		if (strResourceName == strField && nID == GetID ())   {
			pRS->GetFieldValue ("Tipo", nTipo);
			m_ad.nType = nTipo;
			switch (nTipo)   {
				case LUA_TNUMBER :
					pRS->GetFieldValue ("ValoreDouble", m_ad.AdamoData.fp);

					break;
				case LUA_TSTRING :
					pRS->GetFieldValue ("ValoreStringa", str);
					strncpy (m_ad.AdamoData.str, str, NMAXSTRING-1);
					m_ad.AdamoData.str[NMAXSTRING-1] = '\0';
					break;
			}

		}
    }
	else
		pRS->m_pRecordset->Requery (-1);
	switch (m_eType)   {
		case eNumbers :
			str.Format ("%d", (int)m_ad.AdamoData.fp);
			break;
		case eFloat :
			str.Format ("%f", m_ad.AdamoData.fp);
			break;
		case eHex :
			str.Format ("%X", (unsigned int) m_ad.AdamoData.fp);
			break;
		case eLetters :
			break;
	}
	SetWindowText (str);
	return 0;
}

/*
** OnDownload :
*/
int CAdamoSynEdit::OnDownload ()
{
	stRSVariabili& var = GetObjVar ().GetObjVar ();
	if (var.m_strName != "")
		GETSYNCONT ()->OnUpdateDownload (var.m_strName, var.m_nRow, var.m_strCol, var.m_nType, "", 0.0);
	return 0;
}

/*
** CreateFont :
*/
void CAdamoSynEdit::CreateFont ()
{
	m_pFont = GETSYNCONT ()->CreateFont (m_objStyle.GetRSFont().strFaceName, m_objStyle.GetRSFont().nSize, m_objStyle.GetRSFont().bBold);
	if (m_pFont)
		SetFont (m_pFont);
}

/*
** OnEnter :
*/
int CAdamoSynEdit::OnEnter ()
{
	CString strEdit;
	int nB = 0;
	char *p;

	GetWindowText (strEdit);
	m_ad.nType = LUA_TNIL;
	switch (m_eType)   {
		case eNumbers :
		case eFloat :
			m_ad.AdamoData.fp = strtod (strEdit, &p);
			if (*p == '\0')
				m_ad.nType = LUA_TNUMBER;
			break;
		case eHex :
			m_ad.AdamoData.fp = strtol (strEdit, &p, 0);
			if (*p == '\0')
				m_ad.nType = LUA_TNUMBER;
			break;
		case eLetters :
			strncpy (m_ad.AdamoData.str, strEdit, NMAXSTRING-1);
			m_ad.AdamoData.str[NMAXSTRING-1] = '\0';
			m_ad.nType = LUA_TSTRING;
			break;
	}
	return nB;
}

