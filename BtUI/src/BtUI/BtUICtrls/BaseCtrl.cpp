#include "stdafx.h"
#include "basectrl.h"


CBTBaseCtrl::CBTBaseCtrl()
{

}

CBTBaseCtrl::~CBTBaseCtrl()
{

}

void CBTBaseCtrl::SetBackImg( CString& str, HMODULE hModule /*= NULL*/ )
{
	m_imgBack.LoadResource(FindResource(hModule, str, L"PNG"), CXIMAGE_FORMAT_PNG, hModule);
}

void CBTBaseCtrl::SetBackImg( LPCTSTR str, HMODULE hModule /*= NULL*/ )
{
	m_imgBack.LoadResource(FindResource(hModule, str, L"PNG"), CXIMAGE_FORMAT_PNG, hModule);
}

void CBTBaseCtrl::SetCtrlWnd( CRect& rc )
{
	m_rcWnd = rc;
}

void CBTBaseCtrl::SetOrgText( LPCTSTR buf )
{
	m_strOrgText = buf;
}
