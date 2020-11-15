// AdamoDGModule.cpp : file di implementazione
//

#include "stdafx.h"
#include "AdamoDGModule.h"


// AdamoDGModule

IMPLEMENT_DYNCREATE(CAdamoDGModule, CView)

CAdamoDGModule::CAdamoDGModule()
{

}

CAdamoDGModule::~CAdamoDGModule()
{
}

BEGIN_MESSAGE_MAP(CAdamoDGModule, CView)
END_MESSAGE_MAP()


// disegno di AdamoDGModule

void CAdamoDGModule::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: aggiungere qui il codice di disegno
}


// diagnostica di AdamoDGModule

#ifdef _DEBUG
void CAdamoDGModule::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAdamoDGModule::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// gestori di messaggi AdamoDGModule
