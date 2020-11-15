/*
** AdamoVersione.h : contiene le definizioni per la gestione della conversione dei files.
*/

#ifndef __ADAMO__VERSIONE__
#define __ADAMO__VERSIONE__

/* definzione delle versioni in essere */
#define VERSIONE_1_3    0x00010103
#define VERSIONE_2_0    0x00020000
#define VERSIONE_2_6    0x00020600

/* constants definitions */
#define OLDMAXCOLUMNS 64
#define OLDMAXCANOPENNODES 48

/* data definitions */
struct oldRecGlobals   {
    char szName[NMAXIDENTIFIER+1];
    char szModule[_MAX_PATH];
    int nType;
    int nVarType;
    struct   {
        int nArrType;
        int nArrElements;
    } arr;
    struct   {
        int nRows;
        int nCols;
        int anTypes[OLDMAXCOLUMNS];
    } mat;
};

struct RecGlobals   {
    char szName[NMAXIDENTIFIER+1];
    char szModule[_MAX_PATH];
    int nType;
    int nVarType;
    struct   {
        int nArrType;
        int nArrElements;
    } arr;
    struct   {
        int nRows;
        int nCols;
        int anTypes[MAXCOLUMNS];
    } mat;
};

#pragma pack  (push, 8)

struct stHWCanOpenNode1_1
{
    double afpLLIAInf[12];
    double afpLLIASup[12];
    double afpLLOAInf[12];
    double afpLLOASup[12];
    WORD   wTPDOType[4];
    DWORD  dwNodeGuarding;
    DWORD  dwHeartBeatTime;
    bool   bIsVirtual;
    bool   bIsDisabled;
    bool   bIsPolling;
    char   achSpare[1020];
};

struct stHWCanOpenNet1_1
{
    int m_nBaudRate;
    int m_nSyncFrequency;
    int m_nSDOTimeout;
    int m_nGuardTime;
    int m_nLifeTime;
    char chCanSpare[1024];
    stHWCanOpenNode1_1 m_HWCanOpenNode[OLDMAXCANOPENNODES];
};

struct stHWSSCNETStation1_1
{
    bool m_bAbsoluteEncoder;
    double m_fpHomePosition;
};

struct stHWSSCNETNet1_1
{
    int  m_nFrequency;
    bool m_bEnableWarnings;
    stHWSSCNETStation1_1 m_hWSSCNETStation[NMAXMGA023STATION];
    WORD m_awParameters[NMAXMGA023STATION][4][64];
    char chSSCNETSpare[1024];
};

struct stHWSSCNETAbsPos1_1
{
    DWORD dwOPOneRevPos;
    WORD  wOPMultiRevCounter;
};

struct stHWAdamoConf1_1
{
    stHWCanOpenNet1_1 m_HWCanOpenNet[NMAX_SCHEDE];
    stHWSSCNETNet1_1  m_HWSSCNETNet[NMAX_SCHEDE];
    stHWSSCNETAbsPos1_1    m_HWSSCNETAbsPos[16];
    char chHWSpare[10112];
};

#pragma pack (push, 1)

struct stHWSSCNETNet2_0
{
    int  m_nFrequency;
    bool m_bEnableWarnings;
    stHWSSCNETStation m_hWSSCNETStation[NMAXMGA023STATION];
    WORD m_awParameters[NMAXMGA023STATION][4][64];
	int  m_bNetworkEnabled;
    char chSSCNETSpare[1020];
};

struct stHWAdamoConf2_0
{
    stHWCanOpenNet		 m_HWCanOpenNet[NMAX_SCHEDE];
    stHWSSCNETNet2_0     m_HWSSCNETNet[NMAX_SCHEDE];
    stHWSSCNETAbsPos     m_HWSSCNETAbsPos[16];
	stHWEthercatNet		 m_HWEthercatNet;
    char chHWSpare[10096];
};

#pragma pack (pop)
#pragma pack (pop)

#endif
