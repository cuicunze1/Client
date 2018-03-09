// FilePoster.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "FilePoster.h"
#include "afxdialogex.h"


// CFilePoster 对话框

IMPLEMENT_DYNAMIC(CFilePoster, CDialog)

CFilePoster::CFilePoster(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FILEPOSTERDLG, pParent)
	, m_send(_T("发送进度："))
	, m_recv(_T("接受进度："))
	, m_posting(false)
	, m_filePath(_T(""))
{

}

CFilePoster::~CFilePoster()
{
}

void CFilePoster::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SEND, m_send);
	DDX_Text(pDX, IDC_RECV, m_recv);
	DDX_Control(pDX, IDC_PROGRESS_SEND, m_process_s);
	DDX_Control(pDX, IDC_PROGRESS_RECV, m_process_r);
	DDX_Text(pDX, IDC_FILE_PATH, m_filePath);
}


BEGIN_MESSAGE_MAP(CFilePoster, CDialog)
	ON_BN_CLICKED(IDC_FIND_FILE, &CFilePoster::OnBnClickedFindFile)
	ON_BN_CLICKED(IDC_FIND_SEND, &CFilePoster::OnBnClickedFindSend)
	ON_MESSAGE(WM_REDAY_TO_RECEIVE, &CFilePoster::OnRedayToReceive)
END_MESSAGE_MAP()


// CFilePoster 消息处理程序
struct RECVPARAM
{
	HWND hWnd;
	SOCKET sock;
};

int CFilePoster::nFileSize = 0;

void CFilePoster::OnBnClickedFindFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog openFile(true);
	if (IDOK == openFile.DoModal())
	{
		m_filePath = openFile.GetPathName();
		UpdateData(false);
	}
}


void CFilePoster::OnBnClickedFindSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_posting)
	{
		AfxMessageBox(_T("数据发送中，请稍后重试！"));
		return;
	}

	UpdateData();

	if (m_filePath == "")
	{
		AfxMessageBox(_T("请选择要发送的文件路径！"));
		return;
	}

	//if (m_IPAddr.IsBlank())
	//{
	//	AfxMessageBox(_T("请添入接收者的IP地址。"));
	//	return;
	//}

	WIN32_FIND_DATA FindFileData;

	if (INVALID_HANDLE_VALUE == FindFirstFile(m_filePath, &FindFileData))
	{
		AfxMessageBox(_T("文件路径错误或文件不存在!\n请重新指定文件路径！"));
		return;
	}

	//读取对方IP


	CString str1;
	CString str2;
	CString str3;
	CString str4;
	if (!to_IP.IsEmpty())
	{
		int n = to_IP.Find('.');
		str1 = to_IP.Left(n);
		to_IP = to_IP.Right(to_IP.GetLength() - n - 1);
	}
	if (!to_IP.IsEmpty())
	{
		int n = to_IP.Find('.');
		str2 = to_IP.Left(n);
		to_IP = to_IP.Right(to_IP.GetLength() - n - 1);
	}
	if (!to_IP.IsEmpty())
	{
		int n = to_IP.Find('.');
		str3 = to_IP.Left(n);
		str4 = to_IP.Right(to_IP.GetLength() - n - 1);
	}
	long IP1 = _ttol(str1.GetBuffer());
	long IP2 = _ttol(str2.GetBuffer());
	long IP3 = _ttol(str3.GetBuffer());
	long IP4 = _ttol(str4.GetBuffer());
	dwIP = IP1 * 256 * 256 * 256 + IP2 * 256 * 256 + IP3 * 256 + IP4;




	SOCKADDR_IN addrTo;
	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons(10086);
	addrTo.sin_addr.S_un.S_addr = htonl(dwIP);

	char sendBuf[0x112];
	int i;

	sendBuf[0] = 'H';

	for (i = 1; i <= 0x100 && FindFileData.cFileName[i - 1] != '\0'; i++)
		sendBuf[i] = FindFileData.cFileName[i - 1];
	sendBuf[i] = '\0';

	_itoa(FindFileData.nFileSizeLow, &sendBuf[0x101], 10);
	sendBuf[0x111] = '\0';

	sendto(m_socket_udp, sendBuf, 0x112, 0, (SOCKADDR*)&addrTo, sizeof(SOCKADDR));

	char *p_filePath = new char[m_filePath.GetLength() * 2 + 1];
	memset(p_filePath, 0, m_filePath.GetLength() * 2 + 1);
	if (!(this->WChar2MByte(m_filePath.GetBuffer(0), p_filePath, m_filePath.GetLength() * 2 + 1)))
	{
		AfxMessageBox(_T("转换失败！"));
		delete p_filePath;
		return;
	}
	if (!(m_file = fopen(p_filePath, "rb")))
	{
		//MessageBox("读取文件失败！");
		AfxMessageBox(_T("读取文件失败！"));
	}
	delete p_filePath;
	m_nSend = 0;
	m_nFileSize_s = FindFileData.nFileSizeLow;
	m_process_s.SetRange(0, m_nFileSize_s / 0x100 + 1);
	m_posting = true;
}

BOOL CFilePoster::WChar2MByte(LPCTSTR lpSrc, LPSTR lpDest, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);  //返回所需缓冲区大小
	if (nlen<n)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);   //转换
	return TRUE;
}

BOOL CFilePoster::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_process_s.SetStep(1);
	m_process_r.SetStep(1);
	/***********************************************************************/
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("加载套接字库失败！"));
		return FALSE;
	}
	//创建UDP套接字
	m_socket_udp = socket(AF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == m_socket_udp)
	{
		//MessageBox("套接字创建失败！");
		AfxMessageBox(_T("UDP套接字创建失败！"));
		return FALSE;
	}
	SOCKADDR_IN addrSock;
	addrSock.sin_family = AF_INET;
	addrSock.sin_port = htons(10086);
	addrSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int retval;
	retval = bind(m_socket_udp, (SOCKADDR*)&addrSock, sizeof(SOCKADDR));
	if (SOCKET_ERROR == retval)
	{
		//addrSock.sin_port = htons(12306);
		//retval = bind(m_socket_udp, (SOCKADDR*)&addrSock, sizeof(SOCKADDR));
		//if (SOCKET_ERROR == retval)
		//{
		closesocket(m_socket_udp);
		//MessageBox("绑定失败!");
		AfxMessageBox(_T("绑定失败！"));
		return FALSE;
		//}
	}
	//产生一个用于接收数据的线程


	struct RECVPARAM *pRecvParam = new RECVPARAM;
	pRecvParam->sock = m_socket_udp;
	pRecvParam->hWnd = m_hWnd;
	HANDLE hThread = CreateThread(NULL, 0, RecvProc, (LPVOID)pRecvParam, 0, NULL);
	CloseHandle(hThread);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


afx_msg LRESULT CFilePoster::OnRedayToReceive(WPARAM wParam, LPARAM lParam)
{
	char sendBuf[0x112];
	DWORD dwIP_input;
	dwIP_input = _wtol(to_IP);
	SOCKADDR_IN addrTo;
	addrTo.sin_family = AF_INET;
	addrTo.sin_port = htons(10086);
	addrTo.sin_addr.S_un.S_addr = htonl(dwIP);

	int nRead;

	switch (*(char*)wParam)
	{
	case 'D':
	{
		AfxMessageBox(_T("对方拒绝接受你发送的文件！"));
		fclose(m_file);
		m_posting = false;
		break;
	}
	case 'R':
	{
		nRead = fread(&sendBuf[0x12], 1, 0x100, m_file);

		if (nRead < 0x100)
		{
			sendBuf[0] = 'E';
			_itoa(nRead, &sendBuf[1], 10);
			sendto(m_socket_udp, sendBuf, nRead + 0x12, 0, (SOCKADDR*)&addrTo, sizeof(SOCKADDR));
			fclose(m_file);
			m_process_s.SetPos(m_nFileSize_s / 0x100 + 1);
			m_posting = false;
			m_send = "发送进度：(100%)";
			UpdateData(false);
			AfxMessageBox(_T("发送完毕！"));
			m_process_s.SetPos(0);
			m_send = "发送进度：";
			UpdateData(false);
		}
		else
		{
			sendBuf[0] = 'F';
			sendto(m_socket_udp, sendBuf, 0x112, 0, (SOCKADDR*)&addrTo, sizeof(SOCKADDR));
			m_process_s.StepIt();
			m_nSend++;
			m_send.Format(_T("发送进度：(%.1f%%)"), (float)m_nSend / (m_nFileSize_s / 0x100 + 1) * 100);
			UpdateData(false);
		}
		break;
	}
	case 'H':
	{
		m_process_r.SetRange(0, nFileSize / 0x100 + 1);
		m_nRecv = 0;
	}
	case 'F':
	{
		sendto(m_socket_udp, "R", 2, 0, (SOCKADDR*)lParam, sizeof(SOCKADDR));
		m_process_r.StepIt();
		m_nRecv++;
		m_recv.Format(_T("接收进度：(%.1f%%)"), (float)m_nRecv / (nFileSize / 0x100 + 1) * 100);
		UpdateData(false);
		break;
	}
	case 'E':
	{
		m_process_r.SetPos(nFileSize / 0x100 + 1);
		m_recv = _T("接收进度：(100%)");
		UpdateData(false);
		AfxMessageBox(_T("接收完毕！"));
		m_recv = _T("接收进度：");
		m_process_r.SetPos(0);
		UpdateData(false);
		break;
	}
	case 'C':
	{
		sendto(m_socket_udp, "D", 2, 0, (SOCKADDR*)lParam, sizeof(SOCKADDR));
		break;
	}

	}
	return 0;
}


DWORD WINAPI CFilePoster::RecvProc(LPVOID lpParameter)
{
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;
	HWND hWnd = ((RECVPARAM*)lpParameter)->hWnd;
	delete lpParameter;

	SOCKADDR_IN addrFrom;
	int len = sizeof(SOCKADDR);
	char recvBuf[0x112];		//256+17字节的缓冲
	char fileName[0x100];
	int retval, i;
	FILE* file = NULL;
	while (TRUE)
	{
		retval = recvfrom(sock, recvBuf, 0x112, 0, (SOCKADDR*)&addrFrom, &len);  //源地址存入addrFrom

		if (SOCKET_ERROR == retval)  //没有待读的信息
			break;

		if (recvBuf[0] == 'R')
		{
			char wParam = 'R';
			::PostMessage(hWnd,  WM_REDAY_TO_RECEIVE, (WPARAM)&wParam, 0);
		}
		else if (recvBuf[0] == 'D')
		{
			char wParam = 'D';
			::PostMessage(hWnd, WM_REDAY_TO_RECEIVE, (WPARAM)&wParam, 0);
		}
		else if (recvBuf[0] == 'H')
		{

			for (i = 1; i <= 0x100 && recvBuf[i] != '\0'; i++)	//前256字节用于存储文件名
				fileName[i - 1] = recvBuf[i];
			fileName[i - 1] = '\0';
			CString recvMsg;
			nFileSize = atoi(&recvBuf[0x101]);
			char *savefilename = NULL;
			recvMsg.Format(_T("收到文件：%s\n文件大小：%i字节\n是否接收？"),  CStringW(fileName), nFileSize);
			if (IDOK == AfxMessageBox(recvMsg, MB_OKCANCEL))
			{
				CFileDialog saveDlg(false, NULL, CStringW(fileName));

				if (IDOK == saveDlg.DoModal())
				{
					CFilePoster *tmpDlg = new CFilePoster();
					CString strSavePathName = saveDlg.GetPathName();
					char *filename = new char[strSavePathName.GetLength() * 2 + 1];
					memset(filename, 0, strSavePathName.GetLength() * 2 + 1);
					//if (!(tmpDlg->WChar2MByte(strSavePathName, filename, strSavePathName.GetLength())))
					//{
					//	AfxMessageBox(_T("字符转换失败！"));
					//}
					WideCharToMultiByte(CP_OEMCP, 0, strSavePathName, -1, filename, strSavePathName.GetLength() * 2 + 1, 0, FALSE);
					delete tmpDlg;
					if (!(file = fopen(filename, "wb")))
					{
						AfxMessageBox(_T("创建本地文件失败！"));
						continue;
					}
					delete filename;
					char wParam = 'H';
					::PostMessage(hWnd, WM_REDAY_TO_RECEIVE, (WPARAM)&wParam, (LPARAM)&addrFrom);
				}
				else
				{
					char wParam = 'C';
					::PostMessage(hWnd, WM_REDAY_TO_RECEIVE, (WPARAM)&wParam, (LPARAM)&addrFrom);
				}
			}
			else
			{
				char wParam = 'C';
				::PostMessage(hWnd, WM_REDAY_TO_RECEIVE, (WPARAM)&wParam, (LPARAM)&addrFrom);
			}

		}
		else if (recvBuf[0] == 'F')
		{
			fwrite(&recvBuf[0x12], 1, 0x100, file);
			char wParam = 'F';
			::PostMessage(hWnd, WM_REDAY_TO_RECEIVE, (WPARAM)&wParam, (LPARAM)&addrFrom);
		}
		else if (recvBuf[0] == 'E')
		{
			int bufSize = atoi(&recvBuf[1]);
			fwrite(&recvBuf[0x12], 1, bufSize, file);
			fclose(file);
			char wParam = 'E';
			::PostMessage(hWnd, WM_REDAY_TO_RECEIVE, (WPARAM)&wParam, (LPARAM)&addrFrom);
		}
		else
			AfxMessageBox(_T("传送数据过程中出现错误！"));
	}
	return (DWORD)NULL;
}