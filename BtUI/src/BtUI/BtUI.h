// BtUI.h : main header file for the BtUI DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBtUIApp
// See BtUI.cpp for the implementation of this class
//

class CBtUIApp : public CWinApp
{
public:
	CBtUIApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	ULONG_PTR m_gdiplusToken;
};
