#pragma once
//#include "CMISDef.h"
#include "BaseCtrl.h"

class CSimpleStatic : public CStatic,
					  public CJYBaseCtrl
{
public:
	CSimpleStatic();
	~CSimpleStatic();
public:
// 	void SetOrgText(CString& str);
// 	void SetOrgText(LPTSTR buf);
//	void SetTextRect(CRect& rc);
	virtual void UpdateCtrl(BYTE* buf);

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
private:
	//CString m_strText;//原始字符串，相当于字符串表中的ID
	CString m_strOutput;
	//CRect m_rcText;//文字输出区域
	CFont m_font;

	CxImage m_imgBack;
};