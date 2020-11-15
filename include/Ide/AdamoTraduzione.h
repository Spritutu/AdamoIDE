/*
 * AdamoTraduzione
 */

#ifndef __ADAMO__TRADUZIONI__
#define __ADAMO__TRADUZIONI__

/* include files */
#include "path.h"

/* namespace */
using namespace nsPath;

/* import directive */
#import "d:\sviluppo\Adamo\AdamoBin\Traduzioni.DLL" no_namespace

class CTraduzioni 
{
    public :
        CTraduzioni ();
	    bool ApriFileDiLingua (CPath pNomeFile, int nFlags);
        bool CreaFileDiLingua (CPath pNomeFile);
	    bool ChiudiFileDiLingua ();
	    bool MessaggioNumero (LONG dwTipo, LONG dwNumero, CString* szStringa, CString* szMessaggio, CString* szDescrizione, ...);
	    bool MessaggioStringa (LONG dwTipo, CString szStringa, LONG* dwNumero, CString* szMessaggio, CString* szDescrizione, ...);
	    bool ScriviMessaggio (LONG dwTipo, LONG dwNumero, CString szStringa, CString szMessaggio, CString szCommento);
	    bool CancellaMessaggioConNumero (LONG dwTipo, LONG dwNumero);
	    bool CancellaMessaggioConStringa (LONG dwTipo, CString szStringa);
	    bool PrimoNumeroLibero(LONG dwTipo, LONG dwNumero, LONG* nPrimoLibero);
	    bool SettaLinguaCorrente(LONG dwLanguage);
	    bool RitornaLinguaCorrente(LONG* dwLanguage);
	    bool PrimoMessaggioNumero(LONG dwTipo, LONG dwNumero);
	    bool PrimoMessaggioStringa(LONG dwTipo, CString szStringa);
	    bool Successivo(LONG dwTipo, LONG* dwNumero, CString* szStringa, CString* szMessaggio, CString* szDescrizione);
        bool UltimoMessaggioNumero(LONG dwTipo, LONG dwNumero);
        bool UltimoMessaggioStringa (LONG dwTipo, CString szStringa);
        bool Precedente (LONG dwTipo, LONG* dwNumero, CString* szStringa, CString* szMessaggio, CString* szDescrizione);
        bool IsOpen ()   { return m_pTraduzioniObj!=NULL; }
	    bool CancellaSetDiMessaggiConNumero (LONG dwTipo, LONG dwNumero);
	    bool CancellaSetDiMessaggiConStringa (LONG dwTipo, CString szStringa);
        bool CambioLingua ();
		void PropagaMessaggio (bool bPropaga, bool bForceUpdate);
        bool RinominaSetDiMessaggiConStringa (LONG dwTipo, CString szStringaVecchia, CString szStringaNuova);
    private :
        ITraduzioniObjPtr  m_pTraduzioniObj;
        IChangeLanguagePtr m_pChangeLanguageObj;
		bool m_bPropaga;
};

#endif
