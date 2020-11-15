/*
 * ResourceFile.h
 */

#ifndef __ADAMO_RESOURCE_FILE__
#define __ADAMO_RESOURCE_FILE__

/* import directive */
#import <msxml6.dll> 
using namespace MSXML2;

/* type definition */
typedef MSXML2::IXMLDOMDocumentPtr		DocXmlPtr;
typedef MSXML2::IXMLDOMElementPtr		ElementXmlPtr;
typedef MSXML2::IXMLDOMNodePtr			NodeXmlPtr ;
typedef MSXML2::IXMLDOMNodeListPtr		NodeListXmlPtr;
typedef MSXML2::IXMLDOMNamedNodeMapPtr	NodeMapPtr;

/* constant definitions */
#define SYSTEM_TABLE    "SYSTEM TABLE"
#define ACCESS_TABLE    "ACCESS TABLE"
#define SLOT_FIELD_NAME             "Slot"
#define TYPE_FIELD_NAME             "Type"
#define TYPE_SLOT_INDEX_NAME        "TypeSlot"
#define RESTABLE_RESIMAGES      "ResImages"
#define RESTABLE_RESHTML        "ResHTML"
#define RESTABLE_RESTOOLS       "ResTools"
#define RESTABLE_RESHEADS       "ResHeads"
#define RESTABLE_RESCONTROLLISINOTTICO "ResControlliSinottico"
#define RESINDEX_RESIMAGES      "IndexIDImmagine"
#define RESINDEX_RESCONTROLLISINOTTICO "IndexControlliSinottico"
#define RESINDEX_RESRISORSA     "IndexNomeRisorsa"
#define RESINDEX_RESTIPORISORSA "IndexTipoNomeRisorsa"
#define RESINDEX_RESTOOLNAME    "NomeUtensile"
#define RESINDEX_RESTOOLNUMBER  "NumeroUtensile"
#define RESINDEX_RESTOOLTYPE    "TipoUtensile"
#define RESINDEX_RESHEADNAME    "NomeTestina"
#define FIELD_ID                "ID"
#define FIELD_NOMEFORM          "NomeForm"
#define FIELD_TIPO              "Tipo"
#define FIELD_HTML              "HTML"
#define FIELD_SALVA_SU          "SalvaSu"
#define FIELD_SALVA_COME        "SalvaCome"
#define FIELD_MAIN_SYN          "MainSyn"
#define FIELD_IMAGE             "Image"
#define FIELD_VALORI            "NomeVariabile"
#define FIELD_VALORI_RIGA       "Riga"
#define FIELD_VALORI_COLONNA    "Colonna"
#define FIELD_VALORI_TIPO       "Tipo"
#define FIELD_VALORI_STRINGA    "ValoreStringa"
#define FIELD_VALORI_NUMERO     "ValoreDouble"
#define FIELD_IMAGES_ID         "ID"
#define FIELD_IMAGES_IDIMAGES   "IDImmagine"
#define FIELD_IMAGES_TYPE       "IDFile"
#define FIELD_IMAGES_LENGTH     "LengthData"
#define FIELD_IMAGES_FILEDATA   "FileData"
#define FIELD_TOOLS_ID          "ID"
#define FIELD_TOOLS_NOMEUTENSILE    "NomeUtensile"
#define FIELD_TOOLS_DESCRIZIONE     "Descrizione"
#define FIELD_TOOLS_NUMEROUTENSILE  "NumeroUtensile"
#define FIELD_TOOLS_TIPOUTENSILE    "TipoUtensile"
#define FIELD_TOOLS_DIAMETROEDGE1   "DiametroEdge1"
#define FIELD_TOOLS_DIAMETROEDGE2   "DiametroEdge2"
#define FIELD_TOOLS_DIAMETROEDGE3   "DiametroEdge3"
#define FIELD_TOOLS_LUNGHEZZAEDGE1  "LunghezzaEdge1"
#define FIELD_TOOLS_LUNGHEZZAEDGE2  "LunghezzaEdge2"
#define FIELD_TOOLS_LUNGHEZZAEDGE3  "LunghezzaEdge3"
#define FIELD_TOOLS_COMPENSAZIONEDIAMETRO1   "CompensazioneDiametro1"
#define FIELD_TOOLS_COMPENSAZIONEDIAMETRO2   "CompensazioneDiametro2"
#define FIELD_TOOLS_COMPENSAZIONEDIAMETRO3   "CompensazioneDiametro3"
#define FIELD_TOOLS_COMPENSAZIONELUNGHEZZA1  "CompensazioneLunghezza1"
#define FIELD_TOOLS_COMPENSAZIONELUNGHEZZA2  "CompensazioneLunghezza2"
#define FIELD_TOOLS_COMPENSAZIONELUNGHEZZA3  "CompensazioneLunghezza3"
#define FIELD_TOOLS_SCARICOPASSATA  "ScaricoPassata"
#define FIELD_TOOLS_PASSO           "Passo"
#define FIELD_TOOLS_CONETTOUTENSILE "ConettoUtensile"
#define FIELD_TOOLS_ALTEZZADISCO    "AltezzaDisco"
#define FIELD_TOOLS_ROTAZIONEDESTRA "RotazioneDestra"
#define FIELD_TOOLS_ANGOLOSVASATURA "AngoloSvasatura"
#define FIELD_TOOLS_ACCELERAZIONE   "Accelerazione"
#define FIELD_TOOLS_DECELERAZIONE   "Decelerazione"
#define FIELD_TOOLS_VELMINROT       "VelocitaMinimaRotazione"
#define FIELD_TOOLS_VELMAXROT       "VelocitaMassimaRotazione"
#define FIELD_TOOLS_VELLAVORO       "VelocitaLavoro"
#define FIELD_TOOLS_VELINGRESSO     "VelocitaIngresso"
#define FIELD_TOOLS_VELUSCITA       "VelocitaUscita"
#define FIELD_TOOLS_ROTAZIONELAVORO "RotazioneLavoro"
#define FIELD_TOOLS_LUBRIFICAZIONE  "Lubrificazione"
#define FIELD_TOOLS_IMPULSI         "Impulsi"
#define FIELD_TOOLS_INTERVALLOIMPULSI           "IntervalloImpulsi"
#define FIELD_TOOLS_INTERVALLOLUBRIFICAZIONE    "IntervalloLubrificazione"
#define FIELD_TOOLS_OREDILAVORAZIONE            "OreDiLavorazione"
#define FIELD_TOOLS_BITMAP                      "Bitmap"
#define FIELD_HEADS_ID              "ID"
#define FIELD_HEADS_NOMETESTINA     "NomeTestina"
#define FIELD_HEADS_DESCRIZIONE     "Descrizione"
#define FIELD_HEADS_NUMEROCONO      "NumeroCono"
#define FIELD_HEADS_OFFSETX         "OffsetX"
#define FIELD_HEADS_OFFSETY         "OffsetY"
#define FIELD_HEADS_OFFSETZ         "OffsetZ"
#define FIELD_HEADS_ROTAZIONEX      "RotazioneX"
#define FIELD_HEADS_ROTAZIONEY      "RotazioneY"
#define FIELD_HEADS_ROTAZIONEZ      "RotazioneZ"
#define FIELD_HEADS_BRACCIO         "Braccio"
#define FIELD_HEADS_TOOL            "Utensile"
#define FIELD_HEADS_BITMAP          "Bitmap"

#define MAX_DYNRS                   16

/* include file */
#include "ado2.h"
#include "Path.h"
#include "adamoglobal.h"

#define NMAX_DBKEYS 8

using namespace nsPath;

struct stDBKey   {
    CString     strField;
    COleVariant v;
};

class CResourceFile   {
    public :
        CResourceFile ();
        int ApriFileDiRisorse (CPath strNomeFile, int nFlags);
        int CreaFileDiRisorse (CPath strNomeFile, CPath strXMLFile);
        int ChiudiFileDiRisorse ();
		int CreaTabellaResControlliSinottico (_TablePtr pTable);
        int CreaTabellaResHTML (_TablePtr pTable);
        int CreaTabellaResImages (_TablePtr pTable);
        int CreaTabellaResTools (_TablePtr pTable);
        int CreaTabellaResHeads (_TablePtr pTable);
        int AggiungiRecordVuoto (CString str, eResources e);
        int CancellaRecordVuoto (CString str, eResources e);
        int GetFirstRSRecord (CString& str, eResources& e);
        int GetNextRSRecord (CString& str, eResources& e);
        int RinominaRecord (CString strPrev, CString strNew, eResources e);
        int ModificaRecord (CString str, eResources e, CString strXML, bool bIsMainSyn);
        int GetXML (CString str, eResources e, CString& strXML, bool& bIsMainSyn);
        int EsisteImmagine (CString strID);
        int AggiungiImmagine (CString strID, char* p, DWORD nLength);
        int CancellaImmagine (CString strID);
        int GetFirstRSImage (CString& str);
        int GetNextRSImage  (CString& str);
        DWORD LunghezzaImmagine (CString strID);
        int CaricaImmagine (CString strID, char* p);
        int RinominaImmagine (CString strImage, CString strID);
        int GetTableList  (CStringList& slTables);
        int GetFieldsList (CString strTable, CStringList& slFields);
        int GetIndexList  (CString strTable, CStringList& slIndex);
        int GetSubIndexList (CString strTable, CString strIndex, CStringList& slSubIndex);
        int GetFieldType (CString strTable, CString strField, DataTypeEnumX &nType);
		int GetSynList (CStringList& slSyn);
        int CancellaValore (CString strName);
        int SincronizzaVariabili ();
        int AggiungiVariabile (CString strName, CString strColonna, int nRiga, CString strValore, int nTipo);
        int CancellaVariabile (CString strName);
        int ModificaVariabile (CString strName, CString strColonna, int nRiga, CString strValore, int nTipo);
        int ModificaVariabileDB (CString strTable, CString strKeyName, CString strKeyValue, CString strField, CString strValue);
        int CaricaVariabile (CString strName, CString strColonna, int nRiga, int nTipo, CString &strValore);
        int CaricaDB (CString strTable, CString strKeyName, CString strKeyValue, CString strField, CString &strValue);
        bool IsConnectionOpen () { return m_pDb.IsOpen ()!=0; }
        int AddRSRecords (CString strTable, int nType, CPtrArray &data);
        int GetRSRecords (CString strTable, int nType, CStringArray &fields, CPtrArray &data);
        int DeleteRSRecords (CString strTable, int nType);
        int GetInfo ();
		int CreateTable (CString strTable);
		int DeleteTable (CString strTable);
		bool ExistTable (CString strTable);
		_TablePtr FindTable (_CatalogPtr pCatalog, CString strTable);
		_ColumnPtr FindColumn (_TablePtr pTable, CString strColumn);
		int AggiungiUnaColonna (_TablePtr pTable, CString strColumn, int nType);
		int AddGridColumns (CString strTable, stGridColumnData *pgcd, int nColumns);
		int ManageGridColumns (CString strTable, stGridColumnData *pgcd, int nColumns);
		void CheckCompatibilita ();
		int CheckImageColumn (CStringArray& apt);
		CADODatabase *GetDB ();
		CADORecordset *GetRSImages ();
		CADORecordset *GetRSCtrlSyn ();
		CADORecordset *GetRSDB ();
    private :
	    CADORecordset m_pRs;
	    CADORecordset m_pRsImages;
	    CADORecordset m_pRsCtrlSyn;
        CPtrArray     m_pRsArray;
	    CADODatabase  m_pDb;
        CPath         m_strNomeFile;
    private :
        CADODatabase::cadoConnectModeEnum GetMode (int nMode);
        int SeekName (CString str, eResources e);
        int SeekImmagine (CString str);
        int SeekValore (CString str);
        int AddToMap (CString& strHTML, CMapStringToString& mss);
        int GetIndexValue (CString strTable, CString strKeyName, CString strKeyValue, stDBKey *pDBKey, int *nCount);
        int SetIndexValue (CADORecordset &pRs, stDBKey *pDBKey, int nCount, COleSafeArray &sa);
        void SetIndexArray (stDBKey *pDBKey, int nCount, COleSafeArray &sa);
        int CreateTablesFromXML (CString strFile, _CatalogPtr CatalogPtr);
        int CreateTableFromXML (_CatalogPtr CatalogPtr, ElementXmlPtr pEl);
        int AppendFields (_TablePtr pTable, CString strFieldName, COleVariant &vAutoIncrement, COleVariant &vType, COleVariant &vLength, COleVariant &vDefaultValue);
        void AppendTypeField (_TablePtr pTable);
        void AppendSlotField (_TablePtr pTable);
        void AppendTypeSlotIndex (_TablePtr pTable);
        int GetRSIndex ();
        int SeekSlot (int nRS, int nType, int nSlot);
        int AllocRS (CString &strName);
        int FreeRS  (int nRS);
        void AddSingleRecord (int nRS, int nType, int nSlot, CPtrArray &data);
		int ReplaceFormName (_variant_t &v, CString &strHTML, CString &strNew);
};

#endif
