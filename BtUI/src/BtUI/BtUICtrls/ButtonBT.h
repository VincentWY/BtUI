/************************************************************************/
/* ButtonBT.h                                                           */
/************************************************************************/

#pragma once
#include "BaseCtrl.h"

class BTUI_API CButtonBT : public CButton,
					public CBTBaseCtrl
{
public:

	CButtonBT();
	virtual ~CButtonBT();

	// image types
	enum	{
				STD_TYPE	= 0,
				ALT_TYPE,
				DIS_TYPE
			};
public:
	virtual void UpdateCtrl(BYTE* buf);
	// sets the image type
	void SetImage(int type);

	BOOL LoadAltImage(UINT id, LPCTSTR pType = L"PNG");
	BOOL LoadStdImage(UINT id, LPCTSTR pType = L"PNG");
	BOOL LoadStdImage(LPCTSTR pName, HMODULE hInst = NULL);
	BOOL LoadAltImage(LPCTSTR pName, HMODULE hInst = NULL);

	// if false, disables the press state and uses grayscale image if it exists
	void EnableButton(BOOL bEnable = TRUE) { m_bIsDisabled = !bEnable; }

	// in toggle mode each press toggles between std and alt images
	void EnableToggle(BOOL bEnable = TRUE);

	// return the enable/disable state
	BOOL IsDisabled(void) {return (m_bIsDisabled == TRUE); }

	void SetBkGnd(CDC* pDC);

	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);
	void SetHorizontal(bool ImagesAreLaidOutHorizontally = FALSE);
	void DeleteToolTip();

	// set m_bIsFrame value, add by iunknown
	void SetFrameFlag(BOOL mark);

	void SetBtnMove(BOOL mark = TRUE);
	void SetBtnHot(BOOL mark = TRUE);
	void SetSelfText(CString strText, int fontSize = 18, BOOL mark = TRUE);
	void SetBtnText(CString& strText);
protected:

	void PaintBk(CDC* pDC);
	void PaintBtn(CDC* pDC, BOOL mark = FALSE);//edit by iunknown
	//void PaintBtn(CDC* pDC, int nFram);

	BOOL	m_bHaveAltImage;
	BOOL	m_bHaveBitmaps;

	BOOL	m_bIsDisabled;
	BOOL	m_bIsToggle;
	BOOL	m_bIsHovering;
	BOOL	m_bIsTracking;

	int		m_nCurType;
	BOOL	m_bISFrame;//是否绘制圆角边框  add by iunknown
	BOOL	m_bISMove;//鼠标按下时图片是否向右下MOVE一个像素
	BOOL	m_bISHot;//鼠标移上是否高亮
	BOOL	m_bISTextSelf;//按钮文字
	CFont	m_font;
	CString m_strText;
	int		m_nFontSize;

	//CGdiPlusBitmapResource* m_pAltImage;
	//CGdiPlusBitmapResource* m_pStdImage;

	CxImage m_imgAlt;
	CxImage m_imgStd;

	CString			m_tooltext;
	CToolTipCtrl*	m_pToolTip;
	
	void	InitToolTip();

	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_MSG(CButtonBT)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	CDC		m_dcBk;			// button background
	
	CDC		m_dcStd;		// standard button
	CDC		m_dcStdP;		// standard button pressed
	CDC		m_dcStdH;		// standard button hot

	CDC		m_dcAlt;		// alternate button
	CDC		m_dcAltP;		// alternate button pressed
	CDC		m_dcAltH;		// alternate button hot

	CDC		m_dcGS;			// grayscale button (does not have a hot or pressed state)

	CDC*	m_pCurBtn;		// current pointer to one of the above
};
