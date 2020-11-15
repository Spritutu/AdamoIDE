/*
 *
 */ 

#ifndef __ADAMO_TRIGGER_GRID__
#define __ADAMO_TRIGGER_GRID__

#include "gridctrl.h"

/* constants definitions */
#define MAXTRIGGERSROWS  128
#define MAXTRIGGERSNUMBER  4
#define MAXTRIGGERSCOLS    5
#define RT_AD              1
#define RT_CMP             2
#define RT_AX              3

class CTriggerGrid : public CGridCtrl
{
    public :
        CTriggerGrid ();
        void AddAD (CAdamoBase* pab);
        void AddCompare (CAdamoBase* pab);
        void AddAsse (CAdamoBase* pab);
        virtual void OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);
        virtual void OnEndEditCell(int nRow, int nCol, CString str);
        void InitControl ();
        void CloseControl ();
        int  GetTriggers (CPtrList& p);
        int  DeleteRow (); 
        int  GetCount ()   { return m_nCount; }
        void AddADFromFile (int nValue);
        void AddAsseFromFile (int nDatoAsse, int nCmp, int nValue, double fpValue);
        void AddCompareFromFile (int nCmp, double fpValue);
        bool IsModified ()      { return m_bIsModified; }
        void ResetModified ()   { m_bIsModified = false; }
    private :
        int m_nCount;
        int m_nRowType[MAXTRIGGERSNUMBER];
        CStringArray m_slTipo;
        CStringArray m_slAzione;
        CStringArray m_slConfrontoDig;
        CStringArray m_slConfrontoDir;
        CStringArray m_slConfrontoAssi;
        CString m_strBeforeEditing;
        bool    m_bIsModified;
    private :
        bool GestisciAssi (int nRow, int nCol);
        int  GetStatusCode (const char* str);
        int  GetCompare (const char* str);
        int  GetDirectionCode (const char* str);
        int  GetDoubleData (const char* str, double* fp);
        int  GetActiveCode (const char* str);
        void InitStrings ();
        void DestroyStrings ();
};

#endif
