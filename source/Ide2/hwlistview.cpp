// HWListView.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "HWListView.h"
#include "MainFrame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* static functions */
static SearchHW (const char* szHW, const char* szDevice);

/////////////////////////////////////////////////////////////////////////////
// CHWListView

IMPLEMENT_DYNCREATE(CHWListView, CView)

CHWListView::CHWListView()
{
}

CHWListView::~CHWListView()
{
}


BEGIN_MESSAGE_MAP(CHWListView, CView)
	//{{AFX_MSG_MAP(CHWListView)
	ON_WM_CREATE()
	ON_WM_SIZE()
    ON_REGISTERED_MESSAGE(WM_XLISTCTRL_CHECKBOX_CLICKED,OnXlistctrlCheckboxClicked)
	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_COMBO_SELECTION,OnXlistctrlComboSelection)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHWListView drawing

void CHWListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CHWListView diagnostics

#ifdef _DEBUG
void CHWListView::AssertValid() const
{
	CView::AssertValid();
}

void CHWListView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHWListView message handlers
/*
** OnCreate : Create listctrl
*/
int CHWListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	/*style*/
	                 
    DWORD dwStyle = LVS_REPORT | LVS_SINGLESEL |LVS_SHOWSELALWAYS|
		    WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP ;
	if(!m_XListCtrl.CreateEx(WS_EX_CLIENTEDGE, _T("SysListView32"), _T(""),
                    dwStyle, CRect(0,0,0,0), this, IDC_LIST_HW, NULL)) {
        TRACE0("Failed to create list.\n");
		return -1;
    }

   	if (!m_ImageList.Create( IDB_HWTabView, 16, 3, RGB( 0x00,0x80,0x80 )))
	{
		TRACE0("Failed to create image list.\n");
		return -1;
	}
    m_XListCtrl.InsertColumn (0, LOADSTRING (IDS_ADAMOHWLISTVIEW_1), LVCFMT_LEFT,100+16);
	m_XListCtrl.InsertColumn (1, LOADSTRING (IDS_ADAMOHWLISTVIEW_2), LVCFMT_LEFT,100+16);
	m_XListCtrl.InsertColumn (2, LOADSTRING (IDS_ADAMOHWLISTVIEW_3), LVCFMT_LEFT ,100+16);
    
    /*set font*/
    CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
    if(pFrame)
	    m_XListCtrl.SetFont(pFrame->GetProgramFont());         
    /*immagini*/
    CHeaderCtrl* pHeader = m_XListCtrl.GetHeaderCtrl();
    pHeader->SetImageList(&m_ImageList);
    HDITEM hditem;
    for( int i = 0; i < pHeader->GetItemCount(); i++)   {
        pHeader->GetItem(i,&hditem);
        hditem.fmt = HDF_LEFT|HDF_IMAGE|HDF_STRING;
        hditem.mask = HDI_IMAGE|HDI_FORMAT;
        hditem.iImage = i;
        pHeader->SetItem(i,&hditem);
    }    
    /* set extended style*/
	m_XListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT /*| LVS_EX_TRACKSELECT*/);
	/* call EnableToolTips to enable tooltip display*/
	m_XListCtrl.EnableToolTips(FALSE);
	return 0;
}

/*
** OnSize : 
*/
void CHWListView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
    if (::IsWindow(m_XListCtrl.m_hWnd)){
		m_XListCtrl.SetWindowPos (NULL, 0, 0,cx, cy,SWP_SHOWWINDOW);
        ReSizeColumn(cx);
    }	   
}

/*
** OnXlistctrlCheckboxClicked : qunado clicco sul chk cambio la proprietà nell'xml
*/
LRESULT CHWListView::OnXlistctrlCheckboxClicked(WPARAM wParam,LPARAM lParam)
{
    CString strValue;
    CWnd *pWnd = GetParent();
    CWnd *pWnd1 = pWnd->GetParent();
    if(pWnd1->IsKindOf(RUNTIME_CLASS(CConfHWView))){
        CConfHWView* pConfHWView = (CConfHWView*)pWnd1;
        pConfHWView->GetProject()->SetMachineModifiedFlag(true);/*set il flag di modifica*/
        CGruppDevice* pDeviceAtt = (CGruppDevice*) m_XListCtrl.GetItemData(wParam);
        
        if(m_XListCtrl.GetCheckbox(wParam,0) == 1)
            strValue = "yes";
        else { 
            strValue = "no";
            CString strPathFileDat = pConfHWView->GetProject()->GetParamsPathName();
            /*cancello il file Device.dat*/
			DeleteFileBin(strPathFileDat,pDeviceAtt->GetDeviceName());
        }
        
        pConfHWView->SetAttributeDeviceName(pDeviceAtt->GetName(),strValue);
    }
    return NULL;
}

/*
** OnXlistctrlComboSelection: quando cambio il combo cambio la proprietà nell'xml
*/
LRESULT CHWListView::OnXlistctrlComboSelection(WPARAM wParam,LPARAM lParam)
{
    CWnd *pWnd = GetParent();
    CWnd *pWnd1 = pWnd->GetParent();
    if(pWnd1->IsKindOf(RUNTIME_CLASS(CConfHWView))){
        CConfHWView* pConfHWView = (CConfHWView*)pWnd1;
        CGruppDevice* pDeviceAtt = (CGruppDevice*) m_XListCtrl.GetItemData(wParam);
        pConfHWView->GetProject()->SetMachineModifiedFlag(true);/*set il flag di modifica*/
        CString strOptionSel = m_XListCtrl.GetItemText(wParam,lParam);
        pDeviceAtt->SetOptionSel(strOptionSel);
        pConfHWView->SetAttrubuteOption(pDeviceAtt->GetName(),strOptionSel);   
    }
    return NULL;
}

/*
** ReSizeColumn : size width column 
*/
void CHWListView::ReSizeColumn(int nWidth)
{
    for(int i = 0; i< m_XListCtrl.GetColumns(); i++){  
        m_XListCtrl.SetColumnWidth(i,nWidth/m_XListCtrl.GetColumns()-1.5);
    }
}

/*
** DeleteFileBin : cancella il file .dat  
*/
void CHWListView::DeleteFileBin(CString strPathFile,CString strDeviceName)
{
    BOOL bFlag = FALSE;
    int i = 0;
    /*open file dat */
    if(strPathFile !="") {
        CAdamoDatiFile* pDatiFile= new CAdamoDatiFile;
        if (pDatiFile)   {
            pDatiFile->Open (strPathFile, TRUE);
            /*leggo il file*/
            pDatiFile->ReadData();
            /*cancello il contenuto del file */
            
           //enInputDigitale
            for( i = 0 ; i < NMAXINPDIG; i ++) {
                if(SearchHW(pDatiFile->GetAdamoParametri()->AdamoInputDig[i].szHWAddress,strDeviceName) != NULL) {
                    memset( pDatiFile->GetAdamoParametri()->AdamoInputDig[i].szHWAddress,NULL, MAXHWLENGTH );   
                    bFlag = TRUE;
                }
            } 
                
            //enOutputDigitale:
            for( i = 0 ; i < NMAXOUTDIG; i ++) {                  
                if(SearchHW(pDatiFile->GetAdamoParametri()->AdamoOutputDig[i].szHWAddress,strDeviceName) != NULL) {
                    memset( pDatiFile->GetAdamoParametri()->AdamoOutputDig[i].szHWAddress,NULL, MAXHWLENGTH );   
                    bFlag = TRUE;
                }
            }               
                
            //enInputAnalogico:
            for( i = 0 ; i < NMAXINPANA; i ++) {                  
                if(SearchHW(pDatiFile->GetAdamoParametri()->AdamoInputAnag[i].szHWAddress,strDeviceName) != NULL) {
                    memset( pDatiFile->GetAdamoParametri()->AdamoInputAnag[i].szHWAddress,NULL, MAXHWLENGTH );     
                    bFlag = TRUE;
                }
            }
            
            //enOutputAnalogico:
            for( i = 0 ; i < NMAXOUTANA; i ++) {                  
                if(SearchHW(pDatiFile->GetAdamoParametri()->AdamoOutputAnag[i].szHWAddress,strDeviceName) != NULL) {
                    memset( pDatiFile->GetAdamoParametri()->AdamoOutputAnag[i].szHWAddress,NULL, MAXHWLENGTH );       
                    bFlag = TRUE;
                }
            }
                
            //enAsse:
            for( i = 0 ; i < NMAXAXIS; i ++) {                  
                if(SearchHW(pDatiFile->GetAdamoParametri()->AdamoAsse[i].szHWAddress,strDeviceName) != NULL) {
                    memset( pDatiFile->GetAdamoParametri()->AdamoAsse[i].szHWAddress,NULL, MAXHWLENGTH );         
                    bFlag = TRUE;
                }
            }
                
           //enGenericCan:
           for( i = 0 ; i < NMAXGENERICCAN ; i ++) {                  
               if(SearchHW(pDatiFile->GetAdamoParametri()->AdamoCanDevice[i].szHWAddress,strDeviceName) != NULL) {    
                  memset( pDatiFile->GetAdamoParametri()->AdamoCanDevice[i].szHWAddress,NULL, MAXHWLENGTH );         
                  bFlag = TRUE;
               }
            }

           /*se ho trovato un hardware chiedo se cancellare*/
           if(bFlag)
             if ( AfxMessageBox(LOADSTRING (IDS_ADAMOHWLISTVIEW_4), MB_YESNO)==IDYES ) {
                pDatiFile->WriteData(); 
             }
            //pDatiFile->WriteData();
            pDatiFile->CloseFile();
            delete pDatiFile; 
        }
    }

}

/*
** SearchHW :
*/
static SearchHW (const char* szHW, const char* szDevice)
{
    bool b=false;
    char* s=strstr (szHW, szDevice);
    if (s==szHW)
        b=true;
    return b;        
}
