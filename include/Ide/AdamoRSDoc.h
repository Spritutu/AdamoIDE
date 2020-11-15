#if !defined(AFX_ADAMORSDOC_H__B054FEFD_4E46_439B_9689_F058899428B4__INCLUDED_)
#define AFX_ADAMORSDOC_H__B054FEFD_4E46_439B_9689_F058899428B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdamoRSDoc.h : header file
//

/* include files */
#include "ResourceFile.h"
#include "AdamoRSObjects.h"
#include "AdamoRSEditor.h"
#include "ximage.h"

/* forward reference */
class CAdamoRSDoc;

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSAdamoCtrlList document
class CAdamoRSThiraCtrlList : public CAdamoRSCtrlList
{
    public :
        CAdamoRSThiraCtrlList () : m_pDoc (NULL), m_pEditor (NULL)  { }
        void       SetAdamoRSDoc (CAdamoRSDoc *pDoc)   { m_pDoc=pDoc; }
        void       SetEditor (CAdamoRSEditor *pEditor) { m_pEditor=pEditor; }
        eResources GetResourceType ();
        CString    GetResourceName ();
        CSize      GetFormSize     ();
		void       SaveSynCtrls (CString strResourceName, CResourceFile *pRF);
    private :
        CAdamoRSDoc     *m_pDoc;
        CAdamoRSEditor  *m_pEditor;
	private :
		void DelExistingRecords (CADORecordset *pRS, CString strResourceName);
};

/////////////////////////////////////////////////////////////////////////////
// CAdamoRSDoc document
class CAdamoRSDoc : public CDocument
{
    friend CAdamoRSThiraCtrlList;
protected:
	CAdamoRSDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdamoRSDoc)

// Attributes
public:

// Operations
public:
    int AddEmptyRecord (CString str, eResources e);
    int DeleteRecord (CString str, eResources e);
    int RenameRecord (CString strPrev, CString strNew, eResources e);
    int LoadXMLRecord (CString str, eResources e);
    int SaveXMLRecord ();
    CString    GetCurResource ()                { return m_strDoc; }
    CAdamoRSCtrlList* GetCtrlList ()			{ return &m_cl; }
    int IsCurrentResource (CString str, eResources e);
    CAdamoRSContainer* GetRSContainer ()		{ return (CAdamoRSContainer*) &m_ct; }
    void ClearDiagramEntities ()                { m_ct.Clear (); m_ct.ClearSelection (); }
    void UpdateDiagramEntitySize ();
    void UpdateDiagramEntityValue ();
	void GetCurrentXMLRecord (CString& strXML);
    void CancellaMessaggiTradotti (CString strResource, eResources e);
    void CancellaMessaggiTradotti ();
    bool IsDifferentResource (CString strResource, eResources e);
    bool EsisteImmagine (CString strID);
    bool AggiungiImmagine (CString strID, char* p, DWORD nLength);
    bool CancellaImmagine (CString strID);
    DWORD LunghezzaImmagine (CString strID);
    bool CaricaImmagine (CString strID, char* p);
    CxImage* GetImage ()                    { return m_imgJPG; }
    void     SetImage (CxImage* p)          { _delete (m_imgJPG); m_imgJPG=p; }
    CResourceFile *GetRSFile ()             { return &m_rf; }
    int GetFirstRSImage (CString& str)      { return m_rf.GetFirstRSImage (str); }
    int GetNextRSImage  (CString& str)      { return m_rf.GetNextRSImage (str); }
    CxImage* CaricaCxImage (CString strID);
    bool RinominaImmagine (CString strImage, CString strID);
    int GetTableList  (CStringList& slTables)                       { return m_rf.GetTableList (slTables); }
    int GetFieldsList (CString strTable, CStringList& slFields)     { return m_rf.GetFieldsList (strTable, slFields); }
    int GetIndexList  (CString strTable, CStringList& slIndex)      { return m_rf.GetIndexList  (strTable, slIndex); }
    int GetSynList  (CStringList& slSyn)                            { return m_rf.GetSynList (slSyn); }
    int CancellaValore (stRSVariabili& v)                           { return m_rf.CancellaValore (v.m_strName); }
    int SincronizzaVariabili ();
    CString RitornaTraduzioneRecord (CString str, eResources e);
    CString GetRSTranslated (CString str, eResources e);
    // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdamoRSDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdamoRSDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdamoRSDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CResourceFile           m_rf;
    CAdamoRSThiraCtrlList   m_cl;
    CString                 m_strDoc;
    eResources              m_eDoc;
    CAdamoRSContainer       m_ct;
    CxImage*                m_imgJPG;
private :
    int PopulateRSTree ();
    CString GetRootString (eResources e);
    int AggiungiTraduzioneRecord (CString str, eResources e);
    int RinominaTraduzioneRecord (CString strPrev, CString strNew, eResources e);
    int CancellaTraduzioneRecord (CString strResource, eResources e);
	void RinominaDocking (CString strNew);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADAMORSDOC_H__B054FEFD_4E46_439B_9689_F058899428B4__INCLUDED_)
