// VariablesBar.h: interface for the CVariablesBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VARIABLESBAR_H__233488EE_2350_4228_B266_DA29C45596BA__INCLUDED_)
#define AFX_VARIABLESBAR_H__233488EE_2350_4228_B266_DA29C45596BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VarListView.h"

class CVariablesBar
{
public:
	void Init(CWnd *pWnd, int nId);
    CVariablesBar():m_pVarListView(NULL){};
    virtual ~CVariablesBar(){};
	void RemoveAll();
	void AddVariable(const char* szName, const char* szType, const char* szValue);
    void SetPtrDockingPane(CXTPDockingPane *pPane){m_pDockingPane = pPane;};
    CXTPDockingPane *GetPtrDockingPane(){return m_pDockingPane;};
    CVarListView *GetVarLocView(){return m_pVarListView;};
protected:
    CVarListView *m_pVarListView;
    CXTPDockingPane *m_pDockingPane;
};

#endif // !defined(AFX_VARIABLESBAR_H__233488EE_2350_4228_B266_DA29C45596BA__INCLUDED_)
