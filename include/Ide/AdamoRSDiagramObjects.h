/*
** AdamoRSDiagramObjects.h 
*/

#ifndef __ADAMO_ADAMORSDIAGRAMOBJECTS__
#define __ADAMO_ADAMORSDIAGRAMOBJECTS__

/* include files */
#include "DiagramEntity.h"
#include "AdamoRSObjects.h"
#include "ximage.h"

class CAdamoRSDiagramObjects : public CDiagramEntity
{
    public :
        CAdamoRSDiagramObjects () : m_pRSCtrl (NULL)   { }
        void SetRSControl (CAdamoRSControl* pCtrl)      { m_pRSCtrl=pCtrl; }
        CAdamoRSControl* GetRSControl ()                { return m_pRSCtrl; }
        virtual void SetRect (CRect rect);
	    virtual void SetRect (double left, double top, double right, double bottom);
	    virtual void MoveRect (double x, double y);
	    virtual void SetLeft (double left);
	    virtual void SetRight (double right);
	    virtual void SetTop (double top);
	    virtual void SetBottom (double bottom);
        virtual void DrawSelectionMarkers( CDC* dc, CRect rect ) const;
        void UpdateSelected ();
    private :
        CAdamoRSControl* m_pRSCtrl;
};

class CAdamoRSDiagramButton : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramButton)

// Construction
public:
	CAdamoRSDiagramButton();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
    virtual bool    IsButton ()                 { return true; }
};

class CAdamoRSDiagramEdit : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramEdit)

// Construction
public:
	CAdamoRSDiagramEdit();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
};

class CAdamoRSDiagramRadioButton : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramRadioButton)

// Construction
public:
	CAdamoRSDiagramRadioButton();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
};

class CAdamoRSDiagramCheckBox : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramCheckBox)

// Construction
public:
	CAdamoRSDiagramCheckBox();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
};

class CAdamoRSDiagramGroupBox : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramGroupBox)

// Construction
public:
	CAdamoRSDiagramGroupBox ();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    virtual bool    IsTabbeble ()   { return false; }
    void    Select( BOOL selected );
};

class CAdamoRSDiagramLabel : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramLabel)

// Construction
public:
	CAdamoRSDiagramLabel ();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
};

class CAdamoRSDiagramPicture : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramPicture)

// Construction
public:
	CAdamoRSDiagramPicture();
	~CAdamoRSDiagramPicture();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select (BOOL selected);
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
    void    SetImage (CxImage* pImage)   { _delete (m_pImage); m_pImage=pImage; }
    void    SetTransparent (bool b)      { m_bIsTransparent = b; }
    void    SetTransparentColor (COLORREF rgb)   { m_rgbTransparentColor = rgb; }
private :
    CxImage*    m_pImage;
    bool        m_bIsTransparent;
    COLORREF    m_rgbTransparentColor;
};

class CAdamoRSDiagramList : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramList)

// Construction
public:
	CAdamoRSDiagramList ();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
};

class CAdamoRSDiagramCombo : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramCombo)

// Construction
public:
	CAdamoRSDiagramCombo ();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
};

class CAdamoRSDiagramImage : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramImage)

// Construction
public:
	CAdamoRSDiagramImage();
	~CAdamoRSDiagramImage();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select (BOOL selected);
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
    void    SetImage (CxImage* pImage)   { _delete (m_pImage); m_pImage=pImage; }
    void    SetTransparent (bool b)      { m_bIsTransparent = b; }
    void    SetTransparentColor (COLORREF rgb)   { m_rgbTransparentColor = rgb; }
private :
    CxImage*    m_pImage;
    bool        m_bIsTransparent;
    COLORREF    m_rgbTransparentColor;
};

class CAdamoRSDiagramGrid : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramGrid)

// Construction
public:
	CAdamoRSDiagramGrid ();

	static	CDiagramEntity* CreateFromString( const CString& str );

// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
};

class CAdamoRSDiagramAx : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramAx)

// Construction
public:
	CAdamoRSDiagramAx();
	~CAdamoRSDiagramAx();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
private :
	CBitmap *m_pBmp;
};

class CAdamoRSDiagramLed : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramLed)

// Construction
public:
	CAdamoRSDiagramLed();
	~CAdamoRSDiagramLed();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
private :
	CBitmap *m_pBmp;
};

class CAdamoRSDiagramIAngularGauge : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramIAngularGauge)

// Construction
public:
	CAdamoRSDiagramIAngularGauge();
	~CAdamoRSDiagramIAngularGauge();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
private :
	CBitmap *m_pBmp;
};

class CAdamoRSDiagramILinearGauge : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramILinearGauge)

// Construction
public:
	CAdamoRSDiagramILinearGauge();
	~CAdamoRSDiagramILinearGauge();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
private :
	CBitmap *m_pBmp;
};

class CAdamoRSDiagramISevenSegment : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramISevenSegment)

// Construction
public:
	CAdamoRSDiagramISevenSegment();
	~CAdamoRSDiagramISevenSegment();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
private :
	CBitmap *m_pBmp;
};

class CAdamoRSDiagramILabel : public CAdamoRSDiagramObjects
{
    DECLARE_SERIAL(CAdamoRSDiagramILabel)

// Construction
public:
	CAdamoRSDiagramILabel();
	~CAdamoRSDiagramILabel();

	static	CDiagramEntity* CreateFromString( const CString& str );
// Overrides
	virtual CDiagramEntity* Clone();
	virtual void			Draw( CDC* dc, CRect rect );
    void    Select( BOOL selected );
    virtual int     LoadTabIndex ();
    virtual void    SaveTabIndex ();
private :
	CBitmap *m_pBmp;
};

#endif
