// DockLangView.h: interface for the CDockLangView class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CAdamoListAllErrors;
class CAdamoListInterpreterErrors;
class CAdamoListPLCErrors;
class CAdamoListISOErrors;
class CAdamoListHardwareErrors;
class CAdamoListMessages;

class CDockListAllErrors
{
// Construction
public:
	CDockListAllErrors();
	virtual ~CDockListAllErrors();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CAdamoListAllErrors *GetView();
protected:
    CAdamoListAllErrors *m_pListAllErrors;
    CXTPDockingPane *m_pDockingPane;
};

class CDockListInterpreterErrors
{
// Construction
public:
	CDockListInterpreterErrors();
	virtual ~CDockListInterpreterErrors();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CAdamoListInterpreterErrors *GetView();
protected:
    CAdamoListInterpreterErrors *m_pListInterpreterErrors;
    CXTPDockingPane *m_pDockingPane;
};

class CDockListPlcErrors
{
// Construction
public:
	CDockListPlcErrors();
	virtual ~CDockListPlcErrors();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CAdamoListPLCErrors *GetView();
protected:
    CAdamoListPLCErrors *m_pListPlcErrors;
    CXTPDockingPane *m_pDockingPane;
};

class CDockListISOErrors
{
// Construction
public:
	CDockListISOErrors();
	virtual ~CDockListISOErrors();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CAdamoListISOErrors *GetView();
protected:
    CAdamoListISOErrors *m_pListISOErrors;
    CXTPDockingPane *m_pDockingPane;
};

class CDockListHardwareErrors
{
// Construction
public:
	CDockListHardwareErrors();
	virtual ~CDockListHardwareErrors();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CAdamoListHardwareErrors *GetView();
protected:
    CAdamoListHardwareErrors *m_pListHardwareErrors;
    CXTPDockingPane *m_pDockingPane;
};

class CDockListMessages
{
// Construction
public:
	CDockListMessages();
	virtual ~CDockListMessages();
	void Init(CWnd *pWnd, int nId);
    void SetPtrDockingPane(CXTPDockingPane *pPane)  { m_pDockingPane = pPane; }
    CXTPDockingPane *GetPtrDockingPane()            { return m_pDockingPane; }
    CAdamoListMessages *GetView();
protected:
    CAdamoListMessages *m_pListMessages;
    CXTPDockingPane *m_pDockingPane;
};
