#pragma once


// visualizzazione AdamoDGModule

class CAdamoDGModule : public CView
{
	DECLARE_DYNCREATE(CAdamoDGModule)

public :
	CAdamoDGModule();           // costruttore protetto utilizzato dalla creazione dinamica
	~CAdamoDGModule();

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


