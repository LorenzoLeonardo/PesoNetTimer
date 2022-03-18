
// PesoNetTimerDlg.h : header file
//

#pragma once
#include <vector>
using namespace std;

#define WM_DISPLAY_COUNT_DOWN WM_USER + 1
#define WM_TRAYICON_EVENT (WM_APP + 2)


#define ID_SYSTEMTRAY 0x1000
#define ID_ICON_TRAY_INITIAL 0x2000


class CMyStatic : public CStatic
{
public:
	CMyStatic();
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT	message);
};

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

	CToolTipCtrl* m_pToolTipCtrl;
protected:
	// tray icon data
	NOTIFYICONDATA m_NID;

	CPoint m_ptMouseHoverEvent;

	// adds icon to the system tray
	BOOL CreateTrayIcon();

	// displays rectangular tip
	BOOL SetTrayIconTipText(LPCTSTR pszText);

	// displays baloon notification
	// unTimeout is in milliseconds
	BOOL ShowTrayIconBalloon(LPCTSTR pszTitle, LPCTSTR pszText, UINT unTimeout, DWORD dwInfoFlags);

	// sets icon resource
	BOOL SetTrayIcon(HICON hIcon);

	// sets icon resource
	BOOL SetTrayIcon(WORD wIconID);

	// removes icon from tray
	BOOL DestroyTrayIcon();

	afx_msg LRESULT OnTrayIconEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	CMyStatic m_ctrlStaticLogo;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnStatic();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};
