/*
** LuaLibrary gestione delle librerie
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NMAXMODULES 1024
#define HEADER_LUA_FILES  "@@@__THIRALIB__@@@"
#define VERSION_LUA_FILES 0x02090006

extern const char* intermediate;
extern char** astrLibraries;
extern int libraries;
static void CopyLine (char** p, char* v);
static void ExplodeLibrary (char* pLibrary, char** pargv, int* n);
static int  ReadHeader (FILE* f);
static int  ReadVersion (FILE* f);
static int  ReadDescription (FILE* f);
static int  ReadModules (FILE* f, char*** pargv, int* n);
static int	ReadName (FILE* f, char* szName);
static int  ReadCode (FILE* f, unsigned char** buffer, int* buflen);
static int  WriteModule (FILE* f, char* szName, unsigned char* buffer, int buflen, char** pargv, int* n);

char** lua_library (int* argc, char *argv[])
{
	char** pargv;
	int i = 0, n = 0, p;

	pargv = (char **) calloc (NMAXMODULES, sizeof (char *));
	/* ora il .oop */
	CopyLine (&pargv[i], argv[i]);
	i++, n++;
	/* ora le librerie */
	for (p = 0; p<libraries; p++)
		ExplodeLibrary (astrLibraries[p], pargv, &n);
	/* copiamo ora i moduli oggetti */
	while (i < *argc)   {
		CopyLine (&pargv[n], argv[i]);
		n++, i++;
	}
	*argc = n;
	return pargv;
}

void CopyLine (char** p, char* v)
{
	*p = malloc (strlen (v) + 1);
	strcpy (*p, v);
}

void ExplodeLibrary (char* pLibrary, char** pargv, int* n)
{
	FILE* f;
	/* prendiamo la libraria */
	/* esplodiamo la libraria nel direttorio 'intermediate' */
	if ((f = fopen (pLibrary, "rb")) != NULL)   {
		if (!ReadHeader (f))   {
			ReadVersion (f);
			ReadDescription (f);
			ReadModules (f, pargv, n);
		}
		fclose (f);
	}
	/* copiamo i moduli nel  */
}

int ReadHeader (FILE* f)
{
	int nB = 0;
	char szBuffer[256];

	fread (szBuffer, strlen (HEADER_LUA_FILES) + 1, 1, f);
	if (strcmp (szBuffer, HEADER_LUA_FILES))
		nB = -1;
	return nB;
}

int ReadVersion (FILE* f)
{
	int nVersion, nB = 0;
	fread (&nVersion, sizeof (int), 1, f);
	if (nB != VERSION_LUA_FILES)
		nB = -1;
	return nB;
}

int ReadDescription (FILE* f)
{
	int nDescLenght;
	char szDesc[256];

	fread (&nDescLenght, sizeof (int), 1, f);
	fread (szDesc, nDescLenght, 1, f);
	return 0;
}

int ReadModules (FILE* f, char** pargv, int* n)
{
	int nNumModules, buflen, i;
	unsigned char* buffer;
	char szName[256];

	fread (&nNumModules, sizeof (int), 1, f);
	for (i = 0; i<nNumModules; i++)   {
		ReadName (f, szName);
		ReadCode (f, &buffer, &buflen);
		ReadXML (f);
		WriteModule (f, szName, buffer, buflen, pargv, n);
		free (buffer);
	}
	return 0;
}

int	ReadName (FILE* f, char* szName)
{
	int nNameLenght;

	fread (&nNameLenght, sizeof (int), 1, f);
	fread (szName, nNameLenght, 1, f);
	return 0;
}

int ReadCode (FILE* f, unsigned char** buffer, int* buflen)
{
	fread (buflen, sizeof (int), 1, f);
	*buffer = malloc (*buflen);
	fread (*buffer, *buflen, 1, f);
	return 0;
}

int WriteModule (FILE* f, char* szName, unsigned char* buffer, int buflen, char** pargv, int* n)
{
	FILE* fl;
	char szBuffer[_MAX_PATH];

	/* scriviamo per prima cosa il file nel direttorio intermediate */
	sprintf (szBuffer, "%s\\%s.mdl", intermediate, szName);
	if ((fl = fopen (szBuffer, "wb")) != NULL)   {
		fwrite (buffer, buflen, 1, fl);
		fclose (fl);
	}
	/* ora aggiungiamo il nome del file al direttorio */
	CopyLine (&pargv[*n], szBuffer);
	(*n)++;
	return 0;
}

int ReadXML (FILE* f)
{
	int nXmlLenght;
	fread (&nXmlLenght, sizeof (int), 1, f);
	fseek (f, nXmlLenght, SEEK_CUR);
}
