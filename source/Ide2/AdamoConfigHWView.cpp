// AdamoConfigHWView.cpp : implementation file
//

/* include files */
#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoConfigHWView.h"
#include "DlgAdapters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfigHWView

IMPLEMENT_DYNCREATE(CAdamoConfigHWView, CPaneView)

CAdamoConfigHWView::CAdamoConfigHWView() : m_bInitialized (false), m_bScanNetwork (false)
{
}

CAdamoConfigHWView::~CAdamoConfigHWView()
{
}


BEGIN_MESSAGE_MAP(CAdamoConfigHWView, CPaneView)
	//{{AFX_MSG_MAP(CAdamoConfigHWView)
	//ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
    ON_COMMAND(ID_ETHERCAT_ADAPTERS, OnAdapters)
    ON_COMMAND(ID_ETHERCAT_SCAN_NETWORK, OnScanNetwork)
    ON_COMMAND(ID_ETHERCAT_ADDXMLFILE, OnAddXMLFile)
    ON_COMMAND(ID_ETHERCAT_REMOVEXMLFILE, OnRemoveXMLFile)
	ON_WM_CREATE ()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfigHWView drawing

void CAdamoConfigHWView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfigHWView diagnostics

#ifdef _DEBUG
void CAdamoConfigHWView::AssertValid() const
{
	CPaneView::AssertValid();
}

void CAdamoConfigHWView::Dump(CDumpContext& dc) const
{
	CPaneView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdamoConfigHWView message handlers

void CAdamoConfigHWView::OnInitialUpdate() 
{
	CPaneView::OnInitialUpdate();
    if (!m_bInitialized)   {
	    // TODO: Add your specialized code here and/or call the base class
        InitView ();
        m_bInitialized=true;
    }
}

/*
** InitView :
*/
void CAdamoConfigHWView::InitView ()
{
    FillTree ();
}

/*
** SetTreeStyle :
*/
void CAdamoConfigHWView::SetTreeStyle () 
{
    CBitmap bm;

    m_il.Create(16, 16, ILC_MASK|ILC_COLOR8, 1, 1);
    bm.LoadBitmap(IDB_BMP_CONFIGHW);
    m_il.Add(&bm, RGB(0, 128, 128));
    GetTreeCtrl().SetFont (((CMainFrame *)AfxGetMainWnd ())->GetProgramFont ());
	GetTreeCtrl().SetImageList (&m_il, TVSIL_NORMAL);
}

/*
** FillTree :
*/
void CAdamoConfigHWView::FillTree () 
{
    CString str, strBuffer, strNum;
    HTREEITEM htRoot, htLocalBus, htItemYoupsilon, htItemOmikron, htRemoteBus, htItemRemoteModule;
    int i;

	htRoot=GetTreeCtrl().InsertItem ("Local Bus", 10, 10, TVI_ROOT);
	
    str.LoadString (IDS_CONFIG_HW_PCISLOT);
    for (i=0; i<4; i++)   {
        strNum.Format (" %d", i);
        strBuffer=str+strNum;
        htLocalBus=GetTreeCtrl().InsertItem (strBuffer, 0, 0, htRoot);
        GetTreeCtrl().SetItemData (htLocalBus, MakeConfigHWItemData (-1, -1, -1, -1));
        strBuffer.LoadString (IDS_CONFIG_HW_YOUPSILON);
        htItemYoupsilon=GetTreeCtrl().InsertItem (strBuffer, 1, 1, htLocalBus);
        GetTreeCtrl().SetItemData (htItemYoupsilon, MakeConfigHWItemData (-1, -1, -1, -1));
        InsertYoupsilonTree (htItemYoupsilon, PCI_YOUPSILON, i);
        strBuffer.LoadString (IDS_CONFIG_HW_OMIKRON);
        htItemOmikron=GetTreeCtrl().InsertItem (strBuffer, 2, 2, htLocalBus);
        GetTreeCtrl().SetItemData (htItemOmikron, MakeConfigHWItemData (-1, -1, -1, -1));
        InsertOmikronTree (htItemOmikron, PCI_OMIKRON, i);
    }
	str.LoadString (IDS_CONFIG_HW_ETHERCAT);
	m_htItemEthercat=GetTreeCtrl().InsertItem (str, 12, 12, htRoot);
	GetTreeCtrl().SetItemData (m_htItemEthercat, MakeConfigHWItemData (PCI_ETHERCAT, -1, -1, -1));
	htRemoteBus=GetTreeCtrl().InsertItem ("Network", 5, 5, TVI_ROOT);
    str.LoadString (IDS_CONFIG_HW_REMOTE_MODULE);
    for (i=0; i<8; i++)   {
        strNum.Format (" %d", i);
        strBuffer=str+strNum;
        htItemRemoteModule=GetTreeCtrl().InsertItem (strBuffer, 11, 11, htRemoteBus);
		InsertModuleTree (htItemRemoteModule, NET_EPSILON, i);
        InsertOmikronTree (htItemRemoteModule, NET_EPSILON, i);
		InsertEthercatTree (htItemRemoteModule, NET_EPSILON, i);
	}
}

/*
** InsertYoupsilonTree :
*/
void CAdamoConfigHWView::InsertYoupsilonTree (HTREEITEM htItemYoupsilon, int nCardType, int nNode)
{
    CString str;
    HTREEITEM ht;

    str.LoadString (IDS_CONFIG_HW_ONBOARD);
    ht=GetTreeCtrl().InsertItem (str, 3, 3, htItemYoupsilon);
    GetTreeCtrl().SetItemData (ht, MakeConfigHWItemData (PCI_YOUPSILON, nNode, -1, -1));
    InsertCanOpenTree (htItemYoupsilon, nCardType, nNode);
}

/*
** InsertOmikronTree :
*/
void CAdamoConfigHWView::InsertOmikronTree (HTREEITEM htItemOmikron, int nCardType, int nNode)
{
    CString str, strServo, strBuffer, strNum;
    HTREEITEM htItem, htItemData;
    int i;

    str.LoadString (IDS_CONFIG_HW_SSCNETIII);
    htItem=htItemOmikron;
    htItemOmikron=GetTreeCtrl().InsertItem (str, 4, 4, htItemOmikron);
    GetTreeCtrl().SetItemData (htItemOmikron, MakeConfigHWItemData (-1, nNode, -1, -1));
    str.LoadString (IDS_CONFIG_HW_RETE);
    htItemData=GetTreeCtrl().InsertItem (str, 5, 5, htItemOmikron);
    GetTreeCtrl().SetItemData (htItemData, MakeConfigHWItemData (nCardType, nNode, NETWORK_TYPE_SSCNETIII, -1));
    strServo.LoadString (IDS_CONFIG_HW_SERVO);
    for (i=0; i<NMAXMGASTATION; i++)   {
        strNum.Format (" %d", i+1);
        strBuffer=strServo+strNum;
        htItemData=GetTreeCtrl().InsertItem (strBuffer, 6, 6, htItemOmikron);
        GetTreeCtrl().SetItemData (htItemData, MakeConfigHWItemData (nCardType, nNode, NETWORK_TYPE_SSCNETIII, i));
    }
    InsertCanOpenTree (htItem, nCardType, nNode);
}

/*
** InsertCanOpenTree :
*/
void CAdamoConfigHWView::InsertCanOpenTree (HTREEITEM htItem, int nCard, int nNode)
{
    CString str, strNode, strNum, strBuffer;
    HTREEITEM htItemData;
    int i;
    
    str.LoadString (IDS_CONFIG_HW_CANOPEN);
    htItem=GetTreeCtrl().InsertItem (str, 7, 7, htItem);
    GetTreeCtrl().SetItemData (htItem, MakeConfigHWItemData (-1, nNode, -1, -1));
    str.LoadString (IDS_CONFIG_HW_RETE);
    htItemData=GetTreeCtrl().InsertItem (str, 8, 8, htItem);
    GetTreeCtrl().SetItemData (htItemData, MakeConfigHWItemData (nCard, nNode, NETWORK_TYPE_CANOPEN, -1));
    for (i=1; i<=NMAX_CANNODES; i++)   {
        strNode.LoadString (IDS_CONFIG_HW_NODO);
        strNum.Format (" %d", i);
        strBuffer=strNode+strNum;
        htItemData=GetTreeCtrl().InsertItem (strBuffer, 9, 9, htItem);
        GetTreeCtrl().SetItemData (htItemData, MakeConfigHWItemData (nCard, nNode, NETWORK_TYPE_CANOPEN, i));
    }
}

/*
** MakeConfigHWItemData :
*/
DWORD CAdamoConfigHWView::MakeConfigHWItemData (BYTE bCardType, BYTE bPciSlot, BYTE bNetworkType, BYTE bNode)
{
    WORD w1=MAKEWORD (bPciSlot, bCardType);
    WORD w2=MAKEWORD (bNetworkType, bNode);
    DWORD dw=MAKELONG (w1, w2);
    return dw;
}

/*
** OpenHWDialog :
*/
void CAdamoConfigHWView::OpenHWDialog (int nCardType, int nCard, int i, int n)
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)   {
            POSITION pos=pDoc->GetFirstViewPosition ();
            while (pos)   {
                CView* pView=pDoc->GetNextView (pos);
                if (pView)   {
                    if (pView->IsKindOf(RUNTIME_CLASS(CAdamoConfHWView)))   {
                        ((CAdamoConfHWView *)pView)->CreateDlgHW (nCardType, nCard, i, n);
                    }
                }
            }
        }
    }
}

/*
** OnRClick :
*/
void CAdamoConfigHWView::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu  menu;                                
	CPoint pt;

    HTREEITEM ht=GetTreeCtrl().GetSelectedItem ();
    DWORD dw=GetTreeCtrl().GetItemData (ht);
    short w2 = LOWORD (dw);
    char  b4 = HIBYTE (w2);
	if (b4 == PCI_ETHERCAT || b4 == NET_EPSILON)   {
		if (((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ()<=ePswBuilder)  {
			VERIFY(menu.LoadMenu(IDR_EC_XML_FILE));
			GetCursorPos(&pt);
			CMenu* pPopup = menu.GetSubMenu(0);
			ASSERT(pPopup != NULL);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
		}
	}
}

/*
** CreateEthercatDir :
*/
CString CAdamoConfigHWView::CreateEthercatDir ()
{
    CString strPath;

    strPath=GETWS()->GetActiveProject ()->GetProjectDir ();
    strPath=strPath + "\\" + DIRETHERCAT;
    if (_access (strPath, 00))
        CreateDirectory(strPath, NULL);
    return strPath;
}

/*
** LoadEthercatXMLFile :
*/
void CAdamoConfigHWView::LoadEthercatXMLFile (CProject* pPrj, CPtrArray &pSlaves)
{
	FindEthercatNode (pPrj);
	/* per prima cosa cancelliamo tutti i rami dal tree */
	DeleteEthercatNodes ();
	FillEthercatNodes (pSlaves);
}

/*
** DeleteEthercatNodes :
*/
void CAdamoConfigHWView::DeleteEthercatNodes ()
{
	HTREEITEM ht, htNext;

	ht=GetTreeCtrl().GetChildItem(m_htItemEthercat);
	while (ht)   {
		htNext=GetTreeCtrl().GetNextSiblingItem (ht);
		GetTreeCtrl().DeleteItem (ht);
		ht=htNext;
	}
}

/*
** OnDestroy :
*/
void CAdamoConfigHWView::OnDestroy() 
{
	CPaneView::OnDestroy();
}

/*
** FillEthercatNodes :
*/
void CAdamoConfigHWView::FillEthercatNodes (CPtrArray &pSlaves)
{
	CString str;
	HTREEITEM ht;

    str.LoadString (IDS_CONFIG_HW_RETE);
	ht=GetTreeCtrl ().InsertItem (str, 8, 8, m_htItemEthercat);
	GetTreeCtrl().SetItemData (ht, MakeConfigHWItemData (-1, 0, NETWORK_TYPE_ETHERCAT, -1));
	for (int i=0; i<pSlaves.GetSize (); i++)   {
		ht=GetTreeCtrl ().InsertItem (((stAdamoEthSlave *)pSlaves[i])->szName, 9, 9, m_htItemEthercat);
		GetTreeCtrl().SetItemData (ht, MakeConfigHWItemData (-1, -1, NETWORK_TYPE_ETHERCAT, i+1));
	}
}

/*
** SaveHWConfiguration 
*/
void CAdamoConfigHWView::SaveHWConfiguration (CProject *pPrj)
{
	CAdamoTemplate *pTmpl=((CIdeApp *)AfxGetApp ())->GetTemplate (enConfTemplate);
	if (pTmpl)   {
		CProject *pPrj=GETWS ()->GetActiveProject ();
		if (pPrj)   {
			CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
			if (pDoc)   {
				CAdamoHWConfigFile *pHWConfigFile=pPrj->GetHWConfigFile ();
				if (pHWConfigFile)   {
					CPtrArray &slaveArray=pPrj->GetSlaveArray ();
					for (int i=0; i<slaveArray.GetSize (); i++)   {
						for (int n=0; n<MAX_ETHERCAT_SLAVE_TYPE; n++)   {
							pHWConfigFile->SetEthercatDeviceType (i, n, ((stAdamoEthSlave *)slaveArray[i])->nDeviceType[n]);
							pHWConfigFile->EnableEthercatVariables (i, n, ((stAdamoEthSlave *)slaveArray[i])->liEnableVariables[n]);
						}
						pHWConfigFile->SetRequestedState (((stAdamoEthSlave *)slaveArray[i])->nRequestedState);
					}
				}
				pPrj->SetHWConfigModified ();
			}
		}
	}
}

/*
** CleanInfo :
*/
void CAdamoConfigHWView::CleanInfo (CProject *pPrj)
{
	pPrj->ClearSlaveArray ();
	CAdamoDatiFile *pConfigFile=pPrj->GetAdamoDatiFile ();
	if (pConfigFile)   {
		pConfigFile->ResetEthHWAddress ();
		if (pConfigFile->Open (pPrj->GetParamsPathName(), true))   {
			pConfigFile->WriteData ();
			pConfigFile->CloseFile ();
		}
	}
	CAdamoHWConfigFile *pHWConfigFile=pPrj->GetHWConfigFile ();
	if (pHWConfigFile)
		if (pHWConfigFile->Open (pPrj->GetHWConfigPathName(), true))   {
			pHWConfigFile->CleanEthercatInfo ();
			pHWConfigFile->WriteData ();
			pHWConfigFile->CloseFile ();
		}
}

/*
** DestroyAnyDlg :
*/
void CAdamoConfigHWView::DestroyAnyDlg ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)   {
        CConfigDoc *pDoc=(CConfigDoc *)pTmpl->GetDocument ("");
        if (pDoc)   {
            POSITION pos=pDoc->GetFirstViewPosition ();
            while (pos)   {
                CView* pView=pDoc->GetNextView (pos);
                if (pView)   {
                    if (pView->IsKindOf(RUNTIME_CLASS(CAdamoConfHWView)))   {
                        ((CAdamoConfHWView *)pView)->DestroyDlgHW ();
                    }
                }
            }
        }
    }
}

/*
** OnAdapters :
*/
void CAdamoConfigHWView::OnAdapters ()
{
/*
	CAdamoHWConfigFile *pHWConfigFile = GetHWConfigFile ();
	CCmdGetAdapters EA;
    CAdamoMachine *pMachine = GetMachine ();
	if (pMachine && pHWConfigFile)   {
		if (pMachine->GetEthernetAdapters (&EA) == S_OK)   {
			if (EA.nAdapters > 0)   {
				CDlgAdapters dlg;
				dlg.SetAdapters (&EA);
				dlg.SetCurrentAdapter (pHWConfigFile->GetEthernetAdapter ());
				if (dlg.DoModal () == IDOK)   {
					CString strCurrentAdapter = dlg.GetCurrentAdapter ();
					pHWConfigFile->SetEthernetAdapter (strCurrentAdapter);
					UpdateTree (strCurrentAdapter);
					SetHWConfigModified ();
				}
			}
		}
	}
*/
}

/*
** OnScanNetwork :
*/
void CAdamoConfigHWView::OnScanNetwork ()
{
/*
	CAdamoHWConfigFile *pHWConfigFile = GetHWConfigFile ();
    CAdamoMachine *pMachine = GetMachine ();
	if (pMachine)   {
		CString strEthernetAdapter = pHWConfigFile->GetEthernetAdapter ();
		if (strEthernetAdapter != "")   {
			pMachine->EcOpen (strEthernetAdapter.AllocSysString ());
			pMachine->EcScanNetwork ();
			m_bScanNetwork = true;
		}
	}
*/
}

/*
** OnAddXMLFile :
*/
void CAdamoConfigHWView::OnAddXMLFile ()
{
	CProject *pPrj;
	CString strFileName, strPathName, strPathToPresent;
    CPoint pt, p;

    HTREEITEM ht=GetTreeCtrl().GetSelectedItem ();
    DWORD dw=GetTreeCtrl().GetItemData (ht);
    short w2 = LOWORD (dw);
    char  b4 = HIBYTE (w2);
    char  b3 = LOBYTE (w2);
	GetCursorPos(&pt); p=pt;
	GetTreeCtrl().ScreenToClient(&pt);
	pPrj = GetEthercatPrj (b4, b3);
	CAdamoTemplate *pTmpl=((CIdeApp *)AfxGetApp ())->GetTemplate (enConfTemplate);
	if (pTmpl)   {
		if (pPrj)   {
			CAdamoHWConfigFile *pHWConfigFile=pPrj->GetHWConfigFile ();
			if (pHWConfigFile)   {
				m_htItemEthercat = ht;
				strPathName=CreateEthercatDir ();
				strPathToPresent=strPathName + "\\" + "*.xml";
				CKSFileDialog fd(TRUE, "xml", strPathToPresent, OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST, 
					"Ethercat Network Information files (*.xml)|*.xml|All files (*.*)|*.*||", AfxGetMainWnd());
				if (fd.DoModal()==IDOK)   {
					strPathName=strPathName + "\\" + fd.GetFileName ();
					CopyFile ((const char*)fd.GetPathName (), (const char*)strPathName, false);
					CleanInfo (pPrj);
					if (pPrj->LoadEthercatXMLFile (fd.GetFileName ()))   {
						DestroyAnyDlg ();
						pHWConfigFile->SetEthercatMasterXML (fd.GetFileName ());
						if (pPrj)
							pPrj->SetHWConfigModified ();
					}
				}
			}
		}
	}
}

/*
** GetEthercatPrj :
*/
CProject *CAdamoConfigHWView::GetEthercatPrj (int nCardType, int nCard)
{
	CAdamoMachine *pMachine;
	CAdamoPlant* pPlant = GETWS ()->GetPtrPlant ();

	CProject *pPrj = pPlant->GetFirst ();
	while (pPrj)   {
		pMachine = pPrj->GetMachine ();
		if (pMachine)   {
			if (pMachine->GetTypeModule () == enNoModule ||
				(pMachine->GetTypeModule () == enDRTOS && pMachine->GetHWModuleNumber () == nCard)) 
				break;
		}
		pPrj = (CProject *) pPlant->GetNext ();
	}
	return pPrj;
}

/*
** OnRemoveXMLFile :
*/
void CAdamoConfigHWView::OnRemoveXMLFile ()
{
    HTREEITEM ht=GetTreeCtrl().GetSelectedItem ();
    DWORD dw=GetTreeCtrl().GetItemData (ht);
    short w2 = LOWORD (dw);
    char  b4 = HIBYTE (w2);
    char  b3 = LOBYTE (w2);
	CProject *pPrj = GetEthercatPrj (b4, b3);
	if (pPrj)   {
		m_htItemEthercat = ht;
		CleanInfo (pPrj);
		DeleteEthercatNodes ();
	}
}

/*
** InsertModuleTree :
*/
void CAdamoConfigHWView::InsertModuleTree (HTREEITEM htItem, int nCardType, int nNode)
{
	HTREEITEM ht;
	CString str;

    str.LoadString (IDS_CONFIG_HW_RETE);
    ht=GetTreeCtrl().InsertItem (str, 5, 5, htItem);
    GetTreeCtrl().SetItemData (ht, MakeConfigHWItemData (nCardType, nNode, -1, -1));
}

/*
** GetMachine :
*/
CAdamoMachine *CAdamoConfigHWView::GetMachine ()
{
	CAdamoMachine *pMachine = NULL;
    CProject* pPrj=GETWS()->GetActiveProject();
    if (pPrj)
        pMachine = pPrj->GetMachine ();
	return pMachine;
}

/*
** GetHWConfigFile :
*/
CAdamoHWConfigFile *CAdamoConfigHWView::GetHWConfigFile ()
{
	CAdamoHWConfigFile *pHWConfigFile = NULL;
	CAdamoTemplate *pTmpl=((CIdeApp *)AfxGetApp ())->GetTemplate (enConfTemplate);
	if (pTmpl)   {
		CProject *pPrj=GETWS ()->GetActiveProject ();
		if (pPrj)
			pHWConfigFile=pPrj->GetHWConfigFile ();
	}
	return pHWConfigFile;
}

/*
** SetHWConfigModified :
*/
void CAdamoConfigHWView::SetHWConfigModified ()
{
	CAdamoHWConfigFile *pHWConfigFile = NULL;
	CAdamoTemplate *pTmpl=((CIdeApp *)AfxGetApp ())->GetTemplate (enConfTemplate);
	if (pTmpl)   {
		CProject *pPrj=GETWS ()->GetActiveProject ();
		if (pPrj)
			pPrj->SetHWConfigModified ();
	}
}

/*
** UpdateTree :
*/
void CAdamoConfigHWView::UpdateTree (CString strCurrentAdapter)
{
	CString str;
	str.LoadString (IDS_CONFIG_HW_ETHERCAT);
	if (strCurrentAdapter != "")
		str = str + " - " + strCurrentAdapter;
	GetTreeCtrl().SetItemText (m_htItemEthercat, str);
}

/*
** OnAdamoTimer :
*/
void CAdamoConfigHWView::OnAdamoTimer ()
{
/*
	if (m_bScanNetwork)   {
	    CAdamoMachine *pMachine = GetMachine ();
		if (pMachine)   {
			CCmdGetEcScanNetworkStatus cs;
			CProgressCtrl *ECProgressBar = ((CMainFrame *)AfxGetMainWnd ())->GetEthercatProgressBar ();
			CColorStaticST *ECSlaveName = ((CMainFrame *)AfxGetMainWnd ())->GetEthercatSlaveName ();
			if (pMachine->EcGetScanNetworkStatus (&cs) == S_OK)   {
				if (cs.nStatus != EC_SCAN_COMPLETED)   {
					if (cs.nStatus == EC_SCAN_PDO_STARTED)   {
						ECProgressBar->SetPos (cs.nPerc);
						ECSlaveName->SetWindowText (cs.strSlave);
					}
					else
					if (cs.nStatus == EC_SCAN_ERROR)   {
						ECProgressBar->SetPos (0);
						ECSlaveName->SetWindowText ("");
						m_bScanNetwork = false;
					}
				}
				else   {
					m_bScanNetwork = false;
					ECProgressBar->SetPos (0);
					ECSlaveName->SetWindowText ("");
					if (cs.nSlave > 0)
						LoadEcData (cs.nSlave);
				}
			}
		}
	}
*/
}

/*
** RefreshMetrics :
*/
void CAdamoConfigHWView::RefreshMetrics(AppTheme nTheme)
{
	CPaneView::RefreshMetrics(nTheme);
	switch (nTheme)
	{
		case themeVS2010: 
			m_treeCtrl.SetTheme(xtpReportThemeVisualStudio2012Blue); 
			m_treeCtrl.ModifyStyle(TVS_HASLINES, 0);

			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
		case themeVS2012Light: 
			m_treeCtrl.SetTheme(xtpReportThemeVisualStudio2012Light); 
			m_treeCtrl.ModifyStyle(TVS_HASLINES, 0);

			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
		case themeVS2012Dark: 
			m_treeCtrl.SetTheme(xtpReportThemeVisualStudio2012Dark); 
			m_treeCtrl.ModifyStyle(TVS_HASLINES, 0);
			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
		case themeVS2015Light:
		case themeVS2015Dark:
		case themeVS2015Blue: 
			m_treeCtrl.SetTheme (xtpControlThemeVisualStudio2015);
			m_treeCtrl.ModifyStyle (TVS_HASLINES, 0);
			if (XTPSystemVersion()->IsWinVistaOrGreater())
			{
				m_treeCtrl.SetExplorerTheme(TRUE);
			}
			break;
	}	
}

/*
** CreateTree :
*/
int CAdamoConfigHWView::CreateTree ()
{
	if (m_treeCtrl.GetSafeHwnd() == 0)  	{
		if (!m_treeCtrl.Create ( WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_FULLROWSELECT
								| TVS_SHOWSELALWAYS, CRect(0,0, 0, 0), this, ID_TREECTRL_CONFHW))   {
			TRACE0("Unable to create tree control.\n");
			return 0;
		}
	}
	return 1;
}

/*
** OnSize :
*/
void CAdamoConfigHWView::OnSize(UINT nType, int cx, int cy) 
{
	CPaneView::OnSize(nType, cx, cy);
	
	if ( ::IsWindow(GetTreeCtrl ().GetSafeHwnd ()) )
		GetTreeCtrl ().SetWindowPos (NULL, 0, 0, cx, cy, 0);
}

/*
** OnLButtonDblClk :
*/
void CAdamoConfigHWView::OnLButtonDblClk (UINT u, CPoint pt)
{
	// TODO: Add your control notification handler code here
    HTREEITEM ht=GetTreeCtrl().GetSelectedItem ();
    DWORD dw=GetTreeCtrl().GetItemData (ht);
    short w1=HIWORD (dw);
    short w2=LOWORD (dw);
    char b1=LOBYTE (w1);
    char b2=HIBYTE (w1);
    char b3=LOBYTE (w2);
    char b4=HIBYTE (w2);
    switch (b4)   {
        case PCI_YOUPSILON :
            if (b1==-1)
                OpenHWDialog (b4, b3, b2, 0);
            else 
                if (b1==NETWORK_TYPE_CANOPEN && b2==-1)
                    OpenHWDialog (b4, b3, b2, 1);
                else
                    if (b1==NETWORK_TYPE_CANOPEN && b2!=-1)
                        OpenHWDialog (b4, b3, b2, 2);
            break;
        case PCI_OMIKRON :
            if (b1==NETWORK_TYPE_SSCNETIII && b2==-1)
                OpenHWDialog (b4, b3, b2, 3);
            else
                if (b1==NETWORK_TYPE_SSCNETIII && b2!=-1)
                    OpenHWDialog (b4, b3, b2, 4);
                else
                    if (b1==NETWORK_TYPE_CANOPEN && b2==-1)
                        OpenHWDialog (b4, b3, b2, 1);
                    else
                        if (b1==NETWORK_TYPE_CANOPEN && b2!=-1)
                            OpenHWDialog (b4, b3, b2, 2);
            break;
		case NET_EPSILON :
			if (b1 == -1)
                OpenHWDialog (b4, b3, b2, 7);
			else
				if (b1==NETWORK_TYPE_SSCNETIII && b2==-1)
					OpenHWDialog (b4, b3, b2, 3);
				else
					if (b1==NETWORK_TYPE_SSCNETIII && b2!=-1)
						OpenHWDialog (b4, b3, b2, 4);
					else
						if (b1==NETWORK_TYPE_CANOPEN && b2==-1)
							OpenHWDialog (b4, b3, b2, 1);
						else
							if (b1==NETWORK_TYPE_CANOPEN && b2!=-1)
								OpenHWDialog (b4, b3, b2, 2);
			break;
		default :
			if (b1==NETWORK_TYPE_ETHERCAT && b2==-1)
				OpenHWDialog (b4, -1, b2, 5);
			else
				if (b1==NETWORK_TYPE_ETHERCAT)
					OpenHWDialog (b4, -1, b2, 6);
    }
}

int CAdamoConfigHWView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (CreateTree ())
		SetTreeStyle ();
	return 0;
}

/*
** InsertEthercatTree :
*/
void CAdamoConfigHWView::InsertEthercatTree (HTREEITEM htItemRemoteModule, int nCardType, int nNode)
{
	CString str;
	str.LoadString (IDS_CONFIG_HW_ETHERCAT);
	HTREEITEM htItemEthercat=GetTreeCtrl().InsertItem (str, 12, 12, htItemRemoteModule);
	GetTreeCtrl().SetItemData (htItemEthercat, MakeConfigHWItemData (nCardType, nNode, NETWORK_TYPE_ETHERCAT, -1));
}

/*
** FindEthercatNode :
*/
void CAdamoConfigHWView::FindEthercatNode (CProject* pPrj)
{
	CString str;
	HTREEITEM hItem = m_treeCtrl.GetRootItem (), hChildItem;
	DWORD dw;
	if (m_treeCtrl.ItemHasChildren (hItem))   {
		hChildItem = m_treeCtrl.GetChildItem (hItem);
		while (hChildItem != NULL)   {
			str = m_treeCtrl.GetItemText (hChildItem);
			dw = m_treeCtrl.GetItemData (hChildItem);
			short w1 = HIWORD (dw);
			short w2 = LOWORD (dw);
			char  b1 = LOBYTE (w1);
			char  b2 = HIBYTE (w1);
			char  b3 = LOBYTE (w2);
			char  b4 = HIBYTE (w2);
			if (b4 == PCI_ETHERCAT)   {
				if (pPrj->GetMachine ()->GetTypeModule () == enNoModule)   {
					m_htItemEthercat = hChildItem;
					return;
				}
			}
			hChildItem = GetTreeCtrl().GetNextSiblingItem (hChildItem);
		}
	}
	hItem = m_treeCtrl.GetNextSiblingItem (hItem);
	if (m_treeCtrl.ItemHasChildren (hItem))   {
		hChildItem = m_treeCtrl.GetChildItem (hItem);
		int nModule = 0;
		while (hChildItem != NULL)   {
			CString str = m_treeCtrl.GetItemText (hChildItem);
			if (pPrj->GetMachine ()->GetTypeModule () == enDRTOS && pPrj->GetMachine ()->GetHWModuleNumber () == nModule)   {
				if (m_treeCtrl.ItemHasChildren (hChildItem))   {
					hChildItem = m_treeCtrl.GetChildItem (hChildItem);
					while (hChildItem != NULL)   {
						str = m_treeCtrl.GetItemText (hChildItem);
						dw = m_treeCtrl.GetItemData (hChildItem);
						short w1 = HIWORD (dw);
						short w2 = LOWORD (dw);
						char  b1 = LOBYTE (w1);
						char  b2 = HIBYTE (w1);
						char  b3 = LOBYTE (w2);
						char  b4 = HIBYTE (w2);
						if (b1 == NETWORK_TYPE_ETHERCAT)   {
							m_htItemEthercat = hChildItem;
							return;
						}
						hChildItem = GetTreeCtrl().GetNextSiblingItem (hChildItem);
					}
				}
			}
			nModule++;
			hChildItem = GetTreeCtrl().GetNextSiblingItem (hChildItem);
		}
	}
}

BEGIN_MESSAGE_MAP(CAdamoXTPTreeCtrl, CXTPTreeCtrl)
	//{{AFX_MSG_MAP(CAdamoConfigHWView)
	//ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT (NM_RCLICK, OnRClick)
END_MESSAGE_MAP()

void CAdamoXTPTreeCtrl::OnLButtonDblClk(UINT u, CPoint pt)
{
	__super::OnLButtonDblClk (u, pt);
	((CAdamoConfigHWView*)GetParent())->OnLButtonDblClk (u, pt);
}

void CAdamoXTPTreeCtrl::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	((CAdamoConfigHWView*)GetParent())->OnRClick (pNMHDR, pResult);
}

