// paredit.h: C++ derived edit control for numbers/letters etc
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and Microsoft
// WinHelp documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


/////////////////////////////////////////////////////////////////////////////
// CParsedEdit is a specialized CEdit control that only allows characters
//  of a given type.
// This class is used in 3 different ways in the samples

#ifndef _PARSEDIT
#define _PARSEDIT

class CParsedEdit : public CEdit
{
protected:
    WORD     m_wParseStyle;      // C++ member data
    double   m_dMax;
    double   m_dMin;
    double   m_dDelta;
    int      m_Mod;
    int      m_Type;
    int      m_Add;
    void (*m_fpFun)(int, int, int);
    const char* m_pszFPformat;
    
public:
// Construction
    CParsedEdit();
    void SetRange(double min, double max){m_dMin = min; m_dMax = max;};
    void SetDelta(double d){m_dDelta = d;};
    void SetMouseMoveFilter(void (*fp)(int , int , int), int mod, int type, int add){m_fpFun=fp; m_Mod=mod; m_Type=type; m_Add=add;};
    void SetFloatFormat(const char* pszFormat) { m_pszFPformat=pszFormat; };

    // explicit construction (see DERTEST.CPP)
    BOOL Create(DWORD dwStyle /* includes PES_ style*/, const RECT& rect,
        CWnd* pParentWnd, UINT nID);

    // subclassed construction (see SUBTEST.CPP)
    BOOL SubclassEdit(UINT nID, CWnd* pParent, WORD wParseStyle);
    BOOL SubclassEdit(HWND hParent, WORD wParseStyle);

    // for WNDCLASS Registered window
    static BOOL RegisterControlClass();

// Overridables
    virtual void OnBadInput();

// Implementation
protected:
    //{{AFX_MSG(CParsedEdit)
    afx_msg void OnChar(UINT, UINT, UINT); // for character validation 
    afx_msg void OnKeyDown(UINT, UINT, UINT); // for character validation
    afx_msg void OnVScroll(UINT, UINT, CScrollBar*); // for spin buttons
    afx_msg void OnHScroll(UINT, UINT, CScrollBar*); // for spin buttons
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////
// Parsed edit control sub-styles

#define PES_NUMBERS         0x0001
#define PES_FLOATS          0x0002
#define PES_HEXNUMBERS      0x0004   
#define PES_LETTERS         0x0008   //2
#define PES_OTHERCHARS      0x0010   //4
#define PES_UNDERLINE       0X0020
#define PES_BLANK           0X0040
#define PES_ALL             0xFFFF

/////////////////////////////////////////////////////////////////////////////
// Extra control notifications

// above the range for normal EN_ messages
#define PEN_ILLEGALCHAR     0x8000
            // sent to parent when illegal character hit
            // return 0 if you want parsed edit to beep

/////////////////////////////////////////////////////////////////////////////

#endif

