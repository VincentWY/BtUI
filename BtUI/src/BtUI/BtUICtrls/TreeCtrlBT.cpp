/************************************************************************/
/* TreeCtrlBT.cpp : implementation file                                 */
/************************************************************************/

#include "stdafx.h"
#include "TreeCtrlBT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PARENT_HEIGHT 25   //���ڵ�߶�
#define	CHILD_HEIGHT  40   //�ӽڵ�߶�

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlBT

CTreeCtrlBT::CTreeCtrlBT()
{
	m_bEnableRowLine  = true;
	m_bEnableRootBk   = true;
	m_nImageHeight    = 0;
	m_nImageWidth     = 0;
	m_nCellWidth	  = 4;
	m_clrRootText	  = RGB(0, 0, 0);
	m_clrTextNormal   = RGB(36, 36, 36);
	m_clrTextSelect	  = RGB(255, 255, 255);
	m_clrRoot_from    = RGB( 0x80, 0xa0, 0xff );
	m_clrRoot_to      = RGB( 0x80, 0xa0, 0xff );
	m_clrLine		  = RGB(128, 128, 128);

	m_hItemSelect     = NULL;
	m_hItemMouseMove  = NULL;
	m_pCurDrawItemMsg = NULL;

	m_ptOldMouse.x	  = -1;
	m_ptOldMouse.y    = -1;

	m_hResHandle	  = NULL;
	m_dCreate		  = FALSE;
	m_bAutoChange     = TRUE;

	m_bCurItemBackAlt = FALSE;
	m_bSameHeight = TRUE;
	m_bSeparate = FALSE;

	m_FontRoots.CreateFont(
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
		_T("΢���ź�") );              // lpszFacename

	m_FontChild.CreateFont(
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
		_T("΢���ź�") );              // lpszFacename
}

CTreeCtrlBT::~CTreeCtrlBT()
{
	//RELEASE_GDI_RESOURCE(m_bmpBackgroud);
	RELEASE_GDI_RESOURCE(m_FontRoots);
	RELEASE_GDI_RESOURCE(m_FontChild);

	//if ( m_ImageList.GetSafeHandle() )		m_ImageList.DeleteImageList();

	ITEMMSG_MAP::iterator item = m_itmemsg_map.begin();
	for ( ; item != m_itmemsg_map.end(); item++)
	{
		delete item->second;
	}
}

BEGIN_MESSAGE_MAP(CTreeCtrlBT, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlBT)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_NCHITTEST()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlBT message handlers

void CTreeCtrlBT::PreSubclassWindow() 
{
	//SetItemHeight(25);
	//m_nItemHeight = 25;	//���浥Ԫ��Ŀ�߶�
	ModifyStyle(NULL, TVS_SHOWSELALWAYS/* | TVS_SINGLEEXPAND*/);
	CTreeCtrl::PreSubclassWindow();
	if (!m_dCreate)
	{

	}
}

void CTreeCtrlBT::OnSize(UINT nType, int cx, int cy) 
{
	CTreeCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	Calculate();
}

void CTreeCtrlBT::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	Calculate();

	CBitmap bitmap;
	CDC MemeDc;
	MemeDc.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, m_rect.Width(), m_rect.Height());
	CBitmap *pOldBitmap = MemeDc.SelectObject(&bitmap);
	

	DrawBackgroudImg(&MemeDc);
	DrawItem(&MemeDc);

	dc.BitBlt( m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &MemeDc, 0, 0, SRCAND);
	dc.BitBlt( m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &MemeDc, 0, 0,SRCCOPY);

	MemeDc.SelectObject(pOldBitmap);
	MemeDc.DeleteDC();

	m_HScroll.UpdateScrollBar();
	m_VScroll.UpdateScrollBar();
}

void CTreeCtrlBT::Calculate()
{
	GetClientRect(&m_rect);
	SCROLLINFO scroll_info;
	// Determine window portal to draw into taking into account
	// scrolling position
	if ( GetScrollInfo( SB_HORZ, &scroll_info, SIF_POS | SIF_RANGE ) )
	{
		m_h_offset = -scroll_info.nPos;
		m_h_size = max( scroll_info.nMax+1, m_rect.Width());
	}
	else
	{
		m_h_offset = m_rect.left;
		m_h_size = m_rect.Width();
	}
	if ( GetScrollInfo( SB_VERT, &scroll_info, SIF_POS | SIF_RANGE ) )
	{
		if ( scroll_info.nMin == 0 && scroll_info.nMax == 100) 
			scroll_info.nMax = 0;
		m_v_offset = -scroll_info.nPos * GetItemHeight();
		m_v_size = max( (scroll_info.nMax+2)*((int)GetItemHeight()+1), m_rect.Height() );
	}
	else
	{
		m_v_offset = m_rect.top;
		m_v_size = m_rect.Height();
	}
}

ITEM_MSG* CTreeCtrlBT::GetItemMsg(HTREEITEM hItem)
{
	if ( !hItem )
	{
		return NULL;
	}

	ITEM_MSG* pItemMsg = NULL;

	ITEMMSG_MAP::iterator item = m_itmemsg_map.find(hItem);
	if ( item != m_itmemsg_map.end() )
	{
		pItemMsg = item->second;
	}

	return pItemMsg;
}


void CTreeCtrlBT::GradientFillRect( CDC *pDC, CRect &rect, BOOL sel /*= TRUE*/ )
{
	if (sel)
	{
		m_imgSel.Draw(pDC->GetSafeHdc(), rect);
	}
	else
	{
		m_imgMove.Draw(pDC->GetSafeHdc(), rect);
	}
}

void CTreeCtrlBT::DrawItem( CDC *pDC )
{
	// draw items
	HTREEITEM show_item,parent; //��ǰ�������Լ����ĸ����ھ��
	CRect rc_item;				//�������
	DWORD tree_style;			//��������
	int state;					//ĳһ���״̬
	bool selected;				//True:��ʾ����Ҫ����	
	bool has_children;			//True:��ʾ�ǲ�����չ����������
	
	show_item = GetFirstVisibleItem();//��ȡ��һ���ɼ���ľ��
	if ( show_item == NULL )
		return;
	
	tree_style = ::GetWindowLong( m_hWnd, GWL_STYLE ); //��ȡTREE������

	int heightadd = 0;
	
	do
	{
		state = GetItemState(show_item, TVIF_STATE);//��ȡĳһ���״̬
		parent = GetParentItem(show_item);		  //��ȡĳһ��ĸ����ھ��
		
		has_children = ItemHasChildren(show_item) || parent == NULL;
		
		selected = (state & TVIS_SELECTED) && ((this == GetFocus()) || 
			(tree_style & TVS_SHOWSELALWAYS));

		if (GetItemRect(show_item, rc_item, TRUE))//��ȡĳһ��ľ�������
		{
			CRect rcclient;
			GetClientRect(rcclient);
			if (rc_item.top >= rcclient.bottom)
			{
				break;	//���ɼ���ʱ��,�Ͳ���Ҫ�ٻ���
			}

			m_pCurDrawItemMsg = GetItemMsg(show_item);

			CRect rcBK = rc_item;
			rcBK.left = 0;
			rcBK.right = m_rect.Width();

			if (!m_pCurDrawItemMsg->bDraw)
			{
				rcBK.top -= rcBK.Height();
				if (rcBK.top < 0)
				{
					DrawItemBack(rcBK, pDC);
				}
				
				continue;
			}

			if (m_bSameHeight || has_children)
			{
				DrawItemBack(rcBK, pDC);

				if ( selected )	//ѡ��ʱ��ı���
				{
					GradientFillRect(pDC, rcBK);
				}
				else
				{
					if (m_bSeparate)
					{
						DrawRowLine(CPoint(rcBK.left, rcBK.bottom), CPoint(rcBK.right, rcBK.bottom), pDC);
					}
				}
				if (has_children)
				{	
					//����չ������
					DrawExpand(rc_item, state, pDC);
				}

				//������λͼ
				if (GetParentItem(show_item))
				{
					DrawItemImg(rc_item, show_item, pDC);	
				}
				//�����ı�
				DrawItemText(rc_item, show_item, selected, pDC);
			}
			else
			{
				rc_item.bottom += rc_item.Height();
				rcBK.bottom += rcBK.Height(); 
				DrawItemBack(rcBK, pDC);

				if ( selected )	//ѡ��ʱ��ı���
				{
					GradientFillRect(pDC, rcBK);
				}

				//������λͼ
				if (GetRootItem() != show_item)
				{
					DrawItemImg(rc_item, show_item, pDC);	
				}
				//�����ı�
				DrawItemText(rc_item, show_item, selected, pDC);
			}
		}
	}while ((show_item = GetNextVisibleItem(show_item)) != NULL);
}

void CTreeCtrlBT::DrawRowLine(CPoint ptBegin,CPoint ptEnd,CDC *pDc /* = NULL */)
{
	if ( !IsEnableRowLine() )
	{
		return;
	}
	
	if ( NULL == pDc)
	{
		CClientDC dc(this);
		pDc = &dc;
	}
	
	CPen pen(PS_SOLID, 1, m_clrLine);//RGB(235,235,235)
	CPen *pOldPen = pDc->SelectObject(&pen);
	pDc->MoveTo(ptBegin);
	pDc->LineTo(ptEnd);
	pDc->SelectObject(pOldPen);
	pen.DeleteObject();
}

void CTreeCtrlBT::DrawExpand(CRect rect, int state, CDC *pDc /* = NULL */)
{
	if (!m_vecImgList.size())
	{
		return;
	}
	
	if ( NULL == pDc)
	{
		CClientDC dc(this);
		pDc = &dc;
	}
	
	int nImage = 0;
	if ( state & TVIS_EXPANDED )	//չ��
	{
		nImage = 1;
	}
	
	CPoint point;
	point.x = rect.left - m_nImageWidth-m_nCellWidth;
	point.y = rect.top+(rect.Height()-m_nImageHeight)/2;
	m_vecImgList[nImage].Draw(pDc->GetSafeHdc(), point.x, point.y);
}


void CTreeCtrlBT::DrawItemText(CRect rect, HTREEITEM hItem, bool bselected,CDC *pDc /* = NULL */)
{
	if ( NULL == pDc)
	{
		CClientDC dc(this);
		pDc = &dc;
	}
	
	if (/*GetRootItem() != hItem*/GetParentItem(hItem))
	{
		if (m_pCurDrawItemMsg&& m_pCurDrawItemMsg->ItemImg.IsValid())
		{
			int nwidth = m_pCurDrawItemMsg->ItemImg.GetWidth();
			rect.left += nwidth;
			rect.right += nwidth;
		}
	}
	rect.left += m_nCellWidth;
	rect.right += m_nCellWidth;
	
	CString strText = GetItemText( hItem );
	
 	rect.right= m_rect.right;
	
	//��������
	CFont* pOldFont = NULL;
	HTREEITEM hParentItem = GetParentItem(hItem);
	if ( hParentItem )
	{
		pOldFont = pDc->SelectObject(&m_FontChild);
	}
	else
	{
		pOldFont = pDc->SelectObject(&m_FontRoots);
	}
	
	pDc->SetBkMode(TRANSPARENT);
	if ( bselected )
	{
		if ( IsEnableRootBk() )
		{
			pDc->SetTextColor( m_clrTextSelect );
		}
	}
	else
	{
		if (ItemHasChildren(hItem))
		{
			pDc->SetTextColor(m_clrRootText);
		} 
		else
		{
			pDc->SetTextColor(m_clrTextNormal);
		}
		
	}

	pDc->DrawText(strText, rect, DT_LEFT|DT_TOP|DT_VCENTER|DT_SINGLELINE);
	
	pDc->SelectObject(pOldFont);
	pOldFont = NULL;
}

void CTreeCtrlBT::EnableRowLine(BOOL bEnable /* = TRUE  */)
{
	m_bEnableRowLine = bEnable;
}

BOOL CTreeCtrlBT::IsEnableRowLine()
{
	return m_bEnableRowLine;
}

void CTreeCtrlBT::EnableRootBk(BOOL bEnable /* = TRUE  */)
{
	m_bEnableRootBk = bEnable;
}

BOOL CTreeCtrlBT::IsEnableRootBk()
{
	return m_bEnableRootBk;
}

HTREEITEM CTreeCtrlBT::InsertItemEx(HTREEITEM hParent,
									LPCTSTR lpszItem,
									int		heightTime /*= 1*/,
									LPCTSTR lpszImg /*= _T("")*/,
									LPCTSTR lpszImgAlt /*= _T("")*/,
									LPCTSTR lpszBack /*= L""*/,
									BOOL bDraw /*= TRUE*/)
{
	CString stritem;
	//stritem.Format(L" %s      ", lpszItem);
	stritem.Format(L"%s", lpszItem);
	HTREEITEM hItem = InsertItem(stritem,0,1,hParent);

	ITEM_MSG *pItemMsg = new ITEM_MSG;

	pItemMsg->hItem = hItem;

	HRSRC hr = NULL;
	hr = FindResource(m_hResHandle, lpszImg, L"PNG");
	if (hr)
	{
		pItemMsg->ItemImg.LoadResource(hr, CXIMAGE_FORMAT_PNG, m_hResHandle);
	}

	else
	{
		pItemMsg->ItemImg = m_imgDefaultItem;
	}

	hr = FindResource(m_hResHandle, lpszImgAlt, L"PNG");
	if (hr)
	{
		pItemMsg->ItemImgAlt.LoadResource(hr, CXIMAGE_FORMAT_PNG, m_hResHandle);
	}
	else
	{
		pItemMsg->ItemImgAlt = m_imgDefaultItemAlt;
	}
	
	if (bDraw)
	{
		m_bCurItemBackAlt = !m_bCurItemBackAlt;
	}

	hr = FindResource(m_hResHandle, lpszBack, L"PNG");
	if (hr)
	{
		pItemMsg->ItemBack.LoadResource(hr, CXIMAGE_FORMAT_PNG, m_hResHandle);
	}
	else
	{
		if (m_bCurItemBackAlt)
		{
			pItemMsg->ItemBack = m_imgDefaultItemBackAlt;
		}
		else
		{
			pItemMsg->ItemBack = m_imgDefaultItemBack;
		}
	}

	pItemMsg->bDraw = bDraw;

	ITEMMSG_MAP::iterator item = m_itmemsg_map.find(hItem);
	if ( item == m_itmemsg_map.end() )
	{
		m_itmemsg_map[hItem] = pItemMsg;
	}
	else
	{
		item->second = pItemMsg;
	}

	if (heightTime != 1)
	{
		TVITEMEX itemEx;  
		itemEx.mask = TVIF_INTEGRAL;  
		itemEx.iIntegral = 2; //�ǻ����߶ȵ�3��
		itemEx.hItem = hItem;
		SendNotifyMessage(TVM_SETITEM, 0, LPARAM(&itemEx));
	}

	return hItem;	
}

//�˺����ѷ���
HTREEITEM CTreeCtrlBT::InsertItemForHeight( HTREEITEM hParent, LPCTSTR lpszItem, LPCTSTR lpszImg /*= _T("")*/, LPCTSTR lpszImgAlt /*= _T("")*/, LPCTSTR lpszBack /*= L""*/ )
{
	InsertItemEx(hParent, lpszItem, 1, lpszImg, lpszImgAlt, lpszBack);
	lpszItem = L"";
	return InsertItemEx(hParent, lpszItem, 1, lpszImg, lpszImgAlt, lpszBack, FALSE);//���β���ͬ������
}

BOOL CTreeCtrlBT::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return true;//CTreeCtrl::OnEraseBkgnd(pDC);
}

void CTreeCtrlBT::SetSelectAndMovePng( LPCTSTR lpszSel, LPCTSTR lpszMove )
{
	m_imgSel.LoadResource(FindResource(m_hResHandle, lpszSel, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	m_imgMove.LoadResource(FindResource(m_hResHandle, lpszMove, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
}

void CTreeCtrlBT::InitCreateTree(HMODULE hResHandle)
{
	m_dCreate = TRUE;	
	m_hResHandle = hResHandle;
}

int CTreeCtrlBT::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (m_dCreate)
	{
	}
	// TODO:  Add your specialized creation code here

	return 0;
}

BOOL CTreeCtrlBT::SetBackgroudImg( LPCTSTR lpszRecourceName )
{
	SetFont(&m_FontRoots);
	bool res = m_imgBackground.LoadResource(FindResource(m_hResHandle, lpszRecourceName, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	if (res)
	{
		return TRUE;
	}
	return FALSE;
}

void CTreeCtrlBT::SetItemImg( HTREEITEM hItem, LPCTSTR lpszImg, LPCTSTR lpszImgAlt )
{
	if ( !hItem )
	{
		return;
	}
	ITEM_MSG* pItemMsg = GetItemMsg(hItem);	
	if ( !pItemMsg )
	{
		return;
	}

	pItemMsg->ItemImg.LoadResource(FindResource(m_hResHandle, lpszImg, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	pItemMsg->ItemImgAlt.LoadResource(FindResource(m_hResHandle, lpszImgAlt, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
}

void CTreeCtrlBT::SetExpandPng( LPCTSTR lpszExpandAndCollapse )
{
	CxImage img;
	bool res = false;
	res = img.LoadResource(FindResource(m_hResHandle, lpszExpandAndCollapse, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	if (!res)
	{
		return;
	}
	CxImage imgExpand,imgCollapse;
	img.Crop(0, 0, img.GetWidth() / 2, img.GetHeight(), &imgExpand);
	img.Crop(img.GetWidth() / 2, 0, img.GetWidth(), img.GetHeight(), &imgCollapse);
	

	//����߶�//���,�ڻ��Ƶ�ʱ�����

	m_nImageWidth  = imgExpand.GetWidth();
	m_nImageHeight = imgExpand.GetHeight();

	//����VECTOR
	m_vecImgList.push_back(imgExpand);
	m_vecImgList.push_back(imgCollapse);
}

void CTreeCtrlBT::DrawBackgroudImg( CDC* pDc )
{
	if (m_imgBackground.IsValid())
	{
		m_imgBackground.Draw(pDc->GetSafeHdc(), 0, 0);
	}
	else
	{
		pDc->FillSolidRect( m_rect, pDc->GetBkColor() ); 
	}
}

void CTreeCtrlBT::DrawItemImg( CRect rect, HTREEITEM hItem, CDC *pDc /*= NULL*/ )
{
	if (NULL == pDc)
	{
		CClientDC dc(this);
		pDc = &dc;
	}

	if (m_pCurDrawItemMsg && m_pCurDrawItemMsg->ItemImg.IsValid())
	{
		if (m_pCurDrawItemMsg->ItemImg.GetHeight() <= rect.Height())
		{
			switch (m_pCurDrawItemMsg->st)
			{
			case item_alt:
				m_pCurDrawItemMsg->ItemImgAlt.Draw(pDc->GetSafeHdc(), rect.left,
					(rect.Height() - m_pCurDrawItemMsg->ItemImgAlt.GetHeight()) / 2 + rect.top);
				break;
			case item_std:
				m_pCurDrawItemMsg->ItemImg.Draw(pDc->GetSafeHdc(), rect.left,
					(rect.Height() - m_pCurDrawItemMsg->ItemImg.GetHeight()) / 2 + rect.top);
				break;
			default:
				break;
			}
		}
	}
}

void CTreeCtrlBT::SetScrollBarImg( CString strimg )
{
	m_strScrollImg = strimg;
	m_imgScrollBar.LoadResource(FindResource(m_hResHandle, strimg, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
}

void CTreeCtrlBT::SetScrollBarImg( UINT uID )
{
	
}

void CTreeCtrlBT::SetScrollBarImg( LPCTSTR strimg )
{
	m_imgScrollBar.LoadResource(FindResource(m_hResHandle, strimg, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	m_VScroll.InitScrollBar(this, SB_VERT, m_imgScrollBar);
	m_HScroll.InitScrollBar(this, SB_HORZ, m_imgScrollBar);
}

void CTreeCtrlBT::SetDefaultItemImg( CString strimg, CString strimgAlt/* = L"" */)
{
	m_imgDefaultItem.LoadResource(FindResource(m_hResHandle, strimg, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	if (strimgAlt == L"")
	{
		m_imgDefaultItemAlt = m_imgDefaultItem;
	}
	else
	{
		m_imgDefaultItemAlt.LoadResource(FindResource(m_hResHandle, strimgAlt, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	}
}

void CTreeCtrlBT::SetDefaultItemImg( LPCTSTR strimg, LPCTSTR strimgAlt /*= L""*/ )
{
	m_imgDefaultItem.LoadResource(FindResource(m_hResHandle, strimg, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	HRSRC hr = NULL;
	hr = FindResource(m_hResHandle, strimgAlt, L"PNG");
	if (hr)
	{
		m_imgDefaultItemAlt.LoadResource(hr, CXIMAGE_FORMAT_PNG, m_hResHandle);
	}
	else
	{
		m_imgDefaultItemAlt = m_imgDefaultItem;
	}
}

void CTreeCtrlBT::UpdateCtrl(BYTE* buf)
{
	if (buf)
	{
		AfxMessageBox(L"tree");
	}
}

void CTreeCtrlBT::SetImgChangeAuto( BOOL mark )
{
	m_bAutoChange = mark;
}

void CTreeCtrlBT::ResetItemImg( HTREEITEM hItem, ItemImgSt_t st )
{
	if ( !hItem )
	{
		return;
	}
	ITEM_MSG* pItemMsg = GetItemMsg(hItem);	
	if ( !pItemMsg )
	{
		return;
	}
	pItemMsg->st = st;
	Invalidate();
}

CString CTreeCtrlBT::GetItemTextBT( HTREEITEM hItem )
{
	CString strText = GetItemText(hItem);
	return strText;
}

void CTreeCtrlBT::SetItemBackImg( LPCTSTR lpszBack, LPCTSTR lpszBackAlt )
{
	m_imgDefaultItemBack.LoadResource(FindResource(m_hResHandle, lpszBack, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
	m_imgDefaultItemBackAlt.LoadResource(FindResource(m_hResHandle, lpszBackAlt, L"PNG"), CXIMAGE_FORMAT_PNG, m_hResHandle);
}

void CTreeCtrlBT::DrawItemBack( CRect rc, CDC *pDC /*= NULL*/ )
{
	if (NULL == pDC)
	{
		CClientDC dc(this);
		pDC = &dc;
	}
	if (m_pCurDrawItemMsg && m_pCurDrawItemMsg->ItemBack.IsValid())
	{
		if (m_pCurDrawItemMsg->ItemBack.GetHeight() <= rc.Height())
		{
			m_pCurDrawItemMsg->ItemBack.Draw(pDC->GetSafeHdc(), rc.left, rc.top, rc.Width(), rc.Height());
		}
	}
}

void CTreeCtrlBT::SetTreeItemSameHeight( BOOL mark /*= TRUE*/ )
{
	m_bSameHeight = mark;
}

void CTreeCtrlBT::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	ITEM_MSG* item = GetItemMsg(pNMTreeView->itemNew.hItem);
	if (!item->bDraw)
	{
		SelectItem(GetPrevSiblingItem(pNMTreeView->itemNew.hItem));
		*pResult = 1;
		return;
	}
	Invalidate();
	*pResult = 0;
}

void CTreeCtrlBT::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	*pResult = 0;
}

void CTreeCtrlBT::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate();
}


BOOL CTreeCtrlBT::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt ) /*afx_msg void OnMouseMove(UINT nFlags, CPoint point)*/
{
	CTreeCtrl::OnMouseWheel(nFlags, zDelta, pt);
	Invalidate();
	return TRUE;
}

void CTreeCtrlBT::SetSeparateColor( COLORREF clr, BOOL mark /*= TRUE*/ )
{
	m_clrLine = clr;
	m_bSeparate = mark;
}

void CTreeCtrlBT::SetFontNum( int parent, int child )
{
	m_FontRoots.DeleteObject();
	m_FontChild.DeleteObject();
	m_FontRoots.CreateFont(
		parent,                        // nHeight
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
		_T("΢���ź�") );              // lpszFacename

	m_FontChild.CreateFont(
		child,                        // nHeight
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
		_T("΢���ź�") );              // lpszFacename
}

void CTreeCtrlBT::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);

	m_HScroll.ShowWindow(bShow);
	m_VScroll.ShowWindow(bShow);
	// TODO: Add your message handler code here
}

BOOL CTreeCtrlBT::RemoveAllItem()
{
	ITEMMSG_MAP::iterator item = m_itmemsg_map.begin();
	for ( ; item != m_itmemsg_map.end(); item++)
	{
		delete item->second;
	}
	m_itmemsg_map.clear();
	m_pCurDrawItemMsg = NULL;
	return DeleteAllItems();
}

ItemImgSt_t CTreeCtrlBT::GetItemImgSt( HTREEITEM hItem )
{
	if ( !hItem )
	{
		return item_std;
	}
	ITEM_MSG* pItemMsg = GetItemMsg(hItem);	
	if ( !pItemMsg )
	{
		return item_std;
	}
	return pItemMsg->st;
}
