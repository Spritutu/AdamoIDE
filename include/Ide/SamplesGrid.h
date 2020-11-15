/*
 *
 */ 

#ifndef __ADAMO_SAMPLES_GRID__
#define __ADAMO_SAMPLES_GRID__

#include "gridctrl.h"

/* constants definitions */
#define MAXSAMPLESROWS   128
#define MAXSAMPLESNUMBER   8
#define MAXSAMPLESSCOLS    6
#define RT_AD              1
#define RT_CMP             2
#define RT_AX              3

class CSamplesGrid : public CGridCtrl
{
    public :
        CSamplesGrid ();
        void InitControl ();
        void AddDevice(CAdamoBase* pab);
		void AddVariable (CString strVarName, int nRow, CString strColumnName, int nType);
        int  GetSamples (CPtrList& p);
        int  GetSamplesDevices (CPtrList& p);
        int  DeleteRow ();
        int  GetCount ()  { return m_nCount; }
        void AddAsseFromFile (int nDatoAsse);
        void CloseControl ();
        bool IsModified ()      { return m_bIsModified; }
        void ResetModified ()   { m_bIsModified = false; }
		void EndEdit ();
	    void UpdateRow ();
private :
        CStringArray m_slTipo;
        int m_nCount;
        bool m_bIsModified;
        CString m_strBeforeEditing;
		int m_nRow;
		int m_nCol;
    private :
        void AddAxDevice(CAdamoBase* pab);
        void AddOtherDevice(CAdamoBase* pab);
        void InitStrings ();
        void DestroyStrings ();
        void OnEditCell(int nRow, int nCol, CPoint point, UINT nChar);
	    void OnEndEditCell(int nRow, int nCol, CString str);
		void AddRow ();
		void MoveToggleButton ();
};

#endif
