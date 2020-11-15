// DlgConnectToTcp.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "DlgConnectToTcp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoAboutBox dialog


CDlgConnectToTcp::CDlgConnectToTcp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConnectToTcp::IDD, pParent), m_pTreeCtrl (NULL), m_pIPAddressCtrl (NULL), m_opt (NULL)
{
	//{{AFX_DATA_INIT(CAdamoAboutBox)
	//}}AFX_DATA_INIT
}


void CDlgConnectToTcp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoAboutBox)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConnectToTcp, CDialog)
	//{{AFX_MSG_MAP(CAdamoAboutBox)
    ON_NOTIFY(TVN_SELCHANGED, IDC_CT_NETWORKSERVERS, OnSelchanged)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_CT_CONNECT_TCPIP, &CDlgConnectToTcp::OnBnClickedCtConnect)
	ON_BN_CLICKED(IDC_CT_CONNECT_LOCAL, &CDlgConnectToTcp::OnBnClickedCtConnectLocal)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoAboutBox message handlers

BOOL CDlgConnectToTcp::OnInitDialog() 
{
	CDialog::OnInitDialog();
    BeginWaitCursor ();
    m_opt = ((CIdeApp *) AfxGetApp ())->GetAdamoOptions ();
    m_pTreeCtrl = new CTreeCtrl;
    m_pTreeCtrl->Attach (GetDlgItem (IDC_CT_NETWORKSERVERS)->GetSafeHwnd ());
    m_pIPAddressCtrl = new CIPAddressCtrl;
    m_pIPAddressCtrl->Attach (GetDlgItem (IDC_CT_IPADDRESS)->GetSafeHwnd ());
    m_editPort.SubclassEdit(IDC_CT_PORTNUMBER, this, PES_NUMBERS);
    if (m_pTreeCtrl)   {
        LoadImages ();
        EnumerateFunc (NULL, NULL);
		ExpandAll ();
    }
    InitControls ();
    EndWaitCursor ();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL WINAPI CDlgConnectToTcp::EnumerateFunc(LPNETRESOURCE lpnr, HTREEITEM hItem)
{
    DWORD dwResult, dwResultEnum;
    HANDLE hEnum;
    DWORD cbBuffer = 16384;     // 16K is a good size
    DWORD cEntries = -1;        // enumerate all possible entries
    LPNETRESOURCE lpnrLocal;    // pointer to enumerated structures
    DWORD i;
    HTREEITEM hChildItem;

    dwResult = WNetOpenEnum(RESOURCE_GLOBALNET, // all network resources
                            RESOURCETYPE_ANY,   // all resources
                            0,  // enumerate all resources
                            lpnr,       // NULL first time the function is called
                            &hEnum);    // handle to the resource

    if (dwResult != NO_ERROR) {
        return FALSE;
    }
    //
    // Call the GlobalAlloc function to allocate resources.
    //
    lpnrLocal = (LPNETRESOURCE) GlobalAlloc(GPTR, cbBuffer);
    if (lpnrLocal == NULL)
        return FALSE;
    do {
        //
        // Initialize the buffer.
        //
        ZeroMemory(lpnrLocal, cbBuffer);
        //
        // Call the WNetEnumResource function to continue
        //  the enumeration.
        //
        dwResultEnum = WNetEnumResource(hEnum,  // resource handle
                                        &cEntries,      // defined locally as -1
                                        lpnrLocal,      // LPNETRESOURCE
                                        &cbBuffer);     // buffer size
        //
        // If the call succeeds, loop through the structures.
        //
        if (dwResultEnum == NO_ERROR) {
            for (i = 0; i < cEntries; i++) {
                // Call an application-defined function to
                //  display the contents of the NETRESOURCE structures.
                //
                hChildItem = InsertItem (&lpnrLocal[i], hItem);
                //DisplayStruct(i, &lpnrLocal[i]);

                // If the NETRESOURCE structure represents a container resource, 
                //  call the EnumerateFunc function recursively.

                if (RESOURCEUSAGE_CONTAINER == (lpnrLocal[i].dwUsage
                                                & RESOURCEUSAGE_CONTAINER))
                    EnumerateFunc(&lpnrLocal[i], hChildItem);
            }
        }
        // Process errors.
        //
        else if (dwResultEnum != ERROR_NO_MORE_ITEMS)
            break;
    }
    //
    // End do.
    //
    while (dwResultEnum != ERROR_NO_MORE_ITEMS);
	GlobalFree ((HGLOBAL) lpnrLocal);
    //
    // Call WNetCloseEnum to end the enumeration.
    //
    dwResult = WNetCloseEnum(hEnum);

    return TRUE;
}

void CDlgConnectToTcp::LoadImages ()
{
    m_images.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    CBitmap bm;
    bm.LoadBitmap(IDB_BMP_CONNECTTO);
    m_images.Add(&bm, RGB(255, 0, 0));
    /*set image*/
	m_pTreeCtrl->SetImageList (&m_images, TVSIL_NORMAL);	
}

void CDlgConnectToTcp::OnDestroy() 
{
	FreeNetList ();
	m_pTreeCtrl->Detach ();
	m_pIPAddressCtrl->Detach ();
	CDialog::OnDestroy();
    _delete (m_pTreeCtrl);
    _delete (m_pIPAddressCtrl);
}

HTREEITEM CDlgConnectToTcp::InsertItem (LPNETRESOURCE lpnrLocal, HTREEITEM hParentItem)
{
    TV_INSERTSTRUCT sItem;
    HTREEITEM hNewItem = NULL;
    CString strName;
    CString strDesc;
    bool bInsertItem;

    bInsertItem = false;
    switch (lpnrLocal->dwDisplayType) {
        case RESOURCEDISPLAYTYPE_NETWORK :
            sItem.hParent = NULL;sItem.item.iImage = 0; sItem.item.iSelectedImage = 0;
            bInsertItem = true;
            break;
        case RESOURCEDISPLAYTYPE_DOMAIN :
            sItem.hParent = hParentItem;sItem.item.iImage = 1; sItem.item.iSelectedImage = 1;
            bInsertItem = true;
            break;
        case RESOURCEDISPLAYTYPE_SERVER :
            sItem.hParent = hParentItem;sItem.item.iImage = 2; sItem.item.iSelectedImage = 2;
            bInsertItem = true;
            break;
    }
    if (bInsertItem)   {
        if (lpnrLocal->lpRemoteName)
            strName = lpnrLocal->lpRemoteName;
        if (lpnrLocal->lpComment)
            strDesc = lpnrLocal->lpComment;
        CString strNamePlus(strName+"\t"+strDesc);
	    sItem.item.pszText = strNamePlus.GetBuffer(0);
	    sItem.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM;
        sItem.item.stateMask = TVIS_EXPANDEDONCE | TVIS_SELECTED;
        sItem.item.lParam = (LPARAM) new stNodeConnectTcpData;
		((stNodeConnectTcpData *)sItem.item.lParam)->strRemoteName = lpnrLocal->lpRemoteName;
		((stNodeConnectTcpData *)sItem.item.lParam)->dwDisplayType = lpnrLocal->dwDisplayType;
		m_listPNetResource.AddTail ((void *)sItem.item.lParam);
  	    hNewItem = m_pTreeCtrl->InsertItem(&sItem);
        strNamePlus.ReleaseBuffer ();
    }
    return hNewItem;
}

void CDlgConnectToTcp::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    stNodeConnectTcpData *NCTD = (stNodeConnectTcpData *) pNMTreeView->itemNew.lParam;
    in_addr *ptr;
    hostent *host;

	if (NCTD->dwDisplayType == RESOURCEDISPLAYTYPE_SERVER)   {
		CString strFullName = NCTD->strRemoteName;
		if ( 0 == strFullName.Left(2).Compare("\\\\") )   
			strFullName = strFullName.Right(strFullName.GetLength()-2);
		host = gethostbyname(strFullName);

		if (host != NULL)   { 
			ptr = (struct in_addr *) host->h_addr_list[0];                    
			m_pIPAddressCtrl->SetAddress (ptr->S_un.S_un_b.s_b1,
										  ptr->S_un.S_un_b.s_b2,
										  ptr->S_un.S_un_b.s_b3,
										  ptr->S_un.S_un_b.s_b4);
		}
	}
}

void CDlgConnectToTcp::ExpandAll()   
{
    HTREEITEM hRootItem = m_pTreeCtrl->GetRootItem();
    HTREEITEM hItem = hRootItem;

    while (hItem)    {
        if (m_pTreeCtrl->ItemHasChildren(hItem))
            m_pTreeCtrl->Expand(hItem, TVE_EXPAND);
        hItem = GetNextTreeItem(hItem);
    }
}

HTREEITEM CDlgConnectToTcp::GetNextTreeItem(HTREEITEM hItem)
{
      // has this item got any children
    if (m_pTreeCtrl->ItemHasChildren(hItem))  {
        return m_pTreeCtrl->GetNextItem(hItem, TVGN_CHILD);
    }
    else 
    if (m_pTreeCtrl->GetNextItem(hItem, TVGN_NEXT) != NULL)  {
        return m_pTreeCtrl->GetNextItem(hItem, TVGN_NEXT);
    }
    else      {
        // return the next item after our parent
        hItem = m_pTreeCtrl->GetParentItem(hItem);
        if (hItem == NULL)  {
              // no parent
              return NULL;
        }
        while (m_pTreeCtrl->GetNextItem(hItem, TVGN_NEXT) == NULL)  {
              hItem = m_pTreeCtrl->GetParentItem(hItem);
        }
        // next item that follows our parent
        return m_pTreeCtrl->GetNextItem(hItem, TVGN_NEXT);
    }
}

void CDlgConnectToTcp::FreeNetList ()
{
	LPNETRESOURCE lpnrLocal;
	
	for (POSITION pos = m_listPNetResource.GetHeadPosition (); pos; )   {
		lpnrLocal = (LPNETRESOURCE) m_listPNetResource.GetNext (pos);
		GlobalFree((HGLOBAL) lpnrLocal);
	}
}

void CDlgConnectToTcp::InitControls ()
{
    CString strPort;
    BYTE a, b, c, d;
    m_opt->GetTcpIpAddress (a, b, c, d);
    m_pIPAddressCtrl->SetAddress (a, b, c, d);
    strPort.Format ("%d", m_opt->GetTcpIpPort ());
    m_editPort.SetWindowText (strPort);
}

void CDlgConnectToTcp::OnBnClickedCtConnect()
{
    CString strPort;
    int nPort;
    unsigned char a, b, c, d;
    char *p;

    /* salviamo prima di tutto le proprieta */
    m_editPort.GetWindowText (strPort);
    nPort = strtol (strPort, &p, 10);
    if (nPort < 1024 || nPort > 65535)
        AfxMessageBox (LOADSTRING (IDS_INVALID_PORT_ADDRESS));
    else   {
        m_pIPAddressCtrl->GetAddress (a, b, c, d);
        m_opt->SetTypeConnection (enTCPConnection);
        m_opt->SetTcpIpAddress (a, b, c, d);
        m_opt->SetTcpIpPort (nPort);
        m_opt->SaveOptions ();
        /* ora eseguiamo la connessione */
        ((CMainFrame *) AfxGetMainWnd ())->ConnectToTcpIp ();
		OnOK ();
    }
}

void CDlgConnectToTcp::OnBnClickedCtConnectLocal()
{
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	m_opt->SetTypeConnection (enLocalConnection);
	m_opt->SaveOptions ();
	((CMainFrame *) AfxGetMainWnd ())->ConnectToLocal ();
	OnOK ();
}
