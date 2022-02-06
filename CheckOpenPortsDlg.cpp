
// CheckOpenPorstDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CheckOpenPorts.h"
#include "CheckOpenPortsDlg.h"
#include "afxdialogex.h"



CCheckOpenPortsDlg* g_dlg;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CCheckOpenPortsDlg dialog

mutex mtx;

void CCheckOpenPortsDlg::AddToList(CString s)
{
	//mtx.lock();
	m_vList.push_back(s);
	//mtx.unlock();
}
void CCheckOpenPortsDlg::PrintList(CString csInput)
{
	mtx.lock();
	long nLength = m_ctrlResult.GetWindowTextLength();
	m_ctrlResult.SetSel(0, 0);
	m_ctrlResult.ReplaceSel(csInput);
	mtx.unlock();
}
void CCheckOpenPortsDlg::PrintList()
{
//	m_ctrlResult.SetWindowText(_T(""));
	//CString csRes = _T("");
	if (m_vList.empty())
		m_ctrlResult.SetWindowText(_T("No open ports found."));
/*	else
	{
		for (int i = 0; i < m_vList.size(); i++)
			csRes += m_vList[i];
		m_ctrlResult.SetWindowText(csRes);
	}*/
	m_ctrlResult.SetFocus();
	m_ctrlResult.SetSel(-1);
	m_ctrlProgressStatus.ShowWindow(FALSE);
	m_ctrlBtnCheckOpenPorts.EnableWindow(TRUE);
}

CCheckOpenPortsDlg::CCheckOpenPortsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHECKOPENPORST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_tMonitor = NULL;
	m_nCurrentRowSelected = -1;
	m_bHasClickClose = FALSE;
}

void CCheckOpenPortsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_IP, m_ctrlIPAddress);
	DDX_Control(pDX, IDC_EDIT_AREA, m_ctrlResult);
	DDX_Control(pDX, IDC_EDITPORT, m_ctrlPortNum);
	DDX_Control(pDX, IDC_PROGRESS_STATUS, m_ctrlProgressStatus);
	DDX_Control(pDX, IDC_BUTTON_PORT, m_ctrlBtnCheckOpenPorts);
	DDX_Control(pDX, IDC_LIST_LAN, m_ctrlLANConnected);
	DDX_Control(pDX, IDC_EDIT_POLLINGTIME, m_ctrlEditPollingTime);
	DDX_Control(pDX, IDC_BUTTON_LISTEN_LAN, m_ctrlBtnListen);
	DDX_Control(pDX, IDC_BUTTON_STOP_LAN, m_ctrlBtnStopListening);
}

BEGIN_MESSAGE_MAP(CCheckOpenPortsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PORT, &CCheckOpenPortsDlg::OnBnClickedButtonPort)
	ON_BN_CLICKED(IDC_BUTTON2, &CCheckOpenPortsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_CHECKPORT, &CCheckOpenPortsDlg::OnBnClickedButtonCheckport)
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_EDIT_AREA, &CCheckOpenPortsDlg::OnEnChangeEditArea)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN_LAN, &CCheckOpenPortsDlg::OnBnClickedButtonListenLan)
	ON_BN_CLICKED(IDC_BUTTON_STOP_LAN, &CCheckOpenPortsDlg::OnBnClickedButtonStopLan)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LAN, &CCheckOpenPortsDlg::OnNMClickListLan)
	ON_WM_KEYDOWN()
	ON_NOTIFY(HDN_ITEMKEYDOWN, 0, &CCheckOpenPortsDlg::OnHdnItemKeyDownListLan)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_LAN, &CCheckOpenPortsDlg::OnLvnKeydownListLan)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LAN, &CCheckOpenPortsDlg::OnNMDblclkListLan)
END_MESSAGE_MAP()


// CCheckOpenPortsDlg message handlers

BOOL CCheckOpenPortsDlg::OnInitDialog()
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

	m_ctrlIPAddress.SetWindowText(_T("192.168.0.1"));
	SetThreadRunning(false);
	m_ctrlProgressStatus.ShowWindow(FALSE);
	m_ctrlPortNum.SetWindowText(_T("80"));
	m_ctrlEditPollingTime.SetWindowText(_T("5000"));
	m_ctrlBtnStopListening.EnableWindow(FALSE);
	m_ctrlLANConnected.SetExtendedStyle(LVS_EX_FULLROWSELECT| LVS_EX_GRIDLINES);

	dll_handle = LoadLibrary(L"EnzTCP.dll");
	if (dll_handle)
	{
		m_pfnPtrEnumOpenPorts = (LPEnumOpenPorts)GetProcAddress(dll_handle, "EnumOpenPorts");
		m_pfnPtrIsPortOpen = (LPIsPortOpen)GetProcAddress(dll_handle, "IsPortOpen");
		m_pfnPtrStartLocalAreaListening = (FNStartLocalAreaListening)GetProcAddress(dll_handle, "StartLocalAreaListening");
		m_pfnPtrStopLocalAreaListening = (FNStopLocalAreaListening)GetProcAddress(dll_handle, "StopLocalAreaListening");
	}

	LPCTSTR lpcRecHeader[] = { _T("No."), _T("IP Address"), _T("HostName"), _T("MAC Address") };
	int nCol = 0;

	m_ctrlLANConnected.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_FIXED_WIDTH, 30);
	m_ctrlLANConnected.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_LEFT, 90);
	m_ctrlLANConnected.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_LEFT, 90);
	m_ctrlLANConnected.InsertColumn(nCol, lpcRecHeader[nCol++], LVCFMT_LEFT, 120);
	g_dlg = this;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCheckOpenPortsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCheckOpenPortsDlg::OnPaint()
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
HCURSOR CCheckOpenPortsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
string CCheckOpenPortsDlg::UnicodeToMultiByte(wstring& wstr)
{
	if (wstr.empty()) 
		return string();

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

void CCheckOpenPortsDlg::Increment()
{
	m_nThread++;
	m_ctrlProgressStatus.SetPos(m_nThread);

	if(m_nThread >= MAX_PORT)
	{
		m_ctrlBtnCheckOpenPorts.EnableWindow(TRUE);
		m_ctrlProgressStatus.ShowWindow(FALSE);
	}
	
}
char* convert_from_wstring(const WCHAR* wstr)
{
	int wstr_len = (int)wcslen(wstr);
	int num_chars = WideCharToMultiByte(CP_UTF8, 0, wstr, wstr_len, NULL, 0, NULL, NULL);
	CHAR* strTo = (CHAR*)malloc((num_chars + 1) * sizeof(CHAR));
	if (strTo)
	{
		WideCharToMultiByte(CP_UTF8, 0, wstr, wstr_len, strTo, num_chars, NULL, NULL);
		strTo[num_chars] = '\0';
	}
	return strTo;
}
WCHAR* convert_to_wstring(const char* str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), NULL, 0);
	WCHAR* wstrTo = (WCHAR*)malloc((size_needed + 1) * sizeof(WCHAR));
	memset(wstrTo, 0, (size_needed+1) * sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)strlen(str), wstrTo, size_needed);

	return wstrTo;
}
void GetLastErrorMessageString(wstring &str, int nGetLastError)
{
	DWORD dwSize = 0;
	TCHAR lpMessage[0xFF];

	dwSize = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,   // flags
		NULL,                // lpsource
		nGetLastError,                 // message id
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),    // languageid
		lpMessage,              // output buffer
		sizeof(lpMessage),     // size of msgbuf, bytes
		NULL);

	str = lpMessage;
}
void CallbackLANListener(const char* ipAddress, const char* hostName, const char* macAddress, bool bIsopen)
{
	mtx.lock();
	if (strcmp(ipAddress, "end") == 0)
	{
		g_dlg->m_ctrlLANConnected.DeleteAllItems();

		int col = 0;
		map<ULONG, vector<string>>::iterator it = g_dlg->m_mConnected.begin();
		int nRow = 0;
		CString csIPAddress;
		char szIPAddress[32];
	
	
		while (it != g_dlg->m_mConnected.end())
		{
			inet_ntop(AF_INET, (const void*)&(it->first), szIPAddress, sizeof(szIPAddress));
			csIPAddress = szIPAddress;

			g_dlg->m_ctrlLANConnected.InsertItem(LVIF_TEXT | LVIF_STATE, nRow,
				to_wstring(nRow+1).c_str(), 0,0, 0, 0);

			g_dlg->m_ctrlLANConnected.SetItemText(nRow, col + 1, csIPAddress);
			g_dlg->m_ctrlLANConnected.SetItemText(nRow, col + 2, convert_to_wstring(it->second[0].c_str()));
			g_dlg->m_ctrlLANConnected.SetItemText(nRow, col + 3, convert_to_wstring(it->second[1].c_str()));
			it++;
			nRow++;
		}
		if ((g_dlg->m_mConnected.size()-1) < g_dlg->m_nCurrentRowSelected)
			g_dlg->m_nCurrentRowSelected = g_dlg->m_mConnected.size() - 1;

		g_dlg->m_ctrlLANConnected.SetItemState(g_dlg->m_nCurrentRowSelected, LVIS_SELECTED, LVIS_SELECTED);
		g_dlg->m_ctrlLANConnected.SetFocus();
		g_dlg->m_mConnected.clear();
		mtx.unlock();
		return;
	}
	else if (strcmp(ipAddress, "stop") == 0)
	{
		g_dlg->m_ctrlBtnListen.EnableWindow(TRUE);
		g_dlg->SetThreadRunning(false);
		if (g_dlg->HasClickClose())
		{
			FreeLibrary(g_dlg->dll_handle);
			((CDialog*)(g_dlg))->EndDialog(0);
			
		}
	}
	if (bIsopen)
	{
		ULONG ipaddr;

		inet_pton(AF_INET, ipAddress, &ipaddr);
		vector<string> vItem;
		vItem.push_back(hostName);
		vItem.push_back(macAddress);

		g_dlg->m_mConnected[ipaddr] = vItem;
	}

	mtx.unlock();
}
void CallBackEnumPort(char* ipAddress, int nPort, bool bIsopen, int nLastError)
{
	mtx.lock();

	if (ipAddress != NULL)
	{
		CString csStr;
		WCHAR* wr = convert_to_wstring(ipAddress);
		wstring wsLastError;
		GetLastErrorMessageString(wsLastError, nLastError);
		if (bIsopen)
			csStr.Format(_T("%s %d is open.\r\n"), wr, nPort);
		//	else
		//		csStr.Format(_T("%s %d %s."), wr, nPort, wsLastError.c_str());
		free(wr);
		long nLength = g_dlg->m_ctrlResult.GetWindowTextLength();
		g_dlg->m_ctrlResult.SetSel(0, 0);
		g_dlg->m_ctrlResult.ReplaceSel(csStr);
		g_dlg->Increment();
	}
	mtx.unlock();
}

void CCheckOpenPortsDlg::OnBnClickedButtonPort()
{
	m_ctrlBtnCheckOpenPorts.EnableWindow(FALSE);
	m_ctrlResult.SetWindowText(_T(""));
	m_ctrlProgressStatus.ShowWindow(TRUE);
	m_ctrlProgressStatus.SetRange(1, MAX_PORT);
	m_nThread = 0;
	m_ctrlIPAddress.GetWindowText(m_IPAddress);
	wstring strIP(m_IPAddress.GetBuffer());

	m_pfnPtrEnumOpenPorts(UnicodeToMultiByte(strIP).c_str(), MAX_PORT, CallBackEnumPort);
}


void CCheckOpenPortsDlg::OnBnClickedButton2()
{
	m_bStopLoop = true;
//	PrintList();
}


void CCheckOpenPortsDlg::OnBnClickedButtonCheckport()
{

	// TODO: Add your control notification handler code here
	CString cs;
	CString csPort;

	m_ctrlIPAddress.GetWindowText(cs);
	m_ctrlPortNum.GetWindowText(csPort);
	int nLastError = 0;
	wstring wStr(cs.GetBuffer());

	if (m_pfnPtrIsPortOpen(UnicodeToMultiByte(wStr).c_str(), _ttoi(csPort), &nLastError))
	{
		CString csRes;
		m_ctrlResult.GetWindowText(csRes);
		csRes += +_T("Port (") + csPort + _T(") Of (") + cs + _T(") is open.\r\n");
		m_ctrlResult.SetWindowText(csRes);
	}
	else
	{
		CString csRes;
		m_ctrlResult.GetWindowText(csRes);
		csRes += +_T("Port (") + csPort + _T(") Of (") + cs + _T(") is closed.\r\n");
		m_ctrlResult.SetWindowText(csRes);
	}

}


void CCheckOpenPortsDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_pfnPtrStopLocalAreaListening();
	m_bHasClickClose = TRUE;

	if (!m_bIsRunning)
		CDialog::OnClose();
	else
		AfxMessageBox(_T("Still busy scanning. Please wait."));
}


void CCheckOpenPortsDlg::OnEnChangeEditArea()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CCheckOpenPortsDlg::OnBnClickedButtonListenLan()
{
	// TODO: Add your control notification handler code here
	CString csText;
	CString csPollTime;

	m_ctrlBtnListen.EnableWindow(FALSE);
	m_ctrlBtnStopListening.EnableWindow(TRUE);
	m_vList.clear();
	SetThreadRunning(true);
	m_ctrlIPAddress.GetWindowText(csText);
	wstring wstr(csText.GetBuffer());
	string str = UnicodeToMultiByte(wstr);
	m_ctrlEditPollingTime.GetWindowText(csPollTime);
	if (csPollTime.IsEmpty())
	{
		m_ctrlEditPollingTime.SetWindowText(_T("5000"));
		m_pfnPtrStartLocalAreaListening(str.c_str(), CallbackLANListener, 5000);
	}
	else
	{
		int nPollTime = _ttoi(csPollTime);
		m_pfnPtrStartLocalAreaListening(str.c_str(), CallbackLANListener, nPollTime);
	}
}


void CCheckOpenPortsDlg::OnBnClickedButtonStopLan()
{
	// TODO: Add your control notification handler code here
	m_pfnPtrStopLocalAreaListening();
	m_ctrlBtnStopListening.EnableWindow(FALSE);
}


void CCheckOpenPortsDlg::OnNMClickListLan(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_nCurrentRowSelected = pNMItemActivate->iItem;
	
	
	if (pNMItemActivate->iItem > -1)
	{
		CString cs = m_ctrlLANConnected.GetItemText(m_nCurrentRowSelected, 1);
		m_ctrlIPAddress.SetWindowText(cs);
	}
}


void CCheckOpenPortsDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	g_dlg->m_ctrlLANConnected.SetItemState(g_dlg->m_nCurrentRowSelected, LVIS_SELECTED, LVIS_SELECTED);
	g_dlg->m_ctrlLANConnected.SetFocus();
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CCheckOpenPortsDlg::OnHdnItemKeyDownListLan(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here

	OnNMClickListLan(pNMHDR, pResult);
}


void CCheckOpenPortsDlg::OnLvnKeydownListLan(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here

	if (pLVKeyDow->wVKey == VK_UP)
	{
		if(g_dlg->m_nCurrentRowSelected > 0)
		{
			g_dlg->m_nCurrentRowSelected--;
			g_dlg->m_ctrlLANConnected.SetItemState(m_nCurrentRowSelected, LVIS_SELECTED, LVIS_SELECTED);
			g_dlg->m_ctrlLANConnected.SetFocus();
			CString cs = m_ctrlLANConnected.GetItemText(m_nCurrentRowSelected, 1);
			m_ctrlIPAddress.SetWindowText(cs);
		}
	}
	else if (pLVKeyDow->wVKey == VK_DOWN)
	{
		if (g_dlg->m_nCurrentRowSelected < (g_dlg->m_ctrlLANConnected.GetItemCount()-1))
		{
			g_dlg->m_nCurrentRowSelected++;
			g_dlg->m_ctrlLANConnected.SetItemState(m_nCurrentRowSelected, LVIS_SELECTED, LVIS_SELECTED);
			g_dlg->m_ctrlLANConnected.SetFocus();
			CString cs = m_ctrlLANConnected.GetItemText(m_nCurrentRowSelected, 1);
			m_ctrlIPAddress.SetWindowText(cs);
		}
	}

}


void CCheckOpenPortsDlg::OnNMDblclkListLan(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	if (pNMItemActivate->iItem >= 0)
	{
		OnBnClickedButtonPort();
	}
}
