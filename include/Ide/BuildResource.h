#if !defined(AFX_BUILDRESOURCE_H__C3C6C906_45C3_4A99_B419_845E4FC5F86C__INCLUDED_)
#define AFX_BUILDRESOURCE_H__C3C6C906_45C3_4A99_B419_845E4FC5F86C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BuildResource.h : header file
//

/* include files */
#include "AdamoRSEditor.h"

/////////////////////////////////////////////////////////////////////////////
// CBuildResource view

class CBuildResource : public CView
{
protected:
	CBuildResource();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBuildResource)

// Attributes
public:

// Operations
public:
    void AddRSResource ();
    void AddRSFolder ();
    void EditRSResource ();
    void DeleteRSResource ();
    void RenameRSResource (CString strPrev, CString strNew, eResources e);
    void UpdateDiagramEntitySize ();
    void UpdateDiagramSize ();
    void UpdateDiagramValue ();
    void UpdateDiagramSelected ();
    void GetHTMLString (CString& strHTML);
	void GetXMLString (CString& strXML);
    void CloseTabMode ();
    CAdamoRSEditor *GetRSEditor ()  { return m_pEditor; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBuildResource)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBuildResource();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CBuildResource)
	afx_msg void OnRSAddResource();
	afx_msg void OnUpdateRSAddResource(CCmdUI* pCmdUI);
	afx_msg void OnRSEditResource();
	afx_msg void OnUpdateRSEditResource(CCmdUI* pCmdUI);
	afx_msg void OnRSDeleteResource();
	afx_msg void OnUpdateRSDeleteResource(CCmdUI* pCmdUI);
	afx_msg void OnRSCloseView();
	afx_msg void OnUpdateRSCloseView(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInserisciModuloPulsanteDiComando();
	afx_msg void OnUpdateInserisciModuloPulsanteDiComando(CCmdUI* pCmdUI);
	afx_msg void OnEditaCopy();
	afx_msg void OnUpdateEditaCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditaCut();
	afx_msg void OnUpdateEditaCut(CCmdUI* pCmdUI);
	afx_msg void OnEditaDelete();
	afx_msg void OnUpdateEditaDelete(CCmdUI* pCmdUI);
	afx_msg void OnEditaPaste();
	afx_msg void OnUpdateEditaPaste(CCmdUI* pCmdUI);
	afx_msg void OnStrumentiGriglia();
	afx_msg void OnUpdateStrumentiGriglia(CCmdUI* pCmdUI);
	afx_msg void OnStrumentiMargini();
	afx_msg void OnUpdateStrumentiMargini(CCmdUI* pCmdUI);
	afx_msg void OnStrumentiAllineaAiMargini();
	afx_msg void OnUpdateStrumentiAllineaAiMargini(CCmdUI* pCmdUI);
	afx_msg void OnStrumentiAllineaAllaGriglia();
	afx_msg void OnUpdateStrumentiAllineaAllaGriglia(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloCaselladiTesto();
	afx_msg void OnUpdateInserisciModuloCaselladiTesto(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloPulsantediOpzione();
	afx_msg void OnUpdateInserisciModuloPulsantediOpzione(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloCaselladiControllo();
	afx_msg void OnUpdateInserisciModuloCaselladiControllo(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloCaselladiGruppo();
	afx_msg void OnUpdateInserisciModuloCaselladiGruppo(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloLabel();
	afx_msg void OnUpdateInserisciModuloLabel(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloCaselladiRiepilogo();
	afx_msg void OnUpdateInserisciModuloCaselladiRiepilogo(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloCaselladiRiepilogoaDiscesa();
	afx_msg void OnUpdateInserisciModuloCaselladiRiepilogoaDiscesa(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloGrid();
	afx_msg void OnUpdateInserisciModuloGrid(CCmdUI* pCmdUI);
	afx_msg void OnStrumentiOrdineDiTabulazione();
	afx_msg void OnUpdateStrumentiOrdineDiTabulazione(CCmdUI* pCmdUI);
	afx_msg void OnStrumentiMultidraw();
	afx_msg void OnUpdateStrumentiMultidraw(CCmdUI* pCmdUI);
	afx_msg void OnFormatoAllineaASinistra();
	afx_msg void OnFormatoAllineaADestra();
	afx_msg void OnFormatoAllineaAlBasso();
	afx_msg void OnFormatoAllineaAllAlto();
	afx_msg void OnUpdateFormatoAllineaADestra(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatoAllineaAlBasso(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatoAllineaAllAlto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFormatoAllineaASinistra(CCmdUI* pCmdUI);
	afx_msg void OnFormatoRendiugualeEntrambeLeDimensioni();
	afx_msg void OnUpdateFormatoRendiugualeEntrambeLeDimensioni(CCmdUI* pCmdUI);
	afx_msg void OnFormatoRendiUgualeAltezza();
	afx_msg void OnUpdateFormatoRendiUgualeAltezza(CCmdUI* pCmdUI);
	afx_msg void OnFormatoRendiUgualeLunghezza();
	afx_msg void OnUpdateFormatoRendiUgualeLunghezza(CCmdUI* pCmdUI);
	afx_msg void OnFormatoAllineaAlPianoOrizzontale();
	afx_msg void OnUpdateFormatoAllineaAlPianoOrizzontale(CCmdUI* pCmdUI);
	afx_msg void OnFormatoAllineaAllAsseVerticale();
	afx_msg void OnUpdateFormatoAllineaAllAsseVerticale(CCmdUI* pCmdUI);
	afx_msg void OnFormatoCentraNellaFinestraOrizzontale();
	afx_msg void OnUpdateFormatoCentraNellaFinestraOrizzontale(CCmdUI* pCmdUI);
	afx_msg void OnFormatoCentraNellaFinestraVerticale();
	afx_msg void OnUpdateFormatoCentraNellaFinestraVerticale(CCmdUI* pCmdUI);
	afx_msg void OnFormatoDisponiPulsantiDestra();
	afx_msg void OnUpdateFormatoDisponiPulsantiDestra(CCmdUI* pCmdUI);
	afx_msg void OnFormatoDisponiPulsantiInBasso();
	afx_msg void OnUpdateFormatoDisponiPulsantiInBasso(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloImmagine();
	afx_msg void OnUpdateInserisciModuloImmagine(CCmdUI* pCmdUI);
	afx_msg void OnStrumentiSincronizzaVariabili();
	afx_msg void OnInserisciModuloAsse();
	afx_msg void OnUpdateInserisciModuloAsse(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloAngularGauge();
	afx_msg void OnUpdateInserisciModuloIAngularGauge(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloILinearGauge();
	afx_msg void OnUpdateInserisciModuloILinearGauge(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloILed();
	afx_msg void OnUpdateInserisciModuloILed(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloISevenSegment();
	afx_msg void OnUpdateInserisciModuloISevenSegment(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloILabel();
	afx_msg void OnUpdateInserisciModuloILabel(CCmdUI* pCmdUI);
	afx_msg void OnInserisciModuloIPicture();
	afx_msg void OnUpdateInserisciModuloIPicture(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
    CAdamoRSDoc*        m_pDoc;
    CAdamoRSEditor*     m_pEditor;
private :
    void CreateRSTemplate ();
    void CloseRSTemplate ();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUILDRESOURCE_H__C3C6C906_45C3_4A99_B419_845E4FC5F86C__INCLUDED_)
