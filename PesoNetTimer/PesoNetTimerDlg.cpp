
// PesoNetTimerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PesoNetTimer.h"
#include "PesoNetTimerDlg.h"
#include "afxdialogex.h"
#include <strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPesoNetTimerDlg dialog



CPesoNetTimerDlg::CPesoNetTimerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PESONETTIMER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hThreadTimer = NULL;
	m_hStopThread = NULL;
}

void CPesoNetTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TIME, m_ctrlComboBoxTime);
	DDX_Control(pDX, IDC_STATIC_DISPLAY, m_ctrlStaticDisplay);
}

BEGIN_MESSAGE_MAP(CPesoNetTimerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPesoNetTimerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPesoNetTimerDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_DISPLAY_COUNT_DOWN, &CPesoNetTimerDlg::OnDisplayCountDown)
	ON_MESSAGE(WM_TRAYICON_EVENT, &CPesoNetTimerDlg::OnTrayIconEvent)
END_MESSAGE_MAP()


// CPesoNetTimerDlg message handlers

BOOL CPesoNetTimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.
	RECT rect;
	GetClientRect(&rect);

	rect.bottom += 35;
	rect.right += 10;
	//SetWindowPos(&this->wndTopMost, rect.left, rect.top, rect.right, rect.bottom, SWP_NOMOVE | SWP_NOSIZE);
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	InitializeTime();
	EnableCloseButton(FALSE);
	MoveWindow(&rect);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPesoNetTimerDlg::InitializeTime()
{
	CTimeChoice oTimeChoice;
	DWORD dwMilliSec = 60000;

	oTimeChoice.csTimeText = _T("30 seconds");
	oTimeChoice.dwMilliseconds = 30000;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("1 minute");
	oTimeChoice.dwMilliseconds = dwMilliSec;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("2 minutes");
	oTimeChoice.dwMilliseconds = dwMilliSec * 2;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("3 minutes");
	oTimeChoice.dwMilliseconds = dwMilliSec * 3;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("5 minutes");
	oTimeChoice.dwMilliseconds = dwMilliSec * 5;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("10 minutes");
	oTimeChoice.dwMilliseconds = dwMilliSec * 10;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("15 minutes");
	oTimeChoice.dwMilliseconds = dwMilliSec * 15;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("20 minutes");
	oTimeChoice.dwMilliseconds = dwMilliSec * 20;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("25 minutes");
	oTimeChoice.dwMilliseconds = dwMilliSec * 25;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("30 minutes");
	oTimeChoice.dwMilliseconds = dwMilliSec * 30;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("45 minutes");
	oTimeChoice.dwMilliseconds = dwMilliSec * 45;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("1 hour");
	oTimeChoice.dwMilliseconds = dwMilliSec * 60 * 1;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("2 hour");
	oTimeChoice.dwMilliseconds = dwMilliSec * 60 * 2;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("3 hour");
	oTimeChoice.dwMilliseconds = dwMilliSec * 60 * 3;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("4 hour");
	oTimeChoice.dwMilliseconds = dwMilliSec * 60 * 4;
	m_vTimeChoice.push_back(oTimeChoice);

	oTimeChoice.csTimeText = _T("5 hour");
	oTimeChoice.dwMilliseconds = dwMilliSec * 60 * 5;
	m_vTimeChoice.push_back(oTimeChoice);


	for (int i = 0; i < m_vTimeChoice.size(); i++)
	{
		m_ctrlComboBoxTime.AddString(m_vTimeChoice[i].csTimeText);
	}
	m_ctrlComboBoxTime.SetCurSel(0);
}

void CPesoNetTimerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID & SC_MINIMIZE) == SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
		CDialogEx::OnSysCommand(nID, lParam);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPesoNetTimerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPesoNetTimerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

unsigned _stdcall CPesoNetTimerDlg::TimerThread(void* args)
{
	CPesoNetTimerDlg* pDlg = (CPesoNetTimerDlg*)args;
	LASTINPUTINFO lastInput;
	DWORD dwTickCount = 0;
	DWORD dwSelectedTimer = pDlg->m_dwSelectedTime;
	DWORD dwTimeIdle = 0;
	BOOL bRet = 0;
	ZeroMemory(&lastInput, sizeof(lastInput));
	lastInput.cbSize = sizeof(lastInput);
	
	do
	{
		bRet = GetLastInputInfo(&lastInput);
		dwTickCount = GetTickCount();
		dwTimeIdle = dwTickCount - lastInput.dwTime;
		if ((dwSelectedTimer - dwTimeIdle) < 10000)
		{
			pDlg->ShowWindow(SW_NORMAL);
			pDlg->SetFocus();
			pDlg->SetForegroundWindow();
			::SendMessage(pDlg->GetSafeHwnd(), WM_DISPLAY_COUNT_DOWN, (dwSelectedTimer - dwTimeIdle)/1000, SW_NORMAL);
			if ((dwSelectedTimer - dwTimeIdle) < 1000)
			{
				system("shutdown /s /f /t 0");
			}
		}
		else
		{
			::SendMessage(pDlg->GetSafeHwnd(), WM_DISPLAY_COUNT_DOWN, (dwSelectedTimer - dwTimeIdle) / 1000, SW_HIDE);
		}
		Sleep(1);
	} while (WaitForSingleObject(pDlg->m_hStopThread, 0) != WAIT_OBJECT_0);
	return 0;
}

void CPesoNetTimerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialogEx::OnOK();
	//shutdown /s /t 0
	m_dwSelectedTime = m_vTimeChoice[m_ctrlComboBoxTime.GetCurSel()].dwMilliseconds;
	if (m_hThreadTimer)
	{
		SetEvent(m_hStopThread);
		while (::MsgWaitForMultipleObjects(1, &m_hThreadTimer, FALSE, INFINITE,
			QS_SENDMESSAGE) == WAIT_OBJECT_0 + 1)
		{
			MSG message;
			::PeekMessage(&message, 0, 0, 0, PM_NOREMOVE);
		}
		CloseHandle(m_hThreadTimer);
		CloseHandle(m_hStopThread);
		m_hThreadTimer = NULL;
		m_hStopThread = NULL;
	}
	m_hStopThread = CreateEvent(NULL, TRUE, FALSE,NULL);
	m_hThreadTimer = (HANDLE)_beginthreadex(NULL,0, TimerThread,this,0, NULL);
	
	ShowWindow(SW_HIDE);
	CreateTrayIcon();
	SetTrayIconTipText(TEXT("Enzo Tech Peso-Net Timer"));
}

void CPesoNetTimerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (m_hThreadTimer)
	{
		SetEvent(m_hStopThread);
		while (::MsgWaitForMultipleObjects(1, &m_hThreadTimer, FALSE, INFINITE,
			QS_SENDMESSAGE) == WAIT_OBJECT_0 + 1)
		{
			MSG message;
			::PeekMessage(&message, 0, 0, 0, PM_NOREMOVE);
		}
		CloseHandle(m_hThreadTimer);
		CloseHandle(m_hStopThread);
		m_hThreadTimer = NULL;
		m_hStopThread = NULL;
	}
	DestroyTrayIcon();
	OnOK();
}

afx_msg LRESULT CPesoNetTimerDlg::OnDisplayCountDown(WPARAM wParam, LPARAM lParam)
{
	if (lParam == SW_NORMAL)
	{
		CString csText;
		if (wParam <= 1)
			csText.Format(_T("PC is shutting down in %d second."), (INT)(wParam));
		else
			csText.Format(_T("PC is shutting down in %d seconds."), (INT)(wParam));
		m_ctrlStaticDisplay.SetWindowText(csText);
	}
	else if(lParam == SW_HIDE)
	{
		m_ctrlStaticDisplay.SetWindowText(_T(""));
	}
	return 0;
}

void CPesoNetTimerDlg::EnableCloseButton(bool bEnable)
{
	UINT nMenuf = bEnable ? (MF_BYCOMMAND) : (MF_BYCOMMAND | MF_GRAYED | MF_DISABLED);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu)
		pSysMenu->EnableMenuItem(SC_CLOSE, nMenuf);
}

BOOL CPesoNetTimerDlg::CreateTrayIcon()
{
	memset(&m_NID, 0, sizeof(m_NID));
	m_NID.cbSize = sizeof(m_NID);

	// set tray icon ID
	m_NID.uID = ID_SYSTEMTRAY;

	// set handle to the window that receives tray icon notifications
	ASSERT(::IsWindow(GetSafeHwnd()));
	m_NID.hWnd = GetSafeHwnd();

	// set message that will be sent from tray icon to the window 
	m_NID.uCallbackMessage = WM_TRAYICON_EVENT;

	// fields that are being set when adding tray icon 
	m_NID.uFlags = NIF_MESSAGE | NIF_ICON;

	// set image
	m_NID.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	if (!m_NID.hIcon)
		return FALSE;

	m_NID.uVersion = NOTIFYICON_VERSION;

	if (!Shell_NotifyIcon(NIM_ADD, &m_NID))
		return FALSE;

	return Shell_NotifyIcon(NIM_SETVERSION, &m_NID);
}

BOOL CPesoNetTimerDlg::DestroyTrayIcon()
{
	return Shell_NotifyIcon(NIM_DELETE, &m_NID);
}

BOOL CPesoNetTimerDlg::SetTrayIconTipText(LPCTSTR pszText)
{
	if (StringCchCopy(m_NID.szTip, sizeof(m_NID.szTip), pszText) != S_OK)
		return FALSE;

	m_NID.uFlags |= NIF_TIP;
	return Shell_NotifyIcon(NIM_MODIFY, &m_NID);
}

BOOL CPesoNetTimerDlg::ShowTrayIconBalloon(LPCTSTR pszTitle, LPCTSTR pszText, UINT unTimeout, DWORD dwInfoFlags)
{
	m_NID.uFlags |= NIF_INFO;
	m_NID.uTimeout = unTimeout;
	m_NID.dwInfoFlags = dwInfoFlags;

	if (StringCchCopy(m_NID.szInfoTitle, sizeof(m_NID.szInfoTitle), pszTitle) != S_OK)
		return FALSE;

	if (StringCchCopy(m_NID.szInfo, sizeof(m_NID.szInfo), pszText) != S_OK)
		return FALSE;

	return Shell_NotifyIcon(NIM_MODIFY, &m_NID);
}

BOOL CPesoNetTimerDlg::SetTrayIcon(HICON hIcon)
{
	m_NID.hIcon = hIcon;
	m_NID.uFlags |= NIF_ICON;

	return Shell_NotifyIcon(NIM_MODIFY, &m_NID);
}

BOOL CPesoNetTimerDlg::SetTrayIcon(WORD wIconID)
{
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(wIconID));

	if (!hIcon)
		return FALSE;

	return SetTrayIcon(hIcon);
}

void CPesoNetTimerDlg::OnDestroy()
{
	
	CDialog::OnDestroy();
}

LRESULT CPesoNetTimerDlg::OnTrayIconEvent(WPARAM wParam, LPARAM lParam)
{
	if ((UINT)wParam != ID_SYSTEMTRAY)
		return ERROR_SUCCESS;

	switch ((UINT)lParam)
	{
		case WM_MOUSEMOVE:
		{
			// do something
			// e.g. save mouse position in time of event
			GetCursorPos(&m_ptMouseHoverEvent);
			break;
		}
		case WM_LBUTTONDBLCLK:
		{
			// do something
			// e.g. save mouse position in time of event
			ShowWindow(SW_NORMAL);
			DestroyTrayIcon();
			break;
		}
/*		case WM_LBUTTONUP:
		{
			// e.g. show main dialog or set (new) tip text and display baloon:

			CTime timeCurr = CTime::GetCurrentTime();
			CString strTimeCurr;
			strTimeCurr.Format(TEXT("%d:%d:%d"), timeCurr.GetHour(), timeCurr.GetMinute(), timeCurr.GetSecond());
			CString strText(TEXT("This text was set at "));
			strText += strTimeCurr;

			SetTrayIconTipText((LPCTSTR)strText);

			ShowTrayIconBalloon(TEXT("Baloon message title"), TEXT("Left click!"), 1000, NIIF_INFO);

			break;
		}
		case WM_RBUTTONUP:
		{
			// e.g. show context menu or disable tip and display baloon:

			SetTrayIconTipText((LPCTSTR)TEXT(""));
			ShowTrayIconBalloon(TEXT("Baloon message title"), TEXT("Right click!"), 1000, NIIF_INFO);
			break;
		}*/
	}

	return ERROR_SUCCESS;
}
