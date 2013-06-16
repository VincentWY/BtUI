/************************************************************************/
/*CHeaderCtrlBT.h                                                       */
/************************************************************************/

#pragma once

#include "BaseCtrl.h"
// CHeaderCtrlBT

class BTUI_API CHeaderCtrlBT : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CHeaderCtrlBT)

public:
	CHeaderCtrlBT();
	virtual ~CHeaderCtrlBT();
public:
	void SetHeadBackImg(CxImage& img);
	void SetHeadBackImg(LPCTSTR lpszImg);
	void SetHeadBackImg(UINT nID);

	void SetHeadHeight(float nHeightTime = 1.0);//倍数
	void SetFontNum(int nNum = 20);//字号
	void SetTextColor(COLORREF textclr = RGB(0,0,0));
	void SetLineColor(COLORREF lineclr = RGB(0,0,0));

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

public:

	LRESULT OnLayout( WPARAM wParam, LPARAM lParam );

private:
	CxImage m_imgBK;
	HMODULE m_hResHandle;
	float m_Height;  //表头高度，这是倍数,
	int m_nFontSize;
	CFont m_font;
	COLORREF m_clrLine;
	COLORREF m_clrText;
	CPen m_pen;
};


