/*
** AdamoFindInFilesView
*/

#ifndef __ADAMO_FIND_IN_FILES_VIEW__
#define __ADAMO_FIND_IN_FILES_VIEW__

struct SCNotification;

#include "ScintillaBar.h"

class CAdamoFindInFilesView : public CScintillaBar
{
    public:
	    void Init(CWnd *pWnd, int nId);
        CAdamoFindInFilesView () : m_pSciView(NULL), m_nCurErrorLine (0) {}
        virtual ~CAdamoFindInFilesView() {}
	    void Add(const char* szPath, const char* szLine);
	    void Clear();
        void WriteCanceled ();
        void WriteOccurences (int n);
        CScintillaView *GetSciView()    { return m_pSciView; }
    	virtual int OnSci(CWnd* pView, SCNotification* pNotify);
        void GotoNextFFLine ();
        void GotoPrevFFLine ();
    protected:
        CScintillaView *m_pSciView;
    private :
        int  GotoFileLine (CString str);
        bool IsValidFFLine (CString str);
    private :
        int m_nCurErrorLine;
};

#endif
