/*
**
*/

#include "stdafx.h"
#include "IdeMisc.h"

/*
** CreateFont : crea il font che verra' usato nel listview
*/
CFont* CreateFont (const char* strFont, int nHeight)
{
    CFont* pFont = new CFont;
    pFont->CreateFont(
         nHeight,
         0,
         0,
         0,
         FW_NORMAL,
	     FALSE,
         FALSE,
         0,
         ANSI_CHARSET,
	     OUT_DEFAULT_PRECIS,
	     CLIP_DEFAULT_PRECIS,
	     DEFAULT_QUALITY,
	     DEFAULT_PITCH, _T(strFont));
    return pFont;
}
