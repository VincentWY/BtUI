/************************************************************************/
/*EditBT.cpp                                                            */
/************************************************************************/

#pragma once
#include "BaseCtrl.h"

class BTUI_API CEditBT : public CEdit,
					public CBTBaseCtrl
{
public:
	CEditBT();
	~CEditBT();
public:
	virtual void UpdateCtrl(BYTE* buf);
	void SetFontSize(int nSize);
	void SetTextColor(COLORREF clr);
	void SetDefaultText(CString str);
	void SetPassword(BOOL mark = TRUE);

	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
	CFont m_font;
	CString m_strOutput;
	CBrush m_Brush;
	COLORREF m_clrText;
	int m_nFontSize;
	CString strDefaultText;
	BOOL m_bIsPassword;
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};