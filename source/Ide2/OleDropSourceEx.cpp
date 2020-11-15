// OleDropSourceEx.cpp : implementation file
//

#include "stdafx.h"
#include "ide2.h"
#include "OleDropSourceEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COleDropSourceEx

COleDropSourceEx::COleDropSourceEx()
{
}

COleDropSourceEx::~COleDropSourceEx()
{
}

/////////////////////////////////////////////////////////////////////////////
// COleDropSourceEx diagnostics

#ifdef _DEBUG
void COleDropSourceEx::AssertValid() const
{
	COleDropSource::AssertValid();
}

void COleDropSourceEx::Dump(CDumpContext& dc) const
{
	COleDropSource::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COleDropSourceEx serialization

void COleDropSourceEx::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleDropSourceEx commands

SCODE COleDropSourceEx::GiveFeedback(DROPEFFECT dropEffect) 
{		
	return COleDropSource::GiveFeedback(dropEffect);
}
