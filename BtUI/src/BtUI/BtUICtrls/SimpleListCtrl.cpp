#include "stdafx.h"
#include "SimpleListCtrl.h"

BEGIN_MESSAGE_MAP(CSimpleListCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

CSimpleListCtrl::CSimpleListCtrl()
	: m_nRowHeight(0)
	, m_nRow(0)
	, m_nColumn(0)
	, m_nTitleHeight(0)
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
		_T("微软雅黑") );              // lpszFacename
}

CSimpleListCtrl::~CSimpleListCtrl()
{
	DeleteObject(m_font);
}

void CSimpleListCtrl::UpdateCtrl( BYTE* buf )
{
	if (buf)
	{
		JY_CMISCtrlData *data = reinterpret_cast<JY_CMISCtrlData*>(buf);
		m_nRow = data->size;
		JY_CMISListData *listdata = reinterpret_cast<JY_CMISListData*>(data->buf);
		m_nColumn = listdata->colum;
		for (int i = 0; i < m_nRow; i++)
		{
			for (int j = 0; j < m_nColumn; j++)
			{
				m_vecListData.push_back(listdata->vecItem.at(i * m_nColumn + j));
			}
		}
		delete data;
	}
}

void CSimpleListCtrl::SetHeight(int titleheight, int rowheight)
{
	m_nTitleHeight = titleheight;
	m_nRowHeight = rowheight;
}

void CSimpleListCtrl::SetColWidth( CString& str )
{
	int start = 0;
	int end = 0;
	int len = str.GetLength();
	
	do 
	{
		end = str.Find(',', start);
		if (end == -1)
		{
			end = len;
		}
		m_vecColWidth.push_back(_ttoi(str.Mid(start, end - start)));
		start = end + 1;
	} while (len);
}

void CSimpleListCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC memdc;
	memdc.CreateCompatibleDC(&dc);
	CBitmap bmp;

	bmp.CreateCompatibleBitmap(&dc, m_rcWnd.Width(), m_rcWnd.Height());
	memdc.SelectObject(&bmp);
	memdc.SelectObject(&m_font);
	memdc.SetBkMode(TRANSPARENT);
	m_imgBack.Draw(memdc.GetSafeHdc(), 0, 0, m_rcWnd.Width(), m_rcWnd.Height());

	//输出列表文字
	CRect rcOut(0, m_nTitleHeight, 0, 0);
	for (int i = 0; i < m_nRow; i++)
	{
		rcOut.top = m_nTitleHeight + m_nRowHeight * i;
		rcOut.bottom = rcOut.top + m_nRowHeight;
		for (int j = 0; j < m_nColumn; j++)
		{
			rcOut.left = rcOut.right;
			rcOut.right += m_vecColWidth[j];
			memdc.DrawText(m_vecListData[i * m_nRow + j], rcOut, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		}
		rcOut.right = 0;
	}

	dc.BitBlt(0, 0, m_rcWnd.Width(), m_rcWnd.Height(), &memdc, 0, 0, SRCCOPY);
}

BOOL CSimpleListCtrl::OnEraseBkgnd( CDC* pDC )
{
	return TRUE;
}


