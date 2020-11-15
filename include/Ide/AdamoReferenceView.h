#pragma once
#include "scintillabar.h"

class CAdamoReferenceView :	public CScintillaBar
{
	public:
		CAdamoReferenceView(void);
		~CAdamoReferenceView(void);
	    void Init(CWnd *pWnd, int nId);
	    void Clear();
    	virtual int OnSci(CWnd* pView, SCNotification* pNotify);
        CScintillaView *GetSciView()    { return m_pSciView; }
		void Add (const char* szPath, int nLine, int nItem);
		void WritePrologue ();
		void WritePrologue (CString strSymbol, int nRefCount);
	private :
		int GotoFileLine (CString str);
    protected:
        CScintillaView *m_pSciView;
		CString m_strSymbol;
};
