// AdamoRSColumns.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "AdamoRSColumns.h"


// CAdamoRSColumns dialog

IMPLEMENT_DYNAMIC(CAdamoRSColumns, CDialog)

CAdamoRSColumns::CAdamoRSColumns(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoRSColumns::IDD, pParent), m_slImages (NULL)
{
}

CAdamoRSColumns::~CAdamoRSColumns()
{
}

void CAdamoRSColumns::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdamoRSColumns, CDialog)
END_MESSAGE_MAP()


// CAdamoRSColumns message handlers
void CAdamoRSColumns::SetColumnsData (stGridColumnData *pgcd)
{
	m_gcd.m_nType = pgcd->m_nType;
	m_gcd.m_strName = pgcd->m_strName;
	m_gcd.m_strImage = pgcd->m_strImage;
}

void CAdamoRSColumns::GetColumnsData (stGridColumnData *pgcd)
{
	pgcd->m_nType = m_gcd.m_nType;
	pgcd->m_strName = m_gcd.m_strName;
	pgcd->m_strImage = m_gcd.m_strImage;
}

void CAdamoRSColumns::SetImagesList (CStringList *images)
{
	m_slImages = images;
}

BOOL CAdamoRSColumns::OnInitDialog()
{
	int nId;
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	switch (m_gcd.m_nType)   {
		case LUA_TNUMBER :
			nId = IDC_RS_COL_NUMBER; break;
		case LUA_TBOOLEAN :
			nId = IDC_RS_COL_BOOLEAN; break;
		case LUA_TSTRING :
			nId = IDC_RS_COL_STRING; break;
		default :
			nId = IDC_RS_COL_NUMBER; break;
	}
	CheckRadioButton (IDC_RS_COL_NUMBER, IDC_RS_COL_STRING, nId);
	m_editColName.SubclassEdit(IDC_RS_COL_NAME, this, PES_HEXNUMBERS|PES_NUMBERS|PES_LETTERS|PES_UNDERLINE);
	m_editColName.SetWindowText (m_gcd.m_strName);
	FillImageCombo ();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoRSColumns::FillImageCombo ()
{
	CComboBox *cb;
	CString str;
	bool bFirst = true;

	cb = (CComboBox *)GetDlgItem (IDC_RS_COL_IMAGE);
	for (POSITION pos = m_slImages->GetHeadPosition (); pos; )   {
		CString str = m_slImages->GetNext (pos);
		if (bFirst)   {
			cb->AddString ("");
			bFirst = false;
		}
		cb->AddString (str);
	}
	if (m_gcd.m_strImage != "")
		cb->SelectString (-1, m_gcd.m_strImage);
	else
		cb->SelectString (-1, "");
}

void CAdamoRSColumns::OnOK ()
{
	CComboBox *cb;
	int nId;

	cb = (CComboBox *)GetDlgItem (IDC_RS_COL_IMAGE);
	cb->GetWindowText (m_gcd.m_strImage);
	m_editColName.GetWindowText (m_gcd.m_strName);
	nId = GetCheckedRadioButton (IDC_RS_COL_NUMBER, IDC_RS_COL_STRING);
	switch (nId)   {
		case IDC_RS_COL_NUMBER :
			m_gcd.m_nType = LUA_TNUMBER;  break;
		case IDC_RS_COL_BOOLEAN :
			m_gcd.m_nType = LUA_TBOOLEAN; break;
		case IDC_RS_COL_STRING :
			m_gcd.m_nType = LUA_TSTRING;  break;
	}
	CDialog::OnOK ();
}
