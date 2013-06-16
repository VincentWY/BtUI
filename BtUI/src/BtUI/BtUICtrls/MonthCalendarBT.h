/************************************************************************/
/*MonthCalendarBT.h                                                     */
/************************************************************************/

#pragma once
#include "BaseCtrl.h"
#include <queue>


//处理此自定义消息得到日历的数据，若有冲突，请更改此处的值
#define  WM_DATECTRL_MSG	WM_APP + 301


typedef struct tagDateMonthInfo_t
{
	int year;
	int month;
	int day;
	int week;
	tagDateMonthInfo_t()
	{
		year = 0;
		month = 0;
		day = 0;
		week = 0;
	}
	BOOL operator == (tagDateMonthInfo_t& src)
	{
		if (src.year == year &&
			src.month == month &&
			src.day == day &&
			src.week == week)
		{
			return TRUE;
		}
		return FALSE;
	}
}DateMonthInfo_t;


class BTUI_API CMonthCalendarBT : public CStatic
{
	DECLARE_DYNAMIC(CMonthCalendarBT)

public:
	CMonthCalendarBT();
	virtual ~CMonthCalendarBT();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//重新定义创建控件的方法
	BOOL CreateDateCtrl(const RECT& rect, CWnd* pParentWnd, BOOL selline = FALSE, UINT nID = 0xffff, BOOL hasEXPane = FALSE);
	void SetBkColor(COLORREF& clr);
private:
	virtual BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	int GetdDayNumOfMonth(int year, int month);
	CString GetTimeString(CTime& time);
	void UpdateDataInfo(CTime& time);
private:
	CRect m_rcClock;
	int m_nColWidth;
	int m_nRowHeight;
	CFont m_font;
	CFont m_fontBig;
	CFont m_fontMid;
	CStringArray m_arrWeek;
	CStringArray m_arrWeek2;
	CTime m_curTime;
	BOOL m_bSelLine;//选择一行
	int m_arrDay[42];
	BOOL m_arrMark[42];
	int m_ndayWeek;//当前星期几
	int m_nYear;
	int m_nMonth;
	int m_ncurDay;

	COLORREF m_clrBK;
	COLORREF m_clrSel;
	COLORREF m_clrText;
	COLORREF m_clrTextSel;
	COLORREF m_clrTextOther;//不是当月的日期
	int m_nCurRow;
	int m_nCurCol;
	CxImage m_imgSelRow;
	BOOL m_bHasEXPane;
	CRect m_rcDayPane;
	CRect m_rcMonthPane;
	DateMonthInfo_t m_lastSel;

public:
	static std::queue<DateMonthInfo_t> s_qDateInfo;
	static CRITICAL_SECTION s_csDateInfo;
public:
	afx_msg void OnStnClicked();
};


