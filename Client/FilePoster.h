#pragma once
#include "afxcmn.h"

#define	WM_REDAY_TO_RECEIVE WM_USER+1
// CFilePoster 对话框

class CFilePoster : public CDialog
{
	DECLARE_DYNAMIC(CFilePoster)

public:
	CFilePoster(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFilePoster();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEPOSTERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFindFile();
	afx_msg void OnBnClickedFindSend();
	BOOL WChar2MByte(LPCTSTR lpSrc, LPSTR lpDest, int nlen);
	CString m_send;
	CString m_recv;
	CProgressCtrl m_process_s;
	CProgressCtrl m_process_r;
	int m_nFileSize_s;
	int m_nRecv;
	int m_nSend;
	static int nFileSize;
	bool m_posting;
	static DWORD WINAPI RecvProc(LPVOID lpParameter);
	CString m_filePath;
	CString to_IP;
	DWORD dwIP;
private:
	FILE* m_file;
	SOCKET m_socket_udp;
public:
	virtual BOOL OnInitDialog();
protected:
	afx_msg LRESULT OnRedayToReceive(WPARAM wParam, LPARAM lParam);
};
