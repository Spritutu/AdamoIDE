/*
** DockDGSdo.h
*/

#ifndef __ADAMO_DOCKDGSDO__
#define __ADAMO_DOCKDGSDO__

class CDockSGSDO
{
public:
	CDockSGSDO();
	virtual ~CDockSGSDO();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)   { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()   { return m_pDockingPane; }
    CDGSdoView *GetView()   { return m_pDGSdoView; }
protected:
    CDGSdoView *m_pDGSdoView;
    CXTPDockingPane *m_pDockingPane;
};

#endif
