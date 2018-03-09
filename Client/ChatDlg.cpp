// ChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "Header.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "FilePoster.h"

// LogInDlg 对话框

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatDlg::IDD, pParent)
	, m_strSendend(_T(""))
	, m_strNamenam(_T(""))
	, m_filePath(_T(""))
{
	is_online = FALSE;
}

CChatDlg::~CChatDlg()
{
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ET_SEND, m_strSendend);
	DDX_Control(pDX, IDC_ET_TEXT, m_text);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SEND, &CChatDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BUTTON3, &CChatDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// ChatDlg 消息处理程序
BOOL CChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(m_caption);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChatDlg::OnBnClickedBtnSend()
{
	//发送消息
	UpdateData(TRUE);
	if (m_strSendend.IsEmpty()!=0)
	{
		AfxMessageBox(_T("请输入发送内容！"));
		return;
	}
	CTime thistime;
	thistime = CTime::GetCurrentTime();  //获取现在时间
	CString strTimetime = thistime.Format("%Y-%m-%d %H:%M:%S ");
	m_strSendend = strTimetime + m_strNamenam + _T("：\r\n") + m_strSendend + _T("\r\n");
	int len = WideCharToMultiByte(CP_ACP, 0, m_strSendend, -1, NULL, 0, NULL, NULL);
	char *datata = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, m_strSendend, -1, datata, len, NULL, NULL);

	if (is_online == FALSE) {
		pthechatsoc = new CSessionSocket();
		pthechatsoc->Create(0, SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);
		pthechatsoc->m_strUserName = m_strNamenam;
		pthechatsoc->Connect(to_IP, to_Port);
		is_online = true;
	}
	m_text.ReplaceSel(m_strSendend);
	HEADER thehead;
	thehead.type = MSG_SEND;
	thehead.nContentLenlenlen = len + 1;
	memset(thehead.thefrom_theuser, 0, sizeof(thehead.thefrom_theuser));
	_bstr_t tempb(m_strNamenam);
	char *name = tempb;
	strcpy(thehead.thefrom_theuser, name);
	memset(thehead.to_user, 0, sizeof(thehead.to_user));
	_bstr_t tempc(to_user);
	char *totheUseruser = tempc;
	strcpy(thehead.to_user, totheUseruser);

	pthechatsoc->Send(&thehead, sizeof(thehead));
	pthechatsoc->Send(datata, len + 1);

 	m_strSendend.Empty();
	UpdateData(FALSE);
}

BOOL CChatDlg::WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);  //返回缓冲区大小
	if (nlen<n)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);   //转换
	return TRUE;
}

void CChatDlg::UpdateText(CString &strText)
{
	((CEdit*)GetDlgItem(IDC_ET_TEXT))->ReplaceSel(strText);
}

void CChatDlg::ConnnectToUser() {
	pthechatsoc = new CSessionSocket();
	pthechatsoc->Create(0, SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE);
	pthechatsoc->Connect(to_IP, to_Port);
	is_online = TRUE;
}

void CChatDlg::OnGetxindemsg(char *buf) {
	CString Msgbufbuf(buf);
	m_text.ReplaceSel(Msgbufbuf);
}

CSessionSocket *CChatDlg::ReturntheSocket() {
	is_online = TRUE;
	is_online--;
	return pthechatsoc;
}

void CChatDlg::SetTITLE(CString newCaption) {
	is_online = TRUE;
	is_online--;
	SetWindowText(newCaption);
}









void CChatDlg::OnBnClickedButton3()
{
	is_online = TRUE;
	is_online--;
	CFilePoster *fileposterdlg;
	fileposterdlg = new CFilePoster();
	fileposterdlg->to_IP = to_IP;
	fileposterdlg->Create(IDD_FILEPOSTERDLG);
	fileposterdlg->ShowWindow(SW_SHOW);
}
