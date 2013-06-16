/************************************************************************/
/* ListCtrlCl.cpp                                                       */
/************************************************************************/


#include "stdafx.h"
#include "ListCtrlBT.h"
#include "MemDCEx.h"

struct stColor
{
	int nRow;
	int nCol;
	COLORREF rgb;
};
// CListCtrlBT

IMPLEMENT_DYNAMIC(CListCtrlBT, CListCtrl)

CListCtrlBT::CListCtrlBT()
	: m_nRowHeight(25)
	, m_hResHandle(NULL)
	, m_rowClr1(RGB(128, 128, 128))
	, m_rowClr2(RGB(25, 25, 25))
	, m_TextClr(RGB(0, 0, 0))
	, m_LineClr(RGB(36, 36, 36))
	, m_SelClr(RGB(0, 0, 160))
	, m_TextClrSel(RGB(255, 255, 255))
	, m_nFontSize(20)
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
}

CListCtrlBT::~CListCtrlBT()
{
}


BEGIN_MESSAGE_MAP(CListCtrlBT, CListCtrl)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



// CListCtrlBT message handle

void CListCtrlBT::PreSubclassWindow()
{
	ModifyStyle(0, LVS_OWNERDRAWFIXED);
	CListCtrl::PreSubclassWindow();
	CHeaderCtrl *pHeader = GetHeaderCtrl();
	m_Header.SubclassWindow(pHeader->GetSafeHwnd());
}

void CListCtrlBT::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	TCHAR lpBuffer[256];

	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT | LVIF_PARAM ;
	lvi.iItem = lpDrawItemStruct->itemID ; 
	lvi.iSubItem = 0;
	lvi.pszText = lpBuffer ;
	lvi.cchTextMax = sizeof(lpBuffer);
	VERIFY(GetItem(&lvi));

	LV_COLUMN lvc, lvcprev ;
	::ZeroMemory(&lvc, sizeof(lvc));
	::ZeroMemory(&lvcprev, sizeof(lvcprev));
	lvc.mask = LVCF_WIDTH | LVCF_FMT;
	lvcprev.mask = LVCF_WIDTH | LVCF_FMT;

	CDC* pDC;
	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	//CDC memdc;
	//memdc.CreateCompatibleDC(pDC);
	//CBitmap bmp;
	CRect rtClient;
	GetClientRect(&rtClient);
	
	pDC->SelectObject(&m_font);

	for ( int nCol=0; GetColumn(nCol, &lvc); nCol++)
	{
		if ( nCol > 0 ) 
		{
			// Get Previous Column Width in order to move the next display item
			GetColumn(nCol-1, &lvcprev) ;
			lpDrawItemStruct->rcItem.left += lvcprev.cx ;
			lpDrawItemStruct->rcItem.right += lpDrawItemStruct->rcItem.left; 
		}

		CRect rcItem;   
		if (!GetSubItemRect(lpDrawItemStruct->itemID,nCol,LVIR_LABEL,rcItem))   
			continue;   

		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.iItem = lpDrawItemStruct->itemID;
		lvi.mask = LVIF_TEXT | LVIF_PARAM;
		lvi.iSubItem = nCol;
		lvi.pszText = lpBuffer;
		lvi.cchTextMax = sizeof(lpBuffer);
		VERIFY(GetItem(&lvi));
		CRect rcTemp;
		rcTemp = rcItem;

		if (nCol==0)
		{
			rcTemp.left -= 4;
		}

		if ( lpDrawItemStruct->itemState & ODS_SELECTED )
		{
			pDC->FillSolidRect(&rcTemp, m_SelClr);
			pDC->SetTextColor(m_TextClrSel);
		}
		else
		{
			if (lpDrawItemStruct->itemID % 2)
			{
				pDC->FillSolidRect(rcTemp, m_rowClr1);
			}
			else
			{
				pDC->FillSolidRect(rcTemp, m_rowClr2);
			}

			pDC->SetTextColor(m_TextClr);
		}

		pDC->DrawText(lpBuffer, &rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
}

void CListCtrlBT::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nRowHeight>0)
	{
		lpMeasureItemStruct->itemHeight = m_nRowHeight;
	}
}

// 设置表头高度
void CListCtrlBT::SetHeaderHeight(float Height) //设置表头高度
{
	m_Header.SetHeadHeight(Height);
}

void CListCtrlBT::SetRowHeigt(int nHeight) //高置行高
{
	m_nRowHeight = nHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rcWin.Width();
	wp.cy = rcWin.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp);
}

void CListCtrlBT::SetHeaderTextColor(COLORREF color) //设置头部字体颜色
{
	m_Header.SetTextColor(color);
}


void CListCtrlBT::SetTextColor( COLORREF clr, COLORREF clrsel )
{
	m_TextClr = clr;
	m_TextClrSel = clrsel;
}

void CListCtrlBT::SetHeadBK( LPCTSTR lpszImg )
{
	m_Header.SetHeadBackImg(lpszImg);
}

void CListCtrlBT::SetBKImg( LPCTSTR str )
{
	m_imgBK.LoadResource(FindResource(m_hResHandle, str, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
}

void CListCtrlBT::SetRowColor( COLORREF clr1, COLORREF clr2 )
{
	m_rowClr1 = clr1;
	m_rowClr2 = clr2;
}

void CListCtrlBT::SetHeaderLineColor( COLORREF color )
{
	m_Header.SetLineColor(color);
}

void CListCtrlBT::SetLineColor( COLORREF clr )
{
	m_LineClr = clr;
}

void CListCtrlBT::SetHeaderFontSize( int size )
{
	m_Header.SetFontNum(size);
}

void CListCtrlBT::SetFontSize( int size )
{
	m_font.DeleteObject();
	m_nFontSize = size;
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

void CListCtrlBT::OnPaint()
{
	CListCtrl::OnPaint();

	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();
}

BOOL CListCtrlBT::OnEraseBkgnd(CDC* pDC)
{
	return CListCtrl::OnEraseBkgnd(pDC);
}

void CListCtrlBT::SetSelColor( COLORREF clr )
{
	m_SelClr = clr;
}

void CListCtrlBT::SetScrollBarImg( LPCTSTR strimg )
{
	m_imgScrollBar.LoadResource(FindResource(m_hResHandle, strimg, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	m_VScroll.InitScrollBar(this, SB_VERT, m_imgScrollBar);
	m_HScroll.InitScrollBar(this, SB_HORZ, m_imgScrollBar);
}

void CListCtrlBT::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CListCtrl::OnShowWindow(bShow, nStatus);

	m_HScroll.ShowWindow(bShow);
	m_VScroll.ShowWindow(bShow);
}
