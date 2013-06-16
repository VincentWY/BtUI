/************************************************************************/
/*ButtonBT.cpp                                                          */
/************************************************************************/

#include "stdafx.h"
#include "ButtonBT.h"
#include "MemDCEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonBT

CButtonBT::CButtonBT()
{
	m_bHaveBitmaps = FALSE;
	m_bHaveAltImage = FALSE;

	m_pCurBtn = NULL;

	m_bIsDisabled = FALSE;
	m_bIsToggle = FALSE;

	m_bIsHovering = FALSE;
	m_bIsTracking = FALSE;

	m_nCurType = STD_TYPE;

	m_pToolTip = NULL;

	m_bISFrame = FALSE;
	m_bISMove = TRUE;
	m_bISHot = TRUE;
	m_bISTextSelf = FALSE;

	m_nFontSize = 18;
	m_font.CreateFont(
		m_nFontSize,               // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Î¢ÈíÑÅºÚ"));                 // lpszFacename
}

CButtonBT::~CButtonBT()
{
	if(m_pToolTip)	delete m_pToolTip;
}


BEGIN_MESSAGE_MAP(CButtonBT, CButton)
	//{{AFX_MSG_MAP(CButtonBT)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



//=============================================================================
//
// LoadStdImage()
//
// Purpose:     The LoadStdImage() Loads the image for the button.  This 
//				function must be called at a minimum or the button wont do 
//				anything.
//
// Parameters:  
//		[IN]	id
//				resource id, one of the resources already imported with the 
//				resource editor, usually begins with IDR_  
//
//		[IN]	pType
//				pointer to string describing the resource type
//				
// Returns:     BOOL
//				Non zero if successful, otherwise zero
//
//=============================================================================
BOOL CButtonBT::LoadStdImage(UINT id, LPCTSTR pType)
{
	if (m_imgStd.LoadResource(FindResource(NULL, MAKEINTRESOURCE(id), L"PNG"), CXIMAGE_FORMAT_PNG))
	{
		return TRUE;
	}
	
// 	m_pStdImage = new CGdiPlusBitmapResource;
// 	return m_pStdImage->Load(id, pType);
	return FALSE;
}


//=============================================================================
//
// LoadStdImage()
//
// Purpose:     The LoadStdImage() Loads the image for the button.  
//				Add by WuYong at 2011-08-29
//
// Parameters:  
//		[IN]	hInst
//				resource instance handle
//
//		[IN]	pName
//				pointer to the string describing the resource name
//
//		[IN]	pType
//				pointer to string describing the resource type
//				
// Returns:     BOOL
//				Non zero if successful, otherwise zero
//
//=============================================================================
BOOL CButtonBT::LoadStdImage(LPCTSTR pName, HMODULE hInst)
{
	if (m_imgStd.LoadResource(FindResource(hInst, pName, L"PNG"), CXIMAGE_FORMAT_PNG), hInst)
	{
		return TRUE;
	}
	return FALSE;
// 	m_pStdImage = new CGdiPlusBitmapResource;
// 	return m_pStdImage->Load(pName, pType, hInst);
}

//=============================================================================
//
// LoadAltImage()
//
// Purpose:     The LoadAltImage() Loads the altername image for the button.  
//				This function call is optional
// Parameters:  
//		[IN]	id
//				resource id, one of the resources already imported with the 
//				resource editor, usually begins with IDR_  
//
//		[IN]	pType
//				pointer to string describing the resource type
//				
// Returns:     BOOL
//				Non zero if successful, otherwise zero
//
//=============================================================================
BOOL CButtonBT::LoadAltImage(UINT id, LPCTSTR pType)
{
	m_bHaveAltImage = TRUE;
	if (m_imgAlt.LoadResource(FindResource(NULL, MAKEINTRESOURCE(id), L"PNG"), CXIMAGE_FORMAT_PNG))
	{
		return TRUE;
	}
	return FALSE;
// 	m_pAltImage = new CGdiPlusBitmapResource;
// 	return (m_pAltImage->Load(id, pType));

}

BOOL CButtonBT::LoadAltImage( LPCTSTR pName, HMODULE hInst /*= NULL*/ )
{
	m_bHaveAltImage = TRUE;
	if (m_imgAlt.LoadResource(FindResource(hInst, pName, L"PNG"), CXIMAGE_FORMAT_PNG), hInst)
	{
		return TRUE;
	}
	return FALSE;
}


//=============================================================================
//
//	The framework calls this member function when a child control is about to 
//	be drawn.  All the bitmaps are created here on the first call. Every thing
//	is done with a memory DC except the background, which get's it's information 
//	from the parent. The background is needed for transparent portions of PNG 
//	images. An always on top app (such as Task Manager) that is in the way can 
//	cause it to get an incorrect background.  To avoid this, the parent should 
//	call the SetBkGnd function with a memory DC when it creates the background.
//				
//=============================================================================
HBRUSH CButtonBT::CtlColor(CDC* pScreenDC, UINT nCtlColor) 
{
	if(!m_bHaveBitmaps)
	{
		if(!m_imgStd.IsValid())
		{
			return NULL; // Load the standard image with LoadStdImage()
		}

		CBitmap bmp, *pOldBitmap;

		CRect rect;
		GetClientRect(rect);

		// do everything with mem dc
		CMemDCEx memdc(pScreenDC, rect);
		//CGDIPMemDC pDC(pScreenDC, rect);

		//Gdiplus::Graphics graphics(pDC->m_hDC);

		// background
		if (m_dcBk.m_hDC == NULL)
		{

			CRect rect1;
			CClientDC clDC(GetParent());
			GetWindowRect(rect1);
			GetParent()->ScreenToClient(rect1);

			m_dcBk.CreateCompatibleDC(&clDC);
			bmp.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());
			pOldBitmap = m_dcBk.SelectObject(&bmp);
			m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), &clDC, rect1.left, rect1.top, SRCCOPY);
			bmp.DeleteObject();
		}

		// standard image
		if (m_dcStd.m_hDC == NULL)
		{
			PaintBk(&memdc);

			//graphics.DrawImage(*m_pStdImage, 0, 0);
			m_imgStd.Draw(memdc.GetSafeHdc());
		
			m_dcStd.CreateCompatibleDC(&memdc);
			bmp.CreateCompatibleBitmap(&memdc, rect.Width(), rect.Height());
			pOldBitmap = m_dcStd.SelectObject(&bmp);
			m_dcStd.BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
			bmp.DeleteObject();

			// standard image pressed
			if (m_dcStdP.m_hDC == NULL)
			{
				PaintBk(&memdc);

				if (m_bISMove)
				{
					//graphics.DrawImage(*m_pStdImage, 1, 1);
					m_imgStd.Draw(memdc.GetSafeHdc(), 1, 1);
				}
				else
				{
					//graphics.DrawImage(*m_pStdImage, 0, 0);
					m_imgStd.Draw(memdc.GetSafeHdc());
				}
				

				m_dcStdP.CreateCompatibleDC(&memdc);
				bmp.CreateCompatibleBitmap(&memdc, rect.Width(), rect.Height());
				pOldBitmap = m_dcStdP.SelectObject(&bmp);
				m_dcStdP.BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// standard image hot
			if(m_dcStdH.m_hDC == NULL)
			{
				PaintBk(&memdc);

				CxImage imgtemp = m_imgStd;
				imgtemp.ShiftRGB(20,20,20);
				imgtemp.Draw(memdc.GetSafeHdc());
				m_dcStdH.CreateCompatibleDC(&memdc);
				bmp.CreateCompatibleBitmap(&memdc, rect.Width(), rect.Height());
				pOldBitmap = m_dcStdH.SelectObject(&bmp);
				m_dcStdH.BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// grayscale image
			if(m_dcGS.m_hDC == NULL)
			{
				PaintBk(&memdc);

				CxImage imgtemp = m_imgStd;
				imgtemp.GrayScale();
				imgtemp.Draw(memdc.GetSafeHdc());
				m_dcGS.CreateCompatibleDC(&memdc);
				bmp.CreateCompatibleBitmap(&memdc, rect.Width(), rect.Height());
				pOldBitmap = m_dcGS.SelectObject(&bmp);
				m_dcGS.BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}
		}

		// alternate image
		if( (m_dcAlt.m_hDC == NULL) && m_bHaveAltImage )
		{
			PaintBk(&memdc);

			//graphics.DrawImage(*m_pAltImage, 0, 0);
			m_imgAlt.Draw(memdc.GetSafeHdc());
		
			m_dcAlt.CreateCompatibleDC(&memdc);
			bmp.CreateCompatibleBitmap(&memdc, rect.Width(), rect.Height());
			pOldBitmap = m_dcAlt.SelectObject(&bmp);
			m_dcAlt.BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
			bmp.DeleteObject();

			// alternate image pressed
			if( (m_dcAltP.m_hDC == NULL) && m_bHaveAltImage )
			{
				PaintBk(&memdc);

				if (m_bISMove)
				{
					//graphics.DrawImage(*m_pAltImage, 1, 1);
					m_imgAlt.Draw(memdc.GetSafeHdc(), 1, 1);
				}
				else
				{
					//graphics.DrawImage(*m_pAltImage, 0, 0);
					m_imgAlt.Draw(memdc.GetSafeHdc());
				}
				
			
				m_dcAltP.CreateCompatibleDC(&memdc);
				bmp.CreateCompatibleBitmap(&memdc, rect.Width(), rect.Height());
				pOldBitmap = m_dcAltP.SelectObject(&bmp);
				m_dcAltP.BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// alternate image hot
			if(m_dcAltH.m_hDC == NULL)
			{
				PaintBk(&memdc);

				CxImage imgtemp = m_imgAlt;
				imgtemp.ShiftRGB(20,20,20);
				imgtemp.Draw(memdc.GetSafeHdc());
				m_dcAltH.CreateCompatibleDC(&memdc);
				bmp.CreateCompatibleBitmap(&memdc, rect.Width(), rect.Height());
				pOldBitmap = m_dcAltH.SelectObject(&bmp);
				m_dcAltH.BitBlt(0, 0, rect.Width(), rect.Height(), &memdc, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}
		}

		if(m_pCurBtn == NULL)
		{
			m_pCurBtn = &m_dcStd;
		}

		m_bHaveBitmaps = TRUE;
	}

	return NULL;
}

//=============================================================================
// paint the background
//=============================================================================
void CButtonBT::PaintBk(CDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
}

//=============================================================================
// paint the bitmap currently pointed to with m_pCurBtn
//=============================================================================
void CButtonBT::PaintBtn(CDC *pDC, BOOL mark)
{
	CRect rect;
	GetClientRect(rect);
	CMemDCEx memdc(pDC, this);
	memdc.BitBlt(0, 0, rect.Width(), rect.Height(), m_pCurBtn, 0, 0, SRCCOPY);

	if (m_bISTextSelf)
	{
		memdc.SetBkMode(TRANSPARENT);
		memdc.SelectObject(m_font);
		CRect rc(rect);
		rc.left += 6;
		memdc.DrawText(m_strText, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
	}
	if (mark)
	{
		return;
	}

	if (m_bISFrame)
	{
		CPoint pt(5,5);
		CBrush *bru;
		CPen pen;
		pen.CreatePen(PS_SOLID, 1, RGB(138,138,138));
		bru = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CBrush *oldbru = memdc.SelectObject(bru);
		CPen *oldpen = memdc.SelectObject(&pen);
		memdc.RoundRect(rect, pt);
		//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), m_pCurBtn, 0, 0, SRCCOPY);
		memdc.SelectObject(oldbru);
		memdc.SelectObject(oldpen);
	}

}

//=============================================================================
// enables the toggle mode
// returns if it doesn't have the alternate image
//=============================================================================
void CButtonBT::EnableToggle(BOOL bEnable)
{
	if(!m_bHaveAltImage) return;

	m_bIsToggle = bEnable; 

	// this actually makes it start in the std state since toggle is called before paint
	if(bEnable)	m_pCurBtn = &m_dcAlt;
	else		m_pCurBtn = &m_dcStd;

}

//=============================================================================
// sets the image type and disabled state then repaints
//=============================================================================
void CButtonBT::SetImage(int type)
{
	m_nCurType = type;

	(type == DIS_TYPE) ? m_bIsDisabled = TRUE : m_bIsDisabled = FALSE;

	Invalidate();
}

//=============================================================================
// set the control to owner draw
//=============================================================================
void CButtonBT::PreSubclassWindow()
{
	// Set control to owner draw
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

//=============================================================================
// disable double click 
//=============================================================================
BOOL CButtonBT::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	if (m_pToolTip != NULL)
	{
		if (::IsWindow(m_pToolTip->m_hWnd))
		{
			m_pToolTip->RelayEvent(pMsg);		
		}
	}

	return CButton::PreTranslateMessage(pMsg);
}


//=============================================================================
// overide the erase function
//=============================================================================
BOOL CButtonBT::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

//=============================================================================
// Paint the button depending on the state of the mouse
//=============================================================================
void CButtonBT::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// handle disabled state
	if(m_bIsDisabled)
	{
		m_pCurBtn = &m_dcGS;
		PaintBtn(pDC, TRUE);
		return;
	}

	BOOL bIsPressed = (lpDIS->itemState & ODS_SELECTED);

	// handle toggle button
	if(m_bIsToggle && bIsPressed)
	{
		(m_nCurType == STD_TYPE) ? m_nCurType = ALT_TYPE : m_nCurType = STD_TYPE;
	}

	if(bIsPressed)
	{
		if(m_nCurType == STD_TYPE)
			m_pCurBtn = &m_dcStdP;
		else
			m_pCurBtn = &m_dcAltP;
		//PaintBtn(pDC, 0);
		//return;
	}
	else if(m_bIsHovering)
	{

		if(m_nCurType == STD_TYPE)
		{
			if (m_bISHot)
			{
				m_pCurBtn = &m_dcStdH;
			}
			else
			{
				m_pCurBtn = &m_dcStd;
			}
		}
		else
		{
			if (m_bISHot)
			{
				m_pCurBtn = &m_dcAltH;
			}
			else
			{
				m_pCurBtn = &m_dcAlt;
			}
		}

		//PaintBtn(pDC, 0);
	}
	else
	{
		if(m_nCurType == STD_TYPE)
			m_pCurBtn = &m_dcStd;
		else
			m_pCurBtn = &m_dcAlt;
		PaintBtn(pDC, TRUE);
		return;
	}

	// paint the button
	PaintBtn(pDC);
}

//=============================================================================
LRESULT CButtonBT::OnMouseHover(WPARAM wparam, LPARAM lparam) 
//=============================================================================
{
	m_bIsHovering = TRUE;
	Invalidate();
	DeleteToolTip();

	// Create a new Tooltip with new Button Size and Location
	SetToolTipText(m_tooltext);

	if (m_pToolTip != NULL)
	{
		if (::IsWindow(m_pToolTip->m_hWnd))
		{
			//Display ToolTip
			m_pToolTip->Update();
		}
	}

	return 0;
}


//=============================================================================
LRESULT CButtonBT::OnMouseLeave(WPARAM wparam, LPARAM lparam)
//=============================================================================
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	Invalidate();
	return 0;
}

//=============================================================================
void CButtonBT::OnMouseMove(UINT nFlags, CPoint point) 
//=============================================================================
{
	if (!m_bIsTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = _TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}

//=============================================================================
//	
//	Call this member function with a memory DC from the code that paints 
//	the parents background.  Passing the screen DC defeats the purpose of 
//  using this function.
//
//=============================================================================
void CButtonBT::SetBkGnd(CDC* pDC)
{
	CRect rect, rectS;
	CBitmap bmp, *pOldBitmap;

	GetClientRect(rect);
	GetWindowRect(rectS);
	GetParent()->ScreenToClient(rectS);

	m_dcBk.DeleteDC();

	m_dcBk.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = m_dcBk.SelectObject(&bmp);
	m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rectS.left, rectS.top, SRCCOPY);
	bmp.DeleteObject();
}


//=============================================================================
// Set the tooltip with a string resource
//=============================================================================
void CButtonBT::SetToolTipText(UINT nId, BOOL bActivate)
{
	// load string resource
	m_tooltext.LoadString(nId);

	// If string resource is not empty
	if (m_tooltext.IsEmpty() == FALSE)
	{
		SetToolTipText(m_tooltext, bActivate);
	}

}

//=============================================================================
// Set the tooltip with a CString
//=============================================================================
void CButtonBT::SetToolTipText(CString spText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (spText.IsEmpty()) return;

	// Initialize ToolTip
	InitToolTip();
	m_tooltext = spText;

	// If there is no tooltip defined then add it
	if (m_pToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_pToolTip->AddTool(this, m_tooltext, rectBtn, 1);
	}

	// Set text for tooltip
	m_pToolTip->UpdateTipText(m_tooltext, this, 1);
	m_pToolTip->SetDelayTime(2000);
	m_pToolTip->Activate(bActivate);
}

//=============================================================================
void CButtonBT::InitToolTip()
//=============================================================================
{
	if (m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl;
		// Create ToolTip control
		m_pToolTip->Create(this);
		m_pToolTip->Activate(TRUE);
	}
} 

//=============================================================================
void CButtonBT::DeleteToolTip()
//=============================================================================
{
	// Destroy Tooltip incase the size of the button has changed.
	if (m_pToolTip != NULL)
	{
		delete m_pToolTip;
		m_pToolTip = NULL;
	}
}

void CButtonBT::SetFrameFlag( BOOL mark )
{
	m_bISFrame = mark;
}

void CButtonBT::UpdateCtrl(BYTE* buf)
{
	AfxMessageBox(L"BUTTON");
}

void CButtonBT::SetBtnMove( BOOL mark /*= TRUE*/ )
{
	m_bISMove = mark;
}

void CButtonBT::SetBtnHot( BOOL mark /*= TRUE*/ )
{
	m_bISHot = mark;
}

void CButtonBT::SetSelfText( CString strText, int fontSize /*= 18*/, BOOL mark /*= TRUE*/ )
{
	m_strText = strText;
	m_nFontSize = fontSize;
	m_bISTextSelf = mark;
	m_font.DeleteObject();
	m_font.CreateFont(
		m_nFontSize,               // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Î¢ÈíÑÅºÚ"));                 // lpszFacename
}

void CButtonBT::SetBtnText( CString& strText )
{
	if (m_bISTextSelf)
	{
		m_strText = strText;
		Invalidate();
	}
}
