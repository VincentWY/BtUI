#pragma once
#include "..\BtUIHead.h"

#pragma pack(push, 1)
typedef struct tag_BT_CMIS_CtrlData
{
	int size;//针对各种不同控件，该元素表示的意义均有可能不同
	BYTE *buf;
	tag_BT_CMIS_CtrlData()
	{
		size = 0;
		buf = NULL;
	}
	~tag_BT_CMIS_CtrlData()
	{
		delete [] buf;
	}
}BT_CMISCtrlData;


typedef struct tag_BT_CMIS_ListData
{
	int colum;
	std::vector<CString> vecItem;	
	tag_BT_CMIS_ListData()
	{
		colum = 0;
	}
}BT_CMISListData;

#pragma pack(pop)

//base
class BTUI_API CBTBaseCtrl
{
public:
	CBTBaseCtrl();
	virtual ~CBTBaseCtrl();
public:
	virtual void UpdateCtrl(BYTE* buf) = 0;
	void SetBackImg(CString& str, HMODULE hModule = NULL);
	void SetBackImg(LPCTSTR str, HMODULE hModule = NULL);
	void SetCtrlWnd(CRect& rc);
	void SetOrgText(LPCTSTR buf);
protected:
	CxImage m_imgBack;
	CRect m_rcWnd;
	CString m_strOrgText;
};
