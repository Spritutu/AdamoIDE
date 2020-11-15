// DlgImportImage.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "basedialog.h"
#include "dlgmbx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NMAX_ID_LENGTH       32

/////////////////////////////////////////////////////////////////////////////
// CDlgMbx dialog
CDlgMbx::CDlgMbx(CWnd* pParent /*=NULL*/) 
	: CAdamoDialog(CDlgMbx::IDD, pParent), m_nGL (0), m_nLevel (-1), m_nProg (-1), m_pData (NULL)
{
	//{{AFX_DATA_INIT(CDlgMbx)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgMbx::~CDlgMbx ()
{
	_delete (m_pData);
}

void CDlgMbx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMbx)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_MB_REPORT, m_wndReport);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgMbx, CAdamoDialog)
	//{{AFX_MSG_MAP(CDlgMbx)
	ON_WM_CREATE ()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
** OnCreate :
*/
int CDlgMbx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_wndReport.SetTheme(xtpReportThemeOffice2013, TRUE);
	m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureNone;
	m_wndReport.GetImageManager()->SetIcons (IDB_BMP_MB, 0, 0, CSize (16, 16));
	return 0;
}

/*
** OnInitDialog :
*/
BOOL CDlgMbx::OnInitDialog() 
{
	CDialog::OnInitDialog();
	/* formattiamo il report control */
	InitReportCtrl ();
	/* per prima cosa andiamo a leggere i dati dal CNC */
	if (!LoadMbxData ())   {
		/* carichiamo ora il report control */
		LoadReportCtrl ();
	}
	SetWindowText (LOADSTRING (IDS_DLG_MBX_17));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*
** LoadMbxData :
*/
int CDlgMbx::LoadMbxData ()
{
	COleVariant v;
	CString str;
	int nBL, nB = -1;
	/* per prima cosa leggiamo il valore della variabile */
	ReadData (v);
	if (v.vt == VT_R8)   {
		/* deve essere una variabile numerica */
		int nMbx = (int) v.dblVal;
		if (nMbx >= 0 && nMbx < NMAXMAILBOX)   {
			/* andiamo a leggere i dati della mailbox */
			str.Format ("%d", nMbx);
			nBL = GetMachine ()->GetBufferLenght (0, str);
			if (nBL > 0) {
				m_pData = new BYTE [nBL];
				GetMachine ()->GetBufferData (m_pData, nBL);
				nB = 0;
			}
		}
	}
	return nB;
}

/*
** ReadData :
*/
bool CDlgMbx::ReadData (COleVariant& v)
{
    bool b;

    switch (m_nGL)   {
    case 0 :
        b=GetMachine()->GetVariable (GetVariableName (), v) == S_OK;
        break;
    case 1 :
        b=GetMachine()->GetLocalVariable (GetVariableName (), m_nLevel, m_nProg, v) == S_OK;
        break;
    case 2 :
        b=GetMachine()->GetExpressionVariable (GetVariableName (), m_nLevel, v) == S_OK;
        break;
    }
    return b;
}

/*
** InitReportCtrl :
*/
void CDlgMbx::InitReportCtrl ()
{
	CXTPReportColumn* pColumn;

	pColumn = new CXTPReportColumn(0, LOADSTRING (IDS_DLG_MBX_1), 100);
	m_wndReport.AddColumn(pColumn);
	pColumn->SetTreeColumn(TRUE);
	pColumn->SetIconID (0);

	pColumn = new CXTPReportColumn(1, LOADSTRING (IDS_DLG_MBX_2), 100);
	pColumn->SetAutoSize (TRUE);
	m_wndReport.AddColumn(pColumn);
	pColumn->SetIconID (1);
}

/*
** LoadReportCtrl :
*/
void CDlgMbx::LoadReportCtrl ()
{
	CXTPReportRecord* pRecord, *pChild, *pChild_1, *pChild_2;
	CString str;
	int nNumSlotPendenti, nNumMessaggi, n, i;
	BYTE* p = m_pData;

	/* aggiungiamo il primo record, nome mailbox */
	pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
	CXTPReportRecordItemText* pItem = new CXTPReportRecordItemText("MailBox");
	pRecord->AddItem(pItem);
	str = (char *)p; p+=strlen ((char *)p) + 1;
	pItem = new CXTPReportRecordItemText(str);
	pRecord->AddItem(pItem);
	/* secondo record, numero di slot pendenti */
	pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
	pItem = new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_3));
	pRecord->AddItem(pItem);
	nNumSlotPendenti = *(int* )p;
	str.Format ("%d", nNumSlotPendenti); p+=sizeof (int);
	pItem = new CXTPReportRecordItemText(str);
	pRecord->AddItem(pItem);
	/* se ci sono degli slot pendenti, visualizziamoli */
	while (nNumSlotPendenti > 0)   {
		str = (char *)p; p+=strlen ((char *) p) + 1;
		pChild = new CXTPReportRecord();
		pChild->AddItem(new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_4)));
		pChild->AddItem(new CXTPReportRecordItemText(str));
		pRecord->GetChilds()->Add(pChild);
		nNumSlotPendenti--;
	}
	/* ora le tre maschere di bit, slot pendenti, soddisfatti e serviti */
	pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
	pItem = new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_5));
	pRecord->AddItem(pItem);
	str.Format ("%x", *(short *) p); p+=sizeof (short);
	pItem = new CXTPReportRecordItemText(str);
	pRecord->AddItem(pItem);
	pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
	pItem = new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_6));
	pRecord->AddItem(pItem);
	str.Format ("%x", *(short *) p); p+=sizeof (short);
	pItem = new CXTPReportRecordItemText(str);
	pRecord->AddItem(pItem);
	pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
	pItem = new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_7));
	pRecord->AddItem(pItem);
	str.Format ("%x", *(short *) p); p+=sizeof (short);
	pItem = new CXTPReportRecordItemText(str);
	pRecord->AddItem(pItem);
	/* ora il numero di mail box in coda */
	pRecord = m_wndReport.AddRecord(new CXTPReportRecord());
	pItem = new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_8));
	pRecord->AddItem(pItem);
	nNumMessaggi = *(short* )p;
	str.Format ("%d", nNumMessaggi); p+=sizeof (short);
	pItem = new CXTPReportRecordItemText(str);
	pRecord->AddItem(pItem);
	i = 0;
	while (nNumMessaggi > 0)   {
		pChild = new CXTPReportRecord();
		str.Format ("[%d]", i);
		pChild->AddItem(new CXTPReportRecordItemText(str));
		pRecord->GetChilds()->Add(pChild);

		pChild_1 = new CXTPReportRecord();
		str.Format ("%d", *(int *)p); p+=sizeof (int);
		pChild_1->AddItem(new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_9)));
		pChild_1->AddItem(new CXTPReportRecordItemText(str));
		pChild->GetChilds()->Add(pChild_1);
		
		pChild_1 = new CXTPReportRecord();
		int nMessageType = *(int *)p; p+=sizeof (int);
		switch (nMessageType)   {
			case 0 : str = LOADSTRING (IDS_DLG_MBX_10); break;
			case 1 : str = LOADSTRING (IDS_DLG_MBX_11); break;
			case 2 : str = LOADSTRING (IDS_DLG_MBX_12); break;
		}
		pChild_1->AddItem(new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_13)));
		pChild_1->AddItem(new CXTPReportRecordItemText(str));
		pChild->GetChilds()->Add(pChild_1);

		pChild_1 = new CXTPReportRecord();
		str = (char *)p; p+=strlen ((char *) p) + 1;
		pChild_1->AddItem(new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_14)));
		pChild_1->AddItem(new CXTPReportRecordItemText(str));
		pChild->GetChilds()->Add(pChild_1);
		
		pChild_1  = new CXTPReportRecord();
		str = (char *)p; p+=strlen ((char *) p) + 1;
		pChild_1->AddItem(new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_15)));
		pChild_1->AddItem(new CXTPReportRecordItemText(str));
		pChild->GetChilds()->Add(pChild_1);
		
		pChild_1 = new CXTPReportRecord();
		pChild_1->AddItem(new CXTPReportRecordItemText(LOADSTRING (IDS_DLG_MBX_16)));
		pChild->GetChilds()->Add(pChild_1);

		BYTE nElementType = *p; p++; n = 0;
		while (nElementType != '\0')   {
			pChild_2 = new CXTPReportRecord();
			str.Format ("[%d]", n);
			pChild_2->AddItem(new CXTPReportRecordItemText(str));
			switch (nElementType)   {
				case LUA_TNUMBER :
					str.Format ("%.3f", *(double *) p); p+=sizeof (double);
					break;
				case LUA_TSTRING :
					str = p; p+=strlen ((char *)p) + 1;
					break;
				case LUA_TBOOLEAN :
					str = * (int *) p == 0 ? LOADSTRING (IDS_ADAMOFALSO) : LOADSTRING (IDS_ADAMOVERO); p+=sizeof (int);
					break;
			}
			pChild_2->AddItem(new CXTPReportRecordItemText(str));
			pChild_1->GetChilds()->Add(pChild_2);
			n++;
			nElementType = *p; p++;
		}
		nNumMessaggi--; i++;
	}
	/* popoliamo il report control */
	m_wndReport.Populate();
	/* esplodiamo tutti i rami */
	m_wndReport.ExpandAll (TRUE);
}

/*
** OnOK :
*/
void CDlgMbx::OnOK ()
{
	EndDialog (0);
}
