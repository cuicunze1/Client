
// ClientDlg.h : ͷ�ļ�
//

#pragma once

#include "Header.h"

// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif
	void RefreshtheUserInfo(CString strInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	BOOL WChar2MByte(LPCTSTR lpSrc, LPSTR lpDest, int nlen);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CString m_caption;//�Ի����������
	char *toUser;
	afx_msg void On_choose();
	void GetNewMsg(HEADER head, char * buf);
	void OnGetIp(char *buf);
	void OnReceivetheOffMsg(HEADER head, char *buf);
	CPtrList *m_chatList;
	CListBox m_listBox;
	afx_msg void OnLbnSelchangeLbOnline();
};
