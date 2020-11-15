/*
** ConfigModuleGrid.cpp
*/

#include "stdafx.h"
#include "mainframe.h"
#include "WorkSpace.h"
#include "Project.h"
#include "ConfigHWGrid.h"
#include "DlgHWModule.h"

/* construction / destruction */
CConfigModuleGrid::CConfigModuleGrid () : m_pConfigDoc (NULL), m_pDlgHWModule (NULL)
{
}

/*
** InitData :
*/
void CConfigModuleGrid::InitData ()
{
	InitGrid ();
	InitGridImages ();
}

/*
** InitGrid :
*/
void CConfigModuleGrid::InitGrid ()
{
    GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    SetEditable(false);
    SetVirtualMode(false);
    SetFixedColumnCount(1);
    SetColumnCount(4);
    SetFixedRowCount ();
    SetRowCount (2);
    SetEditable ();
    EnableDragAndDrop ();
    SetListMode ();
}

/*
** InitGridImages :
*/
void CConfigModuleGrid::InitGridImages ()
{
    GV_ITEM Item;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_CONFIG_HW_GRID), 16, 1, RGB(0,128,128));
	SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
    Item.row = 0;
    Item.col = 1;
    Item.iImage = 0;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_NOME);
    SetItem(&Item);
    Item.row = 0;
    Item.col = 2;
    Item.iImage = 1;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_DESCRIZIONE);
    SetItem(&Item);
    Item.row = 0;
    Item.col = 3;
    Item.iImage = 17;
    Item.strText=LOADSTRING (IDS_CONFIG_HW_REMOTE_MODULE);
    SetItem(&Item);
}

/*
** OnDrop :
*/
BOOL CConfigModuleGrid::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
    CString strName, strDescription, strModule;
    BOOL b=FALSE;    

    CCellID cid=GetCellFromPt (point);
    if (IsValidCell (cid))   {
        if (!GetDragData (pDataObject, &strName, &strDescription, &strModule))   {
			AssignModule (strName, strDescription, strModule);
			SaveModified (strName);
			SetCurrentProject (strName);
		}
	}
    OnDragLeave();
    return b;
}

/*
** IsValidCell :
*/
bool CConfigModuleGrid::IsValidCell (CCellID &cid)
{
    bool b=false;

	if (cid.col == 1 && cid.row == 1)
		b = true;
    return b;
}

/*
** GetDragData :
*/
int CConfigModuleGrid::GetDragData (COleDataObject* pDataObject, CString *strName, CString *strDescription, CString *strModule)
{
    int nB=-1;
    HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
    CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold this.
    LPSTR szBuffer = new char[::GlobalSize(hmem)];

    sf.Read(szBuffer, ::GlobalSize(hmem));
    ::GlobalUnlock(hmem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems

    char *p=strchr (szBuffer, '@');
    if (p)   {
        char *q = strchr (p+1, '@');
		*p = *q = '\0';
		*strName = szBuffer;
		*strDescription = p + 1;
		*strModule = q + 1;
		nB = 0;
    }
    delete szBuffer;
    return nB;
}

/*
** AssignModule :
*/
int CConfigModuleGrid::AssignModule (CString strName, CString strDescription, CString strModule)
{
	int nB = -1;

	if (GetItemText (1, 1) == "")   {
		/* niente di assegnato, andiamo avanti */
		SetItemText (1, 1, strName);
		SetItemText (1, 2, strDescription);
		SetItemText (1, 3, strModule);
		Invalidate ();
		nB = 0;
	}
	return nB;
}

/*
** OnEditCell :
*/
void CConfigModuleGrid::OnEditCell (int nRow, int nCol, CPoint point, UINT nChar)
{
}

/*
** OnEditCell :
*/
CString CConfigModuleGrid::GetModuleName ()
{
	return GetItemText (1, 1);
}

/*
** SaveModified :
*/
void CConfigModuleGrid::SaveModified (CString strModule)
{
	CProject *pPrj = GETWS ()->GetProjectByName (strModule);
	if (pPrj)
		pPrj->SetMachineModifiedFlag (true);
}

/*
** RemoveCurrentConnection :
*/
void CConfigModuleGrid::RemoveCurrentConnection ()
{
	CString strModule = GetItemText (1, 1);
	if (strModule)   {
		SetItemText (1, 1, "");
		SetItemText (1, 2, "");
		SetItemText (1, 3, "");
		Invalidate ();
		SaveModified (strModule);
	}
}

/*
** SetCurrentProject :
*/
void CConfigModuleGrid::SetParentWnd (CDlgHWModule *pDlgHWModule)
{
	m_pDlgHWModule = pDlgHWModule;
}

/*
** SetCurrentProject :
*/
void CConfigModuleGrid::SetCurrentProject (CString strName)
{
	CProject *pPrj = GETWS ()->GetProjectByName (strName);
	if (pPrj)
		m_pDlgHWModule->SetCurrentProject (pPrj);
}
