/*
 *
 */

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* include files */
#include "mainframe.h"
#include "AdamoLib.h"
#include "TriggerGrid.h"
#include "GridCellCombo.h"
#include "GridCellName.h"
#include "IdeMisc.h"

/* external references */
extern int anCodiciTipo[MAXAXESDATA];

/* common data definitions */
static CString *aszTipoTrigger[MAXAXESDATA];
static CString *aszAzione[6];
static CString *aszConfrontoDig[2];
static CString *aszConfrontoDir[2];
static CString *aszConfrontoAssi[5];

CTriggerGrid::CTriggerGrid () : m_nCount(0), m_bIsModified (false)   {
    memset (m_nRowType, 0, sizeof (m_nRowType));
	memset (aszTipoTrigger, 0, sizeof (aszTipoTrigger));
	memset (aszAzione, 0, sizeof (aszAzione));
	memset (aszConfrontoDig, 0, sizeof (aszConfrontoDig));
	memset (aszConfrontoDir, 0, sizeof (aszConfrontoDir));
	memset (aszConfrontoAssi, 0, sizeof (aszConfrontoAssi));
}

void CTriggerGrid::AddAD (CAdamoBase* pab)
{
    if (m_nCount<MAXTRIGGERSNUMBER)   {
        SetCellType(m_nCount+1,2, RUNTIME_CLASS(CGridCell));
        SetCellType(m_nCount+1,3, RUNTIME_CLASS(CGridCell));
        SetCellType(m_nCount+1,4, RUNTIME_CLASS(CGridCellCombo));
        GetCell(m_nCount+1, 0)->SetText(pab->GetName());
        GetCell(m_nCount+1, 1)->SetText(pab->GetDesc());
        GetCell(m_nCount+1, 2)->SetText("");
        GetCell(m_nCount+1, 3)->SetText("=");
        ((CGridCellCombo*)GetCell(m_nCount+1, 4))->SetOptions(m_slConfrontoDig);
        ((CGridCellCombo*)GetCell(m_nCount+1, 4))->SetStyle (CBS_DROPDOWNLIST);
        GetCell(m_nCount+1, 4)->SetText(*aszConfrontoDig[0]);
        SetItemData (m_nCount+1, 0, (LPARAM)pab);
        m_nRowType[m_nCount]=RT_AD;
        m_nCount++;
        m_bIsModified = true;
    }
    else
        AfxMessageBox (LOADSTRING (IDS_ADAMOTRIGGERGRID_20), MB_ICONSTOP);
}

void CTriggerGrid::AddCompare (CAdamoBase* pab)
{
    if (m_nCount<MAXTRIGGERSNUMBER)   {
        SetCellType(m_nCount+1,2, RUNTIME_CLASS(CGridCell));
        SetCellType(m_nCount+1,3, RUNTIME_CLASS(CGridCellCombo));
        SetCellType(m_nCount+1,4, RUNTIME_CLASS(CGridCell));
        GetCell(m_nCount+1, 0)->SetText(pab->GetName());
        GetCell(m_nCount+1, 1)->SetText(pab->GetDesc());
        GetCell(m_nCount+1, 2)->SetText("");
        GetCell(m_nCount+1, 3)->SetText("");
        ((CGridCellCombo*)GetCell(m_nCount+1, 3))->SetOptions(m_slAzione);
        ((CGridCellCombo*)GetCell(m_nCount+1, 3))->SetStyle (CBS_DROPDOWNLIST);
        GetCell(m_nCount+1, 3)->SetText(*aszAzione[0]);
        SetItemData (m_nCount+1, 0, (LPARAM)pab);
        m_nRowType[m_nCount]=RT_CMP;
        m_nCount++;
        m_bIsModified = true;
    }
    else
        AfxMessageBox (LOADSTRING (IDS_ADAMOTRIGGERGRID_20), MB_ICONSTOP);
}

void CTriggerGrid::AddAsse (CAdamoBase* pab)
{
    if (m_nCount<MAXTRIGGERSNUMBER)   {
        SetCellType(m_nCount+1,2, RUNTIME_CLASS(CGridCellCombo));
        SetCellType(m_nCount+1,3, RUNTIME_CLASS(CGridCell));    /** **/
        SetCellType(m_nCount+1,4, RUNTIME_CLASS(CGridCellCombo));
        GetCell(m_nCount+1, 0)->SetText(pab->GetName());
        GetCell(m_nCount+1, 1)->SetText(pab->GetDesc());
        GetCell(m_nCount+1, 3)->SetText("=");
        ((CGridCellCombo*)GetCell(m_nCount+1, 2))->SetOptions(m_slTipo);
        ((CGridCellCombo*)GetCell(m_nCount+1, 2))->SetStyle (CBS_DROPDOWNLIST);
        GetCell(m_nCount+1, 2)->SetText(*aszTipoTrigger[0]);
        ((CGridCellCombo*)GetCell(m_nCount+1, 4))->SetOptions(m_slConfrontoAssi);
        ((CGridCellCombo*)GetCell(m_nCount+1, 4))->SetStyle (CBS_DROPDOWNLIST);
        GetCell(m_nCount+1, 4)->SetText(*aszConfrontoAssi[1]);
        SetItemData (m_nCount+1, 0, (LPARAM)pab);
        m_nRowType[m_nCount]=RT_AX;
        m_nCount++;
        m_bIsModified = true;
    }
    else
        AfxMessageBox (LOADSTRING (IDS_ADAMOTRIGGERGRID_20), MB_ICONSTOP);
}

void CTriggerGrid::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
    if (nRow<=m_nCount)   {
        switch (m_nRowType[nRow-1])   {
            case RT_AD :
                if (nCol==4)
                    CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
                break;
            case RT_CMP :
                if (nCol==4||nCol==3)
                    CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
                break;
            case RT_AX :
                if (GestisciAssi (nRow, nCol))
                    CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
                break;
        }
        m_strBeforeEditing=GetItemText (nRow, nCol);
    }
}

void CTriggerGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
    if (m_nRowType[m_nCount-1]==RT_AX&&nCol==2)   {
        if (str==*aszTipoTrigger[0])   {
            SetCellType(m_nCount,3, RUNTIME_CLASS(CGridCell));
            SetCellType(m_nCount,4, RUNTIME_CLASS(CGridCellCombo));
            GetCell(m_nCount, 3)->SetText("=");
            ((CGridCellCombo*)GetCell(m_nCount, 4))->SetOptions(m_slConfrontoAssi);
            ((CGridCellCombo*)GetCell(m_nCount, 4))->SetStyle (CBS_DROPDOWNLIST);
            GetCell(m_nCount, 4)->SetText(*aszConfrontoDig[0]);
        }
        else
            if (str==*aszTipoTrigger[1])   {
                SetCellType(m_nCount,3, RUNTIME_CLASS(CGridCell));
                SetCellType(m_nCount,4, RUNTIME_CLASS(CGridCellCombo));
                GetCell(m_nCount, 3)->SetText("=");
                ((CGridCellCombo*)GetCell(m_nCount, 4))->SetOptions(m_slConfrontoDir);
                ((CGridCellCombo*)GetCell(m_nCount, 4))->SetStyle (CBS_DROPDOWNLIST);
                GetCell(m_nCount, 4)->SetText(*aszConfrontoDir[0]);
            }
            else   {
                SetCellType(m_nCount,3, RUNTIME_CLASS(CGridCellCombo));
                SetCellType(m_nCount,4, RUNTIME_CLASS(CGridCell));
                ((CGridCellCombo*)GetCell(m_nCount, 3))->SetOptions(m_slAzione);
                ((CGridCellCombo*)GetCell(m_nCount, 3))->SetStyle (CBS_DROPDOWNLIST);
                GetCell(m_nCount, 3)->SetText(*aszAzione[0]);
                GetCell(m_nCount, 4)->SetText("");
            }
        Invalidate ();
    }
	if (str!=m_strBeforeEditing.Trim ())
        m_bIsModified = true;
    CGridCtrl::OnEndEditCell(nRow, nCol, str);
}

bool CTriggerGrid::GestisciAssi (int nRow, int nCol)
{     
    CString str=GetCell(nRow, 2)->GetText();
    if ((str==*aszTipoTrigger[0]&&nCol==3)||(str==*aszTipoTrigger[1]&&nCol==3))
        return false;
    return true;
}

void CTriggerGrid::InitControl ()
{
    int n;

    InitStrings ();
    m_slTipo.SetSize(sizeof (aszTipoTrigger)/sizeof (CString*));
    for (n=0; n<sizeof (aszTipoTrigger)/sizeof (CString*); n++)
        m_slTipo[n]=*aszTipoTrigger[n];
    
    m_slConfrontoAssi.SetSize(sizeof (aszConfrontoAssi)/sizeof (CString *));
    for (n=0; n<sizeof (aszConfrontoAssi)/sizeof (CString *); n++)
        m_slConfrontoAssi[n]=*aszConfrontoAssi[n];

    m_slAzione.SetSize(sizeof (aszAzione)/sizeof (CString *));
    for (n=0; n<sizeof (aszAzione)/sizeof (CString *); n++)
        m_slAzione[n]=*aszAzione[n];

    m_slConfrontoDig.SetSize(sizeof (aszConfrontoDig)/sizeof (CString *));
    for (n=0; n<sizeof (aszConfrontoDig)/sizeof (CString *); n++)
        m_slConfrontoDig[n]=*aszConfrontoDig[n];

    m_slConfrontoDir.SetSize(sizeof (aszConfrontoDir)/sizeof (CString *));
    for (n=0; n<sizeof (aszConfrontoDir)/sizeof (CString *); n++)
        m_slConfrontoDir[n]=*aszConfrontoDir[n];
}

void CTriggerGrid::CloseControl ()
{
    m_slTipo.RemoveAll();        
    m_slConfrontoAssi.RemoveAll();
    m_slAzione.RemoveAll();
    m_slConfrontoDig.RemoveAll();
    m_slConfrontoDir.RemoveAll();
    DestroyStrings ();
}

int CTriggerGrid::GetTriggers (CPtrList& p)
{
    int i, n, nB=0;

    for (i=0; i<m_nCount; i++)   {
        CAdamoOscEvento* pOsc=new CAdamoOscEvento;
        memset (pOsc, 0, sizeof (CAdamoOscEvento));
        CAdamoBase* pDev=(CAdamoBase*) GetItemData (i+1, 0);
        pOsc->m_nDevice=pDev->GetDeviceID();
        pOsc->m_nTypeDevice=pDev->GetType();
        if (pOsc->m_nTypeDevice==enAsse)   {
            CString str=GetItemText (i+1, 2);
            for (n=0; n<sizeof (aszTipoTrigger)/sizeof (CString*); n++)   {
                if (!strcmp (str, *aszTipoTrigger[n]))   {
                    pOsc->m_nDatoAsse=anCodiciTipo[n];
                    break;
                }
            }
            switch (pOsc->m_nDatoAsse)   {   
                case AX_ST :
                    pOsc->m_nDato=GetStatusCode (GetItemText (i+1, 4));
                    pOsc->m_nCmp=0x02;
                    break;
                case AX_DIR :
                    pOsc->m_nDato=GetDirectionCode (GetItemText (i+1, 4));
                    pOsc->m_nCmp=0x02;
                    break;
                case AX_QT :
                case AX_QR :
                case AX_QENC :
                case AX_EA :
                case AX_VC :
                case AX_VCR :
                case AX_ACCINST :
                case AX_ACCINSTR :
                case AX_JERKINST :
                case AX_JERKINSTR :
                case AX_FF :
                case AX_FFA :
                case AX_P :
                case AX_I :
                case AX_D :
                    pOsc->m_nCmp=GetCompare (GetItemText (i+1, 3));
                    nB=GetDoubleData (GetItemText (i+1, 4), &pOsc->m_fpDato);
                    break;
            }
        }
        else   {
            switch (pOsc->m_nTypeDevice)   {
                case enInputDigitale :
                    pOsc->m_nCmp=0x02;
                    pOsc->m_nDato=GetActiveCode (GetItemText (i+1, 4));
                    break;
                case enOutputDigitale :
                    pOsc->m_nCmp=0x02;
                    pOsc->m_nDato=GetActiveCode (GetItemText (i+1, 4));
                    break;
                case enInputAnalogico :
                    pOsc->m_nCmp=GetCompare (GetItemText (i+1, 3));
                    nB=GetDoubleData (GetItemText (i+1, 4), &pOsc->m_fpDato);
                    break;
                case enOutputAnalogico :
                    pOsc->m_nCmp=GetCompare (GetItemText (i+1, 3));
                    nB=GetDoubleData (GetItemText (i+1, 4), &pOsc->m_fpDato);
                    break;
                case enTimer :
                    pOsc->m_nCmp=GetCompare (GetItemText (i+1, 3));
                    nB=GetDoubleData (GetItemText (i+1, 4), &pOsc->m_fpDato);
                    break;
                case enCounter :
                    pOsc->m_nCmp=GetCompare (GetItemText (i+1, 3));
                    nB=GetDoubleData (GetItemText (i+1, 4), &pOsc->m_fpDato);
                    break;
                case enGenericCan :
                    pOsc->m_nCmp=GetCompare (GetItemText (i+1, 3));
                    nB=GetDoubleData (GetItemText (i+1, 4), &pOsc->m_fpDato);
                    break;
            }
        }
        p.AddTail(pOsc);
    }
    return nB;
}

int CTriggerGrid::GetStatusCode (const char* str)
{
    if (!strcmp (str, *aszConfrontoAssi[0]))
        return ACCEL;
    if (!strcmp (str, *aszConfrontoAssi[1]))
        return REGIME;
    if (!strcmp (str, *aszConfrontoAssi[2]))
        return DECEL;
    if (!strcmp (str, *aszConfrontoAssi[3]))
        return QUOTA;
    if (!strcmp (str, *aszConfrontoAssi[4]))
        return WINDG;
    return 0;
}

int CTriggerGrid::GetDirectionCode (const char* str)
{
    if (!strcmp (str, *aszConfrontoDir[0]))
        return POSDIR;
    if (!strcmp (str, *aszConfrontoDir[1]))
        return NEGDIR;
    return 0;
}

int CTriggerGrid::GetCompare (const char* str)
{
    if (!strcmp (str, *aszAzione[0]))
        return 0x02;
    if (!strcmp (str, *aszAzione[1]))
        return 0x04;
    if (!strcmp (str, *aszAzione[2]))
        return 0x01;
    if (!strcmp (str, *aszAzione[3]))
        return 0x06;
    if (!strcmp (str, *aszAzione[4]))
        return 0x3;
    if (!strcmp (str, *aszAzione[5]))
        return 0x05;
    return 0;
}

int CTriggerGrid::GetDoubleData (const char* str, double* fp)
{
    int nB=0;
    char* p, szBuffer[256];
    strcpy (szBuffer, str);
    *fp=strtod (strdblk (szBuffer), &p);
    if (*p!='\0')
        nB=-1;
    return nB;
}

int CTriggerGrid::GetActiveCode (const char* str)
{
    if (!strcmp (str, *aszConfrontoDig[0]))
        return 1;
    if (!strcmp (str, *aszConfrontoDig[1]))
        return 0;
    return 0;
}

int CTriggerGrid::DeleteRow ()
{
    int n;
    if (m_nCount)   {
        for (n=0; n<5; n++)   {
            SetCellType(m_nCount, n, RUNTIME_CLASS(CGridCell));
            SetItemText (m_nCount, n, "");
        }
        m_nCount--;
        Invalidate ();
        m_bIsModified = true;
    }
    return m_nCount;
}

void CTriggerGrid::AddADFromFile (int nValue)
{
    int nIndex=0;
    if (nValue==0)
        nIndex=1;
    GetCell(m_nCount, 3)->SetText("=");
    GetCell(m_nCount, 4)->SetText(*aszConfrontoDig[nIndex]);
}

void CTriggerGrid::AddAsseFromFile (int nDatoAsse, int nCmp, int nValue, double fpValue)
{
    int nIndex;
    switch (nDatoAsse)   {   
        case AX_ST :
            GetCell(m_nCount, 3)->SetText("=");
            switch (nValue)   {
                case ACCEL :
                    nIndex=0;
                    break;
                case REGIME :
                    nIndex=1;
                    break;
                case DECEL :
                    nIndex=2;
                    break;
                case QUOTA :
                    nIndex=3;
                    break;
                case WINDG :
                    nIndex=4;
                    break;
            }
            GetCell(m_nCount, 4)->SetText(*aszConfrontoAssi[nIndex]);
            break;
        case AX_DIR :
            GetCell(m_nCount, 3)->SetText("=");
            switch (nValue)   {
                case POSDIR :
                    nIndex=0;
                    break;
                case NEGDIR :
                    nIndex=1;
                    break;
            }
            GetCell(m_nCount, 4)->SetText(*aszConfrontoDir[nIndex]);
            break;
        case AX_QT :
        case AX_QR :
        case AX_QENC :
        case AX_EA :
        case AX_VC :
        case AX_VCR :
        case AX_ACCINST :
        case AX_ACCINSTR :
        case AX_JERKINST :
        case AX_JERKINSTR :
        case AX_FF :
        case AX_FFA :
        case AX_P :
        case AX_I :
        case AX_D :
            AddCompareFromFile (nCmp, fpValue);
            break;
    }
}

void CTriggerGrid::AddCompareFromFile (int nCmp, double fpValue)
{
    CString str;
    if (nCmp==0x02)
        str=*aszAzione[0];
    if (nCmp==0x04)
        str=*aszAzione[1];
    if (nCmp==0x01)
        str=*aszAzione[2];
    if (nCmp==0x06)
        str=*aszAzione[3];
    if (nCmp==0x03)
        str=*aszAzione[4];
    if (nCmp==0x05)
        str=*aszAzione[5];
    GetCell(m_nCount, 3)->SetText(str);
    str.Format ("%8.3f", fpValue);
    GetCell(m_nCount, 4)->SetText(str);
}

/*
** InitStrings : inizializza le stringhe della dialog
*/
void CTriggerGrid::InitStrings ()
{
    int i;

    for (i=0; i<MAXAXESDATA; i++)   {
        aszTipoTrigger[i]=new CString;
        *aszTipoTrigger[i]=LOADSTRING (IDS_ADAMODLGOSCILLOSCOPE_5+i);
    }
    for (i=0; i<6; i++)   {
        aszAzione[i]=new CString;
        *aszAzione[i]=LOADSTRING (IDS_ADAMOTRIGGERGRID_1+i);
    }
    for (i=0; i<2; i++)   {
        aszConfrontoDig[i]=new CString;
        *aszConfrontoDig[i]=LOADSTRING (IDS_ADAMOTRIGGERGRID_7+i);
    }
    for (i=0; i<2; i++)   {
        aszConfrontoDir[i]=new CString;
        *aszConfrontoDir[i]=LOADSTRING (IDS_ADAMOTRIGGERGRID_9+i);
    }
    for (i=0; i<5; i++)   {
        aszConfrontoAssi[i]=new CString;
        *aszConfrontoAssi[i]=LOADSTRING (IDS_ADAMOTRIGGERGRID_11+i);
    }
}

/*
** DestroyStrings : distrugge le stringhe della dialog
*/
void CTriggerGrid::DestroyStrings ()
{
    int i;

    for (i=0; i<MAXAXESDATA; i++)
        _delete (aszTipoTrigger[i]);
    for (i=0; i<6; i++)
        _delete (aszAzione[i]);
    for (i=0; i<2; i++)
        _delete (aszConfrontoDig[i]);
    for (i=0; i<2; i++)
        _delete (aszConfrontoDir[i]);
    for (i=0; i<5; i++)
        _delete (aszConfrontoAssi[i]);
}
