#if !defined(AFX_OSCCHANNELDATA_H__1141EEFA_7AA4_46C6_9200_73EDCADC6D1A__INCLUDED_)
#define AFX_OSCCHANNELDATA_H__1141EEFA_7AA4_46C6_9200_73EDCADC6D1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OscChannelData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COscChannelData window

class COscChannelData : public CStatic
{
    friend class CDlgOscilloscope;
// Construction
public:
	COscChannelData();

// Attributes
public:

// Operations
public:
    void SetChannelNumber (int nChannel)                    { m_nChannel=nChannel; }
	void SetName (CString strName)							{ m_strName = strName; }
    void SetType (const char* szType)                       { m_strType=szType; }
    void Select (bool bSelect = true)                       { m_bSelected=bSelect; }
    void SetAdamoDevice (CAdamoBase* pAD)                   { m_pAD=pAD; }
    void SetCurrentValue (const char* szCurrentValue);
	CString GetType ()										{ return m_strType; }
	CAdamoBase *GetAdamoDevice ()							{ return m_pAD; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COscChannelData)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COscChannelData();

	// Generated message map functions
protected:
	//{{AFX_MSG(COscChannelData)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    bool m_bSelected;
	CString m_strName;
    CString m_strType;
    CString m_strCurrentValue;
    int  m_nChannel;
    static DWORD m_colors[MAXSAMPLESNUMBER];
    CAdamoBase* m_pAD;
	CFont *m_pDataFont;
	CFont *m_pCaptionFont;
private :
    void DrawLittleBox (CPaintDC* pDC, int x, int y, int cx, int cy);
    void DrawFrame (CPaintDC* pDC);
    void DrawFrameSelected (CPaintDC* pDC);
    void DrawVariousText (CPaintDC* pDC);
    void DrawSymbol (CPaintDC* dc);
    void DrawCaption (CPaintDC* dc);
    void DrawBackGround (CPaintDC* pDC, bool bSelected);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OSCCHANNELDATA_H__1141EEFA_7AA4_46C6_9200_73EDCADC6D1A__INCLUDED_)
