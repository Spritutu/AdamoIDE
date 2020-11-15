// GridCellName.h: interface for the CGridCellNumeric class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(GRICELLDESC)
#define GRICELLDESC

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridCell.h"

class CGridCellDesc : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellDesc)

public:
    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
    virtual void EndEdit();
protected:
};

#endif // !defined(GRICELLDESC)
