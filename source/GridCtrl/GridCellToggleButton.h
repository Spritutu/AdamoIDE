/*
**
*/

#if !defined(__GRIDCELL_TOGGLEUTTON__)
#define __GRIDCELL_TOGGLEUTTON__

/* include files */
#include "GridCell.h"

class CGridCellToggleButton : public CGridCell  
{
    CGridCellToggleButton ();
    DECLARE_DYNCREATE(CGridCellToggleButton)

public:
    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd);
    virtual void OnClick(CPoint PointCellRelative); 
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar);
    void         ResetToggle ();
private :
    bool  m_bToggled;
    CRect m_Rect;
};

#endif // !defined(AFX_GRIDINTEGERCELL_H__3479ED0D_B57D_4940_B83D_9E2296ED75B5__INCLUDED_)
