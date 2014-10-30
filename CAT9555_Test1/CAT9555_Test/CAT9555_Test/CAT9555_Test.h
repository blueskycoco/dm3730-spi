// CAT9555_Test.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

// CCAT9555_TestApp:
// See CAT9555_Test.cpp for the implementation of this class
//

class CCAT9555_TestApp : public CWinApp
{
public:
	CCAT9555_TestApp();
	
// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CCAT9555_TestApp theApp;
