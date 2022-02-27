
// MFCDemoDlg.cpp : implementation file
//
#include "stdafx.h"
#include "framework.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx, public CSerialIO
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


// CMFCDemoDlg dialog



CMFCDemoDlg::CMFCDemoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SERIALNAME, m_ccbSerialName);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_ccbBaudrate);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_EDIT_SEND1, m_ceSendCmd1);
	DDX_Control(pDX, IDC_BUTTON_SEND1, m_btnSendCmd1);
}

BEGIN_MESSAGE_MAP(CMFCDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO_SERIALNAME, &CMFCDemoDlg::OnCbnDropdownComboSerialname)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMFCDemoDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SEND1, &CMFCDemoDlg::OnBnClickedButtonSend1)
END_MESSAGE_MAP()


// CMFCDemoDlg message handlers

BOOL CMFCDemoDlg::OnInitDialog()
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
	m_ccbBaudrate.InsertString(0, (CString)"4800");
	m_ccbBaudrate.InsertString(1, (CString)"9600");
	m_ccbBaudrate.InsertString(2, (CString)"19200");
	m_ccbBaudrate.InsertString(3, (CString)"115200");
	m_ccbBaudrate.SetCurSel(3);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMFCDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCDemoDlg::OnPaint()
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
HCURSOR CMFCDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCDemoDlg::OnCbnDropdownComboSerialname()
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	m_ccbSerialName.ResetContent();

	TCHAR lpTargetPath[5000];
	DWORD test;
	bool gotPort = 0;

	for (int i = 0; i < 255; i++)
	{
		CString str;
		str.Format(_T("%d"), i);
		CString ComName = CString("COM") + CString(str);

		test = QueryDosDevice(ComName, (LPWSTR)lpTargetPath, 2000);

		if (test != 0)
		{
			m_ccbSerialName.AddString((CString)ComName);
			gotPort = 1;
		}

		if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
		{
			lpTargetPath[2000];
			continue;
		}



	}
	if (!gotPort)
		m_ccbSerialName.AddString((CString)"No Active Ports Found");

}


void CMFCDemoDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
	if (bPortOpened == FALSE)
	{
		CString csPortName;
		CString csBaudRate;
		m_ccbSerialName.GetLBText(m_ccbSerialName.GetCurSel(), csPortName);
		m_ccbBaudrate.GetLBText(m_ccbBaudrate.GetCurSel(), csBaudRate);

		OpenPort(csPortName, csBaudRate);

	}
	else
	{
		ClosePort();
	}
}

/**
* \brief Handle Open Serial Port Events
* \param bSuccess
*/
void CMFCDemoDlg::OnEventOpen(BOOL bSuccess)
{
}

/**
* \brief Handle Close Serial Port Events
* \param bSuccess
*/
void CMFCDemoDlg::OnEventClose(BOOL bSuccess)
{
}

/**
* \brief Handle Receive Data from Serial
* \param inPacket
* \param inLength
*/
void CMFCDemoDlg::OnEventRead(char* inPacket, int inLength)
{
}

/**
* \brief Handle Write Data
* \param nWritten
*/
void CMFCDemoDlg::OnEventWrite(int nWritten)
{
}



void CMFCDemoDlg::OnBnClickedButtonSend1()
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	CString	Cmd;
	if (!GetPortActivateValue()) return;

	m_ceSendCmd1.GetWindowText(Cmd);

	TCHAR* cmd = (LPTSTR)(LPCTSTR)Cmd;
	Write((char*)cmd, Cmd.GetLength());
}
