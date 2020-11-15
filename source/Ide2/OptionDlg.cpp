// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "OptionDlg.h"
#include "MainFrame.h"
#include "AdamoTraduzione.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

extern const char* achSkinStyles[OPTIONS_SKIN_STYLES];

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent), m_opt (NULL)
{
	//{{AFX_DATA_INIT(COptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSelected = NULL;
}


void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_LIST_OPTIONS, OnGetdispinfoListOptions)
	ON_NOTIFY(TVN_SELCHANGED, IDC_LIST_OPTIONS, OnSelchangedListOptions)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTPWM_PROPERTYGRID_NOTIFY, OnGridNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers
BOOL COptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_opt = ((CIdeApp *)AfxGetApp ())->GetAdamoOptions ();
	m_wndProperties.SubclassDlgItem(IDC_LIST_OPTIONS, this);

	GetDlgItem(IDC_WORKSPACE)->GetWindowRect(m_rcWorkspace);
	ScreenToClient(m_rcWorkspace);

	HTREEITEM hItemThira = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_14)), I_IMAGECALLBACK , I_IMAGECALLBACK );
	CreateGridThira(hItemThira);

	HTREEITEM hItemCommunicationServer = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_11)), I_IMAGECALLBACK , I_IMAGECALLBACK );
	CreateGridCommunicationServer(hItemCommunicationServer);

	HTREEITEM hItemConnection = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_9)), I_IMAGECALLBACK , I_IMAGECALLBACK );
	CreateGridConnection(hItemConnection);

	HTREEITEM hItemInternational = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_10)), I_IMAGECALLBACK , I_IMAGECALLBACK );
	CreateGridInternational(hItemInternational);

	HTREEITEM hItemSkin = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_17)), I_IMAGECALLBACK , I_IMAGECALLBACK );
	CreateGridSkin (hItemSkin);

	m_pSelected = &m_wndGridThira;
    m_wndProperties.Select (hItemThira, TVGN_CARET);
	m_wndProperties.Expand (hItemThira, TVE_EXPAND);
    InitCharCheck ();
    return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** OnGetdispinfoListOptions :
*/
void COptionDlg::OnGetdispinfoListOptions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;	

	if (pTVDispInfo->item.mask & (TVIF_SELECTEDIMAGE | TVIF_IMAGE))
	{
		if (m_wndProperties.GetChildItem(pTVDispInfo->item.hItem) != NULL)
		{
			UINT nState = m_wndProperties.GetItemState(pTVDispInfo->item.hItem, TVIF_STATE);
			pTVDispInfo->item.iSelectedImage = pTVDispInfo->item.iImage = nState & TVIS_EXPANDED? 3: 2;		 		
		} 
		else
		{
			ASSERT(m_pSelected);
			pTVDispInfo->item.iSelectedImage = pTVDispInfo->item.iImage = 
				(m_pSelected->m_hItem == pTVDispInfo->item.hItem? 0: 1);
		}
	}
	
	*pResult = 0;
}

/*
** OnSelchangedListOptions :
*/
void COptionDlg::OnSelchangedListOptions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if (pNMTreeView->itemNew.lParam == 0)
		return;
	
	if (pNMTreeView->itemNew.hItem)
	{
	
		CWnd* pPage = (CWnd*)pNMTreeView->itemNew.lParam;

		if (pPage != m_pSelected)
		{
			ASSERT(pPage);
			ASSERT(pPage->IsKindOf(RUNTIME_CLASS(CXTPPropertyGrid)));
		
			pPage->ShowWindow(SW_SHOW);	  
			pPage->EnableWindow();
			pPage->ModifyStyle(0, WS_TABSTOP);

			if (m_pSelected)
			{
				m_pSelected->ShowWindow(SW_HIDE);
			}
			m_pSelected = (CPropertyGridEx*)pPage;

			m_wndProperties.Invalidate(FALSE);
		} 

		HTREEITEM hItemParent = m_wndProperties.GetParentItem(pNMTreeView->itemNew.hItem);
		if (m_pSelected != NULL && hItemParent)
		{
			
			m_wndProperties.SetItemData(hItemParent, (DWORD)m_pSelected);
		}		

	}
	
	*pResult = 0;
}

/*
** OnGridNotify :
*/
LRESULT COptionDlg::OnGridNotify(WPARAM wParam, LPARAM lParam)
{
    CString str;
    eTypeConnection eTC;
    int nPort;
	bool b;
    char *p;

	if (wParam == XTP_PGN_INPLACEBUTTONDOWN)   {
		CXTPPropertyGridInplaceButton* pButton = (CXTPPropertyGridInplaceButton*)lParam;
        if ((pButton->GetItem()->GetID() == ID_OPT_LANGUAGE) && pButton->GetID() == XTP_ID_PROPERTYGRID_EXPANDBUTTON)   {
            CTraduzioni tr;
            if (AfxMessageBox (LOADSTRING (IDS_ADAMOOPT_3), MB_ICONQUESTION|MB_YESNO)==IDYES)   {
                if (((CMainFrame *)AfxGetMainWnd ())->GetTemplate ()!=enNoTemplate)
                    ((CMainFrame *)AfxGetMainWnd ())->OnCloseMode ();
                if (tr.CambioLingua ())   {
                    pButton->GetItem()->SetValue (((CMainFrame *)AfxGetMainWnd ())->GetCurrentLanguage ());
                    ((CMainFrame *)AfxGetMainWnd ())->SwitchModeToolbar ();
                }
            }
            else
                EndDialog (0);
        }
    }
	else
        if (wParam == XTP_PGN_EDIT_CHANGED)  {
            CXTPPropertyGridInplaceEdit* pEdit = DYNAMIC_DOWNCAST(CXTPPropertyGridInplaceEdit, (CWnd*)lParam);
            CXTPPropertyGridItem *pItem = pEdit->GetItem ();
            CheckInput (pItem->GetID(), pEdit);
        }
        else
	    if (wParam==XTP_PGN_ITEMVALUE_CHANGED)   {
            CXTPPropertyGridItem *pItem = (CXTPPropertyGridItem*)lParam;
		    if (pItem)   {
                /* assegnamo prima gli elementi di base */
                switch (pItem->GetID())   {
                    case ID_OPT_CONNECTION :
            			str=pItem->GetValue ();
                        if (str == LOADSTRING (IDS_ADAMOOPT_5))
                            eTC = enLocalConnection;
                        else
                        if (str == LOADSTRING (IDS_ADAMOOPT_6))
                            eTC = enTCPConnection;
                        m_opt->SetTypeConnection (eTC);
                        pItem = m_pSelected->FindItem(ID_OPT_TCPIP_ADDRESS);
                        pItem->SetReadOnly(eTC==enLocalConnection);
                        pItem = m_pSelected->FindItem(ID_OPT_TCP_IP_PORT);
                        pItem->SetReadOnly(eTC==enLocalConnection);
                        break;
                    case ID_OPT_TCPIP_ADDRESS :
            			str=pItem->GetValue ();
                        m_opt->SetTcpIpAddress (str);
                        break;
                    case ID_OPT_TCP_IP_PORT :
            			str=pItem->GetValue ();
                        nPort = strtol (str, &p, 10);
                        m_opt->SetTcpIpPort (nPort);
                        break;
					case ID_OPT_KILLMACHINE :
						b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
						m_opt->SetKillMachine (b);
						break;
					case ID_OPT_LOADVIRTUAL :
						b=((CXTPPropertyGridItemBool*)pItem)->GetBool () != 0;
						m_opt->SetLoadVirtualMachine (b);
						break;
					case ID_OPT_GRIDSTYLE :
            			str=pItem->GetValue ();
						m_opt->SetSkin (str);
						((CMainFrame*) AfxGetMainWnd ())->SetSkin (str);
						break;
                }
            }
        }
    return 0;
}

/*
** CreateGridThira :
*/
void COptionDlg::CreateGridThira (HTREEITEM hItemCategory)
{
    CString str;
	m_wndGridThira.Create(m_rcWorkspace, this, 0);

	CXTPPropertyGridItem* pCategoryApplication = m_wndGridThira.AddCategory(LOADSTRING (IDS_ADAMOOPT_16));
	AddGridItemCheckBox (ID_OPT_LOADVIRTUAL, LOADSTRING (IDS_ADAMOOPT_15), m_opt->GetLoadVirtualMachine (), pCategoryApplication);
	HTREEITEM hItem = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_16)), I_IMAGECALLBACK, I_IMAGECALLBACK, hItemCategory);
	m_wndProperties.SetItemData(hItem, (DWORD)&m_wndGridThira);
	m_wndProperties.SetItemData(hItemCategory, (DWORD)&m_wndGridThira);
	m_wndGridThira.m_hItem = hItem;
	pCategoryApplication->Expand ();
}

/*
** CreateGridConnection :
*/
void COptionDlg::CreateGridCommunicationServer (HTREEITEM hItemCategory)
{
    CString str;
	m_wndGridCommunicationServer.Create(m_rcWorkspace, this, 0);

	CXTPPropertyGridItem* pCategoryApplication = m_wndGridCommunicationServer.AddCategory(LOADSTRING (IDS_ADAMOOPT_12));
	AddGridItemCheckBox (ID_OPT_KILLMACHINE, LOADSTRING (IDS_ADAMOOPT_13), m_opt->GetKillMachine (), pCategoryApplication);
	HTREEITEM hItem = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_11)), I_IMAGECALLBACK, I_IMAGECALLBACK, hItemCategory);
	m_wndProperties.SetItemData(hItem, (DWORD)&m_wndGridCommunicationServer);
	m_wndProperties.SetItemData(hItemCategory, (DWORD)&m_wndGridCommunicationServer);
	m_wndGridCommunicationServer.m_hItem = hItem;
	m_wndGridCommunicationServer.ShowWindow (SW_HIDE);
	pCategoryApplication->Expand ();
}

/*
** CreateGridConnection :
*/
void COptionDlg::CreateGridConnection (HTREEITEM hItemCategory)
{
    CString str;
	m_wndGridConnection.Create(m_rcWorkspace, this, 0);

	CXTPPropertyGridItem* pCategoryApplication = m_wndGridConnection.AddCategory(LOADSTRING (IDS_ADAMOOPT_4));
    eTypeConnection eTC = (eTypeConnection) m_opt->GetTypeConnection ();
    /* combo tipo di connessione */
    AddGridComboConnectionType (pCategoryApplication, eTC); 
    /* edit indirizzo ip */
    CXTPPropertyGridItem *pItem = AddGridItem (ID_OPT_TCPIP_ADDRESS, LOADSTRING (IDS_ADAMOOPT_7), m_opt->GetTcpIpAddress (), pCategoryApplication);
    pItem->SetReadOnly (eTC == enLocalConnection);
    /* edit port number */
    str.Format ("%d", m_opt->GetTcpIpPort ());
    pItem = AddGridItem (ID_OPT_TCP_IP_PORT, LOADSTRING (IDS_ADAMOOPT_8), str, pCategoryApplication);
    pItem->SetReadOnly (eTC == enLocalConnection);
	HTREEITEM hItem = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_9)), I_IMAGECALLBACK, I_IMAGECALLBACK, hItemCategory);
	m_wndProperties.SetItemData(hItem, (DWORD)&m_wndGridConnection);
	m_wndProperties.SetItemData(hItemCategory, (DWORD)&m_wndGridConnection);
	m_wndGridConnection.m_hItem = hItem;
	m_wndGridConnection.ShowWindow (SW_HIDE);
	pCategoryApplication->Expand ();
}

/*
** CreateGridInternational :
*/
void COptionDlg::CreateGridInternational(HTREEITEM hItemCategory)
{
	m_wndGridInternational.Create(m_rcWorkspace, this, 0);

	CXTPPropertyGridItem* pCategoryApplication = m_wndGridInternational.AddCategory(LOADSTRING (IDS_ADAMOOPT_1));
    AddGridItemButtonText (ID_OPT_LANGUAGE, LOADSTRING (IDS_ADAMOOPT_2), pCategoryApplication);

    HTREEITEM hItem = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_10)), I_IMAGECALLBACK, I_IMAGECALLBACK, hItemCategory);
	m_wndProperties.SetItemData(hItem, (DWORD)&m_wndGridInternational);
	m_wndProperties.SetItemData(hItemCategory, (DWORD)&m_wndGridInternational);
	m_wndGridInternational.m_hItem = hItem;
	m_wndGridInternational.ShowWindow(SW_HIDE);
	pCategoryApplication->Expand();
}

/*
** AddGridItemButtonText :
*/
void COptionDlg::AddGridItemButtonText (int iId, CString strCaption, CXTPPropertyGridItem *pItemNode)
{
    CXTPPropertyGridItem *pItem = new CXTPPropertyGridItem(iId, ((CMainFrame *)AfxGetMainWnd ())->GetCurrentLanguage ());
    pItem->SetFlags(xtpGridItemHasExpandButton);
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
}

/*
** AddGridComboConnectionType :
*/
CXTPPropertyGridItem *COptionDlg::AddGridComboConnectionType (CXTPPropertyGridItem* pGridItem, eTypeConnection eConnType)
{
    CXTPPropertyGridItem* pItemCombo=NULL;
    CStringList sl;
    CString str;

    str = eConnType == enLocalConnection ? LOADSTRING (IDS_ADAMOOPT_5) : LOADSTRING (IDS_ADAMOOPT_6);
    pItemCombo = pGridItem->AddChildItem (new CXTPPropertyGridItem (ID_OPT_CONNECTION, str));
    pItemCombo->SetCaption (LOADSTRING (IDS_ADAMOOPT_4));
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
    pList->AddConstraint(LOADSTRING (IDS_ADAMOOPT_5));
    pList->AddConstraint(LOADSTRING (IDS_ADAMOOPT_6));
    pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}

/*
** AddGridItem :
*/
CXTPPropertyGridItem *COptionDlg::AddGridItem (int iId,CString strCaption, CString strValue, CXTPPropertyGridItem *pItemNode)
{
    CXTPPropertyGridItem *pItem = new CXTPPropertyGridItem(iId,strValue);
    (pItemNode->AddChildItem(pItem))->SetCaption(strCaption);
    return pItem;
}

/*
** CheckInput :
*/
void COptionDlg::CheckInput (int nID, CXTPPropertyGridInplaceEdit* pEdit)
{
    switch (nID)   {
        case ID_OPT_TCPIP_ADDRESS :
            CheckIpAddress (pEdit, m_objCheckAddressIP, 32);
            break;
        case ID_OPT_TCP_IP_PORT :
            CheckChars (pEdit, m_objCheckNumbers, 4);
            break;
    }    
}

/*
** InitCharCheck :
*/
void COptionDlg::InitCharCheck ()
{
    m_objCheckAddressIP.AddRange ('0', '9');
	m_objCheckAddressIP.AddChar( '.' );
	m_objCheckNumbers.AddRange( '0', '9' );
}

/*
** CheckChars :
*/
void COptionDlg::CheckChars (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength)
{
    ((CMainFrame *) AfxGetMainWnd ())->CheckChars (pEdit, objCheck, nMaxLength);
}

/*
** CheckIpAddress :
*/
void COptionDlg::CheckIpAddress (CXTPPropertyGridInplaceEdit* pEdit,CCharCheck &objCheck,int nMaxLength)
{
    ((CMainFrame *) AfxGetMainWnd ())->CheckIpAddress (pEdit, objCheck, nMaxLength);
}

/*
** OnOK :
*/
void COptionDlg::OnOK()
{
    // TODO: aggiungere qui il codice specifico e/o chiamare la classe base
    m_opt->SaveOptions ();
    CDialog::OnOK();
}

/*
** AddGridItemCheckBox :
*/
CXTPPropertyGridItem *COptionDlg::AddGridItemCheckBox (int iId, CString strCaption, BOOL bValue, CXTPPropertyGridItem *pItemNode)
{
	CXTPPropertyGridItemBool* pItemBool = (CXTPPropertyGridItemBool*)pItemNode->AddChildItem(
		new CXTPPropertyGridItemBool(strCaption, FALSE));
	pItemBool->SetBool(bValue);
	pItemBool->SetID(iId);
	pItemBool->SetCheckBoxStyle();
    return pItemBool;
}

/*
** CreateGridSkin :
*/
void COptionDlg::CreateGridSkin (HTREEITEM hItemSkin)
{
    CString str;
	m_wndGridSkin.Create (m_rcWorkspace, this, 0);
	CXTPPropertyGridItem* pSkinApplication = m_wndGridSkin.AddCategory(LOADSTRING (IDS_ADAMOOPT_18));
    CString strSkin = m_opt->GetSkin ();
    /* combo tipo di connessione */
    AddGridComboSkin (pSkinApplication, strSkin);
	HTREEITEM hItem = m_wndProperties.InsertItem(_T(LOADSTRING (IDS_ADAMOOPT_19)), I_IMAGECALLBACK, I_IMAGECALLBACK, hItemSkin);
	m_wndProperties.SetItemData(hItem, (DWORD)&m_wndGridSkin);
	m_wndProperties.SetItemData(hItemSkin, (DWORD)&m_wndGridSkin);
	m_wndGridSkin.m_hItem = hItem;
	m_wndGridConnection.ShowWindow (SW_HIDE);
	pSkinApplication->Expand ();
}

/*
** AddGridComboConnectionType :
*/
CXTPPropertyGridItem *COptionDlg::AddGridComboSkin (CXTPPropertyGridItem* pGridItem, CString strSkin)
{
    CXTPPropertyGridItem* pItemCombo = pGridItem->AddChildItem (new CXTPPropertyGridItem (ID_OPT_GRIDSTYLE, strSkin));
    pItemCombo->SetCaption (LOADSTRING (IDS_ADAMOOPT_20));
    CXTPPropertyGridItemConstraints* pList = pItemCombo->GetConstraints();
	for (int i = 0; i<sizeof (achSkinStyles) / sizeof (char*); i++)
		    pList->AddConstraint(achSkinStyles[i]);
    pItemCombo->SetFlags(xtpGridItemHasComboButton|xtpGridItemHasEdit);
    pItemCombo->SetConstraintEdit(TRUE);
    return pItemCombo;
}
