
// MFCDemoDlg.cpp : implementation file
//
#include "stdafx.h"
#include "framework.h"
#include "MFCDemo.h"
#include "MFCDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define EVENT_SEND_BY_TIMER 0
//STX
BYTE bSTX[] = { 0x02 };

//CMD
BYTE bMOVL[] = { 0x4D,0x4F,0x56,0x4C };
BYTE bGPOS[] = { 0x47,0x50,0x4F,0x53 };
BYTE bGVEL[] = { 0x47,0x56,0x45,0x4C };
BYTE bSTT[] = { 0x47,0x53,0x54,0x54 };

//OPTION
BYTE bOPT[] = { 0x0,0x0,0x00 };

//DATA
BYTE bDATA[8] = { 0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x01 };

//SYNC/ACK
BYTE bSYNC[] = { 0x16 };
BYTE bACK[] = { 0x06 };

//ETX
BYTE bETX[] = { 0x03 };

//RECEIVE
BYTE bProtocolDataBuffer[18] = { };
BYTE bProtocolData[8] = {};

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
	DDX_Control(pDX, IDC_BUTTON_POS, mBtnPosition);
	DDX_Control(pDX, IDC_BUTTON_VEL, mBtnVelocity);
	DDX_Control(pDX, IDC_BUTTON_MOV, mBtnMove);
	DDX_Control(pDX, IDC_BUTTON_STT, mBtnStatus);
	DDX_Control(pDX, IDC_LIST_RECEIVEDATA, m_listboxRead);
	DDX_Control(pDX, IDC_CHECK_LED1, m_chLed1);
	DDX_Control(pDX, IDC_CHECK_GPIO1, m_chGPIO1);
	DDX_Control(pDX, IDC_CHECK_LED2, m_chLed2);
	DDX_Control(pDX, IDC_CHECK_GPIO2, m_chGPIO2);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
}

BEGIN_MESSAGE_MAP(CMFCDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_DROPDOWN(IDC_COMBO_SERIALNAME, &CMFCDemoDlg::OnCbnDropdownComboSerialname)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMFCDemoDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SEND1, &CMFCDemoDlg::OnBnClickedButtonSend1)
	ON_BN_CLICKED(IDC_BUTTON_POS, &CMFCDemoDlg::OnBnClickedButtonPos)
	ON_BN_CLICKED(IDC_BUTTON_VEL, &CMFCDemoDlg::OnBnClickedButtonVel)
	ON_BN_CLICKED(IDC_BUTTON_MOV, &CMFCDemoDlg::OnBnClickedButtonMov)
	ON_BN_CLICKED(IDC_BUTTON_STT, &CMFCDemoDlg::OnBnClickedButtonStt)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_TIMER, &CMFCDemoDlg::OnBnClickedRadioTimer)
	ON_BN_CLICKED(IDC_RADIO_EVENT, &CMFCDemoDlg::OnBnClickedRadioEvent)
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
	m_ccbSerialName.ResetContent();

	TCHAR lpTargetPath[5000];
	DWORD test;
	bool gotPort = 0;

	for (int i = 0; i < 255; i++)
	{
		CString str;
		str.Format(_T("%d"), i);
		CString ComName = CString("COM") + CString(str);

		test = QueryDosDevice(ComName, (LPSTR)lpTargetPath, 2000);

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
	CString csInPacket;
	csInPacket.Format((CString)"Receive: ");
	m_listboxRead.InsertString(0, csInPacket);
	csInPacket.Empty();

	for (UINT i = 0; i < (UINT)inLength; i++) {
		csInPacket.AppendFormat((CString)"%02X ", inPacket[i]);
	}
	m_listboxRead.InsertString(0, csInPacket);

	ProcessData(inPacket, inLength);


	CString str;
	str.Format((LPCTSTR)"%d bytes read", inLength);

	m_staticInfo.SetWindowText(str);
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


void CMFCDemoDlg::OnBnClickedButtonPos()
{
	// TODO: Add your control notification handler code here
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	if (!GetPortActivateValue()) return;
	memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(ProtocolFrame + index, bGPOS, sizeof(bGPOS));
	index += sizeof(bGPOS);
	memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	/*std::copy(FRAME.begin(), FRAME.end(), res);*/
	Write((char*)ProtocolFrame, index);

	CString cmd;
	cmd.Format((LPCTSTR)"%s", "POS CMD: ");
	m_listboxRead.InsertString(0, cmd);
	cmd.Empty();
	for (UINT i = 0; i < index; i++) {
		cmd.AppendFormat((LPCTSTR)"%02X ", ProtocolFrame[i]);
	}
	m_listboxRead.InsertString(0, cmd);
}


void CMFCDemoDlg::OnBnClickedButtonVel()
{
	// TODO: Add your control notification handler code here
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	if (!GetPortActivateValue()) return;
	memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(ProtocolFrame + index, bGVEL, sizeof(bGVEL));
	index += sizeof(bGVEL);
	memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	/*std::copy(FRAME.begin(), FRAME.end(), res);*/
	Write((char*)ProtocolFrame, index);

	CString cmd;
	cmd.Format((LPCTSTR)"%s", "VEL CMD: ");
	m_listboxRead.InsertString(0, cmd);
	cmd.Empty();
	for (UINT i = 0; i < index; i++) {
		cmd.AppendFormat((LPCTSTR)"%02X ", ProtocolFrame[i]);
	}
	m_listboxRead.InsertString(0, cmd);
}


void CMFCDemoDlg::OnBnClickedButtonMov()
{
	// TODO: Add your control notification handler code here
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	if (!GetPortActivateValue()) return;
	memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(ProtocolFrame + index, bMOVL, sizeof(bMOVL));
	index += sizeof(bMOVL);
	memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	/*std::copy(FRAME.begin(), FRAME.end(), res);*/
	Write((char*)ProtocolFrame, index);

	CString cmd;
	cmd.Format((LPCTSTR)"%s", "MOV CMD: ");
	m_listboxRead.InsertString(0, cmd);
	cmd.Empty();
	for (UINT i = 0; i < index; i++) {
		cmd.AppendFormat((LPCTSTR)"%02X ", ProtocolFrame[i]);
	}
	m_listboxRead.InsertString(0, cmd);
}


void CMFCDemoDlg::OnBnClickedButtonStt()
{
	// TODO: Add your control notification handler code here
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	if (!GetPortActivateValue()) return;
	memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
	index += sizeof(bSTX);
	memcpy(ProtocolFrame + index, bSTT, sizeof(bSTT));
	index += sizeof(bSTT);
	memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
	index += sizeof(bOPT);
	memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
	index += sizeof(bDATA);
	memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
	index += sizeof(bSYNC);
	memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
	index += sizeof(bETX);
	/*std::copy(FRAME.begin(), FRAME.end(), res);*/
	Write((char*)ProtocolFrame, index);

	CString cmd;
	cmd.Format((LPCTSTR)"%s", "STT CMD: ");
	m_listboxRead.InsertString(0, cmd);
	cmd.Empty();
	for (UINT i = 0; i < index; i++) {
		cmd.AppendFormat((LPCTSTR)"%02X ", ProtocolFrame[i]);
	}
	m_listboxRead.InsertString(0, cmd);
}

VOID CMFCDemoDlg::ProcessData(char* data, int inLength)
{
	CString cmd;
	for (UINT i = 0; i < (UINT)inLength; i++)
	{
		bProtocolDataBuffer[i] = (BYTE)data[i];
	}
	for (UINT i = 1; i <= 4; i++)
	{
		cmd.AppendChar((char)bProtocolDataBuffer[i]);
	}
	for (UINT i = 8; i <= 15; i++)
	{
		bProtocolData[i - 8] = bProtocolDataBuffer[i];
	}

	if (cmd.Compare((LPCSTR)"GPOS") == 0)
	{

	}

	else if (cmd.Compare((LPCSTR)"MOVL") == 0)
	{

	}
	else if (cmd.Compare((LPCSTR)"GVEL") == 0)
	{

	}
	else if (cmd.Compare((LPCSTR)"GSTT") == 0)
	{
		if (bProtocolDataBuffer[12] == 0x01) {
			m_chLed1.SetCheck(1);
		}
		else {
			m_chLed1.SetCheck(0);
		}
		if (bProtocolDataBuffer[13] == 0x01) {
			m_chGPIO1.SetCheck(1);
		}
		else {
			m_chGPIO1.SetCheck(0);
		}
		if (bProtocolDataBuffer[14] == 0x01) {
			m_chLed2.SetCheck(1);
		}
		else {
			m_chLed2.SetCheck(0);
		}
		if (bProtocolDataBuffer[15] == 0x01) {
			m_chGPIO2.SetCheck(1);
		}
		else {
			m_chGPIO2.SetCheck(0);
		}

	}
}

void CMFCDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	BYTE ProtocolFrame[50] = {};
	UINT index = 0;
	CString cmd;

	if (!GetPortActivateValue()) return;

	switch (nIDEvent) {
	case EVENT_SEND_BY_TIMER:
		memcpy(ProtocolFrame + index, bSTX, sizeof(bSTX));
		index += sizeof(bSTX);
		memcpy(ProtocolFrame + index, bGPOS, sizeof(bGPOS));
		index += sizeof(bGPOS);
		memcpy(ProtocolFrame + index, bOPT, sizeof(bOPT));
		index += sizeof(bOPT);
		memcpy(ProtocolFrame + index, bDATA, sizeof(bDATA));
		index += sizeof(bDATA);
		memcpy(ProtocolFrame + index, bSYNC, sizeof(bSYNC));
		index += sizeof(bSYNC);
		memcpy(ProtocolFrame + index, bETX, sizeof(bETX));
		index += sizeof(bETX);

		Write((char*)ProtocolFrame, index);
		cmd.Format((LPCTSTR)"%s", "TIMER STT CMD: ");
		m_listboxRead.InsertString(0, cmd);
		cmd.Empty();
		for (UINT i = 0; i < index; i++)
		{
			cmd.AppendFormat((LPCTSTR)"%02X", ProtocolFrame[i]);
		}
		m_listboxRead.InsertString(0, cmd);
		break;
	default:
		break;
	}
		__super::OnTimer(nIDEvent);
}


void CMFCDemoDlg::OnBnClickedRadioTimer()
{
	// TODO: Add your control notification handler code here
	SetTimer(EVENT_SEND_BY_TIMER, 1000, nullptr);
}


void CMFCDemoDlg::OnBnClickedRadioEvent()
{
	// TODO: Add your control notification handler code here
	KillTimer(EVENT_SEND_BY_TIMER);

	m_listboxRead.InsertString(0, reinterpret_cast<LPCTSTR>("Stop timer"));
}
