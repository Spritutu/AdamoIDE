/*
** SSCNETParGrid.h
*/

#ifndef __ADAMO_SSCNETPARGRID_GRID__
#define __ADAMO_SSCNETPARGRID_GRID__

/* include files */
#include "gridctrl.h"
#include "ConfigDoc.h"

/* constants definitions */
#define NMAX_SSCNETGRIDDATA 64

/* data definitions */
struct stSSCNETGridData   {
    stSSCNETGridData () : m_nNumber (0), m_bIsEditable (false), m_bIsHex (false)   { }
    int     m_nNumber;
    CString m_strParNumber;
    CString m_strParSymbol;
    CString m_strParUnit;
    CString m_strParName;
    bool    m_bIsEditable;
    bool    m_bIsHex;
};

class CSSCNETParGrid : public CGridCtrl
{
    public :
        CSSCNETParGrid ();
        void InitializeData ();
        void SetGridData (int nDataNumber, CString strParNumber, CString strParSymbol, CString strParUnit, CString strParName, bool bIsEditable, bool bIsHex);
        void InitParData (int nPar);
        void SetConfigDoc (CConfigDoc *pConfigDoc)      { m_pConfigDoc=pConfigDoc; }
		void SetProject (CProject *pPrj)				{ m_pPrj = pPrj; }
        void GetParValue (int nPar, WORD *pw);
        void SetParValue (int nPar, WORD w);
        void FillGridCaption ();
        void GetSelectedParams (WORD *pW, int *nNumPar);
    private :
        stSSCNETGridData m_SSCNETGridData[NMAX_SSCNETGRIDDATA];
        CConfigDoc     *m_pConfigDoc;
		CProject	   *m_pPrj;	
        CImageList      m_ImageList;
        int m_nGD;
        int m_nPar;
    private :
        void OnEditCell (int nRow, int nCol, CPoint point, UINT nChar);
        void OnEndEditCell(int nRow, int nCol, CString str);
};

#endif
