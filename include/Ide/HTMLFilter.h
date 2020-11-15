/*
**
*/

#ifndef __ADAMO_HTMLFILTER__
#define __ADAMO_HTMLFILTER__

/* include files */

#include "path.h"
#include "resourcefile.h"

/* namespace */
using namespace nsPath;

class CAdamoHTMLFilter   {
    public :
        CAdamoHTMLFilter (); 
        ~CAdamoHTMLFilter ();
        void Filter (CString &strHTML);
        void Clean ();
        void SetResourceFile (CResourceFile *pRS)   { m_rf=pRS; }
    private :
        CPath GetTemporaryImageFile ();
        CString GetIDImage (CString str);
        void ReplaceIDImage (CString& str, CPath path);
        void DeleteWidthAndHeight (CString& str);
        void RemoveTemporaryImageFile ();
    private :
        CStringList     m_pathTmp;
        CResourceFile  *m_rf;
};

#endif
