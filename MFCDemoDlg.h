
// MFCDemoDlg.h : header file
//

#pragma once
#include "SerialCtrl.h"
#include "afxwin.h"
// CMFCDemoDlg dialog

class CMFCDemoDlg : public CDialogEx, public CSerialIO
{
// Construction
public:
	CMFCDemoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ccbSerialName;
	afx_msg void OnCbnDropdownComboSerialname();
	CComboBox m_ccbBaudrate;
	CButton m_btnOpen;
	afx_msg void OnBnClickedButtonOpen();

	//Add by User
	BOOL bPortOpened = 0;

	virtual void OnEventOpen(BOOL bSuccess);
	virtual void OnEventClose(BOOL bSuccess);
	virtual void OnEventRead(char* inPacket, int inLength);
	virtual void OnEventWrite(int nWritten);


	CEdit m_ceSendCmd1;
	CButton m_btnSendCmd1;
	afx_msg void OnBnClickedButtonSend1();
	CButton mBtnPosition;
	CButton mBtnVelocity;
	CButton mBtnMove;
	CButton mBtnStatus;
	afx_msg void OnBnClickedButtonPos();
	CListBox m_listboxRead;
	afx_msg void OnBnClickedButtonVel();
	afx_msg void OnBnClickedButtonMov();
	afx_msg void OnBnClickedButtonStt();

	VOID ProcessData(char* data, int inLength);
	CButton m_staticInfo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedRadioTimer();
	afx_msg void OnBnClickedRadioEvent();
};
