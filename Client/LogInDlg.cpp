// LogInDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "MD5.h"
#include "LogInDlg.h"
#include "Header.h"
#include "cJSON.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"
#include "SessionSocket.h"



// LogInDlg 对话框

IMPLEMENT_DYNAMIC(CLogInDlg, CDialogEx)

CLogInDlg::CLogInDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
	, m_strthetheUser(_T(""))
	, m_strPsw(_T(""))
{
	//初始化IP地址
	//DWORD 就是unSigned long
	m_dwIP = ntohl(inet_addr("192.168.43.245"));
}

CLogInDlg::~CLogInDlg()
{
}

void CLogInDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ET_NAME, m_strthetheUser);
	DDX_Text(pDX, IDC_ET_PSW, m_strPsw);
}

BEGIN_MESSAGE_MAP(CLogInDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOGOIN, &CLogInDlg::OnBnClickedBtnLogoin)
	ON_BN_CLICKED(IDC_BTN_REGIST, &CLogInDlg::to_Regist)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogInDlg::to_ResetPsw)
END_MESSAGE_MAP()


void CLogInDlg::OnBnClickedBtnLogoin()
{
	//登录	
	UpdateData();
	if (m_strthetheUser.IsEmpty())
	{
		AfxMessageBox(_T("请输入用户名！"));
		return;
	}

	CSessionSocket* pSocket_now = theApp.GetMainSocket();
	if (!pSocket_now->Is_Connect ) {
		CString ip = _T("192.168.43.245");
		pSocket_now->Connect(ip, 5050);
		int a = 1;
		pSocket_now->Is_Connect = a * 10 - 9;
	}

	_bstr_t tempb(m_strPsw);
	char* tempc = tempb;
	char* tempd = tempc;
	string Psw = tempd;
	string strtempa = MD5(Psw).toStr();
	CString MD5Password(strtempa.data());
	for (int i = 0; i < 5; i++)
		i += 2;
	cJSON *json_root = NULL;
	char tempport[6];
	_itoa(theApp.ListenPort, tempport, 10);
	for (int i = 0; i < 5; i++)
		i += 2;
	CString Port(tempport);
	CString str = _T("{\"username\":\"") + m_strthetheUser + _T("\", \"password\":\"") + MD5Password + _T("\"}");
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	for (int i = 0; i < 5; i++)
		i += 2;
	char *data = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, str, -1, data, len, NULL, NULL);

	_bstr_t Name(m_strthetheUser);
	char *name = Name;
	for (int i = 0; i < 5; i++)
		i += 2;
	pSocket_now->m_strUserName = m_strthetheUser;  //将用户名字传递过去
	pSocket_now->LogoIn(data, strlen(data), name);
}

void CLogInDlg::RFS(char *buf) {
	char * buffer = buf;
	cJSON *json_root = NULL;
	json_root = cJSON_Parse(buffer);
	int thecommoncmd = cJSON_GetObjectItem(json_root, "cmd")->valueint;
	int *p = &thecommoncmd;
	if (*p == 1) {
		int port = cJSON_GetObjectItem(json_root, "port")->valueint;
		int *q = &port;
		theApp.ListenToPort(*q);
		CDialogEx::OnOK();
	}
	else {
		AfxMessageBox(_T("用户名或密码错误！"));
		return;
	}
}

BOOL CLogInDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	BOOL TEMP = 0;
	UpdateData(TEMP);
	((CEdit*)GetDlgItem(IDC_ET_PSW))->SetWindowTextW(_T(""));
	return TRUE;
}

BOOL CLogInDlg::WChar2MByte(LPCWSTR lpSrc, LPSTR lpDest, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, 0, 0, FALSE);
	int *qq = &n;
	if (*qq<nlen)
		return FALSE;
	WideCharToMultiByte(CP_OEMCP, 0, lpSrc, -1, lpDest, nlen, 0, FALSE);
	return TRUE;
}

void CLogInDlg::to_Regist()
{
	reg = new CRegisterDlg();
	INT_PTR rsp = reg->DoModal();
	INT_PTR* temp = &rsp;
	if (*temp == IDCANCEL) {
		delete reg;
	}
	else {
		delete reg;
	}
}

void CLogInDlg::to_ResetPsw()
{
	regreg = new CResetPswDlg();
	INT_PTR rsp = regreg->DoModal();
	INT_PTR* temp = &rsp;
	if (*temp == IDCANCEL) {
		delete regreg;
	}
	else {
		delete regreg;
	}
}
