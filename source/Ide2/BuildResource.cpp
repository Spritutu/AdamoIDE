// BuildResource.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "AdamoRSCatalogue.h"
#include "BuildResource.h"
#include "AdamoRSObjects.h"
#include "AdamoRSDiagramObjects.h"
#include "AdamoSelectDevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBuildResource

IMPLEMENT_DYNCREATE(CBuildResource, CView)

CBuildResource::CBuildResource() : m_pDoc (NULL), m_pEditor (NULL)
{
}

CBuildResource::~CBuildResource()
{
    _delete (m_pEditor);
}



BEGIN_MESSAGE_MAP(CBuildResource, CView)
	//{{AFX_MSG_MAP(CBuildResource)
    ON_COMMAND(ID_INSERISCI_RISORSA, OnRSAddResource)
    ON_UPDATE_COMMAND_UI(ID_INSERISCI_RISORSA, OnUpdateRSAddResource)
    ON_COMMAND(ID_EDITA_EDITARISORSA, OnRSEditResource)
    ON_UPDATE_COMMAND_UI(ID_EDITA_EDITARISORSA, OnUpdateRSEditResource)
    ON_COMMAND(ID_EDITA_CANCELLARISORSA, OnRSDeleteResource)
    ON_UPDATE_COMMAND_UI(ID_EDITA_CANCELLARISORSA, OnUpdateRSDeleteResource)
    ON_COMMAND(ID_EDITA_CHIUDIVISTA, OnRSCloseView)
    ON_UPDATE_COMMAND_UI(ID_EDITA_CHIUDIVISTA, OnUpdateRSCloseView)
	ON_WM_SIZE()
	ON_COMMAND(ID_INSERISCI_MODULO_PULSANTEDICOMANDO, OnInserisciModuloPulsanteDiComando)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_PULSANTEDICOMANDO, OnUpdateInserisciModuloPulsanteDiComando)
	ON_COMMAND(ID_EDITA_COPY, OnEditaCopy)
	ON_UPDATE_COMMAND_UI(ID_EDITA_COPY, OnUpdateEditaCopy)
	ON_COMMAND(ID_EDITA_CUT, OnEditaCut)
	ON_UPDATE_COMMAND_UI(ID_EDITA_CUT, OnUpdateEditaCut)
	ON_COMMAND(ID_EDITA_DELETE, OnEditaDelete)
	ON_UPDATE_COMMAND_UI(ID_EDITA_DELETE, OnUpdateEditaDelete)
	ON_COMMAND(ID_EDITA_PASTE, OnEditaPaste)
	ON_UPDATE_COMMAND_UI(ID_EDITA_PASTE, OnUpdateEditaPaste)
	ON_COMMAND(ID_STRUMENTI_GRIGLIA, OnStrumentiGriglia)
	ON_UPDATE_COMMAND_UI(ID_STRUMENTI_GRIGLIA, OnUpdateStrumentiGriglia)
	ON_COMMAND(ID_STRUMENTI_MARGINI, OnStrumentiMargini)
	ON_UPDATE_COMMAND_UI(ID_STRUMENTI_MARGINI, OnUpdateStrumentiMargini)
	ON_COMMAND(ID_STRUMENTI_ALLINEAAIMARGINI, OnStrumentiAllineaAiMargini)
	ON_UPDATE_COMMAND_UI(ID_STRUMENTI_ALLINEAAIMARGINI, OnUpdateStrumentiAllineaAiMargini)
	ON_COMMAND(ID_STRUMENTI_ALLINEAALLAGRIGLIA, OnStrumentiAllineaAllaGriglia)
	ON_UPDATE_COMMAND_UI(ID_STRUMENTI_ALLINEAALLAGRIGLIA, OnUpdateStrumentiAllineaAllaGriglia)
	ON_COMMAND(ID_INSERISCI_MODULO_CASELLADITESTO, OnInserisciModuloCaselladiTesto)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_CASELLADITESTO, OnUpdateInserisciModuloCaselladiTesto)
	ON_COMMAND(ID_INSERISCI_MODULO_PULSANTEDIOPZIONE, OnInserisciModuloPulsantediOpzione)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_PULSANTEDIOPZIONE, OnUpdateInserisciModuloPulsantediOpzione)
	ON_COMMAND(ID_INSERISCI_MODULO_CASELLADICONTROLLO, OnInserisciModuloCaselladiControllo)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_CASELLADICONTROLLO, OnUpdateInserisciModuloCaselladiControllo)
	ON_COMMAND(ID_INSERISCI_MODULO_CASELLADIGRUPPO, OnInserisciModuloCaselladiGruppo)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_CASELLADIGRUPPO, OnUpdateInserisciModuloCaselladiGruppo)
	ON_COMMAND(ID_INSERISCI_MODULO_LABEL, OnInserisciModuloLabel)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_LABEL, OnUpdateInserisciModuloLabel)
	ON_COMMAND(ID_INSERISCI_MODULO_CASELLADIRIEPILOGO, OnInserisciModuloCaselladiRiepilogo)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_CASELLADIRIEPILOGO, OnUpdateInserisciModuloCaselladiRiepilogo)
	ON_COMMAND(ID_INSERISCI_MODULO_CASELLADIRIEPILOGOADISCESA, OnInserisciModuloCaselladiRiepilogoaDiscesa)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_CASELLADIRIEPILOGOADISCESA, OnUpdateInserisciModuloCaselladiRiepilogoaDiscesa)
	ON_COMMAND(ID_INSERISCI_MODULO_GRIGLIADIINPUT, OnInserisciModuloGrid)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_GRIGLIADIINPUT, OnUpdateInserisciModuloGrid)
	ON_COMMAND(ID_STRUMENTI_ORDINEDITABULAZIONE, OnStrumentiOrdineDiTabulazione)
	ON_UPDATE_COMMAND_UI(ID_STRUMENTI_ORDINEDITABULAZIONE, OnUpdateStrumentiOrdineDiTabulazione)
	ON_COMMAND(ID_STRUMENTI_MULTIDRAW, OnStrumentiMultidraw)
	ON_UPDATE_COMMAND_UI(ID_STRUMENTI_MULTIDRAW, OnUpdateStrumentiMultidraw)
	ON_COMMAND(ID_FORMATO_ALLINEA_ASINISTRA, OnFormatoAllineaASinistra)
	ON_COMMAND(ID_FORMATO_ALLINEA_ADESTRA, OnFormatoAllineaADestra)
	ON_COMMAND(ID_FORMATO_ALLINEA_ALBASSO, OnFormatoAllineaAlBasso)
	ON_COMMAND(ID_FORMATO_ALLINEA_ALLALTO, OnFormatoAllineaAllAlto)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_ALLINEA_ADESTRA, OnUpdateFormatoAllineaADestra)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_ALLINEA_ALBASSO, OnUpdateFormatoAllineaAlBasso)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_ALLINEA_ALLALTO, OnUpdateFormatoAllineaAllAlto)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_ALLINEA_ASINISTRA, OnUpdateFormatoAllineaASinistra)
	ON_COMMAND(ID_FORMATO_RENDIUGUALE_ENTRAMBELEDIMENSIONI, OnFormatoRendiugualeEntrambeLeDimensioni)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_RENDIUGUALE_ENTRAMBELEDIMENSIONI, OnUpdateFormatoRendiugualeEntrambeLeDimensioni)
	ON_COMMAND(ID_FORMATO_RENDIUGUALE_ALTEZZA, OnFormatoRendiUgualeAltezza)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_RENDIUGUALE_ALTEZZA, OnUpdateFormatoRendiUgualeAltezza)
	ON_COMMAND(ID_FORMATO_RENDIUGUALE_LUNGHEZZA, OnFormatoRendiUgualeLunghezza)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_RENDIUGUALE_LUNGHEZZA, OnUpdateFormatoRendiUgualeLunghezza)
	ON_COMMAND(ID_FORMATO_ALLINEA_ALPIAMOORIZZONTALE, OnFormatoAllineaAlPianoOrizzontale)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_ALLINEA_ALPIAMOORIZZONTALE, OnUpdateFormatoAllineaAlPianoOrizzontale)
	ON_COMMAND(ID_FORMATO_ALLINEA_ALLASSEVERTICALE, OnFormatoAllineaAllAsseVerticale)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_ALLINEA_ALLASSEVERTICALE, OnUpdateFormatoAllineaAllAsseVerticale)
	ON_COMMAND(ID_FORMATO_CENTRANELLAFINESTRA_ORIZZONTALE, OnFormatoCentraNellaFinestraOrizzontale)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_CENTRANELLAFINESTRA_ORIZZONTALE, OnUpdateFormatoCentraNellaFinestraOrizzontale)
	ON_COMMAND(ID_FORMATO_CENTRANELLAFINESTRA_VERTICALE, OnFormatoCentraNellaFinestraVerticale)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_CENTRANELLAFINESTRA_VERTICALE, OnUpdateFormatoCentraNellaFinestraVerticale)
	ON_COMMAND(ID_FORMATO_DISPONIPULSANTI_DESTRA, OnFormatoDisponiPulsantiDestra)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_DISPONIPULSANTI_DESTRA, OnUpdateFormatoDisponiPulsantiDestra)
	ON_COMMAND(ID_FORMATO_DISPONIPULSANTI_INBASSO, OnFormatoDisponiPulsantiInBasso)
	ON_UPDATE_COMMAND_UI(ID_FORMATO_DISPONIPULSANTI_INBASSO, OnUpdateFormatoDisponiPulsantiInBasso)
	ON_COMMAND(ID_INSERISCI_MODULO_IMMAGINE, OnInserisciModuloImmagine)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_IMMAGINE, OnUpdateInserisciModuloImmagine)
	ON_COMMAND(ID_STRUMENTI_SINCRONIZZA_VARIABILI, OnStrumentiSincronizzaVariabili)
	ON_COMMAND(ID_INSERISCI_MODULO_ASSE, OnInserisciModuloAsse)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_ASSE, OnUpdateInserisciModuloAsse)
	ON_COMMAND(ID_INSERISCI_MODULO_IANGULAR_GAUGE, OnInserisciModuloAngularGauge)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_IANGULAR_GAUGE, OnUpdateInserisciModuloIAngularGauge)
	ON_COMMAND(ID_INSERISCI_MODULO_ILINEAR_GAUGE, OnInserisciModuloILinearGauge)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_ILINEAR_GAUGE, OnUpdateInserisciModuloILinearGauge)
	ON_COMMAND(ID_INSERISCI_MODULO_ILED, OnInserisciModuloILed)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_ILED, OnUpdateInserisciModuloILed)
	ON_COMMAND(ID_INSERISCI_MODULO_ISEVEN_SEGMENT, OnInserisciModuloISevenSegment)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_ISEVEN_SEGMENT, OnUpdateInserisciModuloISevenSegment)
	ON_COMMAND(ID_INSERISCI_MODULO_ILABEL, OnInserisciModuloILabel)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_ILABEL, OnUpdateInserisciModuloILabel)
	ON_COMMAND(ID_INSERISCI_MODULO_IPICTURE, OnInserisciModuloIPicture)
	ON_UPDATE_COMMAND_UI(ID_INSERISCI_MODULO_IPICTURE, OnUpdateInserisciModuloIPicture)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBuildResource drawing

void CBuildResource::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CBuildResource diagnostics

#ifdef _DEBUG
void CBuildResource::AssertValid() const
{
	CView::AssertValid();
}

void CBuildResource::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/*
** AddRSResource :
*/
void CBuildResource::AddRSResource ()
{
    CString str;
    eResources e;

    /* andiamo a prendere la docking del catalogo */
    CAdamoRSCatalogue* rsCat=((CMainFrame*)AfxGetMainWnd ())->GetDockRSCatalogue ()->GetView ();
    if (rsCat)   {
        /* prendiamo un nome valido, dobbiamo chiederlo al tree */
        if (!rsCat->GetValidName (str, &e))   {
            if (!m_pDoc->AddEmptyRecord (str, e))   {
                /* aggiungiamolo al nodo corrente */
                rsCat->AddCatalogueElement (str, "", e, RS_RESOURCE);
            }
        }
    }
}

/*
** EditRSResource :
*/
void CBuildResource::EditRSResource ()
{
    CAdamoRSCatalogue* rsCat=((CMainFrame*)AfxGetMainWnd ())->GetDockRSCatalogue ()->GetView ();
    CString str;
    eResources e;

    if (rsCat)   {
        if (rsCat->IsAValidResource ())   {
            if (!rsCat->GetSelectedName (str, &e))   {
                if (m_pDoc->IsDifferentResource (str, e))   {
                    CloseRSTemplate ();
                    if (!m_pDoc->LoadXMLRecord (str, e))
                        CreateRSTemplate ();
                }
            }
        }
    }
}

/*
** DeleteRSResource :
*/
void CBuildResource::DeleteRSResource ()
{
    CString str;
    eResources e;
    /* andiamo a prendere la docking del catalogo */
    CAdamoRSCatalogue* rsCat=((CMainFrame*)AfxGetMainWnd ())->GetDockRSCatalogue ()->GetView ();
    if (rsCat)   {
        /* prendiamo un nome valido, dobbiamo chiederlo al tree */
        if (rsCat->GetDeleteName (str, &e))   {
            /* cancelliamolo dall'albero */
            rsCat->RemoveCatalogueElement ();
            /* cancelliamolo dalla base dati */
            m_pDoc->DeleteRecord (str, e);
            /* se il record che intendiamo cancellare e' quello corrente, chiudiamo anche la vista */
            if (m_pDoc->IsCurrentResource (str, e))   {
                CloseRSTemplate ();    
            }
        }
    }
}

/*
** RenameRSResource :
*/
void CBuildResource::RenameRSResource (CString strPrev, CString strNew, eResources e)
{
    /* cancelliamolo dalla base dati */
    m_pDoc->RenameRecord (strPrev, strNew, e);
}

/////////////////////////////////////////////////////////////////////////////
// CBuildResource message handlers
/*
** OnRSAddResource :
*/
void CBuildResource::OnRSAddResource()
{
    AddRSResource ();
}

/*
** OnRSAddResource :
*/
void CBuildResource::OnUpdateRSAddResource(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ();
}

/*
** OnRSAddResource :
*/
void CBuildResource::OnRSDeleteResource()
{
    DeleteRSResource ();
}

/*
** OnRSAddResource :
*/
void CBuildResource::OnUpdateRSDeleteResource(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ();
}

/*
** OnRSCloseView :
*/
void CBuildResource::OnRSCloseView ()
{
    CloseRSTemplate ();
}

/*
** OnUpdateRSCloseView :
*/
void CBuildResource::OnUpdateRSCloseView (CCmdUI* pCmdUI)
{
    if (m_pEditor)
        pCmdUI->Enable ();
}

/*
** OnRSEditResource :
*/
void CBuildResource::OnRSEditResource()
{
    EditRSResource ();
}

/*
** OnUpdateRSEditResource :
*/
void CBuildResource::OnUpdateRSEditResource(CCmdUI* pCmdUI)
{
    pCmdUI->Enable ();
}

void CBuildResource::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
    CAdamoTemplate* pTmpl=((CIdeApp*)AfxGetApp())->GetTemplate (enResourcesTemplate);
    m_pDoc=(CAdamoRSDoc*)pTmpl->GetDocument ("");
}

/*
** CreateRSTemplate :
*/
void CBuildResource::CreateRSTemplate ()
{
    /* recuperiamo prima le dimensioni del form */
    CAdamoRSThiraCtrlList* pCtrlList = (CAdamoRSThiraCtrlList *)m_pDoc->GetCtrlList ();
    if (pCtrlList)   {
        CAdamoRSForm* pForm=pCtrlList->GetFormCtrl ();
        if (pForm)   {
            CRect rc=pForm->GetPosition (), rcClient;
            GetClientRect (rcClient);
            if (!m_pEditor)   {
                m_pEditor = new CAdamoRSEditor;
	            m_pEditor->Create( WS_CHILD | WS_VISIBLE, rcClient, this, m_pDoc->GetRSContainer() );
	            m_pEditor->ShowGrid( FALSE );
	            m_pEditor->SetGridColor( ::GetSysColor( COLOR_3DDKSHADOW ) );
	            m_pEditor->SetVirtualSize( CSize( rc.Width(), rc.Height()) );
	            m_pEditor->SetResize( TRUE );
	            m_pEditor->SetModified( FALSE );
                pCtrlList->SetEditor (m_pEditor);
            }
        }
    }
}

/*
** CloseRSTemplate :
*/
void CBuildResource::CloseRSTemplate ()
{
    if (m_pEditor)   {
        CloseTabMode ();
        m_pDoc->SaveXMLRecord ();
        m_pDoc->ClearDiagramEntities ();
        CAdamoRSProperties* pRSProp=((CMainFrame*)AfxGetMainWnd ())->GetDockRSProperties()->GetView ();
        if (pRSProp)
            pRSProp->Clear ();    
        m_pEditor->Clear ();
        m_pEditor->DestroyWindow ();
        _delete (m_pEditor);
    }
}

/*
** OnSize :
*/
void CBuildResource::OnSize (UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	if (m_pEditor)
		m_pEditor->MoveWindow(0, 0, cx, cy);
	
}

void CBuildResource::OnInserisciModuloPulsanteDiComando() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramButton* obj = new CAdamoRSDiagramButton;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

void CBuildResource::OnUpdateInserisciModuloPulsanteDiComando(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

void CBuildResource::OnEditaCopy() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
	    m_pEditor->Copy();
}

void CBuildResource::OnUpdateEditaCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
        m_pEditor->UpdateCopy( pCmdUI );	
}

void CBuildResource::OnEditaCut() 
{
	// TODO: Add your command handler code here
	m_pEditor->Cut();
}

void CBuildResource::OnUpdateEditaCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
        m_pEditor->UpdateCut( pCmdUI );	
}

void CBuildResource::OnEditaDelete() 
{
	// TODO: Add your command handler code here
	m_pEditor->DeleteAllSelected();
}

void CBuildResource::OnUpdateEditaDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    if (m_pEditor->IsAnyObjectSelected())
            pCmdUI->Enable ();
}

void CBuildResource::OnEditaPaste() 
{
	// TODO: Add your command handler code here
    m_pEditor->Paste();
}

void CBuildResource::OnUpdateEditaPaste(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
        m_pEditor->UpdatePaste( pCmdUI );	
}

void CBuildResource::UpdateDiagramEntitySize ()
{
    if (m_pDoc)
        m_pDoc->UpdateDiagramEntitySize ();
}

void CBuildResource::UpdateDiagramSize ()
{
    if (m_pDoc&&m_pEditor)   {
        CAdamoRSCtrlList* pCtrlList=m_pDoc->GetCtrlList ();
        if (pCtrlList)   {
            CAdamoRSForm* pForm=pCtrlList->GetFormCtrl ();
            if (pForm)   {
                CSize size;
                CRect rc=pForm->GetPosition ();
                size.cx=rc.right;
                size.cy=rc.bottom;
                m_pEditor->SetVirtualSize (size);
            }
        }
    }
}

/*
** UpdateDiagramValue :
*/
void CBuildResource::UpdateDiagramValue ()
{
    if (m_pDoc)
        m_pDoc->UpdateDiagramEntityValue ();
}

/*
** UpdateDiagramSelected :
*/
void CBuildResource::UpdateDiagramSelected ()
{
    CRect rc;
    CDiagramEntity* pDE=m_pEditor->GetSelectedObject ();
    if (pDE)
        m_pEditor->InvalidateRect (pDE->GetRect ());
}

void CBuildResource::OnStrumentiGriglia() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)   {
	    m_pEditor->ShowGrid (!m_pEditor->IsGridVisible ());
    }
}

void CBuildResource::OnUpdateStrumentiGriglia(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)   {
        pCmdUI->Enable (!m_pEditor->IsTabVisible ());    
	    pCmdUI->SetCheck (m_pEditor->IsGridVisible ());
    }
}

void CBuildResource::OnStrumentiMargini() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
	    m_pEditor->ShowMargin (!m_pEditor->IsMarginVisible());
}

void CBuildResource::OnUpdateStrumentiMargini (CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)   {
        pCmdUI->Enable (!m_pEditor->IsTabVisible ());    
	    pCmdUI->SetCheck (m_pEditor->IsMarginVisible());
    }
}

void CBuildResource::OnStrumentiAllineaAiMargini() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
	    if (m_pEditor->GetRestraints() == RESTRAINT_MARGIN)
		    m_pEditor->SetRestraints (RESTRAINT_NONE);
	    else
		    m_pEditor->SetRestraints (RESTRAINT_MARGIN);
}

void CBuildResource::OnUpdateStrumentiAllineaAiMargini(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)   {
        pCmdUI->Enable (!m_pEditor->IsTabVisible ());    
        pCmdUI->SetCheck (m_pEditor->GetRestraints() == RESTRAINT_MARGIN);	
    }
}

void CBuildResource::OnStrumentiAllineaAllaGriglia() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->SetSnapToGrid (!m_pEditor->GetSnapToGrid());
}

void CBuildResource::OnUpdateStrumentiAllineaAllaGriglia(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)   {
        pCmdUI->Enable (!m_pEditor->IsTabVisible ());    
        pCmdUI->SetCheck (m_pEditor->GetSnapToGrid ());	
    }
}

void CBuildResource::OnInserisciModuloCaselladiTesto() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramEdit* obj = new CAdamoRSDiagramEdit;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

void CBuildResource::OnUpdateInserisciModuloCaselladiTesto(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

void CBuildResource::OnInserisciModuloPulsantediOpzione() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramRadioButton* obj = new CAdamoRSDiagramRadioButton;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

void CBuildResource::OnUpdateInserisciModuloPulsantediOpzione(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

void CBuildResource::OnInserisciModuloCaselladiControllo() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramCheckBox* obj = new CAdamoRSDiagramCheckBox;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

void CBuildResource::OnUpdateInserisciModuloCaselladiControllo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

void CBuildResource::OnInserisciModuloCaselladiGruppo() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramGroupBox* obj = new CAdamoRSDiagramGroupBox;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

void CBuildResource::OnUpdateInserisciModuloCaselladiGruppo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

void CBuildResource::OnInserisciModuloLabel() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramLabel* obj = new CAdamoRSDiagramLabel;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

void CBuildResource::OnUpdateInserisciModuloLabel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

void CBuildResource::OnInserisciModuloGrid() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramGrid* obj = new CAdamoRSDiagramGrid;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

void CBuildResource::OnUpdateInserisciModuloGrid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

void CBuildResource::OnInserisciModuloCaselladiRiepilogo() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramList* obj = new CAdamoRSDiagramList;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

void CBuildResource::OnUpdateInserisciModuloCaselladiRiepilogo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

void CBuildResource::OnInserisciModuloCaselladiRiepilogoaDiscesa() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramCombo* obj = new CAdamoRSDiagramCombo;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

void CBuildResource::OnUpdateInserisciModuloCaselladiRiepilogoaDiscesa(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

void CBuildResource::OnStrumentiOrdineDiTabulazione() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
	    m_pEditor->ShowTab (!m_pEditor->IsTabVisible ());
}

void CBuildResource::OnUpdateStrumentiOrdineDiTabulazione(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->SetCheck (m_pEditor->IsTabVisible ());
}

void CBuildResource::OnStrumentiMultidraw() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
	    m_pEditor->SetMultidraw( !m_pEditor->GetMultidraw() );
}

void CBuildResource::OnUpdateStrumentiMultidraw(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor&&!m_pEditor->IsTabVisible ())
	    pCmdUI->SetCheck (m_pEditor->GetMultidraw());
}

void CBuildResource::GetXMLString (CString& strXML)
{
    if (m_pDoc)
        m_pDoc->GetCurrentXMLRecord (strXML);
}

void CBuildResource::OnFormatoAllineaASinistra() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->LeftAlignSelected();
}

void CBuildResource::OnFormatoAllineaADestra() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->RightAlignSelected();
}

void CBuildResource::OnFormatoAllineaAlBasso() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->BottomAlignSelected();
}

void CBuildResource::OnFormatoAllineaAllAlto() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->TopAlignSelected();
}

void CBuildResource::OnUpdateFormatoAllineaADestra(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnUpdateFormatoAllineaAlBasso(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnUpdateFormatoAllineaAllAlto(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnUpdateFormatoAllineaASinistra(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnFormatoRendiugualeEntrambeLeDimensioni() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
	    m_pEditor->MakeSameSizeSelected();
}

void CBuildResource::OnUpdateFormatoRendiugualeEntrambeLeDimensioni(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnFormatoRendiUgualeAltezza() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
	    m_pEditor->MakeSameSizeSelectedVertical();
}

void CBuildResource::OnUpdateFormatoRendiUgualeAltezza(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnFormatoRendiUgualeLunghezza() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
	    m_pEditor->MakeSameSizeSelectedHorizontal();
}

void CBuildResource::OnUpdateFormatoRendiUgualeLunghezza(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnFormatoAllineaAlPianoOrizzontale() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->MakeSameHorizontalPlane ();
}

void CBuildResource::OnUpdateFormatoAllineaAlPianoOrizzontale(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnFormatoAllineaAllAsseVerticale() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->MakeSameVerticalPlane ();
}

void CBuildResource::OnUpdateFormatoAllineaAllAsseVerticale(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnFormatoCentraNellaFinestraOrizzontale() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->CenterInDiagramHorizontal ();
}

void CBuildResource::OnUpdateFormatoCentraNellaFinestraOrizzontale(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnFormatoCentraNellaFinestraVerticale() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->CenterInDiagramVertical ();
}

void CBuildResource::OnUpdateFormatoCentraNellaFinestraVerticale(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() > 1 );
}

void CBuildResource::OnFormatoDisponiPulsantiDestra() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->DisposeButtonRight ();
}

void CBuildResource::OnUpdateFormatoDisponiPulsantiDestra(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() >= 1 );
}

void CBuildResource::OnFormatoDisponiPulsantiInBasso() 
{
	// TODO: Add your command handler code here
    if (m_pEditor)
        m_pEditor->DisposeButtonBottom ();
}

void CBuildResource::OnUpdateFormatoDisponiPulsantiInBasso(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_pEditor)
	    pCmdUI->Enable( m_pEditor->GetSelectCount() >= 1 );
}

/*
** AddRSFolder :
*/
void CBuildResource::AddRSFolder ()
{
    CString str;
    eResources e;

    /* andiamo a prendere la docking del catalogo */
    CAdamoRSCatalogue* rsCat=((CMainFrame*)AfxGetMainWnd ())->GetDockRSCatalogue ()->GetView ();
    if (rsCat)   {
        /* prendiamo un nome valido, dobbiamo chiederlo al tree */
        if (!rsCat->GetValidFolderName (str, &e))   {
            /* aggiungiamolo al nodo corrente */
            rsCat->AddCatalogueElement (str, "", e, RS_FOLDER);
        }
    }
}

/*
** OnInserisciModuloImmagine :
*/
void CBuildResource::OnInserisciModuloImmagine() 
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	CAdamoRSDiagramImage* obj = new CAdamoRSDiagramImage;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

/*
** OnUpdateInserisciModuloImmagine :
*/
void CBuildResource::OnUpdateInserisciModuloImmagine(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

/*
** CloseTabMode :
*/
void CBuildResource::CloseTabMode ()
{
	if (m_pEditor&&m_pEditor->IsTabVisible ())
        m_pEditor->ShowTab (FALSE);
}

void CBuildResource::OnStrumentiSincronizzaVariabili() 
{
	// TODO: Add your command handler code here
    if (m_pDoc)
        m_pDoc->SincronizzaVariabili ();
}

/*
** OnInserisciModuloAsse :
*/
void CBuildResource::OnInserisciModuloAsse() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramAx* obj = new CAdamoRSDiagramAx;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

/*
** OnUpdateInserisciModuloAsse :
*/
void CBuildResource::OnUpdateInserisciModuloAsse (CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

/*
** OnInserisciModuloIngressoAnalogico :
*/
void CBuildResource::OnInserisciModuloAngularGauge() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramIAngularGauge* obj = new CAdamoRSDiagramIAngularGauge;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

/*
** OnUpdateInserisciModuloIngressoAnalogico :
*/
void CBuildResource::OnUpdateInserisciModuloIAngularGauge (CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

/*
** OnInserisciModuloUscitaAnalogica :
*/
void CBuildResource::OnInserisciModuloILinearGauge() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramILinearGauge* obj = new CAdamoRSDiagramILinearGauge;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

/*
** OnUpdateInserisciModuloUscitaAnalogica :
*/
void CBuildResource::OnUpdateInserisciModuloILinearGauge (CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

/*
** OnInserisciModuloILed :
*/
void CBuildResource::OnInserisciModuloILed() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramLed* obj = new CAdamoRSDiagramLed;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

/*
** OnUpdateInserisciModuloILed :
*/
void CBuildResource::OnUpdateInserisciModuloILed (CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

/*
** OnInserisciModuloISevenSegment :
*/
void CBuildResource::OnInserisciModuloISevenSegment() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramISevenSegment* obj = new CAdamoRSDiagramISevenSegment;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

/*
** OnUpdateInserisciModuloISevenSegment :
*/
void CBuildResource::OnUpdateInserisciModuloISevenSegment (CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

/*
** OnInserisciModuloILabel :
*/
void CBuildResource::OnInserisciModuloILabel() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramILabel* obj = new CAdamoRSDiagramILabel;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

/*
** OnUpdateInserisciModuloILabel :
*/
void CBuildResource::OnUpdateInserisciModuloILabel (CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}

/*
** OnInserisciModuloIPicture :
*/
void CBuildResource::OnInserisciModuloIPicture() 
{
	// TODO: Add your command handler code here
	CAdamoRSDiagramPicture* obj = new CAdamoRSDiagramPicture;
    obj->SetParent (m_pDoc->GetRSContainer ());
	m_pEditor->StartDrawingObject( obj );
}

/*
** OnUpdateInserisciModuloIPicture :
*/
void CBuildResource::OnUpdateInserisciModuloIPicture (CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    BOOL b=FALSE;
	if (m_pEditor&&!m_pEditor->IsTabVisible ())
        b=TRUE;
    pCmdUI->Enable (b);
}
