// AdamoDGCards.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoTablesSupergrid.h"
#include "workspace.h"
#include "DeviceVariable.h"
#include "mainframe.h"
#include "AdamoTables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTablesSuperGrid

CTablesSuperGrid::CTablesSuperGrid() : m_pMachine (NULL), m_bRinfrescato (false)
{
}

BEGIN_MESSAGE_MAP(CTablesSuperGrid, CXTPReportControl)
	//{{AFX_MSG_MAP(CTablesSuperGrid)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT (XTP_NM_REPORT_VALUECHANGED, OnValueChanged)
	ON_NOTIFY_REFLECT (XTP_NM_REPORT_REQUESTEDIT, OnRequestEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
** FillColumns : configura le colonne del supergridctrl
*/
void CTablesSuperGrid::FillColumns ()
{
	CXTPReportColumn* pColumn;
	int nItemIndex = 0;

	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMOTABLESSUPERGRID_1), 300);
	AddColumn (pColumn);
	pColumn->SetTreeColumn (TRUE);
	pColumn->SetIconID (0);
	pColumn->SetEditable(FALSE);
	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMOTABLESSUPERGRID_2), 300);
	AddColumn (pColumn);
	pColumn->SetIconID (1);
	pColumn->SetEditable(TRUE);
}

/*
** OnCreate :
*/
int CTablesSuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    CBitmap bm;

	if (CXTPReportControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureNone;
	GetImageManager()->SetIcons (IDB_TABLES, 0, 0, CSize (16, 16));
	AllowEdit(TRUE);
	SetSkin (((CIdeApp* )AfxGetApp ())->GetTheme ());
    SetMachine ();
	LoadBitmaps ();
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	return 0;
}

/*
** RinfrescaInformazioni :
*/
void CTablesSuperGrid::RinfrescaInformazioni ()
{
    CAdamoData result;
    int n = 0;
	int nIndexInBuffer = 0;
	int nB;
    bool b=true;

    if (m_pMachine)   {
		CXTPReportRows* pRows = GetRows ();
		if (pRows->GetCount ())   {
			CXTPReportScreenRows *pVisibleRows = pRows->GetScreenRows ();
			int nNumItems=pVisibleRows->GetCount ();
			CDeviceData* pData=new CDeviceData[nNumItems];
			CTablesItemInfo** pInfoTable=new CTablesItemInfo*[nNumItems];
			while (n<nNumItems)   {
				CXTPReportRow *pRow = pVisibleRows->GetAt (n);
				CXTPReportRecord *pRecord = pRow->GetRecord ();
				CTablesItemInfo *pItemInfo = (CTablesItemInfo *) pRecord->GetItem (0)->GetItemData ();
				if (pItemInfo)   {
					CXTPReportRecord *pParentRecord = pRecord->GetParentRecord ();
					if (pParentRecord)   {
						CTablesItemInfo *pParentInfo = (CTablesItemInfo *) pParentRecord->GetItem (0)->GetItemData ();
						CVariableInfo* pVarInfo = pItemInfo->GetVariableInfo ();
						if (pVarInfo->m_nGL)
							nB = m_pMachine->GetLocalTableValues (pVarInfo->m_strName, pParentInfo->GetPath(), pItemInfo->GetKey (), pVarInfo->m_nLevel, pVarInfo->m_nProg, &result);
						else
							nB = m_pMachine->GetGlobalTableValues (pVarInfo->m_strName, pParentInfo->GetPath (), pItemInfo->GetKey (), &result);
						if (nB == S_OK)   {
							pItemInfo->SetData (result);
							if (IsDevice (pItemInfo->GetDataType ()) && result.nType == LUA_TNUMBER)   {
								switch (pItemInfo->GetDataType ())   {
									case OBJ_DIGITAL_INPUT : pData[nIndexInBuffer].m_eDev = enInputDigitale; break;
									case OBJ_DIGITAL_OUPUT : pData[nIndexInBuffer].m_eDev = enOutputDigitale; break;
									case OBJ_ANALOG_INPUT  : pData[nIndexInBuffer].m_eDev = enInputAnalogico; break;
									case OBJ_ANALOG_OUTPUT : pData[nIndexInBuffer].m_eDev = enOutputAnalogico; break;
									case OBJ_AXIS		   : pData[nIndexInBuffer].m_eDev = enAsse; break;
									case OBJ_ENCODER	   : pData[nIndexInBuffer].m_eDev = enEncoder; break;
									case OBJ_TIMER		   : pData[nIndexInBuffer].m_eDev = enTimer; break;
									case OBJ_MEMORY        : pData[nIndexInBuffer].m_eDev = enCounter; break;
								};
								pData[nIndexInBuffer].m_nAddress = (int) result.AdamoData.fp;
								pItemInfo->SetDevice ((CAdamoBase *)((CAdamoCfgMachine* )m_pMachine)->GetItem (pData[nIndexInBuffer].m_eDev, pData[nIndexInBuffer].m_nAddress));
								pInfoTable[nIndexInBuffer] = pItemInfo;
								nIndexInBuffer++;
							}
						}
					}
				}
				n++;
			}
			if (nIndexInBuffer)
				RinfrescaDispositivi (pInfoTable, pData, nIndexInBuffer);
			delete pData;
			delete pInfoTable;
			if (!m_bRinfrescato)   {
				Invalidate ();
				m_bRinfrescato = true;
			}
			RedrawControl ();
		}
	}
}

/*
** FillGrid :
*/
void CTablesSuperGrid::FillGrid () 
{
	CXTPReportRecord* pRecord;
	for (POSITION pos = m_listVarInfo.GetHeadPosition (); pos;)   {
		CVariableInfo* pVarInfo = (CVariableInfo*) m_listVarInfo.GetNext (pos);
		pRecord = AggiungiIlPrimoNodo (pVarInfo);
		if (pRecord)   {
			IteraLeTabelle (pRecord, pVarInfo);
			ConcludiOperazioni (pRecord);
		}
	}
}

/*
** SetMachine :
*/
void CTablesSuperGrid::SetMachine ()
{
    CWorkspace* pWorkSpace=GETWS();
    if (pWorkSpace)   {
        CProject *pActivePrj = pWorkSpace->GetActiveProject();
        if (pActivePrj)
            m_pMachine=pActivePrj->GetMachine ();
    }
}

/*
** InitializeGrid :
*/
void CTablesSuperGrid::InitializeGrid ()
{
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if (pFrame)
	    SetFont(pFrame->GetProgramFont());
    FillColumns ();
    FillGrid ();
}

/*
** OnControlLButtonDown :
*/
//void CTablesSuperGrid::OnControlLButtonDown (UINT nFlags, CPoint point, LVHITTESTINFO& ht)
//{
//	if (ht.iSubItem!=0)   {
//        CTreeItem *t=GetTreeItem (ht.iItem);
//        if (t)   {
//            CTablesItemInfo* pInfo=(CTablesItemInfo*) GetData (t);
//            if (pInfo->GetDataType () != LUA_TBOOLEAN && pInfo->GetDataType () != LUA_TTABLE)   {
//				if (m_pMachine && pInfo->GetDevice() != NULL)   {
//					if (m_ePswLevel<=pInfo->GetDevice()->GetWAcces ())   {
//						switch (pInfo->GetDevice()->GetType ())   {
//							case enInputDigitale :
//								SelettaInputDigitale (nFlags, point, ht, (CAdamoDgtIn*) pInfo->GetDevice());
//								break;
//							case enOutputDigitale :
//								SelettaOutputDigitale (nFlags, point, ht, (CAdamoDgtOut*) pInfo->GetDevice());
//								break;
//							case enInputAnalogico :
//								SelettaInputAnalogico (nFlags, (CAdamoAnalogIn*) pInfo->GetDevice());
//								break;
//							case enOutputAnalogico :
//								SelettaOutputAnalogico (nFlags, (CAdamoAnalogOut*) pInfo->GetDevice());
//								break;
//							case enCounter :
//								SelettaCounter (nFlags, (CAdamoCounter*) pInfo->GetDevice());
//								break;
//							case enTimer :
//								SelettaTimer (nFlags, (CAdamoTimer*) pInfo->GetDevice());
//								break;
//							case enGenericCan :
//								break;
//						}
//					}
//				}
//				else
//					EditLabelEx(ht.iItem, ht.iSubItem);
//            }
//            else
//                if (pInfo->GetData().nType==LUA_TBOOLEAN)   {
//                    SelettaBoolean (point, ht);
//                }
//        }
//    }
//}

/*
** RiempiIlSuperGrid :
*/
void CTablesSuperGrid::RiempiIlSuperGrid (CXTPReportRecord* pRecord, CVariableInfo* pVarInfo, CString strPath, CAdamoValori *pResult, int *pValueType, int nElements)
{
    CXTPReportRecord* pNewRecord;
	CXTPReportRecordItemText* pItem;

    for (int i=0; i<nElements; i++)   {
        CTablesItemInfo* pItemInfo = new CTablesItemInfo ();
        pItemInfo->SetPath (CreaIlPath (strPath, &pResult[i]));
        pItemInfo->SetKey (pResult[i]);
		pItemInfo->SetDataType (pValueType[i]);
		pItemInfo->SetVariableInfo (pVarInfo);
		
		pNewRecord = new CXTPReportRecord();
		pItem = new CXTPReportRecordItemText (GetSGKey (&pResult[i]));
		pItem->SetIconIndex (GetSGImage (pValueType[i]));
		pItem->SetItemData ((DWORD_PTR) pItemInfo);
		pNewRecord->AddItem (pItem);
		pItem = (CXTPReportRecordItemText *) new CXTPViewTableDevices ();
		pNewRecord->AddItem (pItem);
		pRecord->GetChilds ()->Add (pNewRecord);		
        if (pValueType[i]==LUA_TTABLE || pValueType[i]==LUA_TOBJECT)
            IteraLeTabelle (pNewRecord, pVarInfo);
    }
}

/*
** GetSGImage :
*/
int CTablesSuperGrid::GetSGImage (int nType)
{
    int nIndex;

    switch (nType)   {
        case LUA_TNUMBER :			nIndex=4; break;
        case LUA_TSTRING :			nIndex=3; break;
        case LUA_TTABLE  :			nIndex=6; break;
        case LUA_TOBJECT :			nIndex=7; break;
        case LUA_TBOOLEAN:			nIndex=5; break;
        case LUA_TNIL    :			nIndex=2; break;
		case OBJ_DIGITAL_INPUT :	nIndex=8; break;
		case OBJ_DIGITAL_OUPUT :	nIndex=9; break;
		case OBJ_ANALOG_INPUT :		nIndex=10; break;
		case OBJ_ANALOG_OUTPUT :	nIndex=11; break;
		case OBJ_AXIS :				nIndex=12; break;
		case OBJ_ENCODER :			nIndex=13; break;
		case OBJ_TIMER :			nIndex=14; break;
		case OBJ_MEMORY :			nIndex=15; break;
		case OBJ_GENERIC_CAN :		nIndex=16; break;
		default			 :			nIndex=2; break;
    }
    return nIndex;
}

/*
** GetSGKey :
*/
CString CTablesSuperGrid::GetSGKey (CAdamoValori *pResult)
{
    CString str;

    switch (pResult->nType)   {
        case LUA_TNUMBER :
            str.Format ("%d", (int)pResult->AdamoData.fp);
            break;
        case LUA_TSTRING :
            str=pResult->AdamoData.str;
            break;
    }
    return str;
}

/*
** AggiungiIlPrimoNodo :
*/
CXTPReportRecord* CTablesSuperGrid::AggiungiIlPrimoNodo (CVariableInfo* pVarInfo)
{
	CXTPReportRecord* pRecord;
	CXTPReportRecordItemText* pItem;
    CAdamoValori key;
    CAdamoData   data;

	CTablesItemInfo* pItemInfo = new CTablesItemInfo ();
    key.nType=LUA_TNIL;
    data.nType=LUA_TNIL;
    pItemInfo->SetPath ("");
    pItemInfo->SetKey (key);
    pItemInfo->SetData (data);
	pItemInfo->SetVariableInfo (pVarInfo);
	pRecord = new CXTPReportRecord();
	pItem = new CXTPReportRecordItemText (pVarInfo->m_strName);
	pItem->SetIconIndex (6);
	pItem->SetItemData ((DWORD_PTR) pItemInfo);
	pItem->SetEditable(FALSE);
	pRecord->AddItem (pItem);
	pItem = (CXTPReportRecordItemText *) new CXTPViewTableDevices ();
	pItem->SetEditable(FALSE);
	pRecord->AddItem (pItem);
	AddRecord (pRecord);
	return pRecord;
}

/*
** ConcludiOperazioni :
*/
void CTablesSuperGrid::ConcludiOperazioni (CXTPReportRecord* pRecord)
{
	Populate ();
	ExpandAll (TRUE);
	SetMultipleSelection (FALSE);
}

/*
** IteraLeTabelle :
*/
void CTablesSuperGrid::IteraLeTabelle (CXTPReportRecord* pRecord, CVariableInfo* pVarInfo)
{
    CTablesItemInfo* pInfo=(CTablesItemInfo*) pRecord->GetItem (0)->GetItemData ();
    CAdamoValori ad, *pResult;
    CString strPath, strClassName;
    int nElements, *pValueType, nType;
    bool b=true;

	strPath=pInfo->GetPath ();
	nType = m_pMachine->GetObjectClass (pVarInfo->m_strName, strPath, pVarInfo->m_nGL == 0 ? -1 : pVarInfo->m_nLevel, pVarInfo->m_nProg, strClassName);
	if (nType == 0)
		pRecord->GetItem (0)->SetIconIndex (0);
	else
		if (nType == 1)  
			pRecord->GetItem (0)->SetIconIndex (7);
	if (nType >= 0)   {
		ad.nType=LUA_TNIL;
		pResult=new CAdamoValori[1024];
		pValueType=new int [1024];
		if (nType == 1)   {
			GetObjectKeys (strClassName, pResult, pValueType, &nElements);
			if (nElements)
				RiempiIlSuperGrid (pRecord, pVarInfo, strPath, pResult, pValueType, nElements);
		}
		else
			if (nType == 0)   {
				while (b)   {
					if (pVarInfo->m_nGL)
						if (m_pMachine->GetLocalTableKeys (pVarInfo->m_strName, strPath, ad, pVarInfo->m_nLevel, pVarInfo->m_nProg, pResult, pValueType, nElements, b)==S_FALSE)
							break;
						else;
					else
						if (m_pMachine->GetGlobalTableKeys (pVarInfo->m_strName, strPath, ad, pResult, pValueType, nElements, b)==S_FALSE)
							break;
						else;
					if (nElements)   {
						RiempiIlSuperGrid (pRecord, pVarInfo, strPath, pResult, pValueType, nElements);
						ad=pResult[nElements-1];
					}
				}
			}
		delete pValueType;
		delete pResult;
	}
}

/*
** CreaIlPath :
*/
CString CTablesSuperGrid::CreaIlPath (CString strPath, CAdamoValori *pKey)
{
    CString str;

    switch (pKey->nType)   {
        case LUA_TNUMBER :
            if (strPath=="")
                str.Format ("#%d", (int)pKey->AdamoData.fp);
            else
                str.Format ("%s\\#%d", strPath, (int)pKey->AdamoData.fp);
            break;
        case LUA_TSTRING :
            if (strPath=="")
                str.Format ("%s", pKey->AdamoData.str);
            else
                str.Format ("%s\\%s", strPath, pKey->AdamoData.str);
            break;
    }
    return str;
}

/*
** SortDevices :
*/
//int CTablesSuperGrid::SortTables(const void* p1, const void* p2)
//{
//	CTreeItem * pChild1 = *(CTreeItem**)p1;
//	CTreeItem * pChild2 = *((CTreeItem**)p2);
//	CTablesItemInfo *pItem1=(CTablesItemInfo *)(*pChild1).GetNodeInfo ();
//	CTablesItemInfo *pItem2=(CTablesItemInfo *)(*pChild2).GetNodeInfo ();
//    CAdamoValori av1 = pItem1->GetKey ();
//    CAdamoValori av2 = pItem2->GetKey ();
//    
//    if (av1.nType==av2.nType)
//        if (av1.nType==LUA_TNUMBER)
//            return (int)(av1.AdamoData.fp-av2.AdamoData.fp);
//        else
//            return strcmp (av1.AdamoData.str, av2.AdamoData.str);
//    else
//        return av2.nType == LUA_TSTRING ? 1 : -1;
//}

/*
** Sort :
*/
//void CTablesSuperGrid::Sort(CTreeItem* pParent, BOOL bSortChildren, int (__cdecl *pSortFun)(const void *, const void *))
//{
//    CXTPReportControl::Sort (pParent, bSortChildren, pSortFun);
//}
//
/*
** Sort :
*/
BOOL CTablesSuperGrid::OnVkReturn(void)
{
    //int nIndex = GetSelectedItem();
    //if(nIndex!=-1)   {
	   // CTreeItem *pSelItem = GetTreeItem(nIndex);
	   // if (pSelItem != NULL)   {
		  //  BOOL bRedraw=0;
		  //  if (ItemHasChildren(pSelItem))   {
			 //   SetRedraw(0);
			 //   int nScrollIndex=0;
			 //   if (IsCollapsed(pSelItem))   {		
				//    if (OnItemExpanding(pSelItem, nIndex))   {
				//	    nScrollIndex = Expand(pSelItem, nIndex);
				//	    OnItemExpanded(pSelItem, nIndex);
				//	    bRedraw=1;
				//    }
			 //   }	
			 //   else   {
			 //      if (OnCollapsing(pSelItem))   {
				//	    Collapse(pSelItem);
				//	    OnItemCollapsed(pSelItem);
				//	    bRedraw=1;
			 //      }
			 //   }
			 //   SetRedraw(1);
			 //   if (bRedraw)   {
				//    CRect rc;
				//    GetItemRect(nIndex,rc,LVIR_BOUNDS);
				//    InvalidateRect(rc);
				//    UpdateWindow();
				//    EnsureVisible(nScrollIndex,1);
			 //   }
		  //  }
	   // }
    //}
    return 1;
}

/*
** OnUpdateListViewItem :
*/
///void CTablesSuperGrid::OnUpdateListViewItem (CTreeItem* lpItem, LVITEM *plvItem)
//{
 //   CTablesItemInfo* pInfo=(CTablesItemInfo*) GetData (lpItem);
	//CVariableInfo* pVarInfo = pInfo->GetVariableInfo ();
 //   double fp;
 //   bool bNonScrivere=false;
 //   char *p;

 //   if (pInfo)   {
 //       CTreeItem *pt=GetParentItem (lpItem);
 //       if (pt)   {
	//		strdblk (plvItem->pszText);
	//		if (plvItem->pszText[0] != '\0')   {
	//			CTablesItemInfo* pParentInfo=(CTablesItemInfo*) GetData (pt);
	//			CAdamoData ad=pInfo->GetData (), adn;
	//			adn.nType=ad.nType;
	//			switch (ad.nType)   {
	//				case LUA_TNUMBER :
	//					fp=strtod (plvItem->pszText, &p);
	//					if (*p=='\0')
	//						adn.AdamoData.fp=fp;
	//					else
	//						bNonScrivere=true;
	//					break;
	//				case LUA_TSTRING :
	//					if (strlen (strdblk (plvItem->pszText))<=NMAXSTRING)
	//						strcpy (adn.AdamoData.str, plvItem->pszText);
	//					else
	//						bNonScrivere=true;
	//					break;
	//			}
	//			if (!bNonScrivere)   {
	//				CString strPath (pParentInfo->GetPath());
	//				if (pVarInfo->m_nGL)
	//					m_pMachine->SetLocalTableValues (pVarInfo->m_strName, strPath, pInfo->GetKey (), pVarInfo->m_nLevel, pVarInfo->m_nProg, adn);
	//				else
	//					m_pMachine->SetGlobalTableValues (pVarInfo->m_strName, strPath, pInfo->GetKey (), adn);
	//			}
	//		}
 //       }
 //   }
//}

/*
** SelettaBoolean :
*/
void CTablesSuperGrid::SelettaBoolean (CPoint point, LVHITTESTINFO& ht)
{
   // CRect rect;
   // int nOut;

   // GetSubItemRect(ht.iItem, 1, LVIR_BOUNDS, rect);
   // nOut=(point.x<(rect.left+10))||(point.x>rect.left+30);
   // if (!nOut)   {
   //     CTreeItem* t=GetTreeItem(ht.iItem);
   //     if (t)   {
   //         CTablesItemInfo* pInfo=(CTablesItemInfo*) GetData (t);
			//CVariableInfo* pVarInfo = pInfo->GetVariableInfo ();
   //         if (pInfo)   {
   //             CTreeItem *pt=GetParentItem (t);
   //             if (pt)   {
   //                 CTablesItemInfo* pParentInfo=(CTablesItemInfo*) GetData (pt);
   //                 CAdamoData ad=pInfo->GetData (), adn;
   //                 adn.nType=ad.nType;
   //                 adn.AdamoData.b=!ad.AdamoData.b;
   //                 CString strPath (pParentInfo->GetPath());
   //                 if (pVarInfo->m_nGL)
   //                     m_pMachine->SetLocalTableValues (pVarInfo->m_strName, strPath, pInfo->GetKey (), pVarInfo->m_nLevel, pVarInfo->m_nProg, adn);
   //                 else
   //                     m_pMachine->SetGlobalTableValues (pVarInfo->m_strName, strPath, pInfo->GetKey (), adn);
   //             }
   //         }
   //     }
   // }
}

/*
** GetObjectKeys :
*/
void CTablesSuperGrid::GetObjectKeys (CString strClassName, CAdamoValori* pResult, int *pValueType, int* pnElements)
{
	/* prendiamo il container degli oggetti */
	CProject* pPrj = GETWS ()->GetActiveProject ();
	CObjectsContainer& oc = pPrj->GetObjectsContainer ();
	/* cerchiamo l'oggetto letto dal campo */
	CAdamoObj* pObj = oc.GetObject (strClassName);
	CAdamoObjectElement* pObjectElement;
	if (pObj)   {
		/* trovato, riempiamo il vettore delle chiavi */
	    list<CAdamoObjectElement*>::iterator e;
		e = pObj->GetListObjectElements().begin ();
		int n = 0;
		*pnElements = 0;
		while (e!=pObj->GetListObjectElements().end())   {
			pObjectElement = *e;
			if (pObjectElement->KindOf () == eData)   {
				/* riempiamo il vettore delle chiavi */
				pResult[n].nType = LUA_TSTRING;
				strcpy (pResult[n].AdamoData.str, pObjectElement->GetName ());
				pValueType[n] = ((CAdamoObjectData* )pObjectElement)->GetData ().nType;
				n++;
				(*pnElements)++;
			}
			e++;
		}
		pResult[n].nType = LUA_TSTRING;
		strcpy (pResult[n].AdamoData.str, "super");
		pValueType[n] = LUA_TOBJECT;
		n++;
		(*pnElements)++;
	}
}

/*
** RinfrescaDispositivi :
*/
void CTablesSuperGrid::RinfrescaDispositivi (CTablesItemInfo** pInfo, CDeviceData* pData, int nIndexInBuffer)
{
    /* richiediamo il valore */
	int n = 0;
    if (nIndexInBuffer)   {
        if (m_pMachine->GetDevice (nIndexInBuffer, pData) == S_OK)   {
            /* copiamo i dati */
            while (n<nIndexInBuffer)   {
				enDispositivi ed=pData[n].m_eDev;
                switch (ed)   {
                    case enInputDigitale :
					    pData[n].m_nAddress & 1 << 31 ? pInfo[n]->SetStatus (0) : pInfo[n]->ResetStatus (0);
                    case enOutputDigitale :
                        pInfo[n]->GetData().nType = LUA_TBOOLEAN;
                        pInfo[n]->GetData().AdamoData.b = pData[n].m_nDati;
                        break;
                    case enInputAnalogico :
                    case enOutputAnalogico :
                    case enAsse :
                        pInfo[n]->GetData ().nType=LUA_TNUMBER;
                        pInfo[n]->GetData ().AdamoData.fp=pData[n].m_fpDati;
                        break;
                    case enCounter :
                        pInfo[n]->GetData ().nType=pData[n].m_nType;
                        switch (pData[n].m_nType)   {
                            case LUA_TBOOLEAN :
								pInfo[n]->GetData ().nType = LUA_TBOOLEAN;
                                pInfo[n]->GetData ().AdamoData.b=pData[n].m_nDati;
                                break;
                            case LUA_TNUMBER :
								pInfo[n]->GetData ().nType = LUA_TNUMBER;
                                pInfo[n]->GetData ().AdamoData.fp=pData[n].m_fpDati;
                                break;
                            case LUA_TSTRING :
								pInfo[n]->GetData ().nType = LUA_TSTRING;
                                strcpy (pInfo[n]->GetData ().AdamoData.str, pData[n].m_strDati);
                                break;
                        }
                        break;
                    case enTimer :
                        pInfo[n]->GetData ().nType=LUA_TNUMBER;
                        pInfo[n]->GetData ().AdamoData.fp=pData[n].m_fpDati;
                        break;
                }
				n++;
            }
            /* rinfreschiamo il controllo */
            RedrawControl ();
            /* rinfreschiamo ora eventuali dialog aperte */
            ((CMainFrame *)AfxGetMainWnd ())->TimerDialogList ();
        }
    }
}

/*
** IsDevice :
*/
bool CTablesSuperGrid::IsDevice (int nType)
{
	return nType >= OBJ_DIGITAL_INPUT && nType < OBJ_GENERIC_CAN;
}

/*
** DrawData :
*/
void CTablesSuperGrid::DrawData (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, CString str, bool bForceRefresh)
{
	CRect rcLocal (*rc);
	rcLocal.OffsetRect (10, 0);
	if (bForceRefresh)
		::DrawText (hDC, str, -1, &rcLocal, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
	else   {
		CString strDiPrima = CString (pInfo->GetDataDiPrima ()->AdamoData.str);
		if (strDiPrima != str)   {
			::DrawText (hDC, str, -1, &rcLocal, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			strcpy (pInfo->GetDataDiPrima ()->AdamoData.str, str);
		}
	}
}

/*
** DrawBoolean :
*/
void CTablesSuperGrid::DrawBoolean (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, CString str, bool b, bool bForceRefresh)
{
	CRect rcLocal;
	rcLocal.SetRect ((*rc).left+10, (*rc).top, (*rc).left+25, (*rc).bottom);
	if (bForceRefresh)   {
		::DrawFrameControl(hDC, rcLocal, DFC_BUTTON, 
			b ? DFCS_BUTTONCHECK | DFCS_CHECKED : DFCS_BUTTONCHECK);
		rcLocal.SetRect ((*rc).left+30, (*rc).top, (*rc).right, (*rc).bottom);
		::DrawText (hDC, str, -1, &rcLocal, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
	}
	else   {
		CString strDiPrima = CString (pInfo->GetDataDiPrima ()->AdamoData.str);
		if (strDiPrima != str)   {
			::DrawText (hDC, str, -1, &rcLocal, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
			strcpy (pInfo->GetDataDiPrima ()->AdamoData.str, str);
		}
	}
}

/*
** AccendiInputDig :
*/
void CTablesSuperGrid::AccendiInputDig (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh)
{
	CAdamoDgtIn* pDev = (CAdamoDgtIn*) pInfo->GetDevice();
    CBitmap* pBmp, *pBmpForcedInput;
    int nValue=pInfo->GetData ().AdamoData.b;
    int i, nOff;

	if (pDev)   {
		if (bForceRefresh)   {
			for (i=0, nOff=10; i<pDev->GetNumInput (); i++, nOff+=20)   {
				if (nValue&(1<<i))
					pBmp=&m_bmpIO[0];
				else
					pBmp=&m_bmpIO[1];
				((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (hDC, *pBmp, (short)(rc->left+nOff), (short)rc->top, RGB (255, 255, 255));
			}
			if (pInfo->Status (0))   {
				pBmpForcedInput = &m_bmpIO[4];
				((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (hDC, *pBmpForcedInput, (short)(rc->left+nOff), (short)rc->top, RGB (255, 255, 255));
			}
		}
		else   {
			int nValueDiPrima=pInfo->GetDataDiPrima ()->AdamoData.b;
			if (nValueDiPrima!=nValue)   {
				for (i=0, nOff=10; i<pDev->GetNumInput (); i++, nOff+=20)   {
					if (nValue&(1<<i))
						pBmp=&m_bmpIO[0];
					else
						pBmp=&m_bmpIO[1];
					((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (hDC, *pBmp, (short)(rc->left+nOff), (short)rc->top, RGB (255, 255, 255));
				}
				pInfo->GetDataDiPrima ()->AdamoData.b=nValue;
			}
			if (pInfo->IsStatusChanged ())
				if (pInfo->Status (0))   {
					pBmpForcedInput = &m_bmpIO[4];
					((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (hDC, *pBmpForcedInput, (short)(rc->left+nOff), (short)rc->top, RGB (255, 255, 255));
				}
				else   {
					CRect rect;
					rect.left = rc->left+nOff; rect.top = rc->top; 
					rect.right  = rect.left + 16; 
					rect.bottom = rect.top + 16; 
					InvalidateRect (&rect);
			}
		}
	}
}

/*
** AccendiOutputDig :
*/
void CTablesSuperGrid::AccendiOutputDig (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh)
{
	CAdamoDgtOut* pDev = (CAdamoDgtOut*) pInfo->GetDevice();
    CBitmap* pBmp;
    int nValue=pInfo->GetData ().AdamoData.b;
    int i, nOff;
	if (pDev)   {
		if (bForceRefresh)   {
			for (i=0, nOff=10; i<pDev->GetNumOutput (); i++, nOff+=20)   {
				if (nValue&(1<<i))
					pBmp=&m_bmpIO[2];
				else
					pBmp=&m_bmpIO[3];
				((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (hDC, *pBmp, (short)(rc->left+nOff), (short)rc->top, RGB (255, 255, 255));
			}
		}
		else   {
			int nValueDiPrima=pInfo->GetDataDiPrima ()->AdamoData.b;
			if (nValueDiPrima!=nValue)   {
				for (i=0, nOff=10; i<pDev->GetNumOutput (); i++, nOff+=20)   {
					if (nValue&(1<<i))
						pBmp=&m_bmpIO[2];
					else
						pBmp=&m_bmpIO[3];
					((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (hDC, *pBmp, (short)(rc->left+nOff), (short)rc->top, RGB (255, 255, 255));
				}
				pInfo->GetDataDiPrima ()->AdamoData.b=nValue;
			}
	    }
	}
}

/*
** DrawInputAnag :
*/
void CTablesSuperGrid::DrawInputAnag (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh)
{
	CAdamoAnalogIn* pDev = (CAdamoAnalogIn*) pInfo->GetDevice();
    double fp=pInfo->GetData ().AdamoData.fp;
    char szBuffer[256];

	if (pDev)   {
		if (bForceRefresh)   {
			sprintf (szBuffer, "%8.3f", fp);
			::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
		}
		else   {
			double fpDiPrima=pInfo->GetDataDiPrima ()->AdamoData.fp;
			if (!ISEQUAL (fp, fpDiPrima))   {
				ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
				sprintf (szBuffer, "%8.3f", fp);
				::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
				pInfo->GetDataDiPrima ()->AdamoData.fp=fp;
			}
		}
	}
}

/*
** DrawOutputAnag :
*/
void CTablesSuperGrid::DrawOutputAnag (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh)
{
	CAdamoAnalogOut* pDev = (CAdamoAnalogOut*) pInfo->GetDevice();
    double fp=pInfo->GetData ().AdamoData.fp;
    char szBuffer[256];

	if (pDev)   {
		if (bForceRefresh)   {
			sprintf (szBuffer, "%8.3f", fp);
			::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
		}
		else   {
			double fpDiPrima=pInfo->GetDataDiPrima ()->AdamoData.fp;
			if (!ISEQUAL (fp, fpDiPrima))   {
				ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
				sprintf (szBuffer, "%8.3f", fp);
				::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
				pInfo->GetDataDiPrima ()->AdamoData.fp=fp;
			}
		}
	}
}
    
/*
** DrawTimer :
*/
void CTablesSuperGrid::DrawTimer (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh)
{
	CAdamoTimer* pDev = (CAdamoTimer*) pInfo->GetDevice();
    double fp=pInfo->GetData ().AdamoData.fp;
    char szBuffer[256];

	if (pDev)   {
		if (bForceRefresh)   {
			sprintf (szBuffer, "%8.3f", fp);
			::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
		}
		else   {
			double fpDiPrima=pInfo->GetDataDiPrima ()->AdamoData.fp;
			if (!ISEQUAL (fp, fpDiPrima))   {
				ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
				sprintf (szBuffer, "%8.3f", fp);
				::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
				pInfo->GetDataDiPrima ()->AdamoData.fp=fp;
			}
		}
	}
}

/*
** DrawCounter :
*/
void CTablesSuperGrid::DrawCounter (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh)
{
	CAdamoCounter* pDev = (CAdamoCounter*) pInfo->GetDevice();
    char szBuffer[256];

	if (pDev)   {
		if (bForceRefresh)   {
			szBuffer[0]='\0';
			switch (pInfo->GetData ().nType)   {
				case LUA_TBOOLEAN :   {
					CString str=pInfo->GetData ().AdamoData.b ? LOADSTRING (IDS_ADAMOVERO) : LOADSTRING (IDS_ADAMOFALSO);
					DrawBoolean (hDC, pInfo, rc, str, pInfo->GetData ().AdamoData.b != 0);
					}
					break;
				case LUA_TNUMBER :
					sprintf (szBuffer, "%8.3f", pInfo->GetData ().AdamoData.fp);
					break;
				case LUA_TSTRING :
					sprintf (szBuffer, "%s", pInfo->GetData ().AdamoData.str);
					break;
			}
			::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
		}
		else   {
			bool bReDraw=false;
			szBuffer[0]='\0';
			if (pInfo->GetData ().nType == LUA_TBOOLEAN)   {
				if (pInfo->GetDataDiPrima ()->AdamoData.b!=pInfo->GetData ().AdamoData.b)   {
					CString str=pInfo->GetData ().AdamoData.b ? LOADSTRING (IDS_ADAMOVERO) : LOADSTRING (IDS_ADAMOFALSO);
					DrawBoolean (hDC, pInfo, rc, str, pInfo->GetData ().AdamoData.b != 0);
					pInfo->GetDataDiPrima ()->AdamoData.b=pInfo->GetData ().AdamoData.b;
				}
			}
			else   {
				if (pInfo->GetData ().nType == LUA_TNUMBER)   {
					double fpValue=pInfo->GetData ().AdamoData.fp;
					double fpValueDiPrima=pInfo->GetDataDiPrima ()->AdamoData.fp;
					if (!ISEQUAL (fpValue, fpValueDiPrima))   {
						sprintf (szBuffer, "%8.3f", pInfo->GetData ().AdamoData.fp);
						pInfo->GetDataDiPrima ()->AdamoData.fp=fpValue;
						bReDraw=true;
					}
				}
				else   {
					if (pInfo->GetData ().nType == LUA_TSTRING)   {
						if (!strcmp (pInfo->GetData ().AdamoData.str, pInfo->GetDataDiPrima ()->AdamoData.str))
							sprintf (szBuffer, "%s", pInfo->GetData ().AdamoData.str);
							strcpy (pInfo->GetDataDiPrima ()->AdamoData.str, pInfo->GetData ().AdamoData.str);
							bReDraw=true;
					}
				}
			}
			if (bReDraw)
				::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
		}
	}
}

/*
** DrawAxes :
*/
void CTablesSuperGrid::DrawAxes (HDC hDC, CTablesItemInfo* pInfo, CRect* rc, bool bForceRefresh)
{
	CAdamoAxis* pDev = (CAdamoAxis*) pInfo->GetDevice();
    double fp=pInfo->GetData ().AdamoData.fp;
    char szBuffer[256];

	if (pDev)   {
		if (bForceRefresh)   {
			sprintf (szBuffer, "%8.3f", fp);
			::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
		}
		else   {
			double fpDiPrima=pInfo->GetDataDiPrima ()->AdamoData.fp;
			if (!ISEQUAL (fp, fpDiPrima))   {
				ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
				sprintf (szBuffer, "%8.3f", fp);
				::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
				pInfo->GetDataDiPrima ()->AdamoData.fp=fp;
			}
		}
	}
}

/*
** LoadBitmaps :
*/
void CTablesSuperGrid::LoadBitmaps ()
{
    CString strPath=((CMainFrame *)AfxGetMainWnd ())->GetPathImages ();
	m_pngIO[0] = ((CMainFrame *) AfxGetMainWnd ())->LoadPathImages (strPath + "\\" + "Green.png", CXIMAGE_FORMAT_PNG);
	m_pngIO[1] = ((CMainFrame *) AfxGetMainWnd ())->LoadPathImages (strPath + "\\" + "Black.png", CXIMAGE_FORMAT_PNG);
	m_pngIO[2] = ((CMainFrame *) AfxGetMainWnd ())->LoadPathImages (strPath + "\\" + "Orange.png", CXIMAGE_FORMAT_PNG);
	m_pngIO[3] = ((CMainFrame *) AfxGetMainWnd ())->LoadPathImages (strPath + "\\" + "Black.png", CXIMAGE_FORMAT_PNG);
	m_pngIO[4] = ((CMainFrame *) AfxGetMainWnd ())->LoadPathImages (strPath + "\\" + "ForcedInput.png", CXIMAGE_FORMAT_PNG);
	CDC *pDC=GetDC ();
    if (m_pngIO[0])
	    m_bmpIO[0].Attach (m_pngIO[0]->MakeBitmap (pDC->GetSafeHdc()));
    if (m_pngIO[1])
	    m_bmpIO[1].Attach (m_pngIO[1]->MakeBitmap (pDC->GetSafeHdc()));
    if (m_pngIO[2])
	    m_bmpIO[2].Attach (m_pngIO[2]->MakeBitmap (pDC->GetSafeHdc()));
    if (m_pngIO[3])
	    m_bmpIO[3].Attach (m_pngIO[3]->MakeBitmap (pDC->GetSafeHdc()));
    if (m_pngIO[4])
	    m_bmpIO[4].Attach (m_pngIO[4]->MakeBitmap (pDC->GetSafeHdc()));
	ReleaseDC (pDC);
}

/*
** OnDestroy :
*/
void CTablesSuperGrid::OnDestroy()
{
	for (int i = 0; i<5; i++)
		_delete (m_pngIO[i]);
	Clear ();
}

/*
** SelettaInputDigitale :
*/
void CTablesSuperGrid::SelettaInputDigitale (UINT nFlags, CPoint point, int nColumnX, CAdamoDgtIn* pInp)
{
    CDeviceData Data;
    CRect rect;
    int nIn;
	
    nIn=(point.x - nColumnX) / 20;
    if (nIn<pInp->GetNumInput ())   {
        if ((nFlags&MK_CONTROL)||(nFlags&MK_SHIFT))   {
            Data.m_eDev=enInputDigitale;
            Data.m_nAddress=pInp->GetDeviceID ();
            Data.m_nDati=nIn;
            Data.m_nType=(nFlags&MK_CONTROL) ? TYPERESETBIT : TYPESETBIT;
            m_pMachine->SetDevice (1, &Data);
        }
    }
}

/*
** SelettaOutputDigitale :
*/
void CTablesSuperGrid::SelettaOutputDigitale (UINT nFlags, CPoint point, int nColumnX, CAdamoDgtOut* pOut)
{
    CDeviceData Data;
    CRect rect;
    int nOut;

    nOut = (point.x - nColumnX) / 20;
    if (nOut<pOut->GetNumOutput ())   {
        if ((nFlags&MK_CONTROL)||(nFlags&MK_SHIFT))   {
            Data.m_eDev=enOutputDigitale;
            Data.m_nAddress=pOut->GetDeviceID ();
            Data.m_nType=(nFlags&MK_CONTROL) ? TYPERESETBIT : TYPESETBIT;
            Data.m_nDati=nOut;
            m_pMachine->SetDevice (1, &Data);
        }
    }
}

/*
** SelettaInputAnalogico :
*/
void CTablesSuperGrid::SelettaInputAnalogico (UINT nFlags, CAdamoAnalogIn* pInAna)
{
    if (nFlags&MK_CONTROL)   {
    CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd ();
		if (!pMainFrame->ExistDialogList (pInAna->GetPathName ()))   {
			CDeviceVariable *dv = new CDeviceVariable;
			dv->SetMachine (m_pMachine);
			dv->SetData (LOADSTRING (IDS_ADAMODEVICE_3), pInAna->GetPathName(), pInAna->GetName (), LUA_TNUMBER);
			dv->SetDevice (pInAna);
			dv->RunAlways ();
			dv->Create (IDD_GLOBALVARIABLE, this);
			dv->ShowWindow (SW_SHOWNORMAL);
			dv->CenterWindow ();
			pMainFrame->AddDialogList (dv);
		}
	}
}

/*
** SelettaOutputAnalogico :
*/
void CTablesSuperGrid::SelettaOutputAnalogico (UINT nFlags, CAdamoAnalogOut* pOutAna)
{
    if (nFlags&MK_CONTROL)   {
    CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd ();
		if (!pMainFrame->ExistDialogList (pOutAna->GetPathName ()))   {
			CDeviceVariable *dv = new CDeviceVariable;
			dv->SetMachine (m_pMachine);
			dv->SetData (LOADSTRING (IDS_ADAMODEVICE_4), pOutAna->GetPathName(), pOutAna->GetName (), LUA_TNUMBER);
			dv->SetDevice (pOutAna);
			dv->RunAlways ();
			dv->Create (IDD_GLOBALVARIABLE, this);
			dv->ShowWindow (SW_SHOWNORMAL);
			dv->CenterWindow ();
			pMainFrame->AddDialogList (dv);
		}
	}
}

/*
** SelettaTimer :
*/
void CTablesSuperGrid::SelettaTimer (UINT nFlags, CAdamoTimer* pTimer)
{
    CDeviceVariable dlg;

    if (nFlags&MK_CONTROL)   {
		dlg.SetMachine (m_pMachine);
		dlg.SetData (LOADSTRING (IDS_ADAMODEVICE_6), pTimer->GetPathName(), pTimer->GetName (), LUA_TNUMBER);
		dlg.SetDevice (pTimer);
		dlg.RunAlways ();
		dlg.DoModal ();
	}
}

/*
** SelettaCounter :
*/
void CTablesSuperGrid::SelettaCounter (UINT nFlags, CAdamoCounter* pCounter)
{
    CDeviceVariable dlg;

    if (nFlags&MK_CONTROL)   {
		dlg.SetMachine (m_pMachine);
		dlg.SetData (LOADSTRING (IDS_ADAMODEVICE_7), pCounter->GetPathName(), pCounter->GetName (), pCounter->GetCounterType());
		dlg.SetDevice (pCounter);
		dlg.RunAlways ();
		dlg.DoModal ();
	}
}

/*
** AddVariable :
*/
void CTablesSuperGrid::AddVariable (CString strVariableName, int nGL, int nLevel, int nProg)
{
	CVariableInfo* pVarInfo = new CVariableInfo;
	pVarInfo->m_strName = strVariableName;
	pVarInfo->m_nGL = nGL;
	pVarInfo->m_nLevel = nLevel;
	pVarInfo->m_nProg = nProg;
	m_listVarInfo.AddTail (pVarInfo);
}

/*
** Clear :
*/
void CTablesSuperGrid::Clear ()
{
	for (POSITION pos = m_listVarInfo.GetHeadPosition (); pos;)   {
		CVariableInfo* pVarInfo = (CVariableInfo *) m_listVarInfo.GetNext (pos);
		delete pVarInfo;
	}
	m_listVarInfo.RemoveAll ();
	ResetContent (FALSE);
}

/*
** SetSkin :
*/
LRESULT CTablesSuperGrid::SetSkin (AppTheme nSkin)
{
	switch (nSkin)
	{
		case themeVS2010: SetTheme(xtpReportThemeVisualStudio2012Blue); break;
		case themeVS2012Light: SetTheme(xtpReportThemeVisualStudio2012Light); break;
		case themeVS2012Dark: SetTheme(xtpReportThemeVisualStudio2012Dark); break;
		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue: SetTheme(xtpReportThemeVisualStudio2015); break;
	}	
	GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureNone;
	return 0;
}

void CTablesSuperGrid::OnValueChanged(NMHDR* pNotifyStruct, LRESULT*)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
    CTablesItemInfo* pItemInfo=(CTablesItemInfo*) pItemNotify->pRow->GetRecord ()->GetItem (0)->GetItemData ();
	CVariableInfo* pVarInfo = pItemInfo->GetVariableInfo ();
    double fp;
    bool bNonScrivere=false;
    char *p;

    if (pItemInfo)   {
		CXTPReportRecord *pRecord = pItemNotify->pRow->GetRecord ();
		CXTPReportRecord *pParentRecord = pItemNotify->pRow->GetRecord ()->GetParentRecord ();
		if (pParentRecord)   {
			CTablesItemInfo* pParentItemInfo = (CTablesItemInfo*) pParentRecord->GetItem (0)->GetItemData ();
			if (pParentItemInfo)   {
				CString str = ((CXTPReportRecordItemText *)pRecord->GetItem (1))->GetValue ();
				str.Trim ();
				if (str != "")   {
					CAdamoData ad = pItemInfo->GetData (), adn;
					adn.nType=ad.nType;
					switch (ad.nType)   {
						case LUA_TNUMBER :

							fp=strtod (str, &p);
							if (*p=='\0')
								adn.AdamoData.fp=fp;
							else
								bNonScrivere=true;
							break;
						case LUA_TSTRING :
							if (str.GetLength () <= NMAXSTRING)
								strcpy (adn.AdamoData.str, str);
							else
								bNonScrivere=true;
							break;
					}
					if (!bNonScrivere)   {
						CString strPath (pParentItemInfo->GetPath());
						if (pVarInfo->m_nGL)
							m_pMachine->SetLocalTableValues (pVarInfo->m_strName, strPath, pItemInfo->GetKey (), pVarInfo->m_nLevel, pVarInfo->m_nProg, adn);
						else
							m_pMachine->SetGlobalTableValues (pVarInfo->m_strName, strPath, pItemInfo->GetKey (), adn);
					}
				}
			}
		}
    }
}

void CTablesSuperGrid::OnRequestEdit (NMHDR* pNotifyStruct, LRESULT* result)
{
	XTP_NM_REPORTREQUESTEDIT* pItemNotify = (XTP_NM_REPORTREQUESTEDIT*)pNotifyStruct;
    CTablesItemInfo* pItemInfo=(CTablesItemInfo*) pItemNotify->pRow->GetRecord ()->GetItem (0)->GetItemData ();	
	pItemNotify->bCancel = (pItemInfo->GetDevice () != NULL);
}

/*
** OnDrawCaption .
*/
void CXTPViewTableDevices::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CAdamoData ad;
	CString str;
	CXTPReportRecord* pRecord = pDrawArgs->pRow->GetRecord ();
	CTablesItemInfo* pItemInfo = (CTablesItemInfo *) pRecord->GetItem (0)->GetItemData ();
    CAdamoBase* pDev = pItemInfo->GetDevice ();
	int nType = pItemInfo->GetDataType ();
	ad = pItemInfo->GetData ();
	switch (nType)   {
		case LUA_TNUMBER :
			if (ad.nType == LUA_TNUMBER)   {
				str.Format ("%8.3f", ad.AdamoData.fp);
				((CTablesSuperGrid *)(pDrawArgs->pControl))->DrawData (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem, str);
			}
			break;
		case LUA_TSTRING :
			if (ad.nType == LUA_TSTRING)   {
				str=ad.AdamoData.str;
				((CTablesSuperGrid *)(pDrawArgs->pControl))->DrawData (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem, str);
			}
			break;
		case LUA_TBOOLEAN :
			if (ad.nType == LUA_TBOOLEAN)   {
				str=ad.AdamoData.b ? LOADSTRING (IDS_ADAMOVERO) : LOADSTRING (IDS_ADAMOFALSO);
				((CTablesSuperGrid *)(pDrawArgs->pControl))->DrawBoolean (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem, str, ad.AdamoData.b != 0);
			}
			break;
		case OBJ_DIGITAL_INPUT :
			if (ad.nType == LUA_TBOOLEAN)
				((CTablesSuperGrid *)(pDrawArgs->pControl))->AccendiInputDig (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem);
			break;
		case OBJ_DIGITAL_OUPUT :
			if (ad.nType == LUA_TBOOLEAN)
				((CTablesSuperGrid *)(pDrawArgs->pControl))->AccendiOutputDig (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem);
			break;
		case OBJ_ANALOG_INPUT :
			if (ad.nType == LUA_TNUMBER)
				((CTablesSuperGrid *)(pDrawArgs->pControl))->DrawInputAnag (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem);
			break;
		case OBJ_ANALOG_OUTPUT :
			if (ad.nType == LUA_TNUMBER)
				((CTablesSuperGrid *)(pDrawArgs->pControl))->DrawOutputAnag (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem);
			break;
		case OBJ_TIMER :
			if (ad.nType == LUA_TNUMBER)
				((CTablesSuperGrid *)(pDrawArgs->pControl))->DrawTimer (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem);
			break;
		case OBJ_MEMORY :
			if (ad.nType == LUA_TBOOLEAN || ad.nType == LUA_TNUMBER || ad.nType == LUA_TSTRING)
				((CTablesSuperGrid *)(pDrawArgs->pControl))->DrawCounter (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem);
			break;
		case OBJ_AXIS :
			if (ad.nType == LUA_TNUMBER)
				((CTablesSuperGrid *)(pDrawArgs->pControl))->DrawAxes (pDrawArgs->pDC->GetSafeHdc (), pItemInfo, &pDrawArgs->rcItem);
			break;
	}
}
