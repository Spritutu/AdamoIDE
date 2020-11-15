#pragma once

/* include files */
#include "stdafx.h"
#include "gridctrl.h"
#include "GridCellCheck.h"

class CAdamoSynGridCheck : public CGridCellCheck 
{
    DECLARE_DYNCREATE(CAdamoSynGridCheck)
    public :
        CAdamoSynGridCheck ()  { }
    protected :
        void OnClick( CPoint PointCellRelative);
};

class CAdamoSynGrid : public CGridCtrl, public CAdamoSynCtrl
{
	public:
		CAdamoSynGrid(void);
		~CAdamoSynGrid(void);
		void SetGridData (int nRows, int nColumns);
		void SetColumnData (int nColumn, stGridColumnData *pgcd);
		void GetGridData (int *pnRow, int *pnColumns);
		void GetColumnData (int nColumn, stGridColumnData *pgcd);
		eRSCtrl KindOf ()	{ return RSGrid; }
		void SetParentWnd (CWnd *pParent)	{ m_pParent = pParent; }
		BOOL Create ();
		BOOL Destroy ();
		void SetModifiedFlag (bool b = true)   { m_bModifiedFlag = b; }
		int OnSave ();
		int OnLoad ();
		int OnDownload ();
		int OnTimer ()							{ return 0; }
	private :
		int m_nRows;
		int m_nCols;
		stGridColumnData m_gcd[MAXCOLUMNS];
		CImageList m_il;
		CWnd *m_pParent;
		bool m_bModifiedFlag;
		CString m_strBeforeEditing;
	private :
		void LoadColumnsImages ();
		int  LoadDBImage (CString strImageName, CBitmap *bmp);
		void LoadColumns ();
		void LoadRows ();
		void OnEditCell (int nRow, int nCol, CPoint point, UINT nChar);
		void OnEndEditCell(int nRow, int nCol, CString str);
};
