/*
** AdamoVersion : file che contiene le procedure per convertire i file da una versione ad un'altra.
*/

/* include files */
#include "stdafx.h"
#include <io.h>
#include "ide2.h"
#include "Project.h"
#include "AdamoVersion.h"

#define NMAX_CANNODES_VERSION_1_1 48

/* static functions */
static void CopyConvertedData2_0 (stHWAdamoConf1_1 *pHWAdamoConf1_1, stHWAdamoConf2_0 *HWAdamoConf2_0);
static void CopyConvertedData2_6 (stHWAdamoConf2_0 *pHWAdamoConf2_0, stHWAdamoConf *pHWAdamoConf2_6);

/*
** From1_1_1To1_1_3 :
*/
int CProject::From1_1_1To1_1_3 ()
{
    /* convertiamo il file globals.dat */
    int nB=-1;
    char szBuffer[NMAXIDENTIFIER+1];
    char szPath[_MAX_PATH];
    char szFileName[_MAX_FNAME];
    oldRecGlobals rcold;
    RecGlobals rc;

    CFile fOldGlobals, fGlobals;
    GetTempPath (_MAX_PATH, szPath);
    GetTempFileName (szPath, "dat", 0, szFileName);
    CString fnOld=GetProjectDir() + "\\" + GLOBALS_FILE;
    CString fn=szFileName;
    if (!_access (fnOld, 0))   {
        if (fOldGlobals.Open(fnOld, CFile::modeRead|CFile::typeBinary))   {
            if (fGlobals.Open(fn, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))   {
                while (fOldGlobals.Read (&rcold, sizeof (oldRecGlobals))==sizeof (oldRecGlobals))   {
                    rc.nType=rcold.nType;
                    rc.nVarType=rcold.nVarType;
                    strcpy (rc.szModule, rcold.szModule);
                    strcpy (rc.szName, rcold.szName);
                    rc.arr.nArrElements=rcold.arr.nArrElements;
                    rc.arr.nArrType=rcold.arr.nArrType;
                    rc.mat.nCols=rcold.mat.nCols;
                    rc.mat.nRows=rcold.mat.nRows;
                    memset (rc.mat.anTypes, 0, sizeof (rc.mat.anTypes));
                    memcpy (rc.mat.anTypes, rcold.mat.anTypes, sizeof (rc.mat.anTypes));
                    fGlobals.Write (&rc, sizeof (RecGlobals));
                    for (int i=0; i<rc.mat.nCols; i++)   {
                        fOldGlobals.Read (szBuffer, sizeof (szBuffer));
                        fGlobals.Write (szBuffer, sizeof (szBuffer));
                    }
                }
                fGlobals.Close ();
            }
            fOldGlobals.Close ();
            /* ora cancelliamo il file delle globali vecchio */
            CFile::Remove (fnOld);
            /* ora copiamo il file temporaneo */
            CopyFile (fn, fnOld, false);
            /* lavoro svolto */
            nB=0;
        }
    }
    return nB;
}

/*
** From1_1_3To2_0_0  :
*/
int CProject::From1_1_3To2_0_0 ()
{
	CAdamoHWConfigFile *p_HWConfigFile;
	stHWAdamoConf1_1 *p_HWAdamoConf1_1;
	stHWAdamoConf2_0 *p_HWAdamoConf2_0;

    p_HWConfigFile = new CAdamoHWConfigFile;
    p_HWAdamoConf2_0 = new stHWAdamoConf2_0;
    p_HWAdamoConf1_1 = new stHWAdamoConf1_1;
    /* convertiamo il file hardware */
	CString strPath=GetHWConfigPathName();
	if (p_HWConfigFile->Open (strPath, true))   {
		p_HWConfigFile->Read (p_HWAdamoConf1_1, sizeof (stHWAdamoConf1_1));
		CopyConvertedData2_0 (p_HWAdamoConf1_1, p_HWAdamoConf2_0);
        p_HWConfigFile->SeekToBegin ();
		p_HWConfigFile->Write (p_HWAdamoConf2_0, sizeof (stHWAdamoConf2_0));
		p_HWConfigFile->CloseFile ();
	}
    delete p_HWConfigFile;
    delete p_HWAdamoConf1_1;
    delete p_HWAdamoConf2_0;
    return 0;
}

/*
** From2_0_0To2_6_0 :
*/
int CProject::From2_0_0To2_6_0 ()
{
	CAdamoHWConfigFile *p_HWConfigFile;
	stHWAdamoConf2_0 *p_HWAdamoConf2_0;
	stHWAdamoConf    *p_HWAdamoConf2_6;

    p_HWConfigFile = new CAdamoHWConfigFile;
    p_HWAdamoConf2_6 = new stHWAdamoConf;
    p_HWAdamoConf2_0 = new stHWAdamoConf2_0;
    /* convertiamo il file hardware */
	CString strPath=GetHWConfigPathName();
	if (p_HWConfigFile->Open (strPath, true))   {
		p_HWConfigFile->Read (p_HWAdamoConf2_0, sizeof (stHWAdamoConf2_0));
		CopyConvertedData2_6 (p_HWAdamoConf2_0, p_HWAdamoConf2_6);
        p_HWConfigFile->SeekToBegin ();
		p_HWConfigFile->Write (p_HWAdamoConf2_6, sizeof (stHWAdamoConf));
		p_HWConfigFile->CloseFile ();
	}
    delete p_HWConfigFile;
    delete p_HWAdamoConf2_0;
    delete p_HWAdamoConf2_6;
    return 0;
}

/*
** CopyConvertedData2_0 :
*/
void CopyConvertedData2_0 (stHWAdamoConf1_1 *pHWAdamoConf1_1, stHWAdamoConf2_0 *pHWAdamoConf2_0)
{
	int i, n, q, s;

	memset (pHWAdamoConf2_0, 0, sizeof (pHWAdamoConf2_0));
	for (i=0; i<NMAX_SCHEDE; i++)   {
		pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nBaudRate=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_nBaudRate;
		pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nSyncFrequency=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_nSyncFrequency;
		pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nSDOTimeout=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_nSDOTimeout;
		pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nGuardTime=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_nGuardTime;
		pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nLifeTime=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_nLifeTime;
		pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nBaudRate=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_nBaudRate;
		for (n=0; n<NMAX_CANNODES_VERSION_1_1; n++)   {
			for (q=0; q<12; q++)   {
				pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLIAInf[q]=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLIAInf[q];
				pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLIASup[q]=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLIASup[q];
				pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLOAInf[q]=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLOAInf[q];
				pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLOASup[q]=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLOASup[q];
			}
			for (q=0; q<4; q++)   {
				pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].wTPDOType[q]=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].wTPDOType[q];
			}
			pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].dwNodeGuarding=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].dwNodeGuarding;
			pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].dwHeartBeatTime=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].dwHeartBeatTime;
			pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsVirtual=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsVirtual;
			pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsDisabled=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsDisabled;
			pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsPolling=pHWAdamoConf1_1->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsPolling;
		}
	}
	for (i=0; i<NMAX_SCHEDE; i++)   {
		pHWAdamoConf2_0->m_HWSSCNETNet[i].m_nFrequency=pHWAdamoConf1_1->m_HWSSCNETNet[i].m_nFrequency;
		pHWAdamoConf2_0->m_HWSSCNETNet[i].m_bEnableWarnings=pHWAdamoConf1_1->m_HWSSCNETNet[i].m_bEnableWarnings;
		for (n=0; n<NMAXMGA023STATION; n++)   {
			pHWAdamoConf2_0->m_HWSSCNETNet[i].m_hWSSCNETStation[n].m_bAbsoluteEncoder=pHWAdamoConf1_1->m_HWSSCNETNet[i].m_hWSSCNETStation[n].m_bAbsoluteEncoder;
			pHWAdamoConf2_0->m_HWSSCNETNet[i].m_hWSSCNETStation[n].m_fpHomePosition=pHWAdamoConf1_1->m_HWSSCNETNet[i].m_hWSSCNETStation[n].m_fpHomePosition;
		}
		for (n=0; n<NMAXMGA023STATION; n++)   {
            pHWAdamoConf2_0->m_HWSSCNETAbsPos[n].dwOPOneRevPos=pHWAdamoConf1_1->m_HWSSCNETAbsPos[n].dwOPOneRevPos;
            pHWAdamoConf2_0->m_HWSSCNETAbsPos[n].wOPMultiRevCounter=pHWAdamoConf1_1->m_HWSSCNETAbsPos[n].wOPMultiRevCounter;
        }
		for (n=0; n<NMAXMGA023STATION; n++)
			for (q=0; q<4; q++)
				for (s=0; s<64; s++)
					pHWAdamoConf2_0->m_HWSSCNETNet[i].m_awParameters[n][q][s]=pHWAdamoConf1_1->m_HWSSCNETNet[i].m_awParameters[n][q][s];
	}
}

/*
** CopyConvertedData2_6 :
*/
void CopyConvertedData2_6 (stHWAdamoConf2_0 *pHWAdamoConf2_0, stHWAdamoConf *pHWAdamoConf2_6)
{
	int i, n, q, s;

	memset (pHWAdamoConf2_6, 0, sizeof (stHWAdamoConf));
	for (i=0; i<NMAX_SCHEDE; i++)   {
		pHWAdamoConf2_6->m_HWCanOpenNet[i].m_nBaudRate=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nBaudRate;
		pHWAdamoConf2_6->m_HWCanOpenNet[i].m_nSyncFrequency=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nSyncFrequency;
		pHWAdamoConf2_6->m_HWCanOpenNet[i].m_nSDOTimeout=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nSDOTimeout;
		pHWAdamoConf2_6->m_HWCanOpenNet[i].m_nGuardTime=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nGuardTime;
		pHWAdamoConf2_6->m_HWCanOpenNet[i].m_nLifeTime=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nLifeTime;
		pHWAdamoConf2_6->m_HWCanOpenNet[i].m_nBaudRate=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_nBaudRate;
		for (n=0; n<NMAX_CANNODES_VERSION_1_1; n++)   {
			for (q=0; q<12; q++)   {
				pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLIAInf[q]=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLIAInf[q];
				pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLIASup[q]=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLIASup[q];
				pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLOAInf[q]=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLOAInf[q];
				pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLOASup[q]=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].afpLLOASup[q];
			}
			for (q=0; q<4; q++)   {
				pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].wTPDOType[q]=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].wTPDOType[q];
			}
			pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].dwNodeGuarding=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].dwNodeGuarding;
			pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].dwHeartBeatTime=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].dwHeartBeatTime;
			pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsVirtual=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsVirtual;
			pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsDisabled=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsDisabled;
			pHWAdamoConf2_6->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsPolling=pHWAdamoConf2_0->m_HWCanOpenNet[i].m_HWCanOpenNode[n].bIsPolling;
		}
	}
	for (i=0; i<NMAX_SCHEDE; i++)   {
		pHWAdamoConf2_6->m_HWSSCNETNet[i].m_nFrequency=pHWAdamoConf2_0->m_HWSSCNETNet[i].m_nFrequency;
		pHWAdamoConf2_6->m_HWSSCNETNet[i].m_bEnableWarnings=pHWAdamoConf2_0->m_HWSSCNETNet[i].m_bEnableWarnings;
		for (n=0; n<NMAXMGA023STATION; n++)   {
			pHWAdamoConf2_6->m_HWSSCNETNet[i].m_hWSSCNETStation[n].m_bAbsoluteEncoder=pHWAdamoConf2_0->m_HWSSCNETNet[i].m_hWSSCNETStation[n].m_bAbsoluteEncoder;
			pHWAdamoConf2_6->m_HWSSCNETNet[i].m_hWSSCNETStation[n].m_fpHomePosition=pHWAdamoConf2_0->m_HWSSCNETNet[i].m_hWSSCNETStation[n].m_fpHomePosition;
		}
		for (n=0; n<NMAXMGA023STATION; n++)   {
            pHWAdamoConf2_6->m_HWSSCNETAbsPos[n].dwOPOneRevPos=pHWAdamoConf2_0->m_HWSSCNETAbsPos[n].dwOPOneRevPos;
            pHWAdamoConf2_6->m_HWSSCNETAbsPos[n].wOPMultiRevCounter=pHWAdamoConf2_0->m_HWSSCNETAbsPos[n].wOPMultiRevCounter;
        }
		for (n=0; n<NMAXMGA023STATION; n++)
			for (q=0; q<4; q++)
				for (s=0; s<64; s++)
					pHWAdamoConf2_6->m_HWSSCNETParameters[i].m_awParameters[n][q][s]=pHWAdamoConf2_0->m_HWSSCNETNet[i].m_awParameters[n][q][s];
	}
}
