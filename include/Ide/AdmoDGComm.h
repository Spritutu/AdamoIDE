#pragma once


// visualizzazione CAdmoDGComm

class CAdmoDGComm : public CView
{
	DECLARE_DYNCREATE(CAdmoDGComm)

protected:
	CAdmoDGComm();           // costruttore protetto utilizzato dalla creazione dinamica
	virtual ~CAdmoDGComm();

public:
	virtual void OnDraw(CDC* pDC);      // sottoposto a override per la creazione di questa visualizzazione
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


