/*
**
*/

/* include files */
#include "stdafx.h"
#include <math.h>
#include <io.h>
#include "ide2.h"
#include "MainFrame.h"
#include "workspace.h"
#include "AdamoCC.h"
#include "GridCtrl.h"
#include "GridCellCheck.h"
#include "AdamoAxis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* dialog dei correttori incrociati */
CCavStrzDlg::CCavStrzDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCavStrzDlg::IDD, pParent), m_pAx (NULL), m_nRows (0), m_nNumIntervals (NUMINTERVALLI_256)
{
	//{{AFX_DATA_INIT(CCavStrzDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    memset (m_anAsseOccupato, 0, sizeof (m_anAsseOccupato));
    memset (m_bChecked, 0, sizeof (m_bChecked));
}

void CCavStrzDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCavStrzDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//DDX_Control(pDX, IDC_GRID, m_grid);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCavStrzDlg, CDialog)
	//{{AFX_MSG_MAP(CCavStrzDlg)
	ON_BN_CLICKED(IDC_CHKAX1, OnChkAx1)
	ON_BN_CLICKED(IDC_CHKAX2, OnChkAx2)
	ON_BN_CLICKED(IDC_CHKAX3, OnChkAx3)
	ON_BN_CLICKED(IDC_CHKAX4, OnChkAx4)
	ON_BN_CLICKED(IDC_CHKAX5, OnChkAx5)
    ON_CBN_SELENDOK(IDC_CMBAX1, OnSelAsse1)
    ON_CBN_SELENDOK(IDC_CMBAX2, OnSelAsse2)
    ON_CBN_SELENDOK(IDC_CMBAX3, OnSelAsse3)
    ON_CBN_SELENDOK(IDC_CMBAX4, OnSelAsse4)
    ON_CBN_SELENDOK(IDC_CMBAX5, OnSelAsse5)
	ON_BN_CLICKED(IDC_BTNAX1, OnBtnAx1)
	ON_BN_CLICKED(IDC_BTNAX2, OnBtnAx2)
	ON_BN_CLICKED(IDC_BTNAX3, OnBtnAx3)
	ON_BN_CLICKED(IDC_BTNAX4, OnBtnAx4)
	ON_BN_CLICKED(IDC_BTNAX5, OnBtnAx5)
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BTN_DELETECCFILE, OnBtnDeleteCCFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCavStrzDlg message handlers

BOOL CCavStrzDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    /* supermegatemporanei */
    CreateAxesList ();
    /* inizializzazione del grid */
    if (!InitGrid ())   {
        SetWindowText (m_pAx->GetName ());
        CenterWindow ();
    }
    else
        EndDialog (0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCavStrzDlg::OnDestroy() 
{
	CDialog::OnDestroy();
    m_grid.DestroyWindow ();
	// TODO: Add your message handler code here
}

bool CCavStrzDlg::InitGrid ()
{
    CString str;
    double fp;
    int nNumPos, nNumNeg;
    bool b=false;
    RECT rc;

    rc.left =12; rc.top=68; rc.right=790; rc.bottom=350;
    m_grid.Create (rc, this, IDC_GRID, WS_CHILD | WS_TABSTOP | WS_VISIBLE);
    m_grid.GetDefaultCell(FALSE, FALSE)->SetBackClr(RGB(0xFF, 0xFF, 0xE0));
    m_grid.GetDefaultCell(FALSE, FALSE)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
    m_grid.SetEditable(true);
    m_grid.SetVirtualMode(false);
    m_grid.SetFixedRowCount();
    m_grid.SetFixedColumnCount();
    m_grid.SetColumnCount(6);
    m_grid.ExpandColumnsToFit (false);
    m_editIntervals.SubclassEdit(IDC_EDITSTEPS, this, PES_NUMBERS);
    m_editRange.SubclassEdit(IDC_EDITRANGE, this, PES_NUMBERS);
    FillButtons ();
    if (_access (GetPath()+m_strNomeFile, 00)||m_strNomeFile=="")  {
        /* non esiste alcun file di limiti */
        if (GetLimits (&m_fpLimPos, &m_fpLimNeg))   {
            if (fabs (m_fpLimPos-m_fpLimNeg)>1.0)   {
            m_fpStep=CalcInitialStep (m_fpLimPos, m_fpLimNeg);
            CalcNumCorrectors (m_fpStep, m_fpLimPos, m_fpLimNeg, &nNumPos, &nNumNeg);
            FillRows (m_fpStep, m_fpLimPos, m_fpLimNeg, nNumPos, nNumNeg, &fp, &fp);
        }
        else   {
            m_grid.EnableWindow (false);
                AfxMessageBox (LOADSTRING(IDS_ADAMOCC_9));
                b=true;
            }
        }
        else   {
            m_grid.EnableWindow (false);
            AfxMessageBox (LOADSTRING(IDS_ADAMOCC_1));
            b=true;
        }
    }
    else   {
        /* esiste il file, leggiamolo */
        m_file.SetNomeAsse(m_pAx->GetName ());
        FillGridFromFile ();
    }
    InitialSettings ();
    m_grid.ExpandColumnsToFit ();
    m_grid.SetCavStrzDlg(this);
    return b;
}

void CCavStrzDlg::CreateAxesList ()
{
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)
        Iterator ((CAdamoGroup*)pPrj->GetMachineModule());    
}

void CCavStrzDlg::Iterator(CAdamoGroup *pGrp)
{
    pGrp->Init();
    pGrp->DevInit();
    while(pGrp->DevMore()){
        CAdamoBase *pDev = (CAdamoBase *)pGrp->DevGetNext();
        if (pDev->GetType ()== enAsse)
            InsertAx ((CAdamoAxis *)pDev);
    }
    while(pGrp->More())   {
        CObject *obj = (CObject*)pGrp->GetNext();
        Iterator((CAdamoGroup*)obj);
    }
}

void CCavStrzDlg::InsertAx (CAdamoAxis *pDev)
{
    for (POSITION pos2, pos=m_listAxes.GetHeadPosition(); pos2=pos;)   {
        CAdamoAxis* pListDev=(CAdamoAxis*) m_listAxes.GetNext (pos);
        if (pDev->GetName().Compare (pListDev->GetName ())<0)   {
            m_listAxes.InsertBefore (pos2, pDev);
            return;
        }
    }
    m_listAxes.AddTail (pDev);
}

void CCavStrzDlg::OnChkAx1() 
{
}


void CCavStrzDlg::OnChkAx2() 
{
}

void CCavStrzDlg::OnChkAx3() 
{
}

void CCavStrzDlg::OnChkAx4() 
{
}

void CCavStrzDlg::OnChkAx5() 
{
}

void CCavStrzDlg::OnSelAsse1 ()
{
    CString str;
    POSITION pos;
    int nSel, i;
    /* andiamo a cercare l'asse selezionato */
    if ((nSel=((CComboBox*)GetDlgItem (IDC_CMBAX1))->GetCurSel())!=CB_ERR)   {
        ((CComboBox*)GetDlgItem (IDC_CMBAX1))->GetLBText (nSel, str);
        for (pos=m_listAxes.GetHeadPosition(), i=0; pos; i++)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            if (str==pAx->GetName())   {
                m_anAsseOccupato[0]=pAx;
                break;
            }
        }
        /* azzeriamo tutte le righe */
        TutteLeRigheA ("0.000", 1);
    }
}

void CCavStrzDlg::OnSelAsse2 ()
{
    CString str;
    POSITION pos;
    int nSel, i;
    /* andiamo a cercare l'asse selezionato */
    if ((nSel=((CComboBox*)GetDlgItem (IDC_CMBAX2))->GetCurSel())!=CB_ERR)   {
        ((CComboBox*)GetDlgItem (IDC_CMBAX2))->GetLBText (nSel, str);
        for (pos=m_listAxes.GetHeadPosition(), i=0; pos; i++)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            if (str==pAx->GetName())   {
                m_anAsseOccupato[1]=pAx;
                break;
            }
        }
        /* azzeriamo tutte le righe */
        TutteLeRigheA ("0.000", 2);
    }
}

void CCavStrzDlg::OnSelAsse3 ()
{
    CString str;
    POSITION pos;
    int nSel, i;
    /* andiamo a cercare l'asse selezionato */
    if ((nSel=((CComboBox*)GetDlgItem (IDC_CMBAX3))->GetCurSel())!=CB_ERR)   {
        ((CComboBox*)GetDlgItem (IDC_CMBAX3))->GetLBText (nSel, str);
        for (pos=m_listAxes.GetHeadPosition(), i=0; pos; i++)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            if (str==pAx->GetName())   {
                m_anAsseOccupato[2]=pAx;
                break;
            }
        }
        /* azzeriamo tutte le righe */
        TutteLeRigheA ("0.000", 3);
    }
}

void CCavStrzDlg::OnSelAsse4 ()
{
    CString str;
    POSITION pos;
    int nSel, i;
    /* andiamo a cercare l'asse selezionato */
    if ((nSel=((CComboBox*)GetDlgItem (IDC_CMBAX4))->GetCurSel())!=CB_ERR)   {
        ((CComboBox*)GetDlgItem (IDC_CMBAX4))->GetLBText (nSel, str);
        for (pos=m_listAxes.GetHeadPosition(), i=0; pos; i++)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            if (str==pAx->GetName())   {
                m_anAsseOccupato[3]=pAx;
                break;
            }
        }
        /* azzeriamo tutte le righe */
        TutteLeRigheA ("0.000", 4);
    }
}

void CCavStrzDlg::OnSelAsse5 ()
{
    CString str;
    POSITION pos;
    int nSel, i;
    /* andiamo a cercare l'asse selezionato */
    if ((nSel=((CComboBox*)GetDlgItem (IDC_CMBAX5))->GetCurSel())!=CB_ERR)   {
        ((CComboBox*)GetDlgItem (IDC_CMBAX5))->GetLBText (nSel, str);
        for (pos=m_listAxes.GetHeadPosition(), i=0; pos; i++)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            if (str==pAx->GetName())   {
                m_anAsseOccupato[4]=pAx;
                break;
            }
        }
        /* azzeriamo tutte le righe */
        TutteLeRigheA ("0.000", 5);
    }
}

void CCavStrzDlg::TutteLeRigheA (const char* sz, int n)
{
    int i;
    
    for (i=1; i<m_grid.GetRowCount (); i++)
        m_grid.SetItemText (i, n, sz);
    m_grid.Invalidate();
}

bool CCavStrzDlg::IsColumnChecked (int n)
{
    int nId;
    switch (n)   {
        case 1 :
            nId=IDC_CHKAX1;
            break;
        case 2 :
            nId=IDC_CHKAX2;
            break;
        case 3 :
            nId=IDC_CHKAX3;
            break;
        case 4 :
            nId=IDC_CHKAX4;
            break;
        case 5 :
            nId=IDC_CHKAX5;
            break;
        default :
            break;
    }
    return ((CButton*)GetDlgItem (nId))->GetCheck ()!=0;
}

void CCavStrzGrid::OnEditCell(int nRow, int nCol, CPoint point, UINT nChar)
{
    if (m_pDlg->IsColumnChecked (nCol))
        CGridCtrl::OnEditCell (nRow, nCol, point, nChar);
}

void CCavStrzGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
    char *pBuffer = str.GetBuffer (), *p;
    str.Trim ();
    strtod (pBuffer, &p);
    if (*p != '\0')
        str = "0.000";
    CGridCtrl::OnEndEditCell(nRow, nCol, str);
}

double CCavStrzDlg::CalcInitialStep (double fpLimPos, double fpLimNeg)
{
    if (fpLimPos>0.0&&fpLimNeg>0.0)
        m_fpStep=(fpLimPos-fpLimNeg)/m_nNumIntervals;
    else
        if (fpLimPos<0.0&&fpLimNeg<0.0)
            m_fpStep=fabs(fpLimNeg-fpLimPos)/m_nNumIntervals;
        else
            m_fpStep=(fpLimPos+fabs(fpLimNeg))/m_nNumIntervals;
    return m_fpStep;
}

bool CCavStrzDlg::GetLimits (double* fpPos, double* fpNeg)
{
    bool b=false;
    if (m_pAx)   {
        if (m_pAx->GetEnableLimAxesPos()&&m_pAx->GetEnableLimAxesNeg())   {
            *fpPos=m_pAx->GetLimAxisPos ();
            *fpNeg=m_pAx->GetLimAxisNeg ();
            b=true;
        }
    }
    return b;
}

void CCavStrzDlg::CalcNumCorrectors (double fpStep, double fpLimPos, double fpLimNeg, int* nNumPos, int* nNumNeg)
{
    double fp;
    bool b=false;

    *nNumNeg=0;
    fp=0.0;
    if (fpLimPos<0)   {
        fp=fpLimPos;
        b=true;
    }
    while (true)   {
        if (fp<fpLimNeg || ISEQUAL (fp, fpLimNeg))
            break;
        fp-=fpStep;
        (*nNumNeg)++;
    }
    if (ISEQUAL (fp+fpStep, fpLimNeg)&&fp!=0.0&&!b)
        (*nNumNeg)--;
    *nNumPos=0;
    fp=0.0;
    b=false;
    if (fpLimNeg>0)
        fp=fpLimNeg;
    while (true)   {
        if (fp>fpLimPos||ISEQUAL (fp, fpLimPos))
            break;
        fp+=fpStep;
        (*nNumPos)++;
    }
}

void CCavStrzDlg::FillRows (double fpStep, double fpLimPos, double fpLimNeg, int nNumPos, int nNumNeg, double* fpNegInterval, double* fpPosInterval)
{
    CString str;
    double fp;
    int i, nRow;

    if (fpLimNeg<0.0&&fpLimPos>0.0)   {
        /* settiamo il numero di righe e colonne */
        m_nRows=nNumPos+nNumNeg+2;
        m_grid.SetRowCount(m_nRows);
        /* inseriamo prima lo zero */
        str.Format(_T("%8.3f"), 0.0);
        m_grid.SetItemText(nNumNeg, 0, str);
        /* ora i limiti negativi */
        fp=fpStep;
        *fpNegInterval=0;
        for (i=0; i<nNumNeg; i++, fp+=fpStep)   {
            if (fp>fabs (fpLimNeg))
                fp=fabs (fpLimNeg);
            nRow=nNumNeg-i;
            m_grid.GetCell (nRow, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
            str.Format(_T("%8.3f"), -fp);
            m_grid.SetItemText(nRow,0,str);
            *fpNegInterval=-fpStep;
        }
        /* ora lo zero */
        nRow=nNumNeg+1;
        m_grid.GetCell (nRow, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
        str.Format(_T("%8.3f"), 0.0);
        m_grid.SetItemText(nRow,0,str);
        fp=fpStep;
        *fpPosInterval=0.0;
        /* ora i limiti positivi */
        for (i=0; i<nNumPos; i++, fp+=fpStep)   {
            if (fp>fpLimPos)
                fp=fpLimPos;
            nRow=nNumNeg+i+2;
            m_grid.GetCell (nRow, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
            str.Format(_T("%8.3f"), fp);
            m_grid.SetItemText(nRow,0,str);
            *fpPosInterval=fp;
        }
    }
    else
    if (fpLimNeg>=0.0&&fpLimPos>0.0)   {
        m_nRows=nNumPos+nNumNeg+1;
        m_grid.SetRowCount(m_nRows+1);
        fp=fpLimNeg;
        *fpNegInterval=fp-fpStep;
        for (i=0; i<nNumPos+nNumNeg+1; i++, fp+=fpStep)   {
            if (fp>fpLimPos)
                fp=fpLimPos;
            nRow=i+1;
            m_grid.GetCell (nRow, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
            str.Format(_T("%8.3f"), fp);
            m_grid.SetItemText(nRow,0,str);
        }
        *fpPosInterval=fp;
    }
    else   {
        m_nRows=nNumPos+nNumNeg+1;
        m_grid.SetRowCount(m_nRows+1);
        fp=fpLimPos;
        *fpNegInterval=fp+fpStep;
        nRow=m_nRows;
        for (i=0; i<nNumPos+nNumNeg+1; i++, fp-=fpStep, nRow--)   {
            if (fp<fpLimNeg)
                fp=fpLimNeg;
            m_grid.GetCell (nRow, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
            str.Format(_T("%8.3f"), fp);
            m_grid.SetItemText(nRow,0,str);
        }
        *fpPosInterval=fp;
    }
}

/*************
 * FillButtons :
 */
void CCavStrzDlg::FillButtons ()
{
    GetDlgItem (IDC_BTNAX1)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
    GetDlgItem (IDC_BTNAX2)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
    GetDlgItem (IDC_BTNAX3)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
    GetDlgItem (IDC_BTNAX4)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
    GetDlgItem (IDC_BTNAX5)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
}

/*
 * OnBtnAx1
 */
void CCavStrzDlg::OnBtnAx1() 
{
    POSITION pos;
    int i, nSel;
    bool bFound;
    if (!m_bChecked[0])   {
        for (pos=m_listAxes.GetHeadPosition(); pos;)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            bFound=false;
            for (i=0; i<NMAXAXESCORRETTED; i++)
                if (m_anAsseOccupato[i]&&!strcmp (m_anAsseOccupato[i]->GetName (), pAx->GetName()))   {
                    bFound=true;
                    break;
                }
            if (!bFound)   {
                nSel=((CComboBox*)GetDlgItem (IDC_CMBAX1))->AddString (pAx->GetName());
                if (m_pAx==pAx)   {
                    ((CComboBox*)GetDlgItem (IDC_CMBAX1))->SetCurSel (nSel);    
                    OnSelAsse1 ();
                }
            }
        }
        m_bChecked[0]=1;
        GetDlgItem(IDC_BTNAX1)->SetWindowText (LOADSTRING (IDS_ADAMOCC_3));
        ((CButton*)GetDlgItem(IDC_CHKAX1))->SetCheck (1);
    }
    else   {
        m_anAsseOccupato[0]=NULL;
        ((CComboBox*)GetDlgItem (IDC_CMBAX1))->ResetContent ();
        TutteLeRigheA ("", 1);
        m_bChecked[0]=0;
        GetDlgItem(IDC_BTNAX1)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
        ((CButton*)GetDlgItem(IDC_CHKAX1))->SetCheck (0);
        m_file.EmptyColumn (0);
    }
}

/*
 * OnBtnAx2
 */
void CCavStrzDlg::OnBtnAx2() 
{
    POSITION pos;
    int i, n=0, nSel;
    bool bFound;
    if (!m_bChecked[1])   {
        for (pos=m_listAxes.GetHeadPosition(); pos;)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            bFound=false;
            for (i=0; i<NMAXAXESCORRETTED; i++)
                if (m_anAsseOccupato[i]&&!strcmp (m_anAsseOccupato[i]->GetName (), pAx->GetName()))   {
                    bFound=true;
                    break;
                }
            if (!bFound)   {         
                nSel=((CComboBox*)GetDlgItem (IDC_CMBAX2))->AddString (pAx->GetName());
                if (n++==0)   {
                    ((CComboBox*)GetDlgItem (IDC_CMBAX2))->SetCurSel (nSel);    
                    OnSelAsse2 ();
                }
            }
        }
        m_bChecked[1]=1;
        GetDlgItem(IDC_BTNAX2)->SetWindowText (LOADSTRING (IDS_ADAMOCC_3));
        ((CButton*)GetDlgItem(IDC_CHKAX2))->SetCheck (1);
    }
    else   {
        m_anAsseOccupato[1]=NULL;
        ((CComboBox*)GetDlgItem (IDC_CMBAX2))->ResetContent ();
        TutteLeRigheA ("", 2);
        m_bChecked[1]=0;
        GetDlgItem(IDC_BTNAX2)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
        ((CButton*)GetDlgItem(IDC_CHKAX2))->SetCheck (0);
        m_file.EmptyColumn (1);
    }
}

/*
 * OnBtnAx3
 */
void CCavStrzDlg::OnBtnAx3() 
{
    POSITION pos;
    int i, n=0, nSel;
    bool bFound;
    if (!m_bChecked[2])   {
        for (pos=m_listAxes.GetHeadPosition(); pos;)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            bFound=false;
            for (i=0; i<NMAXAXESCORRETTED; i++)
                if (m_anAsseOccupato[i]&&!strcmp (m_anAsseOccupato[i]->GetName (), pAx->GetName()))   {
                    bFound=true;
                    break;
                }
            if (!bFound)   {
                nSel=((CComboBox*)GetDlgItem (IDC_CMBAX3))->AddString (pAx->GetName());
                if (n++==0)   {
                    ((CComboBox*)GetDlgItem (IDC_CMBAX3))->SetCurSel (nSel);    
                    OnSelAsse3 ();
                }
            }
        }
        m_bChecked[2]=1;
        GetDlgItem(IDC_BTNAX3)->SetWindowText (LOADSTRING (IDS_ADAMOCC_3));
        ((CButton*)GetDlgItem(IDC_CHKAX3))->SetCheck (1);
    }
    else   {
        m_anAsseOccupato[2]=NULL;
        ((CComboBox*)GetDlgItem (IDC_CMBAX3))->ResetContent ();
        TutteLeRigheA ("", 3);
        m_bChecked[2]=0;
        GetDlgItem(IDC_BTNAX3)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
        ((CButton*)GetDlgItem(IDC_CHKAX3))->SetCheck (0);
        m_file.EmptyColumn (2);
    }
}

/*
 * OnBtnAx4
 */
void CCavStrzDlg::OnBtnAx4() 
{
    POSITION pos;
    int i, n=0, nSel;
    bool bFound;
    if (!m_bChecked[3])   {
        for (pos=m_listAxes.GetHeadPosition(); pos;)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            bFound=false;
            for (i=0; i<NMAXAXESCORRETTED; i++)
                if (m_anAsseOccupato[i]&&!strcmp (m_anAsseOccupato[i]->GetName (), pAx->GetName()))   {
                    bFound=true;
                    break;
                }
            if (!bFound)   {
                nSel=((CComboBox*)GetDlgItem (IDC_CMBAX4))->AddString (pAx->GetName());
                if (n++==0)   {
                    ((CComboBox*)GetDlgItem (IDC_CMBAX4))->SetCurSel (nSel);
                    OnSelAsse4 ();
                }
            }

        }
        m_bChecked[3]=1;
        GetDlgItem(IDC_BTNAX4)->SetWindowText (LOADSTRING (IDS_ADAMOCC_3));
        ((CButton*)GetDlgItem(IDC_CHKAX4))->SetCheck (1);
    }
    else   {
        m_anAsseOccupato[3]=NULL;
        ((CComboBox*)GetDlgItem (IDC_CMBAX4))->ResetContent ();
        TutteLeRigheA ("", 4);
        m_bChecked[3]=0;
        GetDlgItem(IDC_BTNAX4)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
        ((CButton*)GetDlgItem(IDC_CHKAX4))->SetCheck (0);
        m_file.EmptyColumn (3);
    }
}

/*
 * OnBtnAx5
 */
void CCavStrzDlg::OnBtnAx5() 
{
    POSITION pos;
    int i, n=0, nSel;
    bool bFound;
    if (!m_bChecked[4])   {
        for (pos=m_listAxes.GetHeadPosition(); pos;)   {
            CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
            bFound=false;
            for (i=0; i<NMAXAXESCORRETTED; i++)
                if (m_anAsseOccupato[i]&&!strcmp (m_anAsseOccupato[i]->GetName (), pAx->GetName()))   {
                    bFound=true;
                    break;
                }
            if (!bFound)   {
                nSel=((CComboBox*)GetDlgItem (IDC_CMBAX5))->AddString (pAx->GetName());
                if (n++==0)   {
                    ((CComboBox*)GetDlgItem (IDC_CMBAX5))->SetCurSel (nSel);
                    OnSelAsse5 ();
                }
            }
        }
        m_bChecked[4]=1;
        GetDlgItem(IDC_BTNAX5)->SetWindowText (LOADSTRING (IDS_ADAMOCC_3));
        ((CButton*)GetDlgItem(IDC_CHKAX5))->SetCheck (1);
    }
    else   {
        m_anAsseOccupato[4]=NULL;
        ((CComboBox*)GetDlgItem (IDC_CMBAX5))->ResetContent ();
        TutteLeRigheA ("", 5);
        m_bChecked[4]=0;
        GetDlgItem(IDC_BTNAX5)->SetWindowText (LOADSTRING (IDS_ADAMOCC_2));
        ((CButton*)GetDlgItem(IDC_CHKAX5))->SetCheck (0);
        m_file.EmptyColumn (4);
    }
}

/*
 * InitialSettings :
 */
void CCavStrzDlg::InitialSettings ()
{
    char szBuffer[256];

    sprintf (szBuffer, "%d", m_nNumIntervals);
    m_editIntervals.SetWindowText (szBuffer);
    m_strIntervals = szBuffer;
    sprintf (szBuffer, "%8.3f", m_fpStep);
    m_strRange = szBuffer;
    m_editRange.SetWindowText (szBuffer);
    m_nInitialNumIntervals = m_nNumIntervals;
    m_fpInitialStep = m_fpStep;
    CheckRadioButton (IDC_64STEP, IDC_256STEP, IDC_256STEP);
}

int CCavStrzDlg::FirstFullRow ()
{
    int i, n;
    for (i=m_nRows-1; i>0; i--)   {
        for (n=0; n<NMAXAXESCORRETTED; n++)   {
            CString str=m_grid.GetItemText (i, n+1);
            if (str!=""&&str!="0.000")
                return i;
       }
    }
    return 0;
}

int CCavStrzDlg::Accorcia (int nRows)
{
    double fp;
    int nNumPos, nNumNeg;
    CString str;

    m_nNumIntervals=nRows;
    m_grid.DeleteNonFixedRows();
    m_fpStep=CalcInitialStep (m_fpLimPos, m_fpLimNeg);
    CalcNumCorrectors (m_fpStep, m_fpLimPos, m_fpLimNeg, &nNumPos, &nNumNeg);
    FillRows (m_fpStep, m_fpLimPos, m_fpLimNeg, nNumPos, nNumNeg, &fp, &fp);
    str.Format ("%8.3f", m_fpStep);
    m_editRange.SetWindowText (str);
    return 0;
}

int CCavStrzDlg::Accorcia (double fpStep)
{
    double fp;
    int nNumPos, nNumNeg;
    CString str;

    m_grid.DeleteNonFixedRows();
    m_fpStep=fpStep;
    CalcNumCorrectors (m_fpStep, m_fpLimPos, m_fpLimNeg, &nNumPos, &nNumNeg);
    FillRows (m_fpStep, m_fpLimPos, m_fpLimNeg, nNumPos, nNumNeg, &fp, &fp);
    m_nNumIntervals=m_grid.GetRowCount ()-2;
    str.Format ("%d", m_nNumIntervals);
    m_editIntervals.SetWindowText (str);
    return 0;
}

void CCavStrzDlg::OnOK() 
{
	// TODO: Add extra validation here
    CString strPath;
    double fpValue;
    char szBuffer[256], *p;
    int i, n;
    bool bError=false;

    strPath=GetPath() + m_strNomeFile;
	if (m_file.Open (strPath))   {
        m_file.SetNomeFile(m_strNomeFile);
        m_file.SetNomeAsse (m_pAx->GetName());
        m_file.SetLimits (m_fpLimNeg, m_fpLimPos);
        m_file.SetInterval (m_nNumIntervals);
        m_file.SetStep (m_fpStep);
        m_file.SetMaxRow (m_nRows);
        m_file.ClearAx();
        for (i=0; i<NMAXAXESCORRETTED; i++)   {
            if (m_anAsseOccupato[i])
                m_file.AddAx(m_anAsseOccupato[i]->GetName(), i);
            else
                m_file.AddAx("", i);
        }
        m_file.SetEnabled(((CButton*)GetDlgItem (IDC_CHKAX1))->GetCheck(),
                          ((CButton*)GetDlgItem (IDC_CHKAX2))->GetCheck(),
                          ((CButton*)GetDlgItem (IDC_CHKAX3))->GetCheck(),
                          ((CButton*)GetDlgItem (IDC_CHKAX4))->GetCheck(),
                          ((CButton*)GetDlgItem (IDC_CHKAX5))->GetCheck());
        for (i=0; i<m_nRows; i++)   {
            for (n=0; n<NMAXAXESCORRETTED; n++)   {
                strcpy (szBuffer, m_grid.GetItemText (i+1, n+1));
                if (strdblk(szBuffer)[0]!='\0')   {
                    fpValue=strtod (szBuffer, &p);
                    if (*p=='\0')
                        m_file.SetData(i, n, fpValue);
                }
            }
        }
        m_file.WriteData();
        m_file.Close();
    }
    else   {
        bError=true;
        AfxMessageBox (LOADSTRING (IDS_ADAMOCC_5));
    }
    if (!bError)
	    CDialog::OnOK();
}

CString CCavStrzDlg::GetPath ()
{
    CString str;
    CProject* pPrj=GETWS()->GetActiveProject ();
    if (pPrj)
        str=pPrj->GetCCPathName ();
    else
        str=LOADSTRING (IDS_ADAMOCC_6);
    return str;
}

void CCavStrzDlg::FillGridFromFile ()
{
    double fp, fpLimPos, fpLimNeg, fpNegInterval, fpPosInterval;
    int i, n, nNumPos, nNumNeg;
    bool bLimitsEnabled=false;
    char szBuffer[256];

    if (m_file.Open (GetPath()+m_strNomeFile, 1))   {
        m_file.ReadData();
        m_file.Close ();
        m_fpStep=m_file.GetStep();
        m_fpLimNeg=m_file.GetLimNeg();
        m_fpLimPos=m_file.GetLimPos();
        m_nNumIntervals=(int)m_file.GetInterval();
        m_nRows=m_file.GetMaxRow();
        bLimitsEnabled=GetLimits (&fpLimPos, &fpLimNeg);
        EnableAxes ();
        CalcNumCorrectors (m_fpStep, m_fpLimPos, m_fpLimNeg, &nNumPos, &nNumNeg);        
        FillRows(m_fpStep, m_fpLimPos, m_fpLimNeg, nNumPos, nNumNeg, &fpNegInterval, &fpPosInterval);
        for (i=0; i<m_nRows; i++)   {
            for (n=0; n<NMAXAXESCORRETTED; n++)   {
                fp=m_file.GetData(i, n);
                if (fp!=NOVALUE)   {
                    sprintf (szBuffer, "%8.3f", fp);
                    m_grid.SetItemText(i+1, n+1, szBuffer);
                }
            }
        }
        if (bLimitsEnabled&&(IsLimitChanged (m_fpLimPos, fpLimPos, m_fpStep, true)
                           ||IsLimitChanged (m_fpLimNeg, fpLimNeg, m_fpStep, false)))   {
                int nNumInPiuPos = 0, nNumInPiuNeg = 0;
                /* i limiti sono ancora abilitati, ma evidentemente sono cambiati i range */
                if (IsLimitChanged (m_fpLimNeg, fpLimNeg, m_fpStep, false))   {
                int nOldLimitInStep=(int)(m_fpLimNeg/m_fpStep);
                if (ISEQUALLASCO (fmod (m_fpLimNeg, m_fpStep), 0))
                    m_fpLimNeg > 0 ? nOldLimitInStep-- : nOldLimitInStep++;
                int nNewLimitInStep=(int)(fpLimNeg/m_fpStep);
                if (ISEQUALLASCO (fmod (fpLimNeg, m_fpStep), 0))
                    fpLimNeg > 0 ? nNewLimitInStep-- : nNewLimitInStep++;
                nNumInPiuNeg=abs (nNewLimitInStep-nOldLimitInStep);
                if (nNewLimitInStep==nOldLimitInStep)   {
                    sprintf (szBuffer, "%8.3f", fpLimNeg);
                    m_grid.SetItemText (1, 0, szBuffer);
                }
                else   {
                    /* ora aggiungiamo o togliamo righe al grid */
                    if (fpLimNeg<m_fpLimNeg)   {
                        sprintf (szBuffer, "%8.3f", fpNegInterval);
                        m_grid.SetItemText (1, 0, szBuffer);
                        fp=fpNegInterval-m_fpStep;
                        for (i=0; i<nNumInPiuNeg; i++)   {
                            sprintf (szBuffer, "%8.3f", fp);
                            m_grid.InsertRow (szBuffer, 1);
                            m_grid.GetCell (1, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
                            for (n=0; n<NMAXAXESCORRETTED; n++)
                                if (m_bChecked[n])   {
                                    sprintf (szBuffer, "%8.3f", 0.0);
                                    m_grid.SetItemText (1, n+1, szBuffer);
                                }
                            fp-=m_fpStep;
                        }
                        sprintf (szBuffer, "%8.3f", fpLimNeg);
                        m_grid.SetItemText (1, 0, szBuffer);
                        m_nNumIntervals+=nNumInPiuNeg;
                        m_nRows+=nNumInPiuNeg;
                    }
                    else   {
                        for (i=0; i<nNumInPiuNeg; i++)
                            m_grid.DeleteRow (1);
                        sprintf (szBuffer, "%8.3f", fpLimNeg);
                        m_grid.SetItemText (1, 0, szBuffer);
                        m_nNumIntervals-=nNumInPiuNeg;
                        m_nRows-=nNumInPiuNeg;
                    }
                }
                m_fpLimNeg=fpLimNeg;
            }
                if (IsLimitChanged (m_fpLimPos, fpLimPos, m_fpStep, true))   {
                int nOldLimitInStep=(int)(m_fpLimPos/m_fpStep);
                if (ISEQUALLASCO (fmod (m_fpLimPos, m_fpStep), 0))
                    m_fpLimPos < 0 ? nOldLimitInStep++ : nOldLimitInStep--;
                int nNewLimitInStep=(int)(fpLimPos/m_fpStep);
                if (ISEQUALLASCO (fmod (fpLimPos, m_fpStep), 0))
                    fpLimPos < 0 ? nNewLimitInStep++ : nNewLimitInStep--;
                nNumInPiuPos=abs (nNewLimitInStep-nOldLimitInStep);
                if (nNewLimitInStep==nOldLimitInStep)   {
                    sprintf (szBuffer, "%8.3f", fpLimPos);
                    m_grid.SetItemText (m_grid.GetRowCount()-1, 0, szBuffer);
                }
                else   {
                    /* ora aggiungiamo o togliamo righe al grid */
                    if (fpLimPos>m_fpLimPos)   {
                        sprintf (szBuffer, "%8.3f", fpPosInterval);
                        m_grid.SetItemText (m_grid.GetRowCount()-1, 0, szBuffer);
                        fp=fpPosInterval+m_fpStep;
                        for (i=0; i<nNumInPiuPos; i++)   {
                            sprintf (szBuffer, "%8.3f", fp);
                            m_grid.InsertRow (szBuffer);
                            m_grid.GetCell (i+1, 0)->SetFormat(DT_CENTER|DT_VCENTER|DT_SINGLELINE);
                            for (n=0; n<NMAXAXESCORRETTED; n++)
                                if (m_bChecked[n])   {
                                    sprintf (szBuffer, "%8.3f", 0.0);
                                    m_grid.SetItemText (i+1, n+1, szBuffer);
                                }
                            fp+=m_fpStep;
                        }
                        sprintf (szBuffer, "%8.3f", fpLimPos);
                        m_grid.SetItemText (m_grid.GetRowCount ()-1, 0, szBuffer);
                        m_nNumIntervals+=nNumInPiuPos;
                        m_nRows+=nNumInPiuPos;
                    }
                    else   {
                        for (i=0; i<nNumInPiuPos; i++)
                            m_grid.DeleteRow (m_grid.GetRowCount ()-1);
                        sprintf (szBuffer, "%8.3f", fpLimPos);
                        m_grid.SetItemText (m_grid.GetRowCount ()-1, 0, szBuffer);
                        m_nNumIntervals-=nNumInPiuPos;
                        m_nRows-=nNumInPiuPos;
                    }
                }
                m_fpLimPos=fpLimPos;
            }
            }
            else   {
            if (!bLimitsEnabled)   {
                /* limiti assi non piu' abilitati, cancelliamo il file */
                m_grid.EnableWindow (false);
                AfxMessageBox (LOADSTRING (IDS_ADAMOCC_1));
            }
        }
        sprintf (szBuffer, "%d", m_nNumIntervals);
        m_editIntervals.SetWindowText (szBuffer);
    }
}

/************
 * EnableAxes : la funzione abilita i check box di abilitazione assi
 */
void CCavStrzDlg::EnableAxes ()
{
    CStringArray& sl=m_file.GetAxList();
    int nAx1, nAx2, nAx3, nAx4, nAx5;
    int nIndex;

    m_file.GetEnabled(&nAx1, &nAx2, &nAx3, &nAx4, &nAx5);
    if (sl[0]!="")   {
        OnBtnAx1();
        if ((nIndex=((CComboBox*)GetDlgItem (IDC_CMBAX1))->FindString (-1, sl[0]))>=0)   {
            ((CComboBox*)GetDlgItem (IDC_CMBAX1))->SetCurSel(nIndex);
            SettaAsseOccupato (0, sl[0]);
    }
    }
    if (sl[1]!="")   {
        OnBtnAx2();
        if ((nIndex=((CComboBox*)GetDlgItem (IDC_CMBAX2))->FindString (-1, sl[1]))>=0)   {
            ((CComboBox*)GetDlgItem (IDC_CMBAX2))->SetCurSel(nIndex);
            SettaAsseOccupato (1, sl[1]);
        }
    }
    if (sl[2]!="")   {
        OnBtnAx3();
        if ((nIndex=((CComboBox*)GetDlgItem (IDC_CMBAX3))->FindString (-1, sl[2]))>=0)   {
            ((CComboBox*)GetDlgItem (IDC_CMBAX3))->SetCurSel(nIndex);
            SettaAsseOccupato (2, sl[2]);
    }
    }
    if (sl[3]!="")   {
        OnBtnAx4();
        if ((nIndex=((CComboBox*)GetDlgItem (IDC_CMBAX4))->FindString (-1, sl[3]))>=0)   {
            ((CComboBox*)GetDlgItem (IDC_CMBAX4))->SetCurSel(nIndex);
            SettaAsseOccupato (3, sl[3]);
    }
    }
    if (sl[4]!="")   {
        OnBtnAx5();
        if ((nIndex=((CComboBox*)GetDlgItem (IDC_CMBAX5))->FindString (-1, sl[4]))>=0)   {
            ((CComboBox*)GetDlgItem (IDC_CMBAX5))->SetCurSel(nIndex);
            SettaAsseOccupato (4, sl[4]);
    }
}
    if (nAx1)
        ((CButton*)GetDlgItem(IDC_CHKAX1))->SetCheck (1);
    else
        ((CButton*)GetDlgItem(IDC_CHKAX1))->SetCheck (0);
    if (nAx2)
        ((CButton*)GetDlgItem(IDC_CHKAX2))->SetCheck (1);
    else
        ((CButton*)GetDlgItem(IDC_CHKAX2))->SetCheck (0);
    if (nAx3)
        ((CButton*)GetDlgItem(IDC_CHKAX3))->SetCheck (1);
    else
        ((CButton*)GetDlgItem(IDC_CHKAX3))->SetCheck (0);
    if (nAx4)
        ((CButton*)GetDlgItem(IDC_CHKAX4))->SetCheck (1);
    else
        ((CButton*)GetDlgItem(IDC_CHKAX4))->SetCheck (0);
    if (nAx5)
        ((CButton*)GetDlgItem(IDC_CHKAX5))->SetCheck (1);
    else
        ((CButton*)GetDlgItem(IDC_CHKAX5))->SetCheck (0);
}

void CCavStrzDlg::OnBtnApply()
{
	// TODO: Add your control notification handler code here
    CString str;
    double fp;
    int n;
	char szBuffer[256], *c;

    m_editRange.GetWindowText (szBuffer, sizeof (szBuffer));
    str = szBuffer;
    strdblk (szBuffer);
    if (szBuffer[0]!='\0')   {
        fp=strtod (szBuffer, &c);
        if (!ISEQUAL (fp, m_fpInitialStep))   {
            if (AfxMessageBox (LOADSTRING (IDS_ADAMOCC_10), MB_YESNO) == IDYES)   {
                if (*c=='\0')   {
                    Accorcia (fp);
                    AzzeraColonneAbilitate ();
                    m_fpInitialStep = fp;
                    m_strRange = str;
                }
                else
                    m_editRange.SetWindowText (m_strRange);
            }
            else
                m_editRange.SetWindowText (m_strRange);
        }
    }
    else   {
        m_editIntervals.GetWindowText (szBuffer, sizeof (szBuffer));
        strdblk (szBuffer);
        str = szBuffer;
        fp=strtod (szBuffer, &c);
        if (*c=='\0')   {
            n=(int)fp;
            if (n != m_nInitialNumIntervals)   {
                if (AfxMessageBox (LOADSTRING (IDS_ADAMOCC_10), MB_YESNO) == IDYES)
                    if (n>=2&&n<=NUMINTERVALLI_256)   {
                        Accorcia (n);
                        AzzeraColonneAbilitate ();
                        m_strIntervals = str;
                    }
                    else   {
                        m_editIntervals.SetWindowText (m_strIntervals);
                        AfxMessageBox (LOADSTRING (IDS_ADAMOCC_7));
            }
        }
    }
        else
            m_editIntervals.SetWindowText (m_strIntervals);
    }
}

void CCavStrzDlg::AzzeraColonneAbilitate ()
{
    for (int i = 0; i<5; i++)
        if (IsColumnChecked (i+1))
            TutteLeRigheA ("0.000", i+1);
}

bool CCavStrzDlg::IsLimitChanged (double fpFileLimit, double fpAxLimit, double fpStep, bool bPositive) 
{
    if (bPositive)   {
        if (ISEQUALLASCO (fpAxLimit, fpFileLimit))
            return false;
    }
    else   {
        if (ISEQUALLASCO (fpAxLimit, fpFileLimit))
            return false;
    }
    return true;
}

void CCavStrzDlg::OnBtnDeleteCCFile()
{
    CPath p (GetPath()+m_strNomeFile);
    if (AfxMessageBox (LOADSTRING (IDS_ADAMOCC_8), MB_YESNO)==IDYES)   {
        if (!_access (p, 00))
            CFile::Remove (p);
        m_strNomeFile="";
        OnCancel ();
    }
}

/*
** SettaAsseOccupato :
*/
void CCavStrzDlg::SettaAsseOccupato (int n, CString strNomeAsse)
{
    POSITION pos;

    for (pos=m_listAxes.GetHeadPosition(); pos;)   {
        CAdamoAxis* pAx=(CAdamoAxis*) m_listAxes.GetNext(pos);
        if (pAx->GetName ()==strNomeAsse)
            m_anAsseOccupato[n]=pAx;
    }
}



/* ricordarsi di subclassare con CParseEdit l'edit control del numero di passi */
/* radio button da riposizionare in modo corretto dopo un errore */
/* l'apply sega le quote dal grid, incredible */