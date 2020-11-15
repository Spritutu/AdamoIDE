/*
 * SdoReadGrid.h
 */ 

#ifndef __ADAMO_SDO_READ_GRID__
#define __ADAMO_SDO_READ_GRID__

#include "gridctrl.h"

#define MAXSDOREADSROWS  128
#define MAXSDOREADCOLS   8

class CSdoReadGrid : public CGridCtrl
{
    public :
        CSdoReadGrid ();
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
        CStringArray m_slTipoDiVisualizzazione;
};

#endif
