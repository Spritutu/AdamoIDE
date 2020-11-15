// SlotView.cpp : implementation file
//

#include "stdafx.h"
#include "SlotView.h"
#include "resource.h"
#include "mainframe.h"

#define NMAXSLOTINLISTVIEW 11

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlotView

/*
** Costruttore
*/
CSlotView::CSlotView() : m_pSlotsData (NULL), m_bFreeze (false), m_nLastSelection (0)
{
    m_pSlotsData=new stTaskData [NMAXSLOTINLISTVIEW+1];
}

/*
** Distruttore
*/
CSlotView::~CSlotView()
{
    if (m_pSlotsData)
        delete m_pSlotsData;
}

BEGIN_MESSAGE_MAP(CSlotView, CListCtrl)
	//{{AFX_MSG_MAP(CSlotView)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlotView message handlers

/*
** InitSlotView : inizializza il list view. Fa partire inoltre il timer.
*/
int CSlotView::InitSlotView ()
{
    InitListControl();
    m_nTimer=SetTimer (ID_SLOTTIMER, 200, NULL);
	return 0;
}

/*
** OnDestroy : distrugge i dati del list view
*/
void CSlotView::OnDestroy() 
{
	CListCtrl::OnDestroy();
	// TODO: Add your message handler code here
    DestroyListControl ();
	KillTimer(m_nTimer);
}

/*
** OnTimer : gestione del timer a 200 ms, rinfresca il listview
*/
void CSlotView::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent==ID_SLOTTIMER)   {
		if (!m_bFreeze)
			DaUnaRinfrescata ();
	}
}

/*
** InitListControl : configura il list control, crea bitmap e header.
*/
void CSlotView::InitListControl ()   {
    /* stringhe dell'header del list control */
    CString strItem1(LOADSTRING (IDS_ADAMOSLOTVIEW_1));
    CString strItem2(LOADSTRING (IDS_ADAMOSLOTVIEW_2));
    CString strItem3(LOADSTRING (IDS_ADAMOSLOTVIEW_3));
    CString strItem4(LOADSTRING (IDS_ADAMOSLOTVIEW_4));
    CRect rect;
    /* settiamo il font */
    SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());
    GetWindowRect(&rect);
    /* inseriamo le colonne */
	InsertColumn(0, strItem1, LVCFMT_LEFT,
		rect.Width() * 15/100, 0);
	InsertColumn(1, strItem2, LVCFMT_LEFT,
		rect.Width() * 39/100, 1);
	InsertColumn(2, strItem3, LVCFMT_LEFT,
		(rect.Width() * 23/100)-1, 2);
	InsertColumn(3, strItem4, LVCFMT_LEFT,
		(rect.Width() * 23/100)-2, 2);
    /* ora i bitmap dell' header */
    CHeaderCtrl* hc=GetHeaderCtrl();
    hc->SetFont(((CMainFrame*)AfxGetMainWnd ())->GetProgramFont ());

    /* ora i bitmap del listview */
    m_hlviml=new CImageList;
    m_bmpSSRun=new CBitmap;
    m_bmpSSNotExist=new CBitmap;
    m_bmpSSSuspended=new CBitmap;
    m_bmpSSZombie=new CBitmap;
    m_bmpSSWaitEvent=new CBitmap;
    m_bmpSSBreak=new CBitmap;
    m_bmpSSRun->LoadBitmap(IDB_SLOTSTATUSRUN);
    m_bmpSSNotExist->LoadBitmap (IDB_SLOTSTATUSNOTEXIST);
    m_bmpSSSuspended->LoadBitmap(IDB_SLOTSTATUSSUSPENDED);
    m_bmpSSZombie->LoadBitmap(IDB_SLOTSTATUSZOMBIE);
    m_bmpSSWaitEvent->LoadBitmap(IDB_SLOTSTATUSWAITEVENT);
    m_bmpSSBreak->LoadBitmap(IDB_SLOTSTATUSBREAK);
    m_hlviml->Create (16, 16, ILC_MASK|ILC_COLOR16, 0, 1);
    m_hlviml->Add(m_bmpSSNotExist, RGB(0, 128,128));
    m_hlviml->Add(m_bmpSSSuspended, RGB(0, 128,128));
    m_hlviml->Add(m_bmpSSZombie, RGB(0, 128,128));
    m_hlviml->Add(m_bmpSSWaitEvent, RGB(0, 128,128));
    m_hlviml->Add(m_bmpSSBreak, RGB(0, 128,128));
    m_hlviml->Add(m_bmpSSRun, RGB(0, 128,128));
    SetImageList(m_hlviml, LVSIL_SMALL);

    m_hciml=new CImageList;
    m_bmpHdrSlot=new CBitmap;
    m_bmpHdrNome=new CBitmap;
    m_bmpHdrStato=new CBitmap;
    m_bmpHdrEvento=new CBitmap;
    m_bmpHdrSlot->LoadBitmap(IDB_CAPSLOTS);
    m_bmpHdrNome->LoadBitmap(IDB_CAPTASK);
    m_bmpHdrStato->LoadBitmap(IDB_CAPSTATE);
    m_bmpHdrEvento->LoadBitmap(IDB_CAPEVENTS);
    m_hciml->Create (16, 16, ILC_MASK, 0, 1);
    m_hciml->Add(m_bmpHdrSlot, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrNome, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrStato, RGB(0, 128,128));
    m_hciml->Add(m_bmpHdrEvento, RGB(0, 128,128));
    hc->SetImageList(m_hciml);
    HD_ITEM hd;
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=0;
    hc->SetItem(0, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=1;
    hc->SetItem(1, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=2;
    hc->SetItem(2, &hd);
    hd.mask=HDI_IMAGE | HDI_FORMAT;
    hd.fmt= HDF_LEFT | HDF_IMAGE | HDF_STRING;
    hd.iImage=3;
    hc->SetItem(3, &hd);
    /* settiamo lo stile full row al listview */
    DWORD dw=GetExtendedStyle ();
    dw|=LVS_EX_FULLROWSELECT;
    SetExtendedStyle (dw);
    /* riempiamo tutti i nodi dei possibili slots */
    for (int i=0; i<NMAXTASKS; i++)
        AddItem (i);
    /* eseguiamo la prima rinfrescata */
    OnTimer (ID_SLOTTIMER); 
}

/*
** DestroyListControl : cancella la memoria allocata dalla InitListControl.
*/
void CSlotView::DestroyListControl ()
{
    if (m_bmpHdrSlot)
        delete m_bmpHdrSlot;
    if (m_bmpHdrNome)
        delete m_bmpHdrNome;
    if (m_bmpHdrStato)
        delete m_bmpHdrStato;
    if (m_bmpHdrEvento)
        delete m_bmpHdrEvento;
    if (m_bmpSSRun)
        delete m_bmpSSRun;
    if (m_bmpSSNotExist)
        delete m_bmpSSNotExist;
    if (m_bmpSSSuspended)
        delete m_bmpSSSuspended;
    if (m_bmpSSZombie)
        delete m_bmpSSZombie;
    if (m_bmpSSWaitEvent)
        delete m_bmpSSWaitEvent;
    if (m_bmpSSBreak)
        delete m_bmpSSBreak;
    if (m_hciml)
        delete m_hciml;
    if (m_hlviml)
        delete m_hlviml;
}

/*
** FillListControl aggiorna il list view in funzione di quello che arriva
**                 dal campo. Chiamata dal timer.
*/
void CSlotView::FillListControl (int nSlotIni, int nSlotFin)
{
    stTaskData* p=m_pSlotsData;
    int i, n;

    for (i=nSlotIni, n=0; i<=nSlotFin; i++, n++, p++)
        ModifyItem (p, i);              
}

/*
** ModifyItem : modifica un item al listrview.
*/
void CSlotView::ModifyItem (stTaskData* pSlotData, int nItem)
{
    LV_ITEM *lv=new LV_ITEM;
    char strSlotData[256];
    char strSlotEvent[256];
    int nIndex;

    GetStatusData (pSlotData, &nIndex, strSlotData, strSlotEvent);
    lv->iItem=nItem;
    lv->iSubItem=0;
    lv->mask=LVIF_IMAGE;
    lv->iImage=nIndex;
    SetItem (lv);
    memset (lv, 0, sizeof (LVITEM));
    lv->mask=LVIF_TEXT;
    lv->iItem=nItem;
    lv->iSubItem=1;
    lv->pszText=pSlotData->szTaskName;
    lv->cchTextMax=256;
    SetItem (lv);
    memset (lv, 0, sizeof (LVITEM));
    lv->mask=LVIF_TEXT;
    lv->iItem=nItem;
    lv->iSubItem=2;
    lv->pszText=strSlotData;
    lv->cchTextMax=256;
    SetItem (lv);
    memset (lv, 0, sizeof (LVITEM));
    lv->mask=LVIF_TEXT;
    lv->iItem=nItem;
    lv->iSubItem=3;
    lv->pszText=strSlotEvent;
    lv->cchTextMax=256;
    SetItem (lv);
    SetItemData (nItem, (DWORD)pSlotData);
	delete lv;
}

/*
** GetStatusData : la funzione ritorna l'indice all'interno della lista di
**                 immagini e la stringa corretta a seconda dello stato dello slot
*/
void CSlotView::GetStatusData (stTaskData* pTaskData, int* nIndex, char* strStatus, char* strEvent)
{
    strEvent[0]='\0';
    strStatus[0]='\0';
    if (pTaskData->nTask==-1)
        *nIndex=0;
    else   {
        if (!pTaskData->nStatus)   {
            /* task in running */
            *nIndex=5;
            strcpy (strStatus, LOADSTRING (IDS_ADAMOSLOTVIEW_5));
        }
        else   {
            /* task brekkata */
            if (pTaskData->nStatus&TS_BREAK)   {
                *nIndex=4;
                strcpy (strStatus, LOADSTRING (IDS_ADAMOSLOTVIEW_6));
            }
            else    {
                /* task morta e pronta per essere analizzata */
                if (pTaskData->nStatus&TS_ZOMBIE)   {
                    strcpy (strStatus, LOADSTRING (IDS_ADAMOSLOTVIEW_32));
                    *nIndex=2;
                }
                else   {
                    /* task sospesa */
                    if (pTaskData->nStatus&TS_SUSPENDED)   {
                        *nIndex=1;
                        strcpy (strStatus, LOADSTRING (IDS_ADAMOSLOTVIEW_7));
                    }
                    else   {
                        /* task in attesa di un evento */
                        if (pTaskData->nStatus&TS_WAITEVENT)    {
                            *nIndex=3;
                            strcpy (strStatus, LOADSTRING (IDS_ADAMOSLOTVIEW_8));
                            switch (pTaskData->nEvent)   {
                                case EVENTO_ASSIARRIVATI :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_9));
                                    break;
                                case EVENTO_ATTENDITASK :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_10));
                                    break;
                                case EVENTO_NUMINTBLOCKS :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_11));
                                    break;
                                case EVENTO_NUMENDPROFILE :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_12));
                                    break;
                                case EVENTO_DELAY :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_13));
                                    break;
                                case EVENTO_STATOQUOTE :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_14));
                                    break;
                                case EVENTO_STATOACCELERAZIONE :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_15));
                                    break;
                                case EVENTO_STATODECELERAZIONE :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_16));
                                    break;
                                case EVENTO_STATOREGIME :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_17));
                                    break;
                                case EVENTO_STATOFINESTRA :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_18));
                                    break;
                                case EVENTO_STATOTARGET :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_19));
                                    break;
                                case EVENTO_MESSAGGIODAMANDARE :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_20));
                                    break;
                                case EVENTO_MESSAGGIO_MANDATO :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_21));
                                    break;
                                case EVENTO_MESSAGGIO_RICEVUTO :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_22));
                                    break;
                                case EVENTO_ERRORI :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_23));
                                    break;
                                case EVENTO_MAILBOXLIBERA :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_24));
                                    break;
                                case EVENTO_ATTESAINGRESSO :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_25));
                                    break;
                                case EVENTO_ATTESAUSCITA :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_26));
                                    break;
                                case EVENTO_TIMER :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_27));
                                    break;
                                case EVENTO_SETPOINT :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_28));
                                    break;
                                case EVENTO_ASSIFERMI :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_29));
                                    break;
                                case EVENTO_BUFFERISOPIENO :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_30));
                                    break;
								case EVENTO_BUFFERISONONPIENO :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_34));
									break;
                                case EVENTO_BUFFERISOVUOTO :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_31));
                                    break;
								case EVENTO_RICHIESTASDO :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_35));
									break;
								case EVENTO_EC_MB :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_36));
									break;
								case EVENTO_NUMENDSPLINE :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_37));
									break;
								case EVENTO_ATTENDI_PROGRAMMA_G :
                                    strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_38));
									break;
								case EVENTO_ATTENDI_TC :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_39));
									break;
								case EVENTO_EC_STATES :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_40));
									break;
								case EVENTO_CALCOLA_TC :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_41));
									break;
								case EVENTO_NUMENDSINGLESPLINE :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_42));
									break;
								case EVENTO_CMMASTERPROCESSING :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_43));
									break;
								case EVENTO_CMMASTERMOVEMENT :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_44));
									break;
								case EVENTO_ATTENDI_ESECUTORE :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_45));
									break;
								case EVENTO_RUNTIMEMGA030 :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_46));
									break;
								case EVENTO_FREEZEINTERP :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_47));
									break;
								case EVENTO_ATTENDI_CC :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_48));
									break;
								case EVENTO_ATTENDI_UCC :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_49));
									break;
								case EVENTO_WAIT_CC :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_50));
									break;
								case EVENTO_ATTENDISYNC :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_51));
									break;
								case EVENTO_ATTENDI_SEMAPHORE :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_52));
									break;
								case EVENTO_INIT_SSCNET :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_53));
									break;
								case EVENTO_RAM_STORAGE :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_54));
									break;
								case EVENTO_READ_SSCNET_DISPLAY :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_55));
									break;
								case EVENTO_WAIT_RACCORDO_AL_VOLO :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_56));
									break;
								case EVENTO_WAIT_TICK :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_57));
									break;
								case EVENTO_WAIT_PP_BLOCK :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_58));
									break;
								case EVENTO_ATTENDI_SEMAPHORE_RT :
									strcpy (strEvent, LOADSTRING (IDS_ADAMOSLOTVIEW_59));
									break;
                            }
                        }
                        else   {
                            /* ancora in esecuzione */
                            *nIndex=5;
                            strcpy (strStatus, LOADSTRING (IDS_ADAMOSLOTVIEW_33));
                        }
                    }
                }
            }
        }
    }
}

int CSlotView::GetSelectedSlot (stTaskData* td)
{
    int n;
    if ((n=GetNextItem (-1, LVIS_SELECTED))!=-1)
        memcpy (td, (stTaskData*)GetItemData(n), sizeof (stTaskData));
    return n;    
}

void CSlotView::Freeze ()
{
    m_bFreeze=true;
}

void CSlotView::Refresh ()
{
    m_bFreeze=false;
}

void CSlotView::DaUnaRinfrescata ()
{
    int nSlotIni, nSlotFin;

    nSlotIni=GetTopIndex ();
    nSlotFin=nSlotIni+NMAXSLOTINLISTVIEW;
    if (nSlotFin>NMAXTASKS-1)
        nSlotFin=NMAXTASKS-1;
    /* leggiamo i dati dal campo */
    if (m_pMachine->GetTaskData (nSlotIni, nSlotFin, m_pSlotsData) == S_OK)
        /* riempiamo il list control */
        FillListControl (nSlotIni, nSlotFin);
}

/*
** AddItem : aggiunge un item al listrview.
*/
void CSlotView::AddItem (int nItem)
{
    LV_ITEM lv;
    char szBuffer[256];

    memset (&lv, 0, sizeof (LVITEM));
    lv.mask=LVIF_TEXT;
    lv.iItem=nItem;
    lv.iSubItem=0;
    lv.iImage=0;
    sprintf (szBuffer, "%d", nItem);
    lv.pszText=szBuffer;
    lv.cchTextMax=256;
    InsertItem (&lv);
}
