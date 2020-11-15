#pragma once
#include "afx.h"
#include "adamo.h"

class CPlaneCC   {
    public :
        CPlaneCC ();
        ~CPlaneCC ();
		void SetAsse (int nAsse)   {
			m_nAsse = nAsse;
		}
		void SetXData (int nAsse, double fpOX, double fpLimX, int nPointX)   {
			m_nAssiPiano[0] = nAsse; m_fpOX = fpOX; m_fpLimX = fpLimX; m_nPointX = nPointX; m_fpStepX = fabs ((fpLimX - fpOX) / (nPointX - 1));
		}
		void SetYData (int nAsse, double fpOY, double fpLimY, int nPointY)   {
			m_nAssiPiano[1] = nAsse; m_fpOY = fpOY; m_fpLimY = fpLimY; m_nPointY = nPointY; m_fpStepY = fabs ((fpLimY - fpOY) / (nPointY - 1));
		}
		int  GetAsse ()   {
			return m_nAsse;
		}
		int  GetAsseX ()   {
			return m_nAssiPiano[0];
		}
		int  GetAsseY ()   {
			return m_nAssiPiano[1];
		}
        double GetOX ()   {
            return m_fpOX;
        }
        double GetOY ()   {
            return m_fpOY;
        }
        double GetLimX ()   {
            return m_fpLimX;
        }
        double GetLimY ()   {
            return m_fpLimY;
        }
        int GetPointX ()   {
            return m_nPointX;
        }
        int GetPointY ()   {
            return m_nPointY;
        }
        double GetStepX ()   {
            return m_fpStepX;
        }
        double GetStepY ()   {
            return m_fpStepY;
        }
		void Enable ()					{ m_bEnabled=true; }
		void Disable ()					{ m_bEnabled=false; }
		bool IsEnabled ()				{ return m_bEnabled; }
        void SetValue (int nX, int nY, PointPC& fpValue);
        void GetValue (int nX, int nY, PointPC& fpValue);
		void  SetName (const char *s);
		char *GetName ();
    private :
		char strPC[NMAXSTRING];
		PointPC **m_pData;
		double m_fpOX;
		double m_fpOY;
		double m_fpLimX;
		double m_fpLimY;
		double m_fpStepX;
		double m_fpStepY;
		int    m_nAsse;
		int    m_nAssiPiano[NMAX_APC];
        int    m_nPointX;
        int    m_nPointY;
        bool   m_bEnabled;
};

class CPlaneCCFile : public CStdioFile
{
    public:
        CPlaneCCFile(void);
        void SetAxesList (CPtrList *AxesList)   {
            m_AxesList = AxesList;
        }
        int  Open (const char* szNomeFile, bool bRead);
        void Close ();
        int WriteData ();
        int ReadData ();
        bool GetPCInfo (int nPlane, CmdSetPCInfo &pc);
        bool GetPCData (int nPlane, CmdSetPCData &pd, PointPC *pBuf, int nDim);
        int  GetNumPlanes ()    { return m_nPlanes; }
        CPlaneCC *GetPlaneCC (const char* strArea);
    private :
        CPtrList *m_AxesList;
        CPlaneCC m_PC[NMAX_PLANECC];
        int m_nPlanes;
    private :
        int ExtractStringValue (CString strRead, CString strKey, CString &strValue);
        int ExtractDoubleValue (CString strRead, CString strKey, double &fpValue);
        int ExtractData (CString &strRead, int nPlane, int nRowY);
        int GetAxisLogicAddress (CString strAxes);
        CString GetAxisName (int nLogicAddress);
};
