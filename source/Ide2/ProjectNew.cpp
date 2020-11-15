// ProjectNew.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "workspace.h"
#include "ProjectNew.h"
#include "Project.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectNew dialog


CProjectNew::CProjectNew(CWnd* pParent /*=NULL*/)
	: CDialog(CProjectNew::IDD, pParent), m_nModule (0)
{
	//{{AFX_DATA_INIT(CProjectNew)
	//}}AFX_DATA_INIT
}


void CProjectNew::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectNew)
	DDX_Control(pDX, IDC_PROJECT_NAME, m_ctrlEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectNew, CDialog)
	//{{AFX_MSG_MAP(CProjectNew)
	ON_BN_CLICKED(IDC_RADIO_SIM, OnRadioSim)
	ON_BN_CLICKED(IDC_RADIO_PHYSIC, OnRadioPhysic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectNew message handlers

void CProjectNew::OnOK() 
{
    CString str;
	UpdateData(TRUE);

    m_ctrlEdit.GetWindowText (m_strName);
	if ( m_strName.IsEmpty () )
	{
		AfxMessageBox(LOADSTRING (IDS_ADAMOPROJECTNEW_1));
		return;
	}
    ((CComboBox*)GetDlgItem (IDC_CMBMODULENUMBER))->GetWindowText (str);
    m_nModule=atoi (str);
	CDialog::OnOK();
}

CString CProjectNew::GetProjectName()
{
	return m_strName;
}

BOOL CProjectNew::OnInitDialog() 
{
    int i, n;
    char szBuffer[256];

	CDialog::OnInitDialog();
    m_ctrlEdit.SubclassEdit (IDC_PLANT_NAME, this, PES_LETTERS|PES_HEXNUMBERS|PES_NUMBERS|PES_UNDERLINE);
	CButton *pRadio = (CButton*)GetDlgItem  (IDC_RADIO_SIM);
    pRadio->SetCheck(TRUE);
    m_eTypeMachine = enVirtual;
    for (i=0, n=0; i<NMAXMACHINES; i++)
        if (!GETWS()->ExistModule (i))   {
            sprintf (szBuffer, "%d", i);
	        ((CComboBox*)GetDlgItem (IDC_CMBMODULENUMBER))->InsertString (n++, szBuffer);
        }
    ((CComboBox*)GetDlgItem (IDC_CMBMODULENUMBER))->SetCurSel (0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProjectNew::OnRadioSim() 
{
    m_eTypeMachine = enVirtual;	
}

void CProjectNew::OnRadioPhysic() 
{
	m_eTypeMachine = enPhysical;
}
