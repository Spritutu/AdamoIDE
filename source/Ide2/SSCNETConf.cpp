// SSCNETConf.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "MainFrame.h"
#include "SSCNETConf.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(CSSCNETConf, CStatic)
	//{{AFX_MSG_MAP(CLabel)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
    ON_WM_DESTROY() 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSSCNETConf::CSSCNETConf () : m_pCF (NULL), m_nNumStations (0)
{
    
}

void CSSCNETConf::Init (CAdamoHWConfigFile *pCF)
{
    m_bmp[0].LoadBitmap (IDB_BMP_SSCNETCONF_EMPTY);
    m_bmp[1].LoadBitmap (IDB_BMP_SSCNETCONF_SINGLE);
    m_bmp[2].LoadBitmap (IDB_BMP_SSCNETCONF_DOUBLE);
    m_bmp[3].LoadBitmap (IDB_BMP_SSCNETCONF_TRIPLE);
    m_pCF = pCF;
    for (int i = 0; i<NMAXMGASTATION; i++)   {
        switch (m_pCF->GetStationConf (m_nKinemaAddress, i))   {
            case 0 :
                m_bmpSSCNETStation[i] = &m_bmp[0]; break;
            case 1 :
                m_bmpSSCNETStation[i] = &m_bmp[1]; m_nNumStations++; break;
            case 3 :
                m_bmpSSCNETStation[i] = &m_bmp[2]; m_nNumStations++; break;
            case 7 :
                m_bmpSSCNETStation[i] = &m_bmp[3]; m_nNumStations++; break;
        }
    }
}

void CSSCNETConf::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
    int cx, cy;

    for (int i = 0; i<NMAXMGASTATION; i++)   {
        cx = (i % 10) * 64;
        cy = (i / 10) * 64;
        ((CMainFrame *)AfxGetMainWnd ())->DrawBitmap (dc.GetSafeHdc (), (HBITMAP)m_bmpSSCNETStation[i]->GetSafeHandle (), cx, cy);
    }
}

BOOL CSSCNETConf::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}

void CSSCNETConf::OnDestroy()
{
    for (int i = 0; i<4; i++)
         DeleteObject (m_bmp[i]);
}

int CSSCNETConf::AddSingleStation ()
{
    int nB = -1;
    if (m_nNumStations<NMAXMGASTATION)  {
        m_bmpSSCNETStation[m_nNumStations] = &m_bmp[1];
        m_nNumStations++;
        Invalidate ();
        nB = 0;
    }
    return nB;
}

int CSSCNETConf::AddDoubleStation ()
{
    int nB = -1;
    if (m_nNumStations<NMAXMGASTATION)  {
        m_bmpSSCNETStation[m_nNumStations] = &m_bmp[2];
        m_nNumStations++;
        Invalidate ();
        nB = 0;
    }
    return nB;
}

int CSSCNETConf::AddTripleStation ()
{
    int nB = -1;
    if (m_nNumStations<NMAXMGASTATION)  {
        m_bmpSSCNETStation[m_nNumStations] = &m_bmp[3];
        m_nNumStations++;
        Invalidate ();
        nB = 0;
    }
    return nB;
}

int CSSCNETConf::DeleteStation ()
{
    int nB = -1;
    if (m_nNumStations)  {
        m_nNumStations--;
        m_bmpSSCNETStation[m_nNumStations] = &m_bmp[0];
        Invalidate ();
        nB = 0;
    }
    return nB;
}

int CSSCNETConf::SaveData ()
{
    BYTE conf;

    for (int i = 0; i<NMAXMGASTATION; i++)   {
        conf = 0;
        if (m_bmpSSCNETStation[i] == &m_bmp[0])
            m_pCF->SetStationConf (m_nKinemaAddress, i, conf);
        else
        if (m_bmpSSCNETStation[i] == &m_bmp[1])   {
            conf = 0x01;
            m_pCF->SetStationConf (m_nKinemaAddress, i, conf);
        }
        else
        if (m_bmpSSCNETStation[i] == &m_bmp[2])   {
            conf = 0x03;
            m_pCF->SetStationConf (m_nKinemaAddress, i, conf);
        }
        else 
        if (m_bmpSSCNETStation[i] == &m_bmp[3])   {
            conf = 0x07;
            m_pCF->SetStationConf (m_nKinemaAddress, i, conf);
        }
        else
            m_pCF->SetStationConf (m_nKinemaAddress, i, 0x00);
    }
    return 0;
}

void CSSCNETConf::SetKinemaAddress (int nKinemaAddress)
{
	m_nKinemaAddress = nKinemaAddress;
}
