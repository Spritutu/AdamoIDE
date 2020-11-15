// Workspace.h: interface for the CWorkspace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACE_H__3A2A57CC_E1F8_4FD4_B3E3_3CB26D292BB1__INCLUDED_)
#define AFX_WORKSPACE_H__3A2A57CC_E1F8_4FD4_B3E3_3CB26D292BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* include files */
#include "ide2.h"
#include "Project.h"

/* constants definitions */
#define GETWS()    (((CIdeApp*)AfxGetApp ())->GetCurrentWorkspace ())

class CWorkspace  
{
public:
    void SaveDatConfig();
    CWorkspace();
	virtual ~CWorkspace();
	void DeleteObject(CObject *pObj);
	void SetModified(BOOL bMod);
	void AddTail(CProject *pPrj);
	CProject* NewProject();
	CProject* GetPtrProject(CString strText);
	BOOL Save();
	BOOL Add();
	BOOL Close();
	BOOL Save(CString strPathName,BOOL bSaveIsModified=FALSE);
	BOOL SaveAs();
	bool Load();
    static bool Delete (CString strPlant);
    static void Rename (CString strPlant);
	static bool Copy (CString strPlant, CString &strName, CString &strDescription);
    CProject *GetActiveProject()    { return m_pActiveProject; }
    CString GetName ()              { return m_strName; }
    CString GetDescription ()       { return m_strDesc; }
    void SetActiveProject(CProject *pPrj)    { m_pActiveProject = pPrj; }
    CAdamoPlant* GetPtrPlant()      { return &m_objPlant; }
    CString GetWorkspacePathName () { return m_strPlantPath + "\\" + m_strName; }
    CString GetBasePathName ()      { return m_strPlantPath; }
	CProject* GetProjectByName (CString strText);
   	void SaveModified();
   	void SaveModConfig();
    void LoadPlantSettings ();
    void SavePlantSettings ();
    void Activate ();
    int  ActivateMachine (int nModule);
    bool ExistModule (int nModule);    
    static void RecursiveDelete(CString szPath);
    bool InitialLoad();
    static bool LoadPlantPath ();
    int ChangeLanguage ();
	void RemoveProject();
    int  GetProgramVersion ()           { return m_nProgramVersion; }
    int  GetDataVersion ()              { return m_ps.GetDataVersion (); }
    void SetDataVersion (int nVersion)  { m_ps.SetDataVersion (nVersion); }
	bool IsMultiMachine ()				{ return m_objPlant.GetCount () > 1; }
	static bool CloseCurrentProject ();
	static void RenameMachine (CString strImpianto, CString strOldMachine, CString strMachine);
	static bool GetMachineName (CString strPlant, CStringArray &astrMachine);
	static void NewLWR (CString strImpianto, CString strMachine, CString strNewImpianto, CString strNewMachine);
	static void ChangeLWR (CString strImpianto, CString strOldMachine, CString strMachine);
	static void ChangeLPR (CString strImpianto, CString strOldMachine, CString strMachine);
	static void ChangeCFG (CString strImpianto, CString strOldMachine, CString strMachine);
	static void ChangeDAT (CString strImpianto, CString strOldMachine, CString strMachine);
	static void RenameLastFiles (CString strImpianto, CString strOldMachine, CString strMachine);
	static bool CopyPlant (CString strPlant, CString strMachine, CString strNewPlant, CString strNewMachine); 
	static bool XCopy(LPCTSTR lpSource, LPCTSTR lpDestination);
	static bool DeletePlant (CString strPlant);
	static CString GetPlantDescription (CString strImpianto);
protected:
	BOOL CreateProjectDir(CString strPath);
    BOOL m_bModified;
    CAdamoPlant m_objPlant;
    CProject *m_pActiveProject;
    CString m_strName;
    CString m_strDesc;
    static CString m_strPlantPath;
private :
    static CWorkspace* m_this;
    CAdamoPlantSettings m_ps;
    int m_nProgramVersion;
private :
    bool    GetCurrentPlant (CString& str);
	static void ChangeDATRecords (stAdamoParametri *pAdamoParametri, CString strOldMachine, CString strMachine);
	static void ChangeDATName (char *szName, const char* szOldMachine, const char* szMachine);
};

#endif // !defined(AFX_WORKSPACE_H__3A2A57CC_E1F8_4FD4_B3E3_3CB26D292BB1__INCLUDED_)
