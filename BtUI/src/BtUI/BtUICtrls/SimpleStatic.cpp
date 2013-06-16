#include "stdafx.h"
#include "SimpleStatic.h"

BEGIN_MESSAGE_MAP(CSimpleStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CSimpleStatic::CSimpleStatic()
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

CSimpleStatic::~CSimpleStatic()
{
	m_font.DeleteObject(); 
}

void CSimpleStatic::UpdateCtrl(BYTE* buf)
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

		//SetWindowText(m_strOutput);
		delete data;
	}
}


void CSimpleStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call __super::OnPaint() for painting messages
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	CBitmap bmp;
	
	bmp.CreateCompatibleBitmap(&dc, m_rcWnd.Width(), m_rcWnd.Height());
	memdc.SelectObject(&bmp);
	memdc.SelectObject(&m_font);
	memdc.SetBkMode(TRANSPARENT);
	m_imgBack.Draw(memdc.GetSafeHdc(), 0, 0, m_rcWnd.Width(), m_rcWnd.Height());
	memdc.DrawText(m_strOutput, m_rcWnd, DT_EDITCONTROL | DT_WORDBREAK | DT_LEFT );

	dc.BitBlt(0, 0, m_rcWnd.Width(), m_rcWnd.Height(), &memdc, 0, 0, SRCCOPY);
}

BOOL CSimpleStatic::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}
