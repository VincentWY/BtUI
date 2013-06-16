/************************************************************************/
/* ComboBoxBT.h                                                         */
/************************************************************************/


#pragma once
#include "BaseCtrl.h"
// CComboBoxBT

class BTUI_API CComboBoxBT : public CComboBox
{
	DECLARE_DYNAMIC(CComboBoxBT)

public:
	CComboBoxBT();
	virtual ~CComboBoxBT();

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetBKImg(LPCTSTR str);
	afx_msg void OnPaint();
	void SetTextColor(COLORREF clr);
	void SetDefaultText(CString str);

private:
	//CStatic m_pane;
	CxImage m_imgBK;
	CString m_strSel;
	CFont m_font;
	COLORREF m_clrText;
};


