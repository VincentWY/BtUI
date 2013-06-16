/************************************************************************/
/* ListCtrlBT.h                                                         */
/************************************************************************/

#pragma once
#include "BaseCtrl.h"
#include "HeaderCtrlBT.h"
#include "ScrollBarEx.h"
#include "afxcmn.h"
// CListCtrlBT

class BTUI_API CListCtrlBT : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlBT)

public:
	CListCtrlBT();
	virtual ~CListCtrlBT();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	void SetHeadBK(LPCTSTR lpszImg);
	void SetHeaderHeight(float Height);// 设置表头高度
	void SetHeaderTextColor(COLORREF color);
	void SetHeaderLineColor(COLORREF color);
	void SetHeaderFontSize(int size);

	void SetBKImg(LPCTSTR str);
	void SetRowColor(COLORREF clr1, COLORREF clr2);
	void SetTextColor(COLORREF clr, COLORREF clrsel);
	void SetLineColor(COLORREF clr);
	void SetSelColor(COLORREF clr);
	void SetFontSize(int size);
	void SetRowHeigt(int nHeight); //设置行高

	void SetScrollBarImg(LPCTSTR strimg);

private:
	CHeaderCtrlBT m_Header;
	HMODULE m_hResHandle;
	COLORREF m_rowClr1;
	COLORREF m_rowClr2;
	COLORREF m_TextClr;
	COLORREF m_LineClr;
	COLORREF m_SelClr;
	COLORREF m_TextClrSel;
	CxImage m_imgBK;
	CxImage m_imgScrollBar;
	int m_nRowHeight;// 行高
	int m_nFontSize;

	CFont m_font;

	CScrollBarEx	m_HScroll;
	CScrollBarEx	m_VScroll;
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


