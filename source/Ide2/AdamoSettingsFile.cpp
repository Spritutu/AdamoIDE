/*
** AdamoSettingsFile : file che contiene i file dei settaggi.
*/

#include "stdafx.h"
#include <io.h>
#include "AdamoMachine.h"
#include "workspace.h"

CAdamoMachineSettings::CAdamoMachineSettings ()
{
}

BOOL CAdamoMachineSettings::Open ()
{
    int  nB;
    bool bMode=true;

    CProject* pActiveProject=GETWS()->GetActiveProject();
    CString strPathName=pActiveProject->GetSettingsPathName ();
    if (_access (strPathName, 0))   {
        memset (&m_as, 0, sizeof (stMachineSettings));
        bMode=false;    
    }
    /* apriamo il file */
    nB=CFile::Open (strPathName, bMode ? CFile::typeBinary|CFile::modeReadWrite :
                                        CFile::typeBinary|CFile::modeCreate|CFile::modeReadWrite);
    /* se il file e' appena stato creato, azzeriamone il contenuto */
    if (!bMode)
        Write (&m_as, sizeof (stMachineSettings));
    return nB;
}

void CAdamoMachineSettings::WriteMDSettings (stMDSettings* MDS)
{
    /* scriviamo i dati del manuale diagnostico */
    Seek (OFFSET_MD, CFile::begin);
    Write (MDS, sizeof (struct stMDSettings));
}

void CAdamoMachineSettings::ReadMDSettings (stMDSettings* MDS)
{
    /* leggiamo i dati del manuale diagnostico */
    Seek (OFFSET_MD, CFile::begin);
    Read (MDS, sizeof (struct stMDSettings));
}

void CAdamoMachineSettings::Close()
{
    CFile::Close ();
}

CAdamoPlantSettings::CAdamoPlantSettings ()
{
}

BOOL CAdamoPlantSettings::Open ()
{
    int  nB;
    bool bMode=true;

    CString strPathName=GETWS()->GetWorkspacePathName()+"\\"+PLANT_SETTINGS_FILE;
    if (_access (strPathName, 0))   {
        memset (&m_ps, 0, sizeof (stPlantSettings));
        m_ps.nDataVersion=GETWS()->GetProgramVersion ();
        bMode=false;    
    }
    /* apriamo il file */
    nB=CFile::Open (strPathName, bMode ? CFile::typeBinary|CFile::modeReadWrite :
                                        CFile::typeBinary|CFile::modeCreate|CFile::modeReadWrite);
    /* se il file e' appena stato creato, azzeriamone il contenuto */
    if (!bMode)
        Write (&m_ps, sizeof (stPlantSettings));
    return nB;
}

void CAdamoPlantSettings::WritePlantSettings ()
{
    /* scriviamo i dati del manuale diagnostico */
    Seek (0, CFile::begin);
    Write (&m_ps, sizeof (struct stPlantSettings));
}

void CAdamoPlantSettings::ReadPlantSettings ()
{
    /* leggiamo i dati del manuale diagnostico */
    Seek (0, CFile::begin);
    int nBytesRead=Read (&m_ps, sizeof (struct stPlantSettings));
    if (nBytesRead<sizeof (struct stPlantSettings))
        m_ps.nDataVersion=0x00010000;
}

void CAdamoPlantSettings::Close()
{
    CFile::Close ();
}
