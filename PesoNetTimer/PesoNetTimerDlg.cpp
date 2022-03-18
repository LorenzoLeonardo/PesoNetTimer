
// PesoNetTimerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "PesoNetTimer.h"
#include "PesoNetTimerDlg.h"
#include "afxdialogex.h"

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
}

BEGIN_MESSAGE_MAP(CPesoNetTimerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPesoNetTimerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPesoNetTimerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPesoNetTimerDlg message handlers

BOOL CPesoNetTimerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

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
	BOOL bRet = 0;
	ZeroMemory(&lastInput, sizeof(lastInput));
	lastInput.cbSize = sizeof(lastInput);
	
	do
	{
		bRet = GetLastInputInfo(&lastInput);
		dwTickCount = GetTickCount();
		if ((dwTickCount - lastInput.dwTime) >= dwSelectedTimer)
		{
			system("shutdown /s /t 0");
		}
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
		WaitForSingleObject(m_hThreadTimer, INFINITE);
		CloseHandle(m_hThreadTimer);
		CloseHandle(m_hStopThread);
		m_hThreadTimer = NULL;
		m_hStopThread = NULL;
	}
	m_hStopThread = CreateEvent(NULL, TRUE, FALSE,NULL);
	m_hThreadTimer = (HANDLE)_beginthreadex(NULL,0, TimerThread,this,0, NULL);
	

	ShowWindow(SW_MINIMIZE);
}


void CPesoNetTimerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_MINIMIZE);
}

