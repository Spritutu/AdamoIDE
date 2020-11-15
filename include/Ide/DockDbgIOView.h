/*
** DockDbgIOView.h
*/

#ifndef __ADAMO_DOCKDBGIOVIEW__
#define __ADAMO_DOCKDBGIOVIEW__

/* forward reference */
class CAdamoDbgIOView;

class CDockDbgIOView
{
public:
	CDockDbgIOView();
	virtual ~CDockDbgIOView();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)   { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()   { return m_pDockingPane; }
    CAdamoDbgIOView *GetView()   { return m_pDbgIOView; }
protected:
    CAdamoDbgIOView *m_pDbgIOView;
    CXTPDockingPane *m_pDockingPane;
};

#endif
