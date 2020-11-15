// VariablesBar.cpp: implementation of the CVariablesBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "mainframe.h"
#include "VariablesBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


void CVariablesBar::AddVariable(const char *szName, const char *szType, const char *szValue)
{
    if(m_pVarListView){
        CListCtrl &cListCtrl = m_pVarListView->GetListCtrl();
	    int idx = cListCtrl.InsertItem(cListCtrl.GetItemCount(), szName);
	    cListCtrl.SetItem(idx, 1, LVIF_TEXT, szType, 0, LVIF_TEXT, LVIF_TEXT, 0);
	    cListCtrl.SetItem(idx, 2, LVIF_TEXT, szValue, 0, LVIF_TEXT, LVIF_TEXT, 0);
    }
}

void CVariablesBar::RemoveAll()
{
    if(m_pVarListView)
        m_pVarListView->GetListCtrl().DeleteAllItems();
}

void CVariablesBar::Init(CWnd *pWnd, int nId)
{
    CXTPDockingPane* pPane=((CMainFrame *)pWnd)->GetPaneManager()->FindPane (nId);
    if (pPane)
        m_pVarListView = (CVarListView*)pPane->AttachView(pWnd,RUNTIME_CLASS(CVarListView));
}
