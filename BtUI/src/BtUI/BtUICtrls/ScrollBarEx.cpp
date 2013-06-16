/************************************************************************/
/* ScrollBarEx.cpp : implementation file                                */
/************************************************************************/

#include "stdafx.h"
#include "ScrollBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define SCROLLBAR_NORMAL_MINIMUM 45  //经验值,当Scrollbar宽度/高度小于这一数值时需要特殊处理,保证与系统的处理方式一致
/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx

CScrollBarEx::CScrollBarEx()
	: m_pCtrl(NULL)
	, m_barStatu(SCROLL_NORMAL)
{
}

CScrollBarEx::~CScrollBarEx()
{
}


BEGIN_MESSAGE_MAP(CScrollBarEx, CStatic)
//{{AFX_MSG_MAP(CScrollBarEx)
	ON_WM_PAINT()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx message handlers

void CScrollBarEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (m_nScrollStyle == SB_VERT) 
    {
        DrawVScroll(&dc);
    }
    else if(m_nScrollStyle == SB_HORZ)
	{
		DrawHScroll(&dc);
	}
	
	// Do not call CStatic::OnPaint() for painting messages
}

BOOL CScrollBarEx::InitScrollBar(CWnd *pCtrl, UINT nStyle)
{
    if (pCtrl == NULL) 
    {
        return FALSE;
    }
    m_pCtrl = pCtrl;
	
    int nx, ny;
    if (nStyle == SB_VERT) 
    {
        nx = ::GetSystemMetrics(SM_CXVSCROLL);
        ny = ::GetSystemMetrics(SM_CYVSCROLL);
    }
    else if(nStyle == SB_HORZ)
    {
        nx = GetSystemMetrics(SM_CXHSCROLL);
        ny = GetSystemMetrics(SM_CYHSCROLL);
    }
	else
	{
		return FALSE;
	}
    m_nScrollStyle = nStyle;


	CRect rect;
    pCtrl->GetWindowRect(rect);
	pCtrl->ModifyStyle(0, WS_CLIPSIBLINGS);
//	pCtrl->SetWindowPos(&wndBottom, 0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

	CWnd* pParent = pCtrl->GetParent();
    pParent->ScreenToClient(rect);
    
    if (nStyle == SB_VERT) 
    {
        rect.left = rect.right- nx;		
    }
    else if(nStyle == SB_HORZ)
    {
        rect.top = rect.bottom - ny;
    }
	
    BOOL bResult = Create(NULL, WS_CHILD|SS_LEFT|WS_VISIBLE|WS_EX_TOPMOST, rect, pParent);
    if (!bResult)
    {
        return FALSE;
    }
  
	//UpdateScrollBar();
    return TRUE;
}

BOOL CScrollBarEx::InitScrollBar( CWnd *pCtrl, UINT nStyle, UINT uID, HMODULE hModule /*= NULL*/ )
{
	InitScrollBar(pCtrl, nStyle);
	CxImage img;
	img.LoadResource(FindResource(hModule, MAKEINTRESOURCE(uID), L"PNG"), CXIMAGE_FORMAT_PNG, hModule);
	if (nStyle == SB_VERT)
	{
		img.Crop(0, 0, 15, 15, &m_imgTopNormal);
		img.Crop(0, 15, 15, 30, &m_imgTopMovimg);
		img.Crop(0, 30, 15, 45, &m_imgTopPushed);

		img.Crop(15, 0, 30, 15, &m_imgBottomNormal);
		img.Crop(15, 15, 30, 30, &m_imgBottomMovimg);
		img.Crop(15, 30, 30, 45, &m_imgBottomPushed);

		img.Crop(60, 0, 75, 15, &m_imgVNormal);
		img.Crop(60, 15, 75, 30, &m_imgVMovimg);
		img.Crop(60, 30, 75, 45, &m_imgVPushed);

		img.Crop(90, 0, 105, 15, &m_imgVBack);
	}
	else if (nStyle == SB_HORZ)
	{
		img.Crop(30, 0, 45, 15, &m_imgLeftNormal);
		img.Crop(30, 15, 45, 30, &m_imgLeftMovimg);
		img.Crop(30, 30, 45, 45, &m_imgLeftPushed);

		img.Crop(45, 0, 60, 15, &m_imgRightNormal);
		img.Crop(45, 15, 60, 30, &m_imgRightMovimg);
		img.Crop(45, 30, 60, 45, &m_imgRightPushed);

		img.Crop(75, 0, 90, 15, &m_imgHNormal);
		img.Crop(75, 15, 90, 30, &m_imgHMovimg);
		img.Crop(75, 30, 90, 45, &m_imgHPushed);

		img.Crop(105, 0, 120, 15, &m_imgHBack);
	}
	else
	{
		return FALSE;
	}

	UpdateScrollBar();
}

BOOL CScrollBarEx::InitScrollBar( CWnd *pCtrl, UINT nStyle, CxImage& imgScroll )
{

	InitScrollBar(pCtrl, nStyle);
	CxImage img = imgScroll;
	//img.LoadResource(FindResource(hModule, MAKEINTRESOURCE(uID), L"PNG"), CXIMAGE_FORMAT_PNG, hModule);
	if (nStyle == SB_VERT)
	{
		img.Crop(0, 0, 17, 17, &m_imgTopNormal);
		img.Crop(0, 17, 17, 34, &m_imgTopMovimg);
		img.Crop(0, 34, 17, 51, &m_imgTopPushed);

		img.Crop(17, 0, 34, 17, &m_imgBottomNormal);
		img.Crop(17, 17, 34, 30, &m_imgBottomMovimg);
		img.Crop(17, 34, 34, 51, &m_imgBottomPushed);

		img.Crop(68, 0, 85, 17, &m_imgVNormal);
		img.Crop(68, 17, 85, 34, &m_imgVMovimg);
		img.Crop(68, 34, 85, 51, &m_imgVPushed);

		img.Crop(102, 0, 119, 17, &m_imgVBack);
	}
	else if (nStyle == SB_HORZ)
	{
		img.Crop(34, 0, 51, 17, &m_imgLeftNormal);
		img.Crop(34, 17, 51, 34, &m_imgLeftMovimg);
		img.Crop(34, 34, 51, 51, &m_imgLeftPushed);

		img.Crop(51, 0, 68, 17, &m_imgRightNormal);
		img.Crop(51, 17, 68, 34, &m_imgRightMovimg);
		img.Crop(51, 34, 68, 51, &m_imgRightPushed);

		img.Crop(85, 0, 102, 17, &m_imgHNormal);
		img.Crop(85, 17, 102, 34, &m_imgHMovimg);
		img.Crop(85, 34, 102, 51, &m_imgHPushed);

		img.Crop(119, 0, 136, 17, &m_imgHBack);
	}
	else
	{
		return FALSE;
	}

	UpdateScrollBar();
	return TRUE;
}

void CScrollBarEx::UpdateScrollBar()
{
	if(!IsWindow(m_hWnd) || m_pCtrl == NULL)
	{
		return;
	}
	m_pCtrl->SetWindowPos(&wndBottom, 0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);

	SCROLLINFO sf;
	memset(&sf, 0, sizeof(SCROLLINFO));
	sf.cbSize = sizeof(SCROLLINFO);
	m_pCtrl->GetScrollInfo(m_nScrollStyle, &sf, SIF_ALL);
    if (sf.nPage < 0) 
    {
		ShowWindow(SW_HIDE);
        return;
    }

	SCROLLBARINFO hscr;
	memset(&hscr, 0, sizeof(SCROLLBARINFO));
	hscr.cbSize = sizeof(SCROLLBARINFO);
	CRect rcScrollBar;

	if(m_nScrollStyle == SB_VERT)
	{
		::GetScrollBarInfo(m_pCtrl->m_hWnd, OBJID_VSCROLL, &hscr);
		rcScrollBar = CRect(hscr.rcScrollBar);
		//////////////////////////////////////////////////////////////////////////

	}
	else if(m_nScrollStyle == SB_HORZ)
	{
		::GetScrollBarInfo(m_pCtrl->m_hWnd, OBJID_HSCROLL, &hscr);
		rcScrollBar = CRect(hscr.rcScrollBar);
	}

	GetParent()->ScreenToClient(&rcScrollBar);
//	TRACE("rcScrollBar.left = %d, rcScrollBar.top = %d\n", rcScrollBar.left, rcScrollBar.top);

	SetWindowPos(NULL, rcScrollBar.left, rcScrollBar.top, rcScrollBar.Width(), rcScrollBar.Height(), SWP_SHOWWINDOW|SWP_NOZORDER);
	Invalidate();
}

//draw vertical scrollbar
void CScrollBarEx::DrawVScroll(CDC* pDC)
{
	if(m_pCtrl == NULL)
	{
		return;
	}

	int cxButton     = ::GetSystemMetrics(SM_CXVSCROLL);
    int cyButton     = ::GetSystemMetrics(SM_CYVSCROLL);
	
	if(IsWindow(m_hWnd))
	{		
		SCROLLBARINFO vscr;
		memset(&vscr, 0, sizeof(SCROLLBARINFO));
		vscr.cbSize = sizeof(SCROLLBARINFO);
		if( ::GetScrollBarInfo(m_pCtrl->m_hWnd, OBJID_VSCROLL, &vscr) )
		{ 
			CRect rcVScrollBar(vscr.rcScrollBar);	
			
			int iWidth = vscr.dxyLineButton;
			int iThumbTop = vscr.xyThumbTop;
			int iThumbBottom = vscr.xyThumbBottom;

			if(rcVScrollBar.Height() < ::GetSystemMetrics(SM_CYVSCROLL)*2+1)
			{
				cyButton = (rcVScrollBar.Height() - 1)/2; 
			}
			
			CDC cMemDC;
			cMemDC.CreateCompatibleDC(pDC);
			
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, rcVScrollBar.Width(), rcVScrollBar.Height());
			BITMAP bitmap;

			//三态不切换
			switch (m_barStatu)
			{
			case SCROLL_NORMAL:
				{
					
					//The top arrow button
					cMemDC.SelectObject(&bmp);

					m_imgVBack.Draw2(cMemDC.GetSafeHdc(), 0, 0, rcVScrollBar.Width(), rcVScrollBar.Height());
					m_imgTopNormal.Draw2(cMemDC.GetSafeHdc(), 0, 0, cxButton, cyButton);
					
					//The bottom arrow button
					m_imgBottomNormal.Draw2(cMemDC.GetSafeHdc(), 0, rcVScrollBar.Height() - cyButton, cxButton, cyButton);

					if(rcVScrollBar.Height() <= SCROLLBAR_NORMAL_MINIMUM)
					{
						pDC->BitBlt(0, 0, rcVScrollBar.Width(), rcVScrollBar.Height(), &cMemDC, 0, 0, SRCCOPY);
						return;
					}
					CxImage img;
					m_imgVNormal.Crop(0, 0, 17, 7, &img);
					img.Draw(cMemDC.GetSafeHdc(), 0, iThumbTop, iWidth, 7);
					m_imgVNormal.Crop(0, 7, 17, 10, &img);
					img.Draw2(cMemDC.GetSafeHdc(), 0, iThumbTop + 7, iWidth, iThumbBottom - iThumbTop - 2*7);
					m_imgVNormal.Crop(0, 10, 17, 17, &img);
					img.Draw(cMemDC.GetSafeHdc(), 0, iThumbBottom - 7, iWidth, 7);

					pDC->BitBlt(0, 0, rcVScrollBar.Width(), rcVScrollBar.Height(), &cMemDC, 0, 0,/*rcVScrollBar.Width()-2,rcVScrollBar.Height()-4,*/ SRCCOPY);		
				}
				break;
			case SCROLL_MOVING:
				break;
			case SCROLL_PUSHED:
				break;
			default:
				break;
			}
		}
	}


}

//draw horizontal scrollbar
void CScrollBarEx::DrawHScroll(CDC* pDC)
{
	if(m_pCtrl == NULL)
	{
		return;
	}

	int cxButton     = ::GetSystemMetrics(SM_CXHSCROLL);
    int cyButton     = ::GetSystemMetrics(SM_CYHSCROLL);

	if(IsWindow(m_hWnd))
	{		
		SCROLLBARINFO hscr;
		memset(&hscr, 0, sizeof(SCROLLBARINFO));
		hscr.cbSize = sizeof(SCROLLBARINFO);
		if( ::GetScrollBarInfo(m_pCtrl->m_hWnd, OBJID_HSCROLL, &hscr) )
		{ 
			CRect rcHScrollBar(hscr.rcScrollBar);			
			
			int iHeigth = hscr.dxyLineButton;
			int iThumbLeft = hscr.xyThumbTop;
			int iThumbRight = hscr.xyThumbBottom;

			if(rcHScrollBar.Width() < ::GetSystemMetrics(SM_CXHSCROLL)*2+1)
			{
				cxButton = (rcHScrollBar.Width() - 1)/2; 
			}
			
			CDC cMemDC;
			cMemDC.CreateCompatibleDC(pDC);
			
			CBitmap bmp;
			bmp.CreateCompatibleBitmap(pDC, rcHScrollBar.Width(), rcHScrollBar.Height());
			cMemDC.SelectObject(&bmp);

			switch (m_barStatu)
			{
			case SCROLL_NORMAL:
				{

					//The top arrow button
					m_imgHBack.Draw2(cMemDC.GetSafeHdc(), 0, 0, rcHScrollBar.Width(), rcHScrollBar.Height());
					m_imgLeftNormal.Draw2(cMemDC.GetSafeHdc(), 0, 0, cxButton, cyButton);

					//The bottom arrow button
					m_imgRightNormal.Draw2(cMemDC.GetSafeHdc(), rcHScrollBar.Width() - cxButton, 0, cxButton, cyButton);

					if(rcHScrollBar.Width() <= SCROLLBAR_NORMAL_MINIMUM)
					{
						pDC->BitBlt(0, 0, rcHScrollBar.Width(), rcHScrollBar.Height(), &cMemDC, 0, 0, SRCCOPY);
						return;
					}
					CxImage img;
					m_imgHNormal.Crop(0, 0, 7, 17, &img);
					img.Draw2(cMemDC.GetSafeHdc(), iThumbLeft, 0, 7, iHeigth);
					m_imgHNormal.Crop(7, 0, 10, 17, &img);
					img.Draw2(cMemDC.GetSafeHdc(), iThumbLeft + 7, 0, iThumbRight - iThumbLeft- 2*7, iHeigth);
					m_imgHNormal.Crop(10, 0, 17, 17, &img);
					img.Draw2(cMemDC.GetSafeHdc(), iThumbRight - 7, 0, 7, iHeigth);

					pDC->BitBlt(0, 0, rcHScrollBar.Width(), rcHScrollBar.Height(), &cMemDC, 0, 0, SRCCOPY);		
				}
				break;
			case SCROLL_MOVING:
				break;
			case SCROLL_PUSHED:
				break;
			default:
				break;
			}
		}
	}
}