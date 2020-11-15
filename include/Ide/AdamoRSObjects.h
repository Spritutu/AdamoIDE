/*
** AdamoRSObjects.h
*/

#ifndef __ADAMO_RS_OBJECTS__
#define __ADAMO_RS_OBJECTS__

/* external C reference */
extern "C"   {
    #include "lua.h"
}

/* include files */
#include "adamoglobal.h"
#include "AdamoTraduzione.h"
#include "lexxer.h"

/* constants definitions */
#define DEFAULT_RSCTRL_ID   1000
#define MAX_RSOPTIONS       64
#define MAX_RSCONTROLS      128
#define NMAX_IPICTURE_IMAGES 8

/* import directive */
#import <msxml6.dll> 
using namespace MSXML2;

/* type definition */
typedef MSXML2::IXMLDOMDocumentPtr		DocXmlPtr;
typedef MSXML2::IXMLDOMElementPtr		ElementXmlPtr;
typedef MSXML2::IXMLDOMNodePtr			NodeXmlPtr ;
typedef MSXML2::IXMLDOMNodeListPtr		NodeListXmlPtr;
typedef MSXML2::IXMLDOMNamedNodeMapPtr	NodeMapPtr;

/* control enumeration */
enum eRSCtrl   {
    RSNoControl,
    RSForm,
    RSLabel,
    RSInputText,
    RSInputRadio,
    RSInputCheck,
    RSGroup,
    RSButton,
    RSList,
    RSCombo,
    RSImage,
    RSGrid,
	RSAx,
	RSILed,
	RSIAngularGauge,
	RSILinearGauge,
	RSIDisplay,
	RSILabel,
    RSIPicture
};

/* alignment enumeration */
enum eRSAlignment   {
    leftAlignment,
    centerAlignment,
    rigthAlignment,
    justifyAlignment
};

enum eRSBorderStyle   {
    dottedStyle,
    solidStyle,
    dashedStyle,
    doubleStyle,
    grooveStyle,
    ridgeStyle,
    insetStyle,
    outsetStyle
};

/* button type */
enum eRSButtonType   {
    eStandard,
    eBitmap
};

/* input text type */
enum eEditType
{
	eNone,
	eNumbers,
	eFloat,
	eHex,
	eLetters
};

/* struct definitions */
struct stRSFont   {
    public :
        stRSFont () : strFaceName ("MS Sans Serif"), nSize (8), bStrikeOut (false), bUnderLine (false), bBold (false), bItalic (false)   { }
        CString strFaceName;
        int     nSize;
        bool    bStrikeOut;
        bool    bUnderLine;
        bool    bBold;
        bool    bItalic;
};

struct stRSBorder   {
    public :
        stRSBorder () : nBorderStyle (solidStyle), nSize (1), bAutomaticColor (true), color (RGB(0, 0, 0)), padding_left (0),
            padding_top (0), padding_rigth (0), padding_bottom (0), HasABorder (false)   { }
        bool            HasABorder;
        eRSBorderStyle  nBorderStyle;
        int             nSize;
        bool            bAutomaticColor;
        COLORREF        color;
        int             padding_left; 
        int             padding_top; 
        int             padding_rigth; 
        int             padding_bottom; 
};

struct stRSOpzioni   {
    public :
        stRSOpzioni () : m_bSelected (false)   { }
        CString strOpzione;
        bool    m_bSelected;
        CString strValue;
};

struct stRSVariabili   {
    public :
        stRSVariabili () : m_nRow (-1), m_nType (LUA_TNUMBER)   { }
        CString m_strName;
        CString m_strCol;
        int     m_nRow;
        int     m_nType;
        CString m_strTable;
        CString m_strField;
        CString m_strKeyName;
        CString m_strKeyValue;
        void SaveXML (CString& str, int nID);
        void Load (CString  strLoad);
        void AzzeraVariabili ()     { m_strName=""; m_strCol=""; m_nRow=-1; m_nType=LUA_TNUMBER; }
        void AzzeraDB ()            { m_strTable=""; m_strField=""; m_strKeyName=""; m_strKeyValue=""; }
};

/* class definitions */
class CAdamoRSStyle   {
    private :
        CRect           m_rc;
        stRSFont        m_rsFont;
        stRSBorder      m_rsBorder;
        bool            m_bAutomaticColor;
        COLORREF        m_rsColor;
        bool            m_bAutomaticBackColor;
        COLORREF        m_rsBackColor;
        eRSAlignment    m_rsAlignment;
    public :
        CAdamoRSStyle () : m_bAutomaticColor (true), m_bAutomaticBackColor (true), m_rsAlignment (leftAlignment), m_rsColor (RGB(0, 0, 0)), m_rsBackColor (RGB(0, 0, 0))   { }
        void  SetRect (CRect rc)          { m_rc=rc; }
        CRect GetRect ()                  { return m_rc; }
        stRSFont&   GetRSFont()           { return m_rsFont; }
        stRSBorder& GetRSBorder()         { return m_rsBorder; }
        eRSAlignment GetAlignment ()      { return m_rsAlignment; }
        void SetAlignment (eRSAlignment e){ m_rsAlignment=e; }
        void GetForeColor (bool* bIsForeAutomatic, COLORREF* rgbForeColor) { *bIsForeAutomatic=m_bAutomaticColor; *rgbForeColor=m_rsColor; }
        void GetBackColor (bool* bIsBackAutomatic, COLORREF* rgbBackColor) { *bIsBackAutomatic=m_bAutomaticBackColor; *rgbBackColor=m_rsBackColor; }
        void SetForeColor (COLORREF rgbForeColor) { m_bAutomaticColor=false; m_rsColor=rgbForeColor; }
        void SetBackColor (COLORREF rgbBackColor) { m_bAutomaticBackColor=false; m_rsBackColor=rgbBackColor; }
        void Save (CString& strSave);
        void Load (CString  strLoad);
};

/* forward rfeference */
class CAdamoRSCtrlList;
class CAdamoRSEditor;

class CAdamoRSBase   {
    public :
        CAdamoRSBase () : m_nID (-1), m_nTabIndex (0), m_nZOrder (0), m_bTranslated (false), m_pCtrlList (NULL)   { };
        virtual ~CAdamoRSBase ()   { };
        virtual eRSCtrl KindOf () = 0; 
        virtual int  SaveXML (CString& strSave)=0;
        virtual void SetName (CString strName)       { m_strName=strName; }
        CString GetName ()                      { return m_strName; }
        void SetTabIndex (int nTabIndex)        { m_nTabIndex=nTabIndex; }
        int  GetTabIndex ()                     { return m_nTabIndex; }
        void SetID (int nID)                    { m_nID=nID; }
        int  GetID()                            { return m_nID; }
        void SetZOrder (int nZOrder)            { m_nZOrder=nZOrder; }
        int  GetZOrder ()                       { return m_nZOrder; }
        virtual int  LoadCaption ()             { return 0; }
        virtual int  SaveCaption ()             { return 0; }
        int  LoadTranslatedCaption (CString& strValue);
        int  SaveTranslatedCaption (CString strValue);
        virtual int CloseOperations ()          { return 0; }
        void SetCtrlList (CAdamoRSCtrlList *pCtrlList)  { m_pCtrlList=pCtrlList; }
    protected :
        CAdamoRSCtrlList    *m_pCtrlList;
    private :
        CString m_strName;
        int     m_nZOrder;
        int     m_nTabIndex;
        int     m_nID;
    protected :
        bool m_bTranslated;
};

class CAdamoRSForm : public CAdamoRSBase
{
    public :
        CAdamoRSForm () : m_bBackgroundEnable (false), m_bIsMainSynoptic (false) { }
        ~CAdamoRSForm () { };
        eRSCtrl KindOf ()   { return RSForm; }
        int SaveXML (CString& strHTML);
        void  SetName (CString strName);
        void  SetPositionAbs (int nTop, int nLeft, int nBottom, int nRigth)      { m_rc.SetRect (nTop, nLeft, nBottom, nRigth); }
        CRect GetPosition ()                    { return m_rc; }
		void  EnableBackground (bool b)											 { m_bBackgroundEnable = b; if (!b) m_strBackgroundImage = ""; }
		void  SetBackgroundImage (CString strImage)								 { m_strBackgroundImage = strImage; }
		bool  IsBackgroundEnabled ()											 { return m_bBackgroundEnable; }
		CString GetBackgroundImage ()											 { return m_strBackgroundImage; }
		void  SetMainSynoptic (bool b = true)									 { m_bIsMainSynoptic = b; }
		bool  GetMainSynoptic ()												 { return m_bIsMainSynoptic; }
    private :
        CRect   m_rc;
		bool    m_bBackgroundEnable;
		CString m_strBackgroundImage;
		bool    m_bIsMainSynoptic;
};

class CAdamoRSVarControl
{
    public :
        CAdamoRSVarControl ()   { }
        stRSVariabili& GetObjVar ()   { return m_objVar; }
    protected :
        stRSVariabili m_objVar;
};

class CAdamoRSControl : public CAdamoRSBase, public CAdamoRSVarControl
{
    public :
        CAdamoRSControl ()              { }
        CAdamoRSStyle& GetObjStyle ()   { return m_objStyle; }
    protected :
        CAdamoRSStyle       m_objStyle;
};

class CAdamoRSLabel : public CAdamoRSControl
{
    public :
        CAdamoRSLabel () : m_strValue ("Static")  { };
        eRSCtrl KindOf ()   { return RSLabel; }
        int     SaveXML (CString& strSave);
        CString GetValue ()   { return m_strValue; }
        void    SetValue (CString strValue)   { m_strValue=strValue; }
        void    SetValueOnLoad (CString strValue)   { if (!m_bTranslated) m_strValue=strValue; }
        int  LoadCaption ()             { return CAdamoRSBase::LoadTranslatedCaption (m_strValue); }
        int  SaveCaption ()             { return CAdamoRSBase::SaveTranslatedCaption (m_strValue); }
        int  CloseOperations ()         { return LoadCaption (); }
    private :
        CString m_strValue;
};

class CAdamoRSInputText : public CAdamoRSControl
{
    public :
        CAdamoRSInputText () : m_nSize (32), m_bIsPsw (false), m_bIsAutoUpdate (true) { }
        eRSCtrl KindOf ()   { return RSInputText; }
        int     SaveXML (CString& strSave);
        void    SetValue (CString strValue) { m_strValue=strValue; }
        CString GetValue ()                 { return m_strValue;   }
        void    SetSize (int nSize)         { m_nSize=nSize; }
        int     GetSize ()                  { return m_nSize;  }
        void    SetPsw (bool bIsPsw)        { m_bIsPsw=bIsPsw; }
        bool    GetPsw ()                   { return m_bIsPsw; }
        bool    IsAutoUpdate ()             { return m_bIsAutoUpdate; }
        void    SetAutoUpdate (bool bIsAutoUpdate)  { m_bIsAutoUpdate=bIsAutoUpdate; }
		void    SetType (eEditType e)		{ m_eType = e; }
		eEditType GetType ()				{ return m_eType; }
    private :
        CString m_strValue;
        int     m_nSize;
        bool    m_bIsPsw;
		bool    m_bIsAutoUpdate;
		eEditType m_eType;
};

class CAdamoRSInputRadio : public CAdamoRSControl
{
    public :
        CAdamoRSInputRadio () : m_bIsChecked (false), m_bIsAutoUpdate (true)   { }
        eRSCtrl KindOf ()   { return RSInputRadio; }
        int     SaveXML (CString& strSave);
        void    SetValue (CString strValue) { m_strValue=strValue; }
        CString GetValue ()                 { return m_strValue;   }
        bool    IsChecked ()                { return m_bIsChecked; }
        void    SetChecked (bool bChecked)  { m_bIsChecked=bChecked; }
        bool    IsAutoUpdate ()             { return m_bIsAutoUpdate; }
        void    SetAutoUpdate (bool bIsAutoUpdate)  { m_bIsAutoUpdate=bIsAutoUpdate; }
		void	SetGroup (CString strGruppo)	{ m_strGroup = strGruppo; }
		CString GetGroup ()					{ return m_strGroup; }
    private :
        CString m_strValue;
		CString m_strGroup;
        bool    m_bIsChecked;
        bool    m_bIsAutoUpdate;
};

class CAdamoRSInputCheck : public CAdamoRSControl
{
    public :
        CAdamoRSInputCheck () : m_bIsChecked (false), m_bIsAutoUpdate (true)   { }
        eRSCtrl KindOf ()   { return RSInputCheck; }
        int     SaveXML (CString& strSave);
        void    SetValue (CString strValue) { m_strValue=strValue; }
        CString GetValue ()                 { return m_strValue;   }
        bool    IsChecked ()                { return m_bIsChecked; }
        void    SetChecked (bool bChecked)  { m_bIsChecked=bChecked; }
        bool    IsAutoUpdate ()             { return m_bIsAutoUpdate; }
        void    SetAutoUpdate (bool bIsAutoUpdate)  { m_bIsAutoUpdate=bIsAutoUpdate; }
    private :
        CString m_strValue;
        bool    m_bIsChecked;
        bool    m_bIsAutoUpdate;
};

class CAdamoRSGroup : public CAdamoRSControl
{
    public :
        CAdamoRSGroup () : m_strValue ("GroupBox")  { }
        eRSCtrl KindOf ()   { return RSGroup; }
        int     SaveXML (CString& strSave);
        CString GetValue ()   { return m_strValue; }
        void    SetValue (CString strValue)   { m_strValue=strValue; }
        void    SetValueOnLoad (CString strValue) { if (!m_bTranslated) m_strValue=strValue; }
        int  LoadCaption ()             { return CAdamoRSBase::LoadTranslatedCaption (m_strValue); }
        int  SaveCaption ()             { return CAdamoRSBase::SaveTranslatedCaption (m_strValue); }
        int  CloseOperations ()         { return LoadCaption (); }
    private :
        CString m_strValue;
};

class CAdamoRSButton : public CAdamoRSControl
{
    public :
        CAdamoRSButton () : m_strValue ("Button"), m_eType (eStandard), m_dwAction (0), m_bTransparency (false)  { }
        ~CAdamoRSButton () { }
        eRSCtrl KindOf ()   { return RSButton; }
        int  SaveXML (CString& strSave);
        void    SetValue (CString strValue) { m_strValue=strValue; }
        void    SetValueOnLoad (CString strValue) { if (!m_bTranslated) m_strValue=strValue; }
        CString GetValue ()                 { return m_strValue;   }
        int  LoadCaption ()             { return CAdamoRSBase::LoadTranslatedCaption (m_strValue); }
        int  SaveCaption ()             { return CAdamoRSBase::SaveTranslatedCaption (m_strValue); }
        int  CloseOperations ()         { return LoadCaption (); }
		void SetButtonType (eRSButtonType eType)   { m_eType = eType; }
		eRSButtonType GetButtonType ()			   { return m_eType; }
		void SetButtonImage (CString strImage)	   { m_strImage = strImage; }
		CString GetButtonImage ()				   { return m_strImage; }
		void EnableDownloadAction (bool b = true)  { if (b) bitset (m_dwAction, 0); else bitres (m_dwAction, 0); }
		void EnableLoadAction (bool b = true)      { if (b) bitset (m_dwAction, 1); else bitres (m_dwAction, 1); }
		void EnableSaveAction (bool b = true)      { if (b) bitset (m_dwAction, 2); else bitres (m_dwAction, 2); }
		void EnableChangePageAction (bool b = true){ if (b) bitset (m_dwAction, 3); else bitres (m_dwAction, 3); }
		bool IsDownloadActionEnabled ()			   { return bittest (m_dwAction, 0); }
		bool IsLoadActionEnabled ()				   { return bittest (m_dwAction, 1); }
		bool IsSaveActionEnabled ()				   { return bittest (m_dwAction, 2); }
		bool IsChangePageActionEnabled ()		   { return bittest (m_dwAction, 3); }
		void SetNextPage (CString strPage)		   { m_strPage = strPage; }
		CString GetNextPage ()					   { return m_strPage; }
		void SetTransparency (bool b)			   { m_bTransparency = b; }
		bool IsTransparent ()					   { return m_bTransparency; }
    private :
        CString m_strValue;
		eRSButtonType m_eType;
		CString m_strImage;
		DWORD   m_dwAction;
		CString m_strPage;
		bool    m_bTransparency;
};

class CAdamoRSList : public CAdamoRSControl
{
    public :
        CAdamoRSList () : m_nSize(0), m_bMultipleSelection (false), m_nOptions (0)  {
            for (int i=0; i<MAX_RSOPTIONS; i++)  m_stOpzioni[i].m_bSelected=false;
        }
        eRSCtrl KindOf ()   { return RSList; }
        int     SaveXML (CString& strSave);
        void    SetSize (int nSize)   { m_nSize=nSize; }
        void    SetMultipleSelection (bool bMultipleSelection)   { m_bMultipleSelection=bMultipleSelection; }
        int     GetSize ()            { return m_nSize; }
        bool    IsMultipleSelection ()                          { return m_bMultipleSelection; }
        void    GetRSOptions (stRSOpzioni* pOptions, int* nOptions);
        void    AddRSOption (stRSOpzioni* pOpt, int nPos);
        void    DeleteAllRSOption ()                            { m_nOptions=0; }
        int     LoadCaption ();
        int     SaveCaption ();
        int     CloseOperations ()         { return LoadCaption (); }
    private :
        int         m_nSize;
        bool        m_bMultipleSelection;
        stRSOpzioni m_stOpzioni[MAX_RSOPTIONS];
        int         m_nOptions;
};

class CAdamoRSCombo : public CAdamoRSControl
{
    public :
        CAdamoRSCombo () : m_nOptions (0)  {
            for (int i=0; i<MAX_RSOPTIONS; i++)  m_stOpzioni[i].m_bSelected=false;
        }
        eRSCtrl KindOf ()   { return RSCombo; }
        int     SaveXML (CString& strSave);
        void    GetRSOptions (stRSOpzioni* pOptions, int* nOptions);
        void    AddRSOption (stRSOpzioni* pOpt, int nPos);
        void    DeleteAllRSOption ()                            { m_nOptions=0; }
        int     LoadCaption ();
        int     SaveCaption ();
        int     CloseOperations ()         { return LoadCaption (); }
    private :
        stRSOpzioni m_stOpzioni[MAX_RSOPTIONS];
        int         m_nOptions;
};

class CAdamoRSImage : public CAdamoRSControl
{
    public :
        CAdamoRSImage () : m_bStretch (false), m_bTransparent (false), m_clrTransparentColor (0)   { }
        eRSCtrl KindOf ()   { return RSImage; }
        void SetStretch (bool bStretch)   { m_bStretch = bStretch; }
        bool IsStretched ()               { return m_bStretch; }
        void SetIDImage (CString strID)   { m_strID = strID; }
        CString GetIDImage ()             { return m_strID; }
        void SetTransparent (bool b)      { m_bTransparent = b; }
		void SetTransparentColor (COLORREF col)			{ m_clrTransparentColor = col; }
        bool IsTransparent ()             { return m_bTransparent; }
        COLORREF GetTransparentColor ()                 { return m_clrTransparentColor; }
        int  SaveXML (CString& strSave);
    private :
        bool        m_bStretch;
        bool        m_bTransparent;
        COLORREF    m_clrTransparentColor;
        CString     m_strID;
};

class CAdamoRSGrid : public CAdamoRSControl
{
    public :
		CAdamoRSGrid () : m_nRows (0), m_nCols (0)  {
			for (int i = 0; i < MAXCOLUMNS; i++)
				m_gcd[i].m_nType = LUA_TNUMBER;
		}
        eRSCtrl KindOf ()   { return RSGrid; }
		void SetGridData (int nRows, int nColumns);
		void SetColumnData (int nColumn, stGridColumnData *pgcd);
		void GetGridData (int *pnRow, int *pnColumns);
		void GetColumnData (int nColumn, stGridColumnData *pgcd);
        int  SaveXML (CString& strSave);
	private :
        int m_nRows;
        int m_nCols;
		stGridColumnData m_gcd[MAXCOLUMNS];
};

class CAdamoRSAx : public CAdamoRSControl
{
    public :
		CAdamoRSAx () : m_nID (-1)   { }
        eRSCtrl KindOf ()   { return RSAx; }
        int  SaveXML (CString& strSave);
		void SetData (CString strPath, int nID)   { m_strPath = strPath; m_nID = nID; }
		CString GetData ()						  { return m_strPath; }
		int     GetID ()						  { return m_nID; }
	private :
		CString m_strPath;
		int     m_nID;
};

enum eLedType   {
	eRectangular,
	eDiamond,
	eCircular
};

enum eLedStyle   {
	eNoStyle,
	eLedStyleRaised,
	eLedStyleLowered
};

class CAdamoRSILed : public CAdamoRSControl
{
    public :
		CAdamoRSILed () : m_nID (-1), m_eDev (enBase), m_nData (0), m_eLedType (eRectangular), m_bTransparent (false), m_bShowReflection (false),
		m_activecolor (0), m_eBevelStyle (eNoStyle), m_nSpacingHorizontal (0), m_nSpacingVertical (0), m_nRowCount (0), m_nColCount (0), 
		m_eBorderStyle (eNoStyle), m_backcolor (0) 		{
		}
        eRSCtrl KindOf ()   { return RSILed; }
        int  SaveXML (CString& strSave);
		void SetDevice (CString strPath, enDispositivi eType, int nID, int nData)   {
			m_strPath = strPath;
			m_eDev = eType;
			m_nID = nID;
			m_nData = nData;
		}
		CString GetPath ()							{ return m_strPath; }
		int     GetDeviceID ()						{ return m_nID; }
		enDispositivi GetDeviceType ()				{ return m_eDev; }
		int     GetDeviceData ()					{ return m_nData; }
		void SetLedType (eLedType e)				{ m_eLedType = e; }
		void SetTransparent (bool bTransparent)		{ m_bTransparent = bTransparent; }
		void SetShowReflection (bool bReflection)	{ m_bShowReflection = bReflection; }
		void SetActiveColor (COLORREF col)			{ m_activecolor = col; }
		void SetBevelStyle (eLedStyle e)			{ m_eBevelStyle = e; }
		void SetHorizontalSpacing (int nHS)			{ m_nSpacingHorizontal = nHS; }
		void SetVerticalSpacing (int nHS)			{ m_nSpacingVertical = nHS; }
		void SetRowCount (int nRow)					{ m_nRowCount = nRow; }
		void SetColCount (int nCol)					{ m_nColCount = nCol; }
		void SetBorderStyle (eLedStyle e)			{ m_eBorderStyle = e; }
		void SetBackgroundColor (COLORREF col)		{ m_backcolor = col; }
		eLedType GetLedType ()							{ return m_eLedType; }
		bool GetTransparent ()						{ return m_bTransparent; }
		bool GetShowReflection ()					{ return m_bShowReflection; }
		COLORREF GetActiveColor ()					{ return m_activecolor; }
		eLedStyle GetBevelStyle ()					{ return m_eBevelStyle; }
		int GetHorizontalSpacing ()					{ return m_nSpacingHorizontal; }
		int GetVerticalSpacing ()					{ return m_nSpacingVertical; }
		int GetRowCount ()							{ return m_nRowCount; }
		int GetColCount ()							{ return m_nColCount; }
		eLedStyle GetBorderStyle ()					{ return m_eBorderStyle; }
		COLORREF GetBackgroundColor ()				{ return m_backcolor; }
	private :
		CString m_strPath;
		enDispositivi	m_eDev;
		int     m_nID;
		int     m_nData;
		eLedType		m_eLedType;
		bool			m_bTransparent;
		bool			m_bShowReflection;
		COLORREF		m_activecolor;
		eLedStyle		m_eBevelStyle;
		int				m_nSpacingHorizontal;
		int				m_nSpacingVertical;
		int				m_nRowCount;
		int				m_nColCount;
		eLedStyle		m_eBorderStyle;
		COLORREF		m_backcolor;
};

enum eBorderStyle   {
	eNoBorder,
	eBorderSunken,
	eBorderRaised,
	eBorderLowered
};

enum eStyle   {
	eStyleSunken,
	eStyleRaised,
	eStyleFlat
};

enum ePointerStyle   {
	eArrowLine,
	eArrow,
	eLine,
	eTriangle
};

class CAdamoRSIAngularGauge : public CAdamoRSControl
{
    public :
		CAdamoRSIAngularGauge () : m_eDev (enBase), m_nID (-1), m_nData (0), m_bTransparent (false), m_nPosMin (0), m_nPosMax (100),
 			m_bShowInnerRadius (false), m_bShowOuterRadius (false), m_bReverseScale (false), m_eBorderStyle (eNoBorder),
			m_rgbBackGround (RGB (160, 160, 160)), m_ePointerStyle (eTriangle), m_nPointerSize (10), m_nPointerMargin (0), m_rgbPointer (0),
			m_nArcRadius (60), m_nArcStartAngle (180), m_nAngle (90), m_bHubShow (true), m_bHub3D (false), m_nHubSize (14),	m_rgbHub (0),
			m_bFaceShow (false), m_eFaceStyle (eStyleFlat), m_eBevelStyle (eStyleSunken), m_nBevelSize (5), m_rgbFace (0),
			m_nTickMargin (5), m_nTickLabelMargin (15), m_bTickMajorShow (true), m_nTickMajorNumber (5), m_nTickMajorLenght (7),
			m_rgbTickMajor (0), m_bTickMinorShow (true), m_nTickMinorNumber (4), m_nTickMinorLenght (3), m_rgbTickMinor (0),
			m_nSectionsNumber (0), m_rgbSec1 (0), m_rgbSec2 (0), m_rgbSec3 (0), m_rgbSec4 (0), m_rgbSec5 (0),
			m_nEndSec1 (50), m_nEndSec2 (75), m_nEndSec3 (0), m_nEndSec4 (0)		{ }
        eRSCtrl KindOf ()   { return RSIAngularGauge; }
        int  SaveXML (CString& strSave);
		void SetDevice (CString strPath, enDispositivi eType, int nID, int nData)   {
			m_strPath = strPath;
			m_eDev = eType;
			m_nID = nID;
			m_nData = nData;
		}
		void SetDeviceData (int nData)				{ m_nData = nData; }
		CString GetPath ()							{ return m_strPath; }
		int     GetDeviceID ()						{ return m_nID; }
		enDispositivi GetDeviceType ()				{ return m_eDev; }
		int     GetDeviceData ()					{ return m_nData; }
	private :
		/* Generali */
		CString m_strPath;
		enDispositivi m_eDev;
		int     m_nID;
		int     m_nData;
		bool    m_bTransparent;
		int     m_nPosMin;
		int		m_nPosMax;
		bool    m_bShowInnerRadius;
		bool    m_bShowOuterRadius;
		bool    m_bReverseScale;
		eBorderStyle m_eBorderStyle;
		COLORREF m_rgbBackGround;
		/* Pointers */
		ePointerStyle m_ePointerStyle;
		int		m_nPointerSize;
		int     m_nPointerMargin;
		COLORREF m_rgbPointer;
		/* arc */
		int		m_nArcRadius;
		int     m_nArcStartAngle;
		int     m_nAngle;
		/* hub */
		bool    m_bHubShow;
		bool    m_bHub3D;
		int		m_nHubSize;
		COLORREF m_rgbHub;
		/* face */
		bool    m_bFaceShow;
		eStyle  m_eFaceStyle;
		eStyle  m_eBevelStyle;
		int     m_nBevelSize;
		COLORREF m_rgbFace;
		/* tick */
		int		m_nTickMargin;
		/* tick label */
		int		m_nTickLabelMargin;
		stRSFont m_fntTickLabelFont;
		/* tick major */
		bool    m_bTickMajorShow;
		int     m_nTickMajorNumber;
		int     m_nTickMajorLenght;
		COLORREF m_rgbTickMajor;
		/* tick minor */
		bool    m_bTickMinorShow;
		int     m_nTickMinorNumber;
		int     m_nTickMinorLenght;
		COLORREF m_rgbTickMinor;
		/* sections */
		int     m_nSectionsNumber;
		COLORREF m_rgbSec1;
		COLORREF m_rgbSec2;
		COLORREF m_rgbSec3;
		COLORREF m_rgbSec4;
		COLORREF m_rgbSec5;
		int      m_nEndSec1;
		int      m_nEndSec2;
		int      m_nEndSec3;
		int      m_nEndSec4;
	public :
		/* generali */
		void SetTransparent (bool b)				{ m_bTransparent = b; }
		void SetPosMin (int n)						{ m_nPosMin = n; }
		void SetPosMax (int n)						{ m_nPosMax = n; }
		void ShowInnerRadius (bool b)				{ m_bShowInnerRadius = b; }
		void ShowOuterRadius (bool b)				{ m_bShowOuterRadius = b; }
		void SetReverseScale (bool b)				{ m_bReverseScale = b; }
		void SetBorderStyle (eBorderStyle e)		{ m_eBorderStyle = e; }
		void SetBackGroundColor (COLORREF rgb)		{ m_rgbBackGround = rgb; }
		/* Pointers */
		void SetPointerStyle (ePointerStyle e)		{ m_ePointerStyle = e; }
		void SetPointerSize (int n)					{ m_nPointerSize = n; }
		void SetPointerMargin (int n)				{ m_nPointerMargin = n; }
		void SetPointerColor (COLORREF rgb)		{ m_rgbPointer = rgb; }
		/* arc */
		void SetArcRadius (int n)					{ m_nArcRadius = n; }
		void SetArcStartAngle (int n)				{ m_nArcStartAngle = n; }
		void SetArcAngle (int n)					{ m_nAngle = n; }
		/* hub */
		void ShowHub (bool b)						{ m_bHubShow = b; }
		void ShowHub3D (bool b)						{ m_bHub3D = b; }
		void SetHubSize (int n)						{ m_nHubSize = n; }
		void SetHubColor (COLORREF rgb)				{ m_rgbHub = rgb; }
		/* face */
		void ShowFace (bool b)						{ m_bFaceShow = b; }
		void SetFaceStyle (eStyle e)				{ m_eFaceStyle = e; }
		void SetBevelStyle (eStyle e)				{ m_eBevelStyle = e; }
		void SetBevelSize (int n)					{ m_nBevelSize = n; }
		void SetFaceColor (COLORREF rgb)			{ m_rgbFace = rgb; }
		/* tick */
		void SetTickMargin (int n)					{ m_nTickMargin = n; }
		/* tick label */
		void SetTickLabelMargin (int n)				{ m_nTickLabelMargin = n; }
		void SetTickLabelFont (stRSFont &fnt)		{ m_fntTickLabelFont = fnt; }
		/* tick major */
		void ShowTickMajor (bool b)					{ m_bTickMajorShow = b; }
		void SetTickMajorNumber (int n)				{ m_nTickMajorNumber = n; }
		void SetTickMajorLenght (int n)				{ m_nTickMajorLenght = n; }
		void SetTickMajorColor (COLORREF rgb)		{ m_rgbTickMajor = rgb; }
		/* tick minor */
		void ShowTickMinor (bool b)					{ m_bTickMinorShow = b; }
		void SetTickMinorNumber (int n)				{ m_nTickMinorNumber = n; }
		void SetTickMinorLenght (int n)				{ m_nTickMinorLenght = n; }
		void SetTickMinorColor (COLORREF rgb)		{ m_rgbTickMinor = rgb; }
		/* sections */
		void SetSectionsNumber (int n)				{ m_nSectionsNumber = n; }
		void SetSection1Color (COLORREF rgb)		{ m_rgbSec1 = rgb; }
		void SetSection2Color (COLORREF rgb)		{ m_rgbSec2 = rgb; }
		void SetSection3Color (COLORREF rgb)		{ m_rgbSec3 = rgb; }
		void SetSection4Color (COLORREF rgb)		{ m_rgbSec4 = rgb; }
		void SetSection5Color (COLORREF rgb)		{ m_rgbSec5 = rgb; }
		void SetEndSection1 (int n)					{ m_nEndSec1 = n; }
		void SetEndSection2 (int n)					{ m_nEndSec2 = n; }
		void SetEndSection3 (int n)					{ m_nEndSec3 = n; }
		void SetEndSection4 (int n)					{ m_nEndSec4 = n; }
		/* generale */
		bool IsTransparent ()						{ return m_bTransparent; }
		int  GetPosMin ()							{ return m_nPosMin; }
		int  GetPosMax ()							{ return m_nPosMax; }
		bool IsInnerRadius ()						{ return m_bShowInnerRadius; }
		bool IsOuterRadius ()						{ return m_bShowOuterRadius; }
		bool IsReverseScale ()						{ return m_bReverseScale; }
		eBorderStyle GetBorderStyle ()				{ return m_eBorderStyle; }
		COLORREF GetBackGroundColor ()				{ return m_rgbBackGround; }
		/* Pointers */
		ePointerStyle GetPointerStyle ()			{ return m_ePointerStyle; }
		int GetPointerSize ()						{ return m_nPointerSize; }
		int GetPointerMargin ()						{ return m_nPointerMargin; }
		COLORREF GetPointerColor ()					{ return m_rgbPointer; }
		/* arc */
		int GetArcRadius ()							{ return m_nArcRadius; }
		int GetArcStartAngle ()						{ return m_nArcStartAngle; }
		int GetArcAngle ()							{ return m_nAngle; }
		/* hub */
		bool IsHub ()								{ return m_bHubShow; }
		bool IsHub3D ()								{ return m_bHub3D; }
		int  GetHubSize ()							{ return m_nHubSize; }
		COLORREF GetHubColor ()						{ return m_rgbHub; }
		/* face */
		bool IsFace ()								{ return m_bFaceShow; }
		eStyle GetFaceStyle ()						{ return m_eFaceStyle; }
		eStyle GetBevelStyle ()						{ return m_eBevelStyle; }
		int  GetBevelSize ()						{ return m_nBevelSize; }
		COLORREF GetFaceColor ()					{ return m_rgbFace; }
		/* tick */
		int GetTickMargin ()						{ return m_nTickMargin; }
		/* tick label */
		int GetTickLabelMargin ()					{ return m_nTickLabelMargin; }
		stRSFont& GetTickLabelFont (stRSFont &fnt)	{ return m_fntTickLabelFont; }
		/* tick major */
		bool IsTickMajor ()							{ return m_bTickMajorShow; }
		int GetTickMajorNumber ()					{ return m_nTickMajorNumber; }
		int GetTickMajorLenght ()					{ return m_nTickMajorLenght; }
		COLORREF GetTickMajorColor ()				{ return m_rgbTickMajor; }
		/* tick minor */
		bool IsTickMinor ()							{ return m_bTickMinorShow; }
		int GetTickMinorNumber ()					{ return m_nTickMinorNumber; }
		int GetTickMinorLenght ()					{ return m_nTickMinorLenght; }
		COLORREF GetTickMinorColor ()				{ return m_rgbTickMinor; }
		/* sections */
		int GetSectionsNumber ()					{ return m_nSectionsNumber; }
		COLORREF GetSection1Color ()				{ return m_rgbSec1; }
		COLORREF GetSection2Color ()				{ return m_rgbSec2; }
		COLORREF GetSection3Color ()				{ return m_rgbSec3; }
		COLORREF GetSection4Color ()				{ return m_rgbSec4; }
		COLORREF GetSection5Color ()				{ return m_rgbSec5; }
		int GetEndSection1 ()						{ return m_nEndSec1; }
		int GetEndSection2 ()						{ return m_nEndSec2; }
		int GetEndSection3 ()						{ return m_nEndSec3; }
		int GetEndSection4 ()						{ return m_nEndSec4; }
};

enum eOrientation   {
	eHorizontal,
	eVertical
};

enum eOrientationTicks  {
	eBottomLeft,
	eTopRight
};

enum eLinearGaugeStyle   {
	eNoLinearGaugeStyle,
	eLinearGaugeStyleRaised,
	eLinearGaugeStyleLowered
};

enum eLinearGaugePointerStyle   {
	eLinearGaugePointer,
	eLinearGaugeTriangle,
	eLinearGaugeTLine,
	eLinearGaugeColorBar,
	eLinearGaugeTube,
	eLinearGaugeBarSunken,
	eLinearGaugeBarRaised
};

class CAdamoRSILinearGauge : public CAdamoRSControl
{
    public :
		CAdamoRSILinearGauge () : m_eDev (enBase), m_nID (-1), m_nData (0), m_bTransparent (false), m_nPosMin (0), m_nPosMax (100),
			m_bReverseScale (false), m_eOrientation (eHorizontal), m_eOrientationTicks (eBottomLeft), m_eBorderStyle (eNoLinearGaugeStyle),
			m_rgbBackGround (RGB (160, 160, 160)), m_ePointerStyle (eLinearGaugePointer), m_nPointerSize (10), m_nPointerOffset (0), m_rgbPointer (0),
			m_bPointer3D (false), m_bDrawScaleSide (false), m_bShowTickAxes (true), m_nTickMargin (0), m_bShowLabels (true),
			m_nLabelMargin (5), m_bTickMajorShow (true), m_nTickMajorNumber (5), m_nTickMajorLenght (20),
			m_rgbTickMajor (0), m_bTickMinorShow (true), m_nTickMinorNumber (4), m_nTickMinorLenght (7), m_rgbTickMinor (0),
			m_nSectionsNumber (0), m_rgbSec1 (0), m_rgbSec2 (0), m_rgbSec3 (0), m_rgbSec4 (0), m_rgbSec5 (0),
			m_nEndSec1 (50), m_nEndSec2 (75), m_nEndSec3 (0), m_nEndSec4 (0)		{ }
        eRSCtrl KindOf ()   { return RSILinearGauge; }
		int  SaveXML (CString& strSave);
		void SetDevice (CString strPath, enDispositivi eType, int nID, int nData)   {
			m_strPath = strPath;
			m_eDev = eType;
			m_nID = nID;
			m_nData = nData;
		}
		void SetDeviceData (int nData)				{ m_nData = nData; }
		CString GetPath ()							{ return m_strPath; }
		int  GetDeviceID ()							{ return m_nID; }
		enDispositivi GetDeviceType ()				{ return m_eDev; }
		int     GetDeviceData ()					{ return m_nData; }
	private :
		CString m_strPath;
		enDispositivi m_eDev;
		int     m_nID;
		int     m_nData;
		/* general */
		bool	m_bTransparent;
		int     m_nPosMin;
		int		m_nPosMax;
		bool    m_bReverseScale;
		eOrientation m_eOrientation;
		eOrientationTicks m_eOrientationTicks;
		eLinearGaugeStyle m_eBorderStyle;
		COLORREF m_rgbBackGround;
		/* pointers */
		eLinearGaugePointerStyle m_ePointerStyle;
		int		m_nPointerSize;
		int     m_nPointerOffset;
		COLORREF m_rgbPointer;
		bool    m_bPointer3D;
		bool    m_bDrawScaleSide;
		/* ticks */
		bool	m_bShowTickAxes;
		int 	m_nTickMargin;
		bool	m_bShowLabels;
		int 	m_nLabelMargin;
		stRSFont m_fntLabelFont;
		/* tick major */
		bool    m_bTickMajorShow;
		int     m_nTickMajorNumber;
		int     m_nTickMajorLenght;
		COLORREF m_rgbTickMajor;
		/* tick minor */
		bool    m_bTickMinorShow;
		int     m_nTickMinorNumber;
		int     m_nTickMinorLenght;
		COLORREF m_rgbTickMinor;
		/* sections */
		int     m_nSectionsNumber;
		COLORREF m_rgbSec1;
		COLORREF m_rgbSec2;
		COLORREF m_rgbSec3;
		COLORREF m_rgbSec4;
		COLORREF m_rgbSec5;
		int      m_nEndSec1;
		int      m_nEndSec2;
		int      m_nEndSec3;
		int      m_nEndSec4;
	public :
		/* generali */
		void SetTransparent (bool b)				{ m_bTransparent = b; }
		void SetPosMin (int n)						{ m_nPosMin = n; }
		void SetPosMax (int n)						{ m_nPosMax = n; }
		void SetReverseScale (bool b)				{ m_bReverseScale = b; }
		void SetOrientation (eOrientation e)		{ m_eOrientation = e; }
		void SetOrientationTicks (eOrientationTicks e)	{ m_eOrientationTicks = e; }
		void SetBorderStyle (eLinearGaugeStyle e)	{ m_eBorderStyle = e; }
		void SetBackGroundColor (COLORREF rgb)		{ m_rgbBackGround = rgb; }
		/* pointers */
		void SetPointerStyle (eLinearGaugePointerStyle ePointerStyle)	{ m_ePointerStyle = ePointerStyle; }
		void SetPointerSize (int nPointerSize)		{ m_nPointerSize = nPointerSize; }
		void SetPointerOffset (int nPointerOffset)	{ m_nPointerOffset = nPointerOffset; }
		void SetPointerColor (COLORREF rgbPointer)	{ m_rgbPointer = rgbPointer; }
		void SetPointer3D (bool bPointer3D)			{ m_bPointer3D = bPointer3D; }
		void SetDrawScaleSide (bool bDrawScaleSide)	{ m_bDrawScaleSide = bDrawScaleSide; }
		/* ticks */
		void ShowTickAxes (bool bShowTickAxes)		{ m_bShowTickAxes = bShowTickAxes; }
		void SetTickMargin (int nTickMargin)		{ m_nTickMargin = nTickMargin; }
		void ShowLabels (bool bShowLabels)			{ m_bShowLabels = bShowLabels; }
		void SetLabelMargin (int nLabelMargin)		{ m_nLabelMargin = nLabelMargin; }
		void SetTickLabelFont (stRSFont &fnt)		{ m_fntLabelFont = fnt; }
		/* tick major */
		void ShowTickMajor (bool b)					{ m_bTickMajorShow = b; }
		void SetTickMajorNumber (int n)				{ m_nTickMajorNumber = n; }
		void SetTickMajorLenght (int n)				{ m_nTickMajorLenght = n; }
		void SetTickMajorColor (COLORREF rgb)		{ m_rgbTickMajor = rgb; }
		/* tick minor */
		void ShowTickMinor (bool b)					{ m_bTickMinorShow = b; }
		void SetTickMinorNumber (int n)				{ m_nTickMinorNumber = n; }
		void SetTickMinorLenght (int n)				{ m_nTickMinorLenght = n; }
		void SetTickMinorColor (COLORREF rgb)		{ m_rgbTickMinor = rgb; }
		/* sections */
		void SetSectionsNumber (int n)				{ m_nSectionsNumber = n; }
		void SetSection1Color (COLORREF rgb)		{ m_rgbSec1 = rgb; }
		void SetSection2Color (COLORREF rgb)		{ m_rgbSec2 = rgb; }
		void SetSection3Color (COLORREF rgb)		{ m_rgbSec3 = rgb; }
		void SetSection4Color (COLORREF rgb)		{ m_rgbSec4 = rgb; }
		void SetSection5Color (COLORREF rgb)		{ m_rgbSec5 = rgb; }
		void SetEndSection1 (int n)					{ m_nEndSec1 = n; }
		void SetEndSection2 (int n)					{ m_nEndSec2 = n; }
		void SetEndSection3 (int n)					{ m_nEndSec3 = n; }
		void SetEndSection4 (int n)					{ m_nEndSec4 = n; }
		/* general */
		bool IsTransparent ()						{ return m_bTransparent; }
		int  GetPosMin ()							{ return m_nPosMin; }
		int  GetPosMax ()							{ return m_nPosMax; }
		bool IsReverseScale ()						{ return m_bReverseScale; }
		eOrientation GetOrientation ()				{ return m_eOrientation; }
		eOrientationTicks GetOrientationTicks ()	{ return m_eOrientationTicks; }
		eLinearGaugeStyle GetBorderStyle ()			{ return m_eBorderStyle; }
		COLORREF GetBackGroundColor ()				{ return m_rgbBackGround; }
		/* pointers */
		eLinearGaugePointerStyle GetPointerStyle ()	{ return m_ePointerStyle; }
		int  GetPointerSize ()						{ return m_nPointerSize; }
		int  GetPointerOffset ()					{ return m_nPointerOffset; }
		COLORREF GetPointerColor ()					{ return m_rgbPointer; }
		bool GetPointer3D ()						{ return m_bPointer3D; }
		bool GetDrawScaleSide ()					{ return m_bDrawScaleSide; }
		/* ticks */
		bool IsTickAxes ()							{ return m_bShowTickAxes; }
		int  GetTickMargin ()						{ return m_nTickMargin; }
		bool IsShowLabels ()						{ return m_bShowLabels; }
		int  GetLabelMargin ()						{ return m_nLabelMargin; }
		stRSFont& GetTickLabelFont ()				{ return m_fntLabelFont; }
		/* tick major */
		bool IsTickMajor ()							{ return m_bTickMajorShow; }
		int GetTickMajorNumber ()					{ return m_nTickMajorNumber; }
		int GetTickMajorLenght ()					{ return m_nTickMajorLenght; }
		COLORREF GetTickMajorColor ()				{ return m_rgbTickMajor; }
		/* tick minor */
		bool IsTickMinor ()							{ return m_bTickMinorShow; }
		int GetTickMinorNumber ()					{ return m_nTickMinorNumber; }
		int GetTickMinorLenght ()					{ return m_nTickMinorLenght; }
		COLORREF GetTickMinorColor ()				{ return m_rgbTickMinor; }
		/* sections */
		int GetSectionsNumber ()					{ return m_nSectionsNumber; }
		COLORREF GetSection1Color ()				{ return m_rgbSec1; }
		COLORREF GetSection2Color ()				{ return m_rgbSec2; }
		COLORREF GetSection3Color ()				{ return m_rgbSec3; }
		COLORREF GetSection4Color ()				{ return m_rgbSec4; }
		COLORREF GetSection5Color ()				{ return m_rgbSec5; }
		int GetEndSection1 ()						{ return m_nEndSec1; }
		int GetEndSection2 ()						{ return m_nEndSec2; }
		int GetEndSection3 ()						{ return m_nEndSec3; }
		int GetEndSection4 ()						{ return m_nEndSec4; }
};

enum eDisplayStyle   {
	eNoDisplayStyle,
	eDisplayStyleRaised,
	eDisplayStyleLowered
};

enum eDisplayLeadingStyle   {
	eNoDisplayLeading,
	eDisplayZeroes,
	eDisplaySpaces
};


class CAdamoRSIDisplay : public CAdamoRSControl
{
    public :
		CAdamoRSIDisplay () : m_eDev (enBase), m_nID (-1), m_nData (0), m_bTransparent (false), m_bShowOffSegments (true),
			m_bShowSign (true), m_bAutoSegmentOffColor (true), m_nPrecision (2), m_eBorderStyle (eNoDisplayStyle), 
			m_eLeadingStyle (eDisplaySpaces), m_rgbBackGround (0), m_nCount (4), m_nSpacing (6), m_nSize (1), m_nSeparation (1),
			m_nMargin (1), m_rgbColor (RGB (255, 255, 255)), m_rgbOffColor (RGB (128, 128, 128))	{
		}
        eRSCtrl KindOf ()   { return RSIDisplay; }
		int  SaveXML (CString& strSave);
		void SetDevice (CString strPath, enDispositivi eType, int nID, int nData)   {
			m_strPath = strPath;
			m_eDev = eType;
			m_nID = nID;
			m_nData = nData;
		}
		void SetDeviceData (int nData)				{ m_nData = nData; }
		CString GetPath ()							{ return m_strPath; }
		int  GetDeviceID ()							{ return m_nID; }
		enDispositivi GetDeviceType ()				{ return m_eDev; }
		int     GetDeviceData ()					{ return m_nData; }
	private :
		CString m_strPath;
		enDispositivi m_eDev;
		int     m_nID;
		int     m_nData;
		/* general */
		bool    m_bShowOffSegments;
		bool	m_bTransparent;
		bool	m_bShowSign;
		bool	m_bAutoSegmentOffColor;
		int		m_nPrecision;
		eDisplayStyle m_eBorderStyle;
		eDisplayLeadingStyle m_eLeadingStyle;
		COLORREF m_rgbBackGround;
		/* display */
		int		m_nCount;
		int		m_nSpacing;
		int		m_nSize;
		int		m_nSeparation;
		int		m_nMargin;
		COLORREF m_rgbColor;
		COLORREF m_rgbOffColor;
	public :
		/* general */
		void ShowOffSegment (bool bShowOffSegments)		{ m_bShowOffSegments = bShowOffSegments; }
		void SetTransparent (bool bTransparent)			{ m_bTransparent = bTransparent; }
		void ShowSign (bool bShowSign)					{ m_bShowSign = bShowSign; }
		void SetAutoSegmentOffColor (bool bAutoSegmentOffColor) { m_bAutoSegmentOffColor = bAutoSegmentOffColor; }
		void SetPrecision (int nPrecision)				{ m_nPrecision = nPrecision; }
		void SetDisplayStyle (eDisplayStyle eBorderStyle) { m_eBorderStyle = eBorderStyle; }
		void SetLeadingStyle (eDisplayLeadingStyle eLeadingStyle) { m_eLeadingStyle = eLeadingStyle; }
		void SetBackGroundColor (COLORREF rgb)			{ m_rgbBackGround = rgb; }
		/* display */
		void SetDisplayCount (int nCount)				{ m_nCount = nCount; }
		void SetDisplaySpacing (int nSpacing)			{ m_nSpacing = nSpacing; }
		void SetDisplaySize (int nSize)					{ m_nSize = nSize; }
		void SetDisplaySeparation (int nSeparation)		{ m_nSeparation = nSeparation; }
		void SetDisplayMargin (int nMargin)				{ m_nMargin = nMargin; }
		void SetDisplayColor (COLORREF rgbColor)		{ m_rgbColor = rgbColor; }
		void SetDisplayOffColor (COLORREF rgbOffColor)	{ m_rgbOffColor = rgbOffColor; }
		/* general */
		bool IsOffSegment ()							{ return m_bShowOffSegments; }
		bool IsTransparent ()							{ return m_bTransparent; }
		bool IsSign ()									{ return m_bShowSign; }
		bool IsAutoSegmentOffColor ()					{ return m_bAutoSegmentOffColor; }
		int  GetPrecision ()							{ return m_nPrecision; }
		eDisplayStyle GetDisplayStyle ()				{ return m_eBorderStyle; }
		eDisplayLeadingStyle GetLeadingStyle ()			{ return m_eLeadingStyle; }
		COLORREF GetBackGroundColor ()					{ return m_rgbBackGround; }
		/* display */
		int  GetDisplayCount ()							{ return m_nCount; }
		int  GetDisplaySpacing ()						{ return m_nSpacing; }
		int  GetDisplaySize ()							{ return m_nSize; }
		int  GetDisplaySeparation ()					{ return m_nSeparation; }
		int  GetDisplayMargin ()						{ return m_nMargin; }
		COLORREF GetDisplayColor ()						{ return m_rgbColor; }
		COLORREF GetDisplayOffColor ()					{ return m_rgbOffColor; }
}; 

enum eLabelStyle   {
	eLabelNone,
	eLabelSingle,
	eLabelLowered,
	eLabelRaised
};

enum eLabelAlignment    {
	eLabelCentered,
	eLabelLeft,
	eLabelRight
};

class CAdamoRSILabel : public CAdamoRSControl
{
    public :
		CAdamoRSILabel () : m_eDev (enBase), m_nID (-1), m_nData (0), m_bTransparent (false), m_bAutoSize (false),m_eBorderStyle (eLabelNone),
		m_eAlignment (eLabelCentered), m_bShadow (false), m_nOffsetX (0), m_nOffsetY (0), m_rgbShadowColor (0)	{
		}
        eRSCtrl KindOf ()   { return RSILabel; }
		int  SaveXML (CString& strSave);
		void SetDevice (CString strPath, enDispositivi eType, int nID, int nData)   {
			m_strPath = strPath;
			m_eDev = eType;
			m_nID = nID;
			m_nData = nData;
		}
		void SetDeviceData (int nData)				{ m_nData = nData; }
		CString GetPath ()							{ return m_strPath; }
		int  GetDeviceID ()							{ return m_nID; }
		enDispositivi GetDeviceType ()				{ return m_eDev; }
		int     GetDeviceData ()					{ return m_nData; }
		void    SetValue (CString strValue)			{ m_strValue = strValue; }
		CString GetValue ()							{ return m_strValue; }
	private :
		CString m_strPath;
		enDispositivi m_eDev;
		int     m_nID;
		int     m_nData;
		/* general */
        CString m_strValue;
		bool	m_bTransparent;
		bool	m_bAutoSize;
		eLabelStyle m_eBorderStyle;
		eLabelAlignment m_eAlignment;
		stRSFont m_fntFont;
		bool	m_bShadow;
		int		m_nOffsetX;
		int		m_nOffsetY;
		COLORREF m_rgbColor;
		COLORREF m_rgbBackColor;
		COLORREF m_rgbShadowColor;
	public :
		void SetTransparent (bool bTransparent)				{ m_bTransparent = bTransparent; }
		void SetAutoSize (bool bAutoSize)					{ m_bAutoSize = bAutoSize; }
		void SetStyle (eLabelStyle eBorderStyle)			{ m_eBorderStyle = eBorderStyle; }
		void SetAlignment (eLabelAlignment eAlignment)		{ m_eAlignment = eAlignment; }
		void SetFont (stRSFont &fnt)						{ m_fntFont = fnt; }
		void ShowShadow (bool bShadow)						{ m_bShadow = bShadow; }
		void SetOffsetX (int nOffsetX)						{ m_nOffsetX = nOffsetX; }
		void SetOffsetY (int nOffsetY)						{ m_nOffsetY = nOffsetY; }
		void SetColor (COLORREF rgbColor)					{ m_rgbColor = rgbColor; }
		void SetBackColor (COLORREF rgbBackColor)			{ m_rgbBackColor = rgbBackColor; }
		void SetShadowColor (COLORREF rgbShadowColor)		{ m_rgbShadowColor = rgbShadowColor; }
		bool IsTransparent ()								{ return m_bTransparent; }
		bool IsAutoSize ()									{ return m_bAutoSize; }
		eLabelStyle GetStyle ()								{ return m_eBorderStyle; }
		eLabelAlignment GetAlignment ()						{ return m_eAlignment; }
		stRSFont& GetFont ()								{ return m_fntFont; }
		bool IsShadow ()									{ return m_bShadow; }
		int GetOffsetX ()									{ return m_nOffsetX; }
		int GetOffsetY ()									{ return m_nOffsetY; }
		COLORREF GetColor ()								{ return m_rgbColor; }
		COLORREF GetBackColor ()							{ return m_rgbBackColor; }
		COLORREF GetShadowColor ()							{ return m_rgbShadowColor; }
};

class CAdamoRSIPicture : public CAdamoRSControl
{
    public :
        CAdamoRSIPicture () : m_eDev (enBase), m_nID (-1), m_nData (0), m_bStretch (false), m_bTransparent (false), m_clrTransparentColor (0)   {
		}
        eRSCtrl KindOf ()   { return RSIPicture; }
		int  SaveXML (CString& strSave);
		void SetDevice (CString strPath, enDispositivi eType, int nID, int nData)   {
			m_strPath = strPath;
			m_eDev = eType;
			m_nID = nID;
			m_nData = nData;
		}
		void SetDeviceData (int nData)				{ m_nData = nData; }
		CString GetPath ()							{ return m_strPath; }
		int  GetDeviceID ()							{ return m_nID; }
		enDispositivi GetDeviceType ()				{ return m_eDev; }
		int  GetDeviceData ()					    { return m_nData; }
        void SetStretch (bool bStretch)   { m_bStretch = bStretch; }
        void SetTransparent (bool b)      { m_bTransparent = b; }
		void SetTransparentColor (COLORREF col)			{ m_clrTransparentColor = col; }
        bool IsTransparent ()             { return m_bTransparent; }
        bool IsStretched ()               { return m_bStretch; }
        COLORREF GetTransparentColor ()   { return m_clrTransparentColor; }
        void SetImageID (int nIndex, CString strID)      { if (nIndex >=0 && nIndex < NMAX_IPICTURE_IMAGES) m_strID[nIndex] = strID; }
        CString GetImageID (int nIndex)   { CString str; if (nIndex >=0 && nIndex < NMAX_IPICTURE_IMAGES)  str = m_strID[nIndex]; return str; }
        CString GetFirstIDImage ()        {
            CString str; 
            
            for (int i = 0; i<NMAX_IPICTURE_IMAGES; i++) 
                if (m_strID[i] != "")  { 
                    str = m_strID[i]; 
                    break; 
                } 
            return str; 
        }
        void ClearFirstImage ()   {
            for (int i = 0; i<NMAX_IPICTURE_IMAGES; i++) 
                if (m_strID[i] != "")  { 
                    m_strID[i] = ""; 
                    break; 
                } 
        }
    private :
		CString m_strPath;
		enDispositivi m_eDev;
		int     m_nID;
		int     m_nData;
        bool    m_bStretch;
        bool    m_bTransparent;
        COLORREF m_clrTransparentColor;
        CString m_strID[NMAX_IPICTURE_IMAGES];
};

class CAdamoRSCtrlList : public CPtrList
{
    public :
        CAdamoRSCtrlList ();
        ~CAdamoRSCtrlList ();
        void AddCtrl (CAdamoRSBase* pCtrl);
        void RemoveCtrl (CAdamoRSBase* pCtrl);
        void RemoveAllCtrl ();
        int  Load (CString  strXML);
        int  Save (CString& strXML);
        int  GetValidID ();
        CAdamoRSControl* GetRSCtrl (int nID);
        void SetTitle (CString strTitle)   { m_strTitle=strTitle; }
        CAdamoRSForm* GetFormCtrl ();
        bool IsModified ()                 { return m_bIsModified; }
        void Ready ()                      { m_bIsModified=false; }
        void Modify ()                     { m_bIsModified=true; }
        void Clear ();
		void SetMainSyn (bool bIsMainSyn);	
        void SetTraduzioniObj (CTraduzioni* pTr)    { m_tr=pTr; }
        int  LoadCtrlCaption (int nId, CString strTitle, CString& strCaption);
        int  SaveCtrlCaption (int nId, CString strTitle, CString strCaption);
        virtual eResources GetResourceType () = 0;
        virtual CString    GetResourceName () = 0;
        virtual CSize      GetFormSize ()     = 0;
    private :
        void SavePrologue (CString& strXML);
        void SaveControls (CString& strXML);
        void SaveEpilogue (CString& strXML);
        int  LoadXML (CString strXML);
        int  LoadXMLVariable (ElementXmlPtr p, CAdamoRSVarControl *pCtrl);
        int  LoadXMLOptions (ElementXmlPtr p, CAdamoRSControl *pCtrl, bool bIsCombo);
		int  LoadXMLColumns (ElementXmlPtr p, CAdamoRSControl *pCtrl);
        int  ParseXMLString ();
        int  CreateFormXML (ElementXmlPtr p);
        int  CreateXMLControls (ElementXmlPtr p);
        int  CreateXMLInput (ElementXmlPtr p);
        int  CreateXMLComboList (ElementXmlPtr p);
        int  CreateXMLGroupBox (ElementXmlPtr p);
        int  CreateXMLButton (ElementXmlPtr p);
        int  CreateXMLLabel (ElementXmlPtr p);
        int  CreateXMLInputText (ElementXmlPtr p);
        int  CreateXMLInputRadio (ElementXmlPtr p);
        int  CreateXMLInputCheckBox (ElementXmlPtr p);
        int  CreateXMLInputImage (ElementXmlPtr p);
        int  CreateXMLCombo (ElementXmlPtr p);
        int  CreateXMLList (ElementXmlPtr p);
        int  CreateXMLGrid (ElementXmlPtr p);
        int  CreateXMLAx   (ElementXmlPtr p);
        int  CreateXMLLed  (ElementXmlPtr p);
		int  CreateXMLAngularGauge (ElementXmlPtr p);
		int  CreateXMLLinearGauge (ElementXmlPtr p);
		int  CreateXMLDisplay (ElementXmlPtr p);
		int  CreateXMLStatic (ElementXmlPtr p);
        int  CreateXMLPicture (ElementXmlPtr p);
        CString GetResourceName (CString strId);
    private :
        CString m_strTitle;
        int  m_nValidID;
        bool m_bIsModified;
        DocXmlPtr m_pDocXml;
        CTraduzioni *m_tr;
};

#endif
