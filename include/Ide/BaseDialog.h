/*
** BaseDialog.h
*/

#ifndef __ADAMO_DIALOG__
#define __ADAMO_DIALOG__

class CAdamoDialog : public CDialog
{
    public :
        CAdamoDialog (int nID, CWnd* pParent) : CDialog (nID, pParent), m_pMachine (NULL)   { }
        void SetMachine (CAdamoMachine* pMachine)   { m_pMachine=pMachine; }
        CAdamoMachine *GetMachine ()                { return m_pMachine; }
        void           SetVariableName (CString strName)   { m_strName=strName; }
        CString        GetVariableName ()                  { return m_strName; }
        void           SetKey (CString strKey)            { m_strKey=strKey; }
        CString        GetKey ()                           { return m_strKey; }
        virtual void   OnTimer () = 0;
    private :
        CAdamoMachine *m_pMachine;
        CString        m_strName;
        CString        m_strKey;
};

#endif
