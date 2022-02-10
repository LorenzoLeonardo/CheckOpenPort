#include "pch.h"
#include "CListCtrlCustom.h"


CListCtrlCustom::CListCtrlCustom()
{

}

CListCtrlCustom::~CListCtrlCustom()
{
}

BEGIN_MESSAGE_MAP(CListCtrlCustom, CListCtrl)

	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)

END_MESSAGE_MAP()

void CListCtrlCustom::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	size_t iRow = lplvcd->nmcd.dwItemSpec;
	bool bHighlighted = false;

	switch (lplvcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	}
	// Modify item text and or background
	case CDDS_ITEMPREPAINT:
	case CDDS_SUBITEM:
	{
		if (lplvcd->nmcd.uItemState & CDIS_SELECTED)
		{
		//	lplvcd->nmcd.uItemState = ((lplvcd->nmcd.uItemState) & (~CDIS_SELECTED));
		//lplvcd->clrTextBk = RGB(0, 255, 255);
		}
		else
		{
			if (iRow % 2)
			{
				lplvcd->clrTextBk = RGB(0, 255, 255);
			}
			else
			{
				lplvcd->clrTextBk = RGB(255, 255, 255);
			}
		}

		*pResult = CDRF_SKIPPOSTPAINT;
		break;
	}
	}
}