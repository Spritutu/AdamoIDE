#include "stdafx.h"
#include "ResourceFile.h"
#include "AdamoSynContainer.h"
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynContainer.h"
#include "AdamoSynGrid.h"
#include "resource.h"

/* check del grid */
IMPLEMENT_DYNCREATE(CAdamoSynGridCheck, CGridCellCheck)

void CAdamoSynGridCheck::OnClick (CPoint PointCellRelative)
{
	if (PointCellRelative.x > 0 && PointCellRelative.y > 0)   {
		CGridCellCheck::OnClick (PointCellRelative);
		((CAdamoSynGrid *)m_pGrid)->SetModifiedFlag ();
		m_pGrid->Invalidate ();
	}
}

/* construction / destruction */
CAdamoSynGrid::CAdamoSynGrid(void) : m_pParent (NULL), m_bModifiedFlag (false)
{
}

CAdamoSynGrid::~CAdamoSynGrid(void)
{
	SetImageList (NULL);
}

/*
** Create :
*/
BOOL CAdamoSynGrid::Create ()
{
	BOOL b = CGridCtrl::Create (GetObjStyle ().GetRect (), m_pParent, GetID ());
	if (b)   {
		SetFixedRowCount();
		SetFixedColumnCount();
		SetRowCount (m_nRows + 1);
		SetColumnCount (m_nCols + 1);
		ExpandColumnsToFit (false);
		LoadColumnsImages ();
		LoadColumns ();
		LoadRows ();
	}
	return b;
}

/*
** LoadColumnsImages :
*/
void CAdamoSynGrid::LoadColumnsImages ()
{
	CBitmap *bmp;
	int nID, i;

	if (m_il.Create (16, 16, ILC_MASK, 1, 1))   {
		for (i = 0; i<m_nCols; i++)   {
			if (m_gcd[i].m_strImage != "")   {
				bmp = new CBitmap;
				if (!LoadDBImage (m_gcd[i].m_strImage, bmp))
					m_il.Add (bmp, RGB (0, 128, 128));
				delete bmp;
			}
			else   {
				bmp = new CBitmap;
				switch (m_gcd[i].m_nType)   {
					case LUA_TNUMBER :
						nID = IDB_CAP_NUMBER_TYPE;
						break;
					case LUA_TBOOLEAN :
						nID = IDB_CAP_BOOLEAN_TYPE;
						break;
					case LUA_TSTRING :
						nID = IDB_CAP_STRING_TYPE;
						break;
				}
				bmp->LoadBitmap (nID);
				m_il.Add (bmp, RGB (0, 128, 128));
				delete bmp;
			}
		}
		SetImageList (&m_il);
	}
}

/*
** LoadDBImage :
*/
int CAdamoSynGrid::LoadDBImage (CString strImageName, CBitmap *bmp)
{
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	return pSynContainer->CaricaImmagine (strImageName, bmp);
}

/*
** LoadColumns :
*/
void CAdamoSynGrid::LoadColumns ()
{
	CString str;
    GV_ITEM Item;
	int i;

    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
	for (i = 0; i<m_nCols; i++)   {
		Item.col=i+1;
        Item.iImage=i;
        str=m_gcd[i].m_strName;
        Item.strText=str.GetBuffer (0);
        SetItem(&Item);
	}
}

/*
** Destroy :
*/
BOOL CAdamoSynGrid::Destroy ()
{
	return CGridCtrl::DestroyWindow ();
}

/*
** SetGridData :
*/
void CAdamoSynGrid::SetGridData (int nRows, int nColumns)
{
	if (nRows >= 0 && nColumns >= 0)   {
		m_nRows = nRows;
		m_nCols = nColumns;
	}
}

/*
** SetGridData :
*/
void CAdamoSynGrid::SetColumnData (int nColumn, stGridColumnData *pgcd)
{
	if (nColumn >= 0 && nColumn <= MAXCOLUMNS)   {
		m_gcd[nColumn].m_nType = pgcd->m_nType;
		m_gcd[nColumn].m_strName = pgcd->m_strName;
		m_gcd[nColumn].m_strImage = pgcd->m_strImage;
	}
}

/*
** SetGridData :
*/
void CAdamoSynGrid::GetGridData (int *pnRow, int *pnColumns)
{
	*pnRow = m_nRows;
	*pnColumns = m_nCols;
}

/*
** GetColumnData :
*/
void CAdamoSynGrid::GetColumnData (int nColumn, stGridColumnData *pgcd)
{
	if (nColumn >= 0 && nColumn < MAXCOLUMNS)   {
		pgcd->m_nType = m_gcd[nColumn].m_nType;
		pgcd->m_strName = m_gcd[nColumn].m_strName;
		pgcd->m_strImage = m_gcd[nColumn].m_strImage;
	}
}

/*
** LoadRows :
*/
void CAdamoSynGrid::LoadRows ()
{
	CString str;
	int i, n;

	for (i = 0; i<m_nRows; i++)   {
		str.Format ("%d", i + 1);
		SetItemText (i + 1, 0, str);
		for (n = 0; n<m_nCols; n++)   {
			if (m_gcd[n].m_nType == LUA_TBOOLEAN)
				SetCellType (i + 1, n + 1, RUNTIME_CLASS(CAdamoSynGridCheck));
			else
				GetCell (i + 1, n + 1)->SetFormat (DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		}
	}
}

/*
** OnEditCell :
*/
void CAdamoSynGrid::OnEditCell (int nRow, int nCol, CPoint point, UINT nChar)
{
    m_strBeforeEditing=GetItemText (nRow, nCol);
    CGridCtrl::OnEditCell (nRow, nCol, point, nChar);    
}

/*
** OnEndEditCell :
*/
void CAdamoSynGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
	if (m_strBeforeEditing.Trim () != str)
		SetModifiedFlag ();
    CGridCtrl::OnEndEditCell(nRow, nCol, str);
}

/*
** OnSave :
*/
int CAdamoSynGrid::OnSave ()
{
	CADORecordset pRSGrid;
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CResourceFile *pRF = pSynContainer->GetResourceFile ();
	CString strTable, strField;
	double fp;
	int i, n;
	BOOL bField;
	char *p;

	if (pRF)   {
		CADODatabase *pDB = pRF->GetDB ();
        pRSGrid = CADORecordset(pDB);
		strTable = m_objVar.GetObjVar().m_strTable;
		if (strTable != "")   {
			if (pRSGrid.Open (strTable, CADORecordset::openTable))   {
				if (pRSGrid.GetRecordCount ())   {
					pRSGrid.MoveFirst ();
					do   {
						pRSGrid.Delete ();
						pRSGrid.MoveNext ();
					} while (!pRSGrid.IsEof());
					pRSGrid.Update ();
				}
				for (n = 0; n<m_nRows; n++)   {
					pRSGrid.AddNew ();
					for (i = 0; i<m_nCols; i++)   {
						switch (m_gcd[i].m_nType)   {
							case LUA_TNUMBER :
								strField = GetItemText (n + 1, i + 1);
								strField.Trim ();	
								fp = strtod (strField, &p);
								if (*p != '\0')
									fp = 0.0;
								pRSGrid.SetFieldValue (m_gcd[i].m_strName, fp);
								break;
							case LUA_TSTRING :
								strField = GetItemText (n + 1, i + 1);
								strField.Trim ();	
								pRSGrid.SetFieldValue (m_gcd[i].m_strName, strField);
								break;
							case LUA_TBOOLEAN :
								bField = ((CAdamoSynGridCheck *)GetCell (n + 1, i + 1))->GetCheck ();
								pRSGrid.SetFieldValue (m_gcd[i].m_strName, bField);
								break;
						}
					}
					pRSGrid.SetFieldValue ("Riga", n);
					pRSGrid.Update ();
				}
				pRSGrid.Close ();
			}
		}
	}
	return 0;
}

/*
** OnLoad :
*/
int CAdamoSynGrid::OnLoad ()
{
	CADORecordset pRSGrid;
	CAdamoSynContainer *pSynContainer = GETSYNCONT();
	CResourceFile *pRF = pSynContainer->GetResourceFile ();
	CString strTable, strField;
	double fp;
	int i, n;
	BOOL bField;

	if (pRF)   {
		CADODatabase *pDB = pRF->GetDB ();
        pRSGrid = CADORecordset(pDB);
		strTable = m_objVar.GetObjVar().m_strTable;
		if (strTable != "")   {
			if (pRSGrid.Open (strTable, CADORecordset::openTable))   {
				if (pRSGrid.GetRecordCount ())   {
					pRSGrid.MoveFirst ();
					do   {
						pRSGrid.GetFieldValue ("Riga", n);
						for (i = 0; i<m_nCols; i++)   {
							switch (m_gcd[i].m_nType)   {
								case LUA_TNUMBER :
									pRSGrid.GetFieldValue (m_gcd[i].m_strName, fp);
									strField.Format ("%.2f", fp);
									SetItemText (n + 1, i + 1, strField);
									break;
								case LUA_TSTRING :
									pRSGrid.GetFieldValue (m_gcd[i].m_strName, strField);
									SetItemText (n + 1, i + 1, strField);
									break;
								case LUA_TBOOLEAN :
									pRSGrid.GetFieldValue (m_gcd[i].m_strName, bField);
									((CAdamoSynGridCheck *)GetCell (n + 1, i + 1))->SetCheck (bField);
									break;
							}
						}
						pRSGrid.MoveNext ();
					} while (!pRSGrid.IsEof());
				}
				pRSGrid.Close ();
				Invalidate ();
			}
		}
	}
	return 0;
}

int CAdamoSynGrid::OnDownload ()
{

	return 0;
}
