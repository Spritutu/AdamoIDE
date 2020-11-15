/*
** ConfigHWGrid.h
*/

#ifndef __ADAMO_CONFIGHW_GRID__
#define __ADAMO_CONFIGHW_GRID__

/* include files */
#include "gridctrl.h"
#include "ConfigDoc.h"

/* forward reference */
class CDlgHWModule;

/* constants definitions */
#define NMAX_STCONFIGHW 128

/* data definitions */
struct stConfigHW   {
    stConfigHW () : m_nRow (-1), m_nCol (-1), m_nRowDesc (-1), m_nColDesc (-1)   { }
    int m_nRow;
    int m_nCol;
    int m_nRowDesc;
    int m_nColDesc;
    CString m_strHWAddr;
};

class CConfigHWGridCell : public CGridCell   {
    DECLARE_DYNCREATE (CConfigHWGridCell)
    public :
        CConfigHWGridCell ()    { }
        void    SetTipText (CString str)    { m_strTooltipText=str; }
        LPCTSTR GetTipText () const         { return (LPCTSTR) m_strTooltipText; }
    private :
        CString m_strTooltipText;
};

class CConfigHWGrid : public CGridCtrl
{
    public :
        CConfigHWGrid ();
        void SetConfigDoc (CConfigDoc *pConfigDoc)   { m_pConfigDoc=pConfigDoc; }
		void SetProject (CProject *pPrj)			 { m_pPrj = pPrj; }
        void SetGridData  (int nRow, int nCol, int nRowDesc, int nColDesc, CString strHWAddr);
        void SetType (enDispositivi e)               { m_eType=e; }
        void UpdateData ();
        void RemoveCurrentConnection ();
        bool CleanDeviceInfo (CString strHWAddr, CString *strName, int *nBits);
        void DisassignDevice (int n, int nBits);
        bool Find (const char *szBuffer);
        void SetMode (int nMode)                    { m_nMode=nMode; }
        virtual void FillGridCaption ();
		virtual int  OnConnected (int nRow, int nCol, const char *szHWAddress);
		virtual int  OnRemoveConnection (int nRow, int nCol, const char *szHWAddress);
    private :
        BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
        bool IsValidCell (CCellID &cid, int *n);
        int  GetDragData (COleDataObject* pDataObject, int *nType, int *nID, int *nNumBits, CString *strMacchina, CString *strName, CString *strDescription, CString *strPercorso);
        int  SetDeviceInfo (int nID, CString strHWAddress);
        void AssignDevice (CCellID &cid, CString strName, CString strDescription, CString strPercorso, int nID, int nBits);
        void OnEditCell (int nRow, int nCol, CPoint point, UINT nChar);
        void OnEndEditCell (int nRow, int nCol, CString str);
        void LoadHWConfig ();
        void AssignDeviceOnLoad (CString strName, CString strDescription, CString strHWAddress, int nID, int n, int nBits);
        void ClearSelection ();
        void AssignHWAnalogRange (int n);
        void DisassignHWAnalogRange (int nRow);
        void CutSelectedText ();
        int  AlreadyAssigned (CString strHWAddress);
	protected :
        CImageList  m_ImageList;
        CConfigDoc *m_pConfigDoc;
		CProject   *m_pPrj;
        enDispositivi m_eType;
        stConfigHW  m_stConfigHW[NMAX_STCONFIGHW];
    private :
        int         m_nst;
        int         m_nMode;
};

class CConfigModuleGrid : public CGridCtrl
{
    public :
        CConfigModuleGrid ();
        void SetConfigDoc (CConfigDoc *pConfigDoc)   { m_pConfigDoc=pConfigDoc; }
		void InitData ();
		CString GetModuleName ();
		int  AssignModule (CString strName, CString strDescription, CString strModule);
		void RemoveCurrentConnection ();
		void SetParentWnd (CDlgHWModule *pDlgHWModule);
    private :
        BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
		void InitGrid ();
		void InitGridImages ();
		bool IsValidCell (CCellID &cid);
		int  GetDragData (COleDataObject* pDataObject, CString *strName, CString *strDescription, CString *strModule);
        void OnEditCell (int nRow, int nCol, CPoint point, UINT nChar);
		void SaveModified (CString strModule);
		void SetCurrentProject (CString strName);
	protected :
        CImageList  m_ImageList;
        CConfigDoc *m_pConfigDoc;
		CDlgHWModule *m_pDlgHWModule;
};

#endif
