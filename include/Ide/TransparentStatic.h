#pragma once


// CTransparentStatic

class CTransparentStatic : public CStatic
{
	DECLARE_DYNAMIC(CTransparentStatic)

public:
	CTransparentStatic();
	virtual ~CTransparentStatic();
    void SetXOffset (int nX)   { m_nOffsetX=nX; }
    void SetYOffset (int nY)   { m_nOffsetY=nY; }
    void SetForeColor (DWORD dwFC)   { m_rgbForeColor=dwFC; }
    void SetBackColor (DWORD dwBC)   { m_rgbBackColor=dwBC; }
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private :
    int m_nOffsetX;
    int m_nOffsetY;
    DWORD m_rgbForeColor;
    DWORD m_rgbBackColor;
};


