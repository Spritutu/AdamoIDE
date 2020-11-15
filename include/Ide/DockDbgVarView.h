/*
** DockDbgVarView.h
*/

#ifndef __ADAMO_DOCKDBGVARVIEW__
#define __ADAMO_DOCKDBGVARVIEW__

/* forward reference */
class CAdamoDbgVarView;

class CDockDbgVarView
{
public:
	CDockDbgVarView();
	virtual ~CDockDbgVarView();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)   { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()   { return m_pDockingPane; }
    CAdamoDbgVarView *GetView()   { return m_pDbgVarView; }
protected:
    CAdamoDbgVarView *m_pDbgVarView;
    CXTPDockingPane *m_pDockingPane;
};

#endif
