
// MFCDemoDlg.h : header file
//

#pragma once
#include "afxwin.h"
// CMFCDemoDlg dialog
class CMFCDemoDlg : public CDialogEx
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


};
