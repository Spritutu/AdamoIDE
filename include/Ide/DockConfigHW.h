/*
** DockCongifHW.h
*/

#ifndef __ADAMO_DOCKCONFIGHW__
#define __ADAMO_DOCKCONFIGHW__

class CDockConfigHW
{
public:
	CDockConfigHW();
	virtual ~CDockConfigHW();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)   { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()   { return m_pDockingPane; }
    CAdamoConfigHWView *GetView()   { return m_pConfigHWView; }
protected:
    CAdamoConfigHWView *m_pConfigHWView;
    CXTPDockingPane    *m_pDockingPane;
};

#endif
