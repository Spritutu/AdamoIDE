#if !defined(AFX_DEDITOR_H__C84FB781_CCDF_4AC5_A2A2_F5AD2648F0C4__INCLUDED_)
#define AFX_DEDITOR_H__C84FB781_CCDF_4AC5_A2A2_F5AD2648F0C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DEditor.h : header file
//

#include "AdamoRSObjects.h"
#include "DiagramEditor.h"

/////////////////////////////////////////////////////////////////////////////
// CDEditor window

class CAdamoRSEditor : public CDiagramEditor
{
public:
	virtual void DrawBackground( CDC* dc, CRect rect, double zoom ) const;
	virtual void DrawGrid( CDC* dc, CRect rect, double zoom ) const;
    virtual void DrawSelectionMarkers( CDC* dc ) const;
    virtual CDiagramEntity* GetSelectedObject() const;
    virtual void TabModified ();
protected :
};

class CAdamoRSContainer : public CDiagramEntityContainer
{
    public :
        CAdamoRSContainer () : m_pSelect (NULL), m_pctrlList (NULL)  { }
        void SetSelectedObj (CDiagramEntity* pSelect) { m_pSelect=pSelect; } 
        CDiagramEntity* GetSelectedObj () { return m_pSelect; } 
        void ClearSelection ()            { m_pSelect=NULL; }
        void SetCtrlList (CAdamoRSCtrlList* ctrlList);
        CAdamoRSCtrlList* GetCtrlList ();
        void Load ();
    	virtual void Add( CDiagramEntity* obj );
	    virtual void RemoveAt( int index );
	    virtual void RemoveAll();
	    virtual void Front (CDiagramEntity* obj);
	    virtual void Bottom (CDiagramEntity* obj);
    private :
        CDiagramEntity*     m_pSelect;
        CAdamoRSCtrlList*   m_pctrlList;
    private :
        void AddDiagramButton (CAdamoRSControl* p);
        void AddDiagramEdit   (CAdamoRSControl* p);
        void AddDiagramRadioButton (CAdamoRSControl* p);
        void AddDiagramCheckBox (CAdamoRSControl* p);
        void AddDiagramLabel (CAdamoRSControl* p);
        void AddDiagramGroup (CAdamoRSControl* p);
        void AddDiagramCombo (CAdamoRSControl* p);
        void AddDiagramList (CAdamoRSControl* p);
        void AddDiagramImage (CAdamoRSControl* p);
		void AddDiagramGrid (CAdamoRSControl* p);
		void AddDiagramAx (CAdamoRSControl* p);
		void AddDiagramLed (CAdamoRSControl* p);
		void AddDiagramIAngularGauge (CAdamoRSControl* p);
		void AddDiagramILinearGauge (CAdamoRSControl* p);
		void AddDiagramIDisplay (CAdamoRSControl* p);
		void AddDiagramILabel (CAdamoRSControl* p);
        void AddDiagramIPicture (CAdamoRSControl* p);
        bool DeleteControl (CAdamoRSControl *pRS);
};

#endif // !defined(AFX_DEDITOR_H__C84FB781_CCDF_4AC5_A2A2_F5AD2648F0C4__INCLUDED_)
