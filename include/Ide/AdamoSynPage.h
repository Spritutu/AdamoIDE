#pragma once

/* include files */
#include "AdamoSynBaseCtrl.h"
#include "AdamoSynGrid.h"
#include "AdamoSynButton.h"
#include "AdamoSynCheckBox.h"
#include "AdamoSynGroupBox.h"
#include "AdamoSynRadioButton.h"
#include "AdamoSynLabel.h"
#include "AdamoSynEdit.h"
#include "AdamoSynListBox.h"
#include "AdamoSynComboBox.h"
#include "AdamoSynRTAx.h"
#include "AdamoSynImage.h"
#include "AdamoSynILed.h"
#include "AdamoSynIAngularGauge.h"
#include "AdamoSynILinearGauge.h"
#include "AdamoSynIDisplay.h"
#include "AdamoSynILabel.h"
#include "AdamoSynIPicture.h"

/* import directive */
#import <msxml6.dll> 
using namespace MSXML2;

/* type definition */
typedef MSXML2::IXMLDOMDocumentPtr		DocXmlPtr;
typedef MSXML2::IXMLDOMElementPtr		ElementXmlPtr;
typedef MSXML2::IXMLDOMNodePtr			NodeXmlPtr ;
typedef MSXML2::IXMLDOMNodeListPtr		NodeListXmlPtr;
typedef MSXML2::IXMLDOMNamedNodeMapPtr	NodeMapPtr;

/* forward reference */
class CAdamoSynContainer;

/* struct definitions */
class CAdamoSynPage : public CWnd
{
	DECLARE_DYNAMIC(CAdamoSynPage)
public:
	CAdamoSynPage();
	virtual ~CAdamoSynPage();
	int Load (CString strXML);
	void Resize ();
	void SetName (CString strName)		{ m_strName = strName; }
	CString GetName ()					{ return m_strName; }
	void OnLoad ();
	int  OnTimer ();
protected:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClick (UINT nID);
	afx_msg void OnSelChange (UINT nID);
	DECLARE_MESSAGE_MAP()
private :
	int LoadXMLCtrls (CString strXML);
	int ParseXMLString ();
	int CreateFormXML (ElementXmlPtr p);
	int CreateXMLControls (ElementXmlPtr p);
	int CreateXMLButton (ElementXmlPtr p);
	int CreateXMLGrid (ElementXmlPtr p);
	int CreateXMLInput (ElementXmlPtr p);
	int CreateXMLInputCheckBox (ElementXmlPtr p);
	int CreateXMLInputRadio (ElementXmlPtr p);
	int CreateXMLInputImage (ElementXmlPtr p);
	int CreateXMLGroupBox (ElementXmlPtr p);
	int CreateXMLLabel (ElementXmlPtr p);
	int CreateXMLInputText (ElementXmlPtr p);
	int CreateXMLComboList (ElementXmlPtr p);
	int CreateXMLCombo (ElementXmlPtr p);
	int CreateXMLList (ElementXmlPtr p);
	int CreateXMLAx (ElementXmlPtr p);
	int CreateXMLLed (ElementXmlPtr p);
	int CreateXMLAngularGauge (ElementXmlPtr p);
	int CreateXMLLinearGauge (ElementXmlPtr p);
	int CreateXMLDisplay (ElementXmlPtr p);
	int CreateXMLILabel (ElementXmlPtr p);
    int CreateXMLIPicture (ElementXmlPtr p);
	int LoadXMLOptions (ElementXmlPtr p, CAdamoSynCtrl *pCtrl, bool bIsCombo);
	void SetFormPosition (CString strFormPosition);
	void SetBackgroundImage (CString strImageName);
	int CreateWndCtrls ();
	void ClearCtrls ();
	void GestioneListBoxSelection (CAdamoSynCtrl* pCtrl);
	void GestioneComboBoxSelection (CAdamoSynCtrl* pCtrl);
	void OnEraseBackGround (CDC* pDC);
private :
	DocXmlPtr m_pDocXml;
	CPtrList  m_listCtrls;
	CString	  m_strName;
	CRect	  m_rc;
	bool      m_bBackgroundEnabled;
	CString   m_strImageName;
	CBitmap   m_hBckGround;
	DWORD     m_dwWidth;
	DWORD	  m_dwHeight;
	bool	  m_bKilling;
private :
	void AddCtrl (CAdamoSynCtrl* pCtrl);
	int  LoadXMLColumns (ElementXmlPtr p, CAdamoSynGrid *pCtrl);
	int  LoadXMLVariable (ElementXmlPtr p, CAdamoRSVarControl *pCtrl);
	int  CreateBackground ();
	int  GestioneButtonClick (CAdamoSynCtrl *pCtrl);
	int  GestioneCheckBoxClick (CAdamoSynCtrl *pCtrl);
	int  GestioneRadioButtonClick (CAdamoSynCtrl *pCtrl);
	void OnChangePage (CString strPage);
	void OnDownload ();
	void UnCheckRadioButton (CAdamoSynCtrl *pRadioChecked);
};


