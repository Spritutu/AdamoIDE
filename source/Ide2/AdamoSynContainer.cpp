#include "stdafx.h"
#include "XImage.h"
#include "AdamoSynContainer.h"
#include "AdamoSynPage.h"

#ifdef ADAMO_KOS
    extern void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart, short yStart, COLORREF cTransparentColor);
#endif

CAdamoSynContainer *CAdamoSynContainer::m_pThis = NULL;

/* construction / destruction */
CAdamoSynContainer::CAdamoSynContainer(void) : m_pSynPage (NULL), m_pRF (NULL), m_pParentWnd (NULL), m_pMachine (NULL)
{
	m_pThis = this;
}

CAdamoSynContainer::~CAdamoSynContainer(void)
{
	_delete (m_pSynPage);
	CleanUpStrings ();
}

/*
** CaricaImmagine :
*/
int CAdamoSynContainer::CaricaImmagine (CString str, CBitmap *bmp)
{
    CxImage* pImage=NULL;
    DWORD dwLength; 
	int nR = -1;
    char* p;

    if (m_pRF->EsisteImmagine (str))   {
        /* carichiamo la lunghezza del chunk da caricare */
        if ((dwLength=m_pRF->LunghezzaImmagine (str))>0)   {
            p = new char [dwLength];
            m_pRF->CaricaImmagine (str, p);
            /* creiamo l'immagine */
            pImage=new CxImage ((BYTE*)p, dwLength, CXIMAGE_FORMAT_JPG);
            delete p;
			/* convertiamolo in bitmap */
			bmp->Attach (pImage->MakeBitmap ());
			delete pImage;
			nR = 0;
        }
    }
    return nR;
}

/*
** CaricaImmagine :
*/
int CAdamoSynContainer::CaricaImmagine (CString str, CxImage **ppImage)
{
    DWORD dwLength; 
	int nR = -1;
    char* p;

    if (m_pRF->EsisteImmagine (str))   {
        /* carichiamo la lunghezza del chunk da caricare */
        if ((dwLength=m_pRF->LunghezzaImmagine (str))>0)   {
            p = new char [dwLength];
            m_pRF->CaricaImmagine (str, p);
            /* creiamo l'immagine */
            *ppImage=new CxImage ((BYTE*)p, dwLength, CXIMAGE_FORMAT_JPG);
            delete p;
			nR = 0;
        }
    }
    return nR;
}

/*
** LoadStringXML :
*/
int CAdamoSynContainer::LoadStringXML (CString strResource, CString strXML)
{
	CRect rc;
	int nR = -1;
	_delete (m_pSynPage);
	m_pSynPage = new CAdamoSynPage;
	if (m_pSynPage->Create (NULL, NULL, WS_CHILD | WS_VISIBLE, rc, m_pParentWnd, NULL))   {
		nR = m_pSynPage->Load (strXML);
		m_pSynPage->Resize ();
		m_pSynPage->SetName (strResource);
		m_pSynPage->OnLoad ();
	}
	return nR;
}

/*
** LoadPage :
*/
int CAdamoSynContainer::LoadPage (CString strResource)
{
	CString strXML;

	if (!LoadXML (strResource, strXML))
		LoadStringXML (strResource, strXML);
	return 0;
}

/*
** LoadXML :
*/
int CAdamoSynContainer::LoadXML (CString strResource, CString& strXML)
{
    int nR=-1;
	bool bIsMainSyn;

    /* recuperiamo per prima cosa il record con l'xml */
    if (!m_pRF->GetXML (strResource, eDialogs, strXML, bIsMainSyn))
		nR = 0;
	return nR;
}

/*
** CreateFont :
*/
CFont *CAdamoSynContainer::CreateFont (CString strFont, int nHeight, int nBold)
{
    CFont* pFont = new CFont;
    pFont->CreateFont(
         nHeight,
         0,
         0,
         0,
         nBold,
	     FALSE,
         FALSE,
         0,
         ANSI_CHARSET,
	     OUT_DEFAULT_PRECIS,
	     CLIP_DEFAULT_PRECIS,
	     DEFAULT_QUALITY,
	     DEFAULT_PITCH, _T(strFont));
    return pFont;
}

/*
** OnUpdateDownload :
*/
void CAdamoSynContainer::OnUpdateDownload (CString str, int nRiga, CString strColonna, int nTipo, CString strValue, double fpValue)
{
	if (nRiga == -1 && strColonna == "")
		OnDownloadVariable (str, nTipo, strValue, fpValue);
	else
	if (nRiga != -1 && strColonna == "")
		OnDownloadArrayElement (str, nRiga, nTipo, strValue, fpValue);
	else
	if (nRiga != -1 && strColonna != -1)
		OnDownloadMatrixElement (str, nRiga, strColonna, nTipo, strValue, fpValue);
}

/*
** OnDownload :
*/
void CAdamoSynContainer::OnDownload ()
{
	CADORecordset *pRS = m_pRF->GetRSCtrlSyn ();
	CString str, strValue, strColonna;
	double fpValue;
	int nTipo, nTipoControllo, nRiga;
	
	if (pRS->GetRecordCount ())   {
		pRS->MoveFirst ();
		while (!pRS->IsEof())   {
			pRS->GetFieldValue ("TipoControllo", nTipoControllo);
			pRS->GetFieldValue ("NomeVariabile", str);
			pRS->GetFieldValue ("Riga", nRiga);
			pRS->GetFieldValue ("Colonna", strColonna);
			pRS->GetFieldValue ("ValoreStringa", strValue);
			pRS->GetFieldValue ("ValoreDouble", fpValue);
			pRS->GetFieldValue ("Tipo", nTipo);
			if (nTipoControllo == RSGrid)
				OnDownloadGrid (strValue, str);
			else
			if (nRiga == -1 && strColonna == "")
				OnDownloadVariable (str, nTipo, strValue, fpValue);
			else
			if (nRiga != -1 && strColonna == "")
				OnDownloadArrayElement (str, nRiga, nTipo, strValue, fpValue);
			else
			if (nRiga != -1 && strColonna != -1)
				OnDownloadMatrixElement (str, nRiga, strColonna, nTipo, strValue, fpValue);
			pRS->MoveNext ();
		}
	}
}

/*
** OnDownloadMatrix :
*/
void CAdamoSynContainer::OnDownloadGrid (CString strTable, CString strVariable)
{
	CADORecordset pRSGrid;
	CResourceFile *pRF = GetResourceFile ();
	CAdamoMachine *pMachine = GetMachine ();
	CPtrList cl;
	CString strValue;
	double fpValue;
	int i, nCount, nKeys, nRiga;
	char *bufKeys;
	BOOL bValue;

	if (pRF)   {
		CADODatabase *pDB = pRF->GetDB ();
        pRSGrid = CADORecordset(pDB);
		if (pRF->ExistTable (strTable))   {
			if (pRSGrid.Open (strTable, CADORecordset::openTable))   {
				if (pRSGrid.GetRecordCount ())   {
					GetColumnList (strTable, pRF, &cl, &nCount);
					PrepareBufKeys (&cl, &bufKeys, &nKeys);
					pRSGrid.MoveFirst ();
					CAdamoData *pData = new CAdamoData [nCount];
					while (!pRSGrid.IsEof())    {
						pRSGrid.GetFieldValue ("Riga", nRiga);
						i = 0;
						for (POSITION pos = cl.GetHeadPosition (); pos; i++)   {
							stGridColumnData *pcd = (stGridColumnData *) cl.GetNext (pos);
							pData[i].nType = pcd->m_nType;
							switch (pcd->m_nType)   {
								case LUA_TNUMBER :
									pRSGrid.GetFieldValue (pcd->m_strName, fpValue);
									pData[i].AdamoData.fp = fpValue;
									break;
								case LUA_TSTRING :
									pRSGrid.GetFieldValue (pcd->m_strName, strValue);
									strcpy (pData[i].AdamoData.str, strValue);
									break;
								case LUA_TBOOLEAN :
									pRSGrid.GetFieldValue (pcd->m_strName, bValue);
									pData[i].AdamoData.b = bValue;
									break;
							}
						}
						pMachine->SetMatrix (strVariable, nRiga, -nCount, nKeys, bufKeys, pData);
						pRSGrid.MoveNext ();
					}
					/* seghiamo la memoria che non serve piu' */
					delete pData;
					delete bufKeys;
					for (POSITION pos = cl.GetHeadPosition (); pos; )   {
						stGridColumnData *pcd = (stGridColumnData *) cl.GetNext (pos);
						delete pcd;
					}
					cl.RemoveAll ();
				}
				pRSGrid.Close ();
			}
		}
	}
}

/*
** OnDownloadVariable :
*/
void CAdamoSynContainer::OnDownloadVariable (CString str, int nTipo, CString strValue, double fpValue)
{
	CAdamoMachine *pMachine = GetMachine ();
	COleVariant v;
	if (pMachine)   {
		switch (nTipo)   {
			case LUA_TNUMBER :
				v = fpValue;
				break;
			case LUA_TSTRING :
				v = strValue;
				break;
			case LUA_TBOOLEAN :
				v.vt = VT_BOOL;
				v.boolVal = (short int) fpValue;
				break;
		}
		pMachine->SetVariable (str, v);
	}
}

/*
** OnDownloadArrayElement :
*/
void CAdamoSynContainer::OnDownloadArrayElement (CString str, int nRiga, int nTipo, CString strValue, double fpValue)
{
	CAdamoMachine *pMachine = GetMachine ();
    CAdamoData stData;

	if (pMachine)   {
		stData.nType = nTipo;
		switch (stData.nType)   {
			case LUA_TNUMBER :
				stData.AdamoData.fp = fpValue;
				break;
			case LUA_TSTRING :
				strcpy (stData.AdamoData.str, strValue);
				break;
			case LUA_TBOOLEAN :
				stData.AdamoData.b = fpValue != 0.0;
				break;
		}
		pMachine->SetArray (str, nRiga, nRiga, &stData);
	}
}

/*
** OnDownloadMatrixElement :
*/
void CAdamoSynContainer::OnDownloadMatrixElement (CString str, int nRiga, CString strColonna, int nTipo, CString strValue, double fpValue)
{
	CAdamoMachine *pMachine = GetMachine ();
    CAdamoData stData;
	char szBuffer[256];

	if (pMachine)   {
		stData.nType = nTipo;
		switch (stData.nType)   {
			case LUA_TNUMBER :
				stData.AdamoData.fp = fpValue;
				break;
			case LUA_TSTRING :
				strcpy (stData.AdamoData.str, strValue);
				break;
			case LUA_TBOOLEAN :
				stData.AdamoData.b = fpValue != 0.0;
				break;
		}
		strcpy (szBuffer+2*sizeof(int), strColonna);
		pMachine->SetMatrix (str, nRiga, nRiga, 1, szBuffer, &stData);
	}
}

/*
** PrepareBufKeys :
*/
void CAdamoSynContainer::PrepareBufKeys (CPtrList* cl, char** pBufKeys, int* nBufKeysLen)
{
    int nDim=0, i=0;
    
	for (POSITION pos = cl->GetHeadPosition (); pos; i++)   {
		stGridColumnData *pcd = (stGridColumnData *) cl->GetNext (pos);
        nDim+=strlen (pcd->m_strName)+1;
	}
    nDim+=2*sizeof(int);
    *pBufKeys=new char [nDim];
    char* p=*pBufKeys;
    *((int*)p)=nDim-2*sizeof(int);
    p+=sizeof (int);
    *((int*)p)=i;
    p+=sizeof (int);
	for (POSITION pos = cl->GetHeadPosition (), i=0; pos; i++)   {
		stGridColumnData *pcd = (stGridColumnData *) cl->GetNext (pos);
        strcpy (p, pcd->m_strName);
        p+=strlen (p)+1;
    }
    *nBufKeysLen=nDim;
}

/*
** GetColumnList :
*/
void CAdamoSynContainer::GetColumnList (CString strTable, CResourceFile *pRF, CPtrList* cl, int *nCount)
{
	CADODatabase *pDB = pRF->GetDB ();
    _CatalogPtr pCatalog=NULL;
    _TablePtr   pTable=NULL;
	_ColumnPtr  pColumn=NULL;
    _variant_t v;
	long lCount;
    int nTableCount, nColumnCount, i=0, n;

	pCatalog.CreateInstance(__uuidof(Catalog));
	if (pCatalog)   {
		pCatalog->put_ActiveConnection (_variant_t((IDispatch*)pDB->m_pConnection, TRUE));
		nTableCount=pCatalog->Tables->Count;
        while (i < nTableCount)   {
			lCount = i;
			pTable=pCatalog->Tables->GetItem (lCount);
			if (CString ((char *)pTable->GetName ()) == strTable)   {
				nColumnCount=pTable->Columns->Count;
				n = 0; *nCount = 0;
				while (n < nColumnCount)   {
					lCount = n;
					pColumn=pTable->Columns->GetItem (lCount);
					if (CString ((char *)pColumn->GetName ()) != "Riga" && CString ((char *)pColumn->GetName ()) != "ID")   {
						stGridColumnData *pgd = new stGridColumnData;
						pgd->m_strName = CString ((char *)pColumn->GetName ());
						switch (pColumn->GetType ())   {
							case DataTypeEnumX::adDouble :
								pgd->m_nType = LUA_TNUMBER;
								break;
							case DataTypeEnumX::adVarWChar :
								pgd->m_nType = LUA_TSTRING;
								break;
							case DataTypeEnumX::adBoolean :
								pgd->m_nType = LUA_TBOOLEAN;
								break;
						}
						cl->AddTail (pgd);
						(*nCount)++;	
					}
					n++;
				}
				break;
			}
			i++;
		}
		pCatalog.Release ();
	}
}

/*
** OnTimer :
*/
int CAdamoSynContainer::OnTimer ()
{
	if (m_pSynPage)
		m_pSynPage->OnTimer ();
	return 0;
}

/*
** RequestRTData :
*/
int CAdamoSynContainer::RequestRTData (enDispositivi eDev, int nLogAddress, int nData, int nType, CAdamoData &ad)
{
    struct stDatiAsse da;
	CAdamoData dr;
	CDeviceData* dd=new CDeviceData;
    switch (eDev)   {
        case enAsse :
			dd->m_eDev=enAsse;
			dd->m_nAddress=nLogAddress;
			if (m_pMachine->GetAxes (1, dd, &da) == S_OK)
			    RequestRTAxesData (da, nData, dr);
            else   {
                switch (nType)   {
                    dr.nType = nType;
                    case LUA_TBOOLEAN :
                        dr.AdamoData.b = false;
                        break;
                    case LUA_TNUMBER :
                        dr.AdamoData.fp = 0.0;
                        break;
                    case LUA_TSTRING :
                        dr.AdamoData.str[0] = '\0';
                        break;
                }
            }
			break;
        case enInputDigitale :
			dd->m_eDev = enInputDigitale;
			dd->m_nAddress = nLogAddress;
			dr.nType = LUA_TBOOLEAN;
            if (m_pMachine->GetDevice (1, dd) == S_OK)
			    dr.AdamoData.b = dd->m_nDati;
            else
                dr.AdamoData.b = false;
            break;
        case enOutputDigitale :
			dd->m_eDev = enOutputDigitale;
			dd->m_nAddress = nLogAddress;
			dr.nType = LUA_TBOOLEAN;
            if (m_pMachine->GetDevice (1, dd) == S_OK)
			    dr.AdamoData.b = dd->m_nDati;
            else
                dr.AdamoData.b = false;
            break;
        case enInputAnalogico :
			dd->m_eDev = enInputAnalogico;
			dd->m_nAddress = nLogAddress;
			dr.nType = LUA_TNUMBER;
            if (m_pMachine->GetDevice (1, dd) == S_OK)
			    dr.AdamoData.fp = dd->m_fpDati;
            else
                dr.AdamoData.fp = 0.0;
            break;
        case enOutputAnalogico :
			dd->m_eDev = enOutputAnalogico;
			dd->m_nAddress = nLogAddress;
			dr.nType = LUA_TNUMBER;
            if (m_pMachine->GetDevice (1, dd) == S_OK)
			    dr.AdamoData.fp = dd->m_fpDati;
            else
                dr.AdamoData.fp = 0.0;
            break;
        case enTimer :
			dd->m_eDev = enTimer;
			dd->m_nAddress = nLogAddress;
			dr.nType = LUA_TNUMBER;
            if (m_pMachine->GetDevice (1, dd) == S_OK)
			    dr.AdamoData.fp = dd->m_fpDati; 
            else
                dr.AdamoData.fp = 0.0;
			break;
		case enCounter :
			dd->m_eDev = enCounter;
			dd->m_nAddress = nLogAddress;
            if (m_pMachine->GetDevice (1, dd) == S_OK)   {
			    dr.nType = dd->m_nType;
			    switch (dd->m_nType)   {
				    case LUA_TBOOLEAN :
					    dr.AdamoData.b = dd->m_nDati;
					    break;
				    case LUA_TNUMBER :
					    dr.AdamoData.fp = dd->m_fpDati;
					    break;
				    case LUA_TSTRING :
					    strcpy (dr.AdamoData.str, dd->m_strDati);
					    break;
			    }
            }
            else   {
                switch (nType)   {
                    dr.nType = nType;
                    case LUA_TBOOLEAN :
                        dr.AdamoData.b = false;
                        break;
                    case LUA_TNUMBER :
                        dr.AdamoData.fp = 0.0;
                        break;
                    case LUA_TSTRING :
                        dr.AdamoData.str[0] = '\0';
                        break;
                }
            }
            break;
	}
	ConvertData (dr, nType, ad);
	delete dd;
	return 0;
}

/*
** RequestRTAxesData
*/
int CAdamoSynContainer::RequestRTAxesData (stDatiAsse &dd, int nData, CAdamoData &dr)
{
	switch (nData)   {
		case AX_ST :
			dr.nType = LUA_TBOOLEAN;
			dr.AdamoData.b = dd.nStato;
			break;
		case AX_DIR :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.nDirezione;
			break;
		case AX_QENC :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.fpQuotaEncoder;
			break;
		case AX_QR :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.fpQuotaReale;
			break;
		case AX_QT :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.fpQuotaTeorica;
			break;
		case AX_EA :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.fpErroreAnello;
			break;
		case AX_VC :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = fabs (dd.fpVelAttuale);
			break;
		case AX_VCR :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = fabs (dd.fpVelReale);
			break;
		case AX_ACCINST :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = fabs (dd.fpAccInstInt);
			break;
		case AX_ACCINSTR :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = fabs (dd.fpAccReale);
			break;
		case AX_JERKINST :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = fabs (dd.fpJerkInstInt);
			break;
		case AX_JERKINSTR :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = fabs (dd.fpJerkReale);
			break;
		case AX_FF :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.nFeedForward;
			break;
		case AX_FFA :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.nFeedForwardA;
			break;
		case AX_P :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.nProporzionale;
			break;
		case AX_I :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.nIntegrativa;
			break;
		case AX_D :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.nDerivativa;
			break;
		case AX_PG :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.nProporzionaleGantry;
			break;
		case AX_IG :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.nIntegrativaGantry;
			break;
		case AX_DG :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.nDerivativaGantry;
			break;
		case AX_TORQUE :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = dd.fpTorqueReale;
			break;
		default :
			dr.nType = LUA_TNUMBER;
			dr.AdamoData.fp = 0.0;
			break;
	}
	return 0;
}

/*
** ConvertData :
*/
int CAdamoSynContainer::ConvertData (CAdamoData &dr, int nType, CAdamoData &ad)
{
	switch (dr.nType)   {
		case LUA_TBOOLEAN :
			switch (nType)   {
				case LUA_TBOOLEAN :
					ad.nType = LUA_TBOOLEAN;
					ad.AdamoData.b = dr.AdamoData.b;
					break;
				case LUA_TNUMBER :
					ad.nType = LUA_TNUMBER;
					ad.AdamoData.fp = dr.AdamoData.b ? 1.0 : 0.0;
					break;
				case LUA_TSTRING :
					ad.nType = LUA_TSTRING;
					if (dr.AdamoData.b)
						strcpy (ad.AdamoData.str, GETSYNCONT()->GetString (IDS_SYNCONT_11));
					else
						strcpy (ad.AdamoData.str, GETSYNCONT()->GetString (IDS_SYNCONT_12));
					break;
			}
			break;
		case LUA_TNUMBER :
			switch (nType)   {
				case LUA_TBOOLEAN :
					ad.nType = LUA_TBOOLEAN;
					ad.AdamoData.b = dr.AdamoData.fp == 0.0 ? false : true;
					break;
				case LUA_TNUMBER :
					ad.nType = LUA_TNUMBER;
					ad.AdamoData.fp = dr.AdamoData.fp;
					break;
				case LUA_TSTRING :
					ad.nType = LUA_TSTRING;
					sprintf (ad.AdamoData.str, "%.2f", dr.AdamoData.fp);
					break;
			}
			break;
		case LUA_TSTRING :
			switch (nType)   {
				case LUA_TBOOLEAN :
					ad.nType = LUA_TBOOLEAN;
					ad.AdamoData.b = dr.AdamoData.str[0] == '\0' ? false : true;
					break;
				case LUA_TNUMBER :
					ad.nType = LUA_TNUMBER;
					ad.AdamoData.fp = strtod (dr.AdamoData.str, NULL);
					break;
				case LUA_TSTRING :
					ad.nType = LUA_TSTRING;
					strcpy (ad.AdamoData.str, dr.AdamoData.str);
					break;
			}
			break;
	};
	return 0;
}

/*
** AddStrings :
*/
void CAdamoSynContainer::AddStrings (int nID, CString str)
{
	CString *s = new CString (str);
	m_mapStrings.insert (map<int, CString *>::value_type(nID, s));
}

/*
** CleanUpStrings :
*/
void CAdamoSynContainer::CleanUpStrings ()
{
	CString* pS=NULL;
    map<int, CString *>::iterator it;
    
    it=m_mapStrings.begin();
    while (it!=m_mapStrings.end())   {
        pS=(*it).second;
		delete pS;
		it++;
    }
	m_mapStrings.clear ();
}

/*
** CleanUpStrings :
*/
CString CAdamoSynContainer::GetString (int nID)
{
    map<int, CString *>::iterator it;
	CString *pS = NULL;

	it=m_mapStrings.find (nID);
	if (it!=m_mapStrings.end())
		pS = (*it).second;
	return pS == NULL ? "" : *pS;
}

/*
** RequestVarData :
*/
int CAdamoSynContainer::RequestVarData (CAdamoRSVarControl *rsVar, int nType, CAdamoData &ad)
{
	CAdamoData dr;

	if (rsVar->GetObjVar ().m_nRow == -1 && rsVar->GetObjVar ().m_strCol == "")
		OnUploadVariable (rsVar->GetObjVar ().m_strName, &dr);
	else
	if (rsVar->GetObjVar ().m_nRow != -1 && rsVar->GetObjVar ().m_strCol == "")
		OnUploadArrayElement (rsVar->GetObjVar ().m_strName, rsVar->GetObjVar ().m_nRow, &dr);
	else
	if (rsVar->GetObjVar ().m_nRow != -1 && rsVar->GetObjVar ().m_strCol != -1)
		OnUploadMatrixElement (rsVar->GetObjVar ().m_strName, rsVar->GetObjVar ().m_nRow, rsVar->GetObjVar ().m_strCol, &dr);
	ConvertData (dr, nType, ad);
	return 0;
}

/*
** OnUploadVariable :
*/
void CAdamoSynContainer::OnUploadVariable (CString strVar, CAdamoData *dr)
{
	COleVariant v;

    if (m_pMachine->GetVariable (strVar, v) == S_OK)   {
        switch (v.vt)   {
            case VT_I4 :
				dr->nType = LUA_TBOOLEAN;
				dr->AdamoData.b = v.lVal ? true : false;
                break;
            case VT_R8 :
				dr->nType = LUA_TNUMBER;
				dr->AdamoData.fp = v.dblVal;
                break;
            case VT_BSTR :
				dr->nType = LUA_TSTRING;
				strcpy (dr->AdamoData.str, CString (v.bstrVal));
                break;
        }
	}
}

/*
** OnUploadVariable :
*/
void CAdamoSynContainer::OnUploadArrayElement (CString strVar, int nRow, CAdamoData *dr)
{
    if (m_pMachine->GetArray (strVar, nRow, nRow, dr) == S_FALSE)   {
        dr->nType = LUA_TNIL; dr->AdamoData.fp = 0.0;
    }
}

/*
** OnUploadVariable :
*/
void CAdamoSynContainer::OnUploadMatrixElement (CString strVar, int nRow, CString strCol, CAdamoData *dr)
{
    if (m_pMachine->GetMatrixElement (strVar, nRow, strCol, dr) == S_FALSE)   {
        dr->nType = LUA_TNIL; dr->AdamoData.fp = 0.0;
    }
}

/*
** DrawTransparentBitmap :
*/
void CAdamoSynContainer::DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart, short yStart, COLORREF cTransparentColor)
{
#ifdef ADAMO_KOS
    ::DrawTransparentBitmap (hdc, hBitmap, xStart, yStart, cTransparentColor);
#else
    ((CMainFrame *)AfxGetMainWnd ())->DrawTransparentBitmap (hdc, hBitmap, xStart, yStart, cTransparentColor);
#endif
}
