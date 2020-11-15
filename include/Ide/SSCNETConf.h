#pragma once

/* include files */
#include "stdafx.h"

class CSSCNETConf : public CStatic
{
    public :
        CSSCNETConf();
        void Init (CAdamoHWConfigFile *pCF);
		void SetKinemaAddress (int nKinemaAddress);
        int AddSingleStation ();
        int AddDoubleStation ();
        int AddTripleStation ();
        int DeleteStation ();
        int SaveData ();
    private :
        CBitmap m_bmp[4];
        CBitmap *m_bmpSSCNETStation[NMAXMGASTATION];
        CAdamoHWConfigFile *m_pCF;
        int m_nNumStations;
		int m_nKinemaAddress;
    private :
	    afx_msg void OnPaint();
	    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};
