// GlobalMatrix.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "GlobalMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGlobalMatrix dialog


CGlobalMatrix::CGlobalMatrix(CWnd* pParent /*=NULL*/)
	: CAdamoDialog(CGlobalMatrix::IDD, pParent), m_nColumns (0), m_nRows (0), m_bFreeze (false), m_bInitialized (false)
{
	//{{AFX_DATA_INIT(CGlobalMatrix)
	//}}AFX_DATA_INIT
}


void CGlobalMatrix::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGlobalMatrix)
	DDX_Control(pDX, IDOK, m_ctrlButOk);
	DDX_Control(pDX, IDC_STNAME, m_ctrlName);
	DDX_Control(pDX, IDC_LOADMATRIX, m_butLoadMatrix);
	DDX_Control(pDX, IDC_SAVEMATRIX, m_butSaveMatrix);
	DDX_Control(pDX, IDC_FREEZEMATRIX, m_butFreezeMatrix);
	DDX_Control(pDX, IDC_AGGIORNAMATRIX, m_butUpdateMatrix);
    //DDX_Control(pDX, IDC_GRID, m_Grid);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGlobalMatrix, CDialog)
	//{{AFX_MSG_MAP(CGlobalMatrix)
	ON_BN_CLICKED(IDC_LOADMATRIX, OnLoadMatrix)
	ON_BN_CLICKED(IDC_SAVEMATRIX, OnSaveMatrix)
	ON_BN_CLICKED(IDC_FREEZEMATRIX, OnFreezeMatrix)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_GRID, OnGridEndEdit)
	ON_NOTIFY(GVN_SELCHANGING, IDC_GRID, OnGridStartSelChange)
	ON_NOTIFY(GVN_BUTTONCHECKED, IDC_GRID, OnGridButtonChecked)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_AGGIORNAMATRIX, &CGlobalMatrix::OnBnClickedAggiornamatrix)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGlobalMatrix message handlers

BOOL CGlobalMatrix::OnInitDialog() 
{
    RECT rc;
    ePswLevel PswLevel;
    bool bEditable=false;

    CDialog::OnInitDialog();
    PswLevel=((CMainFrame *)AfxGetMainWnd ())->GetPswLevel ();
    rc.left=12; rc.top=41; rc.right=641; rc.bottom=328;
    m_Grid.Create (rc, this, IDC_GRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_Grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    if (PswLevel<ePswUser)
        bEditable=true;    
    m_Grid.SetEditable(bEditable);
    m_Grid.SetVirtualMode(false);
    m_Grid.SetFixedRowCount();
    m_Grid.SetFixedColumnCount();
    if (m_nColumns>0)   {
        m_Grid.SetRowCount(m_nRows+1);
        m_Grid.SetColumnCount(m_nColumns+1);
        FillColumnsCaption ();
        FillRows ();
    }
    else   {
        m_Grid.SetRowCount(15);
        m_Grid.SetColumnCount(2);
    }
    if (m_nColumns<=NMAX_GRIDCOLS)
        m_Grid.ExpandColumnsToFit (false);
    m_ctrlName.SetWindowText (GetVariableName());
    SetWindowText (GetVariableName ());
    ResizeDialog ();
	m_bInitialized = true;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CGlobalMatrix::SetData (const char* szMatrixName, int nRows, int nColumns, stColumnsData* astColumnsTypes)
{
    int nB=0;
    SetVariableName (szMatrixName);
    SetKey (szMatrixName);
    if (nColumns>0 && nColumns<= MAXCOLUMNS && nRows > 0)   {
        m_nRows=nRows;
        m_nColumns=nColumns;
        memcpy (m_astColumnsTypes, astColumnsTypes, m_nColumns*sizeof (stColumnsData));
    }
    else
        nB=-1;
    return nB;
}

void CGlobalMatrix::FillColumnsCaption ()
{
    GV_ITEM Item;
    int i;

	m_ImageList.Create(MAKEINTRESOURCE(IDB_MATRIXSYMBOLS), 16, 1, RGB(0,128,128));
	m_Grid.SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
    for (i=0; i<m_nColumns; i++)   {
		Item.col = i+1;
        if (m_astColumnsTypes[i].nType==LUA_TSTRING)
            Item.iImage=0;
        else
            if (m_astColumnsTypes[i].nType==LUA_TNUMBER)
                Item.iImage=1;
            else
                Item.iImage=2;
        Item.strText=m_astColumnsTypes[i].szName;
        m_Grid.SetItem(&Item);
    }
}

void CGlobalMatrix::FillRows ()
{
    int i, n;
    CString str;

    for (i=0; i<m_nRows; i++)   {
        for (n=0; n<m_nColumns; n++)
            if (m_astColumnsTypes[n].nType==LUA_TBOOLEAN)   {
                m_Grid.SetItemState(i+1,n+1, m_Grid.GetItemState(i+1,n+1) | GVIS_READONLY);
                m_Grid.SetCellType(i+1,n+1, RUNTIME_CLASS(CGridCellCheck));
            }
        str.Format(_T("%d"), i);
        m_Grid.SetItemText(i+1,0,str);
    }    
}


void CGlobalMatrix::OnLoadMatrix() 
{
	CStdioFile fMatrix;
	CKSFileDialog dlg(TRUE, "csv", GetVariableName ()+".csv", OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST,
        LOADSTRING (IDS_ADAMOGLOBALMATRIX_1), AfxGetMainWnd());
	if ( dlg.DoModal()!=IDOK )
		return;
	if (!fMatrix.Open(dlg.GetPathName(), CFile::modeRead|CFile::typeText))
		AfxMessageBox(LOADSTRING (IDS_ADAMOGLOBALMATRIX_2));
    else   {
        if (FillMatrix (&fMatrix))
            AfxMessageBox (LOADSTRING (IDS_ADAMOGLOBALMATRIX_3));
        fMatrix.Close ();
    }
}

void CGlobalMatrix::OnSaveMatrix() 
{
	CStdioFile fMatrix;
	CKSFileDialog dlg(FALSE, "csv", GetVariableName ()+".csv", OFN_PATHMUSTEXIST, 
		LOADSTRING (IDS_ADAMOGLOBALMATRIX_1), AfxGetMainWnd());
	if ( dlg.DoModal()!=IDOK )
		return;
	if (!fMatrix.Open(dlg.GetPathName(), CFile::modeCreate|CFile::modeWrite|CFile::typeText))
		AfxMessageBox(LOADSTRING (IDS_ADAMOGLOBALMATRIX_2));
    else   {
        FillMatrixFile (&fMatrix);
        fMatrix.Close ();
    }
}

void CGlobalMatrix::OnFreezeMatrix() 
{
	if (m_bFreeze)	 {
        m_bFreeze=false;
        m_butFreezeMatrix.SetWindowText (LOADSTRING (IDS_ADAMODLGSLOTVIEW_1)); 
        OnTimer ();
    }
    else   {
        m_bFreeze=true;
        m_butFreezeMatrix.SetWindowText (LOADSTRING (IDS_ADAMODLGSLOTVIEW_2));
    }
}

int CGlobalMatrix::FillMatrix (CStdioFile* pF)
{
    CMatrixListener ml;
    CAdamoData* pAdamoData=new CAdamoData [m_nColumns];
    CCellRange cr(1, 1, 1, m_nColumns);
    char* pBufKeys, *c, szBuffer[2048];
    int i=0, nB=-1, n, nBufKeysLen, anColumnsType[MAXCOLUMNS];
    bool bError=false;

    for (n=0; n<m_nColumns; n++)
        anColumnsType[n]=m_astColumnsTypes[n].nType;
    ml.SetColumnsTypes (m_nColumns, anColumnsType);
    PrepareBufKeys (&pBufKeys, &nBufKeysLen);
    while (pF->ReadString (szBuffer, 2047))   {
        ml.ResetResults (m_nColumns);
        if (ml.ParseMatrixString (szBuffer, i))
            return -1;
        for (n=0; n<m_nColumns; n++)   {
            switch (m_astColumnsTypes[n].nType)   {
                case LUA_TBOOLEAN :
                    pAdamoData[n].nType=LUA_TBOOLEAN;
                    if (ml.strResults[n]=="1")
                        pAdamoData[n].AdamoData.b=1;
                    else
                        pAdamoData[n].AdamoData.b=0;
                    break;
                case LUA_TNUMBER :
                    pAdamoData[n].nType=LUA_TNUMBER;
                    pAdamoData[n].AdamoData.fp=strtod (ml.strResults[n], &c);
                    if (*c)
                        bError=true;
                    break;
                case LUA_TSTRING :
                    pAdamoData[n].nType=LUA_TSTRING;
                    strcpy (pAdamoData[n].AdamoData.str, ml.strResults[n]);
                    break;
            }
        }
        if (!bError)   {
            GetMachine()->SetMatrix (GetVariableName (), i, -m_nColumns, nBufKeysLen, pBufKeys, pAdamoData);
            i++;
        }
        else   {
            AfxMessageBox (LOADSTRING (IDS_ADAMOGLOBALMATRIX_4));
            break;
        }
    }
    if (m_nColumns<=NMAX_GRIDCOLS)
        m_Grid.ExpandColumnsToFit (false);
    delete pBufKeys;
    delete pAdamoData;
    return 0;
}

void CGlobalMatrix::FillMatrixFile (CStdioFile* fMatrix)
{
    CAdamoData* pAdamoData=new CAdamoData [m_nColumns];
    CCellRange cr(1, 1, 1, m_nColumns);
    CString str;
    char* pBufKeys, szBuffer[256];
    int i, n, nBufKeysLen;
    PrepareBufKeys (&pBufKeys, &nBufKeysLen);
    for (i=0; i<m_nRows; i++)   {
        if (GetMachine()->GetMatrix (GetVariableName (), i, m_nColumns, nBufKeysLen, pBufKeys, pAdamoData)==S_OK)   {
            str="";
            for (n=0; n<m_nColumns; n++)   {
                if (m_astColumnsTypes[n].nType==LUA_TSTRING)
                    str+="\"";
                if (m_astColumnsTypes[n].nType==LUA_TBOOLEAN)
                    if (pAdamoData[n].AdamoData.b)
                        str+="1";
                    else
                        str+="0";
                else
                    if (m_astColumnsTypes[n].nType==LUA_TNUMBER)   {
                        sprintf (szBuffer, "%8.3f", pAdamoData[n].AdamoData.fp);
                        str+=szBuffer;
                    }
                    else
                        str+=pAdamoData[n].AdamoData.str;
                if (m_astColumnsTypes[n].nType==LUA_TSTRING)
                    str+="\"";
                if (n!=m_nColumns-1)
                    str+=",";
            }
            str+="\n";
            fMatrix->WriteString (str);
        }
        else   {
            AfxMessageBox (LOADSTRING (IDS_ADAMOGLOBALMATRIX_4));
            break;
        }
    }
    delete pBufKeys;
    delete pAdamoData;
}

void CMatrixListener::SetColumnsTypes (int nColumns, int* nColumnsType)
{
    memcpy (m_nVettTypes, nColumnsType, nColumns*sizeof (int));
    m_nColumn=nColumns;
}

void CMatrixListener::vRegisterToken (const std::tstring& strTokenText, const cooLexxerTokenRule* pptrRule)
{
    int nIDValue=0;
    if (pptrRule)
        nIDValue=pptrRule->nGetIDValue();
    if ((nIDValue==0&&m_nVettTypes[m_nCurrentColumn]==LUA_TNUMBER) ||
        (nIDValue==300&&m_nVettTypes[m_nCurrentColumn]==LUA_TSTRING) ||
        (nIDValue==301&&m_nVettTypes[m_nCurrentColumn]==LUA_TNUMBER) ||
        (nIDValue==301&&m_nVettTypes[m_nCurrentColumn]==LUA_TBOOLEAN))
        strResults[m_nCurrentColumn]=strTokenText.data();
    else
        if (nIDValue==100)
            m_nCurrentColumn++;
}

int CMatrixListener::ParseMatrixString (char* szBuffer, int i)
{
    int nB=0;
	std::tstringstream	strm(
		_T("[seperators]\n")
		_T("100:,\n")
        _T("1:\\n\n")
        _T("1:\\r\\n\n")
        _T("1: \n")
        _T("1:\t\n")
		_T("[tokens]\n")
		_T("200:sin\n")
		_T("[rules]\n")
		_T("300:strings\n")
		_T("301:numbers\n") );

    try {
   	    // Map with rules for the lexical analyzer
	    cooLexxerMap				map(strm);
	    cooLexxerTextInputStream	tis(szBuffer);
	    cooLexxer					lexxer(&tis,&map,this);

        m_nCurrentColumn=0;
        // Tokenize ...
	    while(!tis.fIsEofReached())
		    lexxer.vParseCharacter();
    }
    catch(cooLexxerException& e)   {
        nB=-1;
    }
    return nB;
}

void CMatrixListener::ResetResults (int nColumns)
{
    for (int i=0; i<nColumns; i++)
        strResults[i]="";
}

void CGlobalMatrix::OnDestroy() 
{
	CDialog::OnDestroy();
    m_Grid.DestroyWindow ();
}

void CGlobalMatrix::OnTimer() 
{
    CAdamoData* pAdamoData;
    char szBuffer[NMAXSTRING];
    int i, n, nBufKeysLen;
    char* pBufKeys;

    if (!m_bFreeze&&GetMachine()->IsCncRunning())	   {
        pAdamoData=new CAdamoData [m_nColumns];
        CCellRange cr=m_Grid.GetVisibleNonFixedCellRange ();
        PrepareBufKeys (&pBufKeys, &nBufKeysLen);
        for (i=cr.GetMinRow()-1; i<=cr.GetMaxRow()-1; i++)   {
            if (GetMachine()->GetMatrix (GetVariableName (), i, m_nColumns, nBufKeysLen, pBufKeys, pAdamoData)==S_OK)   {
                for (n=0; n<m_nColumns; n++)   {
                    switch (pAdamoData[n].nType)   {
                        case LUA_TBOOLEAN :
                            if (pAdamoData[n].AdamoData.b)   {
                                strcpy (szBuffer, LOADSTRING (IDS_ADAMOVERO));
                                ((CGridCellCheck*)m_Grid.GetCell (i+1, n+1))->SetCheck ();
                            }
                            else   {
                                strcpy (szBuffer, LOADSTRING (IDS_ADAMOFALSO));
                                ((CGridCellCheck*)m_Grid.GetCell (i+1, n+1))->SetCheck (FALSE);
                            }    
                            break;
                        case LUA_TNUMBER :
                            sprintf (szBuffer, "%8.3f", pAdamoData[n].AdamoData.fp);
                            break;
                        case LUA_TSTRING :
                            strcpy (szBuffer, pAdamoData[n].AdamoData.str);
                            break;
                        case LUA_TNIL :
                            szBuffer[0]='\0';
                    }
                    if (strcmp (m_Grid.GetItemText (i+1, n+1), szBuffer))
                        m_Grid.SetItemText (i+1, n+1, szBuffer);
                }
            }
            else   {
                for (n=0; n<m_nColumns; n++)
                    m_Grid.SetItemText (i, n+1, "");
                break;
            }
        }
        delete pBufKeys;
        delete pAdamoData;
        m_Grid.Invalidate (false);
    }
}

void CGlobalMatrix::PrepareBufKeys (char** pBufKeys, int* nBufKeysLen)
{
    int nDim=0, i;
    
    for (i=0; i<m_nColumns; i++)
        nDim+=strlen (m_astColumnsTypes[i].szName)+1;
    nDim+=2*sizeof(int);
    *pBufKeys=new char [nDim];
    char* p=*pBufKeys;
    *((int*)p)=nDim-2*sizeof(int);
    p+=sizeof (int);
    *((int*)p)=m_nColumns;    
    p+=sizeof (int);
    for (i=0; i<m_nColumns; i++)   {
        strcpy (p, m_astColumnsTypes[i].szName);
        p+=strlen (p)+1;
    }
    *nBufKeysLen=nDim;
}

void CGlobalMatrix::OnGridEndEdit(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    CAdamoData stData;
    bool bError=false;
    char szBuffer[256];
    char* c;
    if (GetMachine()->IsCncRunning())	   {
        NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
        CString str=m_Grid.GetItemText (pItem->iRow, pItem->iColumn);
        str.TrimLeft(), str.TrimRight ();
        switch (m_astColumnsTypes[pItem->iColumn-1].nType)   {
            case LUA_TBOOLEAN :
                stData.nType=LUA_TBOOLEAN;
                if (!strcmp (str, LOADSTRING (IDS_ADAMOVERO)))
                    stData.AdamoData.b=1;
                else
                    stData.AdamoData.b=0;
                break;
            case LUA_TNUMBER :
                stData.nType=LUA_TNUMBER;
                stData.AdamoData.fp=strtod (str, &c);
                if (*c)
                    bError=true;
                break;
            case LUA_TSTRING :
                stData.nType=LUA_TSTRING;
                strcpy (stData.AdamoData.str, str);
                break;
        }
        if (!bError)    {
            strcpy (szBuffer+2*sizeof(int), m_astColumnsTypes[pItem->iColumn-1].szName);
            GetMachine()->SetMatrix (GetVariableName (), pItem->iRow-1, pItem->iColumn-1, 1, szBuffer, &stData);
        }
    }
}

HBRUSH CGlobalMatrix::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor==CTLCOLOR_STATIC)
        if (pWnd->GetDlgCtrlID()==IDC_STNAME)    {
            pDC->SetBkColor (RGB (255, 255, 255));
            return (HBRUSH)GetStockObject (WHITE_BRUSH);
        }
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CGlobalMatrix::OnGridStartSelChange(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_nCurRow=pItem->iRow;
    m_nCurColumn=pItem->iColumn;
}

void CGlobalMatrix::OnGridButtonChecked(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    GV_CHECKNOTIFY* pItem = (GV_CHECKNOTIFY*) pNotifyStruct;
    if (m_astColumnsTypes[m_nCurColumn-1].nType==LUA_TBOOLEAN)   {
        CAdamoData stData;
        char szBuffer[256];
        stData.nType=LUA_TBOOLEAN;
        stData.AdamoData.b=pItem->iCheck;
        strcpy (szBuffer+2*sizeof(int), m_astColumnsTypes[m_nCurColumn-1].szName);
        GetMachine()->SetMatrix (GetVariableName (), m_nCurRow-1, m_nCurColumn-1, 1, szBuffer, &stData);
    }
}

void CGlobalMatrix::ResizeDialog ()
{
    CRect rc;
    int nOffX, nOffY, nMGC=4, nIncOffY=0;
	bool bChangeStyle = false;

	if (GETWINDOWSVERSION() == WINDOWS_7)   {
		int nElements = m_nRows;
		if (nElements>NMAX_GRIDROWS+1)   {
			nElements=NMAX_GRIDROWS+1;
			bChangeStyle = true;
		}
		int nLimiteInferiore = 41 + ((nElements+1) * ROW_HEIGTH_W7) + 4;
		if (m_nColumns>NMAX_GRIDCOLS)   {
			if (m_nColumns<=11)
				nMGC=(m_nColumns-NMAX_GRIDCOLS);
			else
				nLimiteInferiore+=16;
			nOffX=nMGC*COL_WIDTH_W7;
			if (m_nRows>=NMAX_GRIDROWS)
				nOffX+=15;
			bChangeStyle = true;
		}
		else
			nOffX=0;
	        
		rc.top=41;
		rc.left=11;
		rc.right=640+nOffX;
		rc.bottom=nLimiteInferiore;
		m_rcGridInitialPosition = rc;
		m_Grid.MoveWindow (&rc);

		rc.top=nLimiteInferiore + 10;
		rc.left=11;
		rc.right=86;
		rc.bottom=nLimiteInferiore + 33;
		m_rcLoadMatrixInitialPosition = rc;
		m_butLoadMatrix.MoveWindow (&rc);

		rc.top=nLimiteInferiore + 10;
		rc.left=94;
		rc.right=169;
		rc.bottom=nLimiteInferiore + 33;
		m_rcSaveMatrixInitialPosition = rc;
		m_butSaveMatrix.MoveWindow (&rc);

		rc.top=nLimiteInferiore + 10;
		rc.left=177;
		rc.right=252;
		rc.bottom=nLimiteInferiore + 33;
		m_rcFreezeMatrixInitialPosition = rc;
		m_butFreezeMatrix.MoveWindow (&rc);

		rc.top=nLimiteInferiore + 10;
		rc.left=260;
		rc.right=335;
		rc.bottom=nLimiteInferiore + 33;
		m_rcUpdateMatrixInitialPosition = rc;
		m_butUpdateMatrix.MoveWindow (&rc);

		rc.top=nLimiteInferiore + 10;
		rc.left=564+nOffX;
		rc.right=639+nOffX;
		rc.bottom=nLimiteInferiore + 33;
		m_rcOkInitialPosition = rc;
		m_ctrlButOk.MoveWindow (&rc);

		GetWindowRect (&rc);
		if (bChangeStyle)   {
			rc.bottom=rc.top + nLimiteInferiore + 82;
			rc.right=rc.right+nOffX+12;
		}
		else   {
			rc.bottom=rc.top + nLimiteInferiore + 70;
			rc.right=rc.right+nOffX;
		}
		m_rcInitialPosition = rc;
		MoveWindow (&rc);
		if (m_nColumns<=11)
			m_Grid.ExpandColumnsToFit ();
		else   {
			for (int i=0; i<m_nColumns+1; i++)
				m_Grid.SetColumnWidth (i, COL_WIDTH_W7); 
		}
	}
	else   {
		if (m_nColumns>NMAX_GRIDCOLS)   {
			if (m_nColumns<=11)
				nMGC=(m_nColumns-NMAX_GRIDCOLS);
			else
				nIncOffY=15;
			nOffX=nMGC*COL_WIDTH_XP;
			if (m_nRows>=NMAX_GRIDROWS)
				nOffX+=15;
		}
		else
			nOffX=0;
		if (m_nRows<NMAX_GRIDROWS)
			nOffY=((NMAX_GRIDROWS-m_nRows)*ROW_HEIGTH_XP)-nIncOffY;
		else
			nOffY=-nIncOffY;
	        
		rc.top=41;
		rc.left=11;
		rc.right=640+nOffX;
		rc.bottom=330-nOffY;
		m_Grid.MoveWindow (&rc);

		rc.top=345-nOffY;
		rc.left=11;
		rc.right=86;
		rc.bottom=368-nOffY;
		m_butLoadMatrix.MoveWindow (&rc);

		rc.top=345-nOffY;
		rc.left=94;
		rc.right=169;
		rc.bottom=368-nOffY;
		m_butSaveMatrix.MoveWindow (&rc);

		rc.top=345-nOffY;
		rc.left=177;
		rc.right=252;
		rc.bottom=368-nOffY;
		m_butFreezeMatrix.MoveWindow (&rc);

		rc.top=345-nOffY;
		rc.left=260;
		rc.right=335;
		rc.bottom=368-nOffY;
		m_butUpdateMatrix.MoveWindow (&rc);

		rc.top=345-nOffY;
		rc.left=564+nOffX;
		rc.right=639+nOffX;
		rc.bottom=368-nOffY;
		m_ctrlButOk.MoveWindow (&rc);

		GetWindowRect (&rc);
        rc.bottom=rc.bottom-nOffY;
		rc.right=rc.right+nOffX;
		MoveWindow (&rc);
	}
    CenterWindow ();
}

void CGlobalMatrix::OnClose() 
{
    ((CMainFrame *) AfxGetMainWnd ())->RemoveDialogList (GetKey ());
}

void CGlobalMatrix::OnOK() 
{
    CAdamoDialog::OnOK ();
    OnClose ();
}

void CGlobalMatrix::OnCancel() 
{
    CAdamoDialog::OnCancel ();
    OnClose ();
}

int CGlobalMatrix::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CAdamoDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Aggiungere qui il codice di creazione specifico.
	if (GETWINDOWSVERSION() == WINDOWS_7)   {
		if (m_nRows>NMAX_GRIDROWS+1 || m_nColumns>11)
			SetWindowLong (GetSafeHwnd (), GWL_STYLE, WS_POPUP | WS_CAPTION | WS_THICKFRAME);
	}
	return 0;
}

void CGlobalMatrix::OnSize(UINT nType, int cx, int cy)
{
	CAdamoDialog::OnSize(nType, cx, cy);

	// TODO: aggiungere qui il codice per la gestione dei messaggi.
	if (GETWINDOWSVERSION() == WINDOWS_7 && m_bInitialized)   {
		CRect rc, rcGrid;
		int xDelta, yDelta;

		rc = m_rcGridInitialPosition;
		xDelta = m_rcInitialPosition.right - m_rcGridInitialPosition.right;
		rc.right = cx - xDelta + 16;
		yDelta = m_rcInitialPosition.bottom - m_rcGridInitialPosition.bottom;
		rc.bottom = cy - yDelta + 62;
		m_Grid.MoveWindow (&rc);
		rcGrid = rc;

		rc = m_rcLoadMatrixInitialPosition;
		yDelta = m_rcInitialPosition.bottom - m_rcLoadMatrixInitialPosition.top;
		rc.top = cy - yDelta + 62;
		yDelta = m_rcInitialPosition.bottom - m_rcLoadMatrixInitialPosition.bottom;
		rc.bottom = cy - yDelta + 62;
		m_butLoadMatrix.MoveWindow (&rc);

		rc = m_rcSaveMatrixInitialPosition;
		yDelta = m_rcInitialPosition.bottom - m_rcSaveMatrixInitialPosition.top;
		rc.top = cy - yDelta + 62;
		yDelta = m_rcInitialPosition.bottom - m_rcSaveMatrixInitialPosition.bottom;
		rc.bottom = cy - yDelta + 62;
		m_butSaveMatrix.MoveWindow (&rc);

		rc = m_rcFreezeMatrixInitialPosition;
		yDelta = m_rcInitialPosition.bottom - m_rcFreezeMatrixInitialPosition.top;
		rc.top = cy - yDelta + 62;
		yDelta = m_rcInitialPosition.bottom - m_rcFreezeMatrixInitialPosition.bottom;
		rc.bottom = cy - yDelta + 62;
		m_butFreezeMatrix.MoveWindow (&rc);

		rc = m_rcUpdateMatrixInitialPosition;
		yDelta = m_rcInitialPosition.bottom - m_rcFreezeMatrixInitialPosition.top;
		rc.top = cy - yDelta + 62;
		yDelta = m_rcInitialPosition.bottom - m_rcFreezeMatrixInitialPosition.bottom;
		rc.bottom = cy - yDelta + 62;
		m_butUpdateMatrix.MoveWindow (&rc);

		rc = m_rcOkInitialPosition;
		yDelta = m_rcInitialPosition.bottom - m_rcOkInitialPosition.top;
		rc.top = cy - yDelta + 62;
		yDelta = m_rcInitialPosition.bottom - m_rcOkInitialPosition.bottom;
		rc.bottom = cy - yDelta + 62;
		xDelta = m_rcInitialPosition.right - m_rcOkInitialPosition.left;
		rc.left = cx - xDelta + 16;
		xDelta = m_rcInitialPosition.right - m_rcOkInitialPosition.right;
		rc.right = cx - xDelta + 16;
		m_ctrlButOk.MoveWindow (&rc);
		if (m_nColumns<=11)
			m_Grid.ExpandColumnsToFit ();
	}
}

void CGlobalMatrix::OnBnClickedAggiornamatrix()
{
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	CCellID cid;
	// TODO: aggiungere qui il codice per la gestione della notifica del controllo.
	m_Grid.SetFocus ();
	//m_Grid.GetCell (1, 1)->SetState (GVIS_FOCUSED | GVIS_SELECTED);
	cid = m_Grid.GetTopleftNonFixedCell ();
	m_Grid.SetSelectedRange (cid.row, cid.col, cid.row, cid.col);
	m_Grid.SetFocusCell (cid.row, cid.col);
}
