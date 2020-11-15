// GridCellName.h: interface for the CGridCellNumeric class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GRICELLNAME)
#define GRICELLNAME

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCell.h"

class CGridCellName : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellName)

public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();
protected:
};

#endif // !defined(GRICELLNAME)
