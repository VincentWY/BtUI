#pragma once
#include "BaseCtrl.h"
#include <vector>

class CSimpleEdit : public CEdit,
					public CJYBaseCtrl
{
public:
	CSimpleEdit();
	~CSimpleEdit();
public:
	virtual void UpdateCtrl(BYTE* buf);

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
private:
 	CFont m_font;
	CString m_strOutput;
	CBrush m_Brush;
};