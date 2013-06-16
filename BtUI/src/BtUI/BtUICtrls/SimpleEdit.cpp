#include "stdafx.h"
#include "SimpleEdit.h"

BEGIN_MESSAGE_MAP(CSimpleEdit, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

CSimpleEdit::CSimpleEdit()
// 	: m_nRowHeight(0)
// 	, m_nRow(0)
// 	, m_nColumn(0)
// 	, m_nTitleHeight(0)
{
	m_font.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,			   // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		PROOF_QUALITY,             // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Î¢ÈíÑÅºÚ") );              // lpszFacename
}

CSimpleEdit::~CSimpleEdit()
{
	DeleteObject(m_font);
	DeleteObject(m_Brush);
}

void CSimpleEdit::UpdateCtrl( BYTE* buf )
{
	if (buf)
	{
		JY_CMISCtrlData *data = reinterpret_cast<JY_CMISCtrlData*>(buf);

		CString str;
		int count = data->size;
		int *arr = reinterpret_cast<int*>(data->buf);

		int start = 0;
		int end = 0;
		for (int i = 0; i < count; i++)
		{
			end = m_strOrgText.Find(L"%d", start) + 2;
			str.AppendFormat(m_strOrgText.Mid(start, end - start), arr[i]);
			start = end;
		}
		str.AppendFormat(m_strOrgText.Right(m_strOrgText.GetLength() - start));
		m_strOutput = str;

		SetFont(&m_font);
		SetWindowText(m_strOutput);
		delete data;
	}
}

BOOL CSimpleEdit::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	CBitmap bmp;

	bmp.CreateCompatibleBitmap(pDC, m_rcWnd.Width(), m_rcWnd.Height());
	memdc.SelectObject(&bmp);
	memdc.SelectObject(&m_font);
	memdc.SetBkMode(TRANSPARENT);
	m_imgBack.Draw(memdc.GetSafeHdc(), 0, 0, m_rcWnd.Width(), m_rcWnd.Height());

	pDC->BitBlt(0, 0, m_rcWnd.Width(), m_rcWnd.Height(), &memdc, 0, 0, SRCCOPY);
	return TRUE;
	//return __super::OnEraseBkgnd(pDC);
}

HBRUSH CSimpleEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	HBITMAP bit = m_imgBack.MakeBitmap();
	CBitmap *pbmp = CBitmap::FromHandle(bit);

	m_Brush.DeleteObject();
	m_Brush.CreatePatternBrush(pbmp);
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)m_Brush;
}
