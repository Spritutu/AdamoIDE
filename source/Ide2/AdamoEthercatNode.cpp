// AdamoEthercatNode.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoEthercatNode.h"
#include "AdamoEthercatAssi.h"
#include "AdamoEthercatIO.h"
#include "AdamoEthercatIOAnalogico.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC (CAdamoEthercatNode, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatNode dialog


CAdamoEthercatNode::CAdamoEthercatNode(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoEthercatNode::IDD, pParent), m_bInitialized (false), m_nNode (-1),
	  m_pFlatTabCtrl (NULL), m_pConfigDoc (NULL), m_pPrj (NULL)
{
	//{{AFX_DATA_INIT(CAdamoEthercatNode)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoEthercatNode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoEthercatNode)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoEthercatNode, CDialog)
	//{{AFX_MSG_MAP(CAdamoEthercatNode)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoEthercatNode message handlers

/*
** OnSize :
*/
void CAdamoEthercatNode::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
    if (m_bInitialized)   {
        m_pFlatTabCtrl->MoveWindow (10, 10, cx-20, cy-25);
		m_pFlatTabCtrl->Invalidate ();
    }
}

/*
** OnInitDialog :
*/
BOOL CAdamoEthercatNode::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetConfigDoc ();
	CreateTab ();
	m_bInitialized=true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** CreateTab :
*/
void CAdamoEthercatNode::CreateTab ()
{
    RECT rc;

    memset (&rc, 0, sizeof (RECT));
	m_pFlatTabCtrl = new CXTFlatTabCtrl();
	if (m_pFlatTabCtrl->Create(WS_CHILD | WS_VISIBLE | FTS_XT_BOTTOM | FTS_XT_TOOLTIPS,
		&rc, this, IDC_FLAT_TABCTRL))  {
		for (int i=0; i<MAX_ETHERCAT_SLAVE_TYPE; i++)
			switch (GetDeviceType (i))   {
				case eEthAsse :   {
					CAdamoEthercatAssi *pDialog=new CAdamoEthercatAssi ();
					pDialog->SetConfigDoc (m_pConfigDoc);
					pDialog->SetProject (m_pPrj);
			        pDialog->SetEthercatNode (m_nNode, i);
					pDialog->Create (IDD_DLG_ETHERCAT_ASSI, this);
    				m_pFlatTabCtrl->InsertItem(i, LOADSTRING (IDS_ETHERCAT_ASSE), pDialog);
					break;
				}
				case eEthIO :   {
					CAdamoEthercatIO *pDialog=new CAdamoEthercatIO ();
					pDialog->SetConfigDoc (m_pConfigDoc);
					pDialog->SetProject (m_pPrj);
			        pDialog->SetEthercatNode (m_nNode, i);
					pDialog->Create (IDD_DLG_ETHERCAT_IO, this);
    				m_pFlatTabCtrl->InsertItem(i, LOADSTRING (IDS_ETHERCAT_IO), pDialog);
					break;
				}
				case eEthIOAnalogic :   {
					CAdamoEthercatIOAnalogico *pDialog=new CAdamoEthercatIOAnalogico ();
					pDialog->SetConfigDoc (m_pConfigDoc);
					pDialog->SetProject (m_pPrj);
			        pDialog->SetEthercatNode (m_nNode, i);
					pDialog->Create (IDD_DLG_ETHERCAT_IO_ANALOGICO, this);
    				m_pFlatTabCtrl->InsertItem(i, LOADSTRING (IDS_ETHERCAT_ANALOG), pDialog);
					break;
				}
			}
		m_pFlatTabCtrl->SetCurSel(0);
	}
}

/*
** OnDestroy :
*/
void CAdamoEthercatNode::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	/*
	for (int i=0; i<m_pFlatTabCtrl->GetItemCount (); i++)   {
		CDialog *pDialog=(CDialog *)m_pFlatTabCtrl->GetItemWindow (i);
		if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatAssi)))
			delete ((CAdamoEthercatAssi *) pDialog);
		else
		if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatIO)))
			delete ((CAdamoEthercatIO *) pDialog);
	}
	*/
    delete m_pFlatTabCtrl;
}

/*
** SetConfigDoc :
*/
void CAdamoEthercatNode::SetConfigDoc ()
{
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enConfTemplate);
    if (pTmpl)
        m_pConfigDoc=(CConfigDoc *)pTmpl->GetDocument ("");
}

/*
** Find :
*/
void CAdamoEthercatNode::Find (const char *szBuffer)
{
	for (int i=0; i<m_pFlatTabCtrl->GetItemCount (); i++)   {
		CDialog *pDialog=(CDialog *)m_pFlatTabCtrl->GetItemWindow (i);
		if (strstr (szBuffer, "ID") || strstr (szBuffer, "OD"))   {  
			if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatIO)))   {
				m_pFlatTabCtrl->SetCurSel (i);
				((CAdamoEthercatIO *) pDialog)->Find (szBuffer); break;
			}
		}
		else
		if (strstr (szBuffer, "Ax"))   {
			if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatAssi)))   {
				m_pFlatTabCtrl->SetCurSel (i);
				((CAdamoEthercatAssi *) pDialog)->Find (szBuffer); break;
			}
		}
		if (strstr (szBuffer, "IA") || strstr (szBuffer, "OA"))   {
			if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatIOAnalogico)))   {
				m_pFlatTabCtrl->SetCurSel (i);
				((CAdamoEthercatIOAnalogico *) pDialog)->Find (szBuffer); break;
			}
		}
	}
}

/*
** RemoveCurrentConnection :
*/
void CAdamoEthercatNode::RemoveCurrentConnection ()
{
	for (int i=0; i<m_pFlatTabCtrl->GetItemCount (); i++)   {
		CDialog *pDialog=(CDialog *)m_pFlatTabCtrl->GetItemWindow (i);
		if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatIO)))
			((CAdamoEthercatIO *) pDialog)->RemoveCurrentConnection ();
		else
		if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatAssi)))
			((CAdamoEthercatAssi *) pDialog)->RemoveCurrentConnection ();
		else
		if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatIOAnalogico)))
			((CAdamoEthercatIOAnalogico *) pDialog)->RemoveCurrentConnection ();
	}
}

/*
** OnChangePsw :
*/
void CAdamoEthercatNode::OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)
{
	for (int i=0; i<m_pFlatTabCtrl->GetItemCount (); i++)   {
		CDialog *pDialog=(CDialog *)m_pFlatTabCtrl->GetItemWindow (i);
		if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatAssi)))
			((CAdamoEthercatAssi *) pDialog)->OnChangePsw (oldPsw, newPsw);
		else
		if (pDialog->IsKindOf (RUNTIME_CLASS (CAdamoEthercatIOAnalogico)))
			((CAdamoEthercatIOAnalogico *) pDialog)->OnChangePsw (oldPsw, newPsw);
	}
}

/*
** GetDeviceType :
*/
eEthercatDeviceType CAdamoEthercatNode::GetDeviceType (int nIndex)
{
	eEthercatDeviceType e=eEth;

	CProject *pPrj=((CMainFrame *) AfxGetMainWnd ())->GetProject ();
	if (pPrj)
		e=((stAdamoEthSlave *)pPrj->GetSlaveArray ()[m_nNode-1])->nDeviceType[nIndex];
	return e;
}
