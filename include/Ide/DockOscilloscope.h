/*
** DockOscilloscope.h
*/

#ifndef __ADAMO_DOCKOSCOLLOSCOPE__
#define __ADAMO_DOCKOSCOLLOSCOPE__

class CDockOscilloscope
{
public:
	CDockOscilloscope();
	virtual ~CDockOscilloscope();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)   { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()   { return m_pDockingPane; }
    COscView *GetView()   { return m_pOscView; }
protected:
    COscView *m_pOscView;
    CXTPDockingPane *m_pDockingPane;
};

#endif
