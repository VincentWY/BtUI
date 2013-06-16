/************************************************************************/
/* HeaderCtrlCl.cpp                                                     */
/************************************************************************/


#include "stdafx.h"
#include "HeaderCtrlBT.h"


// CHeaderCtrlBT

IMPLEMENT_DYNAMIC(CHeaderCtrlBT, CHeaderCtrl)

CHeaderCtrlBT::CHeaderCtrlBT()
	: m_Height(1.0)
	, m_hResHandle(NULL)
	, m_nFontSize(20)
	, m_clrLine(RGB(36,36,36))
	, m_clrText(RGB(0, 0, 0))
{
	m_font.CreateFont(
		m_nFontSize,                // nHeight
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
		_T("微软雅黑") );              // lpszFacename
	m_pen.CreatePen(PS_SOLID, 1, m_clrLine);
}

CHeaderCtrlBT::~CHeaderCtrlBT()
{
}


BEGIN_MESSAGE_MAP(CHeaderCtrlBT, CHeaderCtrl)
	ON_WM_PAINT()
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
END_MESSAGE_MAP()



// CHeaderCtrlBT message handle



void CHeaderCtrlBT::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDC memDC;
	CRect rc;
	GetClientRect(rc);
	memDC.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	memDC.SelectObject(&bmp);
	memDC.SelectObject(&m_font);
	memDC.SelectObject(&m_pen);
	memDC.SetTextColor(m_clrText);
	memDC.SetBkMode(TRANSPARENT);
	memDC.FillSolidRect(0, 0, rc.Width(), rc.Height(), RGB(200, 200, 200));
	m_imgBK.Draw2(memDC.GetSafeHdc(), 0, 0, rc.Width(), rc.Height());
	int nItem; 
	nItem = GetItemCount();//得到有几个单元，绘制分割线
	for(int i = 0; i < nItem; i++) 
	{ 
		CRect tRect;
		GetItemRect(i, &tRect);//得到Item的尺寸
		HDITEM hdi;
		TCHAR  lpBuffer[256];
		bool   fFound = false;

		hdi.mask = HDI_TEXT;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = 256;

		GetItem(i, &hdi);
                if (i < nItem - 1)
		{
			CPen penLine;
			penLine.CreatePen(PS_SOLID, 1, m_clrLine);
			CPen* oldLinePen = memDC.SelectObject(&penLine);
			memDC.MoveTo(tRect.right, 0);
			memDC.LineTo(tRect.right, tRect.bottom);
			memDC.SelectObject(oldLinePen);
		}


		UINT nFormat = DT_CENTER;

		memDC.DrawText(hdi.pszText, &tRect, nFormat | DT_SINGLELINE | DT_VCENTER);
	}
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);

}

LRESULT CHeaderCtrlBT::OnLayout( WPARAM wParam, LPARAM lParam )
{
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam); 
	HD_LAYOUT &hdl = *( HD_LAYOUT * ) lParam; 
	RECT *prc = hdl.prc; 
	WINDOWPOS *pwpos = hdl.pwpos; 

	//表头高度为原来1.5倍，如果要动态修改表头高度的话，将1.5设成一个全局变量 
	int nHeight = (int)(pwpos->cy * m_Height);
	pwpos->cy = nHeight; 
	prc->top = nHeight; 
	return lResult; 
}

void CHeaderCtrlBT::SetHeadBackImg( CxImage& img )
{
	m_imgBK = img;
}

void CHeaderCtrlBT::SetHeadBackImg( LPCTSTR lpszImg )
{
	m_imgBK.LoadResource(FindResource(m_hResHandle, lpszImg, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
}

void CHeaderCtrlBT::SetHeadBackImg( UINT nID )
{
	m_imgBK.LoadResource(FindResource(m_hResHandle, MAKEINTRESOURCE(nID), L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
}

void CHeaderCtrlBT::SetHeadHeight( float nHeightTime /*= 1.0*/ )
{
	m_Height = nHeightTime;
}

void CHeaderCtrlBT::SetFontNum( int nNum )
{
	m_nFontSize = nNum;
	m_font.DeleteObject();
	m_font.CreateFont(
		m_nFontSize,                // nHeight
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
		_T("微软雅黑") );              // lpszFacename
}

void CHeaderCtrlBT::SetTextColor( COLORREF textclr /*= RGB(0,0,0)*/ )
{
	m_clrText = textclr;
}

void CHeaderCtrlBT::SetLineColor( COLORREF lineclr /*= RGB(0,0,0)*/ )
{
	m_clrLine = lineclr;
}
