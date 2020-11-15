// paredit.cpp: C++ derived edit control for numbers/letters etc
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

#include "stdafx.h"
#include "paredit.h"
#include "adamolib.h"

/* debug memory directive */
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// ParsedEdit

CParsedEdit::CParsedEdit()
    : m_pszFPformat("%f")
{
    m_wParseStyle = 0;
    m_dDelta = 1;
    m_fpFun=NULL;
    m_Mod=-2;
}

BEGIN_MESSAGE_MAP(CParsedEdit, CEdit)
    //{{AFX_MSG_MAP(CParsedEdit)
    ON_WM_KEYDOWN()    
    ON_WM_CHAR()
    ON_WM_VSCROLL()     // for associated spin controls
    ON_WM_HSCROLL()     // for associated spin controls
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Creating from C++ code

BOOL CParsedEdit::Create(DWORD dwStyle, const RECT& rect,
        CWnd* pParentWnd, UINT nID)
{
    m_wParseStyle = LOWORD(dwStyle);
    // figure out edit control style
    DWORD dwEditStyle = MAKELONG(ES_LEFT, HIWORD(dwStyle));
    return CWnd::Create("EDIT", NULL, dwEditStyle, rect, pParentWnd, nID);
}

/////////////////////////////////////////////////////////////////////////////
// Aliasing on top of an existing Edit control

BOOL CParsedEdit::SubclassEdit(UINT nID, CWnd* pParent, WORD wParseStyle)
{
    m_wParseStyle = wParseStyle;

    // SubclassWindow requires an HWND so we call the Windows GetDlgItem
    // and avoid creating a tempory CWnd and then calling GetSafeHwnd
    HWND hWndEdit = ::GetDlgItem(pParent->m_hWnd, nID);
    if (hWndEdit == NULL)
        return FALSE;
    return SubclassWindow(hWndEdit);
}

/////////////////////////////////////////////////////////////////////////////
// Aliasing on top of an existing Edit control

BOOL CParsedEdit::SubclassEdit(HWND hParent, WORD wParseStyle)
{
    m_wParseStyle = wParseStyle;

    if (hParent == NULL)
        return FALSE;

    return SubclassWindow(hParent);
}

/////////////////////////////////////////////////////////////////////////////
// Input character filter
void CParsedEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    WORD type;

    if (nChar < 0x20)                                 
        type = PES_ALL;                         // always allow control chars
    else if (nChar >= '0' && nChar <= '9')
        type = PES_NUMBERS;
    else if (nChar == '.' || nChar == '+' || nChar == '-')
        type = PES_FLOATS;
    else if (nChar >= 'A' && nChar <= 'F')
        type = PES_HEXNUMBERS;
    else if (nChar >= 'a' && nChar <= 'f')
        type = PES_HEXNUMBERS;
    else if (nChar >= 'A' && nChar <= 'Z')      // hard coded to english
        type = PES_LETTERS;
    else if (nChar >= 'a' && nChar <= 'z')
        type = PES_LETTERS;
    else if (nChar == '_')
        type = PES_UNDERLINE;
    else if (nChar == ' ')
        type = PES_BLANK;
    else
        type = PES_OTHERCHARS;

    if (m_wParseStyle & type)
    {                                                    
    	if (nChar==VK_RETURN && GetParent())
       	//if (nChar==VK_RETURN && m_fpFun && GetParent())
            GetParent()->SendMessage(WM_CHAR, (WPARAM)nChar, MAKELONG(nRepCnt, nFlags));
        else
            CEdit::OnChar(nChar, nRepCnt, nFlags);  // permitted
    }
    else
    {
        // illegal character - inform parent
        OnBadInput();
    }
}

void CParsedEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{                         
    UINT line;      
    if (nChar!=VK_UP && nChar!=VK_DOWN) {
        CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }       
    if (nChar==VK_UP)
        line = SB_LINEUP;
    if (nChar==VK_DOWN)         
        line = SB_LINEDOWN; 
    OnVScroll(line, 0, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Spin controls will send scroll messages
void CParsedEdit::OnVScroll(UINT nSBCode, UINT, CScrollBar*)
{
    double nDelta = 0;
    if (nSBCode == SB_LINEDOWN)
        nDelta = -1*m_dDelta;
    else if (nSBCode == SB_LINEUP)
        nDelta = +1*m_dDelta;
    else
        return; // nothing special

    // set the focus to this edit item and select it all
    CWnd *pWnd=SetFocus();

    //Get the number in the control.
    LONG nOld;
    double fOld;
    char text[21];
    GetParent()->GetDlgItemText(GetDlgCtrlID(), text, 20);
    if (m_wParseStyle & PES_FLOATS)
        fOld = atof(text);    
    else {
        if (m_wParseStyle & PES_HEXNUMBERS){
            char buff[5];
            _snprintf(buff, 5, "%04s", text);
            nOld = htou(buff);
        }
        else    
            nOld = atol(text);
    }
    // The MuScroll control also supports range checking
    // for this example, we just prevent overflow
    BOOL bOk=FALSE;
    if (m_wParseStyle & PES_FLOATS){
        double fNew = fOld + nDelta;
        if (fNew >= m_dMin && fNew <= m_dMax){
            sprintf(text, m_pszFPformat, fNew);
            bOk = TRUE;
        }
    }
    else {
        LONG nNew = nOld + (LONG)nDelta;
        if (nNew >= m_dMin && nNew <= m_dMax) {
            if (m_wParseStyle & PES_HEXNUMBERS)
                sprintf(text, "%X", nNew);
            else
                sprintf(text, "%ld", nNew);
            bOk = TRUE;
        }
    }

    if (bOk) {
        GetParent()->SetDlgItemText(GetDlgCtrlID(), text);
        if (m_fpFun && GetParent())
            GetParent()->SendMessage(WM_CHAR, (WPARAM)VK_RETURN, MAKELONG(1, 0));
    }
    else 
        OnBadInput();
//    SetSel(0, -1);
}

void CParsedEdit::OnHScroll(UINT nSBCode, UINT, CScrollBar*)
{
    double nDelta = 0;
    if (nSBCode == SB_LINELEFT)
        nDelta = -1*m_dDelta;
    else if (nSBCode == SB_LINERIGHT)
        nDelta = +1*m_dDelta;
    else
        return; // nothing special

    // set the focus to this edit item and select it all
    CWnd *pWnd=SetFocus();

    //Get the number in the control.
    LONG nOld;
    double fOld;
    char text[21];
    GetParent()->GetDlgItemText(GetDlgCtrlID(), text, 20);
    if (m_wParseStyle & PES_FLOATS)
        fOld = atof(text);    
    else {
        if (m_wParseStyle & PES_HEXNUMBERS) {
            char buff[5];
            _snprintf(buff, 5, "%04s", text);
            //nOld = htou(buff);
        }
        else    
            nOld = atol(text);
    }
    // The MuScroll control also supports range checking
    // for this example, we just prevent overflow
    BOOL bOk=FALSE;
    if (m_wParseStyle & PES_FLOATS){
        double fNew = fOld + nDelta;
        if (fNew >= m_dMin && fNew <= m_dMax) {
            sprintf(text, m_pszFPformat, fNew);
            bOk=TRUE;
        }
    }
    else {
        LONG nNew = nOld + (LONG)nDelta;
        if (nNew >= m_dMin && nNew <= m_dMax) {
            if (m_wParseStyle & PES_HEXNUMBERS)
                sprintf(text, "%X", nNew);
            else
                sprintf(text, "%ld", nNew);
            bOk=TRUE;
        }
    }

    if (bOk) {
        GetParent()->SetDlgItemText(GetDlgCtrlID(), text);
        if (m_fpFun && GetParent())
            GetParent()->SendMessage(WM_CHAR, (WPARAM)VK_RETURN, MAKELONG(1, 0));
    }
    else 
        OnBadInput();
//    SetSel(0, -1);
}

/////////////////////////////////////////////////////////////////////////////
// default bad input handler, beep (unless parent notification
//    returns -1.  Most parent dialogs will return 0 or 1 for command
//    handlers (i.e. Beep is the default)

void CParsedEdit::OnBadInput()
{
    if (GetParent()->SendMessage(WM_COMMAND, 
        MAKELONG (GetDlgCtrlID(), PEN_ILLEGALCHAR),
        (LPARAM)m_hWnd) != -1)
    {
        MessageBeep(-1);
    }
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// The C++ class CParsedEdit can be made visible to the dialog manager
//   by registering a window class for it
// The C++ class 'CParsedEditExported' is used to implement the
//   creation and destruction of a C++ object as if it were just
//   a normal Windows control.
// In order to hook in the class creation we must provide a special
//   WndProc to create the C++ object and override the PostNcDestroy
//   message to destroy it

class CParsedEditExported : public CParsedEdit      // WNDCLASS exported class
{
public:
    CParsedEditExported(HWND hWnd)
        { VERIFY(Attach(hWnd)); }

// Implementation: (all is implementation since the public interface of
//    this class is identical to CParsedEdit)
protected:
    virtual WNDPROC* GetSuperWndProcAddr();
    static WNDPROC lpfnSuperWndProc;
    virtual void PostNcDestroy();
    static LRESULT CALLBACK WndProcHook(HWND, UINT, WPARAM, LPARAM);


    friend class CParsedEdit;       // for RegisterControlClass

    //{{AFX_MSG(CParsedEditExported)
    afx_msg int OnNcCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP();
};

/////////////////////////////////////////////////////////////////////////////
// Special create hooks

LRESULT CALLBACK
CParsedEditExported::WndProcHook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // create new item and attach it
    CParsedEditExported* pEdit = new CParsedEditExported(hWnd);

    // set up wndproc to AFX one, and call it
    ::SetWindowLong(hWnd, GWL_WNDPROC, (DWORD)AfxWndProc);
#ifdef STRICT
    return ::CallWindowProc(AfxWndProc, hWnd, msg, wParam, lParam);
#else
    return ::CallWindowProc((FARPROC)AfxWndProc, hWnd, msg, wParam, lParam);
#endif
}


BEGIN_MESSAGE_MAP(CParsedEditExported, CParsedEdit)
    //{{AFX_MSG_MAP(CParsedEditExported)
    ON_WM_NCCREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CParsedEditExported::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
    // special create hook
    // example of stripping the sub-style bits from the style specified
    //   in the dialog template to use for some other reason
    m_wParseStyle = LOWORD(lpCreateStruct->style);
    DWORD dwEditStyle = MAKELONG(ES_LEFT, HIWORD(lpCreateStruct->style));

    ::SetWindowLong(m_hWnd, GWL_STYLE, dwEditStyle);
    lpCreateStruct->style = dwEditStyle;
    return CParsedEdit::OnNcCreate(lpCreateStruct);
}

void CParsedEditExported::PostNcDestroy()
{
    // needed to clean up the C++ CWnd object
    delete this;
}

WNDPROC CParsedEditExported::lpfnSuperWndProc = NULL;
WNDPROC* CParsedEditExported::GetSuperWndProcAddr()
{
    return &lpfnSuperWndProc;
}

/////////////////////////////////////////////////////////////////////////////
// Routine to register the class
BOOL CParsedEdit::RegisterControlClass()
{
    WNDCLASS wcls;

    // Always set the super class address
    // since the second instance will not need to register the WndClass,
    //  but will still need to set the super-proc address
    if (!::GetClassInfo(NULL, "edit", &wcls))
    {
        return FALSE;
    }

    // set appropriate super class address
    CParsedEditExported::lpfnSuperWndProc = wcls.lpfnWndProc; // "EDIT" wnd proc

    // check to see if class already registered
    static const char szClass[] = "paredit";
    if (::GetClassInfo(AfxGetInstanceHandle(), szClass, &wcls))
    {
        // name already registered - ok if it was us
        return (wcls.lpfnWndProc == (WNDPROC)CParsedEditExported::WndProcHook);
    }

    // set new values
    wcls.lpfnWndProc = CParsedEditExported::WndProcHook;
    wcls.hInstance = AfxGetInstanceHandle();
    wcls.lpszClassName = szClass;
    return (RegisterClass(&wcls) != 0);
}

void CParsedEdit::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if (m_fpFun && m_Mod!=-2) 
        (m_fpFun)(m_Mod, m_Type, m_Add);
    CEdit::OnMouseMove(nFlags, point);
}

void CParsedEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_fpFun && GetParent())
        GetParent()->SendMessage(WM_LBUTTONDOWN, nFlags, MAKELONG(point.x, point.y));
    CEdit::OnLButtonDown(nFlags, point);
}

void CParsedEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (m_fpFun && GetParent())
        GetParent()->SendMessage(WM_LBUTTONDBLCLK, nFlags, MAKELONG(point.x, point.y));
    CEdit::OnLButtonDblClk(nFlags, point);
}

/////////////////////////////////////////////////////////////////////////////



