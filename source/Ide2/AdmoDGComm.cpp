// AdmoDGComm.cpp : file di implementazione
//

#include "stdafx.h"
#include "Kos.h"
#include "AdmoDGComm.h"


// CAdmoDGComm

IMPLEMENT_DYNCREATE(CAdmoDGComm, CView)

CAdmoDGComm::CAdmoDGComm()
{

}

CAdmoDGComm::~CAdmoDGComm()
{
}

BEGIN_MESSAGE_MAP(CAdmoDGComm, CView)
END_MESSAGE_MAP()


// disegno di CAdmoDGComm

void CAdmoDGComm::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: aggiungere qui il codice di disegno
}


// diagnostica di CAdmoDGComm

#ifdef _DEBUG
void CAdmoDGComm::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAdmoDGComm::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// gestori di messaggi CAdmoDGComm
