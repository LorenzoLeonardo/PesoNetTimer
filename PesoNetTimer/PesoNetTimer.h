
// PesoNetTimer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include <string>

using namespace std;
// CPesoNetTimerApp:
// See PesoNetTimer.cpp for the implementation of this class
//
#define REGISTRY_PATH _T("SOFTWARE\\Enzo Tech Peso-Net Timer")
class CPesoNetTimerApp : public CWinApp
{
public:
	CPesoNetTimerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
protected:
	string decrypt(string s);
	string encrypt(string s);
	char leftRotate(unsigned char n, size_t nTimes);
	char rightRotate(unsigned char n, size_t nTimes);
	CString GetTimeInDays();
	BOOL OnlyOneInstance();

	BOOL  IsAdministrator();
	void  ElevateProcess();
	bool  RegistryRoutine();
	CString m_csDateInstalled;
	int  m_nTimeChoice;
	HANDLE m_hOneInstanceMutex;
};

extern CPesoNetTimerApp theApp;
