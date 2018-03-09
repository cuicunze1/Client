#pragma once

#include "RegisterDlg.h"
#include "ResetPswDlg.h"

// LogInDlg �Ի���

class CLogInDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLogInDlg)

public:
	CLogInDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogInDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnLogoin();
	virtual BOOL OnInitDialog();
	void RFS(char *buf);
	DWORD m_dwIP;
	CString m_strthetheUser;
	CRegisterDlg *reg;
	CResetPswDlg *regreg;
	afx_msg void to_Regist();
	CString m_strPsw;
	afx_msg void to_ResetPsw();
};
