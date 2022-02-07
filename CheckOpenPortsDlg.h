
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
#include <map>
#include <cmath>
using namespace std;

#define MAX_PORT 5000




typedef  void(*LPEnumOpenPorts)(const char*, int, FuncFindOpenPort);
typedef  bool(*LPIsPortOpen)(const char*, int, int*);
typedef void (*FNStartLocalAreaListening)(const char* ipAddress, CallbackLocalAreaListener fnpPtr, int nPollingTime);
typedef void (*FNStopLocalAreaListening)();
typedef bool (*FNStartSNMP)(const char* szAgentIPAddress, const char* szCommunity, int nVersion, DWORD & dwLastError);
typedef smiVALUE (*FNSNMPGet)(const char* szOID, DWORD & dwLastError);
typedef void (*FNEndSNMP)();
typedef bool (*FNGetDefaultGateway)(char szDefaultGateway[]);

WCHAR* convert_to_wstring(const char* str);
char* convert_from_wstring(const WCHAR* wstr);
// CCheckOpenPortsDlg dialog
class CCheckOpenPortsDlg : public CDialogEx
{
// Construction
public:
	CCheckOpenPortsDlg(CWnd* pParent = nullptr);	// standard constructor
	FNSNMPGet m_pfnPtrSNMPGet;
	FNEndSNMP m_pfnPtrEndSNMP;
	FNGetDefaultGateway m_pfnPtrGetDefaultGateway;
	FNStartSNMP m_pfnPtrStartSNMP;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECKOPENPORST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	LPEnumOpenPorts m_pfnPtrEnumOpenPorts;
	LPIsPortOpen m_pfnPtrIsPortOpen;
	FNStartLocalAreaListening m_pfnPtrStartLocalAreaListening;
	FNStopLocalAreaListening m_pfnPtrStopLocalAreaListening;


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
	
	bool m_bHasClickClose;
	CString m_csRouterModel;
	CString m_csRouterBrand;
	CString m_csRouterDescription;


public:
	HMODULE dll_handle;
	int m_nCurrentRowSelected;
	bool HasClickClose()
	{
		return m_bHasClickClose;
	}
	afx_msg void OnBnClickedButtonPort();
	string UnicodeToMultiByte(wstring& wstr);
	HANDLE Handle[MAX_PORT];
	CIPAddressCtrl m_ctrlIPAddress;
	CEdit m_ctrlResult;
	CListCtrl m_ctrlLANConnected;
	map<ULONG, vector<string>> m_mConnected;
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
public:
	afx_msg void OnEnChangeEditArea();

public:
	afx_msg void OnBnClickedButtonListenLan();
	afx_msg void OnBnClickedButtonStopLan();
protected:
	CEdit m_ctrlEditPollingTime;
	HANDLE m_hThreadRouter;
public:
	CButton m_ctrlBtnListen;
	CButton m_ctrlBtnStopListening;
	afx_msg void OnNMClickListLan(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHdnItemKeyDownListLan(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnKeydownListLan(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclkListLan(NMHDR* pNMHDR, LRESULT* pResult);
	void SetRouterBrand(CString cs)
	{
		m_ctrlStaticRouterBrand.SetWindowText(cs);
	}
	void SetRouterName(CString cs)
	{
		m_ctrlStaticRouterName.SetWindowText(cs);
	}
	void SetRouterDescription(CString cs)
	{
		m_ctrlStaticRouterDescription.SetWindowText(cs);
	}
	void SetRouterUpTime(CString cs)
	{
		m_ctrlStaticRouterUpTime.SetWindowText(cs);
	}
private:
	CStatic m_ctrlStaticRouterBrand;
	CStatic m_ctrlStaticRouterName;
	CStatic m_ctrlStaticRouterDescription;
	CStatic m_ctrlStaticRouterUpTime;
};
