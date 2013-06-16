/************************************************************************/
/* RadioButton.h  implementation file                                   */
/************************************************************************/


#include "stdafx.h"
#include "RadioButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRadioButton
CRadioButton::CRadioButton()
{
	hClipRgn=NULL;			// no clipping region
	m_TextColor=GetSysColor(COLOR_BTNTEXT); // default button text color
	m_button_down = m_tracking = m_Checked = false;
}
/////////////////////////////////////////////////////////////////////////////
CRadioButton::~CRadioButton()
{
	if (hClipRgn) 
	{
		DeleteObject(hClipRgn);	// free clip region
	}
}
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CRadioButton, CButton)
	//{{AFX_MSG_MAP(CRadioButton)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_CXSHADE_RADIO , OnRadioInfo)
	ON_MESSAGE(BM_SETCHECK , OnBMSetCheck)
	ON_MESSAGE(BM_GETCHECK , OnBMGetCheck)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CRadioButton message handlers
/////////////////////////////////////////////////////////////////////////////
void CRadioButton::PreSubclassWindow() 
{
	m_Style=GetButtonStyle();	///get specific BS_ styles
	if ((m_Style & BS_AUTOCHECKBOX)==BS_AUTOCHECKBOX)
//		||((m_Style & BS_CHECKBOX)==BS_CHECKBOX))
		m_Style=BS_CHECKBOX;
	else if ((m_Style & BS_AUTORADIOBUTTON)==BS_AUTORADIOBUTTON)
//			||((m_Style & BS_RADIOBUTTON)==BS_RADIOBUTTON))
		m_Style=BS_RADIOBUTTON;
	else { m_Style=BS_PUSHBUTTON; }

	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CRadioButton::OnEraseBkgnd(CDC* pDC) 
{	
	return 1; 
}	// doesn't erase the button background
/////////////////////////////////////////////////////////////////////////////
void CRadioButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    ASSERT (lpDrawItemStruct);
    //TRACE("* Captured: %08X\n", ::GetCapture());

    //Check if the button state in not in inconsistent mode...
    POINT mouse_position;
    if ((m_button_down) && (::GetCapture() == m_hWnd) && (::GetCursorPos(&mouse_position)))
	{
		if (::WindowFromPoint(mouse_position) == m_hWnd)
		{
			if ((GetState() & BST_PUSHED) != BST_PUSHED) 
			{
				//TRACE("* Inconsistency up detected! Fixing.\n");
				SetState(TRUE);
				return;
			}
		}
		else 
		{
			if ((GetState() & BST_PUSHED) == BST_PUSHED) 
			{
				//TRACE("* Inconsistency up detected! Fixing.\n");
				SetState(FALSE);
				return;
			}
		}
	}
	
    //TRACE("* Drawing: %08x\n", lpDrawItemStruct->itemState);
	CString sCaption;
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	// get device context
	//RECT r=lpDrawItemStruct->rcItem;					// context rectangle
	CRect rc(lpDrawItemStruct->rcItem);
	int cx = rc.Width();						// get width
	int cy = rc.Height();						// get height
	// get text box position
	//RECT tr={r.left+m_FocusRectMargin+2,r.top,r.right-m_FocusRectMargin-2,r.bottom};

	GetWindowText(sCaption);							// get button text
	pDC->SetBkMode(TRANSPARENT);

	if (m_dcBk.m_hDC == NULL)
	{
		CRect rect1;
		CClientDC clDC(GetParent());
		GetWindowRect(rect1);
		GetParent()->ScreenToClient(rect1);

		CBitmap bmp;
		m_dcBk.CreateCompatibleDC(&clDC);
		bmp.CreateCompatibleBitmap(&clDC, rc.Width(), rc.Height());
		m_dcBk.SelectObject(&bmp);
		m_dcBk.BitBlt(0, 0, rc.Width(), rc.Height(), &clDC, rect1.left, rect1.top, SRCCOPY);
		bmp.DeleteObject();
	}
	// Select the correct skin 
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
	{	// DISABLED BUTTON
		if (!m_imgDisable.IsValid())
		{
			 // no skin selected for disabled state -> standard button
			pDC->FillSolidRect(rc, GetSysColor(COLOR_BTNFACE));
		}
		else // paint the skin
		{
			DrawImg(pDC, m_imgDisable, rc);
		}

		// paint the etched button text
		pDC->SetTextColor(GetSysColor(COLOR_3DHILIGHT));
		
		//pDC->DrawText(sCaption,&tr,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		//pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		//OffsetRect(&tr,-1,-1);
		//pDC->DrawText(sCaption,&tr,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	} 
	else 
	{										// SELECTED (DOWN) BUTTON
		if ((lpDrawItemStruct->itemState & ODS_SELECTED) || m_Checked)
		{
			if(!m_imgDown.IsValid())
			{
				 // no skin selected for selected state -> standard button
				pDC->FillSolidRect(rc, GetSysColor(COLOR_BTNFACE));
			}
            else 
			{ // paint the skin
				DrawImg(pDC, m_imgDown, rc);
            }
		}
		else 
		{											// DEFAULT BUTTON
			if(!m_imgNormal.IsValid())
			{
				 // no skin selected for normal state -> standard button
				pDC->FillSolidRect(rc, GetSysColor(COLOR_BTNFACE));
			}
			else // paint the skin
                if ((m_tracking)&&(m_imgMove.IsValid()))
				{
					//DrawBitmap(pDC,m_imgMove.MakeBitmap(), r,m_DrawMode);
					DrawImg(pDC, m_imgMove, rc);
				}
				else 
				{
					DrawImg(pDC, m_imgNormal, rc);
				}
		}

		// paint the enabled button text
		//pDC->SetTextColor(m_TextColor);
		//pDC->DrawText(sCaption,&tr,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	}
}

/////////////////////////////////////////////////////////////////////////////
// void CRadioButton::DrawBitmap(CDC* dc, HBITMAP hbmp, RECT r, int DrawMode)
// {
// //	DrawMode: 0=Normal; 1=stretch; 2=tiled fill
// 	if(DrawMode==2){
// 		FillWithBitmap(dc,hbmp,r);
// 		return;
// 	}
// 	if(!hbmp) return;	//safe check
// 
// 	int cx=r.right  - r.left;
// 	int cy=r.bottom - r.top;
// 	CDC dcBmp,dcMask;
// 	dcBmp.CreateCompatibleDC(dc);
// 	HBITMAP hOldBitmap = (HBITMAP)dcBmp.SelectObject(hbmp);
// 	
// 	if (m_imgMask.IsValid()){
// 		dcMask.CreateCompatibleDC(dc);
// 		HBITMAP hOldBitmap1 = (HBITMAP)dcMask.SelectObject(m_imgMask.MakeBitmap());
// 
// 		CDC hdcMem;
// 		hdcMem.CreateCompatibleDC(dc);
// 	    CBitmap hBitmap;
// 		hBitmap.CreateCompatibleBitmap(dc,cx,cy);
// 	    HBITMAP hOldBitmap2 = (HBITMAP)hdcMem.SelectObject(hBitmap);
// 		
//         hdcMem.BitBlt(r.left,r.top,cx,cy,dc,0,0,SRCCOPY);
// 		if(!DrawMode){
// 			hdcMem.BitBlt(r.left,r.top,cx,cy,&dcBmp,0,0,SRCINVERT);
// 			hdcMem.BitBlt(r.left,r.top,cx,cy,&dcMask,0,0,SRCAND);
// 			hdcMem.BitBlt(r.left,r.top,cx,cy,&dcBmp,0,0,SRCINVERT);
// 		} else {
// 			int bx=GetBitmapWidth(hbmp);
// 			int by=GetBitmapHeight(hbmp);
// 			hdcMem.StretchBlt(r.left,r.top,cx,cy,&dcBmp,0,0,bx,by,SRCINVERT);
// 			hdcMem.StretchBlt(r.left,r.top,cx,cy,&dcMask,0,0,bx,by,SRCAND);
// 			hdcMem.StretchBlt(r.left,r.top,cx,cy,&dcBmp,0,0,bx,by,SRCINVERT);
// 		}
//         dc->BitBlt(r.left,r.top,cx,cy,&hdcMem,0,0,SRCCOPY);
// 
// 		hdcMem.SelectObject( hOldBitmap2 );
// 		hdcMem.DeleteDC();
// 		hBitmap.DeleteObject();
// 
// 		dcMask.SelectObject( hOldBitmap1 );
// 		dcMask.DeleteDC();
// 	} else {
// 		if(!DrawMode){
// 			dc->BitBlt(r.left,r.top,cx,cy,&dcBmp,0,0,SRCCOPY);
// 		} else {
// 			int bx=GetBitmapWidth(hbmp);
// 			int by=GetBitmapHeight(hbmp);
// 			dc->StretchBlt(r.left,r.top,cx,cy,&dcBmp,0,0,bx,by,SRCCOPY);
// 		}
// 	}
// 
// 	dcBmp.SelectObject( hOldBitmap );
// 	dcBmp.DeleteDC();
// }

/////////////////////////////////////////////////////////////////////////////
void CRadioButton::SetSkin(UINT normal, UINT down, UINT over, UINT disabled, HMODULE hInst)
{
	m_imgNormal.LoadResource(FindResource(hInst, MAKEINTRESOURCE(normal), L"PNG"), CXIMAGE_FORMAT_PNG, hInst);
	m_imgDown.LoadResource(FindResource(hInst, MAKEINTRESOURCE(down), L"PNG"), CXIMAGE_FORMAT_PNG, hInst);
	m_imgMove.LoadResource(FindResource(hInst, MAKEINTRESOURCE(over), L"PNG"), CXIMAGE_FORMAT_PNG, hInst);
	m_imgDisable.LoadResource(FindResource(hInst, MAKEINTRESOURCE(disabled), L"PNG"), CXIMAGE_FORMAT_PNG, hInst);
}

void CRadioButton::SetSkin(LPCTSTR pNoraml, LPCTSTR pDown, LPCTSTR pOver, LPCTSTR pDisabled, HMODULE hInst)
{
	m_imgNormal.LoadResource(FindResource(hInst, pNoraml, L"PNG"), CXIMAGE_FORMAT_PNG, hInst);
	m_imgDown.LoadResource(FindResource(hInst, pDown, L"PNG"), CXIMAGE_FORMAT_PNG, hInst);
	m_imgMove.LoadResource(FindResource(hInst, pOver, L"PNG"), CXIMAGE_FORMAT_PNG, hInst);
	m_imgDisable.LoadResource(FindResource(hInst, pDisabled, L"PNG"), CXIMAGE_FORMAT_PNG, hInst);
}

/////////////////////////////////////////////////////////////////////////////
COLORREF CRadioButton::SetTextColor(COLORREF new_color)
{
	COLORREF tmp_color = m_TextColor;
	m_TextColor = new_color;
	return tmp_color;			//returns the previous color
}
/////////////////////////////////////////////////////////////////////////////
void CRadioButton::SetToolTipText(CString s)
{
	if(m_tooltip.m_hWnd==NULL)
	{
		if(m_tooltip.Create(this))	//first assignment
		{
			if(m_tooltip.AddTool(this, (LPCTSTR)s))
			{
				m_tooltip.Activate(1);
			}
		}
	} 
	else
	{
		m_tooltip.UpdateTipText((LPCTSTR)s,this);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CRadioButton::SetToolTipColor(COLORREF crText, COLORREF crBkgnd)
{
    if(m_tooltip.m_hWnd != NULL)
    {
		m_tooltip.SetTipTextColor(crText);
        m_tooltip.SetTipBkColor(crBkgnd);
    }
}
/////////////////////////////////////////////////////////////////////////////
void CRadioButton::RelayEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
// This function will create a MSG structure, fill it in a pass it to
// the ToolTip control, m_ttip.  Note that we ensure the point is in window
// coordinates (relative to the control's window).
	if(NULL != m_tooltip.m_hWnd){
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.time = 0;
		msg.pt.x = LOWORD(lParam);
		msg.pt.y = HIWORD(lParam);

		m_tooltip.RelayEvent(&msg);
	}
}


/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnLButtonDblClk
//Class.......: CRadioButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: Used only to be forwarded as WM_LBUTTONDOWN message parameters
//Exceptions..: NONE
//------------
//Description :
//
//  > We do not care about doublelicks - handle this event
//    like an ordinary left-button-down event
//
//---------------------------------------------------------
void CRadioButton::OnLButtonDblClk(UINT flags, CPoint point) 
{
    SendMessage(WM_LBUTTONDOWN, flags, MAKELPARAM(point.x, point.y));
}


/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnLButtonDown
//Class.......: CRadioButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle event when left button is pressed down
//
//---------------------------------------------------------
void CRadioButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: down\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	RelayEvent(WM_LBUTTONDOWN,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //If we are tracking this button, cancel it
    if (m_tracking) {
        TRACKMOUSEEVENT t = {
            sizeof(TRACKMOUSEEVENT),
            TME_CANCEL | TME_LEAVE,
            m_hWnd,
            0
        };
        if (::_TrackMouseEvent(&t)) {
            m_tracking = false;
        }
    }

    //Default-process the message
	CButton::OnLButtonDown(nFlags, point);
    m_button_down = true;
}


/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnLButtonUp
//Class.......: CRadioButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle event when left button is released (goes up)
//
//---------------------------------------------------------
void CRadioButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: up\n", ::GetTickCount());

	if (m_Style){ //track mouse for radio & check buttons
		POINT p2 = point;
		::ClientToScreen(m_hWnd, &p2);
		HWND mouse_wnd = ::WindowFromPoint(p2);
		if (mouse_wnd == m_hWnd){ // mouse is in button
			if (m_Style==BS_CHECKBOX) SetCheck(m_Checked ? 0 : 1);
			if (m_Style==BS_RADIOBUTTON) SetCheck(1);
		}
	}
	//Pass this message to the ToolTip control
	RelayEvent(WM_LBUTTONUP,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //Default-process the message
    m_button_down = false;
	CButton::OnLButtonUp(nFlags, point);
}


/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnMouseMove
//Class.......: CRadioButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: As follows
//    > [in] nFlags: not used
//    > [in] point: coordinates of the mouse pointer when this event was spawned
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle change of mouse position: see the comments in the
//    method for further info.
//
//---------------------------------------------------------
void CRadioButton::OnMouseMove(UINT nFlags, CPoint point)
{
    //TRACE("* %08X: Mouse\n", ::GetTickCount());

	//Pass this message to the ToolTip control
	RelayEvent(WM_MOUSEMOVE,(WPARAM)nFlags,MAKELPARAM(LOWORD(point.x),LOWORD(point.y)));

    //If we are in capture mode, button has been pressed down
    //recently and not yet released - therefore check is we are
    //actually over the button or somewhere else. If the mouse
    //position changed considerably (e.g. we moved mouse pointer
    //from the button to some other place outside button area)
    //force the control to redraw
    //
    if ((m_button_down) && (::GetCapture() == m_hWnd)) {
	    POINT p2 = point;
        ::ClientToScreen(m_hWnd, &p2);
        HWND mouse_wnd = ::WindowFromPoint(p2);

        bool pressed = ((GetState() & BST_PUSHED) == BST_PUSHED);
        bool need_pressed = (mouse_wnd == m_hWnd);
        if (pressed != need_pressed) {
            //TRACE("* %08X Redraw\n", GetTickCount());
            SetState(need_pressed ? TRUE : FALSE);
            Invalidate();
        }
    } else {

	//Otherwise the button is released. That means we should
    //know when we leave its area - and so if we are not tracking
    //this mouse leave event yet, start now!
    //
        if (!m_tracking) {
            TRACKMOUSEEVENT t = {
                sizeof(TRACKMOUSEEVENT),
                TME_LEAVE,
                m_hWnd,
                0
            };
            if (::_TrackMouseEvent(&t)) {
                //TRACE("* Mouse enter\n");
                m_tracking = true;
                Invalidate();
            }
        }
    }

    //Forward this event to superclass
    CButton::OnMouseMove(nFlags, point);
}


/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnMouseLeave
//Class.......: CRadioButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NULL
//Parameters..: NOT USED
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle situation when mouse cursor leaves area of this
//    window (button). This event might be generated ONLY
//    if we explicitely call 'TrackMouseEvent'. This is
//    signalled by setting the m_tracking flag (see the assert
//    precondition) - in 'OnMouseMove' method
//
//  > When a mouse pointer leaves area of this button (i.e.
//    when this method is invoked), presumably the look of
//    the button changes (e.g. when hover/non-hover images are set)
//    and therefore we force the control to redraw.
//
//---------------------------------------------------------
LRESULT CRadioButton::OnMouseLeave(WPARAM, LPARAM)
{
    ASSERT (m_tracking);
    //TRACE("* Mouse leave\n");
    m_tracking = false;
    Invalidate();
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnKillFocus
//Class.......: CRadioButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: NONE
//Parameters..: See superclass documentation
//Exceptions..: NONE
//------------
//Description :
//
//  > If focus is killed during capture, we may no longer
//    have the exclusive access to user input and therefore
//    release it.
//
//  > Such a situation might happens when the user left-clicks
//    this button, keeps the button down and simultaneously
//    presses TAB key.
//
//---------------------------------------------------------
void CRadioButton::OnKillFocus(CWnd *new_wnd)
{
    if (::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
        ASSERT (!m_tracking);
        m_button_down = false;
    }
    CButton::OnKillFocus(new_wnd);
}


/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnClicked
//Class.......: CRadioButton
//
//Author......: Milan Gardian
//Created.....: MAR-2001
//
//Return value: FALSE (do not stop in this handler - forward to parent)
//Parameters..: NONE
//Exceptions..: NONE
//------------
//Description :
//
//  > Keep consistency of attributes of this instance before
//    submitting click event to the parent.
//
//  > Currently NOT used. To use, umcomment line
//    "ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)" in message map
//    at the beginning of this file.
//
//---------------------------------------------------------
BOOL CRadioButton::OnClicked() 
{
    if (::GetCapture() == m_hWnd) {
        ::ReleaseCapture();
        ASSERT (!m_tracking);
    }
    m_button_down = false;
    //Invalidate();
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
//Method......: OnRadioInfo
//Class.......: CRadioButton
//
//Author......: Rainer Mangold
//Created.....: JUN-2001
//
//Return value: NULL
//Parameters..: WPARAM and LPARAM (LPARAM not used)
//Exceptions..: NONE
//------------
//Description :
//
//  > Handle notification, that a Button in the same group was pushed
//
//---------------------------------------------------------
LRESULT CRadioButton::OnRadioInfo(WPARAM wparam, LPARAM)
{
	if (m_Checked){	//only checked buttons need to be unchecked
		m_Checked = false;
		Invalidate();
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
void CRadioButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((m_Style)&&(nChar==' ')){ //needed stuff for check & radio buttons
		if (m_Style==BS_CHECKBOX) SetCheck(m_Checked ? 0 : 1);
		if (m_Style==BS_RADIOBUTTON) SetCheck(1);
	}
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}
/////////////////////////////////////////////////////////////////////////////
//
//Method......: SetCheck
//Class.......: CRadioButton
//
//Author......: Rainer Mangold
//Created.....: JUN-2001
//
//Return value: NONE
//Parameters..: bool
//Exceptions..: NONE
//------------
//Description :
//
//  > Set the state of this button (pushed or not). 
//    Works for both, Radio and CheckBox - Buttons
//
//---------------------------------------------------------
LRESULT CRadioButton::OnBMSetCheck(WPARAM wparam, LPARAM)
{
	m_Checked=wparam!=0;
	switch (m_Style)
	{
	case BS_RADIOBUTTON:
		if (m_Checked) { //uncheck the other radio buttons (in the same group)
			HWND hthis,hwnd2,hpwnd;
			hpwnd=GetParent()->GetSafeHwnd();	//get button parent handle
			hwnd2=hthis=GetSafeHwnd();			//get this button handle
			if (hthis && hpwnd){				//consistency check
				for( ; ; ){	//scan the buttons within the group
					hwnd2=::GetNextDlgGroupItem(hpwnd,hwnd2,0);
					//until we reach again this button
					if ((hwnd2==hthis)||(hwnd2==NULL)) break;
					//post the uncheck message
					::PostMessage(hwnd2, WM_CXSHADE_RADIO, 0, 0);
				}
			}
		}
		break;
	case BS_PUSHBUTTON:
		m_Checked=false;
		ASSERT(false); // Must be a Check or Radio button to use this function
	}

	Invalidate();
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
LRESULT CRadioButton::OnBMGetCheck(WPARAM wparam, LPARAM)
{
	return m_Checked; 
}	//returns the state for check & radio buttons

void CRadioButton::UpdateCtrl( BYTE* buf )
{
	
}

void CRadioButton::DrawImg( CDC* dc, CxImage img, CRect& rc )
{
 	CDC memdc;
 	memdc.CreateCompatibleDC(dc);
 	CBitmap bmp;
 	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
 	memdc.SelectObject(&bmp);
	memdc.BitBlt(0, 0, rc.Width(), rc.Height(), &m_dcBk, 0, 0, SRCCOPY);
 	img.Draw(memdc.GetSafeHdc(), 0, 0);
	//m_dcBk.BitBlt(0, 0, rc.Width(), rc.Height(), &memdc, 0, 0, SRCCOPY);
	dc->BitBlt(0, 0, rc.Width(), rc.Height(), &memdc, 0, 0, SRCCOPY);
}

/////////////////////////////////////////////////////////////////////////////
//EOF
