/*
 * ResourceFile.cpp
 */

/* include files */
#include "stdafx.h"
#include <io.h>
#include "AdamoGlobal.h"
#include "AdamoTraduzione.h"
#include "ResourceFile.h"

/* construction / destruction */
CResourceFile::CResourceFile ()
{
    m_pRsArray.SetSize (MAX_DYNRS);
}

/*
** ApriFileDiRisorse : apre un file di risorse.
*/
int  CResourceFile::ApriFileDiRisorse (CPath strNomeFile, int nFlags)
{
    CString strTable, strBuffer;
    int nB=0;

    if (!_access (strNomeFile, 00))   {
        strBuffer.Format ("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", strNomeFile);
        m_pDb.SetConnectionMode(GetMode (nFlags));
	    if (m_pDb.Open(strBuffer))   {	
            m_strNomeFile=strNomeFile;
		    m_pRs = CADORecordset(&m_pDb);
			CheckCompatibilita ();
		    if (!m_pRs.Open(RESTABLE_RESHTML, CADORecordset::openTable))
		        nB=-3;
            else   {
		        m_pRsImages = CADORecordset(&m_pDb);
		        if (!m_pRsImages.Open(RESTABLE_RESIMAGES, CADORecordset::openTable))
		            nB=-3;
                else   {
		            m_pRsCtrlSyn = CADORecordset(&m_pDb);
		            if (!m_pRsCtrlSyn.Open(RESTABLE_RESCONTROLLISINOTTICO, CADORecordset::openTable))
		                nB=-3;
                }
            }
	    }
	    else
            nB=-2;
    }
    else
        nB=-1;
	return nB;
}

/*
** CreaFileDiRisorse : crea un file di risorse.
*/
int  CResourceFile::CreaFileDiRisorse (CPath strNomeFile, CPath strXMLFile)
{
    _ConnectionPtr pConnection=NULL;
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    CString str;
    int n=0;

    pConnection.CreateInstance(__uuidof(Connection));
    if (pConnection)   {
        pCatalog.CreateInstance(__uuidof(Catalog));
        if (pCatalog)   {
            str.Format ("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", strNomeFile);
            pCatalog->Create (_bstr_t (str));
            pConnection->PutMode((enum ConnectModeEnum)(CADODatabase::connectModeReadWrite|CADODatabase::connectModeShareDenyWrite));
	        if (pConnection->Open (_bstr_t (str), "", "", NULL)==S_OK)   {
                pCatalog->put_ActiveConnection (_variant_t((IDispatch*)pConnection, TRUE));
                pTable.CreateInstance(__uuidof(Table));
                if (pTable)   {
                    pTable->PutParentCatalog (pCatalog);
					CreaTabellaResControlliSinottico (pTable);
					pCatalog->Tables->Append (_variant_t((IDispatch*)pTable, TRUE));
                }
                pTable.CreateInstance(__uuidof(Table));
                if (pTable)   {
                    pTable->PutParentCatalog (pCatalog);
                    CreaTabellaResHTML (pTable);
                    pCatalog->Tables->Append (_variant_t((IDispatch*)pTable, TRUE));
                }
                pTable.CreateInstance(__uuidof(Table));
                if (pTable)   {
                    pTable->PutParentCatalog (pCatalog);
                    CreaTabellaResImages (pTable);
                    pCatalog->Tables->Append (_variant_t((IDispatch*)pTable, TRUE));
                }
                CreateTablesFromXML ((const char *)strXMLFile, pCatalog);
                /*
                pTable.CreateInstance(__uuidof(Table));
                if (pTable)   {
                    pTable->PutParentCatalog (pCatalog);
                    CreaTabellaResTools (pTable);
                    pCatalog->Tables->Append (_variant_t((IDispatch*)pTable, TRUE));
                }
                pTable.CreateInstance(__uuidof(Table));
                if (pTable)   {
                    pTable->PutParentCatalog (pCatalog);
                    CreaTabellaResHeads (pTable);
                    pCatalog->Tables->Append (_variant_t((IDispatch*)pTable, TRUE));
                }
                */
                pConnection->Close ();
            }
            else
                n=-1;
			pCatalog.Release ();
        }
        else 
            n=-2;
    }
    else
        n=-3;
	return n;
}

/*
** ChiudiFileDiRisorse : chiude il file di risorse.
*/
int CResourceFile::ChiudiFileDiRisorse ()
{
    /* chiudiamo recordset e connessione */
    m_pRsCtrlSyn.Close ();
    m_pRsImages.Close ();
    m_pRs.Close();
    m_pDb.Close();
    return 0;
}

/*
** CreaTabellaResValori :
*/
int CResourceFile::CreaTabellaResControlliSinottico (_TablePtr pTable)
{
    _IndexPtr pIndexNomeVariabile;
    _variant_t vAutoIncrement=true;
    _variant_t v;
    int nB=0;
    try   {
        pTable->PutName (RESTABLE_RESCONTROLLISINOTTICO);
        pTable->Columns->Append ("ID", (DataTypeEnumX)3, sizeof (int));
        pTable->Columns->GetItem ("ID")->Properties->GetItem ("Autoincrement")->PutValue (vAutoIncrement);
        /* NomePagina */
        pTable->Columns->Append ("NomePagina", (DataTypeEnumX)202, 255);
	    v="";
        pTable->Columns->GetItem ("NomePagina")->Properties->GetItem ("Default")->PutValue (v);
		/* IDControllo */
        pTable->Columns->Append ("IDControllo", (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem ("IDControllo")->Properties->GetItem ("Default")->PutValue (v);
		/* TipoControllo */
        pTable->Columns->Append ("TipoControllo", (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem ("TipoControllo")->Properties->GetItem ("Default")->PutValue (v);
        /* NomeVariabile */
        pTable->Columns->Append ("NomeVariabile", (DataTypeEnumX)202, 255);
	    v="";
        pTable->Columns->GetItem ("NomeVariabile")->Properties->GetItem ("Default")->PutValue (v);
        /* Riga */
        pTable->Columns->Append ("Riga", (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem ("Riga")->Properties->GetItem ("Default")->PutValue (v);
        /* colonna */
        pTable->Columns->Append ("Colonna", (DataTypeEnumX)202, 32);
	    v="";
        pTable->Columns->GetItem ("Colonna")->Properties->GetItem ("Default")->PutValue (v);
        /* ValoreStringa */
        pTable->Columns->Append ("ValoreStringa", (DataTypeEnumX)202, 32);
	    v="";
        pTable->Columns->GetItem ("ValoreStringa")->Properties->GetItem ("Default")->PutValue (v);
        /* ValoreDouble */
        pTable->Columns->Append ("ValoreDouble", (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem ("ValoreDouble")->Properties->GetItem ("Default")->PutValue (v);
        /* Tipo */
        pTable->Columns->Append ("Tipo", (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem ("Tipo")->Properties->GetItem ("Default")->PutValue (v);
        /* chiave primaria */
        pTable->Keys->Append ("PrimaryKey", adKeyPrimary, "ID", "", "");
        /* creiamo la chiave che indicizza in base al nome della tabella */
        pIndexNomeVariabile.CreateInstance(__uuidof(Index));
        pIndexNomeVariabile->PutName (RESINDEX_RESCONTROLLISINOTTICO);
        pIndexNomeVariabile->Columns->Append ("NomePagina", (DataTypeEnumX)3, sizeof (int));
        pIndexNomeVariabile->Columns->Append ("IDControllo", (DataTypeEnumX)3, sizeof (int));
        pTable->Indexes->Append(_variant_t((IDispatch *)pIndexNomeVariabile));
    }
	catch(_com_error &e)
	{
        nB=-1;
	}
    return nB;
}

/*
** CreaTabellaResHTML :
*/
int CResourceFile::CreaTabellaResHTML (_TablePtr pTable)
{
    _IndexPtr pIndexRisorsa, pIndexTipoRisorsa;
    _variant_t vAutoIncrement=true;
    _variant_t v;
    int nB=0;
    try   {
        pTable->PutName (RESTABLE_RESHTML);
        pTable->Columns->Append ("ID", (DataTypeEnumX)3, sizeof (int));
        pTable->Columns->GetItem ("ID")->Properties->GetItem ("Autoincrement")->PutValue (vAutoIncrement);
        /* NomeForm */
        pTable->Columns->Append ("NomeForm", (DataTypeEnumX)202, 255);
	    v="";
        pTable->Columns->GetItem ("NomeForm")->Properties->GetItem ("Default")->PutValue (v);
        /* Tipo */
        pTable->Columns->Append ("Tipo", (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem ("Tipo")->Properties->GetItem ("Default")->PutValue (v);
        /* Disk/Database */
        pTable->Columns->Append ("SalvaSu", (DataTypeEnumX)11, sizeof (BOOL));
	    v.vt = VT_BOOL;
	    v.boolVal=VARIANT_TRUE;
        pTable->Columns->GetItem ("SalvaSu")->Properties->GetItem ("Default")->PutValue (v);
        /* XML/HTML */
        pTable->Columns->Append ("SalvaCome", (DataTypeEnumX)11, sizeof (BOOL));
	    v.vt = VT_BOOL;
	    v.boolVal=VARIANT_TRUE;
        pTable->Columns->GetItem ("SalvaCome")->Properties->GetItem ("Default")->PutValue (v);
        /* Main Synoptic */
        pTable->Columns->Append ("MainSyn", (DataTypeEnumX)11, sizeof (BOOL));
	    v.vt = VT_BOOL;
	    v.boolVal=VARIANT_FALSE;
        pTable->Columns->GetItem ("MainSyn")->Properties->GetItem ("Default")->PutValue (v);
        /* HTML */
        pTable->Columns->Append ("HTML", (DataTypeEnumX)203, sizeof (int));
        /* chiave primaria */
        pTable->Keys->Append ("PrimaryKey", adKeyPrimary, "ID", "", "");
        /* chiave su NomeRisorsa */
        pIndexRisorsa.CreateInstance(__uuidof(Index));
        pIndexRisorsa->PutName (RESINDEX_RESRISORSA);
        pIndexRisorsa->Columns->Append ("NomeForm", (DataTypeEnumX)3, sizeof (int));
        pTable->Indexes->Append(_variant_t((IDispatch *)pIndexRisorsa));
        /* chiave su Tipo e NomeRisorsa */
        pIndexTipoRisorsa.CreateInstance(__uuidof(Index));
        pIndexTipoRisorsa->PutName (RESINDEX_RESTIPORISORSA);
        pIndexTipoRisorsa->Columns->Append ("Tipo", (DataTypeEnumX)3, sizeof (int));
        pIndexTipoRisorsa->Columns->Append ("NomeForm", (DataTypeEnumX)3, sizeof (int));
        pTable->Indexes->Append(_variant_t((IDispatch *)pIndexTipoRisorsa));
    }
	catch(_com_error &e)
	{
        nB=-1;
	}
    return nB;
}

/*
 * GetMode 
 */
CADODatabase::cadoConnectModeEnum CResourceFile::GetMode (int nMode)
{
    int n;
    
    n = nMode ? CADODatabase::connectModeRead : CADODatabase::connectModeReadWrite;
    n |= CADODatabase::connectModeShareDenyWrite;
    return (CADODatabase::cadoConnectModeEnum) n;
}

/*
** AggiungiRecordVuoto : 
*/
int CResourceFile::AggiungiRecordVuoto (CString str, eResources e)
{
    CString strFormat;
    _variant_t v, v1, v2;
    int n=-1;

    if (m_pRs.IsOpen())   {
        if (SeekName (str, e))   {
            m_pRs.AddNew();
            v=str.AllocSysString ();
            m_pRs.SetFieldValue(FIELD_NOMEFORM, v);
            v1=(long)e;
            m_pRs.SetFieldValue(FIELD_TIPO, v1);
            strFormat.Format ("<?xml version=\"1.0\" encoding=\"utf-8\"?><body>\n<form name=\"position: absolute; left: 0px; top: 0px; width: 200px; height: 100px\" formname=\"%s\">\n</form>\n</body>\n", str);
            v2=strFormat.AllocSysString ();
            m_pRs.SetFieldValue(FIELD_HTML, v2);
			v = (bool) false;
			m_pRs.SetFieldValue(FIELD_MAIN_SYN, v);
            m_pRs.Update();
            m_pRs.m_pRecordset->Requery (-1);
            n=0;
        }
    }        
    return n;
}

/*
** SeekName :
*/
int CResourceFile::SeekName (CString str, eResources e)
{
    COleSafeArray sa;
    _variant_t v, v1;
    long i=0;

    v=(long)e;
    v1=str.AllocSysString ();
    sa.CreateOneDim (VT_VARIANT, 2);
    sa.PutElement (&i, &v);
    i++;
    sa.PutElement (&i, &v1);
    m_pRs.m_pRecordset->PutIndex (RESINDEX_RESTIPORISORSA);
    m_pRs.m_pRecordset->Seek (sa, adSeekFirstEQ);
    return m_pRs.IsEof();
}

/*
** CancellaRecordVuoto :
*/
int CResourceFile::CancellaRecordVuoto (CString str, eResources e)
{
    int n=-1;
    if (m_pRs.IsOpen())   {
        if (!SeekName (str, e))   {
            m_pRs.Delete ();    
            m_pRs.Update ();
            n=0;
        }
        else
            m_pRs.m_pRecordset->Requery (-1);
    }
    return n;
}

/*
** GetFirstRSRecord :
*/
int CResourceFile::GetFirstRSRecord (CString& str, eResources& e)
{
    _variant_t v;
    int n=-1, t;

    if (m_pRs.IsOpen()&&m_pRs.GetRecordCount ())   {
        m_pRs.MoveFirst ();
        m_pRs.GetFieldValue (FIELD_NOMEFORM, v);
        str=v.bstrVal;
        m_pRs.GetFieldValue (FIELD_TIPO, t);
        e=(eResources)t;
        n=0;
    }
    return n;
}

/*
** GetFirstRSRecord :
*/
int CResourceFile::GetNextRSRecord (CString& str, eResources& e)
{
    _variant_t v;
    int n=-1, t;

    if (m_pRs.IsOpen())   {
        m_pRs.MoveNext ();
        if (!m_pRs.IsEof())   {
            m_pRs.GetFieldValue (FIELD_NOMEFORM, v);
            str=v.bstrVal;
            m_pRs.GetFieldValue (FIELD_TIPO, t);
            e=(eResources)t;
            n=0;
        }
    }
    return n;
}

/*
** RinominaRecord :
*/
int CResourceFile::RinominaRecord (CString strPrev, CString strNew, eResources e)
{
	CString strHTML;
    _variant_t v, v1, v2;
    int n=-1;

    if (m_pRs.IsOpen())   {
        if (SeekName (strNew, e))   {
            m_pRs.m_pRecordset->Requery (-1);
            if (!SeekName (strPrev, e))   {
                m_pRs.GetFieldValue (FIELD_HTML, v2);
                m_pRs.Delete ();
                m_pRs.Update ();
                m_pRs.AddNew ();
                v=strNew.AllocSysString ();
                m_pRs.SetFieldValue(FIELD_NOMEFORM, v);
                v1=(long)e;
                m_pRs.SetFieldValue(FIELD_TIPO, v1);
				ReplaceFormName (v2, strHTML, strNew);
				v2 = strHTML.AllocSysString ();
                m_pRs.SetFieldValue(FIELD_HTML, v2);
                m_pRs.Update();
                n=0;
            }
            else
                m_pRs.m_pRecordset->Requery (-1);
        }
    }
    return n;
}

/*
** GetHTML : 
*/
int CResourceFile::GetXML (CString str, eResources e, CString& strXML, bool &bIsMainSyn)
{
    _variant_t v, v1, v2, v3;
    int n=-1;

    if (m_pRs.IsOpen())   {
        if (!SeekName (str, e))   {
            m_pRs.GetFieldValue (FIELD_HTML,       v);
            m_pRs.GetFieldValue (FIELD_SALVA_SU,   v1);
            m_pRs.GetFieldValue (FIELD_SALVA_COME, v2);
            m_pRs.GetFieldValue (FIELD_MAIN_SYN,   v3);
            strXML=v.bstrVal;
			bIsMainSyn = v3.boolVal;
            n=0;
        }
        else
            m_pRs.m_pRecordset->Requery (-1);
    }
    return n;
}

/*
** ModificaRecord :
*/
int CResourceFile::ModificaRecord (CString str, eResources e, CString strXML, bool bIsMainSyn)
{
    _variant_t v, v1, v2, v3, v4, v5;
    int n=-1;

    if (m_pRs.IsOpen())   {
        if (!SeekName (str, e))   {
            m_pRs.Delete ();
            m_pRs.Update ();
            m_pRs.AddNew ();
            v=str.AllocSysString ();
            m_pRs.SetFieldValue(FIELD_NOMEFORM, v);
            v1=(long)e;
            m_pRs.SetFieldValue(FIELD_TIPO, v1);
            v2=strXML.AllocSysString ();
            m_pRs.SetFieldValue(FIELD_HTML, v2);
            v3=(bool) VARIANT_TRUE;
            m_pRs.SetFieldValue(FIELD_SALVA_SU, v3);
            v4=(bool) VARIANT_FALSE;
            m_pRs.SetFieldValue(FIELD_SALVA_COME, v4);
            v5=bIsMainSyn;
            m_pRs.SetFieldValue(FIELD_MAIN_SYN, v5);
            m_pRs.Update();
            n=0;
        }
        else
            m_pRs.m_pRecordset->Requery (-1);
    }
    return n;
}

/*
** CreaTabellaResImages :
*/
int CResourceFile::CreaTabellaResImages (_TablePtr pTable)
{
    _IndexPtr pIndexNomeImmagine;
    _variant_t vAutoIncrement=true;
    _variant_t v;
    int nB=0;
    try   {
        pTable->PutName (RESTABLE_RESIMAGES);
        pTable->Columns->Append (FIELD_IMAGES_ID, (DataTypeEnumX)3, sizeof (int));
        pTable->Columns->GetItem (FIELD_IMAGES_ID)->Properties->GetItem ("Autoincrement")->PutValue (vAutoIncrement);
        /* NomeVariabile */
        pTable->Columns->Append (FIELD_IMAGES_IDIMAGES, (DataTypeEnumX)202, 33);
	    v="";
        pTable->Columns->GetItem (FIELD_IMAGES_IDIMAGES)->Properties->GetItem ("Default")->PutValue (v);
        /* Tipo di file 0 JPG */
        pTable->Columns->Append (FIELD_IMAGES_TYPE, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_IMAGES_TYPE)->Properties->GetItem ("Default")->PutValue (v);
        /* numero di bytes memorizzati nel chunk */
        pTable->Columns->Append (FIELD_IMAGES_LENGTH, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_IMAGES_LENGTH)->Properties->GetItem ("Default")->PutValue (v);
        /* buffer dei dati binari del file */
        pTable->Columns->Append (FIELD_IMAGES_FILEDATA, (DataTypeEnumX)205, sizeof (int));
        /* chiave primaria */
        pTable->Keys->Append ("PrimaryKey", adKeyPrimary, FIELD_IMAGES_ID, "", "");
        /* creiamo la chiave che indicizza in base alL' id dell'immagine nome della tabella */
        pIndexNomeImmagine.CreateInstance(__uuidof(Index));
        pIndexNomeImmagine->PutName (RESINDEX_RESIMAGES);
        pIndexNomeImmagine->Columns->Append (FIELD_IMAGES_IDIMAGES, (DataTypeEnumX)202, 33);
        pIndexNomeImmagine->Columns->Append (FIELD_IMAGES_ID, (DataTypeEnumX)3, sizeof (int));
        pTable->Indexes->Append(_variant_t((IDispatch *)pIndexNomeImmagine));
    }
	catch(_com_error &e)
	{
        nB=-1;
	}
    return nB;
}

/*
** CreaTabellaResTools :
*/
int CResourceFile::CreaTabellaResTools (_TablePtr pTable)
{
    _IndexPtr pIndexNomeUtensile;
    _IndexPtr pIndexNumeroUtensile;
    _IndexPtr pIndexTipoUtensile;
    _variant_t vAutoIncrement=true;
    _variant_t v;
    int nB=0;
    try   {
        pTable->PutName (RESTABLE_RESTOOLS);
        /* ID */
        pTable->Columns->Append (FIELD_TOOLS_ID, (DataTypeEnumX)3, sizeof (int));
        pTable->Columns->GetItem (FIELD_TOOLS_ID)->Properties->GetItem ("Autoincrement")->PutValue (vAutoIncrement);
        /* NomeUtensile */
        pTable->Columns->Append (FIELD_TOOLS_NOMEUTENSILE, (DataTypeEnumX)202, 33);
	    v="";
        pTable->Columns->GetItem (FIELD_TOOLS_NOMEUTENSILE)->Properties->GetItem ("Default")->PutValue (v);
        /* Descrizione */
        pTable->Columns->Append (FIELD_TOOLS_DESCRIZIONE, (DataTypeEnumX)202, 64);
	    v="";
        pTable->Columns->GetItem (FIELD_TOOLS_DESCRIZIONE)->Properties->GetItem ("Default")->PutValue (v);
        /* Numero Utensile */
        pTable->Columns->Append (FIELD_TOOLS_NUMEROUTENSILE, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_TOOLS_NUMEROUTENSILE)->Properties->GetItem ("Default")->PutValue (v);
        /* Tipo Utensile */
        pTable->Columns->Append (FIELD_TOOLS_TIPOUTENSILE, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_TOOLS_TIPOUTENSILE)->Properties->GetItem ("Default")->PutValue (v);
        /* DiametroEdge1 */
        pTable->Columns->Append (FIELD_TOOLS_DIAMETROEDGE1, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_DIAMETROEDGE1)->Properties->GetItem ("Default")->PutValue (v);
        /* DiametroEdge2 */
        pTable->Columns->Append (FIELD_TOOLS_DIAMETROEDGE2, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_DIAMETROEDGE2)->Properties->GetItem ("Default")->PutValue (v);
        /* DiametroEdge3 */
        pTable->Columns->Append (FIELD_TOOLS_DIAMETROEDGE3, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_DIAMETROEDGE3)->Properties->GetItem ("Default")->PutValue (v);
        /* LunghezzaEdge1 */
        pTable->Columns->Append (FIELD_TOOLS_LUNGHEZZAEDGE1, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_LUNGHEZZAEDGE1)->Properties->GetItem ("Default")->PutValue (v);
        /* LunghezzaEdge2 */
        pTable->Columns->Append (FIELD_TOOLS_LUNGHEZZAEDGE2, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_LUNGHEZZAEDGE2)->Properties->GetItem ("Default")->PutValue (v);
        /* LunghezzaEdge3 */
        pTable->Columns->Append (FIELD_TOOLS_LUNGHEZZAEDGE3, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_LUNGHEZZAEDGE3)->Properties->GetItem ("Default")->PutValue (v);
        /* CompensazioneDiametro1 */
        pTable->Columns->Append (FIELD_TOOLS_COMPENSAZIONEDIAMETRO1, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_COMPENSAZIONEDIAMETRO1)->Properties->GetItem ("Default")->PutValue (v);
        /* CompensazioneDiametro2 */
        pTable->Columns->Append (FIELD_TOOLS_COMPENSAZIONEDIAMETRO2, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_COMPENSAZIONEDIAMETRO2)->Properties->GetItem ("Default")->PutValue (v);
        /* CompensazioneDiametro3 */
        pTable->Columns->Append (FIELD_TOOLS_COMPENSAZIONEDIAMETRO3, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_COMPENSAZIONEDIAMETRO3)->Properties->GetItem ("Default")->PutValue (v);
        /* CompensazioneLunghezza1 */
        pTable->Columns->Append (FIELD_TOOLS_COMPENSAZIONELUNGHEZZA1, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_COMPENSAZIONELUNGHEZZA1)->Properties->GetItem ("Default")->PutValue (v);
        /* CompensazioneLunghezza2 */
        pTable->Columns->Append (FIELD_TOOLS_COMPENSAZIONELUNGHEZZA2, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_COMPENSAZIONELUNGHEZZA2)->Properties->GetItem ("Default")->PutValue (v);
        /* CompensazioneLunghezza3 */
        pTable->Columns->Append (FIELD_TOOLS_COMPENSAZIONELUNGHEZZA3, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_COMPENSAZIONELUNGHEZZA3)->Properties->GetItem ("Default")->PutValue (v);
        /* ScaricoPassata */
        pTable->Columns->Append (FIELD_TOOLS_SCARICOPASSATA, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_SCARICOPASSATA)->Properties->GetItem ("Default")->PutValue (v);
        /* Passo */
        pTable->Columns->Append (FIELD_TOOLS_PASSO, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_PASSO)->Properties->GetItem ("Default")->PutValue (v);
        /* ConettoUtensile */
        pTable->Columns->Append (FIELD_TOOLS_CONETTOUTENSILE, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_CONETTOUTENSILE)->Properties->GetItem ("Default")->PutValue (v);
        /* AltezzaDisco */
        pTable->Columns->Append (FIELD_TOOLS_ALTEZZADISCO, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_TOOLS_ALTEZZADISCO)->Properties->GetItem ("Default")->PutValue (v);
        /* RotazioneDestra */
        pTable->Columns->Append (FIELD_TOOLS_ROTAZIONEDESTRA, (DataTypeEnumX)11, sizeof (BOOL));
	    v.vt = VT_BOOL;
	    v.boolVal=VARIANT_FALSE;
        pTable->Columns->GetItem (FIELD_TOOLS_ROTAZIONEDESTRA)->Properties->GetItem ("Default")->PutValue (v);
        /* AngoloSvasatura */
        pTable->Columns->Append (FIELD_TOOLS_ANGOLOSVASATURA, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal=0;
        pTable->Columns->GetItem (FIELD_TOOLS_ANGOLOSVASATURA)->Properties->GetItem ("Default")->PutValue (v);
        /* Accelerazione */
        pTable->Columns->Append (FIELD_TOOLS_ACCELERAZIONE, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal=0;
        pTable->Columns->GetItem (FIELD_TOOLS_ACCELERAZIONE)->Properties->GetItem ("Default")->PutValue (v);
        /* Decelerazione */
        pTable->Columns->Append (FIELD_TOOLS_DECELERAZIONE, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal=0;
        pTable->Columns->GetItem (FIELD_TOOLS_DECELERAZIONE)->Properties->GetItem ("Default")->PutValue (v);
        /* VelocitaMinimaRotazione */
        pTable->Columns->Append (FIELD_TOOLS_VELMINROT, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal=0;
        pTable->Columns->GetItem (FIELD_TOOLS_VELMINROT)->Properties->GetItem ("Default")->PutValue (v);
        /* VelocitaMassimaRotazione */
        pTable->Columns->Append (FIELD_TOOLS_VELMAXROT, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal=0;
        pTable->Columns->GetItem (FIELD_TOOLS_VELMAXROT)->Properties->GetItem ("Default")->PutValue (v);
        /* VelocitaLavoro */
        pTable->Columns->Append (FIELD_TOOLS_VELLAVORO, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal=0;
        pTable->Columns->GetItem (FIELD_TOOLS_VELLAVORO)->Properties->GetItem ("Default")->PutValue (v);
        /* VelocitaIngresso */
        pTable->Columns->Append (FIELD_TOOLS_VELINGRESSO, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal=0;
        pTable->Columns->GetItem (FIELD_TOOLS_VELINGRESSO)->Properties->GetItem ("Default")->PutValue (v);
        /* VelocitaUscita */
        pTable->Columns->Append (FIELD_TOOLS_VELUSCITA, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal=0;
        pTable->Columns->GetItem (FIELD_TOOLS_VELUSCITA)->Properties->GetItem ("Default")->PutValue (v);
        /* RotazioneLavoro */
        pTable->Columns->Append (FIELD_TOOLS_ROTAZIONELAVORO, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal=0;
        pTable->Columns->GetItem (FIELD_TOOLS_ROTAZIONELAVORO)->Properties->GetItem ("Default")->PutValue (v);
        /* Lubrificazione */
        pTable->Columns->Append (FIELD_TOOLS_LUBRIFICAZIONE, (DataTypeEnumX)11, sizeof (BOOL));
	    v.vt = VT_BOOL;
	    v.boolVal=VARIANT_FALSE;
        pTable->Columns->GetItem (FIELD_TOOLS_LUBRIFICAZIONE)->Properties->GetItem ("Default")->PutValue (v);
        /* Impulsi */
        pTable->Columns->Append (FIELD_TOOLS_IMPULSI, (DataTypeEnumX)11, sizeof (BOOL));
	    v.vt = VT_BOOL;
	    v.boolVal=VARIANT_FALSE;
        pTable->Columns->GetItem (FIELD_TOOLS_IMPULSI)->Properties->GetItem ("Default")->PutValue (v);
        /* IntervalloImpulsi */
        pTable->Columns->Append (FIELD_TOOLS_INTERVALLOIMPULSI, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_TOOLS_INTERVALLOIMPULSI)->Properties->GetItem ("Default")->PutValue (v);
        /* IntervalloLubrificazione */
        pTable->Columns->Append (FIELD_TOOLS_INTERVALLOLUBRIFICAZIONE, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_TOOLS_INTERVALLOLUBRIFICAZIONE)->Properties->GetItem ("Default")->PutValue (v);
        /* OreDiLavorazione */
        pTable->Columns->Append (FIELD_TOOLS_OREDILAVORAZIONE, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_TOOLS_OREDILAVORAZIONE)->Properties->GetItem ("Default")->PutValue (v);
        /* Bitmap */
        pTable->Columns->Append (FIELD_TOOLS_BITMAP, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_TOOLS_BITMAP)->Properties->GetItem ("Default")->PutValue (v);
        /* chiave primaria */
        pTable->Keys->Append ("PrimaryKey", adKeyPrimary, FIELD_TOOLS_ID, "", "");
        /* creiamo la chiave che indicizza in base alL' id dell'immagine nome della tabella */
        pIndexNomeUtensile.CreateInstance(__uuidof(Index));
        pIndexNomeUtensile->PutName (RESINDEX_RESTOOLNAME);
        pIndexNomeUtensile->Columns->Append (FIELD_TOOLS_NOMEUTENSILE, (DataTypeEnumX)202, 33);
        pTable->Indexes->Append(_variant_t((IDispatch *)pIndexNomeUtensile));

        pIndexNumeroUtensile.CreateInstance(__uuidof(Index));
        pIndexNumeroUtensile->PutName (RESINDEX_RESTOOLNUMBER);
        pIndexNumeroUtensile->Columns->Append (FIELD_TOOLS_NUMEROUTENSILE, (DataTypeEnumX)3, sizeof (int));
        pTable->Indexes->Append(_variant_t((IDispatch *)pIndexNumeroUtensile));

        pIndexTipoUtensile.CreateInstance(__uuidof(Index));
        pIndexTipoUtensile->PutName (RESINDEX_RESTOOLTYPE);
        pIndexTipoUtensile->Columns->Append (FIELD_TOOLS_TIPOUTENSILE, (DataTypeEnumX)3, sizeof (int));
        pIndexTipoUtensile->Columns->Append (FIELD_TOOLS_NUMEROUTENSILE, (DataTypeEnumX)3, sizeof (int));
        pTable->Indexes->Append(_variant_t((IDispatch *)pIndexTipoUtensile));
    }
	catch(_com_error &e)
	{
        nB=-1;
	}
    return nB;
}

/*
** CreaTabellaResHeads :
*/
int CResourceFile::CreaTabellaResHeads (_TablePtr pTable)
{
    _IndexPtr pIndexNomeTestina;
    _variant_t vAutoIncrement=true;
    _variant_t v;
    int nB=0;
    try   {
        pTable->PutName (RESTABLE_RESHEADS);
        /* ID */
        pTable->Columns->Append (FIELD_HEADS_ID, (DataTypeEnumX)3, sizeof (int));
        pTable->Columns->GetItem (FIELD_HEADS_ID)->Properties->GetItem ("Autoincrement")->PutValue (vAutoIncrement);
        /* NomeTestina */
        pTable->Columns->Append (FIELD_HEADS_NOMETESTINA, (DataTypeEnumX)202, 33);
	    v="";
        pTable->Columns->GetItem (FIELD_HEADS_NOMETESTINA)->Properties->GetItem ("Default")->PutValue (v);
        /* Descrizione */
        pTable->Columns->Append (FIELD_HEADS_DESCRIZIONE, (DataTypeEnumX)202, 64);
	    v="";
        pTable->Columns->GetItem (FIELD_HEADS_DESCRIZIONE)->Properties->GetItem ("Default")->PutValue (v);
        /* NumeroCono */
        pTable->Columns->Append (FIELD_HEADS_NUMEROCONO, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_HEADS_NUMEROCONO)->Properties->GetItem ("Default")->PutValue (v);
        /* OffsetX */
        pTable->Columns->Append (FIELD_HEADS_OFFSETX, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_HEADS_OFFSETX)->Properties->GetItem ("Default")->PutValue (v);
        /* OffsetY */
        pTable->Columns->Append (FIELD_HEADS_OFFSETY, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_HEADS_OFFSETY)->Properties->GetItem ("Default")->PutValue (v);
        /* OffsetZ */
        pTable->Columns->Append (FIELD_HEADS_OFFSETZ, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_HEADS_OFFSETZ)->Properties->GetItem ("Default")->PutValue (v);
        /* RotazioneX */
        pTable->Columns->Append (FIELD_HEADS_ROTAZIONEX, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_HEADS_ROTAZIONEX)->Properties->GetItem ("Default")->PutValue (v);
        /* RotazioneY */
        pTable->Columns->Append (FIELD_HEADS_ROTAZIONEY, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_HEADS_ROTAZIONEY)->Properties->GetItem ("Default")->PutValue (v);
        /* RotazioneZ */
        pTable->Columns->Append (FIELD_HEADS_ROTAZIONEZ, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_HEADS_ROTAZIONEZ)->Properties->GetItem ("Default")->PutValue (v);
        /* Braccio */
        pTable->Columns->Append (FIELD_HEADS_BRACCIO, (DataTypeEnumX)5, sizeof (double));
	    v.vt = VT_R8;
	    v.dblVal = 0.0;
        pTable->Columns->GetItem (FIELD_HEADS_BRACCIO)->Properties->GetItem ("Default")->PutValue (v);
        /* Braccio */
        pTable->Columns->Append (FIELD_HEADS_TOOL, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_HEADS_TOOL)->Properties->GetItem ("Default")->PutValue (v);
        /* Bitmap */
        pTable->Columns->Append (FIELD_TOOLS_BITMAP, (DataTypeEnumX)3, sizeof (int));
	    v.vt = VT_I4;
	    v.lVal = 0;
        pTable->Columns->GetItem (FIELD_TOOLS_BITMAP)->Properties->GetItem ("Default")->PutValue (v);
        /* chiave primaria */
        pTable->Keys->Append ("PrimaryKey", adKeyPrimary, FIELD_HEADS_ID, "", "");
        /* creiamo la chiave che indicizza in base alL' id dell'immagine nome della tabella */
        pIndexNomeTestina.CreateInstance(__uuidof(Index));
        pIndexNomeTestina->PutName (RESINDEX_RESHEADNAME);
        pIndexNomeTestina->Columns->Append (FIELD_HEADS_NOMETESTINA, (DataTypeEnumX)202, 33);
        pIndexNomeTestina->Columns->Append (FIELD_HEADS_NUMEROCONO, (DataTypeEnumX)3, sizeof (int));
        pTable->Indexes->Append(_variant_t((IDispatch *)pIndexNomeTestina));
    }
	catch(_com_error &e)
	{
        nB=-1;
	}
    return nB;
}

/*
** SeekImmagine :
*/
int CResourceFile::SeekImmagine (CString str)
{
    COleSafeArray sa;
    CString strField;
    _variant_t v, v1;
    long i=0;
    int nB=-1;

    try   {
        v=str.AllocSysString ();
        sa.CreateOneDim (VT_VARIANT, 2);
        sa.PutElement (&i, &v);
        i++;
        v1=(long)0;
        sa.PutElement (&i, &v1);
        m_pRsImages.m_pRecordset->PutIndex (RESINDEX_RESIMAGES);
        m_pRsImages.m_pRecordset->Seek (sa, adSeekAfterEQ);
        if (!m_pRsImages.IsEof())   {
            m_pRsImages.GetFieldValue (FIELD_IMAGES_IDIMAGES, strField);
            if (str==strField)
                nB=0;
        }
    }
    catch (_com_error &e)   {
    }
    return nB;
}

/*
** EsisteImmagine :
*/
int CResourceFile::EsisteImmagine (CString strID)
{
    int nB=0;

    if (m_pRsImages.IsOpen())   {
        if (!SeekImmagine (strID))
            nB=1;
        else
            m_pRsImages.m_pRecordset->Requery (-1);
    }
    return nB;
}

/*
** AggiungiImmagine :
*/
int CResourceFile::AggiungiImmagine (CString strID, char* p, DWORD nLength)
{
    _variant_t v, v1, v2;
    int n=-1;

    if (m_pRsImages.IsOpen())   {
        if (SeekImmagine (strID))   {
            m_pRsImages.AddNew();
            v=strID.AllocSysString ();
            m_pRsImages.SetFieldValue(FIELD_IMAGES_IDIMAGES, v);
            v1=(long)0;
            m_pRsImages.SetFieldValue(FIELD_IMAGES_TYPE, v1);
            v2=(long)nLength;
            m_pRsImages.SetFieldValue(FIELD_IMAGES_LENGTH, v2);
	        m_pRsImages.AppendChunk (FIELD_IMAGES_FILEDATA, p, nLength);
            m_pRsImages.Update();
            m_pRsImages.m_pRecordset->Requery (-1);
            n=0;
        }
    }
    return n;
}

/*
** CancellaImmagine :
*/
int CResourceFile::CancellaImmagine (CString strID)
{
    int n=-1;
    if (m_pRsImages.IsOpen())   {
        if (!SeekImmagine (strID))   {
            m_pRsImages.Delete ();    
            m_pRsImages.Update ();
            n=0;
        }
        else
            m_pRsImages.m_pRecordset->Requery (-1);
    }
    return n;
}

/*
** LunghezzaImmagine
*/
DWORD CResourceFile::LunghezzaImmagine (CString strID)
{
    DWORD dw=0;
    long l;

    if (m_pRsImages.IsOpen())   {
        if (!SeekImmagine (strID))   {
            m_pRsImages.GetFieldValue (FIELD_IMAGES_LENGTH, l);
            dw=(DWORD)l;
        }    
        else
            m_pRsImages.m_pRecordset->Requery (-1);
    }
    return dw;
}

/*
** CaricaImmagine :
*/
int CResourceFile::CaricaImmagine (CString strID, char* p)
{
    int n=-1;

    if (m_pRsImages.IsOpen())   {
        if (!SeekImmagine (strID))   {
            m_pRsImages.GetChunk (FIELD_IMAGES_FILEDATA, p);
            n=0;
        }    
        else
            m_pRsImages.m_pRecordset->Requery (-1);
    }
    return n;
}

/*
** GetFirstRSImage :
*/
int CResourceFile::GetFirstRSImage (CString& str)
{
    _variant_t v;
    int n=-1;

    if (m_pRsImages.IsOpen()&&m_pRsImages.GetRecordCount ())   {
        m_pRsImages.MoveFirst ();
        m_pRsImages.GetFieldValue (FIELD_IMAGES_IDIMAGES, v);
        str=v.bstrVal;
        n=0;
    }
    return n;
}

/*
** GetNextRSImage :
*/
int CResourceFile::GetNextRSImage (CString& str)
{
    _variant_t v;
    int n=-1;

    if (m_pRsImages.IsOpen())   {
        m_pRsImages.MoveNext ();
        if (!m_pRsImages.IsEof())   {
            m_pRsImages.GetFieldValue (FIELD_IMAGES_IDIMAGES, v);
            str=v.bstrVal;
            n=0;
        }
    }
    return n;
}

/*
** RinominaImmagine :
*/
int CResourceFile::RinominaImmagine (CString strImage, CString strID)
{
    int n=-1;

    if (m_pRsImages.IsOpen())   {
        if (!SeekImmagine (strImage))   {
            m_pRsImages.Edit ();
            m_pRsImages.SetFieldValue (FIELD_IMAGES_IDIMAGES, strID);
            m_pRsImages.Update ();
            n=0;
        }    
        else
            m_pRsImages.m_pRecordset->Requery (-1);
    }
    return n;
}

/*
** GetTableList :
*/
int CResourceFile::GetTableList (CStringList& slTables)
{
    _ConnectionPtr pConnection=NULL;
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    CString str, strTableName, strType;
    int nB=-1;

    try   {
    pConnection.CreateInstance(__uuidof(Connection));
    if (pConnection)   {
        pCatalog.CreateInstance(__uuidof(Catalog));
        if (pCatalog)   {
            str.Format ("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", m_strNomeFile);
            pConnection->PutMode((enum ConnectModeEnum)(CADODatabase::connectModeReadWrite|CADODatabase::connectModeShareDenyWrite));
	        if (pConnection->Open (_bstr_t (str), "", "", NULL)==S_OK)   {
                pCatalog->put_ActiveConnection (_variant_t((IDispatch*)pConnection, TRUE));
                long nItem, nTableCount=pCatalog->Tables->GetCount();
                while (nTableCount)   {
                    nItem=nTableCount-1;
                    strType=(LPCTSTR)pCatalog->Tables->GetItem(nItem)->GetType ();
                    if (strType!=SYSTEM_TABLE&&strType!=ACCESS_TABLE)   {
                        strTableName = (LPCTSTR)pCatalog->Tables->GetItem(nItem)->GetName();
                        if (strTableName!=RESTABLE_RESHTML&&strTableName!=RESTABLE_RESCONTROLLISINOTTICO&&strTableName!=RESTABLE_RESIMAGES)
                            slTables.AddHead (strTableName);
                    }
                    nTableCount--;
                }
                pConnection->Close (); nB=0;
            }
        }
    }
    }
	catch(_com_error &e)
	{
        nB=-1;
	}
    return nB;
}

/*
** GetFieldsList :
*/
int CResourceFile::GetFieldsList (CString strTable, CStringList& slFields)
{
    _ConnectionPtr pConnection=NULL;
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    _ColumnPtr  pColumn = NULL;
    CString str, strFieldName;
    int nB=-1;

    pConnection.CreateInstance(__uuidof(Connection));
    if (pConnection)   {
        pCatalog.CreateInstance(__uuidof(Catalog));
        if (pCatalog)   {
            str.Format ("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", m_strNomeFile);
            pConnection->PutMode((enum ConnectModeEnum)(CADODatabase::connectModeReadWrite|CADODatabase::connectModeShareDenyWrite));
	        if (pConnection->Open (_bstr_t (str), "", "", NULL)==S_OK)   {
                pCatalog->put_ActiveConnection (_variant_t((IDispatch*)pConnection, TRUE));
                pTable=pCatalog->Tables->GetItem((const char*)strTable);
                long nItem, nFieldsCount=pTable->Columns->Count;
                while (nFieldsCount)   {
                    nItem=nFieldsCount-1;
                    pColumn=pTable->Columns->GetItem (nItem);
                    slFields.AddHead (pColumn->GetName());
                    nFieldsCount--;
                }
                pConnection->Close (); nB=0;
            }
        }
    }
    return nB;
}

/*
** GetIndexList :
*/
int CResourceFile::GetIndexList (CString strTable, CStringList& slIndex)
{
    _ConnectionPtr pConnection=NULL;
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    _IndexPtr   pIndex=NULL;
    CString str, strFieldName;
    int nB=-1;

    pConnection.CreateInstance(__uuidof(Connection));
    if (pConnection)   {
        pCatalog.CreateInstance(__uuidof(Catalog));
        if (pCatalog)   {
            str.Format ("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", m_strNomeFile);
            pConnection->PutMode((enum ConnectModeEnum)(CADODatabase::connectModeReadWrite|CADODatabase::connectModeShareDenyWrite));
	        if (pConnection->Open (_bstr_t (str), "", "", NULL)==S_OK)   {
                pCatalog->put_ActiveConnection (_variant_t((IDispatch*)pConnection, TRUE));
                pTable=pCatalog->Tables->GetItem((const char*)strTable);
                long nItem, nIndexCount=pTable->Indexes->Count;
                while (nIndexCount)   {
                    nItem=nIndexCount-1;
                    pIndex=pTable->Indexes->GetItem (nItem);
                    slIndex.AddHead (pIndex->GetName());
                    nIndexCount--;
                }
                pConnection->Close (); nB=0;
            }
        }
    }
    return nB;
}

/*
** CancellaValore :
*/
int CResourceFile::CancellaValore (CString strName)
{
    CString strField;
    int nB=0;

    if (m_pRsCtrlSyn.IsOpen())   {
        if (!SeekValore (strName))   {
            do   {
                m_pRsCtrlSyn.Delete ();
                m_pRsCtrlSyn.MoveNext ();
                if (m_pRsCtrlSyn.IsEOF())
                    break;
                m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI, strField);
            }   while (strField==strName);
            m_pRsCtrlSyn.Update ();
            nB=1;
        }
        else
            m_pRsImages.m_pRecordset->Requery (-1);
    }
    return nB;
}

/*
** SeekValore :
*/
int CResourceFile::SeekValore (CString str)
{
    COleSafeArray sa;
    CString strField;
    _variant_t v;
    int nB=-1;
    long i;

    try   {
        v=str.AllocSysString ();
        i=0;
        sa.CreateOneDim (VT_VARIANT, 1);
        sa.PutElement (&i, &v);
        //m_pRsCtrlSyn.m_pRecordset->PutIndex (RESINDEX_RESVALORI);
        m_pRsCtrlSyn.m_pRecordset->Seek (sa, adSeekFirstEQ);
        if (!m_pRsCtrlSyn.IsEof())   {
            m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI, strField);
            if (str==strField)
                nB=0;
        }
    }
    catch (_com_error &e)   {
    }
    return nB;
}

/*
** SincronizzaVariabili :
*/
int CResourceFile::SincronizzaVariabili ()
{
    CMapStringToString mss;
    CString strHTML, strMap, strVariable, strColumn;
    int nRow;
    int n=-1;

    /* Componiamo la mappa delle variabili */
    if (m_pRs.IsOpen()&&m_pRs.GetRecordCount ())   {
        m_pRs.MoveFirst ();
        do   {
            m_pRs.GetFieldValue (FIELD_HTML, strHTML);
            AddToMap (strHTML, mss);
            m_pRs.MoveNext ();
        }   while (!m_pRs.IsEof());
    }
    /* ora scorriamo la lista dei record dei valori, se non troviamo un match seghiamo il record */
    if (m_pRsCtrlSyn.IsOpen()&&m_pRsCtrlSyn.GetRecordCount ())   {
        m_pRsCtrlSyn.MoveFirst ();
        do   {
            m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI, strVariable);
            m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI_RIGA, nRow);
            m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI_COLONNA, strColumn);
            strMap.Format ("%s %d %s", (const char*)strVariable, (int)nRow, (const char*)strColumn);
            if (!mss.Lookup (strMap, strVariable))   {
                /* non l'ha trovata, cancelliamo il record */
                m_pRsCtrlSyn.Delete ();    
                m_pRsCtrlSyn.Update ();
            }
            m_pRsCtrlSyn.MoveNext ();
        }   while (!m_pRsCtrlSyn.IsEof());
    }
    mss.RemoveAll ();
    return 0;
}

/*
** AddToMap :
*/
int CResourceFile::AddToMap (CString& strHTML, CMapStringToString& mss)
{
    CString strVariable, strRow, strColumn, strMap;
    int n=0, nIni, nFin;

    while ((n=strHTML.Find ("VariableName=", n))!=-1)   {
        nIni=n+strlen ("VariableName=")+1;
        nFin=strHTML.Find ("\"", nIni);
        strVariable=strHTML.Mid (nIni, nFin-nIni);
        nIni=strHTML.Find ("VariableIndex=\"row: ", nFin+1);
        nIni=nIni+strlen ("VariableIndex=\"row: ");
        nFin=strHTML.Find (";", nIni+1);
        strRow=strHTML.Mid (nIni, nFin-nIni);
        nIni=strHTML.Find ("column: ", nFin+1);
        nIni+=strlen ("column: ");
        nFin=strHTML.Find (";", nIni+1);
        n=nFin+1;
        strColumn=strHTML.Mid (nIni, nFin-nIni);
        strMap.Format ("%s %s %s", strVariable, strRow, strColumn);
        mss.SetAt (strMap, strMap);
    }
    return 0;
}

/*
** AggiungiVariabile :
*/
int CResourceFile::AggiungiVariabile (CString strName, CString strColonna, int nRiga, CString strValore, int nTipo)
{
    _variant_t v, v1, v2, v3, v4;
    double fp;
    long l;
    int n=-1;
    char *p;

    if (m_pRsCtrlSyn.IsOpen())   {
        if (SeekValore (strName))   {
            m_pRsCtrlSyn.AddNew ();
            v=strName.AllocSysString ();
            m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI, v);
            v1=(long)nTipo;
            m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_TIPO, v1);
            v2=strColonna.AllocSysString ();
            m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_COLONNA, v2);
            v3=(long)nRiga;
            m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_RIGA, v3);
            switch (nTipo)   {
                case LUA_TNUMBER :
                    l=strtol (strValore, &p, 0);
                    if (*p!='\0')   
                        l=0;
                    v4=(bool) (l!=0);
                    m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_NUMERO, v4);
                    break;
                case LUA_TBOOLEAN :
                    fp=strtod (strValore, &p);
                    if (*p!='\0')   
                        fp=0.0;
                    v4=(double)fp;
                    m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_NUMERO, v4);
                    break;
                case LUA_TSTRING :
                    v4=strValore.AllocSysString ();
                    m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_STRINGA, v4);
                    break;
            }
            m_pRsCtrlSyn.Update ();
            m_pRsCtrlSyn.m_pRecordset->Requery (-1);
            n=0;
        }
    }
    return n;
}

/*
** CancellaVariabile :
*/
int CResourceFile::CancellaVariabile (CString strName)
{
    int n=-1;
    if (m_pRsCtrlSyn.IsOpen())   {
        if (!SeekValore (strName))   {
            m_pRsCtrlSyn.Delete ();    
            m_pRsCtrlSyn.Update ();
            n=0;
        }
        else
            m_pRsCtrlSyn.m_pRecordset->Requery (-1);
    }
    return n;
}

/*
** ModificaVariabile :
*/
int CResourceFile::ModificaVariabile (CString strName, CString strColonna, int nRiga, CString strValore, int nTipo)
{
    _variant_t v, v1, v2, v3, v4;
    double fp;
    int n=-1;
    char *p;

    if (m_pRsCtrlSyn.IsOpen())   {
        if (!SeekValore (strName))   {
            m_pRsCtrlSyn.Delete ();
            m_pRsCtrlSyn.Update ();
            m_pRsCtrlSyn.AddNew ();
            v=strName.AllocSysString ();
            m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI, v);
            v1=(long)nTipo;
            m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_TIPO, v1);
            v2=strColonna.AllocSysString ();
            m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_COLONNA, v2);
            v3=(long)nRiga;
            m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_RIGA, v3);
            switch (nTipo)   {
                case LUA_TBOOLEAN :
                    fp=strtod (strValore, &p);
                    if (*p!='\0')   
                        fp=0.0;
                    v4=(double) fp;
                    m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_NUMERO, v4);
                    break;
                case LUA_TNUMBER :
                    fp=strtod (strValore, &p);
                    if (*p!='\0')   
                        fp=0.0;
                    v4=(double)fp;
                    m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_NUMERO, v4);
                    break;
                case LUA_TSTRING :
                    v4=strValore.AllocSysString ();
                    m_pRsCtrlSyn.SetFieldValue(FIELD_VALORI_STRINGA, v4);
                    break;
            }
            m_pRsCtrlSyn.Update ();
            n=0;
        }
        else
            m_pRsCtrlSyn.m_pRecordset->Requery (-1);
    }
    return 0;
}

/*
** ModificaVariabileDB :
*/
int CResourceFile::ModificaVariabileDB (CString strTable, CString strKeyName, CString strKeyValue, CString strField, CString strValue)
{
    CStringList slTables, slFields, slIndex;
    POSITION pos;
    bool bFoundTable=false, bFoundField=false, bFoundIndex=false;
    int n=-1;
    /* verificare che esistano sia la tabella l'indice programmato che il campo programmato */
    GetTableList (slTables);
    for (pos=slTables.GetHeadPosition (); pos; )   {
        if (slTables.GetNext (pos)==strTable)   {
            bFoundTable=true; break;
        }
    }
    if (bFoundTable)   {
        GetFieldsList (strTable, slFields);
        GetIndexList (strTable, slIndex);
        for (pos=slFields.GetHeadPosition (); pos; )   {
            if (slFields.GetNext (pos)==strField)   {
                bFoundField=true; break;
            }
        }
        for (pos=slIndex.GetHeadPosition (); pos; )   {
            if (slIndex.GetNext (pos)==strKeyName)   {
                bFoundIndex=true; break;
            }
        }
    }
    if (bFoundTable && bFoundField && bFoundIndex)   {
        /* aprire la tabella indicata */
        CADORecordset pRs;
		pRs = CADORecordset(&m_pDb);
        if (pRs.Open(strTable, CADORecordset::openTable))   {
            /* recuperare il tipo di dato dell indice programmato */
            DataTypeEnumX nTypeField;
            if (!GetFieldType (strTable, strField, nTypeField))   {
                stDBKey DBKey[NMAX_DBKEYS];
                int nCount;
                /* eseguire il parse della stringa dei valori degli indici */
                if (!GetIndexValue (strTable, strKeyName, strKeyValue, DBKey, &nCount))   {
                    COleSafeArray sa;
                    SetIndexArray (DBKey, nCount, sa);
                    /* ricercare il record usando l'indice e i valori passati */
                    pRs.m_pRecordset->PutIndex (strKeyName.AllocSysString ());
                    pRs.m_pRecordset->Seek (sa, adSeekFirstEQ);
                    /* settare il valore */
                    if (pRs.IsEof())   {
                        pRs.AddNew ();
                        SetIndexValue (pRs, DBKey, nCount, sa);
                    }
                    else
                        pRs.Edit ();
                    /* aggiorniamo il nostro campo */
                    double fp; bool b; long l;
                    char *e;
                    switch (nTypeField)   {
                        //case DataTypeEnumX::adInteger :
                        case 3 :
                            l=strtol (strValue, &e, 0);
                            if (*e=='\0')
                                pRs.SetFieldValue (strField, l);
                            break;
                        //case DataTypeEnumX::adDouble :
                        case 5 :
                            fp=strtod (strValue, &e);
                            if (*e=='\0')
                                pRs.SetFieldValue (strField, fp);
                            break;
                        //case DataTypeEnumX::adBoolean :
                        case 11 :
                            l=strtol (strValue, &e, 0);
                            if (*e=='\0')   {
                                b=(l!=0);
                                pRs.SetFieldValue (strField, b);
                            }
                            break;
                        //case DataTypeEnumX::adVarWChar :
                        case 202 :
                            pRs.SetFieldValue (strField, strValue);
                            break;
                    }
                    /* riscivere il record */
                    pRs.Update ();
                    pRs.m_pRecordset->Requery (-1);
                }
            }
            pRs.Close ();
        }        
    }
    return n;
}

/*
** GetFieldType :
*/
int CResourceFile::GetFieldType (CString strTable, CString strField, DataTypeEnumX &nType)
{
    _ConnectionPtr pConnection=NULL;
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    _ColumnPtr  pColumn = NULL;
    CString str, strFieldName;
    int nB=-1;

    pConnection.CreateInstance(__uuidof(Connection));
    if (pConnection)   {
        pCatalog.CreateInstance(__uuidof(Catalog));
        if (pCatalog)   {
            str.Format ("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", m_strNomeFile);
            pConnection->PutMode((enum ConnectModeEnum)(CADODatabase::connectModeReadWrite|CADODatabase::connectModeShareDenyWrite));
	        if (pConnection->Open (_bstr_t (str), "", "", NULL)==S_OK)   {
                pCatalog->put_ActiveConnection (_variant_t((IDispatch*)pConnection, TRUE));
                pTable=pCatalog->Tables->GetItem((const char*)strTable);
                long nItem, nFieldsCount=pTable->Columns->Count;
                while (nFieldsCount)   {
                    nItem=nFieldsCount-1;
                    pColumn=pTable->Columns->GetItem (nItem);
                    CString strColumnField( (char *)pColumn->GetName());
                    if (strField==strColumnField)   {
                        nType=pColumn->GetType () ; nB=0; break;
                    }
                    nFieldsCount--;
                }
                pConnection->Close ();
            }
        }
    }
    return nB;
}

/*
** GetIndexValue :
*/
int CResourceFile::GetIndexValue (CString strTable, CString strKeyName, CString strKeyValue, stDBKey *pDBKey, int *nCount)
{
    CStringList     slSubIndex;
    CString         strName;
    DataTypeEnumX   nTypeField;
    double          fp;
    long            n=0, l;
    int             nB=0;
    char            *p, *e;

    GetSubIndexList (strTable, strKeyName, slSubIndex);
    p=strtok (strKeyValue.GetBuffer (strKeyValue.GetLength ()+1), "|");
    for (POSITION pos=slSubIndex.GetHeadPosition (); pos; n++)   {
        if (p)   {
            strName=slSubIndex.GetNext (pos);
            pDBKey[n].strField=strName;
            GetFieldType (strTable, strName, nTypeField);
            switch (nTypeField)   {
                //case DataTypeEnumX::adInteger :
                case 3 :
                    l=strtol (p, &e, 0);
                    if (*e=='\0')
                        pDBKey[n].v=l;
                    else
                        return -1;
                    break;
                //case DataTypeEnumX::adDouble :
                case 5 :
                    fp=strtod (p, &e);
                    if (*e=='\0')
                        pDBKey[n].v=fp;
                    else
                        return -1;
                    break;
                //case DataTypeEnumX::adVarWChar :
                case 202 :
                    pDBKey[n].v=p;
                    break;
                default :
                    return -1;
            }
            p=strtok (NULL, "|");
        }
        else   {
            nB=-1; break;
        }
    }
    *nCount=n;
    return nB;
}

/*
** SetIndexArray :
*/
void CResourceFile::SetIndexArray (stDBKey *pDBKey, int nCount, COleSafeArray &sa)
{
    sa.CreateOneDim (VT_VARIANT, nCount);
    for (long i=0; i<nCount; i++)
        sa.PutElement (&i, &pDBKey[i].v);
}

/*
** GetSubIndexList :
*/
int CResourceFile::GetSubIndexList (CString strTable, CString strIndex, CStringList& slSubIndex)
{
    _ConnectionPtr pConnection=NULL;
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    _IndexPtr   pIndex=NULL;
    _ColumnPtr  pColumn=NULL;
    CString str, strFieldName;
    int nB=-1;

    pConnection.CreateInstance(__uuidof(Connection));
    if (pConnection)   {
        pCatalog.CreateInstance(__uuidof(Catalog));
        if (pCatalog)   {
            str.Format ("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s", m_strNomeFile);
            pConnection->PutMode((enum ConnectModeEnum)(CADODatabase::connectModeReadWrite|CADODatabase::connectModeShareDenyWrite));
	        if (pConnection->Open (_bstr_t (str), "", "", NULL)==S_OK)   {
                pCatalog->put_ActiveConnection (_variant_t((IDispatch*)pConnection, TRUE));
                pTable=pCatalog->Tables->GetItem((const char*)strTable);
                pIndex=pTable->Indexes->GetItem ((const char*)strIndex);
                long nItem, nIndexCount=pIndex->Columns->Count;
                while (nIndexCount)   {
                    nItem=nIndexCount-1;
                    pColumn=pIndex->Columns->GetItem (nItem);
                    slSubIndex.AddHead (pColumn->GetName());
                    nIndexCount--;
                }
                pConnection->Close (); nB=0;
            }
        }
    }
    return nB;
}

/*
** SetIndexValue :
*/
int CResourceFile::SetIndexValue (CADORecordset &pRs, stDBKey *pDBKey, int nCount, COleSafeArray &sa)
{
    VARIANT *v;
    long n=0;

    for (int i=0; i<nCount; i++)   {
        sa.PtrOfIndex (&n, (void**)&v);
        COleVariant ov (*v);
        switch (ov.vt)   {
            case VT_I4 :
                pRs.SetFieldValue (pDBKey[i].strField, ov.lVal);
                break;
            case VT_R8 :
                pRs.SetFieldValue (pDBKey[i].strField, ov.lVal);
                break;
            case VT_BSTR :
                pRs.SetFieldValue (pDBKey[i].strField, CString (ov.bstrVal));
                break;
        }
        n++;
    }
    return 0;
}

/*
** CaricaVariabile :
*/
int CResourceFile::CaricaVariabile (CString strName, CString strColonna, int nRiga, int nTipo, CString &strValore)
{
    CString strField, strFieldColonna;
    double fp;
    int nFieldRiga, nFieldTipo, nB=-1;

    if (m_pRsCtrlSyn.IsOpen())   {
        if (!SeekValore (strName))   {
            do   {
                m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI_COLONNA, strFieldColonna);
                m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI_RIGA, nFieldRiga);
                m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI_TIPO, nFieldTipo);
                if (strColonna==strFieldColonna && nRiga==nFieldRiga && nTipo==nFieldTipo)   {
                    switch (nTipo)   {
                        case LUA_TBOOLEAN :
                            m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI_NUMERO, fp);
                            strValore.Format ("%d", (int)fp);
                            break;
                        case LUA_TNUMBER :
                            m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI_NUMERO, fp);
                            strValore.Format ("%.3f", fp);
                            break;
                        case LUA_TSTRING :
                            m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI_STRINGA, strValore);
                            break;
                    }
                    nB=0;
                    break;
                }
                m_pRsCtrlSyn.MoveNext ();
                if (m_pRsCtrlSyn.IsEOF())
                    break;
                m_pRsCtrlSyn.GetFieldValue (FIELD_VALORI, strField);
            }   while (strField==strName);
        }
        else
            m_pRsImages.m_pRecordset->Requery (-1);
    }
    return nB;
}

/*
** CaricaDB :
*/
int CResourceFile::CaricaDB (CString strTable, CString strKeyName, CString strKeyValue, CString strField, CString &strValue)
{
    CStringList slTables, slFields, slIndex;
    POSITION pos;
    bool bFoundTable=false, bFoundField=false, bFoundIndex=false;
    int n=-1;
    /* verificare che esistano sia la tabella l'indice programmato che il campo programmato */
    GetTableList (slTables);
    for (pos=slTables.GetHeadPosition (); pos; )   {
        if (slTables.GetNext (pos)==strTable)   {
            bFoundTable=true; break;
        }
    }
    if (bFoundTable)   {
        GetFieldsList (strTable, slFields);
        GetIndexList (strTable, slIndex);
        for (pos=slFields.GetHeadPosition (); pos; )   {
            if (slFields.GetNext (pos)==strField)   {
                bFoundField=true; break;
            }
        }
        for (pos=slIndex.GetHeadPosition (); pos; )   {
            if (slIndex.GetNext (pos)==strKeyName)   {
                bFoundIndex=true; break;
            }
        }
    }
    if (bFoundTable && bFoundField && bFoundIndex)   {
        /* aprire la tabella indicata */
        CADORecordset pRs;
		pRs = CADORecordset(&m_pDb);
        if (pRs.Open(strTable, CADORecordset::openTable))   {
            /* recuperare il tipo di dato dell indice programmato */
            DataTypeEnumX nTypeField;
            if (!GetFieldType (strTable, strField, nTypeField))   {
                stDBKey DBKey[NMAX_DBKEYS];
                int nCount;
                /* eseguire il parse della stringa dei valori degli indici */
                if (!GetIndexValue (strTable, strKeyName, strKeyValue, DBKey, &nCount))   {
                    COleSafeArray sa;
                    SetIndexArray (DBKey, nCount, sa);
                    /* ricercare il record usando l'indice e i valori passati */
                    pRs.m_pRecordset->PutIndex (strKeyName.AllocSysString ());
                    pRs.m_pRecordset->Seek (sa, adSeekFirstEQ);
                    /* settare il valore */
                    if (!pRs.IsEof())   {
                        /* aggiorniamo il nostro campo */
                        double fp; bool b; long l;
                        char *e;
                        switch (nTypeField)   {
                            //case DataTypeEnumX::adInteger :
                            case 3 :
                                pRs.GetFieldValue (strField, l);
                                strValue.Format ("%d", l);
                                break;
                            //case DataTypeEnumX::adDouble :
                            case 5 :
                                pRs.GetFieldValue (strField, fp);
                                strValue.Format ("%.3f", fp);
                                break;
                            //case DataTypeEnumX::adBoolean :
                            case 11 :
                                pRs.GetFieldValue (strField, b);
                                strValue.Format ("%d", b ? 1 : 0);
                                break;
                            //case DataTypeEnumX::adVarWChar :
                            case 202 :
                                pRs.GetFieldValue (strField, strValue);
                                break;
                        }
                        n=0;
                    }
                    else
                        pRs.m_pRecordset->Requery (-1);
                }
            }
            pRs.Close ();
        }        
    }
    return n;
}

/*
** CreateTablesFromXML :
*/
int CResourceFile::CreateTablesFromXML (CString strFile, _CatalogPtr CatalogPtr)
{
    DocXmlPtr       pDocXml;
    ElementXmlPtr   pEl;
    NodeListXmlPtr  pRootChild;
    CString str;
    HRESULT hres=S_FALSE;
    _bstr_t bstrTables;
    int nItems, n;

    /* apriamo il file xml */
    HRESULT hr = pDocXml.CreateInstance( __uuidof(MSXML2::DOMDocument60));
    if (SUCCEEDED(hr))   {
        pDocXml->async = false;
        if (pDocXml->load(COleVariant(strFile)))   {
            /* cominciamo a scorrere i nodi e cerchiamo i nodi tables */
            pEl=pDocXml->documentElement;
            str=(LPCTSTR)pEl->nodeName;
            if (str=="Machine_Settings")   {
		        if (pEl->hasChildNodes())	  {
                    pRootChild = pEl->childNodes;
                    nItems=pRootChild->length;
                    n=0;
                    while (n<nItems)   {
                        str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
                        if (str=="Tables")   {
                            pEl=pRootChild->Getitem(n);
		                    if (pEl->hasChildNodes())	  {
                                pRootChild = pEl->childNodes;
                                nItems=pRootChild->length;
                                n=0;
                                while (n<nItems)   {
                                    str=(LPCTSTR)pRootChild->Getitem(n)->nodeName;
                                    if (str=="Table")
                                        if (CreateTableFromXML (CatalogPtr, pRootChild->Getitem(n)))
                                            hres=S_FALSE;
                                    n++;
                                }
                            }
                        }
                        n++;
                    }
                }
            }
        }
        else
            pDocXml.Release ();
    }
    return hres;
}

/*
** CreateTableFromXML :
*/
int CResourceFile::CreateTableFromXML (_CatalogPtr CatalogPtr, ElementXmlPtr pEl)
{
    ElementXmlPtr  pElChild;
    NodeListXmlPtr pRootChild;
    _TablePtr      pTable=NULL;
    COleVariant v, vAutoIncrement, vType, vDefaultValue, vLength;
    CString strTable, strFieldName;
    int n, nItems, nB=-1;

    /* per prima cosa prendiamo il nome della tabella */
    v=pEl->getAttribute ("Name");
    if (v.vt==VT_BSTR)   {
        pTable.CreateInstance(__uuidof(Table));
        if (pTable)   {
            strTable=CString (v.bstrVal);
            pTable->PutParentCatalog (CatalogPtr);
            pTable->PutName (strTable.AllocSysString());
            AppendTypeField (pTable);
            AppendSlotField (pTable);
            pRootChild = pEl->childNodes;
            nItems=pRootChild->length;
            n=0; nB=0;
            while (n<nItems)   {
                pElChild=pRootChild->Getitem(n);
                /* nome del campo */
                strFieldName=(LPCTSTR)pElChild->nodeName;
                /* autoincremento */
                vAutoIncrement=pElChild->getAttribute ("AutoIncrement");
                /* tipo del campo */
                vType=pElChild->getAttribute ("Type");
                /* valore di default del campo */
                vDefaultValue=pElChild->getAttribute ("DefaultValue");
                /* lunghezza del campo */
                vLength=pElChild->getAttribute ("Length");
                /* appendiamo i campi */
                if (AppendFields (pTable, strFieldName, vAutoIncrement, vType, vLength, vDefaultValue))   {
                    nB=-1; break;
                }
                n++;
            }
            if (!nB)   {
                AppendTypeSlotIndex (pTable);
                CatalogPtr->Tables->Append (_variant_t((IDispatch*)pTable, TRUE));
            }
        }
    }
    return nB;
}

/*
** AppendFields :
*/
int CResourceFile::AppendFields (_TablePtr pTable, CString strFieldName, COleVariant &vAutoIncrement, COleVariant &vType, COleVariant &vLength, COleVariant &vDefaultValue)
{
    CString strType, strLenght;
    DataTypeEnumX ex;
    _variant_t v;
    int nLength, nB=-1;
    bool bFound=true;

    if (vType.vt!=VT_NULL)   {
        strType=CString (vType.bstrVal);
        if (strType=="VT_R8")   {
            ex=(DataTypeEnumX)5;
            nLength=sizeof (double);
            if (vDefaultValue.vt!=VT_NULL)   {
	            v.vt = VT_R8;
	            v.dblVal = atof (CString (vDefaultValue.bstrVal));
            }
            else   {
	            v.vt = VT_R8;
	            v.dblVal = 0.0;
            }
        }
        else
        if (strType=="VT_R4")   {
            ex=(DataTypeEnumX)4;
            nLength=sizeof (float);
            if (vDefaultValue.vt!=VT_NULL)   {
	            v.vt = VT_R4;
	            v.fltVal = (float)atof (CString (vDefaultValue.bstrVal));
            }
            else   {
	            v.vt = VT_R4;
	            v.fltVal = 0.0;
            }
        }
        else
        if (strType=="VT_I4")   {
            ex=(DataTypeEnumX)3;
            nLength=sizeof (int);
            if (vDefaultValue.vt!=VT_NULL)   {
	            v.vt = VT_I4;
	            v.lVal = atoi (CString (vDefaultValue.bstrVal));
            }
            else   {
	            v.vt = VT_I4;
	            v.lVal = 0;
            }
        }
        else
        if (strType=="VT_I2")   {
            ex=(DataTypeEnumX)2;
            nLength=sizeof (short int);
            if (vDefaultValue.vt!=VT_NULL)   {
	            v.vt = VT_I2;
	            v.iVal = (short) atoi (CString (vDefaultValue.bstrVal));
            }
            else   {
	            v.vt = VT_I2;
	            v.iVal = 0;
            }
        }
        else
        if (strType=="VT_BSTR")   {
            ex=(DataTypeEnumX)202;
            if (vLength.vt!=VT_NULL)   {
                strLenght=CString (vLength.bstrVal);
                nLength=atoi (strLenght);
                if (nLength==0)
                    nLength=255;
            }
            else
                nLength=255;
            if (vDefaultValue.vt!=VT_NULL)
                v=CString (vDefaultValue.bstrVal);
            else
                v="";
        }
        else
        if (strType=="VT_BOOL")   {
            ex=(DataTypeEnumX)11;
            nLength=sizeof (BOOL);
            if (vDefaultValue.vt!=VT_NULL)    {
	            v.vt = VT_BOOL;
                if (CString (vDefaultValue.bstrVal)=="false")
	                v.boolVal=VARIANT_FALSE;
                else
                    v.boolVal=VARIANT_TRUE;
            }
            else   {
	            v.vt = VT_BOOL;
	            v.boolVal=VARIANT_TRUE;
            }
        }
        else
            bFound=false;
        if (bFound)   {
            pTable->Columns->Append ((const char *)strFieldName, ex, nLength);
            if (vAutoIncrement.vt!=VT_NULL)   {
                if (CString (vAutoIncrement.bstrVal)=="true")   {
                    _variant_t vAutoIncrement=true;
                    pTable->Columns->GetItem ((const char *)strFieldName)->Properties->GetItem ("Autoincrement")->PutValue (vAutoIncrement);
                }
            }
            else
                pTable->Columns->GetItem ((const char *)strFieldName)->Properties->GetItem ("Default")->PutValue (v);
            nB=0;
        }
    }
    return nB;
}

/*
** AppendTypeField :
*/
void CResourceFile::AppendTypeField (_TablePtr pTable)
{
    DataTypeEnumX ex=(DataTypeEnumX)2;
    int nLength=sizeof (short int);
    _variant_t v;

	v.vt = VT_I2;
	v.iVal = 0;
    pTable->Columns->Append ((const char *)TYPE_FIELD_NAME, ex, nLength);
    pTable->Columns->GetItem ((const char *)TYPE_FIELD_NAME)->Properties->GetItem ("Default")->PutValue (v);
}

/*
** AppendSlotField :
*/
void CResourceFile::AppendSlotField (_TablePtr pTable)
{
    DataTypeEnumX ex=(DataTypeEnumX)2;
    int nLength=sizeof (short int);
    _variant_t v;

	v.vt = VT_I2;
	v.iVal = 0;
    pTable->Columns->Append ((const char *)SLOT_FIELD_NAME, ex, nLength);
    pTable->Columns->GetItem ((const char *)SLOT_FIELD_NAME)->Properties->GetItem ("Default")->PutValue (v);
}

/*
** AppendTypeSlotIndex :
*/
void CResourceFile::AppendTypeSlotIndex (_TablePtr pTable)
{
    _IndexPtr pIndex;

    pIndex.CreateInstance(__uuidof(Index));
    pIndex->PutName (TYPE_SLOT_INDEX_NAME);
    pIndex->Columns->Append (TYPE_FIELD_NAME, (DataTypeEnumX)2, sizeof (short int));
    pIndex->Columns->Append (SLOT_FIELD_NAME, (DataTypeEnumX)2, sizeof (short int));
    pTable->Indexes->Append (_variant_t((IDispatch *)pIndex));
}

/*
** AllocRS :
*/
int CResourceFile::AllocRS (CString &strName)
{
    int nIndex=-1;
    CADORecordset *pRS=new CADORecordset;
    if (pRS)   {
        *pRS=CADORecordset(&m_pDb);
        if (pRS->Open(strName, CADORecordset::openTable))   {
            nIndex=GetRSIndex ();
            if (nIndex!=-1)
                m_pRsArray[nIndex]=pRS;
        }
    }
    return nIndex;
}

/*
** FreeRS :
*/
int CResourceFile::FreeRS  (int nRS)
{
    if (m_pRsArray[nRS]!=NULL)   {
        ((CADORecordset *)m_pRsArray[nRS])->Close ();
        delete m_pRsArray[nRS];
        m_pRsArray[nRS]=NULL;
    }
    return 0;
}

/*
** GetRSIndex :
*/
int CResourceFile::GetRSIndex ()
{
    int n=-1, i, nSize=m_pRsArray.GetSize ();
    bool bFound=false;

    if (nSize)   {
        for (i=0; i<nSize; i++)
            if (m_pRsArray[i]==NULL)   {
                bFound=true; break;
            }
    }
    return i;
}

/*
** SeekSlot :
*/
int CResourceFile::SeekSlot (int nRS, int nType, int nSlot)
{
    COleSafeArray sa;
    _variant_t v, v1;
    long i=0;

    v =(short int)nType;
    v1=(short int)nSlot;
    sa.CreateOneDim (VT_VARIANT, 2);
    sa.PutElement (&i, &v);
    i++;
    sa.PutElement (&i, &v1);
    ((CADORecordset *)m_pRsArray[nRS])->m_pRecordset->PutIndex (TYPE_SLOT_INDEX_NAME);
    ((CADORecordset *)m_pRsArray[nRS])->m_pRecordset->Seek (sa, adSeekFirstEQ);
    return ((CADORecordset *)m_pRsArray[nRS])->IsEof();
}

/*
** AddRSRecords :
*/
int CResourceFile::AddRSRecords (CString strTable, int nType, CPtrArray &data)
{
    COleVariant vType, vSlot;
    /* apriamo il recordset */
    int nRS=AllocRS (strTable), nB=-1;
    if (nRS!=-1)   {
        /* inseriamo tutti i records */
        for (int i=0; i<data.GetSize (); i++)   {
            if (data[i])
                AddSingleRecord (nRS, nType, i, *((CPtrArray *)data[i]));
        }
        /* liberiamo il recordset */
        FreeRS (nRS); nB=0;
    }
    return nB;
}

/*
** AddSingleRecord :
*/
void CResourceFile::AddSingleRecord (int nRS, int nType, int nSlot, CPtrArray &data)
{
    COleVariant vType, vSlot;

    vType=(short int) nType;
    vSlot=(short int) nSlot;
    ((CADORecordset *)m_pRsArray[nRS])->AddNew ();
    ((CADORecordset *)m_pRsArray[nRS])->SetFieldValue (TYPE_FIELD_NAME, (_variant_t)vType);
    ((CADORecordset *)m_pRsArray[nRS])->SetFieldValue (SLOT_FIELD_NAME, (_variant_t)vSlot);
    for (int i=0; i<data.GetSize (); i++)
        ((CADORecordset *)m_pRsArray[nRS])->SetFieldValue (((stDBKey *)data[i])->strField, (_variant_t)((stDBKey *)data[i])->v);
    ((CADORecordset *)m_pRsArray[nRS])->Update ();
}
    
/*
** GetRSRecord :
*/
int CResourceFile::GetRSRecords (CString strTable, int nType, CStringArray &fields, CPtrArray &data)
{
    COleSafeArray sa;
    _variant_t v, v1;
    int nSlot;
    long i=0;

    /* apriamo il recordset */
    int nRS=AllocRS (strTable), nB=-1;
    if (nRS!=-1)   {
        v =(short int)nType;
        sa.CreateOneDim (VT_VARIANT, 1);
        sa.PutElement (&i, &v);
        ((CADORecordset *)m_pRsArray[nRS])->m_pRecordset->PutIndex (TYPE_SLOT_INDEX_NAME);
        ((CADORecordset *)m_pRsArray[nRS])->m_pRecordset->Seek (sa, adSeekAfterEQ);
        if (!((CADORecordset *)m_pRsArray[nRS])->IsEof())   {
            do   {
                CPtrArray *pArr=new CPtrArray;
                ((CADORecordset *)m_pRsArray[nRS])->GetFieldValue (TYPE_FIELD_NAME, v);
                if ((short int) v == nType)   {
					((CADORecordset *)m_pRsArray[nRS])->GetFieldValue (SLOT_FIELD_NAME, v);
					nSlot=(int)(long)v;
					pArr->SetSize (fields.GetSize ());
					for (int i=0; i<fields.GetSize (); i++)   {
						((CADORecordset *)m_pRsArray[nRS])->GetFieldValue (fields[i], v);
						stDBKey *pKey=new stDBKey;
						pKey->v=v;
						(*pArr)[i]=pKey;
					}
					/* ridimensioniamo l'array che contiene i risultati */
					if (nSlot+1>data.GetSize ())
						data.SetSize(nSlot+1);
					data[nSlot]=pArr;
                }
                ((CADORecordset *)m_pRsArray[nRS])->MoveNext ();
            }   while (!((CADORecordset *)m_pRsArray[nRS])->IsEof());
        }
        /* liberiamo il recordset */
        FreeRS (nRS); nB=0;
    }
    return nB;
}

/*
** DeleteRSRecords :
*/
int CResourceFile::DeleteRSRecords (CString strTable, int nType)
{
    _variant_t v;
    int nRS=AllocRS (strTable), nB=-1;
    if (nRS!=-1)   {
        if (!((CADORecordset *)m_pRsArray[nRS])->IsEOF ())   {
			((CADORecordset *)m_pRsArray[nRS])->MoveFirst ();
			while (!((CADORecordset *)m_pRsArray[nRS])->IsEof())   { 
					((CADORecordset *)m_pRsArray[nRS])->GetFieldValue (TYPE_FIELD_NAME, v);
					if ((short int)v == nType)   {
				((CADORecordset *)m_pRsArray[nRS])->Delete ();
				((CADORecordset *)m_pRsArray[nRS])->Update ();
					}
				((CADORecordset *)m_pRsArray[nRS])->MoveNext ();
			}
		}
    }
	FreeRS (nRS); nB=0;
    return nB;
}

int CResourceFile::GetInfo ()
{
    /* supermegadebug */
    ADODB::ObjectStateEnum e;
    e=(ADODB::ObjectStateEnum)m_pDb.GetActiveConnection ()->GetState();
    return (int)e;
}

/*
** CreateTable :
*/
int CResourceFile::CreateTable (CString strTable)
{
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    CString str;
    int n=0;

	pCatalog.CreateInstance(__uuidof(Catalog));
	if (pCatalog)   {
		pCatalog->put_ActiveConnection (_variant_t((IDispatch*)m_pDb.m_pConnection, TRUE));
		pTable.CreateInstance(__uuidof(Table));
		if (pTable)   {
			pTable->PutParentCatalog (pCatalog);
			pTable->PutName (strTable.AllocSysString ());
			pTable->Columns->Append ("ID", (DataTypeEnumX)3, sizeof (int));
			pTable->Columns->GetItem ("ID")->Properties->GetItem ("Autoincrement")->PutValue (true);
			pCatalog->Tables->Append (_variant_t((IDispatch*)pTable, TRUE));
			pTable.Release ();
		}
		pCatalog.Release ();
	}
	return 0;
}

/*
** DeleteTable :
*/
int CResourceFile::DeleteTable (CString strTable)
{
    _variant_t v;
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    CString str;
    int nTableCount, i=0;
	long nCount;

	pCatalog.CreateInstance(__uuidof(Catalog));
	if (pCatalog)   {
		pCatalog->put_ActiveConnection (_variant_t((IDispatch*)m_pDb.m_pConnection, TRUE));
		nTableCount=pCatalog->Tables->Count;
        while (i < nTableCount)   {
			nCount = i;
			pTable=pCatalog->Tables->GetItem (nCount);
			if (CString ((char *)pTable->GetName ()) == strTable)   {
				pCatalog->Tables->Delete (strTable.AllocSysString ());
				break;
			}
			i++;
		}
		pCatalog.Release ();
	}
	return 0;
}

/*
** ExistTable :
*/
bool CResourceFile::ExistTable (CString strTable)
{
    _variant_t v;
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    CString str;
    int nTableCount, i=0;
	long nCount;
	bool b = false;

	pCatalog.CreateInstance(__uuidof(Catalog));
	if (pCatalog)   {
		pCatalog->put_ActiveConnection (_variant_t((IDispatch*)m_pDb.m_pConnection, TRUE));
		nTableCount=pCatalog->Tables->Count;
        while (i < nTableCount)   {
			nCount = i;
			pTable=pCatalog->Tables->GetItem (nCount);
			if (CString ((char *)pTable->GetName ()) == strTable)   {
				b = true;
				break;
			}
			i++;
		}
		pCatalog.Release ();
	}
	return b;
}

/*
** AddGridColumns :
*/
int CResourceFile::AddGridColumns (CString strTable, stGridColumnData *pgcd, int nColumns)
{
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
    _variant_t v;
    CString str;
    int nTableCount, n=0, i=0;
	long nCount;

	pCatalog.CreateInstance(__uuidof(Catalog));
	if (pCatalog)   {
		pCatalog->put_ActiveConnection (_variant_t((IDispatch*)m_pDb.m_pConnection, TRUE));
		nTableCount=pCatalog->Tables->Count;
        while (i < nTableCount)   {
			nCount = i;
			pTable=pCatalog->Tables->GetItem (nCount);
			if (CString ((char *)pTable->GetName ()) == strTable)   {
				pTable->Columns->Append ("Riga", (DataTypeEnumX)3, sizeof (int));
				v.vt = VT_I4; v.lVal = 0;
				pTable->Columns->GetItem ("Riga")->Properties->GetItem ("Default")->PutValue (v);
				while (n < nColumns)   {
					switch (pgcd[n].m_nType)   {
						case LUA_TNUMBER :
							pTable->Columns->Append ((const char *)pgcd[n].m_strName, (DataTypeEnumX)5, sizeof (double));
							v.vt = VT_R8; v.dblVal = 0.0;
							pTable->Columns->GetItem ((const char *)pgcd[n].m_strName)->Properties->GetItem ("Default")->PutValue (v);
							break;
						case LUA_TBOOLEAN :
							pTable->Columns->Append ((const char *)pgcd[n].m_strName, (DataTypeEnumX)11, sizeof (BOOL));
							v.vt = VT_BOOL; v.boolVal=VARIANT_TRUE;
							pTable->Columns->GetItem ((const char *)pgcd[n].m_strName)->Properties->GetItem ("Default")->PutValue (v);
							break;
						case LUA_TSTRING :
							pTable->Columns->Append ((const char *)pgcd[n].m_strName, (DataTypeEnumX)202, 255);
							v="";
							pTable->Columns->GetItem ((const char *)pgcd[n].m_strName)->Properties->GetItem ("Default")->PutValue (v);
							break;
					}
					n++;
				}
				pTable->Keys->Append ("PrimaryKey", adKeyPrimary, "Riga", "", "");
				break;
			}
			i++;
		}
		pCatalog.Release ();
	}
	return 0;
}

int CResourceFile::ManageGridColumns (CString strTable, stGridColumnData *pgcd, int nColumns)
{
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
	_ColumnPtr  pColumn=NULL;
    _variant_t v;
	CStringList sl;
    CString str;
    int nTableCount, nColumnCount, n, i;
	long nCount;
	bool bFound;

	pCatalog.CreateInstance(__uuidof(Catalog));
	if (pCatalog)   {
		pCatalog->put_ActiveConnection (_variant_t((IDispatch*)m_pDb.m_pConnection, TRUE));
		nTableCount=pCatalog->Tables->Count;
		i = 0;
        while (i < nTableCount)   {
			nCount = i;
			pTable=pCatalog->Tables->GetItem (nCount);
			if (CString ((char *)pTable->GetName ()) == strTable)   {
				/* per prima cosa inseriamo le nuova colonne */
				for (i = 0; i<nColumns; i++)   {
					nColumnCount=pTable->Columns->Count;
					n = 0; bFound = false;
					while (n < nColumnCount)   {
						nCount = n;
						pColumn=pTable->Columns->GetItem (nCount);
						if (CString ((char *)pColumn->GetName ()) == pgcd[i].m_strName)   {
							bFound = true;
							break;
						}
						n++;
					}
					if (!bFound)   {
						switch (pgcd[i].m_nType)   {
							case LUA_TNUMBER :
								pTable->Columns->Append ((const char *)pgcd[i].m_strName, (DataTypeEnumX)5, sizeof (double));
								v.vt = VT_R8; v.dblVal = 0.0;
								pTable->Columns->GetItem ((const char *)pgcd[i].m_strName)->Properties->GetItem ("Default")->PutValue (v);
								break;
							case LUA_TBOOLEAN :
								pTable->Columns->Append ((const char *)pgcd[i].m_strName, (DataTypeEnumX)11, sizeof (BOOL));
								v.vt = VT_BOOL; v.boolVal=VARIANT_TRUE;
								pTable->Columns->GetItem ((const char *)pgcd[i].m_strName)->Properties->GetItem ("Default")->PutValue (v);
								break;
							case LUA_TSTRING :
								pTable->Columns->Append ((const char *)pgcd[i].m_strName, (DataTypeEnumX)202, 255);
								v="";
								pTable->Columns->GetItem ((const char *)pgcd[i].m_strName)->Properties->GetItem ("Default")->PutValue (v);
								break;
						}
					}
				}
				n = 0;
				/* ora cancelliamo quelle in eccesso */
				nColumnCount=pTable->Columns->Count;
				while (n < nColumnCount)   {
					nCount = n;
					pColumn=pTable->Columns->GetItem (nCount);
					str = (char *)pColumn->GetName ();
					for (i = 0; i<nColumns; i++)   {
						if (str == pgcd[i].m_strName)   {
							bFound = true;
							break;
						}
					}
					if (!bFound)
						if (str != "Riga" && str != "ID")
							sl.AddTail (str);
					n++;
				}
				for (POSITION pos = sl.GetHeadPosition (); pos;)   {
					str = sl.GetNext (pos);
					pTable->Columns->Delete (str.AllocSysString ());
				}
				break;
			}
			i++;
		}
		pCatalog.Release ();
	}
	return 0;
}

/*
** CheckCompatibilita :
*/
void CResourceFile::CheckCompatibilita ()
{
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;

	/* se c'e' la tabella ResValori seghiamola */
	DeleteTable ("ResValori");
	/* creiamo invece la nuova tabella del sinottico */
	if (!ExistTable (RESTABLE_RESCONTROLLISINOTTICO))   {
		pCatalog.CreateInstance(__uuidof(Catalog));
		if (pCatalog)   {
			pCatalog->put_ActiveConnection (_variant_t((IDispatch*)m_pDb.m_pConnection, TRUE));
			pTable.CreateInstance(__uuidof(Table));
			if (pTable)   {
				pTable->PutParentCatalog (pCatalog);
				CreaTabellaResControlliSinottico (pTable);
				pCatalog->Tables->Append (_variant_t((IDispatch*)pTable, TRUE));
				pTable.Release ();
			}
			pCatalog.Release ();
		}
	}
	/* se esiste la tabella HTML, verifichiamo che ci sia la colonna MainSyn e nel caso aggiungiamola */
	pCatalog.CreateInstance(__uuidof(Catalog));
	if (pCatalog)   {
		pCatalog->put_ActiveConnection (_variant_t((IDispatch*)m_pDb.m_pConnection, TRUE));
		pTable = FindTable (pCatalog, RESTABLE_RESHTML);
		if (pTable)   {
			AggiungiUnaColonna (pTable, FIELD_MAIN_SYN, LUA_TBOOLEAN);
			pTable.Release ();
		}
		pCatalog.Release ();
	}		
}

/*
** FindTable :
*/
_TablePtr CResourceFile::FindTable (_CatalogPtr pCatalog, CString strTable)
{
    _TablePtr pTable, pRetTable = NULL;
	long lCount;
	int nTableCount = pCatalog->Tables->Count, i = 0;

	while (i < nTableCount)   {
		lCount = i;
		pTable=pCatalog->Tables->GetItem (lCount);
		if (CString ((char *)pTable->GetName ()) == strTable)   {
			pRetTable = pTable;
			break;
		}
		i++;
	}
	return pRetTable;
}

/*
** FindColumn :
*/
_ColumnPtr CResourceFile::FindColumn (_TablePtr pTable, CString strColumn)
{
	_ColumnPtr pColumn, pRetColumn = NULL;
	long lCount;
	int nColumnCount = pTable->Columns->Count, n = 0;

	while (n < nColumnCount)   {
		lCount = n;
		pColumn=pTable->Columns->GetItem (lCount);
		if (CString ((char *)pColumn->GetName ()) == strColumn)   {
			pRetColumn = pColumn;
			break;
		}
		n++;
	}
	return pRetColumn;
}

/*
** AggiungiUnaColonna :
*/
int CResourceFile::AggiungiUnaColonna (_TablePtr pTable, CString strColumn, int nType)
{
    _variant_t v;
	int nR = 0;
	if (!FindColumn (pTable, strColumn))   {
		switch (nType)   {
			case LUA_TNUMBER :
				pTable->Columns->Append ((const char *)strColumn, (DataTypeEnumX)5, sizeof (double));
				v.vt = VT_R8; v.dblVal = 0.0;
				pTable->Columns->GetItem ((const char *)strColumn)->Properties->GetItem ("Default")->PutValue (v);
				break;
			case LUA_TBOOLEAN :
				pTable->Columns->Append ((const char *)strColumn, (DataTypeEnumX)11, sizeof (BOOL));
				v.vt = VT_BOOL; v.boolVal=VARIANT_TRUE;
				pTable->Columns->GetItem ((const char *)strColumn)->Properties->GetItem ("Default")->PutValue (v);
				break;
			case LUA_TSTRING :
				pTable->Columns->Append ((const char *)strColumn, (DataTypeEnumX)202, 255);
				v="";
				pTable->Columns->GetItem ((const char *)strColumn)->Properties->GetItem ("Default")->PutValue (v);
				break;
			default :
				nR = -1;
				break;
		}
	}
	else
		nR = -1;
	return nR;
}

/*
** GetDB :
*/
CADODatabase *CResourceFile::GetDB ()
{
	return &m_pDb;
}

/*
** GetDB :
*/
CADORecordset *CResourceFile::GetRSImages ()
{
	return &m_pRsImages;
}

/*
** GetDB :
*/
CADORecordset *CResourceFile::GetRSCtrlSyn ()
{
	return &m_pRsCtrlSyn;
}

/*
** GetDB :
*/
CADORecordset *CResourceFile::GetRSDB ()
{
	return &m_pRs;
}

/*
** GetSynList :
*/
int CResourceFile::GetSynList (CStringList& slSyn)
{
    COleSafeArray sa;
	CString strName;
    _variant_t v;
    long i=0;
	int nTipo;

    v=(long)eDialogs;
    sa.CreateOneDim (VT_VARIANT, 1);
    sa.PutElement (&i, &v);
    // i++;
    // sa.PutElement (&i, &v1);
    m_pRs.m_pRecordset->PutIndex (RESINDEX_RESTIPORISORSA);
    m_pRs.m_pRecordset->Seek (sa, adSeekFirstEQ);
	if (!m_pRs.IsEOF())   {
		do   {
			m_pRs.GetFieldValue ("NomeForm", strName);
			slSyn.AddTail (strName);
			m_pRs.MoveNext ();
            if (m_pRs.IsEOF())
                break;
            m_pRs.GetFieldValue ("Tipo", nTipo);
        }   while (nTipo==eDialogs);
	}
	else
		m_pRs.m_pRecordset->Requery (-1);
	return 0;
}

/*
** CheckImageColumn :
*/
int CResourceFile::CheckImageColumn (CStringArray& apt)
{
	/* inseriamo nelle tabelle di parametrica se non c'e', la colonna image */
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
	int i;

	pCatalog.CreateInstance(__uuidof(Catalog));
	if (pCatalog)   {
		pCatalog->put_ActiveConnection (_variant_t((IDispatch*)m_pDb.m_pConnection, TRUE));
		i = 0;
		while (i < apt.GetSize ())  {
			pTable = FindTable (pCatalog, apt[i]);
			if (pTable)   {
				AggiungiUnaColonna (pTable, FIELD_IMAGE, LUA_TSTRING);
				pTable.Release ();
			}
			i++;
		}
	}
	return 0;
}

/*
** ReplaceFormName :
*/
int CResourceFile::ReplaceFormName (_variant_t &v, CString &strHTML, CString &strNew)
{
	CString strToReplace;

	strHTML = v;
	int nIndexFirst = strHTML.Find ("formname="), nIndexLast;
	if (nIndexFirst != -1)   {
		nIndexFirst += strlen ("formname=") + 1;
		nIndexLast = strHTML.Find ('\"', nIndexFirst);
		strToReplace = strHTML.Mid (nIndexFirst, nIndexLast - nIndexFirst);
		strHTML.Replace (strToReplace, strNew);
	}
	return 0;
}
