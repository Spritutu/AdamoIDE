/*
**
*/

#ifndef __ADAMO_TEMPLATES__
#define __ADAMO_TEMPLATES__

/* onclude files */
#include "stdafx.h"
#include "ide2.h"
#include "workspace.h"
#include "LuaFrame.h"
#include "LuaDoc.h"
#include "LuaView.h"
#include "ConfigDoc.h"
#include "ConfigFrame.h"
#include "AdamoMDDoc.h"
#include "AdamoMDFrame.h"
#include "AdamoMDIOView.h"
#include "ConfTreeView.h"
#include "AdamoDbgIOView.h"
#include "HTMLDoc.h"
#include "HTMLFrame.h"
#include "HTMLView.h"
#include "AdamoSEDoc.h"
#include "AdamoSEFrame.h"
#include "AdamoSEView.h"
#include "AdamoRSDoc.h"
#include "AdamoRSFrame.h"
#include "AdamoRSView.h"
#include "AdamoMDView.h"
#include "AdamoDGView.h"
#include "AdamoDGDocument.h"
#include "AdamoDGFrame.h"

/* templates del programma */
class CAdamoTemplate : public CMultiDocTemplate
{
    protected :
        CView* m_pView;
        CFrameWnd *m_pFrame;
    public :
        CAdamoTemplate (UINT nId,
                        CRuntimeClass* pDoc,
                        CRuntimeClass* pFrame,
                        CRuntimeClass* pView) : CMultiDocTemplate (nId, 
                                                                   pDoc,
                                                                   pFrame,
                                                                   pView), m_pView (NULL), m_pFrame (NULL) {}
        virtual enTemplateTypes KindOf ()   { return enNoTemplate; }
        void RemoveDocument (CDocument* pDoc) { CMultiDocTemplate::RemoveDocument (pDoc); }
        CDocument* GetDocument (CString strPathDoc)   {
            POSITION pos;
            if (strPathDoc=="")   {
                pos=GetFirstDocPosition ();
                if (pos)
                    return GetNextDoc (pos);
            }
            else
                for (POSITION pos=GetFirstDocPosition (); pos;)   {
                    CDocument* pDoc=GetNextDoc (pos);
                    if (pDoc->GetPathName ()==strPathDoc)
                        return pDoc;    
                }
            return NULL;
        }
        CView* GetFirstView (CDocument* pDoc)   {
            CView* pView=NULL;
            POSITION pos=pDoc->GetFirstViewPosition ();
            if (pos)
                pView=pDoc->GetNextView (pos);
            return pView;
        }
    virtual void OnOpen  ()                     {  }
    virtual void OnClose ()                     {  }
    virtual void OnTimer ()                     {  }
    virtual void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw) {  }
    virtual void SetActiveView  (CView* pView)       { m_pView=pView; }
    virtual void SetActiveFrame (CFrameWnd* pFrame)  { m_pFrame=pFrame; }
    virtual CView* GetActiveView ()             { return m_pView; }
};

class CConfTemplate : public CAdamoTemplate
{
    public :
        CConfTemplate () : CAdamoTemplate (IDR_MENU_CONFIG, 
                                           RUNTIME_CLASS(CConfigDoc),
                                           RUNTIME_CLASS(CConfigFrame),
                                           RUNTIME_CLASS(CConfTreeView)) {}
        enTemplateTypes KindOf ()   { return enConfTemplate; }
        void OnOpen  ()   {
            CDocument *pDoc=OpenDocumentFile (NULL);
            if (pDoc)   {
                CView* pView=GetFirstView (pDoc);
				if (pView && pView->GetSafeHwnd ())
                    ((CConfTreeView*)pView)->Activate ();
            }
        }
        void OnClose ()   { 
            CDocument* pDoc=GetDocument ("");
            if (pDoc)
                pDoc->OnCloseDocument ();
        }
        void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)   {
            if (m_pFrame)
                ((CConfigFrame*)m_pFrame)->OnChangePsw (oldPsw, newPsw);
        };
};

class CEditDebugTemplate : public CAdamoTemplate
{
    public :
        CEditDebugTemplate () : CAdamoTemplate (IDR_IDE2TYPE,
                                                RUNTIME_CLASS(CLuaDoc),
                                                RUNTIME_CLASS(CLuaFrame),
                                                RUNTIME_CLASS(CLuaView)) {}
        enTemplateTypes KindOf ()   { return enEditDebugTemplate; }
        int  GetDocumentCount ()    { return m_docList.GetCount (); }
        void OnOpen  ()   {
            if (m_pView)
                ((CLuaView*)m_pView)->Activate ();
            else   {
                CProject* pPrj=GETWS()->GetActiveProject();
                CString strFile = pPrj->GetProjectDir() + "\\" + pPrj->GetName() + ".ada";
                CDocument* pDoc=OpenDocumentFile (strFile);
                if (pDoc)   {
                    CView* pView=GetFirstView (pDoc);
                    if (pView)
                        ((CLuaView*)pView)->Activate ();
                }
            }
        }
        virtual void OnClose ()   {
            CLuaFrame::GetLuaFrame()->OnClose();
        }
        virtual void OnTimer ()   {
            CLuaFrame::GetLuaFrame()->OnTimer();
        }
        virtual void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)   {
            if (m_pFrame)
                ((CLuaFrame*)m_pFrame)->OnChangePsw (oldPsw, newPsw);
        };
};

class CManDiagTemplate : public CAdamoTemplate
{
    public :
        CManDiagTemplate () : CAdamoTemplate (IDR_MENU_MANDIAG,
                                              RUNTIME_CLASS(CAdamoMDDoc),
                                              RUNTIME_CLASS(CAdamoMDFrame),
                                              RUNTIME_CLASS(CAdamoMDView)) {}

        enTemplateTypes KindOf ()   { return enManDiagTemplate; }
        void OnOpen  ()   {
            if (m_pView)
                ((CAdamoMDView*)m_pView)->Activate ();
            else   {
                CDocument* pDoc=OpenDocumentFile (NULL);
                if (pDoc)   {
                    CView* pView=GetFirstView (pDoc);
                    if (pView && pView->GetSafeHwnd ())
                        ((CAdamoMDView*)pView)->Activate ();
                }
            }
        }
        void OnClose ()   {
            if (m_pView)
                ((CAdamoMDView*)m_pView)->OnClose ();
            CAdamoMDFrame::GetMDFrame()->OnClose();
        }
        virtual void OnTimer ()   {
            CAdamoMDFrame::GetMDFrame()->OnTimer();
        }
        virtual void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)   {
            if (m_pView)
                ((CAdamoMDView*)m_pView)->OnChangePsw (oldPsw, newPsw);
        };
};

class CHTMLTemplate : public CAdamoTemplate
{
    public :
        CHTMLTemplate () : CAdamoTemplate (IDR_MENU_MACHINE,
                                           RUNTIME_CLASS(CHTMLDoc),
                                           RUNTIME_CLASS(CHTMLFrame),
                                           RUNTIME_CLASS(CHTMLView)) {}
        enTemplateTypes KindOf ()   { return enHTMLTemplate; }
        void OnOpen  ()   {
            if (m_pView)
                ((CHTMLView*)m_pView)->Activate ();
            else   {
                CDocument* pDoc=OpenDocumentFile (NULL);
                if (pDoc)   {
                    CView* pView=GetFirstView (pDoc);
                    if (pView)
                        ((CHTMLView*)pView)->Activate ();
                }
            }
        }
};

class CShowErrorsTemplate : public CAdamoTemplate   {
    public :
        CShowErrorsTemplate () : CAdamoTemplate (IDR_MENU_SHOWERRORS,
                                                 RUNTIME_CLASS(CAdamoSEDoc),
                                                 RUNTIME_CLASS(CAdamoSEFrame),
                                                 RUNTIME_CLASS(CAdamoSEView)) {}
        enTemplateTypes KindOf ()   { return enShowErrorsTemplate; }
        void OnOpen  ()   {
            CDocument* pDoc;
            if (m_pView)   {
                ((CAdamoSEView*)m_pView)->Activate ();
                pDoc=GetDocument ("");
            }
            else   {
                pDoc=OpenDocumentFile (NULL);
                if (pDoc)   {
                    CView* pView=GetFirstView (pDoc);
                    if (pView)
                        ((CAdamoSEView*)pView)->Activate ();
                }
            }
        }
        void OnClose ()   {
            if (m_pView)
                ((CAdamoSEView*)m_pView)->OnCloseSEView ();
        }
};

class CDiagnosticTemplate : public CAdamoTemplate   {
    public :
        CDiagnosticTemplate () : CAdamoTemplate (IDR_MENU_DIAGNOSTIC,
                                                 RUNTIME_CLASS(CAdamoDGDocument),
                                                 RUNTIME_CLASS(CAdamoDGFrame),
                                                 RUNTIME_CLASS(CAdamoDGView)) {}
        enTemplateTypes KindOf ()   { return enDiagnosticTemplate; }
        void OnOpen  ()   {
            CDocument* pDoc;
            if (m_pView)   {
                ((CAdamoDGView*)m_pView)->Activate ();
                pDoc=GetDocument ("");
            }
            else   {
                pDoc=OpenDocumentFile (NULL);
                if (pDoc)   {
                    CView* pView=GetFirstView (pDoc);
                    if (pView)
                        ((CAdamoDGView*)pView)->Activate ();
                }
            }
        }
        void OnClose ()   {
            if (m_pView)
                ((CAdamoDGView*)m_pView)->OnCloseDGView ();
        }
        virtual void OnTimer ()                     {
            if (m_pView)
                ((CAdamoDGView*)m_pView)->OnAdamoTimer ();
        }
        virtual void OnChangePsw (ePswLevel oldPsw, ePswLevel newPsw)   {
            if (m_pView)
                ((CAdamoDGView*)m_pView)->OnChangePsw (oldPsw, newPsw);
        };
};

class CResourcesTemplate : public CAdamoTemplate   {
    public :
        CResourcesTemplate () : CAdamoTemplate (IDR_MENU_RESOURCES,
                                                RUNTIME_CLASS(CAdamoRSDoc),
                                                RUNTIME_CLASS(CAdamoRSFrame),
                                                RUNTIME_CLASS(CAdamoRSView)) {}
        enTemplateTypes KindOf ()   { return enResourcesTemplate; }
        void OnOpen  ()   {
            if (m_pView)
                ((CAdamoRSView*)m_pView)->Activate ();
            else   {
                CDocument* pDoc=OpenDocumentFile (NULL);
                if (pDoc)   {
                    CView* pView=GetFirstView (pDoc);
                    if (pView)
                        ((CAdamoRSView*)pView)->Activate ();
                }
            }
        }
        void OnClose ()   {
            if (m_pView)
                ((CAdamoRSView*)m_pView)->OnCloseRSView ();
        }
        virtual void OnTimer ()                     {
            if (m_pView)
                ((CAdamoRSView*)m_pView)->OnAdamoTimer ();
        }
};

#endif
