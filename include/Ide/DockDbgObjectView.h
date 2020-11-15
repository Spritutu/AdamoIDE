#pragma once

/* forward reference */
class CAdamoDbgObjectView;

class CDockDbgObjectView
{
public:
	CDockDbgObjectView(void);
	virtual ~CDockDbgObjectView(void);
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)   { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()   { return m_pDockingPane; }
    CAdamoDbgObjectView *GetView()   { return m_pDbgObjectView; }
protected:
    CAdamoDbgObjectView *m_pDbgObjectView;
    CXTPDockingPane *m_pDockingPane;
};
