
// PesoNetTimer.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "PesoNetTimer.h"
#include "PesoNetTimerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPesoNetTimerApp

BEGIN_MESSAGE_MAP(CPesoNetTimerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPesoNetTimerApp construction

CPesoNetTimerApp::CPesoNetTimerApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CPesoNetTimerApp object

CPesoNetTimerApp theApp;


// CPesoNetTimerApp initialization
BOOL  CPesoNetTimerApp::IsAdministrator()
{
	BOOL fIsRunAsAdmin = FALSE;
	DWORD dwError = ERROR_SUCCESS;
	PSID pAdministratorsGroup = NULL;

	// Allocate and initialize a SID of the administrators group.
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pAdministratorsGroup))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

	if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

Cleanup:
	if (pAdministratorsGroup)
	{
		FreeSid(pAdministratorsGroup);
		pAdministratorsGroup = NULL;
	}

	if (ERROR_SUCCESS != dwError)
	{
		throw dwError;
	}

	return fIsRunAsAdmin;
}

void  CPesoNetTimerApp::ElevateProcess()
{
	BOOL bAlreadyRunningAsAdministrator = FALSE;
	try
	{
		bAlreadyRunningAsAdministrator = IsAdministrator();
	}
	catch (...)
	{
		DWORD dwErrorCode = GetLastError();
		TCHAR szMessage[256];
		_stprintf_s(szMessage, ARRAYSIZE(szMessage), L"Error code returned was 0x%08lx", dwErrorCode);
	}
	if (!bAlreadyRunningAsAdministrator)
	{
		wchar_t szPath[MAX_PATH];
		if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath)))
		{
			// Launch itself as admin
			SHELLEXECUTEINFO sei = { sizeof(sei) };
			sei.lpVerb = L"runas";
			sei.lpFile = szPath;
			sei.hwnd = NULL;
			sei.nShow = SW_NORMAL;

			if (!ShellExecuteEx(&sei))
			{
				DWORD dwError = GetLastError();
				if (dwError == ERROR_CANCELLED)
				{
					TCHAR szMessage[256];
					_stprintf_s(szMessage, ARRAYSIZE(szMessage), L"Error code returned was 0x%08lx", dwError);
				}
			}
			else
			{
				_exit(1);  // Quit itself
			}
		}
	}
}

CString CPesoNetTimerApp::GetTimeInDays()
{
	SYSTEMTIME sysTime;
	ULONGLONG ulldays;
	CString sDate;
	DWORD dwError = 0;

	memset(&sysTime, 0, sizeof(sysTime));

	GetLocalTime(&sysTime);
	ulldays = ((sysTime.wYear * 12 * 30) + sysTime.wMonth * 30 + sysTime.wDay);

	sDate.Format(_T("%u"), ulldays);
	return sDate;
}
bool CPesoNetTimerApp::RegistryRoutine()
{
	HKEY hKey;
	DWORD dwError = 0;
	bool bRet = false;
	TCHAR szValue[MAX_PATH];
	DWORD dwSize = sizeof(szValue);
	DWORD dwChoice = 0;
	memset(szValue, 0, dwSize);
	dwError = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGISTRY_PATH, 0, KEY_ALL_ACCESS, &hKey);
	if (dwError != ERROR_SUCCESS)
	{
		if (dwError == ERROR_ACCESS_DENIED)
			MessageBox(NULL, _T("Access denied. Please run the program as Administrator"), _T("Enzo Tech Peso-Net Timer"), MB_ICONERROR);
		else
			MessageBox(NULL, _T("Contact Enzo Tech Computer Solutions for activation of the Tool."), _T("Enzo Tech Peso-Net Timer"), MB_ICONERROR);
		return false;
	}
	RegCloseKey(hKey);

	dwError = RegGetValue(HKEY_LOCAL_MACHINE, REGISTRY_PATH, _T("date"), /*RRF_RT_ANY*/RRF_RT_REG_SZ, NULL, (PVOID)&szValue, &dwSize);
	if (dwError != ERROR_SUCCESS)
	{
		MessageBox(NULL, _T("Contact Enzo Tech Computer Solutions for activation of the Tool."), _T("Enzo Tech Peso-Net Timer"), MB_ICONERROR);
		RegCloseKey(hKey);
		return false;
	}
	dwSize = sizeof(dwChoice);
	dwError = RegGetValue(HKEY_LOCAL_MACHINE, REGISTRY_PATH, _T("timer"), /*RRF_RT_ANY*/RRF_RT_REG_DWORD, NULL, (PVOID)&dwChoice, &dwSize);
	if (dwError != ERROR_SUCCESS)
	{
		MessageBox(NULL, _T("Contact Enzo Tech Computer Solutions for activation of the Tool."), _T("Enzo Tech Peso-Net Timer"), MB_ICONERROR);
		RegCloseKey(hKey);
		return false;
	}

	string sAnswer = decrypt(CW2A(szValue).m_szBuffer);
	if (strcmp(sAnswer.c_str(), CW2A(szValue).m_szBuffer) == 0)
	{
		MessageBox(NULL, _T("Contact Enzo Tech Computer Solutions for activation of the Tool."), _T("Enzo Tech Peso-Net Timer"), MB_ICONERROR);
		return false;
	}
	m_csDateInstalled = CA2W(sAnswer.c_str()).m_szBuffer;

	CString csCurrentTime = GetTimeInDays();

	DWORD dwDateInstalled = _ttol(m_csDateInstalled);
	DWORD dwDateCurrent = _ttol(csCurrentTime);

	if ((dwDateCurrent - dwDateInstalled) >= 7)
	{
		MessageBox(NULL, _T("Software trial has expired, please contact Enzo Tech Computer Solutions for activation of the Tool."), _T("Enzo Tech Peso-Net Timer"), MB_ICONERROR);
		return false;
	}
	m_nTimeChoice = dwChoice;

	return true;
}

char CPesoNetTimerApp::rightRotate(unsigned char n, size_t nTimes)
{
	for (int i = 0; i < nTimes; i++)
	{
		n = (n >> 1) | (n << 7);
	}
	return n;
}

char CPesoNetTimerApp::leftRotate(unsigned char n, size_t nTimes)
{
	for (int i = 0; i < nTimes; i++)
	{
		n = (n << 1) | (n >> 7);
	}
	return n;
}

string CPesoNetTimerApp::encrypt(string s)
{
	string sKey = "061186";
	size_t nLenKey = sKey.length();
	size_t nLen = s.length();
	string sNew = "";

	for (size_t i = 0; i < nLenKey; i++)
	{
		sKey[i] = rightRotate(sKey[i], nLenKey);
	}

	for (size_t i = 0; i < nLen; i++)
	{
		s[i] = rightRotate(s[i], nLen);
	}
	return sKey + s;
}
string CPesoNetTimerApp::decrypt(string s)
{
	string sKey = "061186";
	size_t nLenKey = sKey.length();
	size_t nLen = s.length();
	string sNew = "";

	string sGetKey = s.substr(0, nLenKey);
	for (size_t i = 0; i < nLenKey; i++)
	{
		sGetKey[i] = leftRotate(sGetKey[i], nLenKey);
	}
	if (sGetKey.compare(sKey) != 0)
		return s;

	sNew = s.substr(nLenKey, nLen);
	nLen = sNew.size();
	for (size_t i = 0; i < nLen; i++)
	{
		sNew[i] = leftRotate(sNew[i], nLen);
	}
	return sNew;
}
BOOL CPesoNetTimerApp::OnlyOneInstance()
{
	m_hOneInstanceMutex = CreateMutex(NULL, TRUE, _T("Enzo Tech Peso-Net Timer"));
	if ((m_hOneInstanceMutex != NULL) && (GetLastError() != ERROR_ALREADY_EXISTS))
	{
		return TRUE;
	}
	else
	{
		::MessageBox(NULL, _T("An Instance of this application is already running..."), _T("Enzo Tech Peso-Net Timer"), MB_ICONEXCLAMATION);
		return FALSE;
	}
}
BOOL CPesoNetTimerApp::InitInstance()
{
	if (!OnlyOneInstance())
		return FALSE;
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	if (!IsAdministrator())
	{
	//	CString csMsg;

	//	csMsg = _T("Please elevate the process to make the program work. Do you want to proceed?");
	//	int bRet = ::MessageBox(GetMainWnd()->GetSafeHwnd(), csMsg, _T("Enzo Tech Peso-Net Timer"), MB_YESNO | MB_ICONQUESTION);

	//	if (bRet == IDNO)
	//		return FALSE;
	//	else
			ElevateProcess();
	}
	if (RegistryRoutine())
	{
		CPesoNetTimerDlg dlg;
		dlg.SetChoiceTime(m_nTimeChoice);
		m_pMainWnd = &dlg;
		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
			TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
		}
	}

	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	CloseHandle(m_hOneInstanceMutex);
	return FALSE;
}

