/*
**
*/

/* include files */
#include "stdafx.h"
#include <io.h>
#include "AdamoMachine.h"
#include "AdamoDgtIn.h"
#include "AdamoDgtOut.h"
#include "AdamoAnalogIn.h"
#include "AdamoAnalogOut.h"
#include "AdamoAxis.h"
#include "AdamoCounter.h"
#include "AdamoTimer.h"
#include "AdamoGeneric.h"

/* debug memory directive */
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
** Construction / Destruction
*/
CAdamoDatiFile::CAdamoDatiFile ()
{
}

CAdamoDatiFile::~CAdamoDatiFile()
{
}

/*
** Open : apre il file. Se bMode == 0 azzera tutti gli indici e el strutture
**        e apre il file in crezione distruggendo il contenuto.
*/
BOOL CAdamoDatiFile::Open (CPath pPath, bool bMode)
{
    BOOL b;
    /* se siamo in creazione azzeriamo tutti i registri */
    if (!bMode)   {
        memset (&AdamoParametri, 0, sizeof (AdamoParametri));
        memset (&IdRegistry, 0, sizeof (IdRegistry));
    }
    /* apriamo il file */
    b=CFile::Open (pPath, bMode ? CFile::typeBinary|CFile::modeReadWrite :
                                        CFile::typeBinary|CFile::modeCreate|CFile::modeReadWrite);
    /* creaimo subito la copia di backup, se siamo in creazione */
    if (!bMode)   {
        /* chiudiamo il file se no non possiamo copiarlo */
        Close ();
        /* cambiamo l'estensione del filozzo */
        CPath pPathDest (pPath);
        pPathDest.RenameExtension ("bck");
        /* copiamoci il filozzo */
        CopyFile ((const char*)pPath, (const char*)pPathDest, false);
        /* una bella ricorsioncina */
        b=Open (pPath, TRUE);
    }
    return b;
}

/*
** ReadData : legge la struttura AdamoParametri dal disco.
*/
void CAdamoDatiFile::ReadData ()
{
    Read (&AdamoParametri, sizeof (AdamoParametri));
}

/*
** WriteData : scrive su disco la struttura AdamoParametri.
*/
void CAdamoDatiFile::WriteData ()
{
    Seek (0, CFile::begin);
    Write (&AdamoParametri, sizeof (AdamoParametri));
}

/*
** Close : chiude il file
*/
void CAdamoDatiFile::Close ()
{
    WriteData ();
    CFile::Close ();
}

/*
** CloseFile : chiude solo il file
*/
void CAdamoDatiFile::CloseFile ()
{
    CFile::Close ();
}

/*
** AddAdamoDevice : aggiunge un dipositivo, assegnandone implicitamente l'indirizzo
**                  logico. Incrementa i contatori presenti in stIdRegistry.
*/
int CAdamoDatiFile::AddAdamoDevice (CAdamoBase* pDev)
{
    int n=0;
    switch (pDev->GetType ())   {
        case enInputDigitale:
            n=ManageInputDig (pDev);
            break;
        case enOutputDigitale :
            n=ManageOutputDig (pDev);
            break;
        case enInputAnalogico :
            n=ManageInputAnag (pDev);
            break;
        case enOutputAnalogico :
            n=ManageOutputAnag (pDev);
            break;
        case enCounter :
            n=ManageCounter (pDev);
            break;
        case enTimer :
            n=ManageTimer (pDev);
            break;
        case enAsse :
            n=ManageAxes (pDev);
            break;
        case enGenericCan :
            n=ManageCanDevice (pDev);
            break;
    }
    return n;
}

/*
** RemoveAdamoDevice : cancella se esiste il dispositivo passato per parametro.
*/
int CAdamoDatiFile::RemoveAdamoDevice (CAdamoBase* pDev)
{
    int nIndex;
    if ((nIndex=ExistDevice(pDev))!=-1)   {
        switch (pDev->GetType())   {
            case enInputDigitale :
                AdamoParametri.AdamoInputDig[nIndex].szNome[0]='\0';
                break;    
            case enOutputDigitale :
                AdamoParametri.AdamoOutputDig[nIndex].szNome[0]='\0';
                break;    
            case enInputAnalogico :
                AdamoParametri.AdamoInputAnag[nIndex].szNome[0]='\0';
                break;
            case enOutputAnalogico :
                AdamoParametri.AdamoOutputAnag[nIndex].szNome[0]='\0';
                break;
            case enCounter :
                AdamoParametri.AdamoCounter[nIndex].szNome[0]='\0';
                break;
            case enTimer :
                AdamoParametri.AdamoTimer[nIndex].szNome[0]='\0';
                break;
            case enAsse :
                AdamoParametri.AdamoAsse[nIndex].szNome[0]='\0';
                break;    
            case enGenericCan :
                AdamoParametri.AdamoCanDevice[nIndex].szNome[0]='\0';
                break;    
        } 
    }
	return nIndex;
}

/*
** ModifyAdamoDevice
*/
int CAdamoDatiFile::ModifyAdamoDevice (CAdamoBase* pDev)
{
    int nIndex;
    if ((nIndex=ExistDevice(pDev))!=-1)   {
        switch (pDev->GetType())   {
            case enInputDigitale :
                SetInputDigData (pDev, nIndex);
                break;    
            case enOutputDigitale :
                SetOutputDigData (pDev, nIndex);
                break;    
            case enInputAnalogico :
                SetInputAnagData (pDev, nIndex);
                break;
            case enOutputAnalogico :
                SetOutputAnagData (pDev, nIndex);
                break;
            case enCounter :
                SetCounterData (pDev, nIndex);
                break;
            case enTimer :
                SetTimerData (pDev, nIndex);
                break;
            case enAsse :
                SetAxesData (pDev, nIndex);
                break;    
            case enGenericCan :
                SetCanDeviceData (pDev, nIndex);
                break;    
        } 
    }
	return nIndex;
}

/*
** ExistDevice
*/
int CAdamoDatiFile::ExistDevice (CAdamoBase* pDev)
{
    char szBufCompare[256];
    int i;
    int nB=-1;

    strcpy (szBufCompare, pDev->GetPathName() + "." + pDev->GetName ());    
    switch (pDev->GetType ())   {
        case enInputDigitale :
            for (i=0; i<IdRegistry.m_nInpDig; i++)   {
                if (!strcmp (AdamoParametri.AdamoInputDig[i].szNome,
                             szBufCompare))   {
                    nB=i;
                    break;
                }
            }
            break;    
        case enOutputDigitale :
            for (i=0; i<IdRegistry.m_nOutDig; i++)   {
                if (!strcmp (AdamoParametri.AdamoOutputDig[i].szNome,
                             szBufCompare))   {
                    nB=i;
                    break;
                }
            }
            break;    
        case enInputAnalogico :
            for (i=0; i<IdRegistry.m_nInpAnag; i++)   {
                if (!strcmp (AdamoParametri.AdamoInputAnag[i].szNome,
                             szBufCompare))   {
                    nB=i;
                    break;
                }
            }
            break;
        case enOutputAnalogico :
            for (i=0; i<IdRegistry.m_nOutAnag; i++)   {
                if (!strcmp (AdamoParametri.AdamoOutputAnag[i].szNome,
                             szBufCompare))   {
                    nB=i;
                    break;
                }
            }
            break;
        case enCounter :
            for (i=0; i<IdRegistry.m_nCounter; i++)   {
                if (!strcmp (AdamoParametri.AdamoCounter[i].szNome,
                             szBufCompare))   {
                    nB=i;
                    break;
                }
            }
            break;
        case enTimer :
            for (i=0; i<IdRegistry.m_nTimer; i++)   {
                if (!strcmp (AdamoParametri.AdamoTimer[i].szNome,
                             szBufCompare))   {
                    nB=i;
                    break;
                }
            }
            break;
        case enAsse :
            for (i=0; i<IdRegistry.m_nAxes; i++)   {
                if (!strcmp (AdamoParametri.AdamoAsse[i].szNome,
                             szBufCompare))   {
                    nB=i;
                    break;
                }
            }
            break;    
        case enGenericCan :
            for (i=0; i<IdRegistry.m_nCan; i++)   {
                if (!strcmp (AdamoParametri.AdamoCanDevice[i].szNome,
                             szBufCompare))   {
                    nB=i;
                    break;
                }
            }
            break;
    }
    return nB;
}

/*
** ManageInputDig
*/
int CAdamoDatiFile::ManageInputDig (CAdamoBase* pDev)
{
    int nIndex=IdRegistry.m_nInpDig, nB=0, i;
    if (nIndex<NMAXINPDIG)   {
        pDev->SetDeviceID (nIndex);
        SetInputDigData (pDev, nIndex);
        for (i=nIndex+1; i<NMAXINPDIG; i++)   {
            if (AdamoParametri.AdamoInputDig[i].szNome[0]=='\0')
                break;
        }
        IdRegistry.m_nInpDig=i;
    }
    else
        nB=-1;
    return nB;
}

/*
** ManageOutputDig
*/
int CAdamoDatiFile::ManageOutputDig (CAdamoBase* pDev)
{
    int nIndex=IdRegistry.m_nOutDig, nB=0, i;
    if (nIndex<NMAXOUTDIG)   {
        pDev->SetDeviceID (nIndex);
        SetOutputDigData (pDev, nIndex);
        for (i=nIndex+1; i<NMAXOUTDIG; i++)   {
            if (AdamoParametri.AdamoOutputDig[i].szNome[0]=='\0')
                break;
        }
        IdRegistry.m_nOutDig=i;
    }
    else
        nB=-1;
    return nB;
}

/*
** ManageInputAnag
*/
int CAdamoDatiFile::ManageInputAnag (CAdamoBase* pDev)
{
    int nIndex=IdRegistry.m_nInpAnag, nB=0, i;
    if (nIndex<NMAXINPANA)   {
        pDev->SetDeviceID (nIndex);
        SetInputAnagData (pDev, nIndex);
        for (i=nIndex+1; i<NMAXINPANA; i++)   {
            if (AdamoParametri.AdamoInputAnag[i].szNome[0]=='\0')
                break;
        }
        IdRegistry.m_nInpAnag=i;
    }
    else
        nB=-1;
    return nB;
}

/*
** ManageOutputAnag
*/
int CAdamoDatiFile::ManageOutputAnag (CAdamoBase* pDev)
{
    int nIndex=IdRegistry.m_nOutAnag, nB=0, i;
    if (nIndex<NMAXOUTANA)   {
        pDev->SetDeviceID (nIndex);
        SetOutputAnagData (pDev, nIndex);
        for (i=nIndex+1; i<NMAXOUTANA; i++)   {
            if (AdamoParametri.AdamoOutputAnag[i].szNome[0]=='\0')
                break;
        }
        IdRegistry.m_nOutAnag=i;
    }
    else
        nB=-1;
    return nB;
}

/*
** ManageCounter
*/
int CAdamoDatiFile::ManageCounter (CAdamoBase* pDev)
{
    int nIndex=IdRegistry.m_nCounter, nB=0, i;
    if (nIndex<NMAXCOUNTER)   {
        pDev->SetDeviceID (nIndex);
        SetCounterData (pDev, nIndex);
        for (i=nIndex+1; i<NMAXCOUNTER; i++)   {
            if (AdamoParametri.AdamoCounter[i].szNome[0]=='\0')
                break;
        }
        IdRegistry.m_nCounter=i;
    }
    else
        nB=-1;
    return nB;
}

/*
** ManageTimer
*/
int CAdamoDatiFile::ManageTimer (CAdamoBase* pDev)
{
    int nIndex=IdRegistry.m_nTimer, nB=0, i;
    if (nIndex<NMAXTIMER)   {
        pDev->SetDeviceID (nIndex);
        SetTimerData (pDev, nIndex);
        for (i=nIndex+1; i<NMAXTIMER; i++)   {
            if (AdamoParametri.AdamoTimer[i].szNome[0]=='\0')
                break;
        }
        IdRegistry.m_nTimer=i;
    }
    else
        nB=-1;
    return nB;
}

/*
** ManageAxes
*/
int CAdamoDatiFile::ManageAxes (CAdamoBase* pDev)
{
    int nIndex=IdRegistry.m_nAxes, nB=0, i;
    if (nIndex<NMAXAXIS)   {
        pDev->SetDeviceID (nIndex);
        SetAxesData (pDev, nIndex);
        for (i=nIndex+1; i<NMAXAXIS; i++)   {
            if (AdamoParametri.AdamoAsse[i].szNome[0]=='\0')
                break;
        }
        IdRegistry.m_nAxes=i;
    }
    else
        nB=-1;
    return nB;
}

/*
** ManageCanDevice
*/
int CAdamoDatiFile::ManageCanDevice (CAdamoBase* pDev)
{
    int nIndex=IdRegistry.m_nCan, nB=0, i;
    if (nIndex<NMAXGENERICCAN)   {
        pDev->SetDeviceID (nIndex);
        SetCanDeviceData (pDev, nIndex);
        for (i=nIndex+1; i<NMAXGENERICCAN; i++)   {
            if (AdamoParametri.AdamoCanDevice[i].szNome[0]=='\0')
                break;
        }
        IdRegistry.m_nCan=i;
    }
    else
        nB=-1;
    return nB;
}

/*
** SetInputDigData 
*/
void CAdamoDatiFile::SetInputDigData (CAdamoBase* pDev, int nIndex)
{
    strcpy (AdamoParametri.AdamoInputDig[nIndex].szNome, pDev->GetPathName()+"."+pDev->GetName());
    strcpy (AdamoParametri.AdamoInputDig[nIndex].szDescription, pDev->GetDesc ());
    AdamoParametri.AdamoInputDig[nIndex].nNumInputDig=((CAdamoDgtIn*)pDev)->GetNumInput();
}

/*
** SetOutputDigData
*/
void CAdamoDatiFile::SetOutputDigData (CAdamoBase* pDev, int nIndex)
{
    strcpy (AdamoParametri.AdamoOutputDig[nIndex].szNome, pDev->GetPathName()+"."+pDev->GetName());
    strcpy (AdamoParametri.AdamoOutputDig[nIndex].szDescription, pDev->GetDesc ());
    AdamoParametri.AdamoOutputDig[nIndex].nType=((CAdamoDgtOut*)pDev)->GetIsMonoStable ();
    AdamoParametri.AdamoOutputDig[nIndex].nNumOutputDig=((CAdamoDgtOut*)pDev)->GetNumOutput ();
    AdamoParametri.AdamoOutputDig[nIndex].nTimeOut=((CAdamoDgtOut*)pDev)->GetTimeOut ();
}

/*
** SetInputAnagData
*/
void CAdamoDatiFile::SetInputAnagData (CAdamoBase* pDev, int nIndex)
{
    strcpy (AdamoParametri.AdamoInputAnag[nIndex].szNome, pDev->GetPathName()+"."+pDev->GetName());
    strcpy (AdamoParametri.AdamoInputAnag[nIndex].szDescription, pDev->GetDesc ());
    AdamoParametri.AdamoInputAnag[nIndex].fpLLInf=((CAdamoAnalogIn*)pDev)->GetLLInf ();
    AdamoParametri.AdamoInputAnag[nIndex].fpLLSup=((CAdamoAnalogIn*)pDev)->GetLLSup ();
}

/*
** SetOutputAnagData
*/
void CAdamoDatiFile::SetOutputAnagData (CAdamoBase* pDev, int nIndex)
{
    strcpy (AdamoParametri.AdamoOutputAnag[nIndex].szNome, pDev->GetPathName()+"."+pDev->GetName());
    strcpy (AdamoParametri.AdamoOutputAnag[nIndex].szDescription, pDev->GetDesc ());
    AdamoParametri.AdamoOutputAnag[nIndex].fpLLInf=((CAdamoAnalogOut*)pDev)->GetLLInf ();
    AdamoParametri.AdamoOutputAnag[nIndex].fpLLSup=((CAdamoAnalogOut*)pDev)->GetLLSup ();
}

/*
** SetCounterData
*/
void CAdamoDatiFile::SetCounterData (CAdamoBase* pDev, int nIndex)
{
    strcpy (AdamoParametri.AdamoCounter[nIndex].szNome, pDev->GetPathName()+"."+pDev->GetName());
    strcpy (AdamoParametri.AdamoCounter[nIndex].szDescription, pDev->GetDesc ());
    AdamoParametri.AdamoCounter[nIndex].nType=((CAdamoCounter*)pDev)->GetCounterType ();
}

/*
** SetTimerData
*/
void CAdamoDatiFile::SetTimerData (CAdamoBase* pDev, int nIndex)
{
    strcpy (AdamoParametri.AdamoTimer[nIndex].szNome, pDev->GetPathName()+"."+pDev->GetName());
    strcpy (AdamoParametri.AdamoTimer[nIndex].szDescription, pDev->GetDesc ());
    AdamoParametri.AdamoTimer[nIndex].bEnabled=((CAdamoTimer*)pDev)->GetEnabled ();
    AdamoParametri.AdamoTimer[nIndex].nPeriod=((CAdamoTimer*)pDev)->GetTime ();
    AdamoParametri.AdamoTimer[nIndex].nUpDown=((CAdamoTimer*)pDev)->GetUpDown ();
    AdamoParametri.AdamoTimer[nIndex].nStored=((CAdamoTimer*)pDev)->GetStored ();
}

/*
** SetAxesData
*/
void CAdamoDatiFile::SetAxesData (CAdamoBase* pDev, int nIndex)
{
    strcpy (AdamoParametri.AdamoAsse[nIndex].szNome, pDev->GetPathName()+"."+pDev->GetName());
    strcpy (AdamoParametri.AdamoAsse[nIndex].szDescription, pDev->GetDesc ());
    strcpy (AdamoParametri.AdamoAsse[nIndex].szAlias, ((CAdamoAxis*)pDev)->GetAlias ());
    strcpy (AdamoParametri.AdamoAsse[nIndex].szGName, ((CAdamoAxis*)pDev)->GetGName ());
    AdamoParametri.AdamoAsse[nIndex].fpResolution=((CAdamoAxis*)pDev)->GetResolution();
    AdamoParametri.AdamoAsse[nIndex].nAxisType=((CAdamoAxis*)pDev)->GetAxisType ();
    AdamoParametri.AdamoAsse[nIndex].nAxisCategory =((CAdamoAxis*)pDev)->GetAxesCategory ();
    AdamoParametri.AdamoAsse[nIndex].nUnit=((CAdamoAxis*)pDev)->GetUnit();
    AdamoParametri.AdamoAsse[nIndex].nInvPhase=((CAdamoAxis*)pDev)->GetInvPhase();
    AdamoParametri.AdamoAsse[nIndex].nInvRif=((CAdamoAxis*)pDev)->GetInvRif();
    AdamoParametri.AdamoAsse[nIndex].nWaitStopAxis=((CAdamoAxis*)pDev)->GetWaitStopAxis();
    AdamoParametri.AdamoAsse[nIndex].nEnableNick=((CAdamoAxis*)pDev)->GetEnableNick();
    AdamoParametri.AdamoAsse[nIndex].nEnableJerk=((CAdamoAxis*)pDev)->GetEnableJerk();
    AdamoParametri.AdamoAsse[nIndex].fpSpeed=((CAdamoAxis*)pDev)->GetSpeed();
    AdamoParametri.AdamoAsse[nIndex].fpSpeedMan=((CAdamoAxis*)pDev)->GetSpeedMan();
    AdamoParametri.AdamoAsse[nIndex].fpAcc=((CAdamoAxis*)pDev)->GetAcc();
    AdamoParametri.AdamoAsse[nIndex].fpDec=((CAdamoAxis*)pDev)->GetDec();
    AdamoParametri.AdamoAsse[nIndex].fpProp=((CAdamoAxis*)pDev)->GetProp();
    AdamoParametri.AdamoAsse[nIndex].fpInt=((CAdamoAxis*)pDev)->GetInt();
    AdamoParametri.AdamoAsse[nIndex].fpDer=((CAdamoAxis*)pDev)->GetDer();
    AdamoParametri.AdamoAsse[nIndex].fpFeedForward=((CAdamoAxis*)pDev)->GetFeedForward();
    AdamoParametri.AdamoAsse[nIndex].fpFeedForwardAcc=((CAdamoAxis*)pDev)->GetFeedForwardAcc();
    AdamoParametri.AdamoAsse[nIndex].fpRif=((CAdamoAxis*)pDev)->GetRif();
    AdamoParametri.AdamoAsse[nIndex].nRampType=((CAdamoAxis*)pDev)->GetRampType();
    AdamoParametri.AdamoAsse[nIndex].fpInterSpeed=((CAdamoAxis*)pDev)->GetInterSpeed();
    AdamoParametri.AdamoAsse[nIndex].fpInterAcc=((CAdamoAxis*)pDev)->GetInterAcc();
    AdamoParametri.AdamoAsse[nIndex].fpInterDec=((CAdamoAxis*)pDev)->GetInterDec();
    AdamoParametri.AdamoAsse[nIndex].fpInterProp=((CAdamoAxis*)pDev)->GetInterProp();
    AdamoParametri.AdamoAsse[nIndex].fpInterInt=((CAdamoAxis*)pDev)->GetInterInt();
    AdamoParametri.AdamoAsse[nIndex].fpInterDer=((CAdamoAxis*)pDev)->GetInterDer();
    AdamoParametri.AdamoAsse[nIndex].fpInterFeedForward=((CAdamoAxis*)pDev)->GetInterFeedForward();
    AdamoParametri.AdamoAsse[nIndex].fpInterFeedForwardAcc=((CAdamoAxis*)pDev)->GetInterFeedForwardAcc();
    AdamoParametri.AdamoAsse[nIndex].nInterRampType=((CAdamoAxis*)pDev)->GetInterRampType();
    AdamoParametri.AdamoAsse[nIndex].fpLimEAConc=((CAdamoAxis*)pDev)->GetLimEAConc();
    AdamoParametri.AdamoAsse[nIndex].fpLimEADisc=((CAdamoAxis*)pDev)->GetLimEADisc();
    AdamoParametri.AdamoAsse[nIndex].fpLimEAMin=((CAdamoAxis*)pDev)->GetLimEAMin();
    AdamoParametri.AdamoAsse[nIndex].nServoLimitType=((CAdamoAxis*)pDev)->GetServoLimitType ();
    AdamoParametri.AdamoAsse[nIndex].nEnableLimAxesPos=((CAdamoAxis*)pDev)->GetEnableLimAxesPos ();
    AdamoParametri.AdamoAsse[nIndex].nEnableLimAxesNeg=((CAdamoAxis*)pDev)->GetEnableLimAxesNeg ();
    AdamoParametri.AdamoAsse[nIndex].fpLimAxisPos=((CAdamoAxis*)pDev)->GetLimAxisPos();
    AdamoParametri.AdamoAsse[nIndex].fpLimAxisNeg=((CAdamoAxis*)pDev)->GetLimAxisNeg();
    AdamoParametri.AdamoAsse[nIndex].fpWinPos=((CAdamoAxis*)pDev)->GetWinPos();
    AdamoParametri.AdamoAsse[nIndex].fpWinNeg=((CAdamoAxis*)pDev)->GetWinNeg();
    AdamoParametri.AdamoAsse[nIndex].nDimBufferIntegrale=((CAdamoAxis*)pDev)->GetDimBufferIntegrale ();
    AdamoParametri.AdamoAsse[nIndex].fpPercentualeRampaTrapezoidale=((CAdamoAxis*)pDev)->GetPercRT ();
    AdamoParametri.AdamoAsse[nIndex].fpInterPercentualeRampaTrapezoidale=((CAdamoAxis*)pDev)->GetInterPercRT ();
    AdamoParametri.AdamoAsse[nIndex].nAsseGantry=((CAdamoAxis*)pDev)->GetGantryAxes ();
    AdamoParametri.AdamoAsse[nIndex].nPropGantry=((CAdamoAxis*)pDev)->GetPropGantry ();
    AdamoParametri.AdamoAsse[nIndex].nDerGantry=((CAdamoAxis*)pDev)->GetDerGantry ();
    AdamoParametri.AdamoAsse[nIndex].nIntGantry=((CAdamoAxis*)pDev)->GetIntGantry ();
    AdamoParametri.AdamoAsse[nIndex].nGSlot=((CAdamoAxis*)pDev)->GetGSlot ();
    AdamoParametri.AdamoAsse[nIndex].nControlType=((CAdamoAxis*)pDev)->GetTIC ();
    AdamoParametri.AdamoAsse[nIndex].nMovementCategory=((CAdamoAxis*)pDev)->GetMovementCategory ();
	AdamoParametri.AdamoAsse[nIndex].nAbilitazioneCompensazioneGioco=((CAdamoAxis*)pDev)->GetEnableRecuperoGioco ();
	AdamoParametri.AdamoAsse[nIndex].fpCompensazioneGioco=((CAdamoAxis*)pDev)->GetRecuperoGioco ();
	AdamoParametri.AdamoAsse[nIndex].fpTempoDiCompensazione=((CAdamoAxis*)pDev)->GetTempoDiRecupero ();
	AdamoParametri.AdamoAsse[nIndex].fpRangeQuotaNeg=((CAdamoAxis*)pDev)->GetRangeQuotaNeg ();
	AdamoParametri.AdamoAsse[nIndex].fpRangeQuotaPos=((CAdamoAxis*)pDev)->GetRangeQuotaPos ();
	AdamoParametri.AdamoAsse[nIndex].fpFiltroRotanti=((CAdamoAxis*)pDev)->GetFiltroRotanti ();
	AdamoParametri.AdamoAsse[nIndex].nTempoArrivoInQuota=((CAdamoAxis*)pDev)->GetTempoArrivoInQuota ();
	AdamoParametri.AdamoAsse[nIndex].nTempoAsseFermo=((CAdamoAxis*)pDev)->GetTempoAsseFermo ();
	AdamoParametri.AdamoAsse[nIndex].fpCostanteDiTempoAzionamento=((CAdamoAxis*)pDev)->GetCostanteDiTempoAzionamento ();
	AdamoParametri.AdamoAsse[nIndex].fpVelocitaDiCorrezione=((CAdamoAxis*)pDev)->GetVelocitaDiCorrezione ();
	AdamoParametri.AdamoAsse[nIndex].bCorrezioneDinamica=((CAdamoAxis*)pDev)->GetCorrezioneDinamica ();
	AdamoParametri.AdamoAsse[nIndex].nAbilitaCompensazioneEA=((CAdamoAxis*)pDev)->GetEnableCompensazioneEA ();
	AdamoParametri.AdamoAsse[nIndex].fpCompensazioneEA=((CAdamoAxis*)pDev)->GetCompensazioneEA ();
	AdamoParametri.AdamoAsse[nIndex].fpCompensazioneEAA=((CAdamoAxis*)pDev)->GetCompensazioneEAA ();
	AdamoParametri.AdamoAsse[nIndex].fpIntegrativaKT=((CAdamoAxis*)pDev)->GetIntKT ();
	AdamoParametri.AdamoAsse[nIndex].nEnableSwitchedIntegral=((CAdamoAxis*)pDev)->GetEnableSwitchedInt ();
	AdamoParametri.AdamoAsse[nIndex].fpSwitchedIngetegralSpeed=((CAdamoAxis*)pDev)->GetSwitchedIntSpeed ();
	AdamoParametri.AdamoAsse[nIndex].fpDerivativeN=((CAdamoAxis*)pDev)->GetDerN ();
	AdamoParametri.AdamoAsse[nIndex].fpInterIntegrativaKT=((CAdamoAxis*)pDev)->GetInterIntKT ();
	AdamoParametri.AdamoAsse[nIndex].nInterEnableSwitchedIntegral=((CAdamoAxis*)pDev)->GetInterEnableSwitchedInt ();
	AdamoParametri.AdamoAsse[nIndex].fpInterSwitchedIngetegralSpeed=((CAdamoAxis*)pDev)->GetInterSwitchedIntSpeed ();
	AdamoParametri.AdamoAsse[nIndex].fpInterDerivativeN=((CAdamoAxis*)pDev)->GetInterDerN ();
	AdamoParametri.AdamoAsse[nIndex].wFiltroCollegamentoEncoder=((CAdamoAxis*)pDev)->GetFiltroCollegamentoEncoder ();
	AdamoParametri.AdamoAsse[nIndex].wFiltroServoError=((CAdamoAxis*)pDev)->GetFiltroServoError ();
	memcpy (AdamoParametri.AdamoAsse[nIndex].stCD, ((CAdamoAxis*)pDev)->GetCD(), (sizeof (stCustomData)*MAX_AXIS_CUSTOM));
}

/*
** SetCanDeviceData
*/
void CAdamoDatiFile::SetCanDeviceData (CAdamoBase* pDev, int nIndex)
{
    strcpy (AdamoParametri.AdamoCanDevice[nIndex].szNome, pDev->GetPathName()+"."+pDev->GetName());
    strcpy (AdamoParametri.AdamoCanDevice[nIndex].szDescription, pDev->GetDesc ());
}

/*
** Reset : azzera tutti gli indici, consentendo alla Add di ripartire da zero.
*/
void CAdamoDatiFile::Reset ()
{
    memset (&IdRegistry, 0, sizeof (IdRegistry));
}

/*
** Refresh : la funzione scorre il file dati e aggiorna il file in base al modulo
**           passato per parametro.
*/
void CAdamoDatiFile::Refresh (CAdamoLogicModule* pModule)
{
    CAdamoBase* pDev;
    int i;
    bool bAssigned;

    /* azzeriamo l'indice del primo libero */
    bAssigned=false;
    IdRegistry.m_nInpDig=NMAXINPDIG;
    /* input digitali */
    for (i=0; i<NMAXINPDIG; i++)   {
        if (AdamoParametri.AdamoInputDig[i].szNome[0]!='\0')   {
            /* vediamo se il dispositivo e' ancora in configurazione */
            if ((pDev=(CAdamoBase*)pModule->GetItem(AdamoParametri.AdamoInputDig[i].szNome)) && pDev->GetType () == enInputDigitale)  {
                /* lo e', settiamo l'id */
                pDev->SetDeviceID (i);
                /* aggiorniamo ora i dati del dispositivo */
                SetInputDigData (pDev, i);
            }
            else   {
                /* non lo e', cancelliamolo dal file */
                AdamoParametri.AdamoInputDig[i].szNome[0]='\0';
                AdamoParametri.AdamoInputDig[i].szHWAddress[0]='\0';
                if (!bAssigned&&i<IdRegistry.m_nInpDig)   {
                    IdRegistry.m_nInpDig=i;
                    bAssigned=true;
                }
            }
        }
        else
            if (!bAssigned&&i<IdRegistry.m_nInpDig)   {
                IdRegistry.m_nInpDig=i;
                bAssigned=true;
            }
    }     
    /* output digitali */
    bAssigned=false;
    IdRegistry.m_nOutDig=NMAXOUTDIG;
    for (i=0; i<NMAXOUTDIG; i++)   {
        if (AdamoParametri.AdamoOutputDig[i].szNome[0]!='\0')   {
            /* vediamo se il dispositivo e' ancora in configurazione */
            if ((pDev=(CAdamoBase*)pModule->GetItem(AdamoParametri.AdamoOutputDig[i].szNome)) && pDev->GetType () == enOutputDigitale)  {
                /* lo e', settiamo l'id */
                pDev->SetDeviceID (i);
                /* aggiorniamo ora i dati del dispositivo */
                SetOutputDigData (pDev, i);
            }
            else   {
                /* non lo e', cancelliamolo dal file */
                AdamoParametri.AdamoOutputDig[i].szNome[0]='\0';
                AdamoParametri.AdamoOutputDig[i].szHWAddress[0]='\0';
                if (!bAssigned&&i<IdRegistry.m_nOutDig)   {
                    IdRegistry.m_nOutDig=i;
                    bAssigned=true;
                }
            }
        }
        else
            if (!bAssigned&&i<IdRegistry.m_nOutDig)   {
                IdRegistry.m_nOutDig=i;
                bAssigned=true;
            }
    }     
    /* input analogico */
    bAssigned=false;
    IdRegistry.m_nInpAnag=NMAXINPANA;
    for (i=0; i<NMAXINPANA; i++)   {
        if (AdamoParametri.AdamoInputAnag[i].szNome[0]!='\0')   {
            /* vediamo se il dispositivo e' ancora in configurazione */
            if ((pDev=(CAdamoBase*)pModule->GetItem(AdamoParametri.AdamoInputAnag[i].szNome)) && pDev->GetType () == enInputAnalogico)  {
                /* lo e', settiamo l'id */
                pDev->SetDeviceID (i);
                /* aggiorniamo ora i dati del dispositivo */
                SetInputAnagData (pDev, i);
            }
            else   {
                /* non lo e', cancelliamolo dal file */
                AdamoParametri.AdamoInputAnag[i].szNome[0]='\0';
                AdamoParametri.AdamoInputAnag[i].szHWAddress[0]='\0';
                if (!bAssigned&&i<IdRegistry.m_nInpAnag)   {
                    IdRegistry.m_nInpAnag=i;
                    bAssigned=true;
                }
            }
        }
        else
            if (!bAssigned&&i<IdRegistry.m_nInpAnag)   {
                IdRegistry.m_nInpAnag=i;
                bAssigned=true;
            }
    }     
    /* output analogico */
    bAssigned=false;
    IdRegistry.m_nOutAnag=NMAXOUTANA;
    for (i=0; i<NMAXOUTANA; i++)   {
        if (AdamoParametri.AdamoOutputAnag[i].szNome[0]!='\0')   {
            /* vediamo se il dispositivo e' ancora in configurazione */
            if ((pDev=(CAdamoBase*)pModule->GetItem(AdamoParametri.AdamoOutputAnag[i].szNome)) && pDev->GetType () == enOutputAnalogico)  {
                /* lo e', settiamo l'id */
                pDev->SetDeviceID (i);
                /* aggiorniamo ora i dati del dispositivo */
                SetOutputAnagData (pDev, i);
            }
            else   {
                /* non lo e', cancelliamolo dal file */
                AdamoParametri.AdamoOutputAnag[i].szNome[0]='\0';
                AdamoParametri.AdamoOutputAnag[i].szHWAddress[0]='\0';
                if (!bAssigned&&i<IdRegistry.m_nOutAnag)   {
                    IdRegistry.m_nOutAnag=i;
                    bAssigned=true;
                }
            }
        }
        else
            if (!bAssigned&&i<IdRegistry.m_nOutAnag)   {
                IdRegistry.m_nOutAnag=i;
                bAssigned=true;
            }
    }     
    /* counter */
    bAssigned=false;
    IdRegistry.m_nCounter=NMAXCOUNTER;
    for (i=0; i<NMAXCOUNTER; i++)   {
        if (AdamoParametri.AdamoCounter[i].szNome[0]!='\0')   {
            /* vediamo se il dispositivo e' ancora in configurazione */
            if ((pDev=(CAdamoBase*)pModule->GetItem(AdamoParametri.AdamoCounter[i].szNome)) && pDev->GetType () == enCounter)  {
                /* lo e', settiamo l'id */
                pDev->SetDeviceID (i);
                /* aggiorniamo ora i dati del dispositivo */
                SetCounterData (pDev, i);
            }
            else   {
                /* non lo e', cancelliamolo dal file */
                AdamoParametri.AdamoCounter[i].szNome[0]='\0';
                if (!bAssigned&&i<IdRegistry.m_nCounter)   {
                    IdRegistry.m_nCounter=i;
                    bAssigned=true;
                }
            }
        }
        else
            if (!bAssigned&&i<IdRegistry.m_nCounter)   {
                IdRegistry.m_nCounter=i;
                bAssigned=true;
            }
    }     
    /* timer */
    bAssigned=false;
    IdRegistry.m_nTimer=NMAXTIMER;
    for (i=0; i<NMAXTIMER; i++)   {
        if (AdamoParametri.AdamoTimer[i].szNome[0]!='\0')   {
            /* vediamo se il dispositivo e' ancora in configurazione */
            if ((pDev=(CAdamoBase*)pModule->GetItem(AdamoParametri.AdamoTimer[i].szNome)) && pDev->GetType () == enTimer)  {
                /* lo e', settiamo l'id */
                pDev->SetDeviceID (i);
                /* aggiorniamo ora i dati del dispositivo */
                SetTimerData (pDev, i);
            }
            else   {
                /* non lo e', cancelliamolo dal file */
                AdamoParametri.AdamoTimer[i].szNome[0]='\0';
                if (!bAssigned&&i<IdRegistry.m_nTimer)   {
                    IdRegistry.m_nTimer=i;
                    bAssigned=true;
                }
            }
        }
        else
            if (!bAssigned&&i<IdRegistry.m_nTimer)   {
                IdRegistry.m_nTimer=i;
                bAssigned=true;
            }
    }
    /* assi */
    bAssigned=false;
    IdRegistry.m_nAxes=NMAXAXIS;
    for (i=0; i<NMAXAXIS; i++)   {
        if (AdamoParametri.AdamoAsse[i].szNome[0]!='\0')   {
            /* vediamo se il dispositivo e' ancora in configurazione */
            if ((pDev=(CAdamoBase*)pModule->GetItem(AdamoParametri.AdamoAsse[i].szNome)) && pDev->GetType () == enAsse)  {
                /* lo e', settiamo l'id */
                pDev->SetDeviceID (i);
                /* aggiorniamo ora i dati del dispositivo */
                SetAxesData (pDev, i);
            }
            else   {
                /* non lo e', cancelliamolo dal file */
                AdamoParametri.AdamoAsse[i].szNome[0]='\0';
                AdamoParametri.AdamoAsse[i].szHWAddress[0]='\0';
                if (!bAssigned&&i<IdRegistry.m_nAxes)   {
                    IdRegistry.m_nAxes=i;
                    bAssigned=true;
                }
            }
        }
        else
            if (!bAssigned&&i<IdRegistry.m_nAxes)   {
                IdRegistry.m_nAxes=i;
                bAssigned=true;
            }
    }
    /* dispositivi generici can */
    bAssigned=false;
    IdRegistry.m_nCan=NMAXGENERICCAN;
    for (i=0; i<NMAXGENERICCAN; i++)   {
        if (AdamoParametri.AdamoCanDevice[i].szNome[0]!='\0')   {
            /* vediamo se il dispositivo e' ancora in configurazione */
            if ((pDev=(CAdamoBase*)pModule->GetItem(AdamoParametri.AdamoCanDevice[i].szNome)) && pDev->GetType () == enGenericCan)  {
                /* lo e', settiamo l'id */
                pDev->SetDeviceID (i);
                /* aggiorniamo ora i dati del dispositivo */
                SetCanDeviceData (pDev, i);
            }
            else   {
                /* non lo e', cancelliamolo dal file */
                AdamoParametri.AdamoCanDevice[i].szNome[0]='\0';
                AdamoParametri.AdamoCanDevice[i].szHWAddress[0]='\0';
                if (!bAssigned&&i<IdRegistry.m_nCan)   {
                    IdRegistry.m_nCan=i;
                    bAssigned=true;
                }
            }
        }
        else
            if (!bAssigned&&i<IdRegistry.m_nCan)   {
                IdRegistry.m_nCan=i;
                bAssigned=true;
            }
    }     
}

char* CAdamoDatiFile::GetName (char* szBuffer)
{
    char* p=strrchr (szBuffer, '.');
    return p ? p+1 : szBuffer;
}

int CAdamoDatiFile::GetDeviceName (int nDevice, int nTypeDevice, char* szBuffer)
{
    int nB=-1;
    switch (nTypeDevice)   {

        case enInputDigitale :
            if (AdamoParametri.AdamoInputDig[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, GetName (AdamoParametri.AdamoInputDig[nDevice].szNome));
                nB=0;
            }
            break;    
        case enOutputDigitale :
            if (AdamoParametri.AdamoOutputDig[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, GetName (AdamoParametri.AdamoOutputDig[nDevice].szNome));
                nB=0;
            }
            break;    
        case enInputAnalogico :
            if (AdamoParametri.AdamoInputAnag[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, GetName (AdamoParametri.AdamoInputAnag[nDevice].szNome));
                nB=0;
            }
            break;    
        case enOutputAnalogico :
            if (AdamoParametri.AdamoOutputAnag[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, GetName (AdamoParametri.AdamoOutputAnag[nDevice].szNome));
                nB=0;
            }
            break;    
        case enCounter :
            if (AdamoParametri.AdamoCounter[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, GetName (AdamoParametri.AdamoCounter[nDevice].szNome));
                nB=0;
            }
            break;    
        case enTimer :
            if (AdamoParametri.AdamoTimer[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, GetName (AdamoParametri.AdamoTimer[nDevice].szNome));
                nB=0;
            }
            break;    
        case enAsse :
            if (AdamoParametri.AdamoAsse[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, GetName (AdamoParametri.AdamoAsse[nDevice].szNome));
                nB=0;
            }
            break;    
        case enGenericCan :
            if (AdamoParametri.AdamoCanDevice[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, GetName (AdamoParametri.AdamoCanDevice[nDevice].szNome));
                nB=0;
            }
            break;
    }
    return nB;
}

int CAdamoDatiFile::GetDevicePathName (int nDevice, int nTypeDevice, char* szBuffer)
{
    int nB=-1;
    switch (nTypeDevice)   {

        case enInputDigitale :
            if (AdamoParametri.AdamoInputDig[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoInputDig[nDevice].szNome);
                nB=0;
            }
            break;    
        case enOutputDigitale :
            if (AdamoParametri.AdamoOutputDig[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoOutputDig[nDevice].szNome);
                nB=0;
            }
            break;    
        case enInputAnalogico :
            if (AdamoParametri.AdamoInputAnag[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoInputAnag[nDevice].szNome);
                nB=0;
            }
            break;    
        case enOutputAnalogico :
            if (AdamoParametri.AdamoOutputAnag[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoOutputAnag[nDevice].szNome);
                nB=0;
            }
            break;    
        case enCounter :
            if (AdamoParametri.AdamoCounter[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoCounter[nDevice].szNome);
                nB=0;
            }
            break;    
        case enTimer :
            if (AdamoParametri.AdamoTimer[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoTimer[nDevice].szNome);
                nB=0;
            }
            break;    
        case enAsse :
            if (AdamoParametri.AdamoAsse[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoAsse[nDevice].szNome);
                nB=0;
            }
            break;    
        case enGenericCan :
            if (AdamoParametri.AdamoCanDevice[nDevice].szNome[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoCanDevice[nDevice].szNome);
                nB=0;
            }
            break;
    }
    return nB;
}

int CAdamoDatiFile::GetDeviceHW (int nDevice, int nTypeDevice, char* szBuffer)
{
    int nB=-1;
    switch (nTypeDevice)   {

        case enInputDigitale :
            if (AdamoParametri.AdamoInputDig[nDevice].szHWAddress[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoInputDig[nDevice].szHWAddress);
                nB=0;
            }
            break;    
        case enOutputDigitale :
            if (AdamoParametri.AdamoOutputDig[nDevice].szHWAddress[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoOutputDig[nDevice].szHWAddress);
                nB=0;
            }
            break;    
        case enInputAnalogico :
            if (AdamoParametri.AdamoInputAnag[nDevice].szHWAddress[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoInputAnag[nDevice].szHWAddress);
                nB=0;
            }
            break;    
        case enOutputAnalogico :
            if (AdamoParametri.AdamoOutputAnag[nDevice].szHWAddress[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoOutputAnag[nDevice].szHWAddress);
                nB=0;
            }
            break;    
        case enAsse :
            if (AdamoParametri.AdamoAsse[nDevice].szHWAddress[0]!='\0')   {
                strcpy (szBuffer, AdamoParametri.AdamoAsse[nDevice].szHWAddress);
                nB=0;
            }
            break;    
    }
    return nB;
}

void CAdamoDatiFile::ResetHWAddress ()
{
	int i;

	for (i=0; i<NMAXINPDIG; i++)
		AdamoParametri.AdamoInputDig[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXOUTDIG; i++)
		AdamoParametri.AdamoOutputDig[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXINPANA; i++)
		AdamoParametri.AdamoInputAnag[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXOUTANA; i++)
		AdamoParametri.AdamoOutputAnag[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXAXIS; i++)
		AdamoParametri.AdamoAsse[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXENCODERS; i++)
		AdamoParametri.AdamoEncoder[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXGENERICCAN; i++)
		AdamoParametri.AdamoCanDevice[i].szHWAddress[0]='\0';
}

void CAdamoDatiFile::ResetEthHWAddress ()
{
	int i;

	for (i=0; i<NMAXINPDIG; i++)
		if (AdamoParametri.AdamoInputDig[i].szHWAddress[0]=='E')
			AdamoParametri.AdamoInputDig[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXOUTDIG; i++)
		if (AdamoParametri.AdamoOutputDig[i].szHWAddress[0]=='E')
			AdamoParametri.AdamoOutputDig[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXINPANA; i++)
		if (AdamoParametri.AdamoInputAnag[i].szHWAddress[0]=='E')
			AdamoParametri.AdamoInputAnag[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXOUTANA; i++)
		if (AdamoParametri.AdamoOutputAnag[i].szHWAddress[0]=='E')
			AdamoParametri.AdamoOutputAnag[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXAXIS; i++)
		if (AdamoParametri.AdamoAsse[i].szHWAddress[0]=='E')
			AdamoParametri.AdamoAsse[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXENCODERS; i++)
		if (AdamoParametri.AdamoEncoder[i].szHWAddress[0]=='E')
			AdamoParametri.AdamoEncoder[i].szHWAddress[0]='\0';
	for (i=0; i<NMAXGENERICCAN; i++)
		if (AdamoParametri.AdamoCanDevice[i].szHWAddress[0]=='E')
			AdamoParametri.AdamoCanDevice[i].szHWAddress[0]='\0';
}

/*
** AddAdamoDevice : aggiunge un dipositivo, assegnandone implicitamente l'indirizzo
**                  logico. Incrementa i contatori presenti in stIdRegistry.
*/
int CAdamoDatiFile::ForceAdamoDevice (CAdamoBase* pDev, CString strHWAddress)
{
    int n=0;

	if (pDev->GetDeviceID () != -1)   {
		int nIndex = pDev->GetDeviceID ();
		switch (pDev->GetType ())   {
			case enInputDigitale:
				SetInputDigData (pDev, nIndex);
				strcpy (AdamoParametri.AdamoInputDig[nIndex].szHWAddress, strHWAddress);
				break;
			case enOutputDigitale :
				SetOutputDigData (pDev, nIndex);
				strcpy (AdamoParametri.AdamoOutputDig[nIndex].szHWAddress, strHWAddress);
				break;
			case enInputAnalogico :
				SetInputAnagData (pDev, nIndex);
				strcpy (AdamoParametri.AdamoInputAnag[nIndex].szHWAddress, strHWAddress);
				break;
			case enOutputAnalogico :
				SetOutputAnagData (pDev, nIndex);
				strcpy (AdamoParametri.AdamoOutputAnag[nIndex].szHWAddress, strHWAddress);
				break;
			case enCounter :
				SetCounterData (pDev, nIndex);
				break;
			case enTimer :
				SetTimerData (pDev, nIndex);
				break;
			case enAsse :
				SetAxesData (pDev, nIndex);
				strcpy (AdamoParametri.AdamoAsse[nIndex].szHWAddress, strHWAddress);
				break;
			case enGenericCan :
				SetCanDeviceData (pDev, nIndex);
				break;
		}
	}
    return n;
}

CAdamoHWConfigFile::CAdamoHWConfigFile ()
{
    memset (&m_HWConf, 0, sizeof (stHWAdamoConf));
    for (int i=0; i<NMAX_SCHEDE; i++)   {
        m_HWConf.m_HWCanOpenNet[i].m_nBaudRate=2;
        m_HWConf.m_HWCanOpenNet[i].m_nSyncFrequency=20;
        m_HWConf.m_HWCanOpenNet[i].m_nSDOTimeout=1000;
    }
}

BOOL CAdamoHWConfigFile::Open (CPath pPath, bool bMode)
{
    BOOL b;
    /* se siamo in creazione azzeriamo tutti i registri */
    if (!bMode)
        memset (&m_HWConf, 0, sizeof (stHWAdamoConf));
    /* apriamo il file */
    b=CFile::Open (pPath, bMode ? CFile::typeBinary|CFile::modeReadWrite :
                                        CFile::typeBinary|CFile::modeCreate|CFile::modeReadWrite);
    /* creaimo subito la copia di backup, se siamo in creazione */
    if (!bMode)   {
        /* eseguiamo il flush dei dati */
        WriteData ();
        /* chiudiamo il file se no non possiamo copiarlo */
        Close ();
        /* cambiamo l'estensione del filozzo */
        CPath pPathDest (pPath);
        pPathDest.RenameExtension ("bck");
        /* copiamoci il filozzo */
        CopyFile ((const char*)pPath, (const char*)pPathDest, false);
        /* una bella ricorsioncina */
        b=Open (pPath, TRUE);
    }
    return b;
}

void CAdamoHWConfigFile::WriteData ()
{
    Write (&m_HWConf, sizeof (stHWAdamoConf));
}

void CAdamoHWConfigFile::ReadData  ()
{
    Read (&m_HWConf, sizeof (stHWAdamoConf));
}

void CAdamoHWConfigFile::CloseFile()
{
    CFile::Close ();
}

void CAdamoHWConfigFile::SetVariableData (int nID, int nVar, int nOffset, int nLenght, int nType)
{
	switch (nVar)   {
		case 0 :
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_nOffset=nOffset;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_nLength=nLenght;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_nType=nType;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_bConfigured=true;
			break;
		case 1 :
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_nOffset=nOffset;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_nLength=nLenght;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_nType=nType;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_bConfigured=true;
			break;
		case 2 :
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_nOffset=nOffset;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_nLength=nLenght;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_nType=nType;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_bConfigured=true;
			break;
		case 3 :
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_nOffset=nOffset;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_nLength=nLenght;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_nType=nType;
			m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_bConfigured=true;
			break;
		}
}

void CAdamoHWConfigFile::GetVariableData (int nID, int nVar, bool *pbConfigured, int *pnOffset, int *pnLenght, int *pnType)
{
	switch (nVar)   {
		case 0 :
			*pnOffset=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_nOffset;
			*pnLenght=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_nLength;
			*pnType  =m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_nType;
			*pbConfigured=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_bConfigured;
			break;
		case 1 :
			*pnOffset=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_nOffset;
			*pnLenght=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_nLength;
			*pnType  =m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_nType;
			*pbConfigured=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_bConfigured;
			break;
		case 2 :
			*pnOffset=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_nOffset;
			*pnLenght=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_nLength;
			*pnType  =m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_nType;
			*pbConfigured=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_bConfigured;
			break;
		case 3 :
			*pnOffset=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_nOffset;
			*pnLenght=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_nLength;
			*pnType  =m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_nType;
			*pbConfigured=m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_bConfigured;
			break;
		}
}

void CAdamoHWConfigFile::RemoveVariableData (int nID)
{
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_nOffset=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_nLength=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_nType=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpStatus.m_bConfigured=false;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_nOffset=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_nLength=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_nType=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_inpControl.m_bConfigured=false;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_nOffset=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_nLength=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_nType=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outStatus.m_bConfigured=false;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_nOffset=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_nLength=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_nType=0;
	m_HWConf.m_HWEthercatNet.HWEthAxis[nID].m_outControl.m_bConfigured=false;
}

void CAdamoHWConfigFile::CleanEthercatInfo ()
{ 
	int i, n;

	m_HWConf.m_HWEthercatNet.strMasterXML[0]='\0';
	m_HWConf.m_HWEthercatNet.m_nRequestedState=1;
	for (i=0; i<MAX_ETHERCAT_SLAVE; i++)  {
		for (n=0; n<MAX_ETHERCAT_SLAVE_TYPE; n++)
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].nDeviceType[n]=0; 
		for (n=0; n<MAX_ETHERCAT_ANALOG_DEVICE; n++)   {
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogInput.nHWAddress=0;
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogOutput.nHWAddress=0;
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogInput.nType=-1;
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogOutput.nType=-1;
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogInput.afpLLIAInf=0.0;
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogInput.afpLLIASup=0.0;
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogOutput.afpLLIAInf=0.0;
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogOutput.afpLLIASup=0.0;
		}
		for (n=0; n<MAX_ETHERCAT_SLAVE_TYPE; n++)
			m_HWConf.m_HWEthercatNet.HWEthSlave[i].EnableVariables[n].Reset ();
	}
	for (i=0; i<NMAXAXIS; i++)
		RemoveVariableData (i);
}

int CAdamoHWConfigFile::GetEthercatInpAnalogHW (int nSlave, int nHWAddress, double *fpLimInf, double *fpLimSup)
{
	int nB=-1;
	for (int i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
		if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.nHWAddress==nHWAddress)   {
			*fpLimInf=m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.afpLLIAInf;
			*fpLimSup=m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.afpLLIASup;
			nB=i; break;
		}	
	return nB;
}

int CAdamoHWConfigFile::GetEthercatOutAnalogHW (int nSlave, int nHWAddress, double *fpLimInf, double *fpLimSup)
{
	int nB=-1;
	for (int i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
		if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.nHWAddress==nHWAddress)   {
			*fpLimInf=m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.afpLLIAInf;
			*fpLimSup=m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.afpLLIASup;
			nB=i; break;
		}
	return nB;
}

int CAdamoHWConfigFile::SetEthercatInpAnalogHW (int nSlave, int nHWAddress, int nSelect, double fpLimInf, double fpLimSup)
{
	int i, nB=-1;
	for (i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
		if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.nHWAddress==nHWAddress)
			break;
	if (i==MAX_ETHERCAT_ANALOG_DEVICE)
		for (i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
			if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.nHWAddress==0)
				break;
	if (i!=MAX_ETHERCAT_ANALOG_DEVICE)   {
		m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.nHWAddress=nHWAddress;
		if (nSelect==0 || nSelect==2)
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.afpLLIAInf=fpLimInf;
		if (nSelect==1 || nSelect==2)
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.afpLLIASup=fpLimSup;
		nB=i;
	}
	return nB;
}

int CAdamoHWConfigFile::SetEthercatOutAnalogHW (int nSlave, int nHWAddress, int nSelect, double fpLimInf, double fpLimSup)
{
	int i, nB=-1;
	for (i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
		if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.nHWAddress==nHWAddress)
			break;
	if (i==MAX_ETHERCAT_ANALOG_DEVICE)
		for (i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
			if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.nHWAddress==0)
				break;
	if (i!=MAX_ETHERCAT_ANALOG_DEVICE)   {
		m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.nHWAddress=nHWAddress;
		if (nSelect==0 || nSelect==2)
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.afpLLIAInf=fpLimInf;
		if (nSelect==1 || nSelect==2)
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.afpLLIASup=fpLimSup;
		nB=i;
	}
	return nB;
}

int CAdamoHWConfigFile::SetEthercatInpAnalogType (int nSlave, int nHWAddress, int nType)
{
	int i, nB=-1;
	for (i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
		if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.nHWAddress==nHWAddress)
			break;
	if (i!=MAX_ETHERCAT_ANALOG_DEVICE)   {
		m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.nType=nType;
	}
	return nB;
}

int CAdamoHWConfigFile::SetEthercatOutAnalogType (int nSlave, int nHWAddress, int nType)
{
	int i, nB=-1;
	for (i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
		if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.nHWAddress==nHWAddress)
			break;
	if (i!=MAX_ETHERCAT_ANALOG_DEVICE)   {
		m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.nType=nType;
	}
	return nB;
}

void CAdamoHWConfigFile::RemoveEthercatInpAnalogHW (int nSlave, int nHWAddress)
{
	int i;

	for (i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
		if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.nHWAddress==nHWAddress)   {
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.nHWAddress=0;
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.nType = -1;
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.afpLLIAInf=0.0;
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogInput.afpLLIASup=0.0;
			break;
		}
}

void CAdamoHWConfigFile::RemoveEthercatOutAnalogHW (int nSlave, int nHWAddress)
{
	int i;

	for (i=0; i<MAX_ETHERCAT_ANALOG_DEVICE; i++)
		if (m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.nHWAddress==nHWAddress)   {
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.nHWAddress=0;
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.nType = -1;
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.afpLLIAInf=0.0;
			m_HWConf.m_HWEthercatNet.HWEthSlave[nSlave].EthAnalogDevice[i].AnalogOutput.afpLLIASup=0.0;
			break;
		}
}

BYTE CAdamoHWConfigFile::GetStationConf (int nCard, int nStation)
{
    int nIndex = nStation / 2;
    int nNibble = nStation % 2;
    return m_HWConf.m_HWSSCNETConf[nCard].m_achSSCNETConf[nIndex] >> nNibble * 4 & 0x0f;
}

void CAdamoHWConfigFile::SetStationConf (int nCard, int nStation, BYTE conf)
{
    int nIndex = nStation / 2;
    int nNibble = nStation % 2;
    m_HWConf.m_HWSSCNETConf[nCard].m_achSSCNETConf[nIndex] &= 0xf << abs (nNibble-1) * 4;
    m_HWConf.m_HWSSCNETConf[nCard].m_achSSCNETConf[nIndex] |= conf << nNibble * 4;
}

void CAdamoHWConfigFile::WriteHWConfigXML (CStdioFile *pXMLFile)
{
	WritePrologue (pXMLFile);
	WriteData (pXMLFile);
	WriteEpilogue (pXMLFile);
}

void CAdamoHWConfigFile::WritePrologue (CStdioFile *pXMLFile)
{
	pXMLFile->WriteString ("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
	pXMLFile->WriteString ("<Machine>\n");
}

void CAdamoHWConfigFile::WriteEpilogue (CStdioFile *pXMLFile)
{
	pXMLFile->WriteString ("</Machine>\n");
}

void CAdamoHWConfigFile::WriteData (CStdioFile *pXMLFile)
{
	for (int i = 0; i<NMAX_SCHEDE; i++)
		WriteChannelData (i, pXMLFile);
	WriteEthercatData (pXMLFile);
}

void CAdamoHWConfigFile::WriteChannelData (int nChannel, CStdioFile *pXMLFile)
{
	CString str;

	str.Format ("<Channel_%d>\n", nChannel);
	pXMLFile->WriteString (str);
	WriteCanOpenData (nChannel, pXMLFile);
	WriteSSCNETData (nChannel, pXMLFile);
	str.Format ("</Channel_%d>\n", nChannel);
	pXMLFile->WriteString (str);
}

void CAdamoHWConfigFile::WriteCanOpenData (int nChannel, CStdioFile *pXMLFile)
{
	CString str;

	pXMLFile->WriteString ("<CanOpen>\n");
	str.Format ("<Baudrate>%d</Baudrate>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_nBaudRate);
	pXMLFile->WriteString (str);
	str.Format ("<SyncFrequency>%d</SyncFrequency>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_nSyncFrequency);
	pXMLFile->WriteString (str);
	str.Format ("<SDOTimeout>%d</SDOTimeout>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_nSDOTimeout);
	pXMLFile->WriteString (str);
	str.Format ("<GuardTime>%d</GuardTime>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_nGuardTime);
	pXMLFile->WriteString (str);
	str.Format ("<LifeTime>%d</LifeTime>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_nLifeTime);
	pXMLFile->WriteString (str);
	str.Format ("<EnableWarning>%d</EnableWarning>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_nEnableWarning);
	pXMLFile->WriteString (str);
	for (int i = 1; i<=NMAX_CANNODES; i++)   {
		str.Format ("<Node_%d>\n", i);
		pXMLFile->WriteString (str);
		WriteCanOpenNode (nChannel, i-1, pXMLFile);
		str.Format ("</Node_%d>\n", i);
		pXMLFile->WriteString (str);
	}
	pXMLFile->WriteString ("</CanOpen>\n");
}

void CAdamoHWConfigFile::WriteCanOpenNode (int nChannel, int nNode, CStdioFile *pXMLFile)
{
	CString str;
	
	for (int i = 0; i<12; i++)   {
		str.Format ("<AnalogInput_%d>\n", i+1);
		pXMLFile->WriteString (str);
		str.Format ("<LowerLimit>%.3f</LowerLimit>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_HWCanOpenNode[nNode].afpLLIAInf[i]);
		pXMLFile->WriteString (str);
		str.Format ("<UpperLimit>%.3f</UpperLimit>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_HWCanOpenNode[nNode].afpLLIASup[i]);
		pXMLFile->WriteString (str);
		str.Format ("</AnalogInput_%d>\n", i+1);
		pXMLFile->WriteString (str);
		str.Format ("<AnalogOutput_%d>\n", i+1);
		pXMLFile->WriteString (str);
		str.Format ("<LowerLimit>%.3f</LowerLimit>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_HWCanOpenNode[nNode].afpLLOAInf[i]);
		pXMLFile->WriteString (str);
		str.Format ("<UpperLimit>%.3f</UpperLimit>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_HWCanOpenNode[nNode].afpLLOASup[i]);
		pXMLFile->WriteString (str);
		str.Format ("</AnalogOutput_%d>\n", i+1);
		pXMLFile->WriteString (str);
	}
	str.Format ("<NodeGuarding>%u</NodeGuarding>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_HWCanOpenNode[nNode].dwNodeGuarding);
	pXMLFile->WriteString (str);
	str.Format ("<HeartBeatTime>%u</HeartBeatTime>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_HWCanOpenNode[nNode].dwHeartBeatTime);
	pXMLFile->WriteString (str);
	str.Format ("<Virtual>%d</Virtual>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_HWCanOpenNode[nNode].bIsVirtual);
	pXMLFile->WriteString (str);
	str.Format ("<Disabled>%d</Disabled>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_HWCanOpenNode[nNode].bIsDisabled);
	pXMLFile->WriteString (str);
	str.Format ("<Polling>%d</Polling>\n", m_HWConf.m_HWCanOpenNet[nChannel].m_HWCanOpenNode[nNode].bIsPolling);
	pXMLFile->WriteString (str);
}

void CAdamoHWConfigFile::WriteSSCNETData (int nChannel, CStdioFile *pXMLFile)
{
	CString str;

	pXMLFile->WriteString ("<SSCNET>\n");
	str.Format ("<Frequency>%d</Frequency>\n", m_HWConf.m_HWSSCNETNet[nChannel].m_nFrequency);
	pXMLFile->WriteString (str);
	str.Format ("<EnableWarnings>%d</EnableWarnings>\n", m_HWConf.m_HWSSCNETNet[nChannel].m_bEnableWarnings);
	pXMLFile->WriteString (str);
	str.Format ("<NetworkEnabled>%d</NetworkEnabled>\n", m_HWConf.m_HWSSCNETNet[nChannel].m_bNetworkEnabled);
	pXMLFile->WriteString (str);
	str.Format ("<NetworkType>%d</NetworkType>\n", m_HWConf.m_HWSSCNETNet[nChannel].m_bNetworkType);
	pXMLFile->WriteString (str);
	for (int i = 0; i<NMAXMGASTATION; i++)   {
		str.Format ("<EnableAbsoluteEncoder_Station%d>%d</EnableAbsoluteEncoder_Station%d>\n", i+1, m_HWConf.m_HWSSCNETNet[nChannel].m_hWSSCNETStation[i].m_bAbsoluteEncoder, i+1);
		pXMLFile->WriteString (str);
		str.Format ("<HomePosition_Station%d>%.3f</HomePosition_Station%d>\n", i+1, m_HWConf.m_HWSSCNETNet[nChannel].m_hWSSCNETStation[i].m_fpHomePosition, i+1);
		pXMLFile->WriteString (str);
	}
	for (int i = 0; i<NMAXMGASTATION; i++)   {
		if (nChannel == 0)   {
			str.Format ("<OPOneRevPos_Station%d>%x</OPOneRevPos_Station%d>\n", i+1, m_HWConf.m_HWSSCNETAbsPos[i].dwOPOneRevPos, i+1);
			pXMLFile->WriteString (str);
			str.Format ("<MultiRevCounter_Station%d>%x</MultiRevCounter_Station%d>\n", i+1, m_HWConf.m_HWSSCNETAbsPos[i].wOPMultiRevCounter, i+1);
			pXMLFile->WriteString (str);
		}
		else   {
			str.Format ("<OPOneRevPos_Station%d>%x</OPOneRevPos_Station%d>\n", i+1, m_HWConf.m_HWSSCNETAbsPosMS[nChannel][i].dwOPOneRevPos, i+1);
			pXMLFile->WriteString (str);
			str.Format ("<MultiRevCounter_Station%d>%x</MultiRevCounter_Station%d>\n", i+1, m_HWConf.m_HWSSCNETAbsPosMS[nChannel][i].wOPMultiRevCounter, i+1);
			pXMLFile->WriteString (str);
		}
	}
	for (int i=0, n=0; i<NMAXMGASTATION / 2; i++, n+=2)   {
		str.Format ("<SSCNET_Conf_Station%d_%d>%x</SSCNET_Conf_Station%d_%d>\n", n+1, n+2, m_HWConf.m_HWSSCNETConf[nChannel].m_achSSCNETConf[i], n+1, n+2);
		pXMLFile->WriteString (str);
	}
	pXMLFile->WriteString ("</SSCNET>\n");
}

void CAdamoHWConfigFile::WriteEthercatData (CStdioFile *pXMLFile)
{
	CString str;

	pXMLFile->WriteString ("<Ethercat>\n");
	str.Format ("<Master>%s</Master>\n", m_HWConf.m_HWEthercatNet.strMasterXML);
	pXMLFile->WriteString (str);
	str.Format ("<Slave>%d</Slave>\n", m_HWConf.m_HWEthercatNet.nSlave);
	pXMLFile->WriteString (str);
	str.Format ("<RequestedState>%d</RequestedState>\n", m_HWConf.m_HWEthercatNet.m_nRequestedState);
	pXMLFile->WriteString (str);
	str.Format ("<Frequency>%d</Frequency>\n", m_HWConf.m_HWEthercatNet.m_nFrequency);
	pXMLFile->WriteString (str);
	str.Format ("<ContinousMeasuring>%d</ContinousMeasuring>\n", m_HWConf.m_HWEthercatNet.m_bContinuosMeasuring);
	pXMLFile->WriteString (str);
	str.Format ("<StartDCTiming>%I64d</StartDCTiming>\n", m_HWConf.m_HWEthercatNet.m_nStartDCTiming);
	pXMLFile->WriteString (str);
	str.Format ("<HPEDriver>%s</HPEDriver>\n", m_HWConf.m_HWEthercatNet2.strHpeDriver);
	pXMLFile->WriteString (str);
	str.Format ("<OffsetDCTiming>%d</OffsetDCTiming>\n", m_HWConf.m_HWEthercatNet2.m_nOffsetDCTiming);
	pXMLFile->WriteString (str);
	for (int i=0; i<NMAXAXIS; i++)   {
		str.Format ("<Axis_%d>\n", i);
		pXMLFile->WriteString (str);
		str.Format ("<InpStatus>\n");
		pXMLFile->WriteString (str);
		str.Format ("<Configured>%d</Configured>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_inpStatus.m_bConfigured);
		pXMLFile->WriteString (str);
		str.Format ("<Type>%d</Type>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_inpStatus.m_nType);
		pXMLFile->WriteString (str);
		str.Format ("<Offset>%d</Offset>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_inpStatus.m_nOffset);
		pXMLFile->WriteString (str);
		str.Format ("<Length>%d</Length>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_inpStatus.m_nLength);
		pXMLFile->WriteString (str);
		str.Format ("</InpStatus>\n");
		pXMLFile->WriteString (str);
		str.Format ("<InpControl>\n");
		pXMLFile->WriteString (str);
		str.Format ("<Configured>%d</Configured>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_inpControl.m_bConfigured);
		pXMLFile->WriteString (str);
		str.Format ("<Type>%d</Type>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_inpControl.m_nType);
		pXMLFile->WriteString (str);
		str.Format ("<Offset>%d</Offset>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_inpControl.m_nOffset);
		pXMLFile->WriteString (str);
		str.Format ("<Length>%d</Length>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_inpControl.m_nLength);
		pXMLFile->WriteString (str);
		str.Format ("</InpControl>\n");
		pXMLFile->WriteString (str);
		str.Format ("<OutStatus>\n");
		pXMLFile->WriteString (str);
		str.Format ("<Configured>%d</Configured>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_outStatus.m_bConfigured);
		pXMLFile->WriteString (str);
		str.Format ("<Type>%d</Type>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_outStatus.m_nType);
		pXMLFile->WriteString (str);
		str.Format ("<Offset>%d</Offset>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_outStatus.m_nOffset);
		pXMLFile->WriteString (str);
		str.Format ("<Length>%d</Length>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_outStatus.m_nLength);
		pXMLFile->WriteString (str);
		str.Format ("</OutStatus>\n");
		pXMLFile->WriteString (str);
		str.Format ("<OutControl>\n");
		pXMLFile->WriteString (str);
		str.Format ("<Configured>%d</Configured>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_outControl.m_bConfigured);
		pXMLFile->WriteString (str);
		str.Format ("<Type>%d</Type>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_outControl.m_nType);
		pXMLFile->WriteString (str);
		str.Format ("<Offset>%d</Offset>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_outControl.m_nOffset);
		pXMLFile->WriteString (str);
		str.Format ("<Length>%d</Length>\n", m_HWConf.m_HWEthercatNet.HWEthAxis[i].m_outControl.m_nLength);
		pXMLFile->WriteString (str);
		str.Format ("</OutControl>\n");
		pXMLFile->WriteString (str);
		str.Format ("</Axis_%d>\n", i);
		pXMLFile->WriteString (str);
	}
	for (int i=0; i<MAX_ETHERCAT_SLAVE; i++)   {
		str.Format ("<Slave_%d>\n", i);
		pXMLFile->WriteString (str);
		for (int n=0; n<MAX_ETHERCAT_SLAVE_TYPE; n++)   {
			str.Format ("<Type_%d>%d</Type_%d>\n", n, m_HWConf.m_HWEthercatNet.HWEthSlave[i].nDeviceType[n], n);
			pXMLFile->WriteString (str);
			for (int q = 0; q<4; q++)   {
				str.Format ("<EnableVariables_%d_%d>%I64x</EnableVariables_%d_%d>\n",
					n,
					q,
					m_HWConf.m_HWEthercatNet.HWEthSlave[i].EnableVariables[n].m_mask[q],
					n,
					q);
				pXMLFile->WriteString (str);
			}
		}
		for (int n=0; n<MAX_ETHERCAT_ANALOG_DEVICE; n++)   {
			str.Format ("<Analogue%d>\n", n);
			pXMLFile->WriteString (str);
			str.Format ("<Input_HWAddress>%d</Input_HWAddress>\n", m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogInput.nHWAddress);
			pXMLFile->WriteString (str);
			str.Format ("<Input_LimiteInferiore>%.3f</Input_LimiteInferiore>\n", m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogInput.afpLLIAInf);
			pXMLFile->WriteString (str);
			str.Format ("<Input_LimiteSuperiore>%.3f</Input_LimiteSuperiore>\n", m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogInput.afpLLIASup);
			pXMLFile->WriteString (str);
			str.Format ("<Output_HWAddress>%d</Output_HWAddress>\n", m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogOutput.nHWAddress);
			pXMLFile->WriteString (str);
			str.Format ("<Output_LimiteInferiore>%.3f</Output_LimiteInferiore>\n", m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogOutput.afpLLIAInf);
			pXMLFile->WriteString (str);
			str.Format ("<Output_LimiteSuperiore>%.3f</Output_LimiteSuperiore>\n", m_HWConf.m_HWEthercatNet.HWEthSlave[i].EthAnalogDevice[n].AnalogOutput.afpLLIASup);
			pXMLFile->WriteString (str);
			str.Format ("</Analogue%d>\n", n);
			pXMLFile->WriteString (str);
		}
		str.Format ("</Slave_%d>\n", i);
		pXMLFile->WriteString (str);
	}
	str.Format ("<HpeDriver>%s</HpeDriver>\n", m_HWConf.m_HWEthercatNet2.strHpeDriver);
	pXMLFile->WriteString (str);
	str.Format ("<OffsetDCTiming>%d</OffsetDCTiming>\n", m_HWConf.m_HWEthercatNet2.m_nOffsetDCTiming);
	pXMLFile->WriteString (str);
	pXMLFile->WriteString ("</Ethercat>\n");
}
