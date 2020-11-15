#pragma once

// CAdamoSynRTAxData
class CAdamoSynRTAx;

class CAdamoSynRTAxData : public CStatic
{
	DECLARE_DYNAMIC(CAdamoSynRTAxData)

public:
	CAdamoSynRTAxData();
	virtual ~CAdamoSynRTAxData();
	void SetRTAx (CAdamoSynRTAx *pRTAx)		{ m_pRTAx = pRTAx; }
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private :
	CAdamoSynRTAx *m_pRTAx;
	CFont *m_pFontHeader;
	CFont *m_pFontNumbers;
private :
	int CreateFonts();
};


