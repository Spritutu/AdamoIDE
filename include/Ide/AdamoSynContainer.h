#pragma once

/* include files */
#include "adamorsobjects.h"
#include "resourcefile.h"

/* constants definitions */
#define GETSYNCONT	CAdamoSynContainer::GetSynContainer

#define IDS_SYNCONT_0	0
#define IDS_SYNCONT_1	1
#define IDS_SYNCONT_2	2
#define IDS_SYNCONT_3	3
#define IDS_SYNCONT_4	4
#define IDS_SYNCONT_5	5
#define IDS_SYNCONT_6	6
#define IDS_SYNCONT_7	7
#define IDS_SYNCONT_8	8
#define IDS_SYNCONT_9	9
#define IDS_SYNCONT_10	10
#define IDS_SYNCONT_11	11
#define IDS_SYNCONT_12	12


/* forward reference */
class CAdamoSynPage;
class CxImage;

class CAdamoSynContainer
{
	public:
		CAdamoSynContainer(void);
		~CAdamoSynContainer(void);
		static CAdamoSynContainer *GetSynContainer ()	{ return m_pThis; }
		int CaricaImmagine (CString str, CBitmap *bmp);
        int CaricaImmagine (CString str, CxImage **ppImage);
		int LoadPage (CString strResource);
		int LoadStringXML (CString strResource, CString strXML);
		void SetParentWnd (CView * pWnd)				{ m_pParentWnd = pWnd; }
		void SetResourceFile (CResourceFile *pRF)		{ m_pRF = pRF; }
		CResourceFile *GetResourceFile ()				{ return m_pRF; }		
		void SetMachine (CAdamoMachine *pMachine)		{ m_pMachine = pMachine; }
		CAdamoMachine *GetMachine ()					{ return m_pMachine; }
		CFont *CreateFont (CString strFont, int nHeight, int nBold);
		void OnDownload ();
		void OnUpdateDownload (CString str, int nRiga, CString strColonna, int nTipo, CString strValue, double fpValue);
		int  RequestRTData (enDispositivi eDev, int nLogAddress, int nData, int nType, CAdamoData &ad);
		int  RequestVarData (CAdamoRSVarControl *rsVar, int nType, CAdamoData &ad);
		int OnTimer ();
		void AddStrings (int nID, CString str);
		void CleanUpStrings ();
		CString GetString (int nID);
        void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart, short yStart, COLORREF cTransparentColor);
	private :
		static CAdamoSynContainer *m_pThis;
		CAdamoSynPage *m_pSynPage;
		CResourceFile *m_pRF;
		CView *m_pParentWnd;
		CAdamoMachine *m_pMachine;
		map<int, CString *> m_mapStrings;
	private :
		int LoadXML (CString strResource, CString& strXML);
		void OnDownloadGrid (CString strTable, CString strVariable);
		void OnDownloadVariable (CString str, int nTipo, CString strValue, double fpValue);
		void OnDownloadArrayElement (CString str, int nRiga, int nTipo, CString strValue, double fpValue);
		void OnDownloadMatrixElement (CString str, int nRiga, CString strColonna, int nTipo, CString strValue, double fpValue);
		void PrepareBufKeys (CPtrList* cl, char** pBufKeys, int* nBufKeysLen);
		void GetColumnList (CString strTable, CResourceFile *pRF, CPtrList* cl, int *nCount);
		int  RequestRTAxesData (stDatiAsse &dd, int nData, CAdamoData &ad);
		int  ConvertData (CAdamoData &dr, int nType, CAdamoData &ad);
		void OnUploadVariable (CString strVar, CAdamoData *dr);
		void OnUploadArrayElement (CString strVar, int nRow, CAdamoData *dr);
		void OnUploadMatrixElement (CString strVar, int nRow, CString strCol, CAdamoData *dr);
};
