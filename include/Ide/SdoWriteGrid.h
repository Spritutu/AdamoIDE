/*
 * SdoReadGrid.h
 */ 

#ifndef __ADAMO_SDO_WRITE_GRID__
#define __ADAMO_SDO_WRITE_GRID__

#include "gridctrl.h"

#define MAXSDOWRITESROWS  128
#define MAXSDOWRITECOLS   7

class CSdoWriteGrid : public CGridCtrl
{
    public :
        CSdoWriteGrid ();
        void InitControl ();
        void CloseControl ();
        void SetColumnSize ();
    private :
        void InitColumnCaptions ();
        void InitGrid ();
        void InitStrings ();
        void InitGridLine (int n);
        void CloseStrings ();
        void OnEditCell (int nRow, int nCol, CPoint point, UINT nChar);
    private :
        CImageList m_ImageList;
        CStringArray m_slCanaleCan;
        CStringArray m_slRicevitore;
        CStringArray m_slTipoDato;
};

#endif
