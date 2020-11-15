#pragma once


// Finestra di dialogo DlgSelSymbol

class CDlgSelSymbol : public CDialog
{
	DECLARE_DYNAMIC(CDlgSelSymbol)

public:
	CDlgSelSymbol(CWnd* pParent = nullptr);   // costruttore standard
	virtual ~CDlgSelSymbol();
	void SetSymbols(stBrowserDescriptor* pBrowseData, int nItemSearch) {
		m_pBrowseData = pBrowseData;
		m_nItemSearch = nItemSearch;
	}
	stBrowserDescriptor* GetSelectedBrowserDescriptor() {
		return m_pSelectedBrowseData;
	}
	// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SEL_SYMBOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	// List View di selezione simboli ambigui
	CListCtrl m_SelSymbol;
private :
	stBrowserDescriptor* m_pBrowseData;
	stBrowserDescriptor* m_pSelectedBrowseData;
	int m_nItemSearch;
	CImageList m_images;
private :
	CString GetSymbolName(stBrowserDescriptor* bd);
	CString GetSymbolType(stBrowserDescriptor* bd);
	void	SetImageList();
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
