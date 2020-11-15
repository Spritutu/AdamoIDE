#if !defined(AFX_LUAEDITOR_H__C002AF8D_DFEF_4577_A494_10F2B3DF2458__INCLUDED_)
#define AFX_LUAEDITOR_H__C002AF8D_DFEF_4577_A494_10F2B3DF2458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LuaEditor.h : header file
//

#include "scintilla.h"
#include "PropSet.h"

#define SCE_UNIVERSAL_FOUND_STYLE_SMART 29
#define MAXLINEHIGHLIGHT 400
#define MARK_BOOKMARK 24
#define MARK_HIDELINESBEGIN 23
#define MARK_HIDELINESEND 22

enum folderStyle {FOLDER_TYPE, FOLDER_STYLE_SIMPLE, FOLDER_STYLE_ARROW, FOLDER_STYLE_CIRCLE, FOLDER_STYLE_BOX, FOLDER_STYLE_NONE};

class CAdamoHelpOptions;

/////////////////////////////////////////////////////////////////////////////
// CLuaEditor window

enum eCommentMode
{
	eToggle,
	eComment,
	eUncomment
};

#define ELEMENTS(a)	(sizeof(a) / sizeof(a[0]))

class CProjectFile;

class CLuaEditor : public CWnd
{
// Construction
public:
	CLuaEditor();
    DECLARE_DYNCREATE(CLuaEditor)
    afx_msg LRESULT OnDestroyHelpOptionsWnd (WPARAM wParam, LPARAM lParam);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLuaEditor)
	//}}AFX_VIRTUAL

// Implementation
public:
	void BookmarkClearAll();
	void BookmarkNext(bool forwardScan);
	bool BookmarkPresent(int lineno);
	void BookmarkDelete(int lineno);
	void BookmarkAdd(int lineno);
	void BookmarkToggle(int lineno = -1);
	long GetCurrentColumn();
	int ReplaceAll(LPCSTR szFind,LPCSTR szReplace,BOOL bUseSelection);
	void ReplaceSearchedText(LPCSTR szText);
	BOOL SearchBackward(LPSTR szText);
	BOOL SearchForward(LPSTR szText);
	CString GetSelectedText();
	CString GetCaretText();
	CString GetCaretSymbol(CString& strSymbol, int& nSymbolType);
	CString GetCaretWordBefore();
    virtual void SetSearchflags (int nSearchflags){m_nSearchflags = nSearchflags;};
	void DeleteCallTip();
	int startCalltipWord;
	int currentCallTip;
	int maxCallTips;
	SString currentCallTipWord;
	void FillFunctionDefinition(int pos = -1);
	void CharAdded(char nChar);
	bool StartAutoComplete();
	void SetLuaLexer();
	BOOL PreparePrint(CDC* pDC, CPrintInfo* pInfo);
	void PrintPage(CDC* pDC, CPrintInfo* pInfo);
	void EndPrint(CDC* pDC, CPrintInfo* pInfo);
	void SetStackTraceLevel(int nLevel);
	void SetCallStackMargins();
	int GetCurrentLine();
	void GotoLastLine();
	CString GetLine(int nLine);
	void ClearAll();
	void AddText(const char* szText);
	void SetReadOnly(BOOL bReadOnly);
	void SetEditorMargins();
	void GrabFocus();
	void Copy();
	void Paste();
	BOOL CanPaste();
	void Clear();
	void Cut();
	BOOL CanCutOrClear();
	void SelectAll();
	void Redo();
	BOOL CanRedo();
	void Undo();
	BOOL CanUndo();
	void SetBreakPointsIn(CProjectFile* pPF);
	void GotoLine(int nLine, bool bShowMarker);
	void SetBreakpoint(int nLine);
	void ClearAllBreakpoints();
	BOOL Save(CFile* pFile);
	BOOL Load(CFile* pFile);
	BOOL ToggleBreakpoint(int nLine);
	int  LineFromPoint(CPoint& pt);
	BOOL Create(CWnd* pParentWnd, UINT nCtrlID, DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_TABSTOP);
	long GetSelectionEnd();
	long GetSelectionStart();
	void GotoPosition(long lPos);
	LONG GetCurrentPosition();
    int  GetLineCount ();
    void SelectLine (int nLine);
    void SelectCurrentWord ();
    void SelectCurrentSymbol ();
	bool bracematch();
	void smarthilight();
	bool blockcomment(eCommentMode currCommentMode);
	void gotobracematch (int nMode);
	bool isFolded(int line);
	void fold(int line, bool mode);
	int  GetLineFromPosition (int nPosition);
	void foldAll(bool mode);
    virtual ~CLuaEditor();
	void RemoveHelpOptions ();
	// thread safe functions
	//void tsAddText(const char *szMsg) {	AddText(szMsg); };
	//void tsSetReadOnly(BOOL bReadOnly) { SetReadOnly(bReadOnly); };

	void Print(bool showDialog);
	CharacterRange GetSelection();
    void ClearAllLines ();
    void CleanSelection ();
	void marginClick(int position, int modifiers);
	void EnsureVisible(int line);
	CString GetCaretFunction ();
	CString GetCaretMatrix (CString strSymbol);
	void SelectWord (CString strWord, int nLine, int nItem);
	void SelectSymbol (CString strSymbol, int nLine, int nItem);
	void OnEditShowEnums ();
	void OnEditShowParams ();
	void SetSkin ();
	void DestroyHelpOptionsWindow ();
protected:
    int  m_nSearchflags;
	bool indentOpening;
	bool indentClosing;
	bool callTipIgnoreCase;
	int lastPosCallTip;
	int braceCount;
	bool autoCompleteIgnoreCase;
	WordList apis;
	WordList apis_function;
	SString wordCharacters,autoCompleteStartCharacters;
	SString functionDefinition,calltipWordCharacters;
	SString calltipEndDefinition;
	SString autoCompleteFillUpCharacters;
	bool autoCCausedByOnlyOne,indentMaintain;
	int (*m_fnScintilla)(void*,int,int,int);
	void *m_ptrScintilla;
	CUIntArray	m_pages;
	BOOL m_bShowCalltips;
	CString m_strCallTip;
	CAdamoHelpOptions* m_pAHO;
	CStringList m_slOptions;
	CStringList m_slDescriptions;
	CStringList m_slParameters;
	int Sci(int nCmd, int wParam=0, int lParam=0);
	// Generated message map functions
protected:
	void GotoLineEnsureVisible(int line);
	void EliminateLibrary(char *words);
	void StartAutoCompFunc();
	bool RangeIsAllWhitespace(int start, int end);
	void SetSelection(int anchor, int currentPos);
	int GetLineIndentPosition(int line);
	void SetLineIndentation(int line, int indent);
	int GetLineIndentation(int line);
	int GetLineLength(int line);
	void MaintainIndentation(char ch);
	static int FileLength(const char *path);
	void ReadAPI(const SString &fileNameForExtension,WordList &api);
	int LengthDocument();
	bool StartAutoCompleteWord(bool onlyOneWord);
	void EliminateDuplicateWords(char *words);
	void GetRange(int start, int end, char *text);
	void ContinueCallTip();
	bool StartCallTip();
	void GetLine(char *text, int sizeText, int line=-1);
	int GetCaretInLine();
	int GetCurrentLineNumber();
	int SciString(int nCmd, int wParam, const char *lParam);
	//{{AFX_MSG(CLuaEditor)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private :
	void updateLineNumberWidth ();
	void findMatchingBracePos(int &braceAtCaret, int &braceOpposite);
	void clearIndicator(int indicatorNumber);
	bool isWordChar(char ch) const;
	bool isQualifiedWord(const char *str) const;
	int  MarkAll (CString strToFind, int startRange, int endRange);
	void SetFoldersStyles ();
	void defineMarker (int marker, int markerType, COLORREF fore, COLORREF back, COLORREF foreActive);
	void expand (int &line, bool doExpand, bool force, int visLevels, int level = -1); 
	void runMarkers(bool doHide, int searchStart, bool endOfDoc, bool doDelete);
	void ShowLibraries ();
	void ShowFunctions (CString strLibrary);
	void ShowEnums ();
	void ShowEnumsValues (CString strEnum);
	void ShowParameter ();
	void ShowEnumInCallTip ();
	void GetListPosition (CRect& rc, bool bOneColumn);
	void FillOptions ();
	void FillFunctions (CString strLibrary);
	void FillListCtrl (CRect& rc, int nType, int nMode);
	void FillEnums ();
	void FillEnumsValues (CString strEnum);
	void AutoSelectHelpOptions ();
	bool CompleteHelpOptionWindow ();
	bool MoveHelpOptionWindowUp ();
	bool MoveHelpOptionWindowDown ();
	bool GetCallTipText (const char* words, CString &strCallTipText);
	void Reverse (char* str);
	int  FindValidItem (CString strLine, CString str, int n);
	void SetBckSciStyle (COLORREF rgb);
virtual BOOL PreTranslateMessage(MSG* pMsg);
private :
	HWND m_hwndParent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LUAEDITOR_H__C002AF8D_DFEF_4577_A494_10F2B3DF2458__INCLUDED_)
