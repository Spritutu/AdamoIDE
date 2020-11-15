// TabBitmap.h: interface for the CTabBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABBITMAP_H__99FE2EF7_4266_43B4_B209_AA02036F7166__INCLUDED_)
#define AFX_TABBITMAP_H__99FE2EF7_4266_43B4_B209_AA02036F7166__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTabBitmap : public CBitmap   
{
public:
	CTabBitmap();
	virtual ~CTabBitmap();
    BOOL LoadBitmap(LPCTSTR szFilename);

};

#endif // !defined(AFX_TABBITMAP_H__99FE2EF7_4266_43B4_B209_AA02036F7166__INCLUDED_)
