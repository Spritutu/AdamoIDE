
// AdamoCPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "GridCtrl.h"
#include "GridCellCheck.h"
#include "AdamoCPDlg.h"
#include "GridCellCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CCPGridCellCheck, CGridCellCheck)

CString* aszCPDlgCaption [3];

char* aszCPTypes [] = {
    "Number",
    "Boolean",
    "String"
};

/////////////////////////////////////////////////////////////////////////////
// CAdamoCPDlg dialog

void CCPGridCellCheck::OnClick (CPoint PointCellRelative)
{ 
    if (m_pCP)   {
        m_pCP->SetModified (true);
        CGridCellCheck::OnClick (PointCellRelative);
    }
}

CAdamoCPDlg::CAdamoCPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdamoCPDlg::IDD, pParent), m_pAx (NULL), m_bIsModified (false)
{
	//{{AFX_DATA_INIT(CAdamoCPDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAdamoCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdamoCPDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//DDX_Control(pDX, IDC_GRID, m_grid);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdamoCPDlg, CDialog)
	//{{AFX_MSG_MAP(CAdamoCPDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdamoCPDlg message handlers

CAdamoCPGrid::CAdamoCPGrid () : m_pDlg (NULL)  {
    for (int i=0; i<MAX_AXIS_CUSTOM; i++)
        m_abEdit[i]=false;
}

void CAdamoCPGrid::InitControl ()
{
    m_slTypeCP.SetSize(sizeof (aszCPTypes)/sizeof (char*));
    for (int n=0; n<sizeof (aszCPTypes)/sizeof (char*); n++)
        m_slTypeCP[n]=aszCPTypes[n];

}

void CAdamoCPGrid::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
    if (nCol==1)
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
    else
        if (m_abEdit[nRow-1])   {
            if (nCol==3)   {
                if (GetCell (nRow, nCol)->GetRuntimeClass ()!=RUNTIME_CLASS(CCPGridCellCheck))
                    CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
            }
            else
                CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
        }
}

void CAdamoCPGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
    CString strExisting;
    if (nCol==1)   {
        if (str!="")   {
            strExisting=GetCell(nRow, nCol)->GetText ();
            if (strExisting=="")   {
                SetCellType(nRow, 2, RUNTIME_CLASS(CGridCellCombo));
                ((CGridCellCombo*)GetCell(nRow, 2))->SetOptions(m_slTypeCP);
                ((CGridCellCombo*)GetCell(nRow, 2))->SetStyle (CBS_DROPDOWNLIST);
                GetCell(nRow, 2)->SetText(aszCPTypes[0]);
                m_abEdit[nRow-1]=true;
            }
        }
        else   {
            GetCell(nRow, 2)->SetText("");
            GetCell(nRow, 3)->SetText("");
            SetCellType(nRow, 2, RUNTIME_CLASS(CGridCell));
            m_abEdit[nRow-1]=false;
        }
    }
    else   {
        if (nCol==2)   {
            if (str==aszCPTypes[1])   {
                SetCellType(nRow, 3, RUNTIME_CLASS(CCPGridCellCheck));
                GetCell(nRow, 3)->SetText("");
                ((CCPGridCellCheck *)GetCell(nRow, 3))->SetCPDlg (m_pDlg);
            }
            else
                SetCellType(nRow, 3, RUNTIME_CLASS(CGridCell));
            if (str==aszCPTypes[0])   {
                char *p;
                strExisting=GetCell(nRow, 3)->GetText();
                strExisting.TrimLeft (); strExisting.TrimRight ();
                strtod (strExisting, &p);
                if (*p!='\0')
                    GetCell(nRow, 3)->SetText("");
            }
        }
    }
    CGridCtrl::OnEndEditCell(nRow, nCol, str);
    m_pDlg->SetModified (true);
    Invalidate ();
}

void CAdamoCPGrid::AddLine (int i, int nType, const char* szName, void* pData)
{
    /* copiamo innanzitutto il nome */
    GetCell(i+1, 1)->SetText(szName);
    /* ora il tipo */
    SetCellType(i+1, 2, RUNTIME_CLASS(CGridCellCombo));
    ((CGridCellCombo*)GetCell(i+1, 2))->SetOptions(m_slTypeCP);
    ((CGridCellCombo*)GetCell(i+1, 2))->SetStyle (CBS_DROPDOWNLIST);
    switch (nType)   {
        case LUA_TNUMBER :
            GetCell(i+1, 2)->SetText(aszCPTypes[0]);
            break;
        case LUA_TBOOLEAN :
            GetCell(i+1, 2)->SetText(aszCPTypes[1]);
            break;
        case LUA_TSTRING :
            GetCell(i+1, 2)->SetText(aszCPTypes[2]);
            break;
    }
    /* ora il valore */
    CString str;
    switch (nType)   {
        case LUA_TNUMBER :
            str.Format ("%8.3f", (*((double*)pData)));
            GetCell(i+1, 3)->SetText(str);
            break;
        case LUA_TBOOLEAN :
            SetCellType(i+1, 3, RUNTIME_CLASS(CCPGridCellCheck));
            ((CCPGridCellCheck*)GetCell (i+1, 3))->SetCheck ((int)*((bool*)pData));
            ((CCPGridCellCheck*)GetCell (i+1, 3))->SetCPDlg (m_pDlg);
            break;
        case LUA_TSTRING :
            GetCell(i+1, 3)->SetText((char*)pData);
            break;
    }
    /* marchiamo la riga come editabile */
    m_abEdit[i]=true;
}

bool CAdamoCPGrid::GetLine (int nIndex, int* pnType, char* szName, void* pData)
{
    bool b=true;
    int i;

    nIndex+=1;
    CString str=GetCell(nIndex, 1)->GetText ();
    if (str!="")   {
        strlimcpy (szName, str, NMAXIDENTIFIER+1);
        str=GetCell(nIndex, 2)->GetText ();
        for (i=0; i<3; i++)   {
            if (!strcmp (str, aszCPTypes[i]))
                break;
        }
        if (i<3)   {
            switch (i)   {
                case 0 :
                    *pnType=LUA_TNUMBER;
                    break;
                case 1 :
                    *pnType=LUA_TBOOLEAN;
                    break;
                case 2 :
                    *pnType=LUA_TSTRING;
                    break;
            }
            if (*pnType==LUA_TBOOLEAN)
                *((bool*)pData)=((CCPGridCellCheck*)GetCell(nIndex, 3))->GetCheck ()!=0;
            else   {
                str=GetCell(nIndex, 3)->GetText ();
                if (*pnType==LUA_TSTRING)   {
                    strlimcpy ((char*)pData, str, 64+1);
                }
                else   {
                    char* p;
                    double fp=strtod (str, &p);
                    if (*p=='\0')
                        *((double*)pData)=fp;
                    else   {
                        *((double*)pData)=0.0;
                        b=false;
                    }
                }
            }
        }
    }    
    else
        *pnType=-1;
    return b;
}

void CAdamoCPGrid::SelectCell (int nRow, int nCol)
{
    ClearAllSelection ();
    SetItemState (nRow, nCol, GVIS_SELECTED|GVIS_FOCUSED);
    SetFocusCell (nRow, nCol);
    Invalidate ();
}

void CAdamoCPGrid::ClearAllSelection ()
{
    for (int i=0; i<MAX_AXIS_CUSTOM; i++)
        for (int n=1; n<=3; n++)
            SetItemState(i, n, GetItemState(i, n) & ~(GVIS_SELECTED|GVIS_FOCUSED));
}


BOOL CAdamoCPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    LoadStrings ();
	InitGrid ();
    m_grid.InitControl ();
    FillGrid ();
    m_grid.SetItemState (1, 1, GVIS_SELECTED|GVIS_FOCUSED);
    m_grid.SetFocusCell (1, 1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdamoCPDlg::InitGrid ()
{
    GV_ITEM Item;
    CString str;
    int i;
    RECT rc;

    rc.left=5; rc.top=5; rc.right=452; rc.bottom=333;
    m_grid.Create (rc, this, IDC_GRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_grid.GetDefaultCell(FALSE, FALSE)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    m_grid.SetEditable(true);
    m_grid.SetVirtualMode(false);
    m_grid.SetFixedRowCount();
    m_grid.SetFixedColumnCount();
    m_grid.SetRowCount (MAX_AXIS_CUSTOM+1);
    m_grid.SetColumnCount(3+1);
    m_grid.ExpandColumnsToFit (false);
    /* inizializziamo ora le righe fisse */
    for (i=0; i<MAX_AXIS_CUSTOM; i++)   {
        m_grid.GetCell (i+1, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        str.Format("%d", i+1);
        m_grid.SetItemText(i+1, 0, str);
    }
    /* ora le colonne fisse */
	m_ImageList.Create(MAKEINTRESOURCE(IDB_BMPCUSTOMPARAMETER), 16, 1, RGB(0,128,128));
	m_grid.SetImageList(&m_ImageList);
    Item.mask = GVIF_TEXT | GVIF_IMAGE;
	Item.row = 0;
    for (i=0; i<3; i++)   {
		Item.col=i+1;
        Item.iImage=i;
        Item.strText=*aszCPDlgCaption [i];
        m_grid.SetItem(&Item);
    }
    m_grid.SetCPDlg (this);
}

void CAdamoCPDlg::FillGrid ()
{
    if (m_pAx)   {
        struct stCustomData* pCD=m_pAx->GetCD ();
        for (int i=0; i<MAX_AXIS_CUSTOM; i++)   {
            if (pCD[i].nType!=-1)   {
                m_grid.AddLine (i, pCD[i].nType, pCD[i].szName, &pCD[i].uData);
            }
        }
    }
}

void CAdamoCPDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (FillData ())
	    CDialog::OnOK();
}

bool CAdamoCPDlg::FillData ()
{
    struct stCustomData* pCD=m_pAx->GetCD ();
    int i, nType;
    bool b=true;
    char szName[NMAXIDENTIFIER+1];
    char szBuffer[64+1];

    if (pCD)   {
        for (i=0; i<MAX_AXIS_CUSTOM; i++)   {
            if (m_grid.GetLine (i, &nType, szName, szBuffer))   {
                pCD[i].nType=nType;
                if (nType!=-1)   {
                    strcpy (pCD[i].szName, szName);
                    memcpy (&pCD[i].uData, szBuffer, 64+1);
                }
                else   {
                    pCD[i].szName[0]='\0';
                    memset (&pCD[i].uData, 0, sizeof (pCD[i].uData));
                }
            }
            else   {
                m_grid.SelectCell (i+1, 3);
                AfxMessageBox ("Dati Numerici Errati");
                b=false; break;
            }
        }
    }
    return b;
}

void CAdamoCPDlg::LoadStrings ()
{
    for (int i=0; i<3; i++)   {
        aszCPDlgCaption[i]=new CString;
        *aszCPDlgCaption[i]=LOADSTRING (IDS_ADAMOCPDLG_1+i);
    }
}


void CAdamoCPDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
    for (int i=0; i<3; i++)
        _delete (aszCPDlgCaption[i]);
    m_grid.DestroyWindow ();
}
