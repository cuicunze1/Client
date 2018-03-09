#pragma once

#include "afxwin.h"
#include "afxcmn.h"

// ChatDlg �Ի���

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)

public:
	CChatDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChatDlg();

// �Ի�������
	enum { IDD = IDD_CHATDLG };
public:
	void UpdateText(CString& strText);  //���������¼

// ʵ��
protected:
	BOOL WChar2MByte(LPCTSTR lpSrc, LPSTR lpDest, int nlen);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnSend();
	void ConnnectToUser();
	void OnGetxindemsg(char *);
	CSessionSocket *ReturntheSocket();
	void SetTITLE(CString newCaption);
	CString m_strSendend;
	CString m_strNamenam;
	CString to_user;
	CString to_IP;
	int to_Port;
	BOOL is_online;
	CString m_caption;//�Ի����������

public:
	CSessionSocket *pthechatsoc;
	CEdit m_text;
	CString m_filePath;
	bool m_posting;
public:
	afx_msg void OnEnChangeEtText();
	afx_msg void OnBnClickedButton3();
};
