/************************************************************************/
/* MemDCEx.h                                                                     */
/************************************************************************/

#pragma once
#include "..\BtUIHead.h"

class BTUI_API CMemDCEx : public CDC
{
public:
	CMemDCEx(CDC* pDc, CWnd* pWnd)
	{
		m_pOldDC = pDc;
		CreateCompatibleDC(pDc);
		pWnd->GetClientRect(m_rc);
		m_bmp.CreateCompatibleBitmap(pDc, m_rc.Width(), m_rc.Height());
		SelectObject(&m_bmp);
		FillSolidRect(&m_rc, RGB(128, 128, 128));
	}
	CMemDCEx(CDC* pDc, CRect& rc)
	{
		m_pOldDC = pDc;
		CreateCompatibleDC(pDc);
		m_rc = rc;
		m_bmp.CreateCompatibleBitmap(pDc, m_rc.Width(), m_rc.Height());
		SelectObject(&m_bmp);
		FillSolidRect(&m_rc, RGB(128, 128, 128));
	}
	CMemDCEx(CDC* pDc)
	{
		m_pOldDC = pDc;
		CreateCompatibleDC(pDc);
		pDc->GetWindow()->GetClientRect(m_rc);
		m_bmp.CreateCompatibleBitmap(pDc, m_rc.Width(), m_rc.Height());
		SelectObject(&m_bmp);
		FillSolidRect(&m_rc, RGB(128, 128, 128));
	}
	CMemDCEx(CDC& pDc)
	{
		m_pOldDC = &pDc;
		CreateCompatibleDC(&pDc);
		pDc.GetWindow()->GetClientRect(m_rc);
		m_bmp.CreateCompatibleBitmap(&pDc, m_rc.Width(), m_rc.Height());
		SelectObject(&m_bmp);
		FillSolidRect(&m_rc, RGB(128, 128, 128));
	}
	~CMemDCEx(void)
	{
		m_pOldDC->BitBlt(0, 0, m_rc.Width(), m_rc.Height(), this, 0, 0, SRCCOPY);
		m_bmp.DeleteObject();
		m_pOldDC = NULL;
	}

	void FillDC(COLORREF clr)
	{
		FillSolidRect(m_rc, clr);
	}
private:
	CBitmap m_bmp;
	CDC* m_pOldDC;
	CRect m_rc;
};
