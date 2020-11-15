/*
** DockRS.h
*/

#ifndef __ADAMO_DOCKRS__
#define __ADAMO_DOCKRS__

/* include files */
#include "AdamoRSCatalogue.h"
#include "AdamoRSProperties.h"
#include "AdamoRSActiveXPreview.h"

/* class definition */
class CDockRSCatalogue
{
// Construction
public:
	CDockRSCatalogue();
	virtual ~CDockRSCatalogue();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CAdamoRSCatalogue *GetView()                    { return m_pRSCatalogue; }
protected:
    CAdamoRSCatalogue *m_pRSCatalogue;
    CXTPDockingPane *m_pDockingPane;
};

class CDockRSProperties
{
// Construction
public:
	CDockRSProperties();
	virtual ~CDockRSProperties();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CAdamoRSProperties *GetView()                   { return m_pRSProperties; }
protected:
    CAdamoRSProperties* m_pRSProperties;
    CXTPDockingPane *m_pDockingPane;
};

class CDockRSActiveXPreview
{
// Construction
public:
	CDockRSActiveXPreview();
	virtual ~CDockRSActiveXPreview();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CAdamoRSActiveXPreview *GetView()               { return m_pRSActiveXPreview; }
protected:
    CAdamoRSActiveXPreview* m_pRSActiveXPreview;
    CXTPDockingPane *m_pDockingPane;
};

#endif

