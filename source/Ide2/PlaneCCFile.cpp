#include "stdafx.h"
#include "Adamo.h"
#include "AdamoBase.h"
#include "AdamoAxis.h"
#include "PlaneCCFile.h"

/* construction / destruction */
CPlaneCC::CPlaneCC(void) : m_pData (NULL)
{
    strPC[0] = '\0';
}

CPlaneCC::~CPlaneCC(void)
{
	for (int i = 0; i<m_nPointX; i++)
		_delete (m_pData[i]);
	_delete (m_pData);
}

CPlaneCCFile::CPlaneCCFile () : m_AxesList (NULL), m_nPlanes (0)
{
}

/*
** Open :
*/
int CPlaneCCFile::Open (const char* szNomeFile, bool bRead)
{
    int nB=0;

    if (_access (szNomeFile, 00))   {
        if (!bRead)
            nB=CStdioFile::Open (szNomeFile, CFile::modeCreate | CFile::modeWrite);
    }
    else    {
        if (bRead==true)
            nB=CStdioFile::Open (szNomeFile, CFile::modeRead);
        else 
            nB=CStdioFile::Open (szNomeFile, CFile::modeReadWrite);
    }
    return nB;
}

/*
** Close :
*/
void CPlaneCCFile::Close ()
{
    __super::Close ();
}

/*
** WriteData :
*/
int CPlaneCCFile::WriteData ()
{
    CString strWrite, strData;
    PointPC pp;
    int n;

    /* scriviamo per prima cosa il numero di piani */
    strWrite.Format ("PLANES NUMBER : %d\n", m_nPlanes);
    WriteString (strWrite);
    n = 0;
    while (n < m_nPlanes)   {
        /* interlinea */
        WriteString ("");
        /* nome del piano */
        strWrite.Format ("PLANE NAME : %s\n", m_PC[n].GetName ());
        WriteString (strWrite);
        /* nome dell' asse da correggere */
        strWrite.Format ("AXIS NAME : %s\n", GetAxisName (m_PC[n].GetAsse ()));
        WriteString (strWrite);
        /* nome dell' asse X del piano */
        strWrite.Format ("AXIS NAME X : %s\n", GetAxisName (m_PC[n].GetAsseX ()));
        WriteString (strWrite);
        /* nome dell' asse Y del piano */
        strWrite.Format ("AXIS NAME Y : %s\n", GetAxisName (m_PC[n].GetAsseY ()));
        WriteString (strWrite);
        /* Origine X */
        strWrite.Format ("ORIGIN X : %.3f\n", m_PC[n].GetOX ());
        WriteString (strWrite);
        /* Origine Y */
        strWrite.Format ("ORIGIN Y : %.3f\n", m_PC[n].GetOY ());
        WriteString (strWrite);
        /* Limite X */
        strWrite.Format ("LIMIT X : %.3f\n", m_PC[n].GetLimX ());
        WriteString (strWrite);
        /* Limite Y */
        strWrite.Format ("LIMIT Y : %.3f\n", m_PC[n].GetLimY ());
        WriteString (strWrite);
        /* Points X */
        strWrite.Format ("POINTS X : %d\n", m_PC[n].GetPointX ());
        WriteString (strWrite);
        /* Points Y */
        strWrite.Format ("POINTS Y : %d\n", m_PC[n].GetPointY ());
        WriteString (strWrite);
        /* Abilitato / Disabilitato */
        strWrite.Format ("ENABLED : %d\n", m_PC[n].IsEnabled ());
        WriteString (strWrite);
        /* ora i valori */
        for (int s = 0; s<m_PC[n].GetPointY (); s++)   {
            strWrite = "";
            for (int r = 0; r<m_PC[n].GetPointX (); r++)   {
                m_PC[n].GetValue (r, s, pp);
                strData.Format ("%.3f", pp.z);
                if (r < m_PC[n].GetPointX () - 1)
                    strData+=", ";
                strWrite += strData;
            }
            strWrite += "\n";
            WriteString (strWrite);
        }
        n++;
    }
    return 0;
}

/*
** ReadData :
*/
int CPlaneCCFile::ReadData ()
{
    CString strRead, strValue, strPlane;
    int nX, nY, nZ;
    int nRowY, n;
    double fpOX, fpOY;
    double fpLimX, fpLimY;
    double fpPointsX, fpPointsY;
    double fpEnabled;
    double fpNumPlanes;

	/* se  il file e' appena stato creato, va bene lo stesso */
	if (GetLength () == 0)
		return 0;
    /* numero di piani */
    ReadString (strRead);
    if (ExtractDoubleValue (strRead, "PLANES NUMBER", fpNumPlanes))
        return -1;
    m_nPlanes = (int) fpNumPlanes;
    n = 0;
    while (n < m_nPlanes)   {
        /* nome del piano */
        ReadString (strRead);
        /* eliminiamo eventuali interlinee */
        if (strRead == "")
            continue;
        if (ExtractStringValue (strRead, "PLANE NAME", strPlane))
            return -1;
        /* cominciamo a leggere il nome del primo asse */
        ReadString (strRead);
        if (ExtractStringValue (strRead, "AXIS NAME", strValue))
            return -1;
        nZ = GetAxisLogicAddress (strValue);
        if (nZ == -1)
            return -1;
        /* nome dell'asse X */
        ReadString (strRead);
        if (ExtractStringValue (strRead, "AXIS NAME X", strValue))
            return -1;
        nX = GetAxisLogicAddress (strValue);
        if (nX == -1)
            return -1;
        /* nome dell'asse Y */
        ReadString (strRead);
        if (ExtractStringValue (strRead, "AXIS NAME Y", strValue))
            return -1;
        nY = GetAxisLogicAddress (strValue);
        if (nY == -1)
            return -1;
        /* Origine X */
        ReadString (strRead);
        if (ExtractDoubleValue (strRead, "ORIGIN X", fpOX))
            return -1;
        /* Origine Y */
        ReadString (strRead);
        if (ExtractDoubleValue (strRead, "ORIGIN Y", fpOY))
            return -1;
        /* Limite X */
        ReadString (strRead);
        if (ExtractDoubleValue (strRead, "LIMIT X", fpLimX))
            return -1;
        /* Limite Y */
        ReadString (strRead);
        if (ExtractDoubleValue (strRead, "LIMIT Y", fpLimY))
            return -1;
        /* Numero di Punti X */
        ReadString (strRead);
        if (ExtractDoubleValue (strRead, "POINTS X", fpPointsX))
            return -1;
        /* Numero di Punti Y */
        ReadString (strRead);
        if (ExtractDoubleValue (strRead, "POINTS Y", fpPointsY))
            return -1;
        /* Abilitato / Disabilitato */
        ReadString (strRead);
        if (ExtractDoubleValue (strRead, "ENABLED", fpEnabled))
            return -1;
        /* assegnamo i dati */
        m_PC[n].SetName (strPlane);
        m_PC[n].SetAsse (nZ);
        m_PC[n].SetXData (nX, fpOX, fpLimX, (int) fpPointsX);
        m_PC[n].SetYData (nY, fpOY, fpLimY, (int) fpPointsY);
        fpEnabled == 0 ? m_PC[n].Disable () : m_PC[n].Enable ();
        /* ora i dati */
        nRowY = 0;
        while (ReadString (strRead))
            if (strRead != "")   {
                ExtractData (strRead, n, nRowY++);
            }
        n++;
    }
    return 0;
}

/*
** ExtractStringValue :
*/
int CPlaneCCFile::ExtractStringValue (CString strRead, CString strKey, CString &strValue)
{
    int n;

    if (strRead.Find (strKey) == -1)
        return -1;
    n = strRead.Find (':');
    if (n == -1)
        return -1;
    strValue = strRead.Right (strRead.GetLength () - n - 1);
    strValue.Trim ();
    return 0;
}

/*
** ExtractDoubleValue :
*/
int CPlaneCCFile::ExtractDoubleValue (CString strRead, CString strKey, double &fpValue)
{
    CString strValue;
    int n;
    char *p;

    if (strRead.Find (strKey) == -1)
        return -1;
    n = strRead.Find (':');
    if (n == -1)
        return -1;
    strValue = strRead.Right (strRead.GetLength () - n - 1);
    strValue.Trim ();
    fpValue = strtod (strValue, &p);
    if (*p != '\0')
        return -1;
    return 0;
}

/*
** GetAxisLogicAddress :
*/
int CPlaneCCFile::GetAxisLogicAddress (CString strAxes)
{
    int nDI = -1;

    for (POSITION pos=m_AxesList->GetHeadPosition(); pos; )   {
        CAdamoAxis* pAxes=(CAdamoAxis*)m_AxesList->GetNext (pos);
        if (pAxes->GetName () == strAxes)   {
            nDI = pAxes->GetDeviceID (); break;
        }
    }
    return nDI;
}

/*
** GetAxisName :
*/
CString CPlaneCCFile::GetAxisName (int nLogicAddress)
{
    CString strName;
    int nDI = -1;

    for (POSITION pos=m_AxesList->GetHeadPosition(); pos; )   {
        CAdamoAxis* pAxes=(CAdamoAxis*)m_AxesList->GetNext (pos);
        if (pAxes->GetDeviceID () == nLogicAddress)   {
            strName = pAxes->GetName (); break;
        }
    }
    return strName;
}

/*
** SetValue :
*/
void CPlaneCC::SetValue (int nX, int nY, PointPC& fpValue)
{
    int n;

    if (!m_pData)   {
        m_pData = new PointPC* [m_nPointX];
        n = 0;
        while (n < m_nPointX)   {
            m_pData[n] = new PointPC [m_nPointY];
            n++;
        }
    }
    if (nX < m_nPointX && nY < m_nPointY)
        m_pData[nX][nY] = fpValue;
}

/*
** GetValue :
*/
void CPlaneCC::GetValue (int nX, int nY, PointPC& fpValue)
{
    if (nX < m_nPointX && nY < m_nPointY)
        fpValue = m_pData[nX][nY];
}

/*
** ExtractData :
*/
int CPlaneCCFile::ExtractData (CString &strRead, int nPlane, int nRowY)
{
    PointPC pp;
    CString strValue;
    double fpZ;
    int nRowX, nCurrent, nIndex = 0, nB = 0, nLastCurrent;
    char *p;

    /* ripuliamo i blank */
    strRead.Trim ();
    nRowX = 0;
    nLastCurrent = 0;
    while (true)   {
        if ((nCurrent = strRead.Find (',', nIndex)) != -1)   {
            nLastCurrent = nCurrent;
            strValue = strRead.Mid (nIndex, nCurrent - nIndex);
        }
        else   {
            strValue = strRead.Right (strRead.GetLength () - nLastCurrent - 1);
        }
        strValue.Trim ();
        nIndex = nCurrent + 1;
        fpZ = strtod (strValue, &p);
        if (*p != '\0')   {
            nB = -1; break;
        }
        if (nRowX < m_PC[nPlane].GetPointX ())   {
            pp.x = nRowX * m_PC[nPlane].GetStepX ();
            pp.y = nRowY * m_PC[nPlane].GetStepY ();
            pp.z = fpZ;
            m_PC[nPlane].SetValue (nRowX, nRowY, pp);
            nRowX++;
        }
        else
            break;
    }
    return nB;
}

/*
** GetPCInfo :
*/
bool CPlaneCCFile::GetPCInfo (int nPlane, CmdSetPCInfo &pc)
{
    strcpy (pc.m_strPC, m_PC[nPlane].GetName ());
	pc.m_nAsse = m_PC[nPlane].GetAsse ();
	pc.m_nAsseX = m_PC[nPlane].GetAsseX ();
	pc.m_nAsseY = m_PC[nPlane].GetAsseY ();
	pc.m_fpOX = m_PC[nPlane].GetOX ();
	pc.m_fpOY = m_PC[nPlane].GetOY ();
	pc.m_fpLimX = m_PC[nPlane].GetLimX ();
	pc.m_fpLimY = m_PC[nPlane].GetLimY ();
    pc.m_nPointX = m_PC[nPlane].GetPointX ();
    pc.m_nPointY = m_PC[nPlane].GetPointY ();
    pc.m_bEnabled = m_PC[nPlane].IsEnabled ();
    return true;
}

/*
** GetPCData :
*/
bool CPlaneCCFile::GetPCData (int nPlane, CmdSetPCData &pd, PointPC *pBuf, int nDim)
{
    PointPC *pPC = pBuf;
    int i, n, nPoints;
    int nAvlPoints = nDim / sizeof (PointPC) + 1;
    bool bFinished = false;

    nPoints = 0;
	i = pd.m_nIniX; n = pd.m_nIniY;
    while (i < m_PC[nPlane].GetPointX ())   {
        while (n < m_PC[nPlane].GetPointY ())   {
            if (nPoints == nAvlPoints)   {
                bFinished = true; break;
            }
            m_PC[nPlane].GetValue (i, n, *pPC);
            pPC++; nPoints++; n++;
        }
        if (bFinished)
            break;
		else   {
			i++; n = 0;
		}
    }
    pd.m_nIniX = i;
    pd.m_nIniY = n;
    pd.m_nPCPoints = nPoints;
    return !(i == m_PC[nPlane].GetPointX () && n == 0);
}

/*
** GetPlaneCC :
*/
CPlaneCC *CPlaneCCFile::GetPlaneCC (const char* strArea)
{
    CPlaneCC *PC = NULL;
    for (int i = 0; i<m_nPlanes; i++)   {
        if (!strcmp (m_PC[i].GetName (), strArea))   {
            PC = &m_PC[i]; break;
        }
    }
    if (!PC && m_nPlanes <NMAX_PLANECC)
        PC = &m_PC[m_nPlanes++];
    return PC;
}

/*
** SetName :
*/
void CPlaneCC::SetName (const char *s)
{
	strlimcpy (strPC, s, NMAXSTRING);
}

/*
** GetName :
*/
char *CPlaneCC::GetName ()
{
	return strPC;
}


