// LuaEditor.cpp : implementation file
//

#include "stdafx.h"
#include <stdio.h>

#define Font FontX
#include "Platform.h"
#include "ide2.h"
#include "LuaEditor.h"
#include "ProjectFile.h"
#include "scintilla.h"
#include "SString.h"
#include "SciLexer.h"
#include "MainFrame.h"
#include "AdamoHelpOptions.h"
#include "CSyntaxContainer.h"
#undef Font

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern const char* achSkinStyles[OPTIONS_SKIN_STYLES];

/////////////////////////////////////////////////////////////////////////////
// CLuaEditor

IMPLEMENT_DYNCREATE(CLuaEditor, CWnd)

CLuaEditor::CLuaEditor():m_fnScintilla(NULL),m_ptrScintilla(NULL), m_pAHO (NULL)
{
	m_bShowCalltips = TRUE;
	currentCallTipWord = "";
	lastPosCallTip = 0;
	braceCount = 0;
	startCalltipWord = 0;
	currentCallTip = 0;
	maxCallTips = 1;
	functionDefinition = 0;
	currentCallTipWord = "";
}

CLuaEditor::~CLuaEditor()
{
}

BEGIN_MESSAGE_MAP(CLuaEditor, CWnd)
	//{{AFX_MSG_MAP(CLuaEditor)
	ON_WM_MOUSEMOVE()
    ON_MESSAGE (WM_DESTROY_HELP_OPTIONS_WND, OnDestroyHelpOptionsWnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static bool iswordcharforsel(char ch)
{
	return !strchr("\t\n\r !\"#$%&'()*+,-./:;<=>?@[\\]^`{|}~", ch);
}

static bool IsCallTipSeparator(char ch)
{
	return (ch == ',') || (ch == ';');
}

/////////////////////////////////////////////////////////////////////////////
// CLuaEditor message handlers

BOOL CLuaEditor::Create(CWnd *pParentWnd, UINT nCtrlId, DWORD dwStyle)
{
	BOOL bCreated = CreateEx(0, "Scintilla","", dwStyle,CRect(0,0,0,0),pParentWnd,nCtrlId);

	if ( !bCreated )
		return FALSE;

	m_fnScintilla = (int (__cdecl *)(void *,int,int,int))
		SendMessage(SCI_GETDIRECTFUNCTION,0,0);
	m_ptrScintilla = (void *)SendMessage(SCI_GETDIRECTPOINTER,0,0);

    //defaul font
    const char font[] = "Consolas";
    Sci(SCI_STYLESETFONT,STYLE_DEFAULT, (LPARAM) font);
    Sci(SCI_STYLESETSIZE,STYLE_DEFAULT, 10);
	m_hwndParent = pParentWnd->GetSafeHwnd ();
	return TRUE;
}

int CLuaEditor::Sci(int nCmd, int wParam, int lParam)
{
    if(m_fnScintilla && m_ptrScintilla)
	    return m_fnScintilla(m_ptrScintilla, nCmd, wParam, lParam);

    return NULL;
}


int CLuaEditor::LineFromPoint(CPoint &pt)
{
	return Sci(SCI_LINEFROMPOSITION, Sci(SCI_POSITIONFROMPOINT, pt.x, pt.y), 0);
}

BOOL CLuaEditor::ToggleBreakpoint(int nLine)
{
	if ( Sci(SCI_MARKERGET, nLine-1) & 1 ) 	{
		Sci(SCI_MARKERDELETE, nLine-1, 0);
		return FALSE;
	}
	else
		Sci(SCI_MARKERADD, nLine-1, 0);
	return TRUE;
}

BOOL CLuaEditor::Load(CFile* pFile)
{
	const int blockSize = 131072;

	Sci(SCI_CLEARALL);

	char data[blockSize];
	size_t lenFile = pFile->Read(data, sizeof(data));
	while (lenFile > 0) 
	{
		Sci(SCI_ADDTEXT, lenFile, (long)data);
		lenFile = pFile->Read(data, sizeof(data));
	}

	Sci(SCI_SETEOLMODE, SC_EOL_CRLF);
	Sci(SCI_SETUNDOCOLLECTION, 1);
	Sci(SCI_SETSAVEPOINT);
	Sci(SCI_GOTOPOS, 0);
    Sci(SCI_EMPTYUNDOBUFFER);
	return TRUE;
}

BOOL CLuaEditor::Save(CFile* pFile)
{
	const int blockSize = 131072;

	TextRange tr;
	char data[blockSize + 1];
	int lengthDoc = Sci(SCI_GETLENGTH);
	for (int i = 0; i < lengthDoc; i += blockSize) 
	{
		int grabSize = lengthDoc - i;
		if (grabSize > blockSize)
			grabSize = blockSize;

		tr.chrg.cpMin = i;
		tr.chrg.cpMax = i + grabSize;
		tr.lpstrText = data;
		Sci(SCI_GETTEXTRANGE, 0, long(&tr));
	
		pFile->Write(data, grabSize);
	}

	Sci(SCI_SETSAVEPOINT);

	return TRUE;	  
}

void CLuaEditor::ClearAllBreakpoints()
{
	Sci(SCI_MARKERDELETEALL, 0);
}

void CLuaEditor::SetBreakpoint(int nLine)
{
	Sci(SCI_MARKERADD, nLine-1, 0);
}

void CLuaEditor::GotoLine(int nLine, bool bShowMarker)
{
	Sci(SCI_GOTOLINE, nLine-1);
	if (bShowMarker)   {
	Sci(SCI_MARKERDELETEALL, 1);
	Sci(SCI_MARKERADD, nLine-1, 1);
}
}

void CLuaEditor::SetBreakPointsIn(CProjectFile *pPF)
{
	pPF->RemoveAllBreakPoints();

	int nLine = Sci(SCI_MARKERNEXT, 0, 1);
	while ( nLine>=0 )
	{
		pPF->AddBreakPoint(nLine+1); // from 0-based to 1-based 
		nLine = Sci(SCI_MARKERNEXT, nLine+1, 1);
	}	
}

BOOL CLuaEditor::CanUndo()
{
	return Sci(SCI_CANUNDO);
}

void CLuaEditor::Undo()
{
	Sci(SCI_UNDO);
}

BOOL CLuaEditor::CanRedo()
{
	return Sci(SCI_CANREDO);
}

void CLuaEditor::Redo()
{
	Sci(SCI_REDO);
}

void CLuaEditor::SelectAll()
{
	Sci(SCI_SELECTALL);
}

BOOL CLuaEditor::CanCutOrClear()
{
	int currentPos = Sci(SCI_GETCURRENTPOS);
	int anchor = Sci(SCI_GETANCHOR);

	return currentPos != anchor;
}

void CLuaEditor::Cut()
{
	Sci(SCI_CUT);
}

void CLuaEditor::Clear()
{
	Sci(SCI_CLEAR);
}

BOOL CLuaEditor::CanPaste()
{
	return Sci(SCI_CANPASTE);
}

void CLuaEditor::Paste()
{
	Sci(SCI_PASTE);
}

void CLuaEditor::Copy()
{
	Sci(SCI_COPY);
}

void CLuaEditor::GrabFocus()
{
	Sci(SCI_GRABFOCUS);
}

void CLuaEditor::SetEditorMargins()
{
	Sci (SCI_SETMARGINTYPEN, 0, SC_MARGIN_NUMBER);
	int pixelWidth = 5 * Sci(SCI_TEXTWIDTH, STYLE_LINENUMBER, (int)"9");
	Sci (SCI_SETMARGINWIDTHN, 0, pixelWidth);

	Sci (SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);
	Sci (SCI_SETMARGINWIDTHN, 1, 10);
	Sci (SCI_SETMARGINSENSITIVEN, 1, true);

    Sci (SCI_SETMARGINMASKN, 2, SC_MASK_FOLDERS);
	Sci (SCI_SETMARGINWIDTHN, 2, 14);
    Sci (SCI_SETMARGINSENSITIVEN, 2, true);

	Sci (SCI_MARKERDEFINE, 0, SC_MARK_CIRCLE);
	Sci (SCI_MARKERSETFORE, 0, RGB(0xff, 0x00, 0x00));
	Sci (SCI_MARKERSETBACK, 0, RGB(0xff, 0x00, 0x00));

	Sci (SCI_MARKERDEFINE, 1, SC_MARK_ARROW);
	Sci (SCI_STYLESETFORE, 33, RGB (160, 160, 160));

	Sci (SCI_SETCARETLINEVISIBLE, true, 0);
	Sci (SCI_SETCARETLINEBACK, 0xFFE8E8, 0);

	/*
	Sci (SCI_STYLESETFORE, STYLE_CALLTIP, RGB (0, 0, 0));
	Sci (SCI_STYLESETBACK, STYLE_CALLTIP, RGB (255, 255, 255));
    Sci (SCI_STYLESETFONT,STYLE_CALLTIP, (LPARAM) "Tahoma");
	Sci (SCI_STYLESETBOLD, STYLE_CALLTIP, 1);
	Sci (SCI_STYLESETITALIC, STYLE_CALLTIP, 0);
	Sci (SCI_STYLESETUNDERLINE, STYLE_CALLTIP, 0);
	Sci (SCI_STYLESETSIZE, STYLE_CALLTIP, 10);
	Sci(SCI_CALLTIPUSESTYLE);
	*/

	Sci (SCI_STYLESETFORE, STYLE_BRACELIGHT, RGB (255, 0, 0));
	Sci (SCI_STYLESETBACK, STYLE_BRACELIGHT, RGB (255, 255, 255));
    //Sci (SCI_STYLESETFONT,STYLE_BRACELIGHT, (LPARAM) "Consolas");
    //Sci (SCI_STYLESETFONT,STYLE_BRACELIGHT, (LPARAM) "Courier");
	Sci (SCI_STYLESETBOLD, STYLE_BRACELIGHT, 1);
	Sci (SCI_STYLESETITALIC, STYLE_BRACELIGHT, 0);
	Sci (SCI_STYLESETUNDERLINE, STYLE_BRACELIGHT, 0);
	Sci (SCI_STYLESETSIZE, STYLE_BRACELIGHT, 12);


	Sci (SCI_INDICSETSTYLE, SCE_UNIVERSAL_FOUND_STYLE_SMART, INDIC_ROUNDBOX);
	Sci (SCI_INDICSETALPHA, SCE_UNIVERSAL_FOUND_STYLE_SMART, 100);
	Sci (SCI_INDICSETUNDER, SCE_UNIVERSAL_FOUND_STYLE_SMART, true);
	Sci (SCI_INDICSETFORE,  SCE_UNIVERSAL_FOUND_STYLE_SMART, RGB(0, 0xFF, 0));
}

void CLuaEditor::SetCallStackMargins()
{
	Sci(SCI_SETMARGINTYPEN, 1, SC_MARGIN_SYMBOL);
//	Sci(SCI_SETMARGINWIDTHN, 1, 10);
	Sci(SCI_SETMARGINSENSITIVEN, 1, FALSE);

	Sci(SCI_MARKERDEFINE, 0, SC_MARK_ARROW);
}

void CLuaEditor::SetReadOnly(BOOL bReadOnly)
{
	Sci(SCI_SETREADONLY, bReadOnly);
}

void CLuaEditor::AddText(const char* szText)
{
	Sci(SCI_ADDTEXT, strlen(szText), (int)szText);
}

void CLuaEditor::ClearAll()
{
	Sci(SCI_CLEARALL);
}

CString CLuaEditor::GetLine(int nLine)
{
	CString strLine;
	int nLineLength = Sci(SCI_LINELENGTH, nLine-1);
	if ( nLineLength>0 )
	{
		Sci(SCI_GETLINE, nLine-1, (int)strLine.GetBuffer(nLineLength));
		strLine.ReleaseBuffer();
	}

	return strLine;
}

void CLuaEditor::GotoLastLine()
{
	int nLine = Sci(SCI_GETLINECOUNT);
	Sci(SCI_GOTOLINE, nLine);
}

int CLuaEditor::GetCurrentLine()
{
	return Sci(SCI_LINEFROMPOSITION, Sci(SCI_GETCURRENTPOS)) + 1;

}

int CLuaEditor::GetLineCount()
{
	int nLine = Sci(SCI_GETLINECOUNT);
    return nLine;
}

void CLuaEditor::SelectLine(int nLine)
{
	int lineStart = Sci(SCI_POSITIONFROMLINE, nLine-1);
    int lineEnd = lineStart + GetLine (nLine).GetLength ();
    Sci(SCI_GOTOLINE, nLine-1);
    Sci (SCI_SETSELECTIONSTART, lineStart);
    Sci (SCI_SETSELECTIONEND, lineEnd);
}

void CLuaEditor::SetStackTraceLevel(int nLevel)
{
	Sci(SCI_MARKERDELETEALL, 0);
	Sci(SCI_MARKERADD, nLevel, 0);
}

CharacterRange CLuaEditor::GetSelection() 
{
	CharacterRange crange;
	crange.cpMin = Sci(SCI_GETSELECTIONSTART);
	crange.cpMax = Sci(SCI_GETSELECTIONEND);
	return crange;
}

BOOL CLuaEditor::PreparePrint(CDC *pDC, CPrintInfo *pInfo)
{
	CharacterRange crange = GetSelection();
	int startPos = crange.cpMin;
	int endPos = crange.cpMax;

	LONG lengthDoc = Sci(SCI_GETLENGTH);
	LONG lengthPrinted = 0;
	LONG lengthDocMax = lengthDoc;

	// Requested to print selection
	if (pInfo->m_pPD->m_pd.Flags & PD_SELECTION) {
		if (startPos > endPos) {
			lengthPrinted = endPos;
			lengthDoc = startPos;
		} else {
			lengthPrinted = startPos;
			lengthDoc = endPos;
		}

		if (lengthPrinted < 0)
			lengthPrinted = 0;
		if (lengthDoc > lengthDocMax)
			lengthDoc = lengthDocMax;
	}

	Sci(SCI_SETWRAPMODE, SC_WRAP_WORD);

	m_pages.RemoveAll();

	RangeToFormat frPrint;
	frPrint.hdc = pDC->GetSafeHdc();
	frPrint.hdcTarget = pDC->m_hAttribDC;
	frPrint.rcPage.left		= frPrint.rc.left	= 0;
	frPrint.rcPage.right	= frPrint.rc.right	= pDC->GetDeviceCaps(HORZRES);
	frPrint.rcPage.top		= frPrint.rc.top	= 0;
	frPrint.rcPage.bottom	= frPrint.rc.bottom = pDC->GetDeviceCaps(VERTRES);

	while (lengthPrinted < lengthDoc) {
		frPrint.chrg.cpMin = lengthPrinted;
		frPrint.chrg.cpMax = lengthDoc;

		m_pages.Add(lengthPrinted);

		lengthPrinted = Sci(SCI_FORMATRANGE, FALSE,
		                           reinterpret_cast<LPARAM>(&frPrint));
	}

	Sci(SCI_FORMATRANGE, FALSE, 0);

	pInfo->SetMaxPage(m_pages.GetSize());

	return TRUE;
}

void CLuaEditor::PrintPage(CDC* pDC, CPrintInfo* pInfo)
{
	RangeToFormat frPrint;
	frPrint.hdc = pDC->GetSafeHdc();
	frPrint.hdcTarget = pDC->m_hAttribDC;
	frPrint.rc.left = pInfo->m_rectDraw.left;
	frPrint.rc.right = pInfo->m_rectDraw.right;
	frPrint.rc.top = pInfo->m_rectDraw.top;
	frPrint.rc.bottom = pInfo->m_rectDraw.bottom;
	frPrint.rcPage.left = pInfo->m_rectDraw.left;
	frPrint.rcPage.right = pInfo->m_rectDraw.right;
	frPrint.rcPage.top = pInfo->m_rectDraw.top;
	frPrint.rcPage.bottom = pInfo->m_rectDraw.bottom;

	frPrint.chrg.cpMin = m_pages[pInfo->m_nCurPage - 1];
	frPrint.chrg.cpMax = Sci(SCI_GETLENGTH);

	Sci(SCI_FORMATRANGE, TRUE, reinterpret_cast<LPARAM>(&frPrint));
}

void CLuaEditor::EndPrint(CDC *pDC, CPrintInfo *pInfo)
{
	Sci(SCI_SETWRAPMODE, SC_WRAP_NONE);
}


void CLuaEditor::SetLuaLexer()
{
   CString strPath;
   DWORD dwCount=256;

   apis.Clear();
   apis_function.Clear();

   strPath = GETOPT()->GetPathLibraryHelp ();
  
   ReadAPI(SString(strPath + "\\library.api"),apis);
   ReadAPI(SString(strPath + "\\function.api"),apis_function);

   const char font[] = "Consolas";
   const char monospace[] = "Consolas";
   const short fontsize = 10;
   const char keywords[] = "and break do else elseif end false for function global if in local nil not or repeat return then true until while define undefine ifdef ifnotdef elsedef enddef switch case is new table delete public private object number string boolean virtual";
   char *library = apis_function.GetNearestWords("",0 ,0);
 
   // set style bits, choose the right lexer (Lua) and set the keywords list
   Sci(SCI_SETSTYLEBITS,5,0);
   Sci(SCI_SETLEXER,SCLEX_LUA,0);
   Sci(SCI_SETPROPERTY, reinterpret_cast<WPARAM>("fold"), reinterpret_cast<LPARAM>("1"));
	
   Sci(SCI_SETKEYWORDS,0,(LPARAM)keywords);
   if (library) 
    Sci(SCI_SETKEYWORDS,1,(LPARAM)library);   
   // set up basic features (iguides on, tab=4, tabs-to-spaces, EOL=CRLF)
   Sci(SCI_SETINDENTATIONGUIDES,1,0);
   Sci(SCI_SETTABWIDTH,4,0);
   Sci(SCI_SETUSETABS,0,0);
   Sci(SCI_SETEOLMODE,SC_EOL_CRLF,0);
   // now set up the styles (remember you have to set up font name for each style;
   // if you fail to do so, bold/italics will not work (only color will work)
   // !!colors are in format BGR!!
   // style 32: default
   Sci(SCI_STYLESETBACK, STYLE_DEFAULT, RGB(0xf5,0xf5,0xf5));
   Sci(SCI_STYLESETFONT,32, (LPARAM) font);
   Sci(SCI_STYLESETSIZE,32, fontsize);
   // style 0: whitespace
   Sci(SCI_STYLESETFORE,0, 0x808080);
   Sci(SCI_STYLESETBACK,0, RGB(0xf5,0xf5,0xf5));
   // style 1: block comment --[[...]]
   Sci(SCI_STYLESETFONT,1, (int)monospace);
//   Sci(SCI_STYLESETITALIC,1,1);
   Sci(SCI_STYLESETSIZE,1, fontsize);
   Sci(SCI_STYLESETFORE,1, 0x008000);
   Sci(SCI_STYLESETBACK,1, RGB(0xf5,0xf5,0xf5));
   // style 2: line comment (green)
   Sci(SCI_STYLESETFONT,2, (int)monospace);
//   Sci(SCI_STYLESETITALIC,2,1);
   Sci(SCI_STYLESETSIZE,2, fontsize);
   Sci(SCI_STYLESETFORE,2, 0x008000);
   Sci(SCI_STYLESETBACK,2, RGB(0xf5,0xf5,0xf5));
   // style 3: doc comment (grey???)
   Sci(SCI_STYLESETFORE,3, 0x7F7F7F);      
   Sci(SCI_STYLESETBACK,3, RGB(0xf5,0xf5,0xf5));
   // style 4: numbers 
   Sci(SCI_STYLESETFORE,4, 0x7f7f00);
   Sci(SCI_STYLESETBACK,4, RGB(0xf5,0xf5,0xf5));
   // style 5: keywords
   Sci(SCI_STYLESETFONT,5, (int)font);
   Sci(SCI_STYLESETSIZE,5, (int)fontsize);
   Sci(SCI_STYLESETFORE,5, 0xFF0000);
   Sci(SCI_STYLESETBACK,5, RGB(0xf5,0xf5,0xf5));
   // style 6: double qouted strings (???)
   Sci(SCI_STYLESETFORE,6, 0x800080);
   Sci(SCI_STYLESETBACK,6, RGB(0xf5,0xf5,0xf5));
   // style 7: single quoted strings (???)
   Sci(SCI_STYLESETFORE,7, 0x7F007F);
   Sci(SCI_STYLESETBACK,7, RGB(0xf5,0xf5,0xf5));
   // style 8: UUIDs (IDL only, not used in Lua)
   // style 9: preprocessor directives (not used in Lua 4)
   // style 10: operators
   Sci(SCI_STYLESETFONT,10, (int)font);
   Sci(SCI_STYLESETSIZE,10, fontsize);
   Sci(SCI_STYLESETFORE,10, 0x000000);
   Sci(SCI_STYLESETBACK,10, RGB(0xf5,0xf5,0xf5));
   // style 11: identifiers (leave to default)
   Sci(SCI_STYLESETBACK,11, RGB(0xf5,0xf5,0xf5));
   // style 12: end of line where string is not closed
   Sci(SCI_STYLESETFORE,12, 0x000000);
   Sci(SCI_STYLESETBACK,12, 0xE0C0E0);
   Sci(SCI_STYLESETEOLFILLED,12, 1);
   // style 13: library
   Sci(SCI_STYLESETFONT,13, (int)font);
   Sci(SCI_STYLESETSIZE,13, (int)fontsize);
   Sci(SCI_STYLESETFORE,13, 0x5D5D5E);
   Sci(SCI_STYLESETBACK,13, RGB(0xf5,0xf5,0xf5));

   //type and color bookmark
   Sci(SCI_MARKERSETFORE, ADAMO_MARKER_BOOKMARK,0x7F0000);
   Sci(SCI_MARKERSETBACK, ADAMO_MARKER_BOOKMARK,0xFFFF80);
   Sci(SCI_MARKERDEFINE, ADAMO_MARKER_BOOKMARK, SC_MARK_SMALLRECT);

   SetFoldersStyles ();

   wordCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_%";
   autoCompleteStartCharacters = ".";

   callTipIgnoreCase = 0;
   calltipWordCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_#.";
   calltipEndDefinition = ")";
   autoCompleteFillUpCharacters = "";
   autoCompleteIgnoreCase = false;
   indentMaintain = true;
   indentOpening = false;
   indentClosing = false;

   Sci(SCI_CALLTIPSETBACK,0xffffff,0);
   SciString(SCI_AUTOCSETFILLUPS,0,autoCompleteFillUpCharacters.c_str());
   Sci(SCI_AUTOCSETIGNORECASE, autoCompleteIgnoreCase ? 1 : 0);

   delete []library;
   SetSkin ();
}


void CLuaEditor::OnMouseMove(UINT nFlags, CPoint point) 
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if ( m_bShowCalltips && pFrame->GetMode()==CMainFrame::modeDebugBreak )
	{
		char  linebuf[1000];
		int  pos  =  Sci(SCI_POSITIONFROMPOINT, point.x, point.y);
		int start = Sci(SCI_WORDSTARTPOSITION, pos, TRUE);
		int end = Sci(SCI_WORDENDPOSITION, pos, TRUE);
		TextRange tr;
		tr.chrg.cpMin = start;
		tr.chrg.cpMax = end;
		tr.lpstrText = linebuf;
		Sci(SCI_GETTEXTRANGE, 0, long(&tr));
		
		CString strCalltip;
		if ( pFrame->GetCalltip(linebuf, strCalltip) )
		{
			if  (Sci(SCI_CALLTIPACTIVE) && m_strCallTip!=strCalltip)
					Sci(SCI_CALLTIPCANCEL);

			if (!Sci(SCI_CALLTIPACTIVE))
			{
				Sci(SCI_CALLTIPSHOW,  start,  (int)strCalltip.GetBuffer(0));
				strCalltip.ReleaseBuffer();
				m_strCallTip = strCalltip;
			};
		}
		else if (Sci(SCI_CALLTIPACTIVE))
					Sci(SCI_CALLTIPCANCEL);
	}

    CWnd::OnMouseMove(nFlags, point);
}

int CLuaEditor::SciString(int nCmd, int wParam, const char *lParam)
{
	return Sci(nCmd, wParam, reinterpret_cast<sptr_t>(lParam));
}

int CLuaEditor::GetCurrentLineNumber()
{
	return Sci(SCI_LINEFROMPOSITION,Sci(SCI_GETCURRENTPOS));
}

int CLuaEditor::GetCaretInLine()
{
	int caret = Sci(SCI_GETCURRENTPOS);
	int line = Sci(SCI_LINEFROMPOSITION, caret);
	int lineStart = Sci(SCI_POSITIONFROMLINE, line);
	return caret - lineStart;
}

void CLuaEditor::GetLine(char *text, int sizeText, int line)
{
	if (line < 0)
		line = GetCurrentLineNumber();
	int lineStart = Sci(SCI_POSITIONFROMLINE, line);
	int lineEnd = Sci(SCI_GETLINEENDPOSITION, line);
	int lineMax = lineStart + sizeText - 1;
	if (lineEnd > lineMax)
		lineEnd = lineMax;
	GetRange(lineStart, lineEnd, text);
	text[lineEnd - lineStart] = '\0';
}

void CLuaEditor::CharAdded(char nChar)
{
	CharacterRange crange = GetSelection();
	int selStart = crange.cpMin;
	int selEnd = crange.cpMax;
	if ((selEnd == selStart) && (selStart > 0)) {
		int style = Sci(SCI_GETSTYLEAT, selStart - 1, 0);
		if (style != 1) {
			if (Sci(SCI_CALLTIPACTIVE)) {
				if (nChar == ')') {
					braceCount--;
					if (braceCount < 1)
						Sci(SCI_CALLTIPCANCEL);
					else
						StartCallTip();
				} else if (nChar == '(') {
					braceCount++;
					StartCallTip();
				} else if (autoCompleteStartCharacters.contains(nChar)) {
					ShowEnumInCallTip ();
				}
				else   {
					ContinueCallTip();
				}
			} else if (Sci(SCI_AUTOCACTIVE)) {
				if (nChar == '(') {
					braceCount++;
					StartCallTip();
				} else if (nChar == ')') {
					braceCount--;
				} else if (!wordCharacters.contains(nChar)) {
					Sci(SCI_AUTOCCANCEL);
					if (autoCompleteStartCharacters.contains(nChar)) {
						StartAutoComplete();
						StartAutoCompFunc();
					}
				} 
				else 
				if (autoCCausedByOnlyOne) {
					StartAutoCompleteWord(true);
				}
			} else {
				if (nChar == '(') {
					braceCount = 1;
					StartCallTip();
				} else {
					if (IsCallTipSeparator (nChar))   {
						StartCallTip();
					}
					else   {
						autoCCausedByOnlyOne = false;
						if (indentMaintain)
							MaintainIndentation(nChar);
						if (autoCompleteStartCharacters.contains(nChar)) {
							StartAutoCompFunc();
						} else if (wordCharacters.contains(nChar)) {
							StartAutoCompleteWord(true);
							if(Sci(SCI_AUTOCACTIVE))
								autoCCausedByOnlyOne = true;
							else
								autoCCausedByOnlyOne = false;
							AutoSelectHelpOptions ();
						}
					}
				}
			}
		}
	}
}

bool CLuaEditor::StartCallTip()
{
	char linebuf[1000];
	currentCallTip = 0;
	currentCallTipWord = "";
	GetLine(linebuf, sizeof(linebuf));
	int current = GetCaretInLine();
	int pos = Sci(SCI_GETCURRENTPOS);
	int braces;
	do {
		braces = 0;
		while (current > 0 && (braces || linebuf[current - 1] != '(')) {
			if (linebuf[current - 1] == '(')
				braces--;
			else if (linebuf[current - 1] == ')')
				braces++;
			current--;
			pos--;
		}
		if (current > 0) {
			current--;
			pos--;
		} else
			break;
		while (current > 0 && isspace(linebuf[current - 1])) {
			current--;
			pos--;
		}
	} while (current > 0 && !calltipWordCharacters.contains(linebuf[current - 1]));
	if (current <= 0)
		return true;

	startCalltipWord = current - 1;
	while (startCalltipWord > 0 &&
	        calltipWordCharacters.contains(linebuf[startCalltipWord - 1])) {
		startCalltipWord--;
	}

	linebuf[current] = '\0';
	currentCallTipWord = linebuf + startCalltipWord;
	functionDefinition = "";
	FillFunctionDefinition(pos);
	return true;
}

bool CLuaEditor::StartAutoComplete()
{
	char linebuf[1000];
	GetLine(linebuf, sizeof(linebuf));
	int current = GetCaretInLine();

	int startword = current;

	while ((startword > 0) &&
	        (wordCharacters.contains(linebuf[startword - 1]) ||
	         autoCompleteStartCharacters.contains(linebuf[startword - 1])))
		startword--;

	linebuf[current] = '\0';
	const char *root = linebuf + startword;
	int rootlen = current - startword;
	if (apis) {
		char *words = apis.GetNearestWords(root, rootlen, autoCompleteIgnoreCase);
		if (words) {
			ShowLibraries ();
			EliminateDuplicateWords(words);
			//SciString(SCI_AUTOCSHOW, rootlen, words);
			Sci(SCI_CALLTIPCANCEL);
			delete []words;
		}
	}
	return true;
}

void CLuaEditor::ContinueCallTip()
{
	char linebuf[1000];
	GetLine(linebuf, sizeof(linebuf));
	int current = GetCaretInLine();

	int braces = 0;
	int commas = 0;
	for (int i = startCalltipWord; i < current; i++) {
		if (linebuf[i] == '(')
			braces++;
		else if (linebuf[i] == ')' && braces > 0)
			braces--;
		else if (braces == 1 && IsCallTipSeparator(linebuf[i]))
			commas++;
	}
	int startHighlight = 0;
	while (functionDefinition[startHighlight] && functionDefinition[startHighlight] != '(')
		startHighlight++;
	if (functionDefinition[startHighlight] == '(')
		startHighlight++;
	while (functionDefinition[startHighlight] && commas > 0) {
		if (IsCallTipSeparator(functionDefinition[startHighlight]))
			commas--;
		// If it reached the end of the argument list it means that the user typed in more
		// arguments than the ones listed in the calltip
		if (functionDefinition[startHighlight] == ')')
			commas = 0;
		else
			startHighlight++;
	}
	if (IsCallTipSeparator(functionDefinition[startHighlight]))
		startHighlight++;
	int endHighlight = startHighlight;
	while (functionDefinition[endHighlight] && !IsCallTipSeparator(functionDefinition[endHighlight]) && functionDefinition[endHighlight] != ')')
		endHighlight++;

	Sci(SCI_CALLTIPSETHLT, startHighlight, endHighlight);
}

void CLuaEditor::GetRange(int start, int end, char *text)
{
	TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = text;
	Sci(SCI_GETTEXTRANGE, 0, (LPARAM)&tr);
}

void CLuaEditor::EliminateDuplicateWords(char *words)
{
	char *firstWord = words;
	char *firstSpace = strchr(firstWord, ' ');
	char *secondWord;
	char *secondSpace;
	int firstLen, secondLen;

	while (firstSpace) {
		firstLen = firstSpace - firstWord;
		secondWord = firstWord + firstLen + 1;
		secondSpace = strchr(secondWord, ' ');

		if (secondSpace)
			secondLen = secondSpace - secondWord;
		else
			secondLen = strlen(secondWord);

		if (firstLen == secondLen &&
		        !strncmp(firstWord, secondWord, firstLen)) {
			strcpy(firstWord, secondWord);
			firstSpace = strchr(firstWord, ' ');
		} else {
			firstWord = secondWord;
			firstSpace = secondSpace;
		}
	}
}

bool CLuaEditor::StartAutoCompleteWord(bool onlyOneWord)
{
	char linebuf[1000];
	GetLine(linebuf, sizeof(linebuf));
	int current = GetCaretInLine();
	if (current >= static_cast<int>(sizeof(linebuf)))
		return false;
	
	int startword = current;
	while (startword > 0 && wordCharacters.contains(linebuf[startword - 1]))
		startword--;
	if (startword == current)
		return true;
	linebuf[current] = '\0';
	const char *root = linebuf + startword;
	int rootlen = current - startword;
	int doclen = LengthDocument();
	TextToFind ft = {{0, 0}, 0, {0, 0}};
	ft.lpstrText = const_cast<char*>(root);
	ft.chrg.cpMin = 0;
	ft.chrgText.cpMin = 0;
	ft.chrgText.cpMax = 0;
	int flags = SCFIND_WORDSTART | (autoCompleteIgnoreCase ? 0 : SCFIND_MATCHCASE);
	int posCurrentWord = Sci(SCI_GETCURRENTPOS) - rootlen;
	int minWordLength = 0;
	int nwords = 0;

	// wordsNear contains a list of words separated by single spaces and with a space
	// at the start and end. this makes it easy to search for words.
	SString wordsNear;
	wordsNear.setsizegrowth(1000);
	wordsNear += " ";

	for (;;) {	// search all the document
		ft.chrg.cpMax = doclen;
		int posFind = SciString(SCI_FINDTEXT, flags, reinterpret_cast<char *>(&ft));
		if (posFind == -1 || posFind >= doclen)
			break;
		if (posFind == posCurrentWord) {
			ft.chrg.cpMin = posFind + rootlen;
			continue;
		}
		// Grab the word and put spaces around it
		const int wordMaxSize = 80;
		char wordstart[wordMaxSize];
		wordstart[0] = ' ';
		GetRange(posFind, __min(posFind + wordMaxSize - 3, doclen), wordstart + 1);
		char *wordend = wordstart + 1 + rootlen;
		while (iswordcharforsel(*wordend))
			wordend++;
		*wordend++ = ' ';
		*wordend = '\0';
		int wordlen = wordend - wordstart - 2;
		if (wordlen > rootlen) {
			if (!wordsNear.contains(wordstart)) {	// add a new entry
				wordsNear += wordstart + 1;
				if (minWordLength < wordlen)
					minWordLength = wordlen;

				nwords++;
				if (onlyOneWord && nwords > 1) {
					return true;
				}
			}
		}
		ft.chrg.cpMin = posFind + wordlen;
	}
	size_t length = wordsNear.length();
	if ((length > 2) && (!onlyOneWord || (minWordLength > rootlen))) {
		WordList wl;
		wl.Set(wordsNear.c_str());
		char *words = wl.GetNearestWords("", 0, autoCompleteIgnoreCase);
		SciString(SCI_AUTOCSHOW, rootlen, words);
		delete []words;
	} else {
		Sci(SCI_AUTOCCANCEL);
	}
	return true;
}

int CLuaEditor::LengthDocument()
{
	return Sci(SCI_GETLENGTH);
}

void CLuaEditor::ReadAPI(const SString &fileNameForExtension,WordList &api)
{
	SString apisFileNames = fileNameForExtension;

	size_t nameLength = apisFileNames.length();
	if (nameLength) {
		const char *apiFileName = fileNameForExtension.c_str();
		const char *nameEnd = apiFileName + nameLength;

		int tlen = 0;    // total api length

		// Calculate total length
		while (apiFileName < nameEnd) {
			tlen += FileLength(apiFileName);
			apiFileName += strlen(apiFileName) + 1;
		}

		// Load files
		if (tlen > 0) {
			char *buffer = api.Allocate(tlen);
			if (buffer) {
				apiFileName = fileNameForExtension.c_str();
				tlen = 0;
				while (apiFileName < nameEnd) {
					FILE *fp = fopen(apiFileName, "rb");
					if (fp) {
						fseek(fp, 0, SEEK_END);
						int len = ftell(fp);
						fseek(fp, 0, SEEK_SET);
						fread(buffer + tlen, 1, len, fp);
						tlen += len;
						fclose(fp);
					}
					apiFileName += strlen(apiFileName) + 1;
				}
				api.SetFromAllocated();
			}
		}
	}
}

int CLuaEditor::FileLength(const char *path)
{
	int len = 0;
	FILE *fp = fopen(path, "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		len = ftell(fp);
		fclose(fp);
	}
	return len;
}

void CLuaEditor::FillFunctionDefinition(int pos)
{
	CString strCallTip;
	if (pos > 0) {
		lastPosCallTip = pos;
	}
	if (GetCallTipText (currentCallTipWord.c_str(), strCallTip))   {
		maxCallTips = 1;

		// Should get current api definition
		functionDefinition = strCallTip.GetBuffer ();;
			if (maxCallTips > 1) {
				functionDefinition.insert(0, "\001");
			}

			if (calltipEndDefinition != "") {
				int posEndDef = functionDefinition.search(calltipEndDefinition.c_str());
				if (maxCallTips > 1) {
					if ((posEndDef > 1) &&
					        ((posEndDef + calltipEndDefinition.length()) < functionDefinition.length())) {
						functionDefinition.insert(posEndDef + calltipEndDefinition.length(), "\n\002");
					} else {
						functionDefinition.append("\n\002");
					}
				} else {
					if ((posEndDef > 1) &&
					        ((posEndDef + calltipEndDefinition.length()) < functionDefinition.length())) {
						functionDefinition.insert(posEndDef + calltipEndDefinition.length(), "\n");
					}
				}
			}
			SciString(SCI_CALLTIPSHOW, lastPosCallTip - currentCallTipWord.length(), functionDefinition.c_str());
			ContinueCallTip();
		}
	}

void CLuaEditor::MaintainIndentation(char ch)
{
	int eolMode = Sci(SCI_GETEOLMODE);
	int curLine = GetCurrentLineNumber();
	int lastLine = curLine - 1;
	int indentAmount = 0;

	if (((eolMode == SC_EOL_CRLF || eolMode == SC_EOL_LF) && ch == '\n') ||
	        (eolMode == SC_EOL_CR && ch == '\r')) {
		if (true) {
			while (lastLine >= 0 && GetLineLength(lastLine) == 0)
				lastLine--;
		}
		if (lastLine >= 0) {
			indentAmount = GetLineIndentation(lastLine);
		}
		if (indentAmount > 0) {
			SetLineIndentation(curLine, indentAmount);
		}
	}
}

int CLuaEditor::GetLineLength(int line)
{
	return Sci(SCI_GETLINEENDPOSITION, line) - Sci(SCI_POSITIONFROMLINE, line);
}

int CLuaEditor::GetLineIndentation(int line)
{
	return Sci(SCI_GETLINEINDENTATION, line);
}

void CLuaEditor::SetLineIndentation(int line, int indent)
{
	if (indent < 0)
		return;
	CharacterRange crange = GetSelection();
	int posBefore = GetLineIndentPosition(line);
	Sci(SCI_SETLINEINDENTATION, line, indent);
	int posAfter = GetLineIndentPosition(line);
	int posDifference = posAfter - posBefore;
	if (posAfter > posBefore) {
		// Move selection on
		if (crange.cpMin >= posBefore) {
			crange.cpMin += posDifference;
		}
		if (crange.cpMax >= posBefore) {
			crange.cpMax += posDifference;
		}
	} else if (posAfter < posBefore) {
		// Move selection back
		if (crange.cpMin >= posAfter) {
			if (crange.cpMin >= posBefore)
				crange.cpMin += posDifference;
			else
				crange.cpMin = posAfter;
		}
		if (crange.cpMax >= posAfter) {
			if (crange.cpMax >= posBefore)
				crange.cpMax += posDifference;
			else
				crange.cpMax = posAfter;
		}
	}
	SetSelection(crange.cpMin, crange.cpMax);
}

int CLuaEditor::GetLineIndentPosition(int line)
{
	return Sci(SCI_GETLINEINDENTPOSITION, line);
}

void CLuaEditor::SetSelection(int anchor, int currentPos)
{
	Sci(SCI_SETSEL, anchor, currentPos);
}

void CLuaEditor::StartAutoCompFunc()
{
	char linebuf[1000];
	GetLine(linebuf, sizeof(linebuf));
	int current = GetCaretInLine();

	int startword = current;

	while ((startword > 0) &&
	        (wordCharacters.contains(linebuf[startword - 1]) ||
	         autoCompleteStartCharacters.contains(linebuf[startword - 1])))
		startword--;

	linebuf[current] = '\0';
	const char *root = linebuf + startword;
	int rootlen = current - startword;
	CString str (root);
	str =str.Left (str.GetLength ()-1);
	/* riempiamo la lista delle librerie */
	FillFunctions (str);
	if (m_slOptions.GetCount () != 0)
		ShowFunctions (str);
	else   {
		FillEnumsValues (str);
		if (m_slOptions.GetCount () != 0)
			ShowEnumsValues (str);
	}
}

void CLuaEditor::EliminateLibrary(char *words)
{
	int start = 0;
	int end;
	CString sWords = words;
	CString sRis;

	sWords = sWords + " ";

	do{
		start = sWords.Find('.',start) + 1;
		end = sWords.Find(' ',start) + 1;
		sRis = sRis + sWords.Mid(start,end - start);
	}while(end < sWords.GetLength() - 1);

	strcpy(words,(LPCTSTR)sRis.Mid(0,sRis.GetLength()-1));
}

void CLuaEditor::DeleteCallTip()
{
	Sci(SCI_CALLTIPCANCEL);
}

CString CLuaEditor::GetSelectedText()
{
   long lLen = (GetSelectionEnd() - GetSelectionStart()) + 1;
   if (lLen > 0)   {
      TCHAR *p = new TCHAR[lLen+1];
      if (p != NULL)   {
         *p = '\0';
         Sci(SCI_GETSELTEXT, 0, (long)p);
         CString strReturn = p;
         delete [] p;
         return strReturn;
      }
   }
   return _T("");
}

long CLuaEditor::GetSelectionStart()
{
   return Sci(SCI_GETSELECTIONSTART, 0, 0);
}

long CLuaEditor::GetSelectionEnd()
{
   return Sci(SCI_GETSELECTIONEND, 0, 0);
}

BOOL CLuaEditor::SearchForward(LPSTR szText)
{
	int nLine;
    bool bAgain=true;
    if (szText == NULL)
      return FALSE;
    long lPos = GetCurrentPosition();
    long lMaxSel = GetSelectionEnd();
    TextToFind tf;
    tf.lpstrText = szText;
    if (lMaxSel >= 0)
        tf.chrg.cpMin = lMaxSel+1;
    else
        tf.chrg.cpMin = lPos+1;
    tf.chrg.cpMax = Sci(SCI_GETLENGTH, 0, 0);
    while (true)   {
        lPos = Sci(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
        if (lPos >= 0)   {
          SetFocus();
		  nLine = GetLineFromPosition (lPos);
		  EnsureVisible(nLine);
          GotoPosition(lPos);
          Sci(SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
          Sci(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
          return TRUE;
        }
        else   {
            if (bAgain)   {
                tf.chrg.cpMax=tf.chrg.cpMin;
                tf.chrg.cpMin=0;
                bAgain=false;
            }
            else
                break;
        }        
    }
    return FALSE;
}

LONG CLuaEditor::GetCurrentPosition()
{
   return Sci(SCI_GETCURRENTPOS, 0, 0);
}

void CLuaEditor::GotoPosition(long lPos)
{
	Sci(SCI_GOTOPOS, lPos, 0);
}

BOOL CLuaEditor::SearchBackward(LPSTR szText)
{
	int nLine;
    bool bAgain=true;

    if (szText == NULL)
        return FALSE;
    long lPos = GetCurrentPosition();
    long lMinSel = GetSelectionStart();
    TextToFind tf;
    tf.lpstrText = szText;
    if (lMinSel >= 0)
        tf.chrg.cpMin = lMinSel-1;
    else
        tf.chrg.cpMin = lPos-1;
    tf.chrg.cpMax = 0;
    while (true)   {
        lPos = Sci(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
        if (lPos >= 0)   {
            SetFocus();
		    nLine = GetLineFromPosition (lPos);
		    EnsureVisible(nLine);
            GotoPosition(lPos);
            Sci(SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
            Sci(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
            return TRUE;
        }
        else   {
            if (bAgain)   {
                tf.chrg.cpMax=tf.chrg.cpMin;
                tf.chrg.cpMin=Sci(SCI_GETLENGTH, 0, 0);
                bAgain=false;
            }
            else
                break;
        }        
    }
    return FALSE;
}

void CLuaEditor::ReplaceSearchedText(LPCSTR szText)
{
   if (szText == NULL)
      return;
	Sci(SCI_TARGETFROMSELECTION, 0, 0);
   if (m_nSearchflags & SCFIND_REGEXP)
      Sci(SCI_REPLACETARGETRE, strlen(szText), (long)szText);
   else
      Sci(SCI_REPLACETARGET, strlen(szText), (long)szText);
}

int CLuaEditor::ReplaceAll(LPCSTR szFind, LPCSTR szReplace, BOOL bUseSelection)
{
   int nCount = 0;
// different branches for replace in selection or total file
   if (bUseSelection)   {
//    get starting selection range
      long lLen = 0;
      long lStart = GetSelectionStart();
      long lEnd = GetSelectionEnd();
//    set target to selection
      Sci(SCI_SETTARGETSTART, lStart);
      Sci(SCI_SETTARGETEND, lEnd);
//    try to find text in target for the first time
	  long lPos = Sci(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
//    loop over selection until end of selection reached - moving the target start each time
      while (lPos < lEnd && lPos >= 0)   {
         if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
            lLen = Sci(SCI_REPLACETARGETRE, strlen(szReplace), (long)szReplace);
         else
            lLen = Sci(SCI_REPLACETARGET, strlen(szReplace), (long)szReplace);
//       the end of the selection was changed - recalc the end
         lEnd = GetSelectionEnd();
//       move start of target behind last change and end of target to new end of selection
         Sci(SCI_SETTARGETSTART, lPos+lLen);
         Sci(SCI_SETTARGETEND, lEnd);
//       find again - if nothing found loop exits
	      lPos = Sci(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
         nCount++;
      }
   }
   else   {
//    start with first and last char in buffer
      long lLen = 0;
      long lStart = 0;
      long lEnd = Sci(SCI_GETTEXTLENGTH, 0, 0);
//    set target to selection
      Sci(SCI_SETTARGETSTART, lStart, 0);
      Sci(SCI_SETTARGETEND, lEnd, 0);
//    try to find text in target for the first time
	   long lPos = Sci(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
//    loop over selection until end of selection reached - moving the target start each time
      while (lPos < lEnd && lPos >= 0)   {
         if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
            lLen = Sci(SCI_REPLACETARGETRE, strlen(szReplace), (long)szReplace);
         else
            lLen = Sci(SCI_REPLACETARGET, strlen(szReplace), (long)szReplace);
//       the end of the selection was changed - recalc the end
         lEnd = Sci(SCI_GETTEXTLENGTH, 0, 0);;
//       move start of target behind last change and end of target to new end of buffer
         Sci(SCI_SETTARGETSTART, lPos+lLen);
         Sci(SCI_SETTARGETEND, lEnd);
//       find again - if nothing found loop exits
	      lPos = Sci(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
         nCount++;
      }
   }
   return nCount;
}

long CLuaEditor::GetCurrentColumn()
{
   long lPos = Sci(SCI_GETCURRENTPOS, 0, 0);
   return Sci(SCI_GETCOLUMN, lPos, 0) + 1;
}

void CLuaEditor::BookmarkToggle(int lineno)
{
	if (lineno == -1)
		lineno = GetCurrentLineNumber();
	if (BookmarkPresent(lineno)) {
		BookmarkDelete(lineno);
	} else {
		BookmarkAdd(lineno);
	}
}

void CLuaEditor::BookmarkAdd(int lineno)
{
	if (lineno == -1)
		lineno = GetCurrentLineNumber();
	if (!BookmarkPresent(lineno))
		Sci(SCI_MARKERADD, lineno, ADAMO_MARKER_BOOKMARK);
}

void CLuaEditor::BookmarkDelete(int lineno)
{
	if (lineno == -1)
		lineno = GetCurrentLineNumber();
	if (BookmarkPresent(lineno))
		Sci(SCI_MARKERDELETE, lineno, ADAMO_MARKER_BOOKMARK);
}

bool CLuaEditor::BookmarkPresent(int lineno)
{
	if (lineno == -1)
		lineno = GetCurrentLineNumber();
	int state = Sci(SCI_MARKERGET, lineno);

    if(state & (1 << ADAMO_MARKER_BOOKMARK))
        return true;

    return false;

}

void CLuaEditor::BookmarkNext(bool forwardScan)
{
	int lineno = GetCurrentLineNumber();
	int sci_marker = SCI_MARKERNEXT;
	int lineStart = lineno + 1;	//Scan starting from next line
	int lineRetry = 0;				//If not found, try from the beginning
	if (!forwardScan) {
		lineStart = lineno - 1;		//Scan starting from previous line
		lineRetry = Sci(SCI_GETLINECOUNT, 0, 0L);	//If not found, try from the end
		sci_marker = SCI_MARKERPREVIOUS;
	}
	int nextLine = Sci(sci_marker, lineStart, 1 << ADAMO_MARKER_BOOKMARK);
	if (nextLine < 0)
		nextLine = Sci(sci_marker, lineRetry, 1 << ADAMO_MARKER_BOOKMARK);
	if (nextLine < 0 || nextLine == lineno);	// No bookmark (of the given type) or only one, and already on it
	else {
		GotoLineEnsureVisible(nextLine);
	}
}

void CLuaEditor::GotoLineEnsureVisible(int line)
{
	Sci(SCI_ENSUREVISIBLEENFORCEPOLICY, line);
	Sci(SCI_GOTOLINE, line);
}

void CLuaEditor::EnsureVisible(int line)
{
	Sci(SCI_ENSUREVISIBLEENFORCEPOLICY, line);
}

void CLuaEditor::BookmarkClearAll()
{
    Sci(SCI_MARKERDELETEALL, ADAMO_MARKER_BOOKMARK);
}

CString CLuaEditor::GetCaretText()
{
	char  linebuf[1024];

    linebuf[0]='\0';
	int pos   = GetCurrentPosition ();
	int start = Sci(SCI_WORDSTARTPOSITION, pos, TRUE);
	int end = Sci(SCI_WORDENDPOSITION, pos, TRUE);
	TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = linebuf;
	Sci(SCI_GETTEXTRANGE, 0, long(&tr));
    return CString (linebuf);
}

CString CLuaEditor::GetCaretSymbol(CString& strSymbol, int& nSymbolType)
{
	char  linebuf[1024];

    linebuf[0]='\0';
	int pos   = GetCurrentPosition ();
	int origpos = pos;
	int start, end;
	bool bCeIlPunto = true, bIsMethod = false;

	/* per prima cosa ricaviamo il simbolo */
	while (bCeIlPunto)   {
		start = Sci(SCI_WORDSTARTPOSITION, pos, TRUE);
		TCHAR charBefore = Sci (SCI_GETCHARAT, start - 1, 0);
		if (charBefore == '.')
			pos = start - 1;
		else {
			if (charBefore == ':')
				bIsMethod = true;
			bCeIlPunto = false;
		}
	}
	bCeIlPunto = true;
	pos = origpos;
	while (bCeIlPunto)   {
		end = Sci(SCI_WORDENDPOSITION, pos, TRUE);
		TCHAR charBefore = Sci (SCI_GETCHARAT, end, 0);
		if (charBefore == '.')
			pos = end + 1;
		else
			bCeIlPunto = false;
	}
	TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = linebuf;
	Sci(SCI_GETTEXTRANGE, 0, long(&tr));
	if (linebuf[0] == '.' || linebuf[0] == ':')
		memmove (linebuf, &linebuf[1], strlen (linebuf));
	strSymbol = linebuf;
	/* ora il tipo di simbolo */
	nSymbolType = -1;
	if (GetCaretWordBefore() == "new")
		nSymbolType = BROWSER_TYPE_OBJECT;
	else
		if (bIsMethod)
			nSymbolType = BROWSER_TYPE_METHOD;
	return strSymbol;
}

CString CLuaEditor::GetCaretWordBefore()
{
	CString str;
	int start, end, lineStart, line;
	int pos = GetCurrentPosition();
	char  linebuf[1024], charBefore = ' ';

	start = Sci(SCI_WORDSTARTPOSITION, pos, TRUE);
	line = Sci(SCI_LINEFROMPOSITION, pos);
	lineStart = Sci(SCI_POSITIONFROMLINE, line);
	while (--start > lineStart && (charBefore == ' ' || charBefore == '\t'))
		charBefore = Sci(SCI_GETCHARAT, start, 0);
	if (start > lineStart) {
		pos = start;
		start = Sci(SCI_WORDSTARTPOSITION, pos, TRUE);
		end = Sci(SCI_WORDENDPOSITION, pos, TRUE);
		TextRange tr;
		tr.chrg.cpMin = start;
		tr.chrg.cpMax = end;
		tr.lpstrText = linebuf;
		Sci(SCI_GETTEXTRANGE, 0, long(&tr));
		str = CString (linebuf);
	}
	return str;
}

void CLuaEditor::ClearAllLines ()
{
	Sci(SCI_MARKERDELETEALL, 1);
}

void CLuaEditor::CleanSelection ()
{
    int currentPos = Sci(SCI_GETCURRENTPOS);
    Sci (SCI_SETSELECTIONSTART, currentPos);
    Sci (SCI_SETSELECTIONEND, currentPos);
}

void CLuaEditor::SelectCurrentWord ()
{
    int pos = Sci(SCI_GETCURRENTPOS, 0, 0);
	int start = Sci(SCI_WORDSTARTPOSITION, pos, TRUE);
	int end = Sci(SCI_WORDENDPOSITION, pos, TRUE);
	Sci(SCI_SETSEL, start, end);
}

void CLuaEditor::SelectCurrentSymbol ()
{
    int pos = Sci(SCI_GETCURRENTPOS, 0, 0);
	int origpos = pos;
	int start, end;
	bool bCeIlPunto = true;

	while (bCeIlPunto)   {
		start = Sci(SCI_WORDSTARTPOSITION, pos, TRUE);
		TCHAR charBefore = Sci (SCI_GETCHARAT, start - 1, 0);
		if (charBefore == '.')
			pos = start - 1;
		else
			bCeIlPunto = false;
	}
	bCeIlPunto = true;
	pos = origpos;
	while (bCeIlPunto)   {
		end = Sci(SCI_WORDENDPOSITION, pos, TRUE);
		TCHAR charBefore = Sci (SCI_GETCHARAT, end, 0);
		if (charBefore == '.')
			pos = end + 1;
		else
			bCeIlPunto = false;
	}
	TCHAR ch = Sci (SCI_GETCHARAT, start, 0);
	if (ch == '.')
		start++;
	Sci(SCI_SETSEL, start, end);
}

void CLuaEditor::updateLineNumberWidth () {
	int linesVisible = (int) Sci(SCI_LINESONSCREEN, 0, 0);
	if (linesVisible)
	{
		int firstVisibleLineVis = (int) Sci(SCI_GETFIRSTVISIBLELINE, 0, 0);
		int lastVisibleLineVis = linesVisible + firstVisibleLineVis + 1;
		int lastVisibleLineDoc = (int) Sci(SCI_DOCLINEFROMVISIBLE, lastVisibleLineVis, 0);
		int i = 0;
		while (lastVisibleLineDoc)
		{
			lastVisibleLineDoc /= 10;
			++i;
		}
		i = max(i, 3);
		{
			int pixelWidth = int(8 + i * Sci(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)"8"));
			Sci(SCI_SETMARGINWIDTHN, 0, pixelWidth);
		}
	}
};


/*
** braceMatch :
*/
bool CLuaEditor::bracematch()
{
	int braceAtCaret = -1, braceOpposite = -1;

	findMatchingBracePos(braceAtCaret, braceOpposite);
	if ((braceAtCaret != -1) && (braceOpposite == -1))    {
		Sci(SCI_BRACEBADLIGHT, braceAtCaret, 0);
		Sci(SCI_SETHIGHLIGHTGUIDE, 0, 0);
	}
    else
		Sci(SCI_BRACEHIGHLIGHT, braceAtCaret, braceOpposite);
	return (braceAtCaret != -1);
}

/*
** findMatchingBracePos :
*/
void CLuaEditor::findMatchingBracePos(int &braceAtCaret, int &braceOpposite)
{
	int caretPos = Sci(SCI_GETCURRENTPOS, 0, 0);
	braceAtCaret = -1;
	braceOpposite = -1;
	TCHAR charBefore = '\0';

	int lengthDoc = Sci (SCI_GETLENGTH, 0, 0);

	if ((lengthDoc > 0) && (caretPos > 0))   {
		charBefore = Sci (SCI_GETCHARAT, caretPos - 1, 0);
	}
	// Priority goes to character before caret
	if (charBefore && strchr(TEXT("[](){}"), charBefore))    {
		braceAtCaret = caretPos - 1;
	}

	if (lengthDoc > 0  && (braceAtCaret < 0))   {
		// No brace found so check other side
		TCHAR charAfter = Sci (SCI_GETCHARAT, caretPos, 0);
		if (charAfter && strchr(TEXT("[](){}"), charAfter))   {
			braceAtCaret = caretPos;
		}
	}
	if (braceAtCaret >= 0)
		braceOpposite = Sci (SCI_BRACEMATCH, braceAtCaret, 0);
}

/*
** smarthilight :
*/
void CLuaEditor::smarthilight()
{
	//Get selection
	CharacterRange range = GetSelection();

	//Clear marks
	clearIndicator(SCE_UNIVERSAL_FOUND_STYLE_SMART);

	//If nothing selected, dont mark anything
	if (range.cpMin == range.cpMax)
	{
		return;
	}

	int textlen = range.cpMax - range.cpMin + 1;

	CString strSelectedtext = GetSelectedText();

	bool valid = true;
	//The word has to consist if wordChars only, and the characters before and after something else
	if (!isQualifiedWord(strSelectedtext))
		valid = false;
	else
	{
		UCHAR c = (UCHAR) Sci (SCI_GETCHARAT, range.cpMax, 0);
		if (c)
		{
			if (isWordChar(char(c)))
				valid = false;
		}
		c = (UCHAR) Sci (SCI_GETCHARAT, range.cpMin-1, 0);
		if (c)
		{
			if (isWordChar(char(c)))
				valid = false;
		}
	}
	if (!valid)
		return;

	// save target locations for other search functions
	int originalStartPos = (int) Sci (SCI_GETTARGETSTART, 0, 0);
	int originalEndPos = (int) Sci (SCI_GETTARGETEND, 0, 0);

	// Get the range of text visible and highlight everything in it
	int firstLine =		(int) Sci (SCI_GETFIRSTVISIBLELINE, 0, 0);
	int nrLines =	min((int) Sci (SCI_LINESONSCREEN, 0, 0), MAXLINEHIGHLIGHT ) + 1;
	int lastLine =		firstLine+nrLines;
	int startPos =		0;
	int endPos =		0;
	int currentLine = firstLine;
	int prevDocLineChecked = -1;	//invalid start

	for(; currentLine < lastLine; ++currentLine)
	{
		int docLine = (int) Sci (SCI_DOCLINEFROMVISIBLE, currentLine, 0);
		if (docLine == prevDocLineChecked)
			continue;	//still on same line (wordwrap)
		prevDocLineChecked = docLine;
		startPos = (int) Sci (SCI_POSITIONFROMLINE, docLine, 0);
		endPos = (int) Sci (SCI_POSITIONFROMLINE, docLine+1, 0);
		if (endPos == -1) {	//past EOF
			endPos = LengthDocument() - 1;
			MarkAll (strSelectedtext, startPos, endPos);
			break;
		} else {
			MarkAll (strSelectedtext, startPos, endPos);
		}
	}
	// restore the original targets to avoid conflicts with the search/replace functions
	Sci (SCI_SETTARGETSTART, originalStartPos, 0);
	Sci (SCI_SETTARGETEND, originalEndPos, 0);
}

/*
** clearIndicator :
*/
void CLuaEditor::clearIndicator(int indicatorNumber) 
{
	int docStart = 0;
	int docEnd = LengthDocument ();
	Sci (SCI_SETINDICATORCURRENT, indicatorNumber);
	Sci (SCI_INDICATORCLEARRANGE, docStart, docEnd-docStart);
};

/*
** isQualifiedWord :
*/
bool CLuaEditor::isQualifiedWord(const char *str) const
{
	for (size_t i = 0, len = strlen(str) ; i < len ; ++i)
	{
		if (!isWordChar(str[i]))
			return false;
	}
	return true;
};

/*
** isWordChar :
*/
bool CLuaEditor::isWordChar(char ch) const
{
	if ((UCHAR)ch < 0x20) 
		return false;
	
	switch(ch)
	{
		case ' ':
		case '	':
		case '\n':
		case '\r':
		case '.':
		case ',':
		case '?':
		case ';':
		case ':':
		case '!':
		case '(':
		case ')':
		case '[':
		case ']':
		case '+':
		case '-':
		case '*':
		case '/':
		case '#':
		case '@':
		case '^':
		case '%':
		case '$':
		case '"':
		case '\'':
		case '~':
		case '&':
		case '{':
		case '}':
		case '|':
		case '=':
		case '<':
		case '>':
		case '\\':
			return false;
	}
	return true;
};

/*
** MarkAll :
*/
int CLuaEditor::MarkAll (CString strToFind, int startRange, int endRange)
{
	int nbProcessed = 0;

	if (startRange == endRange)
		return nbProcessed;
	if (strToFind == "")
		return nbProcessed;

	int stringSizeFind = 0;
	int stringSizeReplace = 0;
	
	int targetStart = 0;
	int targetEnd = 0;

	//Initial range for searching
	DWORD dwFlags = SCFIND_WHOLEWORD | SCFIND_MATCHCASE;
	Sci (SCI_SETSEARCHFLAGS, dwFlags);
	
	while (targetStart != -1 && targetStart != -2)	{
		Sci (SCI_SETTARGETSTART, startRange, 0); Sci (SCI_SETTARGETEND, endRange, 0);
		targetStart = Sci (SCI_SEARCHINTARGET, strToFind.GetLength (), (long)(const char *)strToFind);
		// If we've not found anything, just break out of the loop
		if (targetStart == -1 || targetStart == -2)
			break;
		targetEnd = Sci (SCI_GETTARGETEND, 0, 0);
		if (targetEnd > endRange)
			break;
		int foundTextLen = targetEnd - targetStart;
		int replaceDelta = 0;
		if (foundTextLen > 0)
		{
			Sci (SCI_SETINDICATORCURRENT,  SCE_UNIVERSAL_FOUND_STYLE_SMART, 0);
			Sci (SCI_INDICATORFILLRANGE,  targetStart, foundTextLen);
		}
		++nbProcessed;
        // After the processing of the last string occurence the search loop should be stopped
        // This helps to avoid the endless replacement during the EOL ("$") searching
        if( targetStart + foundTextLen == endRange )
            break;
		startRange = targetStart + foundTextLen + replaceDelta;		//search from result onwards
		endRange += replaceDelta;									//adjust end of range in case of replace
	}  
	return nbProcessed;
}

/*
** doBlockComment : 
*/
bool CLuaEditor::blockcomment(eCommentMode currCommentMode)
{
	//--LS: BlockToStreamComment:
	CString symbolStart;
	CString symbolEnd;

	CString symbol = "--";


    CString comment(symbol);
    comment += TEXT(" ");

    size_t comment_length = comment.GetLength ();
    size_t selectionStart = Sci (SCI_GETSELECTIONSTART, 0, 0);
    size_t selectionEnd = Sci (SCI_GETSELECTIONEND, 0, 0);
    size_t caretPosition = Sci (SCI_GETCURRENTPOS, 0, 0);
    // checking if caret is located in _beginning_ of selected block
    bool move_caret = caretPosition < selectionEnd;
    int selStartLine = Sci (SCI_LINEFROMPOSITION, selectionStart, 0);
    int selEndLine = Sci (SCI_LINEFROMPOSITION, selectionEnd, 0);
    int lines = selEndLine - selStartLine;
    size_t firstSelLineStart = Sci (SCI_POSITIONFROMLINE, selStartLine, 0);
    // "caret return" is part of the last selected line
    if ((lines > 0) && (selectionEnd == static_cast<size_t>(Sci (SCI_POSITIONFROMLINE, selEndLine, 0))))
		selEndLine--;
	//--FLS: count lines which were un-commented to decide if undoStreamComment() shall be called.
	int nUncomments = 0;
    Sci (SCI_BEGINUNDOACTION, 0, 0);

    for (int i = selStartLine; i <= selEndLine; ++i)
	{
		int lineStart = Sci (SCI_POSITIONFROMLINE, i, 0);
        int lineIndent = lineStart;
        int lineEnd = Sci (SCI_GETLINEENDPOSITION, i, 0);

		size_t linebufferSize = lineEnd - lineStart + 2;
		char* linebuf = new char[linebufferSize];

        lineIndent = Sci (SCI_GETLINEINDENTPOSITION, i, 0);
		GetRange (lineIndent, lineEnd, linebuf);

        CString linebufStr = linebuf;
		delete [] linebuf;

        // empty lines are not commented
        if (linebufStr.GetLength () < 1)
			continue;

   		if (currCommentMode != eComment)   {
			//--FLS: In order to do get case insensitive comparison use strnicmp() instead case-sensitive comparison.
			//      Case insensitive comparison is needed e.g. for "REM" and "rem" in Batchfiles.
			//if (linebufStr.substr(0, comment_length - 1) == comment.substr(0, comment_length - 1))
			if (linebufStr.Left (2) == comment.Left (2))   {
				int len = linebufStr.Left (3) == comment.Left (3) ? comment_length : comment_length - 1;
				Sci (SCI_SETSEL, lineIndent, lineIndent + len);
				Sci (SCI_REPLACESEL, 0, (WPARAM)"");
				if (i == selStartLine) // is this the first selected line?
					selectionStart -= len;
				selectionEnd -= len; // every iteration
				++nUncomments;
				continue;
			}
		}
		if ((currCommentMode == eToggle) || (currCommentMode == eComment))   {
			if (i == selStartLine) // is this the first selected line?
				selectionStart += comment_length;
			selectionEnd += comment_length; // every iteration
			Sci (SCI_INSERTTEXT, lineIndent, (LPARAM) (const char *) comment);
		}
     }
    // after uncommenting selection may promote itself to the lines
    // before the first initially selected line;
    // another problem - if only comment symbol was selected;
    if (selectionStart < firstSelLineStart)
	{
        if (selectionStart >= selectionEnd - (comment_length - 1))
			selectionEnd = firstSelLineStart;
        selectionStart = firstSelLineStart;
    }
    if (move_caret)  {
        // moving caret to the beginning of selected block
        Sci (SCI_GOTOPOS, selectionEnd, 0);
        Sci (SCI_SETCURRENTPOS, selectionStart, 0);
    }
	else	{
        Sci (SCI_SETSEL, selectionStart, selectionEnd);
    }
    Sci (SCI_ENDUNDOACTION, 0, 0);
    return true;
}

/*
** gotomatchbrace :
*/
void CLuaEditor::gotobracematch (int nMode)
{
	int braceAtCaret = -1;
	int braceOpposite = -1;
	findMatchingBracePos(braceAtCaret, braceOpposite);

	if (braceOpposite != -1)   {
		if(nMode == 0)
			Sci (SCI_GOTOPOS, braceOpposite, 0);
		else
			Sci (SCI_SETSEL, min(braceAtCaret, braceOpposite), max(braceAtCaret, braceOpposite) + 1);
	}
}

/*
** SetFoldersStyles :
*/
void CLuaEditor::SetFoldersStyles ()
{
	defineMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_CIRCLEMINUS, RGB (255, 255, 255), RGB(128, 128, 128), RGB (255, 255, 255));
	defineMarker(SC_MARKNUM_FOLDER, SC_MARK_CIRCLEPLUS, RGB (255, 255, 255), RGB(128, 128, 128), RGB (255, 255, 255));
	defineMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE, RGB (255, 255, 255), RGB(128, 128, 128), RGB (255, 255, 255));
	defineMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE, RGB (255, 255, 255), RGB(128, 128, 128), RGB (255, 255, 255));
	defineMarker(SC_MARKNUM_FOLDEREND, SC_MARK_CIRCLEPLUSCONNECTED, RGB (255, 255, 255), RGB(128, 128, 128), RGB (255, 255, 255));
	defineMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_CIRCLEMINUSCONNECTED, RGB (255, 255, 255), RGB(128, 128, 128), RGB (255, 255, 255));
	defineMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE, RGB (255, 255, 255), RGB(128, 128, 128), RGB (255, 255, 255));
}

/*
** defineMarker :
*/
void CLuaEditor::defineMarker (int marker, int markerType, COLORREF fore, COLORREF back, COLORREF foreActive) 
{
    Sci (SCI_MARKERDEFINE, marker, markerType);
    Sci (SCI_MARKERSETFORE, marker, fore);
    Sci (SCI_MARKERSETBACK, marker, back);
	Sci (SCI_MARKERSETBACKSELECTED, marker, foreActive);
};

/*
** marginClick :
*/
void CLuaEditor::marginClick(int position, int modifiers)
{
	int lineClick = Sci (SCI_LINEFROMPOSITION, position, 0);
	int levelClick = Sci (SCI_GETFOLDLEVEL, lineClick, 0);
	if (levelClick & SC_FOLDLEVELHEADERFLAG)
    {
		if (modifiers & SCMOD_SHIFT)
        {
			// Ensure all children visible
			Sci (SCI_SETFOLDEXPANDED, lineClick, 1);
			expand (lineClick, true, true, 100, levelClick);
		}
        else if (modifiers & SCMOD_CTRL) 
        {
			if (isFolded(lineClick)) 
            {
				// Contract this line and all children
				Sci (SCI_SETFOLDEXPANDED, lineClick, 0);
				expand(lineClick, false, true, 0, levelClick);
			} 
            else 
            {
				// Expand this line and all children
				Sci (SCI_SETFOLDEXPANDED, lineClick, 1);
				expand(lineClick, true, true, 100, levelClick);
			}
		} 
        else 
        {
			// Toggle this line
			bool mode = isFolded(lineClick);
			fold(lineClick, !mode);
			runMarkers(true, lineClick, true, false);
		}
	}
}

/*
** expand :
*/
void CLuaEditor::expand (int &line, bool doExpand, bool force, int visLevels, int level)
{
	int lineMaxSubord = Sci (SCI_GETLASTCHILD, line, level & SC_FOLDLEVELNUMBERMASK);
	++line;
	while (line <= lineMaxSubord)
    {
		if (force) 
        {
			if (visLevels > 0)
				Sci (SCI_SHOWLINES, line, line);
			else
				Sci (SCI_HIDELINES, line, line);
		} 
        else 
        {
			if (doExpand)
				Sci (SCI_SHOWLINES, line, line);
		}
		int levelLine = level;
		if (levelLine == -1)
			levelLine = Sci (SCI_GETFOLDLEVEL, line, 0);
		if (levelLine & SC_FOLDLEVELHEADERFLAG)
        {
			if (force) 
            {
				if (visLevels > 1)
					Sci (SCI_SETFOLDEXPANDED, line, 1);
				else
					Sci (SCI_SETFOLDEXPANDED, line, 0);
				expand(line, doExpand, force, visLevels - 1);
			} 
            else
            {
				if (doExpand)
                {
					if (!isFolded(line))
						Sci (SCI_SETFOLDEXPANDED, line, 1);
					expand(line, true, force, visLevels - 1);
				} 
                else 
                {
					expand(line, false, force, visLevels - 1);
				}
			}
		}
        else
        {
			++line;
		}
	}
	runMarkers(true, 0, true, false);
}

/*
** isFolded :
*/
bool CLuaEditor::isFolded(int line)
{
	return (Sci (SCI_GETFOLDEXPANDED, line, 0) != 0);
};

/*
** fold :
*/
void CLuaEditor::fold(int line, bool mode)
{
    int endStyled = Sci (SCI_GETENDSTYLED, 0, 0);
    int len = Sci (SCI_GETTEXTLENGTH, 0, 0);

    if (endStyled < len)
        Sci (SCI_COLOURISE, 0, -1);

	int headerLine;
	int level = Sci (SCI_GETFOLDLEVEL, line);
		
	if (level & SC_FOLDLEVELHEADERFLAG)
		headerLine = line;
	else
	{
		headerLine = Sci (SCI_GETFOLDPARENT, line, 0);
		if (headerLine == -1)
			return;
	}

	if (isFolded(headerLine) != mode)
	{
		Sci (SCI_TOGGLEFOLD, headerLine);

		SCNotification scnN;
		scnN.nmhdr.code = SCN_FOLDINGSTATECHANGED;
		scnN.nmhdr.hwndFrom = this->GetSafeHwnd ();
		scnN.nmhdr.idFrom = 0;
		scnN.line = headerLine;
		scnN.foldLevelNow = isFolded(headerLine)?1:0; //folded:1, unfolded:0

		::SendMessage(m_hwndParent, WM_NOTIFY, 0, (LPARAM)&scnN);
	}
}

/*
** runMarkers :
*/
void CLuaEditor::runMarkers(bool doHide, int searchStart, bool endOfDoc, bool doDelete)
{
	int maxLines = Sci (SCI_GETLINECOUNT, 0, 0);
	if (doHide) {
		int startHiding = searchStart;
		bool isInSection = false;
		for (int i = searchStart; i < maxLines; ++i) {
			int state = Sci (SCI_MARKERGET, i, 0);
			if ( ((state & (1 << MARK_HIDELINESEND)) != 0) ) {
				if (isInSection) {
					Sci (SCI_HIDELINES, startHiding, i-1);
					if (!endOfDoc) {
						return;	//done, only single section requested
					}	//otherwise keep going
				}
				isInSection = false;
			}
			if ( ((state & (1 << MARK_HIDELINESBEGIN)) != 0) ) {
				isInSection = true;
				startHiding = i+1;
			}

		}
	} else {
		int startShowing = searchStart;
		bool isInSection = false;
		for(int i = searchStart; i < maxLines; ++i) {
			int state = Sci (SCI_MARKERGET, i);
			if ( ((state & (1 << MARK_HIDELINESEND)) != 0) ) {
				if (doDelete)
					Sci (SCI_MARKERDELETE, i, MARK_HIDELINESEND);
				 else if (isInSection) {
					if (startShowing >= i) {	//because of fold skipping, we passed the close tag. In that case we cant do anything
						if (!endOfDoc) {
							return;
						} else {
							continue;
						}
					}
					Sci (SCI_SHOWLINES, startShowing, i-1);
					if (!endOfDoc) {
						return;	//done, only single section requested
					}	//otherwise keep going
					isInSection = false;
				}
			}
			if ( ((state & (1 << MARK_HIDELINESBEGIN)) != 0) ) {
				if (doDelete)
					Sci (SCI_MARKERDELETE, i, MARK_HIDELINESBEGIN);
				else {
					isInSection = true;
					startShowing = i+1;
				}
			}

			int levelLine = Sci(SCI_GETFOLDLEVEL, i, 0);
			if (levelLine & SC_FOLDLEVELHEADERFLAG) {	//fold section. Dont show lines if fold is closed
				if (isInSection && !isFolded(i)) {
					Sci (SCI_SHOWLINES, startShowing, i);
					startShowing = Sci (SCI_GETLASTCHILD, i, (levelLine & SC_FOLDLEVELNUMBERMASK));
				}
			}
		}
	}
}

/*
** GetLineFromPosition :
*/
int CLuaEditor::GetLineFromPosition (int nPosition)
{
	return Sci (SCI_LINEFROMPOSITION, nPosition, 0);
}

/*
** foldAll :
*/
void CLuaEditor::foldAll(bool mode)
{
	int maxLine = Sci (SCI_GETLINECOUNT, 0, 0);

	for (int line = 0; line < maxLine; ++line) 
	{
		int level = Sci (SCI_GETFOLDLEVEL, line, 0);
		if (level & SC_FOLDLEVELHEADERFLAG) 
			if (isFolded(line) != mode)
				fold(line, mode);
	}
}

/*
** GetCaretFunction :
*/
CString CLuaEditor::GetCaretFunction ()
{
	CString strFunction, strLine;
	int nState, nName, n, nB = 1;
	char *pName = new char [2048];
	bool bOk;
	/* prendiamo la linea corrente */
	int nLine = GetCurrentLine ();
	while (nLine > 0)   {
		strLine = GetLine (nLine);
		if ((n = strLine.Find ("function")) != -1)   {
			/* trovata */
			nB = 0;
			n += CString ("function").GetLength ();
			nName = nState = 0;
			bOk = false;
			while (n < strLine.GetLength () && !bOk)   {
				switch (nState)   {
					case 0 :
						/* andiamo a cercare l'inizio del nome della function */
						if (strLine[n] == ' ' || strLine[n] == '\t')
							break;
						else
							nState = 1;
					case 1 :
						/* andiamo a cercare la fine del nome */
						if (strLine[n] == ' ' || strLine[n] == '\t' || strLine[n] == '(')   {
							pName[nName] = '\0';
							bOk = true;
						}
						else
							pName[nName++] = strLine[n];
						break;
				}
				n++;
			}
			if (bOk)   {
				/* abbiamo trovato una function, memorizziamola nel browser */
				strFunction = pName;
				break;
			}
		}
		nLine--;
	}
	delete pName;
	return strFunction;
}

/*
** GetCaretMatrix :
*/
CString CLuaEditor::GetCaretMatrix (CString strSymbol)
{
	CString strLine, strRet;
	int nLine = GetCurrentLine ();
	int pos   = GetCurrentPosition ();
	int start, nIndex = 0, nState = 0, n;
	char pName[1024];
	bool bFindVariableName = false, bOk = false;

	if (nLine > 0)   {
		strLine = GetLine (nLine);
		pos = Sci(SCI_WORDSTARTPOSITION, pos, TRUE);
		start = Sci(SCI_POSITIONFROMLINE, nLine-1);
		while (pos > start)   {
			TCHAR ch = Sci (SCI_GETCHARAT, pos, 0);
			switch (nState)   {
				case 0 :
					if (ch == '.')
						nState = 1;
					else
						if (ch == ':')   {
							bFindVariableName = true;
							nLine--;
							pos = 0;
						}
					break;
				case 1 :
					if (ch == ']')
						nState = 2;
					break;
				case 2:
					if (ch == '[')
						nState = 3;
					break;
				case 3 :
					if (ch != ' ' && ch != 't')   {
						nState = 4;
						continue;
					}
					break;
				case 4 :
					if (iswordcharforsel (ch))
						pName[nIndex++] = ch;
					else   {
						pName[nIndex] = '\0';
						Reverse (pName);
						strRet = pName;
						pos = 0;
						break;
					}
			}
			pos--;
		}
		if (bFindVariableName)   {
			while (nLine > 0)   {
				strLine = GetLine (nLine);
				if ((n = strLine.Find ("global")) != -1)   {
					/* trovata */
					n += CString ("global").GetLength ();
					nState = 0;
					bOk = false;
					while (n < strLine.GetLength () && !bOk)   {
						switch (nState)   {
							case 0 :
								/* andiamo a cercare il nome della globl */
								if (strLine[n] == ' ' || strLine[n] == '\t')
									break;
								else
									nState = 1;
							case 1 :
								/* andiamo a cercare la fine del nome */
								if (strLine[n] == ' ' || strLine[n] == '\t' || strLine[n] == '[')   {
									pName[nIndex] = '\0';
									bOk = true;
								}
								else
									pName[nIndex++] = strLine[n];
								break;
						}
						n++;
					}
					if (bOk)   {
						/* abbiamo trovato una function, memorizziamola nel browser */
						strRet = pName;
						break;
					}
				}
				nLine--;
			}
		}
	}	
	return strRet;
}

/*
** Reverse :
*/
void CLuaEditor::Reverse (char* str)
{
	char szBuffer[1024];
	int nLenght = strlen (str);
	for (int i = 0, n = nLenght - 1; i<nLenght; i++, n--)
		szBuffer[n] = str[i];
	memcpy (str, szBuffer, nLenght);
}

/*
** SelectWord :
*/
void CLuaEditor::SelectWord (CString strWord, int nLine, int nItem)
{
	int nStartPosition, nStartItem, i = 0, n = 0;
	CString strLine = GetLine (nLine);

	if (nItem > 0)   {
		while (i < nItem)   {
			n = strLine.Find (strWord, n);
			if (n == -1)
				break;
			else   {
				nStartItem = n;
				n += strWord.GetLength ();
			}
			i++;
		}
		if (n != -1)   {	
			nStartPosition = Sci(SCI_POSITIONFROMLINE, nLine-1);
			nStartPosition += nStartItem;
			GotoPosition (nStartPosition);
			SelectCurrentWord ();
		}
	}
}

/*
** SelectSymbol :
*/
void CLuaEditor::SelectSymbol (CString strSymbol, int nLine, int nItem)
{
	int nStartPosition, nStartItem, i = 0, n = 0;
	CString strLine = GetLine (nLine);

	if (nItem > 0)   {
		while (i < nItem)   {
			n = FindValidItem (strLine, strSymbol, n);
			if (n == -1)
				break;
			else   {
				nStartItem = n;
				n += strSymbol.GetLength ();
			}
			i++;
		}
		if (n != -1)   {	
			nStartPosition = Sci(SCI_POSITIONFROMLINE, nLine-1);
			nStartPosition += nStartItem;
			GotoPosition (nStartPosition);
			SelectCurrentSymbol ();
		}
	}
}

/*
** OnDestroyHelpOptionsWnd :
*/
LRESULT CLuaEditor::OnDestroyHelpOptionsWnd (WPARAM wParam, LPARAM lParam)
{
	_delete (m_pAHO);
	return 0;
}

/*
** ShowLibraries :
*/
void CLuaEditor::ShowLibraries ()
{
	CRect rc;
	/* riempiamo la lista delle librerie */
	FillOptions ();
	/* vediamo dove posizionare il listview */
	GetListPosition (rc, false);
	/* riempiamo e visualizziamo il listctrl */
	FillListCtrl (rc, 0, 0);
}

/*
** ShowFunctions :
*/
void CLuaEditor::ShowFunctions (CString strLibrary)
{
	CRect rc;
	/* vediamo dove posizionare il listview */
	GetListPosition (rc, false);
	/* riempiamo e visualizziamo il listctrl */
	FillListCtrl (rc, 1, 0);
}

/*
** ShowEnums :
*/
void CLuaEditor::ShowEnums ()
{
	CRect rc;
	/* riempiamo la lista delle librerie */
	FillEnums ();
	/* vediamo dove posizionare il listview */
	GetListPosition (rc, true);
	/* riempiamo e visualizziamo il listctrl */
	FillListCtrl (rc, 2, 1);
}

/*
** ShowEnums :
*/
void CLuaEditor::ShowEnumsValues (CString strEnum)
{
	CRect rc;
	/* vediamo dove posizionare il listview */
	GetListPosition (rc, true);
	/* riempiamo e visualizziamo il listctrl */
	FillListCtrl (rc, 3, 1);
}

/*
** GetListPosition :
*/
void CLuaEditor::GetListPosition (CRect& rc, bool bOneColumn)
{
	CRect rcClient;
	int nSizeX = bOneColumn ? 180 : 500;
	int nSizeY = m_slOptions.GetCount () >= 10 ? 192 : m_slOptions.GetCount () * 19 + 2;
	GetClientRect (&rcClient);
	int nPos = Sci(SCI_GETCURRENTPOS);
	int nX = Sci (SCI_POINTXFROMPOSITION, 0, nPos);
	int nY = Sci (SCI_POINTYFROMPOSITION, 0, nPos);
	int nLineHeight = Sci (SCI_TEXTHEIGHT, GetLineFromPosition (nPos));
	if (nY < rcClient.bottom / 2)
		rc.top = nY + nLineHeight + 2;
	else
		rc.top = nY - nSizeY - 2;
	rc.left = nX;
	rc.bottom = rc.top + nSizeY;
	rc.right = rc.left + nSizeX;
}

/*
** FillOptions :
*/
void CLuaEditor::FillOptions ()
{
	CProject *pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		m_slOptions.RemoveAll ();
		m_slDescriptions.RemoveAll ();
		CSyntaxContainer& sc = pPrj->GetSyntaxContainer ();
		sc.GetLibrariesList (m_slOptions, m_slDescriptions);
	}
}

/*
** FillFunctions :
*/
void CLuaEditor::FillFunctions (CString strLibrary)
{
	CProject *pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		m_slOptions.RemoveAll ();
		m_slDescriptions.RemoveAll ();
		CSyntaxContainer& sc = pPrj->GetSyntaxContainer ();
		sc.GetFunctionsList (strLibrary, m_slOptions, m_slDescriptions);
	}
}

/*
** FillEnums :
*/
void CLuaEditor::FillEnums ()
{
	CProject *pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		m_slOptions.RemoveAll ();
		CSyntaxContainer& sc = pPrj->GetSyntaxContainer ();
		sc.GetEnumList  (m_slOptions);
	}
}

/*
** FillEnumsValues :
*/
void CLuaEditor::FillEnumsValues (CString strEnum)
{
	CProject *pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		m_slOptions.RemoveAll ();
		CSyntaxContainer& sc = pPrj->GetSyntaxContainer ();
		sc.GetEnumValues  (strEnum, m_slOptions, m_slDescriptions);
	}
}

/*
** FillListCtrl :
*/ 
void CLuaEditor::FillListCtrl (CRect& rc, int nType, int nMode)
{
	_delete (m_pAHO);
	m_pAHO = new CAdamoHelpOptions (nMode);
	m_pAHO->Create (WS_CHILD|WS_VISIBLE|WS_BORDER, rc, this, 1001);
	m_pAHO->SetCtrl ();
	m_pAHO->SetData (&m_slOptions, &m_slDescriptions, nType);
	m_pAHO->LoadData ();
	m_pAHO->SelectLastItem ();
}

/*
** AutoSelectHelpOptions :
*/
void CLuaEditor::AutoSelectHelpOptions ()
{
	if (m_pAHO)   {
		CString strCaretText = GetCaretText ();
		m_pAHO->SelectWord (strCaretText);
	}
}

/*
** PreTranslateMessage :
*/
BOOL CLuaEditor::PreTranslateMessage(MSG* pMsg) 
{
    if (pMsg->message==WM_KEYDOWN)   {
        switch (pMsg->wParam)   {
			case VK_ESCAPE :
				DestroyHelpOptionsWindow ();
				break;
			case VK_RETURN :
			case VK_TAB :
				if (CompleteHelpOptionWindow ())
					return TRUE;
				break;
			case VK_UP :
				if (MoveHelpOptionWindowUp ())
					return TRUE;
			case VK_DOWN :
				if (MoveHelpOptionWindowDown ())
					return TRUE;
		}
	}
	else
    if (pMsg->message==WM_LBUTTONDOWN)   {
		if (pMsg->hwnd == GetSafeHwnd ())
			DestroyHelpOptionsWindow ();
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/*
** DestroyHelpOptionsWindow :
*/
void CLuaEditor::DestroyHelpOptionsWindow ()
{
	if (m_pAHO)
		_delete (m_pAHO);
}

/*
** CompleteHelpOptionWindow :
*/
bool CLuaEditor::CompleteHelpOptionWindow ()
{
	bool b = false;

	if (m_pAHO)   {
		CString str = m_pAHO->GetSelectedOption ();
		if (str != "")   {
			LONG l = GetCurrentPosition ();
			CString strCaretText = GetCaretText ();
			Sci (SCI_DELETERANGE, l - strCaretText.GetLength (), strCaretText.GetLength ());
			AddText (str);
			DestroyHelpOptionsWindow ();
			b = true;
		}
	}
	return b;
}

/*
** MoveHelpOptionWindowUp :
*/
bool CLuaEditor::MoveHelpOptionWindowUp ()
{
	bool b = false;

	if (m_pAHO)   {
		m_pAHO->MoveSelectedOptionUp ();
		b = true;
	}
	return b;
}

/*
** MoveHelpOptionWindowDown :
*/
bool CLuaEditor::MoveHelpOptionWindowDown ()
{
	bool b = false;

	if (m_pAHO)   {
		m_pAHO->MoveSelectedOptionDown ();
		b = true;
	}
	return b;
}

/*
** GetCallTipText :
*/
bool CLuaEditor::GetCallTipText (const char* words, CString &strCallTipText)
{
	CString str (words);
	CString strParameter;
	int nPunto, i = 0;
	bool bFound = false;

	CProject *pPrj = GETWS ()->GetActiveProject ();
	if (pPrj)   {
		m_slParameters.RemoveAll ();
		nPunto = str.Find ('.');
		if (nPunto != -1)   {
			CString strLibrary = str.Left (nPunto);
			CString strFunction = str.Right (str.GetLength () - nPunto - 1);
			CSyntaxContainer& sc = pPrj->GetSyntaxContainer ();
			if (sc.ExistFunction (strLibrary, strFunction))   {
				sc.GetParametersList (strLibrary, strFunction, m_slParameters, m_slDescriptions);
				strCallTipText.Format ("%s (", str);
				for (POSITION pos = m_slParameters.GetHeadPosition (); pos; i++)   {
					strParameter = m_slParameters.GetNext (pos);
					strCallTipText += strParameter;
					if (i < m_slParameters.GetCount () - 1)
						strCallTipText += ", ";
				}
				strCallTipText += ")";
				bFound = true;
			}
		}
		else   {
			CFunctionsContainer& fc = pPrj->GetFunctionsContainer ();
			CString strModule, strFunction (words);
			if (fc.ExistFunction (strFunction, strModule))   {
				fc.GetParametersList (strModule, strFunction, m_slParameters, m_slDescriptions);
				strCallTipText.Format ("%s (", str);
				for (POSITION pos = m_slParameters.GetHeadPosition (); pos; i++)   {
					strParameter = m_slParameters.GetNext (pos);
					strCallTipText += strParameter;
					if (i < m_slParameters.GetCount () - 1)
						strCallTipText += ", ";
				}
				strCallTipText += ")";
				bFound = true;
			}
		}
	}
	return bFound;
}

/*
** ShowEnums :
*/
void CLuaEditor::OnEditShowEnums ()
{
	ShowEnums ();
}

/*
** ShowEnums :
*/
void CLuaEditor::OnEditShowParams ()
{
	ShowParameter ();
}

/*
** ShowParameter :
*/
void CLuaEditor::ShowParameter ()
{
	CString str (currentCallTipWord.c_str ());
	int nPunto;
	CProject *pPrj = GETWS ()->GetActiveProject ();
	char linebuf[1000];

	if (pPrj)   {
		if (Sci(SCI_CALLTIPACTIVE))   {
			GetLine(linebuf, sizeof(linebuf));
			int current = GetCaretInLine();
			int braces = 0;
			int commas = 0;
			for (int i = startCalltipWord; i < current; i++) {
				if (linebuf[i] == '(')
					braces++;
				else if (linebuf[i] == ')' && braces > 0)
					braces--;
				else if (braces == 1 && IsCallTipSeparator(linebuf[i]))
					commas++;
			}
			m_slParameters.RemoveAll ();
			m_slDescriptions.RemoveAll ();
			nPunto = str.Find ('.');
			if (nPunto != -1)   {
				CString strLibrary = str.Left (nPunto);
				CString strFunction = str.Right (str.GetLength () - nPunto - 1);
				CString strParameter, strEnum;
				int i = 0;
				CSyntaxContainer& sc = pPrj->GetSyntaxContainer ();
				/* prendiamo la lista dei parametri */
				if (sc.ExistFunction (strLibrary, strFunction))   {
					sc.GetParametersList (strLibrary, strFunction, m_slParameters, m_slDescriptions);
					/* prendiamo il parametro che ci interessa */
					for (POSITION pos = m_slParameters.GetHeadPosition (); pos; i++)   {
						strParameter = m_slParameters.GetNext (pos);
						if (i == commas)
							break;
					}
					/* ora che abbiamo il parametro interessato, prendiamo l'eventuale enum */
					sc.GetParametersEnum (strLibrary, strFunction, strParameter, strEnum);
					if (strEnum != "")   {
						AddText (strEnum + ".");
						FillEnumsValues (strEnum);
						if (m_slOptions.GetCount () != 0)
							ShowEnumsValues (str);
					}
				}
			}
		}
	}
}

/*
** ShowEnumInCallTip :
*/
void CLuaEditor::ShowEnumInCallTip ()
{
	char linebuf[1000];
	GetLine(linebuf, sizeof(linebuf));
	int current = GetCaretInLine();

	int startword = current;

	while ((startword > 0) &&
	        (wordCharacters.contains(linebuf[startword - 1]) ||
	         autoCompleteStartCharacters.contains(linebuf[startword - 1])))
		startword--;

	linebuf[current] = '\0';
	const char *root = linebuf + startword;
	int rootlen = current - startword;
	CString str (root);
	str =str.Left (str.GetLength ()-1);
	FillEnumsValues (str);
	if (m_slOptions.GetCount () != 0)
		ShowEnumsValues (str);
}

/*
** FindValidItem :
*/
int CLuaEditor::FindValidItem (CString line, CString str, int n)
{
	int s, q;
	bool bIsValid = false;

	while (n < line.GetLength ())    {
		s = line.Find (str, n);
		if (s != -1)   {
			q = s + str.GetLength ();
			if (s == 0 || !iswordcharforsel (line[s-1]))   {
				if (line[q] == '\0' || !iswordcharforsel (line[q]))   {
					bIsValid = true;
					break;
				}
				else
					n = q;
			}
			else
				n = q;
		}
		else
			break;
	}
	return bIsValid ? s : -1;	
}

/*
** SetSkin :
*/
void CLuaEditor::SetSkin ()
{
	CString strSkin = ((CIdeApp *) AfxGetApp ())->GetAdamoOptions ()->GetSkin ();
	for (int i = 0; i<sizeof (achSkinStyles) / sizeof (char*); i++)   {
		if (strSkin == achSkinStyles[i])   {
			switch (i)   {
				case 0 :
					SetBckSciStyle (RGB(0xff,0xff,0xff));
					break;
				case 1 :
					SetBckSciStyle (RGB(0xf5,0xf5,0xf5));
					break;
				case 2 :
					SetBckSciStyle (RGB(0x00,0x00,0x00));
					break;
				case 3 :
					SetBckSciStyle (RGB(0xff,0xff,0xff));
					break;
				case 4 :
					SetBckSciStyle (RGB(0xf5,0xf5,0xf5));
					break;
				case 5 :
					SetBckSciStyle (RGB(0x00,0x00,0x00));
					break;
			}
			break;
		}
	}
}

/*
** SetBckSciStyle :
*/
void CLuaEditor::SetBckSciStyle (COLORREF rgb)
{
   Sci(SCI_STYLESETBACK, STYLE_DEFAULT, rgb);
   Sci(SCI_STYLESETBACK,0, rgb);
   Sci(SCI_STYLESETBACK,1, rgb);
   Sci(SCI_STYLESETBACK,2, rgb);
   Sci(SCI_STYLESETBACK,3, rgb);
   Sci(SCI_STYLESETBACK,4, rgb);
   Sci(SCI_STYLESETBACK,5, rgb);
   Sci(SCI_STYLESETBACK,6, rgb);
   Sci(SCI_STYLESETBACK,7, rgb);
   Sci(SCI_STYLESETBACK,10, rgb);
   Sci(SCI_STYLESETBACK,11, rgb);
   Sci(SCI_STYLESETBACK,13, rgb);
}
