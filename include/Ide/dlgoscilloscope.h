//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_DLGOSCILLOSCOPE_H__1751C5B8_5F7D_4DFC_9D75_E47324DC7630__INCLUDED_)
#define AFX_DLGOSCILLOSCOPE_H__1751C5B8_5F7D_4DFC_9D75_E47324DC7630__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgOscilloscope.h : header file
//

/* include files */
#include "Path.h"
#include "dlgsamples.h"
#include "dlgtriggers.h"
#include "OscChannelData.h"
#include "iknobx.h"
#include "xSkinButton.h"

/* namespace */
using namespace nsPath;

/* constanst definitions */
#define DIROSC "Osc"

/* forward reference */
class CAdamoMDOscBar;

#ifdef ADAMO_SIMBA
#import "D:\Sviluppo\Adamo\Adamobin\iPlotLibrary.tlb" named_guids
#else
#import "iPlotLibrary.tlb" named_guids
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOscilloscope dialog

struct stOscCanale  {
    struct stScale* pScale;
    int    nScale;    
};

class CDlgOscilloscope : public CDialog
{
// Construction
public:
	CDlgOscilloscope(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgOscilloscope)
	enum { IDD = IDD_DLGOSCILLOSCOPE };
	CStatic	m_ctrlOscStatus;
	COscChannelData	m_ctrlOscChannelData8;
	COscChannelData	m_ctrlOscChannelData7;
	COscChannelData	m_ctrlOscChannelData6;
	COscChannelData	m_ctrlOscChannelData5;
	COscChannelData	m_ctrlOscChannelData4;
	COscChannelData	m_ctrlOscChannelData3;
	COscChannelData	m_ctrlOscChannelData2;
	COscChannelData	m_ctrlOscChannelData1;
	CiKnobX	m_knobXAxis;
	CiKnobX	m_knobYAxis;
	CxSkinButton m_btnXPlus;
	CxSkinButton m_btnXMinus;
	CxSkinButton m_btnYPlus;
	CxSkinButton m_btnYMinus;
	CStatic m_stXValue;
	CStatic m_stYValue;
	CStatic m_stXUMM;
	CStatic m_stYUMM;
	//}}AFX_DATA
    static __inline CDlgOscilloscope* GetOscilloscope () { return m_pThis; }
    void SettaDlgTriggers (CDlgTriggers* pDlgTriggers)   { m_pDlgTriggers=pDlgTriggers; }
    void SettaDlgSamples  (CDlgSamples* pDlgSamples)     { m_pDlgSamples=pDlgSamples; }

    void OnAdamoTimer () ;
    void OnOscStart ();
    void OnOscStartPretrigger ();
    void OnOscStop ();
    void OnOscClear ();
	void OnOscZoomToFit ();
    void OnOscOptions ();
	void OnOscDeltaXCursor ();
	void OnOscDeltaYCursor ();
    void OnOscLoad ();
    void OnOscSave ();
    void OnOscCursor ();
    void InitOscilloscope ();
    void TraceFromAxSettings (CString strDevice);
    long GetStatus ()                                    { return m_nStatus; }    
    long GetWorkStatus ()                                { return m_nWorkStatus; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgOscilloscope)
	protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgOscilloscope)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnXMinus();
	afx_msg void OnXPlus();
	afx_msg void OnYMinus();
	afx_msg void OnYPlus();
	afx_msg void OnDataCursorChange(long Index);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPositionChangeIknobX();
	afx_msg void OnMouseUpIknobX(short Button, short Shift, long X, long Y);
	afx_msg void OnPositionChangeFinishedIknobX();
	afx_msg void OnPositionChangeIknobY();
	afx_msg void OnMouseUpIknobY(short Button, short Shift, long X, long Y);
	afx_msg void OnPositionChangeFinishedIknobY();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
    afx_msg LRESULT OnSelectOscChannel (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private :
    static CDlgOscilloscope* m_pThis;
    HWND        m_hWndOsc;
    IUnknown*   m_iUnknown;
    CComPtr<iPlotLibrary::IiPlotX> m_iPlotX1iDispatch;
    bool m_bInitialized;
    CPtrList        m_pTriggers;
    CPtrList        m_pSamples;
    CDlgSamples*    m_pDlgSamples;
    CDlgTriggers*   m_pDlgTriggers;
    CAdamoMachine*  m_pMachine;
    int             m_nFrequency;
    int             m_nBufferSize;  
    int             m_nContinue;
    int             m_nAndOr;
    char*           m_pBuf;
    bool            m_bOscStarted;
    int             m_nOscIndex;
    int             m_nIndexXScale;
    double          m_fpBaseSpan;
    int             m_nTimerCounter;
    double          m_fpScaleY;
    bool            m_bCleared;
    stOscCanale     m_OscCanali[MAXSAMPLESNUMBER];
    int             m_nCurSample;
    COscChannelData* m_pOscChannelData[MAXSAMPLESNUMBER];
    int             m_nOscCount;
    int             m_nCursorIndex;
	int				m_nCursorDeltaXIndex;
	int				m_nCursorDeltaYIndex;
    long            m_nStatus;
    int             m_nWorkStatus;
	CFont*			m_pFont;
private :
    int  SendTriggers ();
    int  SendSamples ();
    int  StartOscilloscope (int nMode);
    int  StopOscilloscope ();
    void CleanLists ();
    int  ParseData (char** p);
    int  ParseAxes (char* p, CAdamoOscDato* pOsc, int nChannel);
    int  ParseDevices (char* p, CAdamoOscDato* pOsc, int nChannel);
	int  ParseVariables (char* p, CAdamoOscDato* pOsc, int nChannel);
    void AddChannels ();
    void AddYAxes ();
    void InitIPlotX ();
    void AdjustXAxes ();
    void AdjustYAxes (int i);
    void AdjustAllYAxes ();
    void InitOscChannel (int n);
    void InitVettOscChannelData ();
    void ShowOscChannelData ();
    void SetCursorName (int nIndexChannel);
    void ResetCursorValues ();
    void OnOscStatus (const char* s);
    CPath CreateOscDir ();
    void SaveEpilogue (CPath strPath);
    int  GetDeviceName (int nDevice, int nTypeDevice, CString& strDevice);
    bool GetOscFile (CString strPathName, CPath& strOscFile);
    void InitializeTrace (int i, int nIndiceDiScala, double fpControlMin, double fpControlSpan);
    void GetTokensConf (char* p);
    void GetTokensTrigger (char *p, char* szTriggerName, int* nTipoDev, int* nDatoAsse, int* nCmp, int* nValue, double* fpValue);
    void GetTokensSample (char *p, char* szSampleName, int* nTipoDev, int* nDatoAsse, int* nIndiceDiScala, double* fpControlMin, double* fpControlSpan);
    void InitStrings ();
    void DestroyStrings ();
	void InitKnobs ();
	void InitLabels ();
	void InitButtons ();
    void OnOK ();
    void OnCancel ();
	void SetXLabel ();
	void SetYLabel ();
	CString GetUnitString (enDispositivi nDevice, int nType, enUnitaMisura nUnit);
	CString GetUnitSpace (enUnitaMisura nUnit);
	CString GetUnitTorque (enUnitaMisura nUnit);
	CString GetUnitSpeed (enUnitaMisura nUnit);
	CString GetUnitAcc (enUnitaMisura nUnit);
	CString GetUnitVolt (enUnitaMisura nUnit);
	CString GetUnitTimer (enUnitaMisura nUnit);
	void SetUnitLabel (CAdamoBase* ab, CString strType);
	void ZoomToFit ();
	void ShowDeltaXCursor ();
	void ShowDeltaYCursor ();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOSCILLOSCOPE_H__1751C5B8_5F7D_4DFC_9D75_E47324DC7630__INCLUDED_)
