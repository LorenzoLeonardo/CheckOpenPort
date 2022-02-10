#pragma once
#include <afxcmn.h>
class CListCtrlCustom :
    public CListCtrl
{
public:
    CListCtrlCustom();
    virtual ~CListCtrlCustom();

protected:

    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
    DECLARE_MESSAGE_MAP()
};

