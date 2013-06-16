/************************************************************************/
/* MonthCalendarBT.h                                                    */
/************************************************************************/

#include "stdafx.h"
#include "MonthCalendarBT.h"
#include "MemDCEx.h"

std::queue<DateMonthInfo_t> CMonthCalendarBT::s_qDateInfo;
CRITICAL_SECTION CMonthCalendarBT::s_csDateInfo;

IMPLEMENT_DYNAMIC(CMonthCalendarBT, CStatic)

CMonthCalendarBT::CMonthCalendarBT()
	: m_nRowHeight(0)
	, m_nColWidth(0)
	, m_bSelLine(FALSE)
	, m_ndayWeek(0)
	, m_ncurDay(0)
	, m_nYear(0)
	, m_nMonth(0)
	, m_clrBK(RGB(255, 255, 255))
	, m_clrSel(RGB(0, 154, 250))
	, m_clrText(RGB(120, 120, 120))
	, m_clrTextOther(RGB(200, 200, 200))
	, m_clrTextSel(RGB(255, 255, 255))
	, m_nCurCol(-1)
	, m_nCurRow(-1)
{
	m_arrWeek.Add(L"周一");
	m_arrWeek.Add(L"周二");
	m_arrWeek.Add(L"周三");
	m_arrWeek.Add(L"周四");
	m_arrWeek.Add(L"周五");
	m_arrWeek.Add(L"周六");
	m_arrWeek.Add(L"周日");
	m_arrWeek2.Add(L"星期日");
	m_arrWeek2.Add(L"星期一");
	m_arrWeek2.Add(L"星期二");
	m_arrWeek2.Add(L"星期三");
	m_arrWeek2.Add(L"星期四");
	m_arrWeek2.Add(L"星期五");
	m_arrWeek2.Add(L"星期六");

	ZeroMemory(m_arrDay, 6 * 7 * sizeof(int));
	ZeroMemory(m_arrMark, 6 * 7 * sizeof(BOOL));
	BOOL res = m_font.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,			   // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		PROOF_QUALITY,             // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑") );          // lpszFacename
	if (!res)
	{
		res = m_font.CreateFont(
			16,                        // nHeight
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
			_T("宋体") );              // lpszFacename
	}
	m_fontBig.CreateFont(
		160,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,			   // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		PROOF_QUALITY,             // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑") );          // lpszFacename
	m_fontMid.CreateFont(
		25,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		600,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,			   // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		PROOF_QUALITY,             // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑") );          // lpszFacename
}

CMonthCalendarBT::~CMonthCalendarBT()
{
}


BEGIN_MESSAGE_MAP(CMonthCalendarBT, CStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//ON_WM_LBUTTONDOWN()
	ON_CONTROL_REFLECT(STN_CLICKED, &CMonthCalendarBT::OnStnClicked)
END_MESSAGE_MAP()



// MonthCalendarBT message handlers



int CMonthCalendarBT::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CMonthCalendarBT::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CMemDCEx memdc(&dc, this);
	memdc.FillSolidRect(m_rcClock, m_clrBK);
	
	memdc.SetBkMode(TRANSPARENT);
	memdc.SetTextColor(m_clrText);
	CRect rc;
	CRect rcOut;
	CString strday;
	if (m_bHasEXPane)
	{
		memdc.SetTextColor(RGB(180, 180, 180));
		if (m_ncurDay < 10)
		{
			strday.Format(L"0%d", m_ncurDay);
		}
		else
		{
			strday.Format(L"%d", m_ncurDay);
		}
		rc = CRect(0, 0, m_rcDayPane.Width(), m_rcDayPane.Height() - 40);
		rcOut = CRect(8, m_rcDayPane.Height() - 35, m_rcDayPane.Width(), m_rcDayPane.Height());
		memdc.SelectObject(m_fontBig);
		memdc.DrawText(strday, rc, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
		CTime t_time(m_nYear, m_nMonth, m_ncurDay, 0, 0, 0);
		memdc.SelectObject(m_fontMid);
		memdc.DrawText(GetTimeString(t_time), rcOut, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	}


	//画星期抬头
	memdc.SelectObject(&m_font);
	rc = CRect(m_rcMonthPane.left, m_rcMonthPane.top, m_rcMonthPane.left+m_nColWidth, m_rcMonthPane.top+m_nRowHeight);
	rcOut = CRect(m_rcMonthPane.left, 0, m_rcMonthPane.left, 0);
	for (int i = 0; i < 7; i++)
	{
		memdc.DrawText(m_arrWeek.GetAt(i), rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);	
		rc.OffsetRect(m_nColWidth, 0);
	}
	//绘制日期
	
	for (int i = 0; i < 6; i++)
	{
		rcOut.top = m_nRowHeight + m_nRowHeight * i;
		rcOut.bottom = rcOut.top + m_nRowHeight;
		for (int j = 0; j < 7; j++)
		{
			rcOut.left = rcOut.right;
			rcOut.right += m_nColWidth;

			if (m_arrMark[i * 7 + j])
			{
				memdc.SetTextColor(m_clrText);
			}
			else
			{
				memdc.SetTextColor(m_clrTextOther);
			}

			if (m_arrDay[i * 7 + j] < 10)
			{
				strday.Format(L" %d", m_arrDay[i * 7 + j]);
			}
			else
			{
				strday.Format(L"%d", m_arrDay[i * 7 + j]);
			}
			if (i == m_nCurRow && j == m_nCurCol)
			{
				CRect rcsel = rcOut;
				rcsel.InflateRect(-3, -2);
				memdc.FillSolidRect(rcsel, m_clrSel);
				memdc.SetTextColor(m_clrTextSel);
			}

			memdc.DrawText(strday, rcOut, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		rcOut.right = m_rcMonthPane.left;
	}
}

BOOL CMonthCalendarBT::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// TODO: Add your specialized code here and/or call the base class
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}

BOOL CMonthCalendarBT::CreateDateCtrl(const RECT& rect, CWnd* pParentWnd, BOOL selline, UINT nID, BOOL hasEXPane)
{
	m_bSelLine = selline;
	m_rcClock = CRect(0,0,rect.right - rect.left, rect.bottom - rect.top);
	if (hasEXPane)
	{
		m_rcDayPane = CRect(0, 0, m_rcClock.Width() / 2, m_rcClock.Height());
		m_rcMonthPane = CRect(m_rcClock.Width() / 2 + 1, 0, m_rcClock.Width(), m_rcClock.Height());
		m_nColWidth = (m_rcClock.Width()/2) / 7.0;
	}
	else
	{
		m_rcDayPane = CRect(0, 0, 0, 0);
		m_rcMonthPane = m_rcClock;
		m_nColWidth = m_rcClock.Width() / 7.0;
	}
	m_nRowHeight = m_rcClock.Height() / 7.0;

	m_curTime = CTime::GetCurrentTime();
	UpdateDataInfo(m_curTime);

	m_bHasEXPane = hasEXPane;
	InitializeCriticalSection(&CMonthCalendarBT::s_csDateInfo);
	
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOTIFY;
	return Create(L"datactrl", dwStyle, rect, pParentWnd, nID);
}

int CMonthCalendarBT::GetdDayNumOfMonth( int year, int month )
{
	switch (month)
	{
	case 1:
		return 31;
		break;
	case 2:
		if((year % 4 == 0 && year % 100 != 0)
			|| ( year % 400 == 0))
		{
			return 29;
		}
		else
		{
			return 28;	
		}
		break;
	case 3:
		return 30;
		break;
	case 4:
		return 30;
		break;
	case 5:
		return 31;
		break;
	case 6:
		return 30;
		break;
	case 7:
		return 31;
		break;
	case 8:
		return 31;
		break;
	case 9:
		return 30;
		break;
	case 10:
		return 31;
		break;
	case 11:
		return 30;
		break;
	case 12:
		return 31;
		break;
	default:
		return 30;
		break;
	}
}

void CMonthCalendarBT::OnStnClicked()
{
	// TODO: Add your control notification handler code here
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	int tempx;
	int tempy;
	if (point.y < m_nRowHeight)
	{
		return;
	}
	if (m_bHasEXPane)
	{
		if (point.x < m_rcMonthPane.left)
		{
			return;
		}
		tempx = (point.x - m_rcMonthPane.left) / m_nColWidth;
	}
	else
	{
		tempx = point.x / m_nColWidth;
	}
	
	tempy = (point.y - m_nRowHeight) / m_nRowHeight;

	DateMonthInfo_t datainfo;
	int index = tempy * 7 + tempx;
	if (index < 0 || index >= 42)
	{
		return;
	}
	if (!m_arrMark[index])
	{
		m_ncurDay = m_arrDay[index];
		if (m_ncurDay > 15)
		{
			m_nMonth--;
		}
		else
		{
			m_nMonth++;
		}
		if (m_nMonth > 12)
		{
			m_nMonth = 1;
			m_nYear++;
		}
		else if (m_nMonth == 0)
		{
			m_nMonth = 12;
			m_nYear--;
		}
		CTime curtime(m_nYear, m_nMonth, m_ncurDay, 0, 0, 0);
		UpdateDataInfo(curtime);
	}
	else
	{
		m_nCurCol = tempx;
		m_nCurRow = tempy;
		m_ncurDay = m_arrDay[index];
		//str.Format(L"%d", m_arrDay[tempy * 7 + tempx]);
		//AfxMessageBox(str);
		m_ndayWeek = tempx + 1;
	}
	datainfo.year = m_nYear;
	datainfo.month = m_nMonth;
	datainfo.day = m_ncurDay;
	datainfo.week = m_ndayWeek;
	if (m_lastSel == datainfo)
	{
		return;
	}
	m_lastSel = datainfo;

	EnterCriticalSection(&s_csDateInfo);
	s_qDateInfo.push(datainfo);
	LeaveCriticalSection(&s_csDateInfo);

	(GetParent())->PostMessage(WM_DATECTRL_MSG, 0, 0);
	Invalidate();
}

void CMonthCalendarBT::SetBkColor( COLORREF& clr )
{
	m_clrBK = clr;
}

CString CMonthCalendarBT::GetTimeString(CTime& time)
{
	CString strTime;
	strTime.Format(L"%d年", time.GetYear());
	int temp = 0;
	temp = time.GetMonth();
	if (temp < 10)
	{
		strTime.AppendFormat(L"0%d月", temp);
	}
	else
	{
		strTime.AppendFormat(L"%d月", temp);
	}
	temp = time.GetDay();
	if (temp < 10)
	{
		strTime.AppendFormat(L"0%d日", temp);
	}
	else
	{
		strTime.AppendFormat(L"%d日", temp);
	}
	strTime += m_arrWeek2[time.GetDayOfWeek()-1];
	return strTime;
}

void CMonthCalendarBT::UpdateDataInfo( CTime& time )
{
	m_ndayWeek = time.GetDayOfWeek();
	m_ncurDay = time.GetDay();
	m_nYear = time.GetYear();
	m_nMonth = time.GetMonth();
	int monthday = GetdDayNumOfMonth(m_nYear, m_nMonth);
	int lastmonthday = 0;

	CTime temptime(m_nYear, m_nMonth, 1, 0, 0, 0);//当月1号
	int tempweekday = temptime.GetDayOfWeek();
	if (tempweekday == 1)
	{
		tempweekday = 8;
	}
	if (tempweekday == 2)
	{
		tempweekday = 9;
	}
	for (int i = 0; i < monthday; i++)
	{
		m_arrDay[tempweekday - 2 + i] = i + 1;
		m_arrMark[tempweekday - 2 + i] = TRUE;
	}
	int monthdaypre = GetdDayNumOfMonth(m_nYear, m_nMonth - 1);
	for (int i = 0; i < tempweekday - 2; i++)
	{
		m_arrDay[tempweekday - 3 - i] = monthdaypre--;
		m_arrMark[tempweekday - 3 - i] = FALSE;
	}

	for (int i = monthday + tempweekday - 2, j = 1; i < 42; i++)
	{
		m_arrDay[i] = j++;
		m_arrMark[i] = FALSE;
	}

	int temp = tempweekday - 3 + m_ncurDay;
	m_nCurRow = temp / 7;
	m_nCurCol = temp % 7;
}
