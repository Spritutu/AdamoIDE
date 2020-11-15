#pragma once

/////////////////////////////////////////////////////////////////////////////
// CDlgMbx dialog

class CDlgMbx : public CAdamoDialog
{
// Construction
public:
	CDlgMbx(CWnd* pParent = NULL);   // standard constructor
	~CDlgMbx ();
    void SetGL (int nGL)                               { m_nGL=nGL; }
    void SetLocalData (int nLevel, int nProg)          { m_nLevel=nLevel; m_nProg=nProg; }
// Dialog Data
	//{{AFX_DATA(CDlgMbx)
	enum { IDD = IDD_DLG_MB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgMbx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void OnTimer ()   {
	}
	// Generated message map functions
	//{{AFX_MSG(CDlgMbx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	CXTPReportControl m_wndReport;
	BYTE* m_pData;
    int	m_nGL;
    int	m_nLevel;
    int	m_nProg;
private :
	bool ReadData (COleVariant& v);
	int  LoadMbxData ();
    void OnOK ();
	void InitReportCtrl ();
	void LoadReportCtrl ();
};

