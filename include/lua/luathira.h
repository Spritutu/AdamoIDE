/*
** $Id: luathira.c,v 1.44 2003/04/07 20:34:20 lhf Exp $
** Lua / Thira Interface header file
*/

/* include files */
#include "stdlib.h"
#include "stdio.h"
#include "lparser.h"
#include "lauxlib.h"
#include "llex.h"

/* constants definitions */
#define NMAXIDENTIFIER 32
#define MAXCOLUMNS      128
#define GLOBALS_DAT		"globals.dat"
#define FUNCTION_API	"function.api"
#define BROWSER_DAT		"browser.dat"
#define GLOBALVARIABLE      0
#define GLOBALARRAY         1
#define GLOBALMATRIX        2
#define BROWSER_TYPE_FUNCTION    10

/* external references */
#ifdef __cplusplus
extern "C" const char *globals;
#endif

/* global variables record */
struct recGlobals   {
    char szName[NMAXIDENTIFIER+1];
    char szModule[_MAX_PATH];
    int nType;
    int nVarType;
    struct   {
        int nArrType;
        int nArrElements;
    } arr;
    struct   {
        int nRows;
        int nCols;
        int anTypes[MAXCOLUMNS];
    } mat;
};

struct stGlobalDescriptor   {
    char strName[NMAXIDENTIFIER+1];
    char strModule[_MAX_PATH];
    int nType;
    int nVarType;
    struct stArr  {
        int nArrType;
        int nArrElements;
    } arr;
    struct stMat  {
        int nRows;
        int nCols;
        int anTypes[MAXCOLUMNS];
        char* strColumnName[MAXCOLUMNS];
    } mat;
};

struct stFunctionDescriptor   {
    char  strName[NMAXIDENTIFIER+1];
	unsigned int dwArgs;
	unsigned int bZeroArgs;
};

struct stBrowserDescriptor
{
    int nBrowserType;
    int nBrowserReference;
    char szSymbol[NMAXIDENTIFIER+1];
    char szModule[_MAX_PATH+1];
    int nLine;
};

/* functions prototyping */
#ifdef __cplusplus
extern "C"   {
#endif
	int  lua_thira_load_global_vars (const char* szPath);
	int  lua_thira_load_functions ();
	int lua_thira_load_lua_functions (const char* szPath);
	int  lua_thira_unload_global_vars ();
	int  lua_thira_unload_functions ();
	int lua_thira_unload_lua_functions ();
	struct stGlobalDescriptor* lua_thira_search_global_var (const char* strGV);
	struct stFunctionDescriptor* lua_thira_search_C_function (const char *strFN);
	struct stBrowserDescriptor* lua_thira_search_lua_function (const char *strLF);
	int  lua_thira_check_global_var (LexState *ls, const char *szvar, int nbrackets, expdesc *row, expdesc *col, int nfield, expdesc *field, int bIsLocal);
	int  lua_thira_check_C_functions (LexState *ls, const char *szvar, int nfield, expdesc *field, int nArgs, int bIsLocal, char *szres);
	char *lua_thira_error (char *sz, const char *str, int n);
#ifdef __cplusplus
}
#endif

static void AddGlobalVariable (struct stGlobalDescriptor* p);
static void AddFunction (struct stFunctionDescriptor* p);
static void AddLuaFunction (struct stBrowserDescriptor* p);
static void ClearMapGlobalVariables ();
static void ClearMapFunctions ();
static void ClearMapLuaFunctions ();
static void GetGlobalsFromRecord (struct recGlobals* rc, struct stGlobalDescriptor* p);
static int  GetFunctionsPath (char *szBuffer);
