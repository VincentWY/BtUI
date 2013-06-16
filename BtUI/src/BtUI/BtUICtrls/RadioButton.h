/************************************************************************/
/*RadioButton.h                                                         */
/************************************************************************/


#pragma once

#include "BaseCtrl.h"

#define WM_CXSHADE_RADIO WM_APP + 0x100
/////////////////////////////////////////////////////////////////////////////
// CRadioButton window
class BTUI_API CRadioButton : public CButton,
					 public CBTBaseCtrl
{
// Construction
public:
	CRadioButton();

// Attributes
private:

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRadioButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
public:
	virtual void UpdateCtrl(BYTE* buf);
// Implementation
public:
	void SetToolTipText(CString s);
	void SetToolTipColor(COLORREF crText, COLORREF crBkgnd);
	COLORREF SetTextColor(COLORREF new_color);
	void SetSkin(UINT normal, UINT down, UINT over = 0, UINT disabled = 0, HMODULE hInst = NULL);
	void SetSkin(LPCTSTR pNoraml, LPCTSTR pDown, LPCTSTR pOver = NULL, LPCTSTR pDisabled = NULL, HMODULE hInst = NULL);

	virtual ~CRadioButton();
	// Generated message map functions
protected:
	bool	m_Checked;					//radio & check buttons
	DWORD	m_Style;					//radio & check buttons
    bool m_tracking;
    bool m_button_down;
	void RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
	CToolTipCtrl m_tooltip;
	//CBitmap m_bNormal,m_bDown,m_bDisabled,m_bMask,m_bOver,m_bFocus; //skin bitmaps
	//short	m_FocusRectMargin;		//dotted margin offset
	COLORREF m_TextColor;			//button text color
	HRGN	hClipRgn;				//clipping region

	void	DrawImg(CDC* dc, CxImage img, CRect& rc);

	//{{AFX_MSG(CRadioButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnClicked();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	afx_msg LRESULT OnRadioInfo(WPARAM, LPARAM);
	afx_msg LRESULT OnBMSetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnBMGetCheck(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

private:
	CxImage m_imgNormal;
	CxImage m_imgDown;
	CxImage m_imgMove;
	CxImage m_imgDisable;
	CDC		m_dcBk;			// button background
};
