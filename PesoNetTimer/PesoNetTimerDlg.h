
// PesoNetTimerDlg.h : header file
//

#pragma once
#include <vector>
using namespace std;

#define WM_DISPLAY_COUNT_DOWN WM_USER + 1
class CTimeChoice
{
public:
	CString csTimeText;
	DWORD dwMilliseconds;
};

// CPesoNetTimerDlg dialog
class CPesoNetTimerDlg : public CDialogEx
{
// Construction
public:
	CPesoNetTimerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PESONETTIMER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	static unsigned _stdcall TimerThread(void* args);
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	CComboBox m_ctrlComboBoxTime;
	vector<CTimeChoice> m_vTimeChoice;
	void InitializeTime();
	HANDLE m_hThreadTimer;
	HANDLE m_hStopThread;
	DWORD  m_dwSelectedTime;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	afx_msg LRESULT OnDisplayCountDown(WPARAM wParam, LPARAM lParam);
	void EnableCloseButton(bool bEnable);
private:
	CStatic m_ctrlStaticDisplay;
};
