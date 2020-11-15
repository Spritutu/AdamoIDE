// TabBitmap.cpp: implementation of the CTabBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ide2.h"
#include "TabBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTabBitmap::CTabBitmap()
{

}

CTabBitmap::~CTabBitmap()
{

}


BOOL CTabBitmap::LoadBitmap(LPCTSTR szFilename)
{
    ASSERT(szFilename);
	DeleteObject();

	HBITMAP hBitmap = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, szFilename, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);
	return Attach(hBitmap);
}

