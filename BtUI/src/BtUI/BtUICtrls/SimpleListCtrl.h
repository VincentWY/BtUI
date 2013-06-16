#pragma once
#include "BaseCtrl.h"
#include <vector>

class CSimpleListCtrl : public CStatic,
						public CJYBaseCtrl
{
public:
	CSimpleListCtrl();
	~CSimpleListCtrl();
public:
	virtual void UpdateCtrl(BYTE* buf);
	void SetHeight(int titleheight, int rowheight);
	void SetColWidth(CString& str);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
	CFont m_font;
	//CxImage m_imgBack;
	std::vector<int> m_vecColWidth;
	int m_nRowHeight;
	int m_nTitleHeight;
	int m_nRow;
	int m_nColumn;
	std::vector<CString> m_vecListData;
};