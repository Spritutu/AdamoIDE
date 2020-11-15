/* 
 * AdamoTraduzione
 */

/* include files */
#include "stdafx.h"
#include "AdamoTraduzione.h"

/* debug memory directive */
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
 *
 */
CTraduzioni::CTraduzioni () : m_pTraduzioniObj (NULL), m_pChangeLanguageObj (NULL), m_bPropaga (false)
{
}

/*
 *
 */
bool CTraduzioni::ApriFileDiLingua (CPath pNomeFile, int nFlags)
{
    CString szNomeFile (pNomeFile);
    BSTR bstr;
    bool nB=false;
    HRESULT hr = m_pTraduzioniObj.CreateInstance(__uuidof(TraduzioniObj));
    if (hr==S_OK)   {
		if (!m_pTraduzioniObj->ApriFileDiLingua(bstr=szNomeFile.AllocSysString(), 0))   {
			m_pTraduzioniObj->put_PropagaMessaggio (m_bPropaga);
            nB=true;
		}
        else
            m_pTraduzioniObj.Release ();
        SysFreeString (bstr);
    }
    return nB;
}

/*
 *
 */
bool CTraduzioni::CreaFileDiLingua (CPath pNomeFile)
{
    CString szNomeFile (pNomeFile);
    BSTR bstr;
    bool nB=false;
    HRESULT hr = m_pTraduzioniObj.CreateInstance(__uuidof(TraduzioniObj));
    if (hr==S_OK)   {
        if (!m_pTraduzioniObj->CreaFileDiLingua(bstr=szNomeFile.AllocSysString()))   {
            CPath pPath (szNomeFile);
            pPath.RenameExtension ("bck");
            CopyFile ((const char*)pNomeFile, (const char*)pPath, false);
            nB=true;
        }
        SysFreeString (bstr);
    }
    return nB;
}

/*
 *
 */
bool CTraduzioni::ChiudiFileDiLingua ()
{
    bool nB=false;
    if (m_pTraduzioniObj)   {
        m_pTraduzioniObj->ChiudiFileDiLingua ();
        m_pTraduzioniObj.Release ();
        nB=true;
    }
    return nB;
}

/*
 *
 */
bool CTraduzioni::MessaggioNumero (LONG dwTipo, LONG dwNumero, CString* szStringa, CString* szMessaggio, CString* szDescrizione, ...)
{
    bool b=false;
    BSTR strStringa, strMessaggio, strDescrizione;    
    if (m_pTraduzioniObj->MessaggioNumero (dwTipo, dwNumero, &strStringa, &strMessaggio, &strDescrizione, NULL)==S_OK)   {
        *szStringa=strStringa;
        *szMessaggio=strMessaggio;
        *szDescrizione=strDescrizione;
        SysFreeString (strStringa);
        SysFreeString (strMessaggio);
        SysFreeString (strDescrizione);
        b=true;
    }
    return b;
}

/*
 *
 */
bool CTraduzioni::MessaggioStringa (LONG dwTipo, CString szStringa, LONG* dwNumero, CString* szMessaggio, CString* szDescrizione, ...)
{
    bool b=false;
    BSTR strStringa, strMessaggio, strDescrizione;    
    if (m_pTraduzioniObj->MessaggioStringa (dwTipo, strStringa=szStringa.AllocSysString(), dwNumero, &strMessaggio, &strDescrizione, NULL)==S_OK)   {
        *szMessaggio=strMessaggio;
        *szDescrizione=strDescrizione;
        SysFreeString (strMessaggio);
        SysFreeString (strDescrizione);
        b=true;
    }
    SysFreeString (strStringa);
    return b;
}

/*
 *
 */
bool CTraduzioni::ScriviMessaggio (LONG dwTipo, LONG dwNumero, CString szStringa, CString szMessaggio, CString szCommento)
{
    BSTR bstrStringa, bstrMessaggio, bstrDescrizione;
    bool nB=false;
    if (m_pTraduzioniObj->ScriviMessaggio (dwTipo, dwNumero, 
                                           bstrStringa=szStringa.AllocSysString(), 
                                           bstrMessaggio=szMessaggio.AllocSysString(), 
                                           bstrDescrizione=szCommento.AllocSysString())==S_OK)   {
        SysFreeString (bstrStringa);
        SysFreeString (bstrMessaggio);
        SysFreeString (bstrDescrizione);
        nB=true;
    }
    return nB;
}

/*
 *
 */
bool CTraduzioni::CancellaMessaggioConNumero (LONG dwTipo, LONG dwNumero)
{
    bool nB=false;
    if (m_pTraduzioniObj->CancellaMessaggioConNumero (dwTipo, dwNumero)==S_OK)
        nB=true;
    return nB;
}

/*
 *
 */
bool CTraduzioni::CancellaMessaggioConStringa (LONG dwTipo, CString szStringa)
{
    BSTR bstrStringa;
    bool nB=false;
    if (m_pTraduzioniObj->CancellaMessaggioConStringa (dwTipo, bstrStringa=szStringa.AllocSysString ())==S_OK)
        nB=true;
    SysFreeString (bstrStringa);
    return nB;
}

/*
 *
 */
bool CTraduzioni::PrimoNumeroLibero(LONG dwTipo, LONG dwNumero, LONG* nPrimoLibero)
{
    return true;
}

/*
 *
 */
bool CTraduzioni::SettaLinguaCorrente(LONG dwLanguage)
{
    return true;
}

/*
 *
 */
bool CTraduzioni::RitornaLinguaCorrente(LONG* dwLanguage)
{
    return true;
}

/*
 *
 */
bool CTraduzioni::PrimoMessaggioNumero(LONG dwTipo, LONG dwNumero)
{
    return m_pTraduzioniObj->PrimoMessaggioNumero (dwTipo, dwNumero)==S_OK;
}

/*
 *
 */
bool CTraduzioni::PrimoMessaggioStringa (LONG dwTipo, CString szStringa)
{
    HRESULT hr;
    BSTR bstrStringa;
    hr=m_pTraduzioniObj->PrimoMessaggioStringa (dwTipo, bstrStringa=szStringa.AllocSysString());
    SysFreeString (bstrStringa);
    return hr==S_OK;
}

/*
 *
 */
bool CTraduzioni::Successivo (LONG dwTipo, LONG* dwNumero, CString* szStringa, CString* szMessaggio, CString* szDescrizione)
{
    bool b;
    BSTR strStringa, strMessaggio, strDescrizione;    
    b=m_pTraduzioniObj->Successivo (dwTipo, dwNumero, &strStringa, &strMessaggio, &strDescrizione)==0;
    *szStringa=strStringa;
    *szMessaggio=strMessaggio;
    *szDescrizione=strDescrizione;
    SysFreeString (strStringa);
    SysFreeString (strMessaggio);
    SysFreeString (strDescrizione);
    return b;
}

/*
 *
 */
bool CTraduzioni::UltimoMessaggioNumero(LONG dwTipo, LONG dwNumero)
{
    return m_pTraduzioniObj->UltimoMessaggioNumero (dwTipo, dwNumero)==S_OK;
}

/*
 *
 */
bool CTraduzioni::UltimoMessaggioStringa (LONG dwTipo, CString szStringa)
{
    HRESULT hr;
    BSTR bstrStringa;
    hr=m_pTraduzioniObj->UltimoMessaggioStringa (dwTipo, bstrStringa=szStringa.AllocSysString());
    SysFreeString (bstrStringa);
    return hr==S_OK;
}

/*
 *
 */
bool CTraduzioni::Precedente (LONG dwTipo, LONG* dwNumero, CString* szStringa, CString* szMessaggio, CString* szDescrizione)
{
    bool b;
    BSTR strStringa, strMessaggio, strDescrizione;    
    b=m_pTraduzioniObj->Precedente (dwTipo, dwNumero, &strStringa, &strMessaggio, &strDescrizione)==0;
    *szStringa=strStringa;
    *szMessaggio=strMessaggio;
    *szDescrizione=strDescrizione;
    SysFreeString (strStringa);
    SysFreeString (strMessaggio);
    SysFreeString (strDescrizione);
    return b;
}

/*
** CancellaSetDiMessaggiConNumero :
*/
bool CTraduzioni::CancellaSetDiMessaggiConNumero (LONG dwTipo, LONG dwNumero)
{
    bool nB=false;
    if (m_pTraduzioniObj->CancellaSetDiMessaggiConNumero (dwTipo, dwNumero)==S_OK)
        nB=true;
    return nB;
}

/*
** CancellaSetDiMessaggiConStringa :
*/
bool CTraduzioni::CancellaSetDiMessaggiConStringa (LONG dwTipo, CString szStringa)
{
    BSTR bstrStringa;
    bool nB=false;
    if (m_pTraduzioniObj->CancellaSetDiMessaggiConStringa (dwTipo, bstrStringa=szStringa.AllocSysString ())==S_OK)
        nB=true;
    SysFreeString (bstrStringa);
    return nB;
}

/*
** CambioLingua : gestisce il cambio lingua 
*/
bool CTraduzioni::CambioLingua ()
{
    bool b=false;
    HRESULT hr = m_pChangeLanguageObj.CreateInstance(__uuidof (ChangeLanguage));
    if (hr==S_OK)
        b=m_pChangeLanguageObj->ChangeLanguage ()==S_OK;
    return b;
}

/*
** PropagaMessaggio : gestisce il cambio lingua 
*/
void CTraduzioni::PropagaMessaggio (bool bPropaga, bool bForceUpdate)
{
	m_bPropaga = bPropaga;
	if (bForceUpdate)
		m_pTraduzioniObj->put_PropagaMessaggio (m_bPropaga);
}

/*
** RinominaSetDiMessaggiConStringa : gestisce il cambio lingua 
*/
bool CTraduzioni::RinominaSetDiMessaggiConStringa (LONG dwTipo, CString strOld, CString strNew)
{
    HRESULT hr;
    BSTR bstrOld, bstrNew;
    hr=m_pTraduzioniObj->RinominaSetDiMessaggiConStringa (dwTipo, bstrOld=strOld.AllocSysString(), bstrNew=strNew.AllocSysString ());
    SysFreeString (bstrOld);
    SysFreeString (bstrNew);
    return hr==S_OK;
}
