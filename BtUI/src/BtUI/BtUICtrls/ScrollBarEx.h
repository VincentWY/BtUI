/************************************************************************/
/* ScrollBarEx.h : header file                                          */
/************************************************************************/
#pragma once

#include "BaseCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx window
typedef enum tagStatus_t
{
	SCROLL_NORMAL,
	SCROLL_MOVING,
	SCROLL_PUSHED
}ScrollStatus_t;

class CScrollBarEx : public CStatic
{
	// Construction
public:
	CScrollBarEx();
	
	// Attributes
public:
	BOOL InitScrollBar(CWnd *pCtrl, UINT nStyle);
	BOOL InitScrollBar(CWnd *pCtrl, UINT nStyle, UINT uID, HMODULE hModule = NULL);
	BOOL InitScrollBar(CWnd *pCtrl, UINT nStyle, CxImage& imgScroll);
	void UpdateScrollBar();
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollBarEx)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CScrollBarEx();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CScrollBarEx)
	afx_msg void OnPaint();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
private:
	CWnd* m_pCtrl;
	UINT m_nScrollStyle;
private:
	void DrawVScroll(CDC* pDC);
	void DrawHScroll(CDC* pDC);
private:
	ScrollStatus_t m_barStatu;	

	//top
	CxImage m_imgTopNormal;
	CxImage m_imgTopMovimg;
	CxImage m_imgTopPushed;

	//bottom
	CxImage m_imgBottomNormal;
	CxImage m_imgBottomMovimg;
	CxImage m_imgBottomPushed;

	//left
	CxImage m_imgLeftNormal;
	CxImage m_imgLeftMovimg;
	CxImage m_imgLeftPushed;

	//right
	CxImage m_imgRightNormal;
	CxImage m_imgRightMovimg;
	CxImage m_imgRightPushed;

	//V bar
	CxImage m_imgVNormal;
	CxImage m_imgVMovimg;
	CxImage m_imgVPushed;

	//H bar
	CxImage m_imgHNormal;
	CxImage m_imgHMovimg;
	CxImage m_imgHPushed;
	
	//back
	CxImage m_imgVBack;
	CxImage m_imgHBack;
};

/////////////////////////////////////////////////////////////////////////////
