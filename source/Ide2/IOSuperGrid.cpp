// IOSuperGrid.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include <afxtoolbarimages.h>
#include "ide2.h"
#include "AdamoGlobal.h"
#include "IOSuperGrid.h"
#include "DeviceVariable.h"
#include "MainFrame.h"

extern UINT m_nRefreshMetrics;

/////////////////////////////////////////////////////////////////////////////
// CIOSuperGrid
CIOSuperGrid::CIOSuperGrid() : m_nItemHeight (0), m_pMachine (NULL), m_pDragImage (NULL), m_bDragging (false), m_nMode (0), 
	m_bInitialized (false), m_nInput (-1), m_bForced (false), m_pRecord (NULL), m_Dragging (false), m_ButtonDown (false)
{
	m_pngIO[0] = m_pngIO[1] = m_pngIO[2] = m_pngIO[3] = m_pngIO[4] = NULL;
}

CIOSuperGrid::~CIOSuperGrid()
{
}

BEGIN_MESSAGE_MAP(CIOSuperGrid, CXTPReportControl)
	//{{AFX_MSG_MAP(CIOSuperGrid)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnReportLButtonDown)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnReportRButtonDown)
	ON_REGISTERED_MESSAGE(m_nRefreshMetrics, OnRefreshMetrics)
	ON_WM_DESTROY()
    ON_COMMAND(ID_FORCESINGLEINPUT, OnForceSingleInput)
    ON_COMMAND(ID_UNFORCESINGLEINPUT, OnUnForceSingleInput)
    ON_COMMAND(ID_FORCEALLINPUT_ON, OnForceAllInputsOn)
    ON_COMMAND(ID_FORCEALLINPUT_OFF, OnForceAllInputsOff)
	ON_WM_LBUTTONDOWN ()
	ON_WM_LBUTTONUP ()
	ON_WM_MOUSEMOVE ()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIOSuperGrid message handlers
int CIOSuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTPReportControl::OnCreate(lpCreateStruct) == -1)
		return -1;
    if (!m_image.Create (16, 16, ILC_COLOR24|ILC_MASK, 1, 1))
		return -1;
    m_ePswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
	GetImageManager()->RemoveAll();
	VERIFY(GetImageManager()->SetIcons (IDB_CONF_TREE, 0, 0, CSize (16, 16)));
    m_cxImage=m_cyImage=16;
    CWorkspace* pWP=GETWS();
    if (pWP)
        m_pMachine=GETWS()->GetActiveProject ()->GetMachine ();
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
	return 0;
}

/*
** OnDestroy :
*/
void CIOSuperGrid::OnDestroy()
{
	__super::OnDestroy ();
	CXTPReportRecords* pRecords = GetRecords ();
	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecord = pRecords->GetAt (i);
		if (pRecord->HasChildren ())
			CleanData (pRecord);
	}
	_delete (m_pngIO[0]);
	_delete (m_pngIO[1]);
	_delete (m_pngIO[2]);
	_delete (m_pngIO[3]);
	_delete (m_pngIO[4]);
}

/*
** CleanData :
*/
void CIOSuperGrid::CleanData (CXTPReportRecord* pRecord)
{
	for (int i = 0; i<pRecord->GetChilds ()->GetCount (); i++)   {
		CXTPReportRecords* pChilds = pRecord->GetChilds ();
		CXTPReportRecord* pChild = pChilds->GetAt (i);
		if (pChild->HasChildren ())
			CleanData (pChild);
		CIOItemInfo* pItemInfo = (CIOItemInfo *) pChild->GetItem (0)->GetItemData ();
		if (pItemInfo)
			delete pItemInfo;
	}
}

/*
** InitializeGrid :
*/
void CIOSuperGrid::InitializeGrid()
{
	/* settiamo il font */
	if (!m_bInitialized)   {
		CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
		if (pFrame)
			SetFont(pFrame->GetProgramFont());
		FillColumns ();
		m_bInitialized = true;
	}
    FillGrid ();
    SetFocus ();
	Populate ();
	/* esplodiamo tutti i rami */
	ExpandAll (TRUE);
	SetMultipleSelection (FALSE);
	SetSkin (((CIdeApp* )AfxGetApp ())->GetTheme ());
}

/*
** OnPreviewKeyDown :
*/
BOOL CIOSuperGrid::OnPreviewKeyDown(UINT& rnChar, UINT nRepCnt, UINT nFlags)
{
	if (rnChar == VK_RETURN)   {
		CXTPReportSelectedRows* pSelectedRows = GetSelectedRows ();
		if (pSelectedRows->GetCount () > 0)   {
			CXTPReportRow *pRow = pSelectedRows->GetAt (0);
			if (pRow)   {
				CXTPReportRecord *pRecord = pRow->GetRecord ();
				if (!pRecord->HasChildren ())   {
					CIOItemInfo* pInfo=(CIOItemInfo*)pRecord->GetItem (0)->GetItemData ();
					switch (pInfo->GetDevice()->GetType ())   {
						case enInputAnalogico :
							if (m_ePswLevel<=pInfo->GetDevice()->GetWAcces ())
								SelettaInputAnalogico (MK_CONTROL, (CAdamoAnalogIn*) pInfo->GetDevice());
							break;
						case enOutputAnalogico :
							if (m_ePswLevel<=pInfo->GetDevice()->GetWAcces ())
								SelettaOutputAnalogico (MK_CONTROL, (CAdamoAnalogOut*) pInfo->GetDevice());
							break;
						case enCounter :
							if (m_ePswLevel<=pInfo->GetDevice()->GetWAcces ())
								SelettaCounter (MK_CONTROL, (CAdamoCounter*) pInfo->GetDevice());
							break;
						case enTimer :
							if (m_ePswLevel<=pInfo->GetDevice()->GetWAcces ())
								SelettaTimer (MK_CONTROL, (CAdamoTimer*) pInfo->GetDevice());
							break;
						case enGenericCan :
							break;
					}
				}
			}
		}
	}
    return 1;
}

/*
** FillGrid :
*/
void CIOSuperGrid::FillGrid ()
{   
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)   {
		AddItem (pPrj->GetMachineModule (), NULL);
		CallIterator (pPrj);
		TogliIGruppiSenzaFigli ();
    }
}

/*
** Iterator :
*/
void CIOSuperGrid::Iterator(CAdamoGroup *pGrp, CXTPReportRecord* pParent, CAdamoDatiFile *pFile, bool bIsPhysical)
{
    int nCountDev = 0, nCountGrp = 0;

    pGrp->Init();
    pGrp->DevInit();
    while (pGrp->More())   {
        CAdamoGroup *obj = (CAdamoGroup*)pGrp->GetNext();
        CXTPReportRecord* pNewParent = AddItem (obj, pParent);
        Iterator (obj, pNewParent, pFile, bIsPhysical);
		nCountGrp++;
    }
    while (pGrp->DevMore())   {
        CAdamoBase *pDev = (CAdamoBase*)pGrp->DevGetNext();
        if (pDev)   {
			char szBuffer[MAXHWLENGTH];
			bool bIsDeviceConnected=!bIsPhysical || (bIsPhysical && (pDev->GetType () == enTimer || pDev->GetType () == enCounter )) || (bIsPhysical && !pFile->GetDeviceHW (pDev->GetDeviceID (), pDev->GetType (), szBuffer));
            //if (m_ePswLevel<=pDev->GetRAcces () && bIsDeviceConnected)   {
                AddItem (pDev, pParent);
                nCountDev++;
            //}
        }
    }
}

/*
** AddItem :
*/
CXTPReportRecord* CIOSuperGrid::AddItem (CAdamoBase* pDev, CXTPReportRecord* pParentRecord)
{
	CXTPReportRecord* pRecord = NULL;
	CXTPReportRecordItemText* pItem;
	CString strName, strDesc;

    strName = pDev->GetName();
    strDesc = pDev->GetDesc();
	CIOItemInfo* pItemInfo = new CIOItemInfo;
	pItemInfo->SetDevice (pDev);
	pRecord = new CXTPReportRecord();
	pItem = new CXTPReportRecordItemText (strName);
	pItem->SetItemData ((DWORD_PTR) pItemInfo);
	pItem->SetIconIndex (SelectImageIndex (pDev));
	pRecord->AddItem (pItem);
	if (m_nMode == 0)   {
		pItem = (CXTPReportRecordItemText *) new CXTPReportRecordItemViewDevices ();
		pRecord->AddItem (pItem);
	}
	if (m_nMode != 2)   {
		pItem = new CXTPReportRecordItemText (pDev->GetDesc ());
		pRecord->AddItem (pItem);
	}
	if (!pParentRecord)
		AddRecord (pRecord);
	else   {
		int nIndex;
		GetIndex (pParentRecord, pDev, nIndex);
		pParentRecord->GetChilds()->InsertAt (nIndex, pRecord);
	}
	return pRecord;
}

/*
** FillColumns :
*/
void CIOSuperGrid::FillColumns ()
{
	CXTPReportColumn* pColumn;
	int nItemIndex = 0;

	pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMOIOSUPERGRID_1), 300);
	AddColumn (pColumn);
	pColumn->SetTreeColumn (TRUE);
	pColumn->SetIconID (1);
	if (m_nMode == 0 || m_nMode == 2)   {
		pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMOIOSUPERGRID_3), 300);
		AddColumn (pColumn);
		pColumn->SetIconID (28);
	}
    if (m_nMode == 0 || m_nMode == 1)   {
		pColumn = new CXTPReportColumn(nItemIndex++, LOADSTRING (IDS_ADAMOIOSUPERGRID_2), 300);
		AddColumn (pColumn);
		pColumn->SetIconID (27);
	}
}

/*
** SelectImageIndex :
*/
int CIOSuperGrid::SelectImageIndex (CAdamoBase* pDev)
{
    int nB;

	if (pDev->IsKindOf (RUNTIME_CLASS(CAdamoLogicModule)))
		nB = 1;
	else
		switch (pDev->GetType ())   {
			case enBase :
				nB=2;
				break;
			case enInputDigitale :
				nB=6;
				break;
			case enOutputDigitale :
				nB=7;
				break;
			case enInputAnalogico :
				nB=4;
				break;
			case enOutputAnalogico :
				nB=5;
				break;
			case enCounter :
				nB=8;
				break;
			case enTimer :
				nB=9;
				break;
			case enAsse :
				nB=3;
				break;
			case enGenericCan :
				nB=1;
				break;
		}
    return nB;
}

/*
** OnReportLButtonDown :
*/
void CIOSuperGrid::OnReportLButtonDown (NMHDR* pNotifyStruct, LRESULT* pResult)
{
	XTP_NM_REPORTRECORDITEM* pItemNotify = reinterpret_cast<XTP_NM_REPORTRECORDITEM*>(pNotifyStruct);
    if (m_pMachine && (m_nMode == 0 || m_nMode == 2))   {
        CIOItemInfo* pInfo=(CIOItemInfo *) pItemNotify->pItem->GetRecord ()->GetItem (0)->GetItemData ();
        if (m_ePswLevel<=pInfo->GetDevice()->GetWAcces ())   {
			CRect rcCol = GetColumns ()->GetAt (1)->GetRect ();
			int nKeyState = GetKeyState (VK_CONTROL);
			int nShiftState = GetKeyState (VK_SHIFT);
			int nFlags = 0;
			if (nKeyState & 0x8000)
				nFlags |= MK_CONTROL;
			if (nShiftState & 0x8000)
				nFlags |= MK_SHIFT;
            switch (pInfo->GetDevice()->GetType ())   {
                case enInputDigitale :
					pItemNotify->pt.x -= rcCol.left;
                    SelettaInputDigitale (nFlags, pItemNotify->pt, 0, (CAdamoDgtIn*) pInfo->GetDevice());
                    break;
                case enOutputDigitale :
					pItemNotify->pt.x -= rcCol.left;
					SelettaOutputDigitale (nFlags, pItemNotify->pt, 0, (CAdamoDgtOut*) pInfo->GetDevice());
                    break;
                case enInputAnalogico :
                    SelettaInputAnalogico (nFlags, (CAdamoAnalogIn*) pInfo->GetDevice());
                    break;
                case enOutputAnalogico :
                    SelettaOutputAnalogico (nFlags, (CAdamoAnalogOut*) pInfo->GetDevice());
                    break;
                case enCounter :
                    SelettaCounter (nFlags, (CAdamoCounter*) pInfo->GetDevice());
                    break;
                case enTimer :
                    SelettaTimer (nFlags, (CAdamoTimer*) pInfo->GetDevice());
                    break;
                case enGenericCan :
                    break;
            }
        }
    }                               
}

/* 
** RinfrescaDispositivi :
*/
void CIOSuperGrid::RinfrescaDispositivi ()
{
	CXTPReportRows* pRows = GetRows ();
    if (m_pMachine && pRows->GetCount ())   {
		CXTPReportScreenRows *pVisibleRows = pRows->GetScreenRows ();
        int nNumItems=pVisibleRows->GetCount ();
        /* allochiamo il buffer */
        CDeviceData* pData=new CDeviceData[nNumItems];
        /* componiamo il buffer con i dispositivi non stringa da chiedere */
        int n=0, nIndexInBuffer=0;
        while (n<nNumItems)   {
			CXTPReportRow *pRow = pVisibleRows->GetAt (n);
            CIOItemInfo* pInfo=(CIOItemInfo*) pRow->GetRecord ()->GetItem (0)->GetItemData ();
            enDispositivi ed=pInfo->GetDevice ()->GetType ();
            if (ed!=enGenericCan&&ed!=enBase)   {
                pData[nIndexInBuffer].m_eDev=ed;
                pData[nIndexInBuffer].m_nAddress=pInfo->GetDevice ()->GetDeviceID ();
                nIndexInBuffer++;
            }
            n++;
        }
        /* richiediamo il valore */
        if (nIndexInBuffer)   {
            if (m_pMachine->GetDevice (nIndexInBuffer, pData) == S_OK)   {
                /* copiamo i dati */
                n=0, nIndexInBuffer=0;
                while (n<nNumItems)   {
					CXTPReportRow *pRow = pVisibleRows->GetAt (n);
                    CIOItemInfo* pInfo=(CIOItemInfo*) pRow->GetRecord ()->GetItem (0)->GetItemData ();
                    enDispositivi ed=pInfo->GetDevice ()->GetType ();
                    if (ed!=enGenericCan&&ed!=enBase)   {
                        switch (ed)   {
                            case enInputDigitale :
							    pData[nIndexInBuffer].m_nAddress & 1 << 31 ? pInfo->SetStatus (0) : pInfo->ResetStatus (0);
                            case enOutputDigitale :
                                pInfo->GetDeviceData ()->nType=0;
                                pInfo->GetDeviceData ()->AdamoData.b=pData[nIndexInBuffer].m_nDati;
                                break;
                            case enInputAnalogico :
                            case enOutputAnalogico :
                            case enAsse :
                                pInfo->GetDeviceData ()->nType=0;
                                pInfo->GetDeviceData ()->AdamoData.fp=pData[nIndexInBuffer].m_fpDati;
                                break;
                            case enCounter :
                                pInfo->GetDeviceData ()->nType=pData[nIndexInBuffer].m_nType;
                                switch (pData[nIndexInBuffer].m_nType)   {
                                    case LUA_TBOOLEAN :
                                        pInfo->GetDeviceData ()->AdamoData.b=pData[nIndexInBuffer].m_nDati;
                                        break;
                                    case LUA_TNUMBER :
                                        pInfo->GetDeviceData ()->AdamoData.fp=pData[nIndexInBuffer].m_fpDati;
                                        break;
                                    case LUA_TSTRING :
                                        strcpy (pInfo->GetDeviceData ()->AdamoData.str, pData[nIndexInBuffer].m_strDati);
                                        break;
                                }
                                break;
                            case enTimer :
                                pInfo->GetDeviceData ()->nType=0;
                                pInfo->GetDeviceData ()->AdamoData.fp=pData[nIndexInBuffer].m_fpDati;
                                break;
                        }
                        nIndexInBuffer++;
                    }
                    n++;
                }
                /* rinfreschiamo il controllo */
                RedrawControl ();
                /* rinfreschiamo ora eventuali dialog aperte */
                ((CMainFrame *)AfxGetMainWnd ())->TimerDialogList ();
            }
        }
        /* liberiamo il buffer */
        delete pData;
    }
}

/*
** AccendiInputDig :
*/
void CIOSuperGrid::AccendiInputDig (HDC hDC, 
                                    CAdamoDgtIn* pDev, 
                                    CIOItemInfo* pInfo,
                                    CRect* rc,
                                    bool bForceRefresh)
{
    CBitmap* pBmp, *pBmpForcedInput;
    int nValue=pInfo->GetDeviceData ()->AdamoData.b;
    int i, nOff;
    if (bForceRefresh)   {
        for (i=0, nOff=0; i<pDev->GetNumInput (); i++, nOff+=20)   {
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
        int nValueDiPrima=pInfo->GetDeviceDataDiPrima ()->AdamoData.b;
        if (nValueDiPrima!=nValue)   {
            for (i=0, nOff=0; i<pDev->GetNumInput (); i++, nOff+=20)   {
                if (nValue&(1<<i))
                    pBmp=&m_bmpIO[0];
                else
                    pBmp=&m_bmpIO[1];
                ((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (hDC, *pBmp, (short)(rc->left+nOff), (short)rc->top, RGB (255, 255, 255));
            }
            pInfo->GetDeviceDataDiPrima ()->AdamoData.b=nValue;
        }
		if (pInfo->IsStatusChanged ())   {
			nOff = 20 * pDev->GetNumInput ();
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
void CIOSuperGrid::AccendiOutputDig (HDC hDC,
                                    CAdamoDgtOut* pDev, 
                                    CIOItemInfo* pInfo,
                                    CRect* rc,
                                    bool bForceRefresh)
{
    CBitmap* pBmp;
    int nValue=pInfo->GetDeviceData ()->AdamoData.b;
    int i, nOff;
    if (bForceRefresh)   {
        for (i=0, nOff=0; i<pDev->GetNumOutput (); i++, nOff+=20)   {
            if (nValue&(1<<i))
                pBmp=&m_bmpIO[2];
            else
                pBmp=&m_bmpIO[3];
            ((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (hDC, *pBmp, (short)(rc->left+nOff), (short)rc->top, RGB (255, 255, 255));
        }
    }
    else   {
        int nValueDiPrima=pInfo->GetDeviceDataDiPrima ()->AdamoData.b;
        if (nValueDiPrima!=nValue)   {
            for (i=0, nOff=0; i<pDev->GetNumOutput (); i++, nOff+=20)   {
                if (nValue&(1<<i))
                    pBmp=&m_bmpIO[2];
                else
                    pBmp=&m_bmpIO[3];
                ((CMainFrame*)AfxGetMainWnd ())->DrawTransparentBitmap (hDC, *pBmp, (short)(rc->left+nOff), (short)rc->top, RGB (255, 255, 255));
            }
            pInfo->GetDeviceDataDiPrima ()->AdamoData.b=nValue;
        }
    }
}

/*
** DrawInputAnag :
*/
void CIOSuperGrid::DrawInputAnag (HDC hDC, 
                                    CAdamoAnalogIn* pDev, 
                                    CIOItemInfo* pInfo,
                                    CRect* rc,
                                    bool bForceRefresh)
{
    double fp=pInfo->GetDeviceData ()->AdamoData.fp;
    char szBuffer[256];

    if (bForceRefresh)   {
        sprintf (szBuffer, "%8.3f", fp);
        ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
    }
    else   {
        double fpDiPrima=pInfo->GetDeviceDataDiPrima ()->AdamoData.fp;
        if (!ISEQUAL (fp, fpDiPrima))   {
            ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
            sprintf (szBuffer, "%8.3f", fp);
            ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
            pInfo->GetDeviceDataDiPrima ()->AdamoData.fp=fp;
        }
    }
}

/*
** DrawOutputAnag :
*/
void CIOSuperGrid::DrawOutputAnag (HDC hDC,
                                    CAdamoAnalogOut* pDev, 
                                    CIOItemInfo* pInfo,
                                    CRect* rc,
                                    bool bForceRefresh)
{
    double fp=pInfo->GetDeviceData ()->AdamoData.fp;
    char szBuffer[256];

    if (bForceRefresh)   {
        sprintf (szBuffer, "%8.3f", fp);
        ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
    }
    else   {
        double fpDiPrima=pInfo->GetDeviceDataDiPrima ()->AdamoData.fp;
        if (!ISEQUAL (fp, fpDiPrima))   {
            ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
            sprintf (szBuffer, "%8.3f", fp);
            ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
            pInfo->GetDeviceDataDiPrima ()->AdamoData.fp=fp;
        }
    }
}
    
/*
** DrawTimer :
*/
void CIOSuperGrid::DrawTimer (HDC hDC,
                                    CAdamoTimer* pDev, 
                                    CIOItemInfo* pInfo,
                                    CRect* rc,
                                    bool bForceRefresh)
{
    double fp=pInfo->GetDeviceData ()->AdamoData.fp;
    char szBuffer[256];

    if (bForceRefresh)   {
        sprintf (szBuffer, "%8.3f", fp);
        ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
    }
    else   {
        double fpDiPrima=pInfo->GetDeviceDataDiPrima ()->AdamoData.fp;
        if (!ISEQUAL (fp, fpDiPrima))   {
            ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
            sprintf (szBuffer, "%8.3f", fp);
            ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
            pInfo->GetDeviceDataDiPrima ()->AdamoData.fp=fp;
        }
    }
}

/*
** DrawCounter :
*/
void CIOSuperGrid::DrawCounter (HDC hDC, 
                                    CAdamoCounter* pDev, 
                                    CIOItemInfo* pInfo,
                                    CRect* rc,
                                    bool bForceRefresh)
{
    char szBuffer[256];

    if (bForceRefresh)   {
        szBuffer[0]='\0';
        switch (pInfo->GetDeviceData ()->nType)   {
            case LUA_TBOOLEAN :
                sprintf (szBuffer, "%s", pInfo->GetDeviceData ()->AdamoData.b ? "True" : "False");
                break;
            case LUA_TNUMBER :
                sprintf (szBuffer, "%8.3f", pInfo->GetDeviceData ()->AdamoData.fp);
                break;
            case LUA_TSTRING :
                sprintf (szBuffer, "%s", pInfo->GetDeviceData ()->AdamoData.str);
                break;
        }
        ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
    }
    else   {
        bool bReDraw=false;
        szBuffer[0]='\0';
        if (pInfo->GetDeviceData ()->nType == LUA_TBOOLEAN)   {
            int nValue=pInfo->GetDeviceData ()->AdamoData.b;
            int nValueDiPrima=pInfo->GetDeviceDataDiPrima ()->AdamoData.b;
            if (nValueDiPrima!=nValue)   {
                sprintf (szBuffer, "%s", pInfo->GetDeviceData ()->AdamoData.b ? "True" : "False");
                pInfo->GetDeviceDataDiPrima ()->AdamoData.b=nValue;
                bReDraw=true;
            }
        }
        else   {
            if (pInfo->GetDeviceData ()->nType == LUA_TNUMBER)   {
                double fpValue=pInfo->GetDeviceData ()->AdamoData.fp;
                double fpValueDiPrima=pInfo->GetDeviceDataDiPrima ()->AdamoData.fp;
                if (!ISEQUAL (fpValue, fpValueDiPrima))   {
                    sprintf (szBuffer, "%8.3f", pInfo->GetDeviceData ()->AdamoData.fp);
                    pInfo->GetDeviceDataDiPrima ()->AdamoData.fp=fpValue;
                    bReDraw=true;
                }
            }
            else   {
                if (pInfo->GetDeviceData ()->nType == LUA_TSTRING)   {
                    if (!strcmp (pInfo->GetDeviceData ()->AdamoData.str, pInfo->GetDeviceDataDiPrima ()->AdamoData.str))
                        sprintf (szBuffer, "%s", pInfo->GetDeviceData ()->AdamoData.str);
                        strcpy (pInfo->GetDeviceDataDiPrima ()->AdamoData.str, pInfo->GetDeviceData ()->AdamoData.str);
                        bReDraw=true;
                }
            }
        }
        if (bReDraw)
            ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
    }
}

/*
** DrawAxes :
*/
void CIOSuperGrid::DrawAxes (HDC hDC,
                                    CAdamoAxis* pDev, 
                                    CIOItemInfo* pInfo,
                                    CRect* rc,
                                    bool bForceRefresh)
{
    double fp=pInfo->GetDeviceData ()->AdamoData.fp;
    char szBuffer[256];

    if (bForceRefresh)   {
        sprintf (szBuffer, "%8.3f", fp);
        ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
    }
    else   {
        double fpDiPrima=pInfo->GetDeviceDataDiPrima ()->AdamoData.fp;
        if (!ISEQUAL (fp, fpDiPrima))   {
            ExtTextOut(hDC, 0, 0, ETO_OPAQUE, rc, NULL, 0, NULL);
            sprintf (szBuffer, "%8.3f", fp);
            ::DrawText (hDC, szBuffer, -1, rc, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
            pInfo->GetDeviceDataDiPrima ()->AdamoData.fp=fp;
        }
    }
}

/*
** SelettaInputDigitale :
*/
void CIOSuperGrid::SelettaInputDigitale (UINT nFlags, CPoint point, int nColumnX, CAdamoDgtIn* pInp)
{
    CDeviceData Data;
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
void CIOSuperGrid::SelettaOutputDigitale (UINT nFlags, CPoint point, int nColumnX, CAdamoDgtOut* pOut)
{
    CDeviceData Data;
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
void CIOSuperGrid::SelettaInputAnalogico (UINT nFlags, CAdamoAnalogIn* pInAna)
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
		dv->SetParentWnd (this);
        pMainFrame->AddDialogList (dv);
    }
}
}

/*
** SelettaOutputAnalogico :
*/
void CIOSuperGrid::SelettaOutputAnalogico (UINT nFlags, CAdamoAnalogOut* pOutAna)
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
		dv->SetParentWnd (this);
        pMainFrame->AddDialogList (dv);
    }
}
}

/*
** SelettaTimer :
*/
void CIOSuperGrid::SelettaTimer (UINT nFlags, CAdamoTimer* pTimer)
{
    if (nFlags&MK_CONTROL)   {
		CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd ();
		if (!pMainFrame->ExistDialogList (pTimer->GetPathName ()))   {
			CDeviceVariable *dv = new CDeviceVariable;
			dv->SetMachine (m_pMachine);
			dv->SetData (LOADSTRING (IDS_ADAMODEVICE_4), pTimer->GetPathName(), pTimer->GetName (), LUA_TNUMBER);
			dv->SetDevice (pTimer);
			dv->RunAlways ();
			dv->Create (IDD_GLOBALVARIABLE, this);
			dv->ShowWindow (SW_SHOWNORMAL);
			dv->CenterWindow ();
			dv->SetParentWnd (this);
			pMainFrame->AddDialogList (dv);
		}
	}
}

/*
** SelettaCounter :
*/
void CIOSuperGrid::SelettaCounter (UINT nFlags, CAdamoCounter* pCounter)
{
    if (nFlags&MK_CONTROL)   {
		CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd ();
		if (!pMainFrame->ExistDialogList (pCounter->GetPathName ()))   {
			CDeviceVariable *dv = new CDeviceVariable;
			dv->SetMachine (m_pMachine);
			dv->SetData (LOADSTRING (IDS_ADAMODEVICE_4), pCounter->GetPathName(), pCounter->GetName (), LUA_TNUMBER);
			dv->SetDevice (pCounter);
			dv->RunAlways ();
			dv->Create (IDD_GLOBALVARIABLE, this);
			dv->ShowWindow (SW_SHOWNORMAL);
			dv->CenterWindow ();
			dv->SetParentWnd (this);
			pMainFrame->AddDialogList (dv);
		}
	}
}

/*
** OnChangePsw :
*/
void CIOSuperGrid::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    ResetContent ();
    m_ePswLevel=newPsw;
    FillGrid ();
    Invalidate ();
}

/*
** CallIterator :
*/ 
void CIOSuperGrid::CallIterator (CProject *pPrj)
{
	if (pPrj)   {
		CAdamoDatiFile *pFile=new CAdamoDatiFile;
		bool bIsPhysical=pPrj->GetMachineType ()==enPhysical;
		if (pFile->Open (pPrj->GetParamsPathName (), TRUE))   {
			CXTPReportRecords* pRecords = GetRecords ();
			pFile->ReadData ();
			for (int i = 0; i<pRecords->GetCount (); i++)   {   
				CXTPReportRecord* pRecord = pRecords->GetAt (i);
				Iterator ((CAdamoGroup*)pPrj->GetMachineModule(), pRecord, pFile, bIsPhysical);
			}
			pFile->Close ();
		}
		delete pFile;
	}
}

/*
** GetSelectedDevice :
*/
void CIOSuperGrid::GetSelectedDevice (CString& strDevicePath, enDispositivi& e, int& nID)
{
	CXTPReportSelectedRows* pSelectedRows = GetSelectedRows ();
	CXTPReportRow* pRow;

	if (pSelectedRows->GetCount () > 0)   {
		pRow = pSelectedRows->GetAt (0);
		if (pRow)   {
			CIOItemInfo* pInfo=(CIOItemInfo*) pRow->GetRecord ()->GetItem (0)->GetItemData ();
			if (pInfo)   {
				CAdamoBase *pDev = pInfo->GetDevice ();
				if (pDev)   {
					strDevicePath = pDev->GetPathName () + "." + pDev->GetName ();
					nID = pDev->GetDeviceID ();
					e = pDev->GetType ();
				}
				else   {
					strDevicePath = "";
					e = enBase;
					nID = -1;
				}
			}
		}
	}
}

/*
** SelectDevice :
*/
void CIOSuperGrid::SelectDevice (enDispositivi e, int nID)
{
	CXTPReportRecords* pRecords = GetRecords ();
	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecord = pRecords->GetAt (i);
		if (pRecord->HasChildren ())
			IteratorSearch (pRecord, e, nID);
	}
}

/*
** IteratorSearch :
*/
int CIOSuperGrid::IteratorSearch (CXTPReportRecord* pRecord, enDispositivi e, int nID)
{
	CXTPReportRecords* pRecords = pRecord->GetChilds ();
	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecordItem = pRecords->GetAt (i);
		if (pRecordItem->HasChildren ())   {
			if (IteratorSearch (pRecordItem, e, nID))
				return 1;
		}
		else   {
			CIOItemInfo* pInfo = (CIOItemInfo *) pRecordItem->GetItem (0)->GetItemData ();
			CAdamoBase* pDev = pInfo->GetDevice ();
			if (pDev)   {
				if (pDev->GetDeviceID () == nID && pDev->GetType () == e)   {
					SelectRecord (pRecordItem);
					return 1;
				}
			}
		}
	}
	return 0;
}

/*
** SelectRecord :
*/
void CIOSuperGrid::SelectRecord (CXTPReportRecord* pRecord)
{
	CIOItemInfo* pObj = (CIOItemInfo* ) pRecord->GetItem (0)->GetItemData ();
	CXTPReportRows* pRows = GetRows();
	for (int i = 0; i<pRows->GetCount (); i++)   {
		CXTPReportRow* pRow = pRows->GetAt (i);
		CXTPReportRecord* pRowRecord = pRow->GetRecord ();
		CIOItemInfo* pRowObj = (CIOItemInfo* ) pRowRecord->GetItem (0)->GetItemData ();
		if (pRowObj == pObj)
			SetFocusedRow (pRow);
	}
}

/*
** OnReportRButtonDown :
*/
void CIOSuperGrid::OnReportRButtonDown(NMHDR* pNotifyStruct, LRESULT* pResult) 
{
 	XTP_NM_REPORTRECORDITEM* pItemNotify = reinterpret_cast<XTP_NM_REPORTRECORDITEM*>(pNotifyStruct);
   HMENU hMenu = NULL;
    HMENU hSubMenu;

	// TODO: Add your control notification handler code here
    if (((CMainFrame *) AfxGetMainWnd ())->GetPswLevel ()<=ePswBuilder)   {
	    CPoint pt;
		CIOItemInfo* pInfo=(CIOItemInfo*) pItemNotify->pRow->GetRecord ()->GetItem (0)->GetItemData ();
		if (pInfo)   {
            m_bForced = pInfo->Status (0) != 0;
            if (m_bForced)
		        hMenu = LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_FORCEDINPUT1));
            else
				hMenu = LoadMenu(theApp.m_hInstance, MAKEINTRESOURCE(IDR_MENU_FORCEDINPUT));
			if ( !hMenu )
				return;
				hSubMenu = GetSubMenu(hMenu, 0);
			if (!hSubMenu) 		{
				DestroyMenu(hMenu);
				return;
			}
			CAdamoBase *pDev = pInfo->GetDevice ();
			if (pDev)   {
				if (pDev->GetType () != enInputDigitale)
                    if (m_bForced)
					    ::EnableMenuItem (hSubMenu, ID_UNFORCESINGLEINPUT, MF_DISABLED);
                    else
					::EnableMenuItem (hSubMenu, ID_FORCESINGLEINPUT, MF_DISABLED);
				else   {
					m_nInput = pDev->GetDeviceID ();
					m_bForced = pInfo->Status (0) != 0;
				}
			}
		}
		GetCursorPos(&pt);
		::SetForegroundWindow(m_hWnd);	
		::TrackPopupMenu(hSubMenu, 0, pt.x, pt.y, 0, m_hWnd, NULL);
		DestroyMenu(hMenu);
    }	    
	*pResult = 0;
}

/*
** OnForceSingleInput :
*/
void CIOSuperGrid::OnForceSingleInput ()
{
	if (m_pMachine && m_nInput != -1)   
		m_pMachine->ForceInput (m_nInput, !m_bForced);
	m_nInput = -1;
}

/*
** OnUnForceSingleInput :
*/
void CIOSuperGrid::OnUnForceSingleInput ()
{
	if (m_pMachine && m_nInput != -1)   
		m_pMachine->ForceInput (m_nInput, !m_bForced);
	m_nInput = -1;
}

/*
** OnForceAllInputsON :
*/
void CIOSuperGrid::OnForceAllInputsOn()
{
	if (m_pMachine && m_nInput != -1)   
		m_pMachine->ForceInput (-1, true);
}

/*
** OnForceAllInputsOFF :
*/
void CIOSuperGrid::OnForceAllInputsOff ()
{
	if (m_pMachine)   
		m_pMachine->ForceInput (-1, false);
}

/*
** GetIndex :
*/
void CIOSuperGrid::GetIndex (CXTPReportRecord* pRecord, CAdamoBase* pDev, int& nIndex)
{
	CObject *pObj = pDev;
	nIndex = 0;
	for (int i = 0; i<pRecord->GetChilds()->GetCount (); i++)   {
		CXTPReportRecord* pRecReport = pRecord->GetChilds()->GetAt (i);
		CXTPReportRecord* pNextRecReport = NULL;
		if (i < pRecord->GetChilds()->GetCount () - 1)
			pNextRecReport = pRecord->GetChilds()->GetAt (i+1);
		CIOItemInfo* pInfo = (CIOItemInfo *) pRecReport->GetItem (0)->GetItemData ();
		CObject* pObjReport = pInfo->GetDevice ();
		CObject* pObjNextReport = NULL;
		if (pNextRecReport)   {
			pInfo = (CIOItemInfo *) pNextRecReport->GetItem (0)->GetItemData ();
			pObjNextReport = pInfo->GetDevice ();
		}
		int nOrder1 = GetDeviceOrder (pObjReport);
		int nOrder2 = GetDeviceOrder (pObj);
		int nOrder3 = INT_MAX;
		if (pObjNextReport)
			nOrder3 = GetDeviceOrder (pObjNextReport);
		if (nOrder2 < nOrder1)   {
			nIndex = pRecReport->GetIndex (); break;
		}
		else
			if (nOrder2 > nOrder1 && nOrder2 < nOrder3)   {
				nIndex = pRecReport->GetIndex ()+1; break;
			}
			else
				if (nOrder2 == nOrder1)   {
					CString str1 = ((CAdamoBase*) pObjReport)->GetName ();
					CString str2 = ((CAdamoBase*) pObj)->GetName ();
					CString str3 = "";
					if (pObjNextReport)
						str3 = ((CAdamoBase*) pObjNextReport)->GetName ();
					if (str2 < str1)   {
						nIndex = pRecReport->GetIndex (); break;
					}
					else
						if (str3 != "")   {
							if (str2 >= str1 && str2 < str3)   {
								nIndex = pRecReport->GetIndex ()+1; break;
							}
						}
						else   {
							nIndex = pRecReport->GetIndex ()+1; break;
						}
				}
	}
}

/*
** TogliIGruppiSenzaFigli :
*/
void CIOSuperGrid::TogliIGruppiSenzaFigli ()
{
	CXTPReportRecords* pRecords = GetRecords ();
	
	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecord = pRecords->GetAt (i);
		if (pRecord->HasChildren ())
			TogliIGruppiSenzaFigli (pRecord);
	}
}

/*
** TogliIGruppiSenzaFigli :
*/
void CIOSuperGrid::TogliIGruppiSenzaFigli (CXTPReportRecord* pRecord)
{
	bool bHasChildren = false;

	for (int i = 0; i<pRecord->GetChilds ()->GetCount (); i++)   {
		CXTPReportRecords* pChilds = pRecord->GetChilds ();
		CXTPReportRecord* pChild = pChilds->GetAt (i);
		if (pChild->HasChildren ())   {
			TogliIGruppiSenzaFigli (pChild);
			bHasChildren = true;
		}
		CIOItemInfo* pInfo = (CIOItemInfo* ) pChild->GetItem (0)->GetItemData ();
		if (!bHasChildren && pInfo->GetDevice ()->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))   {
			pRecord->GetChilds ()->RemoveAt (pChild->GetIndex ()); i--;
		}
	}
}

/*
** GetDeviceOrder :
*/
int CIOSuperGrid::GetDeviceOrder (CObject *pObj)
{
    if(!pObj)
        return NULL;

    if(pObj->IsKindOf(RUNTIME_CLASS(CProject)))
        return 1;
    else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))
        return 2;
    else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoDgtIn)))
        return 3;
    else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoDgtOut)))
        return 4;
    else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoAnalogIn)))
        return 5;
    else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoAnalogOut)))
        return 6;
    else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoAxis)))
        return 7;
    else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoTimer)))
        return 8;
    else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoCounter)))
        return 9;
    else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoGenericCan)))
        return 10;

    return NULL;
}

/*
** OnRefreshMetrics :
*/
LRESULT CIOSuperGrid::OnRefreshMetrics(WPARAM wParam, LPARAM /*lParam*/)
{
	SetSkin ((AppTheme) wParam);
	RedrawControl ();
	return 0;
}

/*
** SetSkin :
*/
LRESULT CIOSuperGrid::SetSkin (AppTheme nSkin)
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

/*
** OnLButtonDown :
*/
void CIOSuperGrid::OnLButtonDown (UINT nFlags, CPoint point)
{
	__super::OnLButtonDown (nFlags, point);
    if (((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ()<=ePswBuilder)  {
		m_ButtonDown = true;
		m_point = point;
	}
}

/*
** OnLButtonUp :
*/
void CIOSuperGrid::OnLButtonUp (UINT nFlags, CPoint point)
{
	__super::OnLButtonUp (nFlags, point);
	CPoint pt (point);
	ClientToScreen (&pt);
	// get the CWnd pointer of the window that is under the mouse cursor
	CWnd* pDropWnd = WindowFromPoint (pt);
	ASSERT (pDropWnd);
	m_ButtonDown = false;
	if (m_Dragging)   {
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CGridCtrl)))   {
			if (m_pRecord)   {
				CIOItemInfo* pInfo = (CIOItemInfo *) m_pRecord->GetItem (0)->GetItemData ();
				if (pInfo)
					pDropWnd->GetParent ()->SendMessage (WM_DRAGOSCDEVICE, 0, (LPARAM)pInfo->GetDevice ());
			}
		}
		m_Dragging = false;
		m_pRecord = NULL;
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		ReleaseCapture ();
	}
}

/*
** OnMouseMove :
*/
void CIOSuperGrid::OnMouseMove (UINT nFlags, CPoint point)
{
	if (!m_Dragging)   {
		__super::OnMouseMove (nFlags, point);
		if (m_ButtonDown)   {
			CPoint p = point - m_point;
			if (abs (p.x) > 4 || abs (p.y) > 4)   {
				CXTPReportSelectedRows* pSelectedRows = GetSelectedRows ();
				if (pSelectedRows->GetCount () > 0)   {
					CXTPReportRow* pRow = pSelectedRows->GetAt (0);
					if (pRow)   {
						m_pRecord = pRow->GetRecord ();
						m_Dragging = true;
						OnDrag (m_pRecord, point);
						SetCapture ();
					}
				}
			}
		}
	}
	else
		OnDrag (m_pRecord, point);
}

/*
** OnDrag :
*/
void CIOSuperGrid::OnDrag (CXTPReportRecord* pRecord, CPoint point)
{
	CPoint pt (point);
	ClientToScreen (&pt);
	CWnd* pDropWnd = WindowFromPoint (pt);
	ASSERT (pDropWnd);

	if (pDropWnd->IsKindOf (RUNTIME_CLASS (CGridCtrl)))
		SetCursor(AfxGetApp()->LoadCursor (IDC_DROPCURSOR));
	else
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_NO));
}

/*
** OnDrawCaption .
*/
void CXTPReportRecordItemViewDevices::OnDrawCaption (XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
								   XTP_REPORTRECORDITEM_METRICS* pMetrics)
{
	CXTPReportRecord* pRecord = pDrawArgs->pRow->GetRecord ();
	CIOItemInfo* pIOInfo = (CIOItemInfo *) pRecord->GetItem (0)->GetItemData ();
	switch (pIOInfo->GetDevice ()->GetType ())   {
		case enBase :
			break;
		case enInputDigitale :
			((CIOSuperGrid *)(pDrawArgs->pControl))->AccendiInputDig (pDrawArgs->pDC->GetSafeHdc (), 
				(CAdamoDgtIn*) pIOInfo->GetDevice (), 
				pIOInfo, 
				&pDrawArgs->rcItem);
			break;
		case enOutputDigitale :
			((CIOSuperGrid *)(pDrawArgs->pControl))->AccendiOutputDig (pDrawArgs->pDC->GetSafeHdc (), 
				(CAdamoDgtOut*) pIOInfo->GetDevice (), 
				pIOInfo, 
				&pDrawArgs->rcItem);
			break;
		case enInputAnalogico :
			((CIOSuperGrid *)(pDrawArgs->pControl))->DrawInputAnag (pDrawArgs->pDC->GetSafeHdc (), 
				(CAdamoAnalogIn*) pIOInfo->GetDevice (), 
				pIOInfo, 
				&pDrawArgs->rcItem);
			break;
		case enOutputAnalogico :
			((CIOSuperGrid *)(pDrawArgs->pControl))->DrawOutputAnag (pDrawArgs->pDC->GetSafeHdc (), 
				(CAdamoAnalogOut*) pIOInfo->GetDevice (), 
				pIOInfo, 
				&pDrawArgs->rcItem);
			break;
		case enCounter :
			((CIOSuperGrid *)(pDrawArgs->pControl))->DrawCounter (pDrawArgs->pDC->GetSafeHdc (), 
				(CAdamoCounter*) pIOInfo->GetDevice (), 
				pIOInfo, 
				&pDrawArgs->rcItem);
			break;
		case enTimer :
			((CIOSuperGrid *)(pDrawArgs->pControl))->DrawTimer (pDrawArgs->pDC->GetSafeHdc (), 
				(CAdamoTimer*) pIOInfo->GetDevice (), 
				pIOInfo, 
				&pDrawArgs->rcItem);
			break;
		case enAsse :
			((CIOSuperGrid *)(pDrawArgs->pControl))->DrawAxes (pDrawArgs->pDC->GetSafeHdc (), 
				(CAdamoAxis*) pIOInfo->GetDevice (), 
				pIOInfo, 
				&pDrawArgs->rcItem);
			break;
	}
}
