/*
** ConfigHWGrid.cpp
*/

#include "stdafx.h"
#include "mainframe.h"
#include "WorkSpace.h"
#include "Project.h"
#include "ConfigHWGrid.h"

IMPLEMENT_DYNCREATE(CConfigHWGridCell, CGridCell)

CConfigHWGrid::CConfigHWGrid () : m_pConfigDoc (NULL), m_nst (0), m_eType (enBase), m_nMode (0), m_pPrj (NULL)
{
    ShowAlwaysTitleTips ();
}

/*
** OnDrop :
*/
BOOL CConfigHWGrid::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
    CString strMacchina, strName, strDescription, strPercorso, strHWAddress;
    int nType, nID, nBits, n;
    BOOL b=FALSE;    

    CCellID cid=GetCellFromPt (point);
    if (IsValidCell (cid, &n))   {
        if (!GetDragData (pDataObject, &nType, &nID, &nBits, &strMacchina, &strName, &strDescription, &strPercorso))   {
			if (m_pPrj->GetMachineModule ()->GetName () == strMacchina)   {
            if (nType==m_eType)   {
                if (!AlreadyAssigned (m_stConfigHW[n].m_strHWAddr))   {
					if (!OnConnected (cid.row, cid.col, m_stConfigHW[n].m_strHWAddr))   {
						if (!SetDeviceInfo (nID, m_stConfigHW[n].m_strHWAddr))   {
							AssignDevice (cid, strName, strDescription, strPercorso, nID, nBits);
							((CMainFrame *)AfxGetMainWnd ())->Connect ();
								m_pPrj->SetHWConfigModified ();
							}
						}
					}
                }
            }
        }
    }
    OnDragLeave();
    return b;
}

/*
** SetGridData :
*/
void CConfigHWGrid::SetGridData  (int nRow, int nCol, int nRowDesc, int nColDesc, CString strHWAddr)
{
    m_stConfigHW[m_nst].m_nCol=nCol;
    m_stConfigHW[m_nst].m_nRow=nRow;
    m_stConfigHW[m_nst].m_nColDesc=nColDesc;
    m_stConfigHW[m_nst].m_nRowDesc=nRowDesc;
    m_stConfigHW[m_nst].m_strHWAddr=strHWAddr;
    m_nst++;
}

/*
** IsValidCell :
*/
bool CConfigHWGrid::IsValidCell (CCellID &cid, int *n)
{
    bool b=false;

    for (int i=0; i<m_nst; i++)   {
        if (cid.row==m_stConfigHW[i].m_nRow && cid.col==m_stConfigHW[i].m_nCol)   {
            *n=i;
            b=true; break;
        }
    }
    return b;
}

/*
** GetDragData :
*/
int CConfigHWGrid::GetDragData (COleDataObject* pDataObject, int *nType, int *nID, int *nNumBits, CString* strMacchina, CString *strName, CString *strDescription, CString *strPercorso)
{
    int nB=-1;
    HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
    CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold this.
    LPSTR szBuffer = new char[::GlobalSize(hmem)];

    sf.Read(szBuffer, ::GlobalSize(hmem));
    ::GlobalUnlock(hmem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems
    char *p=strchr (szBuffer, '#');
    if (p)   {
        char *q=strchr (p+1, '#');
        char *r=strchr (q+1, '#');
        *p='\0'; *q='\0'; *r='\0';
        *strPercorso=szBuffer;
        char *t=strrchr (szBuffer, '.');
		char *o=strchr (szBuffer, '.');
		*o = '\0';
		*strMacchina= szBuffer;
        if (t)   {
            *strName=t+1;
            *strDescription=p+1;
            *nNumBits=atoi (q+1);
            char *s=strchr (r+1, '-');
            *s='\0';
            *nType=atoi (r+1);
            *nID=atoi (s+1);
            nB=0;
        }
    }
    delete szBuffer;
    return nB;
}

/*
** SetDeviceInfo :
*/
int CConfigHWGrid::SetDeviceInfo (int nID, CString strHWAddress)
{
    CAdamoDatiFile *adf=m_pPrj->GetAdamoDatiFile ();
    stAdamoParametri *pap=adf->GetAdamoParametri ();
    int nB=-1;
    switch (m_eType)   {
        case enInputDigitale :
            if (pap->AdamoInputDig[nID].szNome[0]!='\0')
                if (pap->AdamoInputDig[nID].szHWAddress[0]=='\0')   {
                    strcpy (pap->AdamoInputDig[nID].szHWAddress, strHWAddress);
                    nB=0;
                }
            break;    
        case enOutputDigitale :
            if (pap->AdamoOutputDig[nID].szNome[0]!='\0')
                if (pap->AdamoOutputDig[nID].szHWAddress[0]=='\0')   {
                    strcpy (pap->AdamoOutputDig[nID].szHWAddress, strHWAddress);
                    nB=0;
                }
            break;    
        case enInputAnalogico :
            if (pap->AdamoInputAnag[nID].szNome[0]!='\0')
                if (pap->AdamoInputAnag[nID].szHWAddress[0]=='\0')   {
                    strcpy (pap->AdamoInputAnag[nID].szHWAddress, strHWAddress);
                    nB=0;
                }
            break;    
        case enOutputAnalogico :
            if (pap->AdamoOutputAnag[nID].szNome[0]!='\0')
                if (pap->AdamoOutputAnag[nID].szHWAddress[0]=='\0')   {
                    strcpy (pap->AdamoOutputAnag[nID].szHWAddress, strHWAddress);
                    nB=0;
                }
            break;    
        case enAsse :
            if (pap->AdamoAsse[nID].szNome[0]!='\0')
                if (pap->AdamoAsse[nID].szHWAddress[0]=='\0')   {
                    strcpy (pap->AdamoAsse[nID].szHWAddress, strHWAddress);
                    nB=0;
                }
            break;    
        case enGenericCan :
            if (pap->AdamoCanDevice[nID].szNome[0]!='\0')
                if (pap->AdamoCanDevice[nID].szHWAddress[0]=='\0')   {
                    strcpy (pap->AdamoCanDevice[nID].szHWAddress, strHWAddress);
                    nB=0;
                }
            break;
    }
    return nB;
}

/*
** AssignDevice :
*/
void CConfigHWGrid::AssignDevice (CCellID &cid, CString strName, CString strDescription, CString strPercorso, int nID, int nBits)
{
    int n;

    IsValidCell (cid, &n);
    for (int i=0; i<nBits; i++)   {
        SetItemText (m_stConfigHW[n+i].m_nRow, m_stConfigHW[n+i].m_nCol, strName);
		SetItemData (m_stConfigHW[n+i].m_nRow, m_stConfigHW[n+i].m_nCol, nID);
        if (m_stConfigHW[n+i].m_nColDesc!=-1 && m_stConfigHW[n+i].m_nRowDesc!=-1)
            SetItemText (m_stConfigHW[n+i].m_nRowDesc, m_stConfigHW[n+i].m_nColDesc, strDescription);
        CConfigHWGridCell *pHWGridCell=(CConfigHWGridCell *)GetCell (cid.row, cid.col);
        if (pHWGridCell)
            pHWGridCell->SetTipText (strPercorso);
    }
    Invalidate ();
}

/*
** UpdateData :
*/
void CConfigHWGrid::UpdateData ()
{
    for (int i=0; i<m_nst; i++)   {
        GetCell (m_stConfigHW[i].m_nRow, m_stConfigHW[i].m_nCol)->SetBackClr (RGB (0xE0, 0xE0, 0xFF));
    }
    LoadHWConfig ();
}

/*
** OnEditCell :
*/
void CConfigHWGrid::OnEditCell (int nRow, int nCol, CPoint point, UINT nChar)
{
    if (m_nMode==2)
        if (GetItemText (nRow, 1)!="")
            CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
}

/*
** LoadHWConfig :
*/
void CConfigHWGrid::LoadHWConfig ()
{
    int i, n;
    CAdamoDatiFile *adf=m_pPrj->GetAdamoDatiFile ();
    stAdamoParametri *pap=adf->GetAdamoParametri ();
    int nB=-1;
    switch (m_eType)   {
        case enInputDigitale :
            for (i=0; i<NMAXINPDIG; i++)   {
                if (pap->AdamoInputDig[i].szNome[0]!='\0')   {
                    for (n=0; n<m_nst; n++)
                        if (!strcmp (pap->AdamoInputDig[i].szHWAddress, m_stConfigHW[n].m_strHWAddr))   {
                             AssignDeviceOnLoad (pap->AdamoInputDig[i].szNome, pap->AdamoInputDig[i].szDescription, m_stConfigHW[n].m_strHWAddr, i, n, 1); break;
                        }
                }
            }
            break;    
        case enOutputDigitale :
            for (i=0; i<NMAXOUTDIG; i++)   {
                if (pap->AdamoOutputDig[i].szNome[0]!='\0')   {
                    for (n=0; n<m_nst; n++)
                        if (!strcmp (pap->AdamoOutputDig[i].szHWAddress, m_stConfigHW[n].m_strHWAddr))   {
                             AssignDeviceOnLoad (pap->AdamoOutputDig[i].szNome, pap->AdamoOutputDig[i].szDescription, m_stConfigHW[n].m_strHWAddr, i, n, 1); break;
                        }
                }
            }
            break;    
        case enInputAnalogico :
            for (i=0; i<NMAXINPANA; i++)   {
                if (pap->AdamoInputAnag[i].szNome[0]!='\0')   {
                    for (n=0; n<m_nst; n++)
                        if (!strcmp (pap->AdamoInputAnag[i].szHWAddress, m_stConfigHW[n].m_strHWAddr))   {
                             AssignDeviceOnLoad (pap->AdamoInputAnag[i].szNome, pap->AdamoInputAnag[i].szDescription, m_stConfigHW[n].m_strHWAddr, i, n, 1); break;
                        }
                }
            }
            break;    
        case enOutputAnalogico :
            for (i=0; i<NMAXOUTANA; i++)   {
                if (pap->AdamoOutputAnag[i].szNome[0]!='\0')   {
                    for (n=0; n<m_nst; n++)
                        if (!strcmp (pap->AdamoOutputAnag[i].szHWAddress, m_stConfigHW[n].m_strHWAddr))   {
                             AssignDeviceOnLoad (pap->AdamoOutputAnag[i].szNome, pap->AdamoOutputAnag[i].szDescription, m_stConfigHW[n].m_strHWAddr, i, n, 1); break;
                        }
                }
            }
            break;    
        case enAsse :
            for (i=0; i<NMAXAXIS; i++)   {
                if (pap->AdamoAsse[i].szNome[0]!='\0')   {
                    for (n=0; n<m_nst; n++)
                        if (!strcmp (pap->AdamoAsse[i].szHWAddress, m_stConfigHW[n].m_strHWAddr))   {
                             AssignDeviceOnLoad (pap->AdamoAsse[i].szNome, pap->AdamoAsse[i].szDescription, m_stConfigHW[n].m_strHWAddr, i, n, 1); break;
                        }
                }
            }
            break;    
        case enGenericCan :
            for (i=0; i<NMAXGENERICCAN; i++)   {
                if (pap->AdamoCanDevice[i].szNome[0]!='\0')   {
                    for (n=0; n<m_nst; n++)
                        if (!strcmp (pap->AdamoCanDevice[i].szHWAddress, m_stConfigHW[n].m_strHWAddr))   {
                             AssignDeviceOnLoad (pap->AdamoCanDevice[i].szNome, pap->AdamoCanDevice[i].szDescription, m_stConfigHW[n].m_strHWAddr, i, n, 1); break;
                        }
                }
            }
            break;
    }
}

/*
** AssignDeviceOnLoad :
*/
void CConfigHWGrid::AssignDeviceOnLoad (CString strName, CString strDescription, CString strHWAddress, int nID, int n, int nBits)
{
    CString strDescTrad;
    CString strMessaggio, strDescrizione;
    CCellID cid (m_stConfigHW[n].m_nRow, m_stConfigHW[n].m_nCol);
    LONG dw;

    CProject *pPrj=GETWS ()->GetActiveProject ();
    if (pPrj)   {
        CTraduzioni* pTR=pPrj->GetTraduzioni ();
        if (pTR->MessaggioStringa (1, strName, &dw, &strMessaggio, &strDescrizione))
            strDescTrad=strMessaggio;
    }
    AssignHWAnalogRange (n);
    CAdamoDatiFile *pADF=m_pPrj->GetAdamoDatiFile ();
    strMessaggio=pADF->GetName (strName.GetBuffer (0));
    strName.ReleaseBuffer ();
    AssignDevice (cid, strMessaggio, strDescTrad, strName, nID, nBits);
	OnConnected (m_stConfigHW[n].m_nRow, m_stConfigHW[n].m_nCol, strHWAddress);
}

/*
** RemoveCurrentConnection : 
*/
void CConfigHWGrid::RemoveCurrentConnection ()
{
    CString strName;
    CCellID cell;
    int nBits, nState;

    for (int i=0; i<m_nst; i++)   {
        nState=GetItemState (m_stConfigHW[i].m_nRow, m_stConfigHW[i].m_nCol);
        if (nState & GVNI_SELECTED)   {
			if (!OnRemoveConnection (m_stConfigHW[i].m_nRow, m_stConfigHW[i].m_nCol, m_stConfigHW[i].m_strHWAddr))   {
				if (CleanDeviceInfo (m_stConfigHW[i].m_strHWAddr, &strName, &nBits))   {
					DisassignDevice (i, nBits);
					((CMainFrame *)AfxGetMainWnd ())->Disconnect (strName);
					m_pPrj->SetHWConfigModified ();
				}
			}
        }
    }
    Invalidate ();
}

/*
** CleanDeviceInfo : 
*/
bool CConfigHWGrid::CleanDeviceInfo (CString strHWAddr, CString *strName, int *nBits)
{
    int i;
    bool b=false;
    CAdamoDatiFile *adf=m_pPrj->GetAdamoDatiFile ();
    stAdamoParametri *pap=adf->GetAdamoParametri ();

    switch (m_eType)   {
        case enInputDigitale :
            for (i=0; i<NMAXINPDIG; i++)   {
                if (!strcmp (pap->AdamoInputDig[i].szHWAddress, strHWAddr))   {
                    pap->AdamoInputDig[i].szHWAddress[0]='\0';
                    *strName=pap->AdamoInputDig[i].szNome;
                    *nBits=pap->AdamoInputDig[i].nNumInputDig;
                    b=true;
                }
            }
            break;    
        case enOutputDigitale :
            for (i=0; i<NMAXOUTDIG; i++)   {
                if (!strcmp (pap->AdamoOutputDig[i].szHWAddress, strHWAddr))   {
                    pap->AdamoOutputDig[i].szHWAddress[0]='\0';
                    *strName=pap->AdamoOutputDig[i].szNome;
                    *nBits=pap->AdamoOutputDig[i].nNumOutputDig;
                    b=true;
                }
            }
            break;    
        case enInputAnalogico :
            for (i=0; i<NMAXINPANA; i++)   {
                if (!strcmp (pap->AdamoInputAnag[i].szHWAddress, strHWAddr))   {
                    pap->AdamoInputAnag[i].szHWAddress[0]='\0';
                    *strName=pap->AdamoInputAnag[i].szNome;
                    *nBits=1;
                    b=true;
                }
            }
            break;    
        case enOutputAnalogico :
            for (i=0; i<NMAXOUTANA; i++)   {
                if (!strcmp (pap->AdamoOutputAnag[i].szHWAddress, strHWAddr))   {
                    pap->AdamoOutputAnag[i].szHWAddress[0]='\0';
                    *strName=pap->AdamoOutputAnag[i].szNome;
                    *nBits=1;
                    b=true;
                }
            }
            break;    
        case enAsse :
            for (i=0; i<NMAXAXIS; i++)   {
                if (!strcmp (pap->AdamoAsse[i].szHWAddress, strHWAddr))   {
                    pap->AdamoAsse[i].szHWAddress[0]='\0';
                    *strName=pap->AdamoAsse[i].szNome;
                    *nBits=1;
                    b=true;
                }
            }
            break;    
        case enGenericCan :
            for (i=0; i<NMAXGENERICCAN; i++)   {
                if (!strcmp (pap->AdamoCanDevice[i].szHWAddress, strHWAddr))   {
                    pap->AdamoCanDevice[i].szHWAddress[0]='\0';
                    *strName=pap->AdamoCanDevice[i].szNome;
                    *nBits=1;
                    b=true;
                }
            }
            break;
    }
    return b;
}

/*
** DisassignDevice : 
*/
void CConfigHWGrid::DisassignDevice (int n, int nBits)
{
    for (int i=0; i<nBits; i++)   {
        SetItemText (m_stConfigHW[n+i].m_nRow, m_stConfigHW[n+i].m_nCol, "");
		SetItemData (m_stConfigHW[n+i].m_nRow, m_stConfigHW[n+i].m_nCol, NULL);
        if (m_stConfigHW[n+i].m_nColDesc!=-1 && m_stConfigHW[n+i].m_nRow!=-1)
            SetItemText (m_stConfigHW[n+i].m_nRowDesc, m_stConfigHW[n+i].m_nColDesc, "");
        CConfigHWGridCell *pHWGridCell=(CConfigHWGridCell *)GetCell (m_stConfigHW[n+i].m_nRow, m_stConfigHW[n+i].m_nCol);
        if (pHWGridCell)
            pHWGridCell->SetTipText ("");
    }
    if (m_nMode==2)
        DisassignHWAnalogRange (m_stConfigHW[n].m_nRow);
}

/*
** Find :
*/
bool CConfigHWGrid::Find (const char *szBuffer)
{
    bool b=false;
    ClearSelection ();
    for (int i=0; i<m_nst; i++)   {
        if (!strcmp (szBuffer, m_stConfigHW[i].m_strHWAddr))   {
            if (m_stConfigHW[i].m_nRowDesc==-1 && m_stConfigHW[i].m_nColDesc==-1)
                SetSelectedRange (m_stConfigHW[i].m_nRow, m_stConfigHW[i].m_nCol, m_stConfigHW[i].m_nRow, m_stConfigHW[i].m_nCol, TRUE, TRUE);
            else
                SetSelectedRange (m_stConfigHW[i].m_nRow, m_stConfigHW[i].m_nCol, m_stConfigHW[i].m_nRowDesc, m_stConfigHW[i].m_nColDesc, TRUE, TRUE);
            b=true;
        }
    }
    return b;
}

/*
** ClearSelection :
*/
void CConfigHWGrid::ClearSelection ()
{
    for (int i=1; i<GetRowCount (); i++)   {
        for (int n=1; n<GetColumnCount (); n++)   {
            int nState=GetItemState (i, n);
            nState=nState & (~GVNI_SELECTED);
            nState=nState & (~GVNI_FOCUSED);
            SetItemState (i, n, nState);
        }
    }
    Invalidate ();
}

/*
** FillCaption :
*/
void CConfigHWGrid::FillGridCaption ()
{
    GV_ITEM Item;
    CString str;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_CONFIG_HW_GRID), 16, 1, RGB(0,128,128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
    if (m_nMode==0)   {
	    Item.row = 0;
	    Item.col = 1;
        Item.iImage = 0;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_NOME);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 2;
        Item.iImage = 1;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_DESCRIZIONE);
        SetItem(&Item);
    }
    else
    if (m_nMode==1)   {
	    Item.row = 0;
	    Item.col = 1;
        Item.iImage = 2;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_BIT0);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 2;
        Item.iImage = 3;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_BIT1);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 3;
        Item.iImage = 4;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_BIT2);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 4;
        Item.iImage = 5;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_BIT3);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 5;
        Item.iImage = 6;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_BIT4);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 6;
        Item.iImage = 7;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_BIT5);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 7;
        Item.iImage = 8;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_BIT6);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 8;
        Item.iImage = 9;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_BIT7);
        SetItem(&Item);
    }
    else
    if (m_nMode==2)   {
	    Item.row = 0;
	    Item.col = 1;
        Item.iImage = 0;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_NOME);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 2;
        Item.iImage = 1;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_DESCRIZIONE);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 3;
        Item.iImage = 13;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_LLINF);
        SetItem(&Item);
	    Item.row = 0;
	    Item.col = 4;
        Item.iImage = 14;
        Item.strText=LOADSTRING (IDS_CONFIG_HW_LLSUP);
        SetItem(&Item);
    }
}

/*
** DisassignHWAnalogRange :
*/
void CConfigHWGrid::DisassignHWAnalogRange (int nRow)
{
    SetItemText (nRow, 3, "");
    SetItemText (nRow, 4, "");
}

/*
** CutSelectedText :
*/
void CConfigHWGrid::CutSelectedText ()
{
}

/*
** AlreadyAssigned :
*/
int CConfigHWGrid::AlreadyAssigned (CString strHWAddress)
{
    CAdamoDatiFile *adf=m_pPrj->GetAdamoDatiFile ();
    stAdamoParametri *pap=adf->GetAdamoParametri ();
    int i, nB=0;
    switch (m_eType)   {
        case enInputDigitale :
            for (i=0; i<NMAXINPDIG; i++)
                if (pap->AdamoInputDig[i].szHWAddress[0]!='\0')
                    if (!strcmp (pap->AdamoInputDig[i].szHWAddress, strHWAddress))   {
                        nB=-1;
                        break;
                    }
            break;    
        case enOutputDigitale :
            for (i=0; i<NMAXOUTDIG; i++)
                if (pap->AdamoOutputDig[i].szHWAddress[0]!='\0')
                    if (!strcmp (pap->AdamoOutputDig[i].szHWAddress, strHWAddress))   {
                        nB=-1;
                        break;
                    }
            break;    
        case enInputAnalogico :
            for (i=0; i<NMAXINPANA; i++)
                if (pap->AdamoInputAnag[i].szHWAddress[0]!='\0')
                    if (!strcmp (pap->AdamoInputAnag[i].szHWAddress, strHWAddress))   {
                        nB=-1;
                        break;
                    }
            break;    
        case enOutputAnalogico :
            for (i=0; i<NMAXOUTANA; i++)
                if (pap->AdamoOutputAnag[i].szHWAddress[0]!='\0')
                    if (!strcmp (pap->AdamoOutputAnag[i].szHWAddress, strHWAddress))   {
                        nB=-1;
                        break;
                    }
            break;    
        case enAsse :
            for (i=0; i<NMAXAXIS; i++)
                if (pap->AdamoAsse[i].szHWAddress[0]!='\0')
                    if (!strcmp (pap->AdamoAsse[i].szHWAddress, strHWAddress))   {
                        nB=-1;
                        break;
                    }
            break;    
        case enGenericCan :
            for (i=0; i<NMAXGENERICCAN; i++)
                if (pap->AdamoCanDevice[i].szHWAddress[0]!='\0')
                    if (!strcmp (pap->AdamoCanDevice[i].szHWAddress, strHWAddress))   {
                        nB=-1;
                        break;
                    }
            break;
    }
    return nB;
}

/*
** OnEndEditCell :
*/
void CConfigHWGrid::OnEndEditCell (int nRow, int nCol, CString str)
{
    double fp;
    char *p;

    CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
    if (pACF)   {
        int nCanNode=((CMainFrame *) AfxGetMainWnd ())->GetNodeFromHWAddress (m_stConfigHW[nRow-1].m_strHWAddr);
        int nCanChannel=((CMainFrame *) AfxGetMainWnd ())->GetChannelFromHWAddress (m_stConfigHW[nRow-1].m_strHWAddr);
        int nCanPin=((CMainFrame *) AfxGetMainWnd ())->GetPinFromHWAddress (m_stConfigHW[nRow-1].m_strHWAddr);
        stHWCanOpenNode &stCON=pACF->GetCanNode (nCanChannel, nCanNode);
        fp=strtod (str, &p);
        if (*p=='\0')   {
            if (nCol==3)   {
                if (m_eType==enInputAnalogico)
                    stCON.afpLLIAInf[nCanPin]=fp;
                else
                if (m_eType==enOutputAnalogico)
                    stCON.afpLLOAInf[nCanPin]=fp;
            }
            else
            if (nCol==4)   {
                if (m_eType==enInputAnalogico)
                    stCON.afpLLIASup[nCanPin]=fp;
                else
                if (m_eType==enOutputAnalogico)
                    stCON.afpLLOASup[nCanPin]=fp;
            }
            m_pPrj->SetHWConfigModified ();
        }
        else
            str="";
    }
    else
        str="";
    CGridCtrl::OnEndEditCell (nRow, nCol, str);
}

/*
** AssignHWAnalogRange :
*/
void CConfigHWGrid::AssignHWAnalogRange (int n)
{
    if (m_nMode==2)   {
        CAdamoHWConfigFile *pACF=m_pPrj->GetHWConfigFile ();
        if (pACF)   {
            double fpLLInf=0.0, fpLLSup=0.0;
            int nCanNode=((CMainFrame *) AfxGetMainWnd ())->GetNodeFromHWAddress (m_stConfigHW[n].m_strHWAddr);
            int nCanChannel=((CMainFrame *) AfxGetMainWnd ())->GetChannelFromHWAddress (m_stConfigHW[n].m_strHWAddr);
            int nCanPin=((CMainFrame *) AfxGetMainWnd ())->GetPinFromHWAddress (m_stConfigHW[n].m_strHWAddr);
            stHWCanOpenNode &stCON=pACF->GetCanNode (nCanChannel, nCanNode);
            if (m_eType==enInputAnalogico)   {
                fpLLInf=stCON.afpLLIAInf[nCanPin];
                fpLLSup=stCON.afpLLIASup[nCanPin];
            }
            else
            if (m_eType==enOutputAnalogico)   {
                fpLLInf=stCON.afpLLOAInf[nCanPin];
                fpLLSup=stCON.afpLLOASup[nCanPin];
            }
            CString str;
            if (!ISEQUAL (fpLLInf, 0.0))   {
                str.Format ("%8.3f", fpLLInf);
                SetItemText (m_stConfigHW[n].m_nRow, 3, str);
            }
            if (!ISEQUAL (fpLLSup, 0.0))   {
                str.Format ("%8.3f", fpLLSup);
                SetItemText (m_stConfigHW[n].m_nRow, 4, str);
            }
        }
    }
}

/*
** OnConnected :
*/
int CConfigHWGrid::OnConnected (int nRow, int nCol, const char *szHWAddress)
{
	return 0;
}

/*
** OnRemoveConnection :
*/
int CConfigHWGrid::OnRemoveConnection (int nRow, int nCol, const char *szHWAddress)
{
	return 0;
}
