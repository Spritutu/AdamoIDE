// ConfTree.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "ConfTreeView.h"
#include "MainFrame.h"
#include "NewDev.h"
#include "OleDropSourceEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfTreeView

IMPLEMENT_DYNCREATE(CConfTreeView, CPaneView)

CConfTreeView::CConfTreeView() : m_pPlant(NULL),m_isConfig(FALSE),m_isHW(FALSE),m_objCurNode (NULL),
                                 m_objCurObj (NULL),m_objCurProject (NULL),m_objCurrentGroup (NULL),
                                 m_pConfFind (NULL), m_bLoaded (false), m_bInitialized (false), m_ePswLevel (ePswUser),
								 m_pRecord (NULL)
{

}

CConfTreeView::~CConfTreeView()
{

}

BEGIN_MESSAGE_MAP(CConfTreeView, CPaneView)
	//{{AFX_MSG_MAP(CConfTreeView)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_DEVICE_ADDAXIS, OnEditDeviceAddaxis)
	ON_COMMAND(ID_EDIT_ADDGROUP, OnEditAddgroup)
	ON_COMMAND(ID_EDIT_DEVICE_ADDANALOGINPUT, OnEditDeviceAddanaloginput)
	ON_COMMAND(ID_EDIT_DEVICE_ADDANALOGOUT, OnEditDeviceAddanalogout)
	ON_COMMAND(ID_EDIT_DEVICE_ADDCOUNTER, OnEditDeviceAddcounter)
	ON_COMMAND(ID_EDIT_DEVICE_ADDDIGITALIN, OnEditDeviceAdddigitalin)
	ON_COMMAND(ID_EDIT_DEVICE_ADDDIGITALOUT, OnEditDeviceAdddigitalout)
	ON_COMMAND(ID_EDIT_DEVICE_ADDTIMER, OnEditDeviceAddtimer)
	ON_COMMAND(ID_EDIT_DEVICE_ADDGENERIC, OnEditDeviceAddgeneric)
	ON_COMMAND(ID_EDIT_ADDMACHINE, OnEditAddmachine)
	ON_COMMAND(IDM_ACTIVATE_MACHINE, OnActivateMachine)
	ON_UPDATE_COMMAND_UI(IDM_ACTIVATE_MACHINE, OnUpdateActivateMachine)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEVICE_ADDAXIS, OnUpdateEditDeviceAddaxis)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDGROUP, OnUpdateEditAddgroup)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEVICE_ADDANALOGINPUT, OnUpdateEditDeviceAddanaloginput)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEVICE_ADDGENERIC, OnUpdateEditDeviceAddgeneric)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEVICE_ADDTIMER, OnUpdateEditDeviceAddtimer)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEVICE_ADDDIGITALOUT, OnUpdateEditDeviceAdddigitalout)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEVICE_ADDDIGITALIN, OnUpdateEditDeviceAdddigitalin)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEVICE_ADDCOUNTER, OnUpdateEditDeviceAddcounter)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DEVICE_ADDANALOGOUT, OnUpdateEditDeviceAddanalogout )
	ON_UPDATE_COMMAND_UI(ID_EDIT_ADDMACHINE, OnUpdateEditDeviceAddmachine )
	ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, ID_REPCTRL_CONFTREE, OnReportSelChanged)
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND, OnUpdateEditFind)
    ON_MESSAGE (WM_CLOSEDIALOGFIND, OnCloseDialogFind)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* To Do List */
// Find delle stringhe nell'albero
// Notifiche TVN_KEYDOWN e TVN_BEGINDRAG
// Notifica NM_RCLICK
// + SortItem
// + Gestione dell skin
// + CreateNewDev, implementare
// + abilitare o nascondere le colonne

/////////////////////////////////////////////////////////////////////////////
// CConfTreeView drawing

void CConfTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CConfTreeView diagnostics

#ifdef _DEBUG
void CConfTreeView::AssertValid() const
{
	CPaneView::AssertValid();
}

void CConfTreeView::Dump(CDumpContext& dc) const
{
	CPaneView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConfTreeView message handlers
int CConfTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPaneView::OnCreate (lpCreateStruct) == -1)
		return -1;
	CMainFrame *pMainFrame=(CMainFrame *)AfxGetMainWnd ();
	if (pMainFrame)
		m_ePswLevel=pMainFrame->GetPswLevel ();
	return 0;
}

CXTPReportRecord* CConfTreeView::AddItem(CObject *pObj) 
{
	CXTPReportRecord* pRecord = NULL;
	CXTPReportRow* pRow = NULL;
	CObject *pSelect = NULL;
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows ();

	if (pSelectedRows->GetCount () > 0)   {
		pRow = pSelectedRows->GetAt (0);
		if (pRow)   {
			pRecord = pRow->GetRecord ();
			CXTPReportRecordItem* pItem = pRecord->GetItem (0);
			if (pItem)
				pSelect = (CObject *) pItem->GetItemData ();
		}
	}
    if (pSelect && !(pSelect->IsKindOf(RUNTIME_CLASS(CAdamoGroup)) || pSelect->IsKindOf(RUNTIME_CLASS(CProject))))
		pRecord = pRecord->GetParentRecord ();
    return AggiungiDispositivoAlTree(pObj, pRecord);
}

CXTPReportRecord* CConfTreeView::AddItemRecord (CObject *pObj, CXTPReportRecord* pRecord) 
{
	CXTPReportRecord* pRetRecord = NULL;
	CXTPReportRecordItemText* pItem;
    CString strName;
    CString strDesc;
	int nIconIndex;

    if(pRecord == NULL || pObj->IsKindOf(RUNTIME_CLASS(CProject))){
		/* e' una macchina, aggiungiamo un record root */
        strName = ((CAdamoBase*)((CProject*)pObj)->GetMachineModule())->GetName();
        strDesc = ((CAdamoBase*)((CProject*)pObj)->GetMachineModule())->GetDesc();
		pRetRecord = pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
		pItem = new CXTPReportRecordItemText (strName);
		pItem->SetItemData ((DWORD_PTR) pObj);
		pItem->SetIconIndex (0);
		pRecord->AddItem (pItem);
		pItem = new CXTPReportRecordItemText (strDesc);
		pRecord->AddItem (pItem);
		pItem = new CXTPReportRecordItemText ("");
		pRecord->AddItem (pItem);
	}
	else    {
		if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoGroup))){
			strName = ((CAdamoBase*)pObj)->GetName();
			strDesc = ((CAdamoBase*)pObj)->GetDesc();
			nIconIndex = 2;
		}else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoAxis))){
			strName = ((CAdamoBase*)pObj)->GetName();
			strDesc = ((CAdamoBase*)pObj)->GetDesc();
			nIconIndex = 3;
		}else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoAnalogIn))){
			strName = ((CAdamoBase*)pObj)->GetName();
			strDesc = ((CAdamoBase*)pObj)->GetDesc();
			nIconIndex = 4;
		}else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoAnalogOut))){
			strName = ((CAdamoBase*)pObj)->GetName();
			strDesc = ((CAdamoBase*)pObj)->GetDesc();
			nIconIndex = 5;
		}else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoDgtIn))){
			strName = ((CAdamoBase*)pObj)->GetName();
			strDesc = ((CAdamoBase*)pObj)->GetDesc();
			nIconIndex = 6;
		}else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoDgtOut))){
			strName = ((CAdamoBase*)pObj)->GetName();
			strDesc = ((CAdamoBase*)pObj)->GetDesc();
			nIconIndex = 7;
		}else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoCounter))){
			strName = ((CAdamoBase*)pObj)->GetName();
			strDesc = ((CAdamoBase*)pObj)->GetDesc();
			nIconIndex = 8;
		}else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoTimer))){
			strName = ((CAdamoBase*)pObj)->GetName();
			strDesc = ((CAdamoBase*)pObj)->GetDesc();
			nIconIndex = 9;
		}else if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoGenericCan))){
			strName = ((CAdamoBase*)pObj)->GetName();
			strDesc = ((CAdamoBase*)pObj)->GetDesc();
			nIconIndex = 10;
		}
		CXTPReportRecord* pChild = new CXTPReportRecord();
		pItem = new CXTPReportRecordItemText (strName);
		pItem->SetItemData ((DWORD_PTR) pObj);
		pItem->SetIconIndex (nIconIndex);
		pChild->AddItem (pItem);
		pItem = new CXTPReportRecordItemText (strDesc);
		pChild->AddItem (pItem);
		pItem = new CXTPReportRecordItemText ("");
		pChild->AddItem (pItem);
		/* cerchiamo l'indice corretto */
		int nIndex = 0;
		for (int i = 0; i<pRecord->GetChilds()->GetCount (); i++)   {
			CXTPReportRecord* pRecReport = pRecord->GetChilds()->GetAt (i);
			CXTPReportRecord* pNextRecReport = NULL;
			if (i < pRecord->GetChilds()->GetCount () - 1)
				pNextRecReport = pRecord->GetChilds()->GetAt (i+1);
			CObject* pObjReport = (CObject* )pRecReport->GetItem (0)->GetItemData ();
			CObject* pObjNextReport = NULL;
			if (pNextRecReport)
				pObjNextReport = (CObject* )pNextRecReport->GetItem (0)->GetItemData ();
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
		pRecord->GetChilds()->InsertAt (nIndex, pChild);
		pRetRecord = pChild;
	}       
    return pRetRecord;
}

void CConfTreeView::UpdateTree()
{   
    CProject* pProject;
    /* cancelliamo tutti gli elementi */
	m_wndReport.ResetContent ();
	if (m_isConfig||m_isHW)   {
        /* carichiamo tutte le macchine */
        if (m_pPlant)   {
            m_pPlant->Init();
            while (m_pPlant->More())   {
                pProject = (CProject*)m_pPlant->GetNext();
                CXTPReportRecord* pRecord = AddItem (pProject);
                if (pRecord)
                    Iterator (pProject->GetMachineModule(), pRecord);
            }
            if (m_isHW)
                TogliIGruppiSenzaFigli ();
        }
        /* attiviamo la macchina corrente */
        ActivateMachine (0);
        SearchItemDrop();
        m_wndReport.SetFocus ();
    }
    else   {
        /* carichiamo solo il progetto attivo */
        pProject=GETWS()->GetActiveProject();
        if (pProject)   {
            CXTPReportRecord* pRecord = AddItem (pProject);
            Iterator (pProject->GetMachineModule(), pRecord);
        }
    }
	CXTPReportRows* pRows = m_wndReport.GetRows ();
	if (pRows->GetCount () > 0)   {
		CXTPReportRow* pRow = pRows->GetAt (0);
		m_wndReport.SetFocusedRow (pRow);
	}
    /* focus sull'albero e seleziona il primo item */
    m_wndReport.SetFocus ();
	m_wndReport.Populate ();
	/* esplodiamo tutti i rami */
	m_wndReport.ExpandAll (TRUE);
	/* selezioniamo il primo record */
    m_bLoaded=true;
}

void CConfTreeView::Iterator(CAdamoGroup *pGrp, CXTPReportRecord* pRecord)
{
    pGrp->Init();
    pGrp->DevInit();

    while(pGrp->DevMore()){
        CAdamoBase *pDisp = (CAdamoBase*)pGrp->DevGetNext();
        if(pDisp)
            AggiungiDispositivoAlTree(pDisp, pRecord);
    }

    while(pGrp->More()){
        CAdamoGroup *obj = (CAdamoGroup*)pGrp->GetNext();
        CXTPReportRecord* pParent = AggiungiDispositivoAlTree (obj, pRecord);
        if (pParent)
            Iterator (obj, pParent);
    }
}

void CConfTreeView::Iterator(CAdamoGroup *pGrp)
{
    pGrp->Init();
    pGrp->DevInit();

    while(pGrp->DevMore()){
        CObject *dev = (CObject*)pGrp->DevGetNext();
        ((CAdamoBase*)dev)->SetParentObj((CObject*)pGrp);
    }

    while(pGrp->More()){
        CObject *obj = (CObject*)pGrp->GetNext();
        ((CAdamoBase*)obj)->SetParentObj((CObject*)pGrp);
        Iterator((CAdamoGroup*)obj);
    }
}

void CConfTreeView::SendObj(CXTPReportRecord* pRecord, CObject *pObj)
{   
    if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoGroup)) || pObj->IsKindOf(RUNTIME_CLASS(CProject))){
        m_objCurNode = pObj;
        m_objCurObj = NULL;
    }
	else   {
        CXTPReportRecord* pParent = pRecord->GetParentRecord ();
        m_objCurNode = (CObject*)pParent->GetItem (0)->GetItemData ();
        m_objCurObj = pObj;
    }
	CProject* pProject = GetProject (pRecord);
    if (pProject)
        m_objCurProject = pProject;
    CMainFrame* pFrm = (CMainFrame*)(AfxGetApp()->GetMainWnd());
    CDockDevProperty *pDock = pFrm->GetDockDevProperty();
    if(pDock)
        pDock->SetObject(pObj);
}

void CConfTreeView::StoreObj(CXTPReportRecord* pRecord, CObject *pObj)
{   
    if(pObj->IsKindOf(RUNTIME_CLASS(CAdamoGroup)) || pObj->IsKindOf(RUNTIME_CLASS(CProject)))  {
        m_objCurNode = pObj;
        m_objCurObj = NULL;
    }
	else   {
        CXTPReportRecord* pParent = pRecord->GetParentRecord ();
        m_objCurNode = (CObject*)pParent->GetItem (0)->GetItemData ();
        m_objCurObj = pObj;
    }
	CProject* pProject = GetProject (pRecord);
    if (pProject)
        m_objCurProject = pProject;
}

CProject* CConfTreeView::GetProject(CXTPReportRecord* pRecord)
{
	CXTPReportRecord* pParent = pRecord->GetParentRecord ();
    CObject *pObj = NULL;

    while(pParent){
        pObj = (CObject*)pParent->GetItem (0)->GetItemData ();
        if(pObj->IsKindOf(RUNTIME_CLASS(CProject)))
            return (CProject*)pObj;
        pParent = pParent->GetParentRecord ();
    }

    return (CProject*)pRecord->GetItem (0)->GetItemData ();
}

void CConfTreeView::DeleteItem(CObject *pObj)
{   
	CXTPReportRows* pRows = m_wndReport.GetRows();
	for (int i = 0; i<pRows->GetCount (); i++)   {
		CXTPReportRow* pRow = pRows->GetAt (i);
		CXTPReportRecord* pRecord = pRow->GetRecord ();
		CObject* pRowObj = (CObject* ) pRecord->GetItem (0)->GetItemData ();
		if (pRowObj == pObj)   {
			m_wndReport.RemoveRowEx (pRow);
			if (i >= pRows->GetCount ())
				i--;
			CXTPReportRow* pRow = pRows->GetAt (i);
			m_wndReport.SetFocusedRow (pRow);
			LRESULT nResult;
			OnReportSelChanged (NULL, &nResult);
			m_wndReport.SetFocus ();
			break;
		}
	}
}

void CConfTreeView::Update(CObject *pObj)
{
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows ();
	CXTPReportRow* pRow;
	CXTPReportRecord* pRecord;

	if (pSelectedRows->GetCount () > 0)   {
		pRow = pSelectedRows->GetAt (0);
		if (pRow)   {
			pRecord = pRow->GetRecord ();
			CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*) pRecord->GetItem (0);
			pItem->SetValue (((CAdamoBase*)pObj)->GetName());
			pItem = (CXTPReportRecordItemText*) pRecord->GetItem (1);
			pItem->SetValue (((CAdamoBase*)pObj)->GetDesc());
			m_wndReport.UpdateRecord (pRecord, FALSE);
		}
	}
}

/*
** OnInitialUpdate :
*/
void CConfTreeView::OnInitialUpdate() 
{
    CPaneView::OnInitialUpdate();    
    if (!m_bInitialized)   {
		CDocument *pDoc = GetDocument ();
        InitView ();
        m_bInitialized=true;
    }
}

/*
** InitView :
*/
void CConfTreeView::InitView ()
{
	CXTPReportColumn* pColumn;
    CString str;

	/* per prima cosa creiamo il report control */
	m_wndReport.Create ( WS_CHILD | WS_VISIBLE, CRect(0,0, 0, 0), this, ID_REPCTRL_CONFTREE);
    /*aggiunge le colonnne al tree*/
	pColumn = new CXTPReportColumn(0, LOADSTRING (IDS_ADAMOCONFTREEVIEW_1), 300);
	m_wndReport.AddColumn(pColumn);
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetIconID (21);
	pColumn = new CXTPReportColumn(1, LOADSTRING (IDS_ADAMOCONFTREEVIEW_2), 300);
	pColumn->SetIconID (46);
	m_wndReport.AddColumn(pColumn);
	pColumn = new CXTPReportColumn(2, LOADSTRING (IDS_ADAMOCONFTREEVIEW_8), 300);
	pColumn->SetIconID (44);
	m_wndReport.AddColumn(pColumn);
	/* inizializziamo le immagini */
	UpdateImages ();
	RefreshMetrics (((CIdeApp *)AfxGetApp ())->GetTheme ());
}

int CALLBACK CConfTreeView::CompareFunc (LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    return GetDeviceOrder((CObject*)lParam1) - GetDeviceOrder((CObject*)lParam2);
}

int CConfTreeView::GetDeviceOrder(CObject *pObj)
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
** OnEditAddmachine :
*/
void CConfTreeView::OnEditAddmachine ()
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)   {
	    CProject *pPrj;
        if (pPrj=GETWS()->NewProject())    {
            CXTPReportRecord* pRecord = AddItem(pPrj);
			m_wndReport.Populate ();
			if(pRecord)   {
				pRecord->SetExpanded (TRUE);
				SelectRecord (pRecord);
			}
            GETWS()->ActivateMachine (pPrj->GetModuleNumber ());
            pPrj->CreateByType ();
            pPrj->Save ();
        }
    }
}

void CConfTreeView::OnEditAddgroup() 
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)
        CreateNewDev(RUNTIME_CLASS(CAdamoGroup));
}

void CConfTreeView::OnEditDeviceAddaxis() 
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)
        CreateNewDev(RUNTIME_CLASS(CAdamoAxis));
}

void CConfTreeView::OnEditDeviceAddanaloginput() 
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)
        CreateNewDev(RUNTIME_CLASS(CAdamoAnalogIn));
}

void CConfTreeView::OnEditDeviceAddanalogout() 
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)
        CreateNewDev(RUNTIME_CLASS(CAdamoAnalogOut));	
}

void CConfTreeView::OnEditDeviceAddcounter() 
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)
        CreateNewDev(RUNTIME_CLASS(CAdamoCounter));		
}

void CConfTreeView::OnEditDeviceAdddigitalin() 
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)
        CreateNewDev(RUNTIME_CLASS(CAdamoDgtIn));			
}

void CConfTreeView::OnEditDeviceAdddigitalout() 
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)
        CreateNewDev(RUNTIME_CLASS(CAdamoDgtOut));				
}

void CConfTreeView::OnEditDeviceAddtimer() 
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)
        CreateNewDev(RUNTIME_CLASS(CAdamoTimer));					
}

void CConfTreeView::OnEditDeviceAddgeneric() 
{
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()!=CMainFrame::modeHardware)
        CreateNewDev(RUNTIME_CLASS(CAdamoGenericCan));						
}

BOOL CConfTreeView::CreateNewDev(CRuntimeClass *pRunTime,CArchive *pAr)
{
    BOOL b=FALSE;

    if (m_objCurNode)  {

        if(m_objCurNode->IsKindOf(RUNTIME_CLASS(CAdamoGroup))&&pRunTime==RUNTIME_CLASS(CAdamoGroup))   {
            if (!CheckPathLength ())   {
                AfxMessageBox (LOADSTRING (IDS_ADAMOCONFTREEVIEW_3), MB_ICONWARNING);
                return FALSE;
            }
        }

        CNewDev objNewDev(pRunTime,m_objCurNode);
        CObject *pDev = (CObject*)objNewDev.GetObject();

        if(pAr)   {
            pDev->Serialize(*pAr);
            CopyNameManage(pDev);
            if(pDev->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))
                Iterator((CAdamoGroup*)pDev);
			if(pDev->IsKindOf(RUNTIME_CLASS(CAdamoAxis)))   {
				((CAdamoAxis *)pDev)->SetGName ("");
				((CAdamoAxis *)pDev)->SetAlias ("");
			}
        }

        if(m_objCurProject)
            m_objCurProject->SetMachineModifiedFlag (true);

        if(m_objCurNode->IsKindOf(RUNTIME_CLASS(CProject)))
            ((CAdamoBase*)pDev)->SetParentObj(((CProject*)m_objCurNode)->GetMachineModule());
        else if(m_objCurNode->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))
            ((CAdamoBase*)pDev)->SetParentObj(m_objCurNode);

        AddTail(m_objCurNode,pDev);
       
        CXTPReportRecord* pRecord = AddItem(pDev);
		m_wndReport.Populate ();
		if(pRecord)   {
			pRecord->SetExpanded (TRUE);
	        SelectRecord (pRecord);
		}
        b=TRUE;
    }
    return b;
}

void CConfTreeView::AddTail(CObject *pNode,CObject *pObj)
{
    if(pNode->IsKindOf(RUNTIME_CLASS(CProject)))
        ((CAdamoGroup*)((CProject*)pNode)->GetMachineModule())->AddTail(pObj);
    else if(pNode->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))
        ((CAdamoGroup *)pNode)->AddTail(pObj);
}

void CConfTreeView::CopyNameManage(CObject *pObj)
{
    CString strName(((CAdamoBase*)pObj)->GetName());
    CString strNew(strName);
    int nCount = 0;
    CAdamoGroup *pNode;

    if(m_objCurNode->IsKindOf(RUNTIME_CLASS(CProject)))
        pNode = (CAdamoGroup*)((CProject*)m_objCurNode)->GetMachineModule();
    else
        pNode = (CAdamoGroup*)m_objCurNode;

	pNode->Init();
    pNode->DevInit();

    while(pNode->DevMore()){
        CAdamoBase *pDev = (CAdamoBase*)pNode->DevGetNext();
        if(pDev->GetName() == strNew){
            if(nCount)
                strNew.Format(LOADSTRING (IDS_ADAMOCONFTREEVIEW_4), ++nCount, strName);
            else    {
                strNew.Format(LOADSTRING (IDS_ADAMOCONFTREEVIEW_5), strName);
                ++nCount;
            }
        }
    }

    while(pNode->More()){
        CAdamoBase *pGroup = (CAdamoBase*)pNode->GetNext();
        if(pGroup->GetName() == strNew){
            if(nCount)
                strNew.Format(LOADSTRING (IDS_ADAMOCONFTREEVIEW_4), ++nCount, strName);
            else   {
                strNew.Format(LOADSTRING (IDS_ADAMOCONFTREEVIEW_5), strName);
                ++nCount;
            }
        }
    }

    ((CAdamoBase*)pObj)->SetName(strNew);
}

void CConfTreeView::OnEditCopy() 
{
    CObject *pTmpCopyObj = NULL;

    if(m_objCurObj)
        pTmpCopyObj = m_objCurObj;
    else if(m_objCurNode)
        pTmpCopyObj = m_objCurNode;

    if(pTmpCopyObj){
        CSharedFile mf (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
        CArchive ar(&mf, CArchive::store);    
        ar.WriteClass(pTmpCopyObj->GetRuntimeClass());
        CObject *pCopyObj = NULL;
        ar.Write(&pCopyObj,sizeof(CObject));    
        pTmpCopyObj->Serialize(ar);    
        ar.Close(); // Flush and close.
        HGLOBAL hMem = mf.Detach();
        if (!hMem) return;
        // Send the Clipboard the data.
        OpenClipboard();
        EmptyClipboard();
        SetClipboardData(theApp.m_uiCopyClip, hMem);
        CloseClipboard();
    }
}

void CConfTreeView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
    if (((CMainFrame *)AfxGetMainWnd ())->GetMode()!=CMainFrame::modeHardware)   {
        if (m_ePswLevel<=ePswBuilder)
            if(m_objCurObj)
                pCmdUI->Enable(((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                               ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic);
            else if(m_objCurNode)
                pCmdUI->Enable(!m_objCurNode->IsKindOf(RUNTIME_CLASS(CProject)) && 
					        (((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                             ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic));
            else
                pCmdUI->Enable(FALSE);
        else
            pCmdUI->Enable(FALSE);
    }
    else
        pCmdUI->Enable(FALSE);
}

void CConfTreeView::OnEditCut() 
{
    CObject *pTmpCopyObj = NULL;

    if(m_objCurObj)
        pTmpCopyObj = m_objCurObj;
    else if(m_objCurNode)
        pTmpCopyObj = m_objCurNode;

    if(pTmpCopyObj){
        CObject *pCopyObj=NULL;
        CSharedFile mf (GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
        CArchive ar(&mf, CArchive::store);      
        ar.WriteClass(pTmpCopyObj->GetRuntimeClass());
        pCopyObj = pTmpCopyObj;
        ar.Write(&pCopyObj,sizeof(CObject));    
        pTmpCopyObj->Serialize(ar);    
        ar.Close(); // Flush and close.
        HGLOBAL hMem = mf.Detach();
        if (!hMem) return;
        // Send the Clipboard the data.
        OpenClipboard();
        EmptyClipboard();
        SetClipboardData(theApp.m_uiCopyClip, hMem);
        CloseClipboard();
    }
}

void CConfTreeView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
    OnUpdateEditCopy(pCmdUI);
}

void CConfTreeView::OnEditPaste() 
{
    OpenClipboard();
    HGLOBAL hMem = ::GetClipboardData(theApp.m_uiCopyClip);    
    if (!hMem) {
        CloseClipboard();
        return;
    }    
    // Create a mem file.
    CSharedFile mf;
    mf.SetHandle(hMem); 
    // Create the archive and get the data.
    CArchive ar(&mf, CArchive::load);
    CRuntimeClass *pRuntime = ar.ReadClass();
    CObject *pCopyObj;
    ar.Read(&pCopyObj,sizeof(CObject));
    CreateNewDev (pRuntime,&ar);
    if(pCopyObj)   {
        ClearObj(pCopyObj);
        ::EmptyClipboard();
    }
    ar.Close();
    mf.Detach();
    CloseClipboard();
}

void CConfTreeView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
    if (m_ePswLevel<=ePswBuilder)    {
        OpenClipboard();
        UINT uiFmt = 0;
        while (uiFmt = EnumClipboardFormats(uiFmt)) {
            if (uiFmt == theApp.m_uiCopyClip && 
				    (((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig ||
                     ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic)) {
                CloseClipboard();
                pCmdUI->Enable(TRUE);
                return;
            }
        }
        pCmdUI->Enable(FALSE);
        CloseClipboard();    
    }
    else
        pCmdUI->Enable(FALSE);
}

void CConfTreeView::OnEditClear() 
{
    if ( AfxMessageBox(LOADSTRING (IDS_ADAMOCONFTREEVIEW_6), MB_YESNO|MB_ICONQUESTION)==IDYES)   {
		if(m_objCurObj)   {
            ClearObj(m_objCurObj);
			m_objCurObj = NULL;
		}
		else if(m_objCurNode)   {
            ClearObj(m_objCurNode);
			m_objCurNode = NULL;
		}
        else if(m_objCurProject)   {
            m_objCurProject->SetMachineModifiedFlag (true);
            ClearObj(m_objCurProject);
        }
    }
}

void CConfTreeView::ClearObj(CObject *pObj)
{
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd ();
    if(pFrame){
        DeleteItem(pObj);
        pFrame->GetWorkSpace()->DeleteObject(pObj);
    }
}

void CConfTreeView::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

void CConfTreeView::DropItemOnList(int iIndexItem)
{
	switch(iIndexItem){
	    case 0:
            OnEditAddmachine();
	    break;
	    case 1:
			OnEditAddgroup();
	    break;
	    case 2:
			OnEditDeviceAddaxis();
	    break;
	    case 3:
			OnEditDeviceAddcounter();
	    break;
	    case 4:
			OnEditDeviceAddtimer();
	    break;
	    case 5:
			OnEditDeviceAddanaloginput();
	    break;
	    case 6:
			OnEditDeviceAddanalogout();
	    break;
	    case 7:
			OnEditDeviceAdddigitalin();
	    break;
	    case 8:
			OnEditDeviceAdddigitalout();
	    break;
	    case 9:
			OnEditDeviceAddgeneric();
	    break;
	}	
}

void CConfTreeView::CmdRouteMenu(CWnd* pWnd,CMenu* pPopupMenu)
{
	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	state.m_pParentMenu = pPopupMenu;
	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();

	for (state.m_nIndex = 0; 
	     state.m_nIndex < state.m_nIndexMax; 
	     state.m_nIndex++) 
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);

		// menu separator or invalid cmd - ignore it
		if (state.m_nID == 0) continue; 

		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to child menu if so
			CMenu* pSub=pPopupMenu->GetSubMenu(state.m_nIndex);
			if(pSub) CmdRouteMenu(pWnd,pSub);
		}
		else 
		{
			// normal menu item, Auto disable if command is 
			// _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(pWnd, FALSE);
		}
	}
}

void CConfTreeView::OnDrag (CXTPReportRecord* pRecord) 
{
    // get object from item
	CXTPReportRecordItem* pItem = pRecord->GetItem (0);
    CAdamoBase *pObjBase = (CAdamoBase *) pItem->GetItemData ();
	m_pRecord = pRecord;
    if(!(pObjBase->IsKindOf(RUNTIME_CLASS(CProject)) ||
            pObjBase->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))){        
        // Create the drag&drop source and data objects
        COleDropSourceEx *pDropSource = new COleDropSourceEx;
        COleDataSource *pDataSource = new COleDataSource;
        //create shared for clipboard
        CSharedFile file(GMEM_ZEROINIT|GMEM_DDESHARE|GMEM_MOVEABLE);
        CArchive ar(&file, CArchive::store);
        // Write the number of items and columns
    
        if(pObjBase)
            if(m_isHW){
                ar.WriteString(pObjBase->GetPathName()+"."+pObjBase->GetName()+ 
                                "#"+pObjBase->GetDesc ()+"#"+GetOffSetObj(pObjBase)+"#"+GetTypeObj(pObjBase)+
                                                        "-" +GetDeviceIndex(pObjBase));
			    //se l'item è di coloroe rosso no drag
				if (pItem->GetTextColor () == RGB (255,0,0))
					return;
            }else 
				ar.WriteString(pObjBase->GetPathName(true)+"."+pObjBase->GetName());
                                                   
        ar.Close();
        // put the file object into the data object
        pDataSource->CacheGlobalData(CF_TEXT, file.Detach());
        pDataSource->DoDragDrop(DROPEFFECT_MOVE|DROPEFFECT_COPY,NULL, pDropSource);
        //delete object
        delete pDropSource;
        delete pDataSource;
        /* diamo il focus all'editor e puliamo la selezione */
        if (!m_isHW)   {
            CLuaEditor *pLuaEditor=((CMainFrame *)AfxGetMainWnd ())->GetActiveLuaEditor ();
            if (pLuaEditor)   {
                pLuaEditor->CleanSelection ();
                pLuaEditor->SetFocus ();
            }
        }
    }
	else   {
		if (pObjBase->IsKindOf(RUNTIME_CLASS(CProject)))   {
			if (m_isHW)   {
				CString strName, strDesc, strModule;
				COleDropSourceEx *pDropSource = new COleDropSourceEx;
				COleDataSource *pDataSource = new COleDataSource;
				//create shared for clipboard
				CSharedFile file(GMEM_ZEROINIT|GMEM_DDESHARE|GMEM_MOVEABLE);
				CArchive ar(&file, CArchive::store);
				// Write the number of items and columns
				strName = ((CAdamoBase*)((CProject*)pObjBase)->GetMachineModule())->GetName();
				strDesc = ((CAdamoBase*)((CProject*)pObjBase)->GetMachineModule())->GetDesc();
				strModule.Format ("%d", ((CProject*)pObjBase)->GetModuleNumber ());
				ar.WriteString (strName+"@"+strDesc+"@"+strModule);
				ar.Close();
				// put the file object into the data object
				pDataSource->CacheGlobalData(CF_TEXT, file.Detach());
				pDataSource->DoDragDrop(DROPEFFECT_MOVE|DROPEFFECT_COPY,NULL, pDropSource);
				// delete object
				delete pDropSource;
				delete pDataSource;
			}
		}
	}
}

void CConfTreeView::SetIsConfig(BOOL isConfig)
{
    m_isConfig = isConfig;
    if (m_isConfig)   {
        CGridView *pView=((CMainFrame *)AfxGetMainWnd ())->GetDockDevProperty ()->GetView ();
        if (pView)
            pView->SetPswLevel (m_ePswLevel);
    }
}

void CConfTreeView::SetIsHW(BOOL isHW)
{
    m_isHW = isHW;
}

/*
** OnActivateMachine : la funzione viene chiamata quando si deve attivare una macchina.
*/
void CConfTreeView::OnActivateMachine() 
{
	/* prendiamo la macchina selezionata */
	CXTPReportRecord* pRecord = NULL;
	CXTPReportRow* pRow = NULL;
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows ();

	if (pSelectedRows->GetCount () > 0)   {
		pRow = pSelectedRows->GetAt (0);
		if (pRow)
			pRecord = pRow->GetRecord ();
	}
    if (pRecord)   {
        CProject* pPrj=GetProject (pRecord);
        if (pPrj)
            /* comunichiamo al workspace che si vuole cambiare la macchina attiva */
            GETWS()->ActivateMachine (pPrj->GetModuleNumber());
    }
}

/*
** GetTypeObj: tipo di dispositivo
*/
CString CConfTreeView::GetTypeObj(CAdamoBase *pObjBase)
{ 
    char szTypeID [33];
    _itoa( pObjBase->GetType(),szTypeID, 10 );
    CString strType(szTypeID);
    return strType;
}

/*
** GetOffSetObj: offset
*/
CString CConfTreeView::GetOffSetObj(CAdamoBase *pObjBase)
{
    CString strOffSet;
    char szOffset[33];
    if (pObjBase->IsKindOf(RUNTIME_CLASS(CAdamoDgtIn))) {
        _itoa(((CAdamoDgtIn*)pObjBase)->GetNumInput(),szOffset, 10 );
        CString strTemp(szOffset);
        strOffSet = strTemp;
    } else if(pObjBase->IsKindOf(RUNTIME_CLASS(CAdamoDgtOut))) {
        _itoa(((CAdamoDgtOut*)pObjBase)->GetNumOutput(),szOffset, 10 );
        CString strTemp(szOffset);
        strOffSet = strTemp;
    } else 
        strOffSet = "1";

    return strOffSet;
}

/*
** GetDeviceIndex: indice del dispositivo
*/
CString CConfTreeView::GetDeviceIndex(CAdamoBase *pObjBase)
{   
    char szIndex [33];
    _itoa( pObjBase->GetDeviceID(),szIndex, 10 );
    CString strIndex(szIndex);
    return strIndex;

}

/*
** SetItemBold : boldizza un item del tree
*/
void CConfTreeView::SetItemBold (CProject* pPrj)
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords ();

	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecord = pRecords->GetAt (i);
		CXTPReportRecordItem* pItem = pRecord->GetItem (0);
        CProject* pItemData=(CProject*)pItem->GetItemData (); 
		if (pItemData == pPrj)   {
			pItem->SetBold (TRUE);
			pItem->SetIconIndex (1);
			break;
		}
	}
}

/*
** ActivateMachine : chiamata dal Workspace per attivare una macchina
*/
void CConfTreeView::ActivateMachine (int nModule)
{
    ClearAllRoot ();
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)
        SetItemBold (pPrj);
}

/*
** ClearAllRoot : toglie il bold dalla lista delle macchine, le marca tutte graficamente non attive
*/
void CConfTreeView::ClearAllRoot ()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords ();

	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecord = pRecords->GetAt (i);
		pRecord->GetItem (0)->SetBold (FALSE);
	}
}

/*
** GetRootMachine: Nome macchina selezionata
*/
CString CConfTreeView::GetRootMachine (CXTPReportRecord* pRecord)
{
    CProject*         pProject;
    CString           strMachineName;
    CString           strPathDirMachine;
    CAdamoCfgMachine* objCfgMachine;

    if (pProject = GetProject(pRecord))   {
       strPathDirMachine = pProject->GetProjectDir ();
       objCfgMachine = (CAdamoCfgMachine*)pProject->GetMachine ();
       strMachineName = objCfgMachine->GetAdamoModule()->GetName ();
    }
    return strMachineName;
}

/*
** GetPathMachine: path macchina selezionata
*/
CString CConfTreeView::GetPathMachine(CXTPReportRecord* pRecord)
{
    CProject* pProject;
    CString   strPathDirMachine;

    if(pProject = GetProject(pRecord)){
       strPathDirMachine = pProject->GetProjectDir();
    }
    return strPathDirMachine;
}


/*
** SetColorItemDrag: quando è finto il drag setto il colore dell'item draggato 
*/
void CConfTreeView::SetColorItemDrag()
{	
	SetItemColor (m_pRecord->GetItem (0), RGB(255,0,0));
	m_pRecord = NULL;
	m_wndReport.RedrawControl ();
}

/*
** SearchItemDrop: 
*/
void CConfTreeView::SearchItemDrop(bool bLoadFromProject)
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords ();
	CAdamoDatiFile* pDatiFile;
	CString strPath;

	bLoadFromProject = true;
	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecord = pRecords->GetAt (i);
		if (bLoadFromProject)   {
			CProject* pPrj = GetProject (pRecord);
			strPath = pPrj->GetProjectDir ();
			pDatiFile = pPrj->GetAdamoDatiFile ();
			if(pRecord->HasChildren ())
				IteratorItem(pRecord, pDatiFile);
		}
		else   {
			strPath = GetPathMachine(pRecord);
			pDatiFile = new CAdamoDatiFile();
			if (pDatiFile->Open (strPath + "\\" + DEVICE_FILE, TRUE))   {
				pDatiFile->ReadData();
				if(pRecord->HasChildren ())
					IteratorItem(pRecord, pDatiFile);
				pDatiFile->CloseFile();
			}	
			delete pDatiFile;
		}
	}
}

/*
** IteratorItem: itero l'abelo nel drag
*/
void CConfTreeView::IteratorItem(CXTPReportRecord* pRecord, CAdamoDatiFile* pDatiFile)
{
	CXTPReportRecords* pRecords = pRecord->GetChilds ();
	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pCycleRecord = pRecords->GetAt (i);
		if (pCycleRecord->HasChildren ())
			IteratorItem (pCycleRecord, pDatiFile);
		else
			if (m_isHW)
				SearchDrop (pCycleRecord, pDatiFile);
			else
				SearchHWAddress (pCycleRecord, pDatiFile);
	}	
}

/*
** IteratorItem: itero l'abero nella cancellazione
*/
void CConfTreeView::IteratorItem(CXTPReportRecord* pRecord,const char* szDelete)
{
	CXTPReportRecords* pRecords = pRecord->GetChilds ();
	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecord = pRecords->GetAt (i);
		if (pRecord->HasChildren ())
			IteratorItem (pRecord, szDelete);
		else
			SearchItem(pRecord, szDelete);
	}
}

/*
** SearchDrop: setto l'item tree
*/
void CConfTreeView::SearchDrop(CXTPReportRecord* pRecord, CAdamoDatiFile* pDatiFile)
{
	CAdamoBase *pObjBase = (CAdamoBase* ) pRecord->GetItem (0)->GetItemData ();
	CString str;

    if(!(pObjBase->IsKindOf(RUNTIME_CLASS(CProject)) || pObjBase->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))) {        
		if(!SearchFileBin(pDatiFile,pObjBase->GetType(),pObjBase->GetDeviceID(), str))
			SetItemColor (pRecord->GetItem (0), RGB(255,0,0));
	}                             
}

/*
** SearchHWAddress : setto l'item tree
*/
void CConfTreeView::SearchHWAddress (CXTPReportRecord* pRecord, CAdamoDatiFile* pDatiFile)
{
	CAdamoBase *pObjBase = (CAdamoBase* ) pRecord->GetItem (0)->GetItemData ();
	CString str;
	if(!(pObjBase->IsKindOf(RUNTIME_CLASS(CProject)) || pObjBase->IsKindOf(RUNTIME_CLASS(CAdamoGroup))))  { 
		CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*) pRecord->GetItem (0);
		pItem = (CXTPReportRecordItemText*) pRecord->GetItem (2);
		SearchFileBin (pDatiFile,pObjBase->GetType(),pObjBase->GetDeviceID(), str);
		pItem->SetValue (str);
	}
}

/*
** SearchFileBin: cerco all'interno del file se szHWAddress non è vuoto
*/
BOOL CConfTreeView::SearchFileBin(CAdamoDatiFile* pDatiFile,enDispositivi enDev,int nIndex, CString& str)
{
    int i = 0;
	BOOL bFlag = FALSE;

	switch (enDev) {
		case enInputDigitale: 
			if(pDatiFile->GetAdamoParametri()->AdamoInputDig[nIndex].szHWAddress[0]=='\0')
				bFlag = TRUE;
			else
				str = pDatiFile->GetAdamoParametri()->AdamoInputDig[nIndex].szHWAddress;
			break;	
		case enOutputDigitale:                
			if(pDatiFile->GetAdamoParametri()->AdamoOutputDig[nIndex].szHWAddress[0]=='\0')
				bFlag = TRUE;
			else
				str = pDatiFile->GetAdamoParametri()->AdamoOutputDig[nIndex].szHWAddress;
			break;
		case enInputAnalogico:                 
			if(pDatiFile->GetAdamoParametri()->AdamoInputAnag[nIndex].szHWAddress[0]=='\0')
				bFlag = TRUE;     
			else
				str = pDatiFile->GetAdamoParametri()->AdamoInputAnag[nIndex].szHWAddress;
			break;
		case enOutputAnalogico:         
			if(pDatiFile->GetAdamoParametri()->AdamoOutputAnag[nIndex].szHWAddress[0]=='\0')
				bFlag = TRUE;
			else
				str = pDatiFile->GetAdamoParametri()->AdamoOutputAnag[nIndex].szHWAddress;
			break;
		case enAsse:                  
			if(pDatiFile->GetAdamoParametri()->AdamoAsse[nIndex].szHWAddress[0]=='\0')
				bFlag = TRUE;       
			else
				str = pDatiFile->GetAdamoParametri()->AdamoAsse[nIndex].szHWAddress;
			break;
		case enGenericCan:                
			if(pDatiFile->GetAdamoParametri()->AdamoCanDevice[nIndex].szHWAddress[0]=='\0')
				bFlag = TRUE;          
			else
				str = pDatiFile->GetAdamoParametri()->AdamoCanDevice[nIndex].szHWAddress;
			break;
        default :
            bFlag = TRUE;          
            break;
    }
	return bFlag;
}

/*
** DelColorItemDrag: ripristino l'item dopo la cancellazione drag
*/
void CConfTreeView::DelColorItemDrag(const char *szDelete)
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords ();
	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecord = pRecords->GetAt (i);
		if (pRecord->HasChildren ())
			IteratorItem (pRecord, szDelete);
	}
	m_wndReport.RedrawControl ();
}

/*
** SearchItem: qunado trovo l'item  setto il colore nero
*/
void CConfTreeView::SearchItem(CXTPReportRecord* pRecord, const char *szDelete)
{
	CString strDelete = szDelete;
	CAdamoBase *pObjBase = (CAdamoBase *) pRecord->GetItem (0)->GetItemData ();
    if(!(pObjBase->IsKindOf(RUNTIME_CLASS(CProject)) ||
            pObjBase->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))) {        
		if(pObjBase){
			if(pObjBase->GetPathName()+"."+pObjBase->GetName() == szDelete)   {
				CXTPReportRecordItem* pItem = pRecord->GetItem (0);
				SetItemColor (pItem, RGB(0,0,0));
			}
		}
	}
}

/*
** GetRootItem :
*/
CXTPReportRecord* CConfTreeView::GetRootItem ()
{
	CXTPReportRecord* pRecord, *pParent;
	CXTPReportRow* pRow;
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows();

	if (pSelectedRows->GetCount () > 0)   {
		pRow = pSelectedRows->GetAt (0);
		pRecord = pRow->GetRecord ();
		pParent = pRecord->GetParentRecord ();
		while (pParent)   {
			pRecord = pParent;
			pParent = pParent->GetParentRecord ();
		}
	}
    return pRecord;
}

/*
** Activate :
*/
void CConfTreeView::Activate()
{
	CFrameWnd* pFrame = GetParentFrame();

	if (pFrame != NULL)
		pFrame->ActivateFrame();
	else
		TRACE0("Error: Can not find a frame for document to activate.\n");

	CFrameWnd* pAppFrame;
	if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
	{
		ASSERT_KINDOF(CFrameWnd, pAppFrame);
		pAppFrame->ActivateFrame();
	}
    /* comunichiamo al template che  siamo stati attivati */
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    pTmpl->SetActiveView (this);
    pTmpl->SetActiveFrame (pFrame);
}

/*
** AggiungiDispositivoAlTree :
*/
CXTPReportRecord* CConfTreeView::AggiungiDispositivoAlTree (CObject *pObj, CXTPReportRecord* pRecord)
{
    if (m_isHW)   {
        if (pObj->IsKindOf(RUNTIME_CLASS(CAdamoTimer)))
            return NULL;
        else 
        if (pObj->IsKindOf(RUNTIME_CLASS(CAdamoCounter)))
            return NULL;
        else 
        if (pObj->IsKindOf(RUNTIME_CLASS(CAdamoGenericCan)))
            return NULL;
    }
    return AddItemRecord (pObj, pRecord);
}

/*
** TogliIGruppiSenzaFigli :
*/
void CConfTreeView::TogliIGruppiSenzaFigli ()
{
	CXTPReportRecords* pRecords = m_wndReport.GetRecords ();
	
	for (int i = 0; i<pRecords->GetCount (); i++)   {
		CXTPReportRecord* pRecord = pRecords->GetAt (i);
		if (pRecord->HasChildren ())
			TogliIGruppiSenzaFigli (pRecord);
	}
}

/*
** TogliIGruppiSenzaFigli :
*/
void CConfTreeView::TogliIGruppiSenzaFigli (CXTPReportRecord* pRecord)
{
	bool bHasChildren = false;

	for (int i = 0; i<pRecord->GetChilds ()->GetCount (); i++)   {
		CXTPReportRecords* pChilds = pRecord->GetChilds ();
		CXTPReportRecord* pChild = pChilds->GetAt (i);
		if (pChild->HasChildren ())   {
			TogliIGruppiSenzaFigli (pChild);
			bHasChildren = true;
		}
		CObject* pObj = (CObject* ) pChild->GetItem (0)->GetItemData ();
		if (!bHasChildren && pObj->IsKindOf(RUNTIME_CLASS(CAdamoGroup)))   {
			pRecord->GetChilds ()->RemoveAt (pChild->GetIndex ()); i--;
		}
	}
}

void CConfTreeView::OnEditFind() 
{
	// TODO: Add your command handler code here
    if (((CMainFrame*)AfxGetMainWnd ())->GetMode ()==CMainFrame::modeConfig)
        FindInLogicTree ();
}

void CConfTreeView::OnUpdateEditFind(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    pCmdUI->Enable ();
}

int CConfTreeView::FindInLogicTree() 
{
    if (!m_pConfFind)   {
        m_pConfFind=new CConfFind (this);
        m_pConfFind->SetConfTreeView (this);
        m_pConfFind->Create (IDD_DLGCONFFIND, this);
    }
    return 0;
}

CAdamoBase *CConfTreeView::GetSelectedDevice () 
{
	CXTPReportRecord* pRecord = NULL;
	CXTPReportRow* pRow = NULL;
	CAdamoBase *pSelect = NULL;
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows ();

	if (pSelectedRows->GetCount () > 0)   {
		pRow = pSelectedRows->GetAt (0);
		if (pRow)   {
			pRecord = pRow->GetRecord ();
			CXTPReportRecordItem* pItem = pRecord->GetItem (0);
			if (pItem)
				pSelect = (CAdamoBase *) pItem->GetItemData ();
		}
	}
	return pSelect;
}

CProject *CConfTreeView::GetSelectedProject ()
{
	return m_objCurProject;
}

//HTREEITEM CConfTreeView::FindTreeNode (HTREEITEM hItem, HTREEITEM hItemBS, bool* bs)
//{
    //HTREEITEM hItemBack=NULL, hChildItem;
    //bool bFound=false;

    //if (hItem)   {
    //    do   {
    //        if (*bs)
    //            bFound=FindText (hItem);
    //        if (bFound)   {
    //            hItemBack=hItem; break;
    //        }
    //        else
    //            if (hItem==hItemBS)
    //                *bs=true;
    //        if (GetTreeCtrl().ItemHasChildren (hItem))   {
    //            hChildItem = GetTreeCtrl().GetChildItem(hItem);
    //            if (hItemBack=FindTreeNode (hChildItem, hItemBS, bs))
    //                return hItemBack;
    //        }
    //        hItem=GetTreeCtrl().GetNextSiblingItem (hItem);
    //    }   while (hItem);
    //}
    //return hItemBack;
//}

/*
** FindText :
*/
//bool CConfTreeView::FindText (HTREEITEM hItem)
//{
//    CString str=GetTreeCtrl().GetItemText (hItem);
//    CString strToFind;
//    int n;
//    bool nB=false;
//    
//    m_pConfFind->m_ctrlSearchText.GetWindowText (strToFind);
//    if (!m_pConfFind->m_bMatchCase)   {
//        str.MakeUpper();
//        strToFind.MakeUpper ();
//    }
//    if ((n=str.Find (strToFind, 0))!=-1)   {
//        if (m_pConfFind->m_bMatchWords)   {
//            char* p=str.GetBuffer (str.GetLength()+1), ch;
//            ch=p[n+strToFind.GetLength()];
//            if (ch==' '||ch=='\t'||ch=='\0')
//                nB=true;
//        }
//        else
//            nB=true;
//    }
//    return nB;
//}

/*
** OnCloseDialogFind :
*/
LRESULT CConfTreeView::OnCloseDialogFind (WPARAM wParam, LPARAM lParam)
{
    if (m_pConfFind)   {
        m_pConfFind->DestroyWindow ();
        _delete (m_pConfFind);
    }
    return 0;
}

/*
** CheckPathLength :
*/
BOOL CConfTreeView::CheckPathLength ()
{
    int i=0, nFind;
    BOOL b=FALSE;

    CString strPath=((CAdamoBase*) m_objCurNode)->GetPathName ()+"."+((CAdamoBase*) m_objCurNode)->GetName();
    nFind=strPath.Find ('.', 0);
    while (nFind!=-1)   {
        i++;
        nFind=strPath.Find ('.', nFind+1);
    }
    if (i<=5)
        b=TRUE;
    return b;
}

/*
** OnUpdateEditDeviceAddaxis :
*/
void CConfTreeView::OnUpdateEditDeviceAddaxis (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditAddgroup :
*/
void CConfTreeView::OnUpdateEditAddgroup (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditDeviceAddanaloginput :
*/
void CConfTreeView::OnUpdateEditDeviceAddanaloginput (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditDeviceAddanalogout :
*/
void CConfTreeView::OnUpdateEditDeviceAddanalogout (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditDeviceAddcounter :
*/
void CConfTreeView::OnUpdateEditDeviceAddcounter (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditDeviceAdddigitalin :
*/
void CConfTreeView::OnUpdateEditDeviceAdddigitalin (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditDeviceAdddigitalout :
*/
void CConfTreeView::OnUpdateEditDeviceAdddigitalout (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditDeviceAddtimer :
*/
void CConfTreeView::OnUpdateEditDeviceAddtimer (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditDeviceAddgeneric :
*/
void CConfTreeView::OnUpdateEditDeviceAddgeneric (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditDeviceAddmachine :
*/
void CConfTreeView::OnUpdateEditDeviceAddmachine (CCmdUI* pCmdUI) 
{
	bool bEnable=((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeConfig||
                   ((CMainFrame *)AfxGetMainWnd ())->GetMode()==CMainFrame::modeLogic;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnUpdateEditDeviceAddmachine :
*/
void CConfTreeView::OnUpdateActivateMachine (CCmdUI* pCmdUI) 
{
    bool bEnable = true;;
    if (m_ePswLevel>ePswBuilder)
        bEnable=false;
    pCmdUI->Enable (bEnable);
}

/*
** OnChangePsw :
*/
void CConfTreeView::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
    m_ePswLevel=newPsw;
    if (m_isConfig)   {
        CGridView *pView=((CMainFrame *)AfxGetMainWnd ())->GetDockDevProperty ()->GetView ();
        if (pView)
            pView->OnChangePsw (oldPsw, newPsw);
    }
}

/*
** SetItemColor :
*/
void CConfTreeView::SetItemColor (CXTPReportRecordItem* pItem, COLORREF rgb)
{
	pItem->SetTextColor (rgb);
}

/*
** OnReportSelChanged :
*/
void CConfTreeView::OnReportSelChanged (NMHDR* pNotifyStruct, LRESULT* pResult	)
{
	CXTPReportRecord* pRecord = NULL;
	CXTPReportRow* pRow = NULL;
	CObject *pSelect = NULL;
	CXTPReportSelectedRows* pSelectedRows = m_wndReport.GetSelectedRows ();

	if (pSelectedRows->GetCount () > 0)   {
		pRow = pSelectedRows->GetAt (0);
		if (pRow)   {
			pRecord = pRow->GetRecord ();
			CXTPReportRecordItem* pItem = pRecord->GetItem (0);
			if (pItem)
				pSelect = (CObject *) pItem->GetItemData ();
		}
	}
	if (pSelect)
		if(m_isConfig)
			SendObj(pRecord, pSelect);
		else
			StoreObj (pRecord, pSelect);
	/*
	XTP_NM_REPORTRECORDITEM* pItemNotify = reinterpret_cast<XTP_NM_REPORTRECORDITEM*>(pNotifyStruct);
	CXTPReportRecord* pRecord = pItemNotify->pRow->GetRecord ();
    CObject *pObj = (CObject*) pRecord->GetItem (0)->GetItemData ();
    if(m_isConfig)
        SendObj(pRecord, pObj);
	else
		StoreObj (pRecord, pObj);
	*/
	*pResult = 0;
}

/*
** OnSize :
*/
void CConfTreeView::OnSize(UINT nType, int cx, int cy) 
{
	CPaneView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(m_wndReport.m_hWnd) )
		m_wndReport.SetWindowPos(NULL, 0, 0, cx, cy, 0);
}

/*
** UpdateImages :
*/
void CConfTreeView::UpdateImages ()
{
	m_wndReport.GetImageManager()->RemoveAll();
	VERIFY(m_wndReport.GetImageManager()->SetIcons (IDB_CONF_TREE, 0, 0, CSize (16, 16)));
}

/*
** RefreshMetrics :
*/
void CConfTreeView::RefreshMetrics(AppTheme nTheme)
{
	switch (nTheme)
	{
		case themeVS2010: m_wndReport.SetTheme(xtpReportThemeVisualStudio2012Blue); break;
		case themeVS2012Light: m_wndReport.SetTheme(xtpReportThemeVisualStudio2012Light); break;
		case themeVS2012Dark: m_wndReport.SetTheme(xtpReportThemeVisualStudio2012Dark); break;
		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue: m_wndReport.SetTheme(xtpReportThemeVisualStudio2015); break;
	}	
	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureNone;
	m_wndReport.GetPaintManager()->SetGridStyle (FALSE, xtpReportLineStyleNone);
	m_wndReport.GetPaintManager()->SetGridStyle (TRUE, xtpReportLineStyleNone);
}

/*
** SelectRecord :
*/
void CConfTreeView::SelectRecord (CXTPReportRecord* pRecord)
{
	CObject* pObj = (CObject* ) pRecord->GetItem (0)->GetItemData ();
	CXTPReportRows* pRows = m_wndReport.GetRows();
	for (int i = 0; i<pRows->GetCount (); i++)   {
		CXTPReportRow* pRow = pRows->GetAt (i);
		CXTPReportRecord* pRowRecord = pRow->GetRecord ();
		CObject* pRowObj = (CObject* ) pRowRecord->GetItem (0)->GetItemData ();
		if (pRowObj == pObj)   {
			m_wndReport.SetFocusedRow (pRow);
		}
	}
}

/*
** SetNoHeader :
*/
void CConfTreeView::SetNoHeader (BOOL bShow)
{
	//return;
	/* nascondiamo l'header */
	m_wndReport.ShowHeader (FALSE);
	/* togliamo l'autosize delle colonne */
	CXTPReportColumns* columns = m_wndReport.GetColumns ();
	/* settiamo una dimansione della prima colonna pari a 200 DPI */
	CXTPReportColumn* column = columns->GetAt (0);
	column->SetAutoSize (FALSE);
	column->SetWidth (400);
	column = columns->GetAt (1);
	column->SetAutoSize (FALSE);
	column->SetWidth (400);
	column->SetAutoSize (FALSE);
	column = columns->GetAt (2);
	column->SetWidth (400);
}

BEGIN_MESSAGE_MAP(CAdamoCXTPReportControl, CXTPReportControl)
	ON_WM_LBUTTONDOWN ()
	ON_WM_LBUTTONUP ()
	ON_WM_MOUSEMOVE ()
END_MESSAGE_MAP()

void CAdamoCXTPReportControl::OnLButtonDown (UINT nFlags, CPoint point)
{
    //if (((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ()<=ePswBuilder)  {
	__super::OnLButtonDown (nFlags, point);
	m_ButtonDown = true;
	m_point = point;
	//}
}

void CAdamoCXTPReportControl::OnLButtonUp (UINT nFlags, CPoint point)
{
	__super::OnLButtonUp (nFlags, point);
	m_ButtonDown = false;
}

void CAdamoCXTPReportControl::OnMouseMove (UINT nFlags, CPoint point)
{
	if (m_Dragging == false)   {
		__super::OnMouseMove (nFlags, point);
		if (m_ButtonDown)   {
			CPoint p = point - m_point;
			if (abs (p.x) > 4 || abs (p.y) > 4)   {
				CXTPReportSelectedRows* pSelectedRows = GetSelectedRows ();
				if (pSelectedRows->GetCount () > 0)   {
					CXTPReportRow* pRow = pSelectedRows->GetAt (0);
					if (pRow)   {
						CXTPReportRecord* pRecord = pRow->GetRecord ();
						m_Dragging = true;
						//SetCapture ();
						((CConfTreeView*)GetParent())->OnDrag (pRecord);
						m_Dragging = false;
						m_ButtonDown = false;
					}
				}
			}
		}
	}
	else
		__super::OnMouseMove (nFlags, point);
}

