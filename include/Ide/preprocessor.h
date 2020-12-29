/*
** preprocessor.h
*/

#ifndef __ADAMO__PREPROCESSOR__
#define __ADAMO__PREPROCESSOR__

/* include files */
#include "adamo.h"
#include "AdamoInt.h"
#include "AdamoComm.h"
#if defined ADAMO_IDE || defined ADAMO_KOS
#include "AdamoMachine.h"
#endif
#include "CVariableContainer.h"
#include "CBrowserContainer.h"
#include "CSyntaxContainer.h"
#include "CDefinesContainer.h"
#include "AdamoFileDati.h"
#include "AdamoTraduzione.h"
#include "stackx.h"

extern "C"    {
    #include "lobject.h"
}

#include "lexxer.h"

/* constants definitions */
#define SEPVIRGOLA                  27
#define SEPPARENTESIQUADRAAPERTA    28
#define SEPPARENTESIQUADRACHIUSA    29
#define SEPDUEPUNTI                 30
#define SEPPARENTESITONDAAPERTA     31
#define SEPPARENTESITONDACHIUSA     32

#define TOKNOTOK                      0
#define TOKGLOBAL                   200
#define TOKDEFINE                   201
#define TOKUNDEFINE                 202
#define TOKIFDEF                    203
#define TOKIFNOTDEF                 204
#define TOKELSEDEF                  205
#define TOKENDDEF                   206
#define TOK_STRING_LOAD             207
#define TOK_CONF_GETNAME            208
#define TOK_CONF_GETPATH            209
#define TOK_CONF_GETDESCRIPTION     210
#define TOKGROUP                    211
#define TOKSWITCH                   212
#define TOKCASE                     213
#define TOKNEW                      214
#define TOKDELETE                   215
#define TOK_ISO_DISPATCHMESSAGE     216
#define TOKOBJECT                   217
#define TOKFUNCTION                 218

#define TOKBOOLEAN                  251
#define TOKNUMBER                   252
#define TOKSTRING                   253
#define TOKAS                       254
#define TOKMATRIX                   255
#define TOKARRAY                    256
#define TOKIS                       257
#define TOKTABLE                    258
#define TOKPUBLIC                   259
#define TOKPRIVATE                  260
#define TOKVIRTUAL                  261
#define TOKEND		                405

#define RULSYMBOL	                  0
#define RULSTRINGS                  300
#define RULNUMBERS                  301
#define RULFPNUMBERS                302
#define RULHEXNUMBERS               303
#define RULFPDEVICE                 304
#define RULFPDEVICETYPE             305

#define LUALINECOMMENT              419
#define LUAOPENBLOCKCOMMENT         420
#define LUACLOSEBLOCKCOMMENT        421

#define TODONOTHING                   0
#define TODONEWLINE                   1
#define TODOGLOBAL                    2
#define TODOSUBSTITUTION              3
#define TODOGROUP                     4
#define TODOADDDEFINE                 5
#define TODOREMOVEDEFINE              6
#define TODOIFDEF                     7

#define AXISINFOBASE                500
#define AXISINFOLAST                577

#define PHYSICAL_DEFINE             "PHYSIC"
#define SIMULATOR_DEFINE            "SIMULATION"

#define NMAXPTOKENS                 256
#define NMAXCHKSYNTOKENS             32
#define NMAXCHKSYNTYPES               8

#define TOKSOST_DIGITALINPUT        600
#define TOKSOST_DIGITALOUTPUT       601
#define TOKSOST_ANALOGINPUT         602
#define TOKSOST_ANALOGOUTPUT        603
#define TOKSOST_AXIS                604
#define TOKSOST_ENCODER             605
#define TOKSOST_TIMER               606
#define TOKSOST_STORAGE             607
#define TOKSOST_DEVICECAN           608
#define TOKSOST_ON                  609
#define TOKSOST_OFF                 610
#define TOKSOST_UP                  611
#define TOKSOST_DOWN                612
#define TOKSOST_POSITIVE            613
#define TOKSOST_NEGATIVE            614
#define TOKSOST_CLKW                615
#define TOKSOST_CCLKW               616
#define TOKSOST_SKIP                617
#define TOKSOST_WAIT                618
#define TOKSOST_ACK                 619
#define TOKSOST_DIFFERENT           620

#define NMAXAT                      256
#define ISTOKENAKEY(x)              (x>=TOKGLOBAL&&x<=TOKOBJECT)

/* data definitions */
struct stAdamoToken   {
    stAdamoToken () : pDev (NULL) {}
    CString  strToken;
    CString  strRiporto;
    int      nID;
    int      nIndex;
    CAdamoBase* pDev;
};

struct stStackSwitch   {
    stStackSwitch () : nStep (0) { }
    CString strVariable;
    int     nStep;
};

struct stStackDefine   {
    stStackDefine () : bWriteCode (false), bElse (false), bSaveCode (false) {}
    bool bSaveCode;
    bool bWriteCode;
    bool bElse;
};

struct stChkSyntaxEl   {
    unsigned short int  nID[524][8];
    unsigned char nModificatore[524];
};

/* GetAxisInfo */
struct stAxisInfo   {
    int   nCode;
    char* pStr;
};
            
/* class definitions */
class CPreProcessorListener : public cooLexxerListener  {
    friend class CPreProcessor;
    public :
        CPreProcessorListener ();
	    virtual	bool fCheckValid() const	{ return true; };
	    virtual bool fShouldDelete() const  { return false; };
	    virtual	void vRegisterToken(const std::tstring& strTokenText, const cooLexxerTokenRule* pptrRule);
        int  ParsePreProcessorString (char* szBuffer, int i);
        void SetVariableContainer (CVariableContainer* glbArray)   { m_glbArray=glbArray; }
        void SetBrowserContainer  (CBrowserContainer*  symArray)   { m_symArray=symArray; }
        void SetSyntaxContainer  (CSyntaxContainer*  synArray)     { m_synArray=synArray; }
        void SetDefineContainer(CDefinesContainer* defArray)       { m_defArray = defArray; }
        void SetPathDest (const char* szPathDest)                  { m_strPathDest=szPathDest; }
        void SetTraduzioniObj (CTraduzioni* pTr)                   { m_tr=pTr; }
        void SetDefineTokenMap (CMapStringToString* deftok)        { m_deftok=deftok; }
        void SetAdamoMachine (CAdamoMachine* objMachine)           {
            m_objMachine=(CAdamoCfgMachine*)objMachine;
            m_strMachineName=m_objMachine->GetAdamoModule()->GetName();
        }
        CString GetErrorString ()                                  { return m_strError; }
        CString GetResult ()                                       { return m_strResult; }
    private :
        cooLexxerMap m_map;
        CAdamoCfgMachine* m_objMachine;
        CPtrArray m_ptrCodeArray;
        struct  stAdamoToken* m_apTokenKey[NMAXPTOKENS];
        int     m_nTokenKey;
        CString m_strBuffer;
        CString m_strConCat;
        CString m_strResult;
        CString m_strWithName;
        CString m_strMachineName;
        bool    m_bNewLineFlag;
        bool    m_bSomethingInteresting;
        bool    m_bLineComment;
        bool    m_bBlockComment;
        struct  stChkSyntaxEl TokenSyntax[NMAXCHKSYNTOKENS][NMAXCHKSYNTYPES];
        CVariableContainer* m_glbArray;
        CBrowserContainer*  m_symArray;
		CSyntaxContainer*   m_synArray;
        CDefinesContainer*  m_defArray;
        CString m_strPathDest;
        CTraduzioni* m_tr;
        CMapStringToString* m_deftok;
        bool m_bWriteCode;
        CString m_strError;
        CString m_strBlockComment;
        int     m_nError;
        CStackX<stStackDefine> m_stackDefine;
        CStackX<stStackSwitch> m_stackSwitch;
        int m_nProgLine;
		int m_nBrowseLine;
		bool m_bGiorgettiClary;
		CString m_strCurrentFunction;
		CString m_strCurrentObject;
		CStringList m_strlistVirtualFunction;
    private :
        bool Translate ();
        bool ManageUnderScore ();
        bool ThereIsUnderScore (CString& szBuffer);
        void CleanUp ();
        void WriteResult ();
        void InitSyntaxStructures ();
        bool Substitution ();
        bool ManageConfigurationSyntax (stAdamoToken* p);
        bool ManageEnumSyntax (stAdamoToken* p);
        int  IsConfigurationSyntax (const char* szConf);
        int  GetConfigurationToken (char* szConf, char* szNewToken);
		int  GetEnumToken (char* szEnum, char* szNewToken);
        void SetTokenNumber (stAdamoToken* p, const char* sz);
        bool Action ();
        bool ManageGlobal (stAdamoToken* p);
        bool ManageGroup (stAdamoToken* p);
        bool ManageStringLoad (stAdamoToken* p);
        void InitGlobalToken (stGlobalDescriptor* gd);
        int  ProbablyIs (stAdamoToken* p);
        bool SaveResult (stGlobalDescriptor* gd);
        CString WriteResultLine (struct stGlobalDescriptor* gd);
        bool ManageVariable (stAdamoToken* p, stGlobalDescriptor* gd);
        bool ManageArray (stAdamoToken* p, stGlobalDescriptor* gd);
        bool ManageMatrix (stAdamoToken* p, stGlobalDescriptor* gd);
        bool ManageDefine (stAdamoToken* p);
        bool ManageUnDefine (stAdamoToken* p);
        bool ManageIfDefine (stAdamoToken* p);
        bool ManageIfNotDefine (stAdamoToken* p);
        bool ManageElseDefine (stAdamoToken* p);
        bool ManageEndDefine (stAdamoToken* p);
        void ChangeLine (CString str);
        bool Operation (CAdamoData* adOp1, int nOp, CAdamoData* adOp2);
        void Cast (CAdamoData* adOp1, CAdamoData* adOp2);
        bool GetIfDefOperator (int nId, CString strToken, CAdamoData* pOp, bool* bDefined);
        bool GetOperation (int nID, int* nOp);
        void ManageErrors ();
        bool CheckSyntax ();
        void CleanUP ();
        void GestioneGetAxisInfo (stAdamoToken* p);
        int  CorrectID (stAdamoToken* p, int nID);
        bool CheckParticularRules (stAdamoToken* p, int nCode);
        bool ExistConfigurationToken (stAdamoToken* p);
        bool ManageConfSyntax (stAdamoToken* p);
        bool IsTokenDefined (CString strToken);
        bool ManageSwitch (stAdamoToken* p);
        bool ManageCase (stAdamoToken* p);
        void RemoveEl (int nIndex, int nCount);
        int  FilterBlockComment (const char* sz, int nID);
        bool ManageNew (stAdamoToken* p);
        bool ManageDelete (stAdamoToken* p);
        int  FilterInputBuffer ();
        void DeleteBlanks (CString& str, int& nPos);
        bool IsSeparator (char ch);
        bool ManageDefineValue (stAdamoToken* p);
        bool IsDefineWorld (int nID);
        bool ManageIsoDispatchMessage (stAdamoToken* p);
        void ManageObjects ();
        CString BuildSwitchVariable (stAdamoToken* p);
		int  CharacterCount (CString str, char ch );
		int  AssignNumericID (const std::tstring& strTokenText);
		void Browsing ();
		int  CalcolaIstanza (CString strSymbol, int nIndex);
		void InitCreateObject ();
		void EndCreateObject ();
		int  Objects ();
		CString GetInstanceName (int nIndex);
		bool ManageObjectInstance (stAdamoToken* p, stGlobalDescriptor* gd);
		int CalcolaIstanzaTable (CString strSymbol, int nIndex);
};

class CPreProcessor
{
    public :
        CPreProcessor () : m_objMachine (NULL), m_bBackupPreprocessedFile (false), m_bAbortCompilation (false)   { }
        int StartPreProcessing (CString strPathSource, CString strPathDest, bool bIsMain);
        int GetProgLine ()   { return m_nProgLine; }
        void SetVariableContainer (CVariableContainer* glbArray)   { pl.SetVariableContainer (glbArray); }
        void SetBrowserContainer  (CBrowserContainer*  symArray)   { pl.SetBrowserContainer (symArray); }
		void SetSyntaxContainer (CSyntaxContainer* synArray)	   { pl.SetSyntaxContainer (synArray); }
        void SetDefineContainer(CDefinesContainer* defArray)       { pl.SetDefineContainer(defArray); }
        void SetAdamoMachine (CAdamoMachine* objMachine)   { m_objMachine=(CAdamoCfgMachine*)objMachine, pl.SetAdamoMachine (objMachine); }
        void SetTraduzioniObj (CTraduzioni* pTr)   { pl.SetTraduzioniObj (pTr); }
		void SetIntermediateDir (CString strPathIntermediate)		{ m_strPathIntermediate = strPathIntermediate; }
		CString GetIntermediateDir ()								{ return m_strPathIntermediate; }
        CString GetOutputString ();
		void AbortCompilation ()									{ m_bAbortCompilation = true; }
		bool IsCompilationAborted ()								{ return m_bAbortCompilation; }
        CMapStringToString& GetDefinesTokens()                      { return m_deftok; }
        CMapStringToString& GetGlobalDefinesTokens()                { return m_defglbtok; }
private :
        CString m_strPathSource;
        CString m_strPathDest;
		CString m_strPathIntermediate;
        int m_nProgLine;
        CPreProcessorListener pl;
        CMapStringToString m_deftok;
        CMapStringToString m_defglbtok;
        CAdamoCfgMachine* m_objMachine;
        bool m_bBackupPreprocessedFile;
		bool m_bAbortCompilation;
    private :
        void InitPreprocessing (bool bIsMain);
        int  PreProcess (CStdioFile* pFS, CStdioFile* pFD);
        void CopyMainMapToLocal ();
        void BackupPreprocessedFile ();
        void ViewPreprocessedFile (CString strPath);
};

#endif
