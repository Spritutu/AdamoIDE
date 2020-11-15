/*
 *
 */

/* include files */
#include "stdafx.h"
#include "mainframe.h"
#include "GridCellCombo.h"
#include "SamplesGrid.h"
#include "IdeMisc.h"
#include "gridcelltogglebutton.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/* external references */
CString *aszTipoSamples[MAXAXESDATA];
extern int anCodiciTipo[MAXAXESDATA];
extern int anCodiciTipoStringa[MAXAXESSTRINGDATA];

CSamplesGrid::CSamplesGrid () : m_nCount (0), m_bIsModified (false), m_nRow (-1), m_nCol (-1)
{
	memset (aszTipoSamples, 0, sizeof (aszTipoSamples));
}

void CSamplesGrid::InitControl ()
{
    int n;

    InitStrings ();
    m_slTipo.SetSize(sizeof (aszTipoSamples)/sizeof (CString*));
    for (n=0; n<sizeof (aszTipoSamples)/sizeof (CString*); n++)
        m_slTipo[n]=*aszTipoSamples[n];
	AddRow ();
}

void CSamplesGrid::AddDevice(CAdamoBase* pab)
{
    if (m_nCount<MAXSAMPLESNUMBER)   {
        if (pab->GetType()!=enBase)   {
            switch (pab->GetType())   {
                case enAsse :
                    AddAxDevice (pab);
                    break;
                default :
                    AddOtherDevice (pab);
                    break;
            }
			MoveToggleButton ();
            m_nCount++;
            m_bIsModified = true;
        }
    }
    else
        AfxMessageBox (LOADSTRING (IDS_ADAMOSAMPLESGRID_1), MB_ICONSTOP);
}

void CSamplesGrid::AddAxDevice(CAdamoBase* pab)
{
    SetCellType(m_nCount+1,2, RUNTIME_CLASS(CGridCellCombo));
    GetCell(m_nCount+1, 0)->SetText(pab->GetName());
    GetCell(m_nCount+1, 1)->SetText(pab->GetDesc());
    ((CGridCellCombo*)GetCell(m_nCount+1, 2))->SetOptions(m_slTipo);
    ((CGridCellCombo*)GetCell(m_nCount+1, 2))->SetStyle (CBS_DROPDOWNLIST);
    GetCell(m_nCount+1, 2)->SetText(*aszTipoSamples[0]);
    SetItemData (m_nCount+1, 0, (LPARAM)pab);
}

void CSamplesGrid::AddOtherDevice(CAdamoBase* pab)
{
    GetCell(m_nCount+1, 0)->SetText(pab->GetName());
    GetCell(m_nCount+1, 1)->SetText(pab->GetDesc());
    SetItemData (m_nCount+1, 0, (LPARAM)pab);
}

void CSamplesGrid::AddVariable (CString strVarName, int nRow, CString strColumnName, int nType)
{
	CString strRow;

	strRow.Format ("%d", nRow);
    GetCell(m_nCount+1, 3)->SetText(strVarName);
    GetCell(m_nCount+1, 4)->SetText(strRow);
    GetCell(m_nCount+1, 5)->SetText(strColumnName);
	CAdamoBase *pDevVar = new CAdamoBase;
	pDevVar->SetName (strVarName);
	pDevVar->SetDeviceID (nRow);
	pDevVar->SetDesc (strColumnName);
	pDevVar->SetIsPublic (nType);
    SetItemData (m_nCount+1, 0, (LPARAM)pDevVar);
    m_nCount++;
	AddRow ();
    m_bIsModified = true;
	Invalidate ();
}

void CSamplesGrid::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
    if (nRow<=m_nCount && nCol==2)   {
        CAdamoBase* pDev=(CAdamoBase*) GetItemData (nRow, 0);
		if (pDev)   {
			switch (pDev->GetType())   {
				case enAsse :
					m_nRow = nRow; m_nCol = nCol;
					CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
					m_strBeforeEditing=GetItemText (nRow, nCol);
					break;
				default :
					break;
			}
	    }
    }
}

int CSamplesGrid::GetSamples (CPtrList& p)
{
    int i, n;
    for (i=0; i<m_nCount; i++)   {
        CAdamoOscDato* pOsc=new CAdamoOscDato;
        memset (pOsc, 0, sizeof (CAdamoOscDato));
        CAdamoBase* pDev=(CAdamoBase*) GetItemData (i+1, 0);
		if (pDev && pDev->GetType () != enBase)   {
			/* campinamento di un dispositivo */
			pOsc->m_nDevice=pDev->GetDeviceID();
			pOsc->m_nTypeDevice=pDev->GetType();
			if (pOsc->m_nTypeDevice==enAsse)
				for (n=0; n<sizeof (aszTipoSamples)/sizeof (CString*); n++)   {
					CString str=GetItemText (i+1, 2);
					if (!strcmp (str, *aszTipoSamples[n]))   {
						pOsc->m_nDatoAsse=anCodiciTipo[n];
						break;
					}
				}
		}
		else   {
			/* campionamento di una variabile */
			CString strVarName = GetItemText (i+1, 3);
			CString strRow = GetItemText (i+1, 4);
			CString strColumnName = GetItemText (i+1, 5);
			CAdamoBase *pVarData = (CAdamoBase *) GetItemData (i+1, 0);
			char *p;

			strVarName.Trim ();
			strRow.Trim ();
			strColumnName.Trim ();
			if (strVarName != "")   {
				if (strVarName.GetLength () <= NMAXIDENTIFIER &&
					strColumnName.GetLength () <= NMAXIDENTIFIER)   {
						strcpy (pOsc->szVarName, strVarName);
						strcpy (pOsc->szColumnName, strColumnName);
						pOsc->m_nRow = strtol (strRow, &p, 10);
						pOsc->m_nVarType = pVarData->GetIsPublic ();
				}
				else
					continue;
			}
			else
				continue;
		}
        p.AddTail(pOsc);
    }
    return 0;
}

int CSamplesGrid::GetSamplesDevices (CPtrList& p)
{
    int i;

    for (i=0; i<m_nCount; i++)   {
        CAdamoBase* pDev=(CAdamoBase*) GetItemData (i+1, 0);
        p.AddTail(pDev);
    }
    return 0;
}

int CSamplesGrid::DeleteRow ()
{
    int n;
    if (m_nCount)   {
		CAdamoBase *pDevBase = (CAdamoBase *) GetItemData (m_nCount, 0);
		if (pDevBase && pDevBase->GetType () == enBase)
			delete pDevBase;
        for (n=0; n<6; n++)   {
            SetCellType(m_nCount, n, RUNTIME_CLASS(CGridCell));
            SetItemText (m_nCount, n, "");
			SetItemData (m_nCount, 0, NULL);
        }
        SetCellType(m_nCount+1, 3, RUNTIME_CLASS(CGridCell));
        SetCellType(m_nCount, 3, RUNTIME_CLASS(CGridCellToggleButton));
        m_nCount--;
        Invalidate ();
        m_bIsModified = true;
    }
    return m_nCount;
}

void CSamplesGrid::AddAsseFromFile (int nDatoAsse)
{
    GetCell(m_nCount, 2)->SetText(*aszTipoSamples[anCodiciTipoStringa[nDatoAsse]]);
}

/*
** CloseControl :
*/
void CSamplesGrid::CloseControl ()
{
    DestroyStrings ();
}

/*
** InitStrings : inizializza le stringhe della dialog
*/
void CSamplesGrid::InitStrings ()
{
    for (int i=0; i<MAXAXESDATA; i++)   {
        aszTipoSamples[i]=new CString;
        *aszTipoSamples[i]=LOADSTRING (IDS_OSC_SAMPLES_1+i);
    }
}

/*
** DestroyStrings : distrugge le stringhe della dialog
*/
void CSamplesGrid::DestroyStrings ()
{
    for (int i=0; i<MAXAXESDATA; i++)
        _delete (aszTipoSamples[i]);
}

/*
** OnEndEditCell :
*/
void CSamplesGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
	if (str!=m_strBeforeEditing.Trim ())
        m_bIsModified = true;
    CGridCtrl::OnEndEditCell(nRow, nCol, str);
	m_nRow = m_nCol = -1;
}

/*
** EndEdit :
*/
void CSamplesGrid::EndEdit ()
{
	if (m_nRow != -1 && m_nCol != -1)   {
		GetCell(m_nRow, m_nCol)->EndEdit ();
	}
}

/*
** InitRows :
*/
void CSamplesGrid::AddRow ()
{
	SetItemData (m_nCount + 1, 0, NULL);
    SetItemState(m_nCount + 1, 3, GetItemState(m_nCount + 1, 1) | GVIS_READONLY);
    SetCellType(m_nCount + 1, 3, RUNTIME_CLASS(CGridCellToggleButton));
}

/*
** UpdateRow
*/
void CSamplesGrid::UpdateRow ()
{
    SetItemState(m_nCount, 3, GetItemState(m_nCount + 1, 1) | GVIS_READONLY);
    SetCellType(m_nCount, 3, RUNTIME_CLASS(CGridCell));
	Invalidate ();
}

/*
** MoveToggleButton :
*/
void CSamplesGrid::MoveToggleButton ()
{
    SetCellType(m_nCount + 1, 3, RUNTIME_CLASS(CGridCell));
    SetCellType(m_nCount + 2, 3, RUNTIME_CLASS(CGridCellToggleButton));
}
