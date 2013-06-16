/************************************************************************/
/*EditBT.h                                                              */
/************************************************************************/

#include "stdafx.h"
#include "EditBT.h"

BEGIN_MESSAGE_MAP(CEditBT, CEdit)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
END_MESSAGE_MAP()

CEditBT::CEditBT()
 	: m_nFontSize(20)
 	, m_clrText(RGB(128, 128, 128))
	, strDefaultText("")
	, m_bIsPassword(FALSE)
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

CEditBT::~CEditBT()
{
	DeleteObject(m_font);
	DeleteObject(m_Brush);
}

void CEditBT::UpdateCtrl( BYTE* buf )
{
	if (buf)
	{
		BT_CMISCtrlData *data = reinterpret_cast<BT_CMISCtrlData*>(buf);

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

BOOL CEditBT::OnEraseBkgnd(CDC* pDC)
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

HBRUSH CEditBT::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a non-NULL brush if the parent's handler should not be called
	HBITMAP bit = m_imgBack.MakeBitmap();
	CBitmap *pbmp = CBitmap::FromHandle(bit);

	m_Brush.DeleteObject();
	m_Brush.CreatePatternBrush(pbmp);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_clrText);
	return (HBRUSH)m_Brush;
}

BOOL CEditBT::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~WS_BORDER;
	return __super::PreCreateWindow(cs);
}

void CEditBT::SetFontSize( int nSize )
{
	m_font.DeleteObject();
	if (m_font.CreateFont(
		nSize,                        // nHeight
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
		_T("Î¢ÈíÑÅºÚ")))              // lpszFacename
	{
		m_nFontSize = nSize;
	}
	else
	{
		m_font.CreateFont(
			nSize,                        // nHeight
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
			_T("ËÎÌå") );             // lpszFacename
	}
	SetFont(&m_font);
}

void CEditBT::SetTextColor( COLORREF clr )
{
	m_clrText = clr;
}

void CEditBT::SetDefaultText( CString str )
{
	strDefaultText = str;
	SetWindowText(strDefaultText);
}

void CEditBT::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
	CString strNow;
	GetWindowText(strNow);
	if (m_bIsPassword)
	{
		SetPasswordChar('*');
		//ModifyStyleEx(0, ES_PASSWORD);
	}
	if (strNow == strDefaultText)
	{
		SetWindowText(L"");
	}
}

void CEditBT::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
	CString strNow;
	GetWindowText(strNow);
	if (!strNow.GetLength())
	{
		SetPasswordChar(0);
		SetWindowText(strDefaultText);
	}
}

void CEditBT::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	BOOL res = ModifyStyle(WS_BORDER, 0);
	res = ModifyStyleEx(WS_EX_CLIENTEDGE /*| WS_EX_DLGMODALFRAME | WS_EX_STATICEDGE | WS_EX_WINDOWEDGE*/, 0);
	__super::PreSubclassWindow();
}

int CEditBT::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	lpCreateStruct->style &= ~WS_BORDER;
	lpCreateStruct->dwExStyle &= ~WS_EX_CLIENTEDGE;
	return 0;
}

void CEditBT::SetPassword( BOOL mark /*= TRUE*/ )
{
	m_bIsPassword = mark;
}
