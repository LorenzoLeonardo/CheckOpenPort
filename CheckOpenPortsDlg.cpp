
// CheckOpenPorstDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CheckOpenPorts.h"
#include "CheckOpenPortsDlg.h"
#include "afxdialogex.h"


#pragma comment(lib, "Ws2_32.lib")
typedef struct
{
	HANDLE handle;
	int nPort;
}TMONITOR;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HANDLE m_hExit;
CCheckOpenPortsDlg* g_dlgPtr = NULL;
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
	//AfxGetThread()->PumpMessage();
	//m_ctrlResult.SetFocus();
	//m_ctrlResult.SetSel(-1);

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
}

void CCheckOpenPortsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_IP, m_ctrlIPAddress);
	DDX_Control(pDX, IDC_EDIT_AREA, m_ctrlResult);
	DDX_Control(pDX, IDC_EDITPORT, m_ctrlPortNum);
	DDX_Control(pDX, IDC_PROGRESS_STATUS, m_ctrlProgressStatus);
	DDX_Control(pDX, IDC_BUTTON_PORT, m_ctrlBtnCheckOpenPorts);
}

BEGIN_MESSAGE_MAP(CCheckOpenPortsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PORT, &CCheckOpenPortsDlg::OnBnClickedButtonPort)
	ON_BN_CLICKED(IDC_BUTTON2, &CCheckOpenPortsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_CHECKPORT, &CCheckOpenPortsDlg::OnBnClickedButtonCheckport)
	ON_WM_CLOSE()
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

bool CCheckOpenPortsDlg::IsPortOpen(TCHAR* ipAdd, TCHAR* port)
{
	//mtx.lock();
	WSADATA wsaData;
	int iResult;
	SOCKET ConnectSocket = INVALID_SOCKET;

	// Initialize Winsock

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		//mtx.unlock();
		return false;
	}
	printf("WSAStartup\r\n");

	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	CString cs;
	m_ctrlIPAddress.GetWindowText(cs);

	// Resolve the server address and port
	wstring ipAddress(ipAdd);
	wstring nPort(port);
	iResult = getaddrinfo(UnicodeToMultiByte(ipAddress).c_str(), UnicodeToMultiByte(nPort).c_str(), &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		//mtx.unlock();
		return false;
	}
	printf("getaddrinfo\r\n");
	// Attempt to connect to the first address returned by
// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
		ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		//mtx.unlock();
		return false;
	}
	printf("socket\r\n");

	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		freeaddrinfo(result);
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		//mtx.unlock();
		return false;
	}
	freeaddrinfo(result);
	closesocket(ConnectSocket);
	WSACleanup();
	//mtx.unlock();
	return true;
}
void CCheckOpenPortsDlg::Increment()
{
	m_nThread++;
	m_ctrlProgressStatus.SetPos(m_nThread);
	
}

void ThreadMultiFunc(LPVOID pParam)
{
	CString cs;
	TMONITOR* tMon = (TMONITOR *) pParam;
	
	if (!g_dlgPtr->IsStopped())
	{

		cs = g_dlgPtr->GetIPAddress();
		CString csRes;
		CString csPort;

		csPort.Format(_T("%d"), tMon->nPort);

		if (g_dlgPtr->IsPortOpen(cs.GetBuffer(), csPort.GetBuffer()))
		{
			if (g_dlgPtr->IsStopped())
			{
				delete tMon;
				tMon = NULL;
			}
			csRes = _T("Port (") + csPort + _T(") Of (") + cs + _T(") is open.\r\n");
			g_dlgPtr->AddToList(csRes);
			g_dlgPtr->PrintList(csRes);
		}
		//else
		//{
		//	csRes = +_T("Port (") + csPort + _T(") Of (") + cs + _T(") is closed.\r\n");
		//	g_dlgPtr->PrintList(csRes);
		//}
		if (g_dlgPtr->IsStopped())
		{
			delete tMon;
			tMon = NULL;
		}
		else
		{
			g_dlgPtr->Increment();
			delete tMon;
			tMon = NULL;
		}
	}

	if (tMon != NULL)
	{
		delete tMon;
		tMon = NULL;
	}
	return;
}

void ThreadMonitorThreads(LPVOID pParam)
{
	vector<thread*>* PDlg = (vector<thread*>*)pParam;
	
	for (int i = 0; i < (*PDlg).size(); i++)
		(* PDlg)[i]->join();

	for (int i = 0; i < (*PDlg).size(); i++)
		delete (* PDlg)[i];

	(*PDlg).clear();
	g_dlgPtr->PrintList();
	return;
}
void CCheckOpenPortsDlg::OnBnClickedButtonPort()
{
	m_ctrlBtnCheckOpenPorts.EnableWindow(FALSE);
	m_bStopLoop = false;
	if (!v_Thread.empty())
		return;
	m_vList.clear();
	if (m_tMonitor != NULL)
	{
		delete m_tMonitor;
		m_tMonitor = NULL;
	}
	m_ctrlResult.SetWindowText(_T(""));

	m_ctrlProgressStatus.ShowWindow(TRUE);
	m_ctrlProgressStatus.SetRange(0, MAX_PORT);
	m_nThread = MAX_PORT/10;
	m_ctrlIPAddress.GetWindowText(m_IPAddress);
	g_dlgPtr = this;
	
	for (int i = 0; i < MAX_PORT && !m_bStopLoop; i++)
	{
		TMONITOR* tMon = new TMONITOR;
		tMon->nPort = i;
		m_ctrlProgressStatus.SetPos(m_nThread);
		AfxGetThread()->PumpMessage();
		thread *t = new thread(ThreadMultiFunc, tMon);
		v_Thread.push_back(t);
	}

	m_tMonitor = new thread(ThreadMonitorThreads, &v_Thread);
	m_tMonitor->detach();
	
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
	if (IsPortOpen(cs.GetBuffer(), csPort.GetBuffer()))
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
	if (m_tMonitor != NULL)
	{
		delete m_tMonitor;
		m_tMonitor = NULL;
	}
	CDialogEx::OnClose();
}
