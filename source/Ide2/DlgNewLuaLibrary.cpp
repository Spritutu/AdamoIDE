// DlgLuaLibraries.cpp : file di implementazione
//
#include "stdafx.h"
#include "mainframe.h"
#include "DlgNewLuaLibrary.h"
#include "adamolibrary.h"

IMPLEMENT_DYNAMIC(CAdamoDlgNewLuaLibrary, CDialog)

CAdamoDlgNewLuaLibrary::CAdamoDlgNewLuaLibrary(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoDlgNewLuaLibrary::IDD, pParent)
{

}

CAdamoDlgNewLuaLibrary::~CAdamoDlgNewLuaLibrary()
{
}

void CAdamoDlgNewLuaLibrary::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdamoDlgNewLuaLibrary, CDialog)
END_MESSAGE_MAP()

BOOL CAdamoDlgNewLuaLibrary::OnInitDialog()
{
	CDialog::OnInitDialog();
    CenterWindow ();
	m_ctrlName.SubclassEdit (IDC_LIBRARY_NAME, this, PES_LETTERS|PES_HEXNUMBERS|PES_NUMBERS|PES_UNDERLINE);
	m_ctrlDesc.SubclassEdit (IDC_LIBRARY_DESC, this, PES_LETTERS|PES_HEXNUMBERS|PES_NUMBERS|PES_UNDERLINE|PES_BLANK);
	return TRUE;
}

void CAdamoDlgNewLuaLibrary::OnOK ()
{
    GetDlgItemText (IDC_LIBRARY_NAME, m_strName); 
    GetDlgItemText (IDC_LIBRARY_DESC, m_strDesc);
    m_strName.TrimLeft (); m_strName.TrimRight ();
    m_strDesc.TrimLeft (); m_strDesc.TrimRight ();
    CDialog::OnOK ();
}