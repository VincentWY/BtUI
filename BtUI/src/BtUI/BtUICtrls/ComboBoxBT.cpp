/************************************************************************/
/* ComboBoxBT.cpp : implementation file                                 */
/************************************************************************/
#include "stdafx.h"
#include "ComboBoxBT.h"
#include "MemDCEx.h"


// CComboBoxBT

IMPLEMENT_DYNAMIC(CComboBoxBT, CComboBox)

CComboBoxBT::CComboBoxBT()
	: m_strSel(L"")
	, m_clrText(RGB(255, 255, 255))
{
	m_font.CreateFont(
		18,                        // nHeight
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

CComboBoxBT::~CComboBoxBT()
{
}


BEGIN_MESSAGE_MAP(CComboBoxBT, CComboBox)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CComboBoxBT::SetBKImg( LPCTSTR str )
{
	m_imgBK.LoadResource(FindResource(NULL, str, L"PNG"), CXIMAGE_FORMAT_PNG);
	SetItemHeight(-1, m_imgBK.GetHeight() - 6);
	SetItemHeight(0, m_imgBK.GetHeight() - 6);
	SetFont(&m_font);
}

void CComboBoxBT::OnPaint()
{
	CPaintDC dc(this);
	//CPaintDC subdc(m_pane);
	CMemDCEx memdc(&dc, this);
	memdc.SetBkMode(TRANSPARENT);
	m_imgBK.Draw(memdc.GetSafeHdc());
	CRect rc;
	GetClientRect(rc);
	rc.left += 5;
	rc.right -= rc.Height();
	memdc.SelectObject(&m_font);
	int index = GetCurSel();
	if (index != CB_ERR)
	{
		GetLBText(index, m_strSel);
	}
	memdc.SetTextColor(m_clrText);
	memdc.DrawText(m_strSel, rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

}

void CComboBoxBT::SetTextColor( COLORREF clr )
{
	m_clrText = clr;
}

void CComboBoxBT::SetDefaultText( CString str )
{
	m_strSel = str;
}
