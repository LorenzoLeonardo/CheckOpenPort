
// CheckOpenPorstDlg.h : header file
//

#pragma once
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <process.h>
#include <mutex>
#include <vector>
#include "EnzTCP.h"

using namespace std;

#define MAX_PORT 5000




typedef  HANDLE(*LPEnumOpenPorts)(const char*, int, FuncFindOpenPort);
typedef  void(*LPCleanEnumOpenPorts)(HANDLE hHandle);
WCHAR* convert_to_wstring(const char* str);
char* convert_from_wstring(const WCHAR* wstr);
// CCheckOpenPortsDlg dialog
class CCheckOpenPortsDlg : public CDialogEx
{
// Construction
public:
	CCheckOpenPortsDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECKOPENPORST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	LPEnumOpenPorts m_pfnPtrEnumOpenPorts;
	LPCleanEnumOpenPorts m_pfnPtrCleanEnumOpenPorts;


// Implementation
protected:
	HICON m_hIcon;

	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	bool m_bStopLoop;
	bool m_bIsRunning;
	vector<CString> m_vList;
	CString m_IPAddress;
	int m_nThread;
	vector<thread*> v_Thread;
	thread* m_tMonitor;
	HMODULE dll_handle;
	HANDLE m_hHandle;
public:
	afx_msg void OnBnClickedButtonPort();
	string UnicodeToMultiByte(wstring& wstr);
	HANDLE Handle[MAX_PORT];
	CIPAddressCtrl m_ctrlIPAddress;
	CEdit m_ctrlResult;

	vector<thread*> GetHandles()
	{
		return v_Thread;
	}
	void SetThreadRunning(bool b)
	{
		m_bIsRunning = b;
	}
	void Increment();
	void AddToList(CString s);
	void PrintList();
	void PrintList(CString csInput);
	bool IsStopped()
	{
		return m_bStopLoop;
	}
	bool IsThreadRunning()
	{
		return m_bIsRunning;
	}
	CString GetIPAddress()
	{
		return m_IPAddress;
	}
	afx_msg void OnBnClickedButton2();
	CEdit m_ctrlPortNum;
	afx_msg void OnBnClickedButtonCheckport();
protected:
	CProgressCtrl m_ctrlProgressStatus;
public:
	afx_msg void OnClose();
protected:
	CButton m_ctrlBtnCheckOpenPorts;
};
