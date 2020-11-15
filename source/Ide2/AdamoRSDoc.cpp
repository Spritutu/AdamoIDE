// AdamoRSDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoRSDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDoc

IMPLEMENT_DYNCREATE(CAdamoRSDoc, CDocument)

CAdamoRSDoc::CAdamoRSDoc() : m_imgJPG (NULL)
{
}

BOOL CAdamoRSDoc::OnNewDocument()
{
    SetTitle (LOADSTRING (IDS_ADAMORSDOC_1));
	if (!CDocument::OnNewDocument())
		return FALSE;
    /* recuperiamo il path delle risorse */
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        CString strPath=pPrj->GetResourcesPathName ();
        CString strXMLPath=pPrj->GetConfXMLPathName ();
        /* apriamo il file delle risorse */
        if (m_rf.ApriFileDiRisorse (strPath, 0))   {
            m_rf.CreaFileDiRisorse (strPath, strXMLPath);
            m_rf.ApriFileDiRisorse (strPath, 0);
        }
        /* settiamo l'oggetto delle traduzioni */
        m_cl.SetAdamoRSDoc (this);
        m_cl.SetTraduzioniObj (pPrj->GetTraduzioni ());
        /* carichiamo il tree della docking */
        PopulateRSTree ();
        /* settiamo il ctrl-list dei dati dentro il container degli oggetti diagram */
        m_ct.SetCtrlList (&m_cl);
    }
	return TRUE;
}

CAdamoRSDoc::~CAdamoRSDoc()
{
    _delete (m_imgJPG);
}


BEGIN_MESSAGE_MAP(CAdamoRSDoc, CDocument)
	//{{AFX_MSG_MAP(CAdamoRSDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDoc diagnostics

#ifdef _DEBUG
void CAdamoRSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAdamoRSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDoc serialization

void CAdamoRSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

void CAdamoRSDoc::OnCloseDocument() 
{
    SaveXMLRecord ();
    m_cl.Clear ();
    m_ct.Clear ();
    m_rf.ChiudiFileDiRisorse ();
    /* andiamo a prendere la docking */
    CAdamoRSCatalogue* rsCat=((CMainFrame*)AfxGetMainWnd ())->GetDockRSCatalogue ()->GetView ();
    /* puliamo la docking */
    if (rsCat)
        rsCat->Clear ();
	/* settiamo a null la view attiva */
    CResourcesTemplate* tmpl=(CResourcesTemplate*) GetDocTemplate ();
    if (tmpl)
        tmpl->SetActiveView (NULL);
	CDocument::OnCloseDocument();
}

/*
** AddEmptyRecord :
*/
int  CAdamoRSDoc::AddEmptyRecord (CString str, eResources e)
{
    //AggiungiTraduzioneRecord (str, e);
    return m_rf.AggiungiRecordVuoto (str, e);
}

/*
** DeleteRecord :
*/
int CAdamoRSDoc::DeleteRecord (CString str, eResources e)
{
    //CancellaTraduzioneRecord (str, e);
    CancellaMessaggiTradotti (str, e);
    return m_rf.CancellaRecordVuoto (str, e);
}

/*
** RenameRecord :
*/
int CAdamoRSDoc::RenameRecord (CString strPrev, CString strNew, eResources e)
{
	int nB = -1;
    //RinominaTraduzioneRecord (strPrev, strNew, e);
	if (!m_rf.RinominaRecord (strPrev, strNew, e))   {
		RinominaDocking (strNew);
		nB = 0;
	}
	return nB;
}

/*
** PopulateRSTree : popola il tree della docking.
*/
int CAdamoRSDoc::PopulateRSTree ()
{
    CString str, strTrad, strImage;
    eResources e;
    
    /* andiamo a prendere la docking */
    CAdamoRSCatalogue* rsCat=((CMainFrame*)AfxGetMainWnd ())->GetDockRSCatalogue ()->GetView ();
    if (rsCat)   {
        /* leggiamo il primo record nel data base */
        if (!m_rf.GetFirstRSRecord (str, e))   {
            do   {
                /* prendiamo la risorsa tradotta */
                strTrad=GetRSTranslated (str, e);
                /* aggiungiamo al tree */
                rsCat->AddCatalogueElement (str, strTrad, e, RS_LOAD);
                /* andiamo a leggere i record successivi */
            } while (!m_rf.GetNextRSRecord (str, e));
        }
        if (!m_rf.GetFirstRSImage (strImage))   {
            do   {
                /* aggiungiamo al tree */
                rsCat->AddImageElement (strImage);
                /* andiamo a leggere i record successivi */
            } while (!m_rf.GetNextRSImage (strImage));
        }
    }
    return 0;
}

/*
** LoadHTMLRecord :
*/
int CAdamoRSDoc::LoadXMLRecord (CString str, eResources e)
{
    CString strXML;
	bool bIsMainSyn;
    int n=-1;

    /* recuperiamo per prima cosa il record con l'html */
    if (!m_rf.GetXML (str, e, strXML, bIsMainSyn))   {
        m_strDoc=str; m_eDoc=e;
        /* ora carichiamo la lista dei controlli */
        m_cl.Load (strXML);
		m_cl.SetMainSyn (bIsMainSyn);
        m_ct.Load ();
        n=0;
    }
    return n;
}

/*
** SaveHTMLRecord :
*/
int CAdamoRSDoc::SaveXMLRecord ()
{
    CString strXML;
    int n=-1;

    /* se e' modificato, salviamolo */
    if (m_ct.IsModified ())   {
        if (AfxMessageBox (LOADSTRING (IDS_ADAMORSDOC_2), MB_YESNO|MB_ICONQUESTION)==IDYES)   {
            m_cl.Ready ();
            m_ct.SetModified (FALSE);
            CAdamoRSForm *pForm=m_cl.GetFormCtrl ();
            if (pForm)   {
                CancellaMessaggiTradotti ();
                m_cl.Save (strXML);
                m_rf.ModificaRecord (m_strDoc, m_eDoc, strXML, pForm->GetMainSynoptic ());
				m_cl.SaveSynCtrls (m_strDoc, &m_rf);
            }
			m_ct.SetModified (FALSE);
            n=0;
        }
    }
    return n;
}

/*
** IsCurrentResource :
*/
int CAdamoRSDoc::IsCurrentResource (CString str, eResources e)
{
    return str==m_strDoc && e==m_eDoc;
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDoc commands
void CAdamoRSDoc::UpdateDiagramEntitySize ()
{
    CAdamoRSDiagramObjects* pDE;
    pDE=(CAdamoRSDiagramObjects*)m_ct.GetSelectedObj();
    if (pDE)   {
        CAdamoRSControl* pAC=pDE->GetRSControl ();    
        if (pAC)   {
            CRect rc=pAC->GetObjStyle().GetRect();
            pDE->SetRect (rc);
        }
    }
}

/*
** UpdateDiagramEntityValue :
*/
void CAdamoRSDoc::UpdateDiagramEntityValue ()
{
    CString strImage;
    CAdamoRSDiagramObjects* pDE;
    pDE=(CAdamoRSDiagramObjects*)m_ct.GetSelectedObj();
    if (pDE)   {
        CAdamoRSControl* pAC=pDE->GetRSControl ();    
        if (pAC)   {
            switch (pAC->KindOf ())   {
                case RSLabel :
                    pDE->SetTitle (((CAdamoRSLabel*)pAC)->GetValue());
                    break;
                case RSButton :
                    pDE->SetTitle (((CAdamoRSButton*)pAC)->GetValue());
                    break;
                case RSGroup :
                    pDE->SetTitle (((CAdamoRSGroup*)pAC)->GetValue());
                    break;
                case RSImage :
                    strImage = ((CAdamoRSImage*)pAC)->GetIDImage();
                    if (strImage != "")   {
                        CxImage* pImage=CaricaCxImage (strImage);
                        ((CAdamoRSDiagramImage*) pDE)->SetImage (pImage);
                    }
                    ((CAdamoRSDiagramImage*) pDE)->SetTransparent (((CAdamoRSImage*)pAC)->IsTransparent ());
                    ((CAdamoRSDiagramImage*) pDE)->SetTransparentColor (((CAdamoRSImage*)pAC)->GetTransparentColor ());
                    break;
                case RSIPicture :
                    strImage = ((CAdamoRSIPicture*)pAC)->GetFirstIDImage();
                    if (strImage != "")   {
                        CxImage* pImage=CaricaCxImage (strImage);
                        ((CAdamoRSDiagramPicture*) pDE)->SetImage (pImage);
                    }
                    ((CAdamoRSDiagramPicture*) pDE)->SetTransparent (((CAdamoRSIPicture*)pAC)->IsTransparent ());
                    ((CAdamoRSDiagramPicture*) pDE)->SetTransparentColor (((CAdamoRSIPicture*)pAC)->GetTransparentColor ());
                    break;
            }
        }
    }
}

/*
** GetCurrentXMLRecord :
*/
void CAdamoRSDoc::GetCurrentXMLRecord (CString& strXML)
{
    CAdamoRSForm *pForm=m_cl.GetFormCtrl ();
    if (pForm)   {
        CancellaMessaggiTradotti ();
        m_cl.Save (strXML);
    }
}

/*
** CancellaMessaggiTradotti :
*/
void CAdamoRSDoc::CancellaMessaggiTradotti (CString strResource, eResources e)
{
    CString str, strId, strMsg, strDesc;

    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
        CTraduzioni* pTr=pPrj->GetTraduzioni ();
        if (pTr)   {
            /* ora cancelliamo tutti i controlli */
            str=GetRootString(e)+"//"+strResource;
            pTr->CancellaSetDiMessaggiConStringa (2, str);
        }
    }
}

/*
** CancellaMessaggiTradotti :
*/
void CAdamoRSDoc::CancellaMessaggiTradotti ()
{
    CancellaMessaggiTradotti (m_strDoc, m_eDoc);
}

/*
** IsDifferentResource :
*/
bool CAdamoRSDoc::IsDifferentResource (CString strResource, eResources e)
{
    return !(strResource==m_strDoc && e==m_eDoc);
}

/*
** EsisteImmagine :
*/
bool CAdamoRSDoc::EsisteImmagine (CString strID)
{
    return m_rf.EsisteImmagine (strID)!=0;
}

/*
** EsisteImmagine :
*/
bool CAdamoRSDoc::AggiungiImmagine (CString strID, char* p, DWORD nLength)
{
    return m_rf.AggiungiImmagine (strID, p, nLength)==0;
}

/*
** CancellaImmagine :
*/
bool CAdamoRSDoc::CancellaImmagine (CString strID)
{
    return m_rf.CancellaImmagine (strID)==0;
}

/*
** LunghezzaImmagine :
*/
DWORD CAdamoRSDoc::LunghezzaImmagine (CString strID)
{
    return m_rf.LunghezzaImmagine (strID);
}

/*
** CaricaImmagine :
*/
bool CAdamoRSDoc::CaricaImmagine (CString strID, char* p)
{
    return m_rf.CaricaImmagine (strID, p)==0;
}

/*
** CaricaCxImage :
*/
CxImage* CAdamoRSDoc::CaricaCxImage (CString strID)
{
    CxImage* pImage=NULL;
    DWORD dwLength; 
    char* p;

    if (EsisteImmagine (strID))   {
        /* carichiamo la lunghezza del chunk da caricare */
        if ((dwLength=LunghezzaImmagine (strID))>0)   {
            BeginWaitCursor ();
            p=new char [dwLength];
            CaricaImmagine (strID, p);
            /* creiamo l'immagine */
            pImage=new CxImage ((BYTE*)p, dwLength, CXIMAGE_FORMAT_JPG);
            /* rinfreschiamo il video */
            delete p;
            EndWaitCursor ();
        }
    }
    return pImage;
}

/*
** RinominaRSImage :
*/
bool CAdamoRSDoc::RinominaImmagine (CString strImage, CString strID)
{
    return m_rf.RinominaImmagine (strImage, strID)==0;
}

/*
** AggiungiTraduzioneRecord :
*/
int CAdamoRSDoc::AggiungiTraduzioneRecord (CString str, eResources e)
{
    CProject* pPrj=GETWS()->GetActiveProject ();
    int nB=0;

    if (pPrj)   {
        CTraduzioni* pTr=pPrj->GetTraduzioni ();
        if (pTr)   {
            str=GetRootString(e)+"//"+str;
            if (!pTr->ScriviMessaggio (2, -1, str, str, ""))
                nB=-1;
        }
    }
    return nB;
}

/*
** RinominaTraduzioneRecord :
*/
int CAdamoRSDoc::RinominaTraduzioneRecord (CString strPrev, CString strNew, eResources e)
{
    CProject* pPrj=GETWS()->GetActiveProject ();
    CString strKey, strMessaggio, strDescrizione, strPathPrev, strPathNew;
    int nB=0;

    if (pPrj)   {
        CTraduzioni* pTr=pPrj->GetTraduzioni ();
        if (pTr)   {
            strPathPrev=GetRootString(e)+"//"+strPrev;
            strPathNew =GetRootString(e)+"//"+strNew;
            pTr->RinominaSetDiMessaggiConStringa (2, strPathPrev, strPathNew);
        }
    }
    return nB;
}

/*
** RinominaTraduzioneRecord :
*/
CString CAdamoRSDoc::RitornaTraduzioneRecord (CString str, eResources e)
{
    CProject* pPrj=GETWS()->GetActiveProject ();
    CString strKey, strMessaggio, strDescrizione, strPathPrev, strPathNew;
    LONG dwNumero;
    CString strPath, strRet;

    if (pPrj)   {
        CTraduzioni* pTr=pPrj->GetTraduzioni ();
        if (pTr)   {
            strPath=GetRootString(e)+"//"+str;
            if (pTr->MessaggioStringa (2, strPath, &dwNumero, &strMessaggio, &strDescrizione))
                strRet=strMessaggio;
        }
    }
    return strRet;
}

/*
** CancellaTraduzioneRecord :
*/
int CAdamoRSDoc::CancellaTraduzioneRecord (CString strResource, eResources e)
{
    CProject* pPrj=GETWS()->GetActiveProject ();
    CString str;
    
    if (pPrj)   {
        CTraduzioni* pTr=pPrj->GetTraduzioni ();
        if (pTr)   {
            str=GetRootString(e)+"//"+strResource;
            pTr->CancellaMessaggioConStringa (2, str);
        }
    }
    return 0;
}

/*
** SincronizzaVariabili :
*/
int CAdamoRSDoc::SincronizzaVariabili ()
{
    return m_rf.SincronizzaVariabili ();
}

/*
** GetRSTranslated :
*/
CString CAdamoRSDoc::GetRSTranslated (CString str, eResources e)
{
    CString strRec=RitornaTraduzioneRecord (str, e), strRet;
    if (strRec!="")   {
        const char *pRet=strRec.GetBuffer (0);
        pRet=strchr (pRet, '/');
        strRec.ReleaseBuffer ();
        if (pRet)
            strRet=pRet+2;
    }
    return strRet;
}

/*
** GetRootString :
*/
CString CAdamoRSDoc::GetRootString (eResources e)
{
    CString str;

    switch (e)   {
        case eParametriche :
            str="Parametriche";
            break;
        case eUtensili :
            str="Utensili";
            break;
        case eTestine :
            str="Testine";
            break;
        case eDialogs :
            str="Dialogs";
            break;
    }
    return str;
}

/*
** RinominaDocking :
*/
void CAdamoRSDoc::RinominaDocking (CString strNew)
{
	CAdamoRSProperties* pRSProp=((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ();
	if (pRSProp)   {
		pRSProp->RenameRSForm (strNew);
	}
}

/*
** GetResourceType :
*/
eResources CAdamoRSThiraCtrlList::GetResourceType ()
{
    return m_pDoc->m_eDoc;
}

/*
** GetResourceName :
*/
CString CAdamoRSThiraCtrlList::GetResourceName ()
{
    return m_pDoc->m_strDoc;
}

/*
** GetFormSize :
*/
CSize CAdamoRSThiraCtrlList::GetFormSize ()
{
    if (m_pEditor)
        return m_pEditor->GetVirtualSize ();
    return CSize (200, 100);
}

/*
** SaveSynCtrls :
*/
void CAdamoRSThiraCtrlList::SaveSynCtrls (CString strResourceName, CResourceFile *pRF)
{
	CString strValue;
	double fpValue;
	int nTipo;
	bool bUpdate;

	CADORecordset *pRS = pRF->GetRSCtrlSyn ();
	/* cancelliamo tutti i record gia esitenti */
	DelExistingRecords (pRS, strResourceName);
    for (POSITION pos=GetHeadPosition (); pos; )   {
        /* salviamo tutti i controlli uno ad uno */
		bUpdate = false;
		fpValue = 0.0; strValue = "";
        CAdamoRSControl* pCtrl=(CAdamoRSControl*) GetNext (pos);
		switch (pCtrl->KindOf ())   {
			case RSInputText :
				bUpdate = true;	
				strValue = "Text";
				nTipo = LUA_TSTRING;
				break;
			case RSInputRadio :
				bUpdate = true;
				nTipo = LUA_TBOOLEAN;
				fpValue = 0;
				break;
			case RSInputCheck :
				bUpdate = true;	
				nTipo = LUA_TBOOLEAN;
				fpValue = 0;
				break;
			case RSList :
				bUpdate = true;	
				nTipo = LUA_TNUMBER;
				fpValue = 0;
				break;
			case RSCombo :
				bUpdate = true;	
				nTipo = LUA_TNUMBER;
				fpValue = 0;
				break;
			case RSGrid :
				bUpdate = true;	
				nTipo = LUA_TNUMBER;
				strValue = pCtrl->GetObjVar().m_strTable;
				fpValue = 0;
				break;
		}
		if (bUpdate)   {
			pRS->AddNew ();
			pRS->SetFieldValue ("NomePagina", strResourceName);
			pRS->SetFieldValue ("IDControllo", pCtrl->GetID ());
			pRS->SetFieldValue ("TipoControllo", pCtrl->KindOf ());
			pRS->SetFieldValue ("NomeVariabile", pCtrl->GetObjVar().m_strName);
			pRS->SetFieldValue ("Riga", pCtrl->GetObjVar().m_nRow);
			pRS->SetFieldValue ("Colonna", pCtrl->GetObjVar().m_strCol);
			pRS->SetFieldValue ("ValoreStringa", strValue);
			pRS->SetFieldValue ("ValoreDouble", fpValue);
			pRS->SetFieldValue ("Tipo", nTipo);
			pRS->Update ();
		}
	}
}

/*
** DelExistingRecords :
*/
void CAdamoRSThiraCtrlList::DelExistingRecords (CADORecordset *pRS, CString strResourceName)
{
    COleSafeArray sa;
    CString strField;
    _variant_t v;
    int nB=-1;
    long i;

    v=strResourceName.AllocSysString ();
    i=0;
    sa.CreateOneDim (VT_VARIANT, 1);
    sa.PutElement (&i, &v);
	pRS->m_pRecordset->PutIndex (CString (RESINDEX_RESCONTROLLISINOTTICO).AllocSysString ());
    pRS->m_pRecordset->Seek (sa, adSeekFirstEQ);
    if (!pRS->IsEof())   {
        pRS->GetFieldValue ("NomePagina", strField);
		if (strResourceName == strField)   {
			do   {
				pRS->Delete ();
				pRS->MoveNext ();
				if (pRS->IsEOF())
					break;
				pRS->GetFieldValue ("NomePagina", strField);
			} while (strField == strResourceName);
			pRS->Update ();
		}
    }
	else
		pRS->m_pRecordset->Requery (-1);
}
