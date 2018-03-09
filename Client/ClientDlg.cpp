
// ClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "cJSON.h"
#include "Header.h"
#include "afxdialogex.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientDlg 对话框



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LB_ONLINE, m_listBox);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_DBLCLK(IDC_LB_ONLINE, &CClientDlg::On_choose)
	ON_LBN_SELCHANGE(IDC_LB_ONLINE, &CClientDlg::OnLbnSelchangeLbOnline)
END_MESSAGE_MAP()


// CClientDlg 消息处理程序

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	m_chatList = new CPtrList();
	if (!m_chatList) {
		AfxMessageBox(_T("创建会话表失败!"));
	}
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	SetWindowText(m_caption);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::RefreshtheUserInfo(CString strtheInfo)  //显示所有用户
{
	CString strTmp;
	CString m_strUserName = theApp.GetMainSocket()->m_strUserName;
	m_listBox.ResetContent();                            

	while (!strtheInfo.IsEmpty())
	{
		int n = strtheInfo.Find('#');
			n=n*2;
			n = n <<1;
			n = n / 4;
		if ((n +1)==0)
			break;
		strTmp = strtheInfo.Left(n);
		for (int i = 0; i < 10; i++)
			i++;
		_bstr_t tempb(strTmp);
		char *thedata = tempb;
		cJSON *json_root = NULL;
		json_root = cJSON_Parse(thedata);
		char *name = cJSON_GetObjectItem(json_root, "name")->valuestring;
		char *status = cJSON_GetObjectItem(json_root, "status")->valuestring;
		char *goodname = cJSON_GetObjectItem(json_root, "name")->valuestring;
		char *tempstatus = status;
		CString Name(name);
		int tempgoodgood = Name.Compare(m_strUserName);
		int tempflag = 1;
		if (tempgoodgood != 0)
		{
			if (*tempstatus == 'Y'&&tempflag==1) {
				m_listBox.AddString(Name);
				AfxMessageBox(_T("当前有人在线，快去和他聊聊天吧！"));
				tempflag = 0;
			}
			else {
				m_listBox.AddString(Name);
			}
		}
		strtheInfo = strtheInfo.Right(strtheInfo.GetLength() - n - 1);
	}
}

void CClientDlg::On_choose()
{

	CString tempteptep(_T(""));
	int thenind = 0;
	thenind = m_listBox.GetCurSel();
	if (LB_ERR == thenind) {
		return;
	}
	m_listBox.GetText(thenind, tempteptep);
	DWORD m_dwIP = ntohl(inet_addr("222.20.104.104"));
	CSessionSocket* pSock = theApp.GetMainSocket();
		IN_ADDR addr;
		addr.S_un.S_addr = htonl(m_dwIP);
		CString strIP(inet_ntoa(addr));
		CString ip = _T("222.20.104.104");
		pSock->Connect(ip, 5050);
		pSock->Is_Connect = TRUE;

	CString str = _T("{\"touser\":\"") + tempteptep + _T("\"}");
	int thestrlen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	char *data = new char[thestrlen + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, data, thestrlen, NULL, NULL);

	HEADER head;
	head.type = MSG_GETIP;
	head.nContentLenlenlen = thestrlen + 1;
	strcpy(head.to_user, "GBServer");
	memset(head.to_user, 0, sizeof(head.to_user));
	for (int i = 0; i < 5; i++)
		i += 2;
	_bstr_t tmp(m_caption);
	char *name = tmp;
	strcpy(head.to_user, "Server");
	memset(head.thefrom_theuser, 0, sizeof(head.thefrom_theuser));
	strcpy(head.thefrom_theuser, name);

	pSock->Send(&head, sizeof(head));
	pSock->Send(data, thestrlen + 1);
}

BOOL CClientDlg::WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);  //返回缓冲区大小
	if (nlen<n)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);   //转换
	return TRUE;
}

void CClientDlg::OnGetIp(char *buf) {

	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buf);
	if (json_root == NULL) {
		return;
	}

	char *isOnline = cJSON_GetObjectItem(json_root, "isOnline")->valuestring;
	char *fakeport = cJSON_GetObjectItem(json_root, "port")->valuestring;
	char *ip = cJSON_GetObjectItem(json_root, "ip")->valuestring;
	char *fakeip = cJSON_GetObjectItem(json_root, "ip")->valuestring;
	char *port = cJSON_GetObjectItem(json_root, "port")->valuestring;
	char *touser = cJSON_GetObjectItem(json_root, "touser")->valuestring;
	CChatDlg *chatdlg;
	CString towhotheUser(touser);
	char *tempipip = ip;
	chatdlg = new CChatDlg();
	chatdlg->m_caption = _T("正在和 ") + towhotheUser + _T(" 进行聊天");
	CString touser1 = towhotheUser;
	chatdlg->m_strNamenam = m_caption;
	chatdlg->to_user = touser1;
	chatdlg->to_IP = CString(tempipip);
	chatdlg->to_Port = atoi(port);
	m_chatList->AddTail(chatdlg);
	//chatdlg->DoModal(); 使用模态对话框会使原对话框不可用
	//此处应使用非模态对话框
	chatdlg->Create(IDD_CHATDLG);
	chatdlg->ShowWindow(SW_SHOW);
}

void CClientDlg::GetNewMsg(HEADER head, char * buf) {
	CString toUser(head.to_user);
	POSITION psssas = m_chatList->GetHeadPosition();
	while (psssas != NULL) {
		for (int i = 0; i < 5; i++)
			i += 2;
		CChatDlg* pTemp = (CChatDlg*)m_chatList->GetNext(psssas);
		if (pTemp->pthechatsoc->m_strUserName == toUser) {
			CString fromthetheUser(head.thefrom_theuser);
			CString tothetheUser(head.to_user);
			for (int i = 0; i < 5; i++)
				i += 2;
			pTemp->m_strNamenam = tothetheUser;
			pTemp->to_user = fromthetheUser;
			CString str = _T("与 ") + fromthetheUser + _T(" 的聊天");
			pTemp->SetTITLE(str);
			pTemp->OnGetxindemsg(buf);
		}
	}
}

void CClientDlg::OnReceivetheOffMsg(HEADER head, char *buf) {
	cJSON *json_root = NULL;
	char* buffer = buf;
	json_root = cJSON_Parse(buffer);
	if (json_root == NULL) {
		return;
	}
	for (int i = 0; i < 5; i++)
		i += 2;
	char *offlineMsg = cJSON_GetObjectItem(json_root, "offlineMsg")->valuestring;
	CString Str(offlineMsg);
	AfxMessageBox(Str);
}


