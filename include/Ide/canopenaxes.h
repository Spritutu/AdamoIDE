#if !defined(AFX_CANOPENASSI_H__9796E483_0E7C_4943_B8C7_CBA33F0DBE59__INCLUDED_)
#define AFX_CANOPENASSI_H__9796E483_0E7C_4943_B8C7_CBA33F0DBE59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// canopendigitalio.h : header file
//

#include "ConfigDoc.h"
#include "ConfigHWGrid.h"

#define STATUS_INDEX				0x6040
#define STATUS_SUBINDEX				0
#define CONTROL_INDEX				0x6041
#define CONTROL_SUBINDEX			0
#define POSITION_ACTUAL_INDEX		0x6064
#define POSITION_ACTUAL_SUBINDEX	0
#define POSITION_TARGET_INDEX		0x607a
#define POSITION_TARGET_SUBINDEX	0
#define SPEED_ACTUAL_INDEX			0x606c
#define SPEED_ACTUAL_SUBINDEX		0
#define SPEED_TARGET_INDEX			0x60ff
#define SPEED_TARGET_SUBINDEX		0
#define TORQUE_ACTUAL_INDEX			0x6077
#define TORQUE_ACTUAL_SUBINDEX		0
#define TORQUE_TARGET_INDEX			0x6071
#define TORQUE_TARGET_SUBINDEX		0

class CCanOpenAxesGridCtrl : public CGridCtrl
{
	public :
		CCanOpenAxesGridCtrl () : m_pPrj (NULL)  {
		}
		void SetProject (CProject* pPrj)   {
			m_pPrj = pPrj;
		}
		BOOL  ValidateEdit(int nRow, int nCol, LPCTSTR str)   {
			BOOL b = FALSE;
			char *p;
			int nValue = strtol (str, &p, 0);
			if (*p == '\0')   {
				if (nValue > 0x1000 && nValue < 0x8000)   {
					m_pPrj->SetHWConfigModified ();	
					b = TRUE;
				}
			}
			return b;
		}
	private :
		CProject *m_pPrj;
};

/////////////////////////////////////////////////////////////////////////////
// CCanOpenDigitalIO dialog

class CCanOpenAxes : public CDialog
{
// Construction
public:
	CCanOpenAxes(CWnd* pParent = NULL);   // standard constructor
    void SetKinemaAddress (int nKinemaAddress)  { m_nKinemaAddress=nKinemaAddress; }
    void SetCanOpenNode (int nNode)             { m_nNode=nNode; }
    void SetConfigDoc (CConfigDoc *pDoc)        { m_pConfigDoc=pDoc; }
	void SetProject (CProject *pPrj)			{ m_pPrj = pPrj; }
    void RemoveCurrentConnection ();
    void Find (const char *szBuffer);
	void Save ();
	void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw);
// Dialog Data
	//{{AFX_DATA(CCanOpenDigitalIO)
	enum { IDD = IDD_DLG_HW_CANOPEN_AXES };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCanOpenDigitalIO)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCanOpenDigitalIO)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CConfigDoc     *m_pConfigDoc;
	CProject	   *m_pPrj;
    CConfigHWGrid   m_GridAxes;
	CStatic			m_stDSP402Title;
	CButton			m_EnableDSP402;
	CStatic			m_stAxesTitle;
	CStatic			m_stVarControl;
	CStatic			m_stVarCommand;
    bool            m_bInitialized;
    int             m_nKinemaAddress;
    int             m_nNode;
	CFont			*m_pFont;
	CCanOpenAxesGridCtrl m_gridControlObjects;
	CCanOpenAxesGridCtrl m_gridCommandObjects;
	CImageList      m_ImageList;
    ePswLevel       m_ePswLevel;
private :
	void InitializeGrid (CConfigHWGrid& m_Grid, int nID, int nRow, int nMode);
    void InitializeGridData ();
    void InitializeTitles ();
	void AssignObjects (CGridCtrl *pGrid, int nRow, int nCol, int *value);
	void OnOK ();
	void OnCancel ();
	void CreateFont ();
	void InitData ();
	void InitializeCommandGridObjects (int nID);
	void InitializeControlGridObjects (int nID);
	void InitColumnGridCommandCaptions ();
	void InitColumnGridControlCaptions ();
	void SetDSP402Objects ();
	void LoadDSP402Objects (CAdamoHWConfigFile *pACF);
	void InitPsw ();
	afx_msg void OnRadio16bit();
	afx_msg void OnRadio32bit();
	afx_msg void OnCheckDSP402 ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANOPENDIGITALIO_H__9796E483_0E7C_4943_B8C7_CBA33F0DBE59__INCLUDED_)
