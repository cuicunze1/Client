
// Client.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "ChatDlg.h"
#include "LogInDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientApp ����

CClientApp::CClientApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴����ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CClientApp ����

CClientApp theApp;


// CClientApp ��ʼ��

BOOL CClientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	SetRegistryKey(_T("ChatClient"));

	//��ʼ��Socket
	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("��ʼ��Socket��ʧ�ܣ�"));
		return false;
	}

	m_pSocket = new CSessionSocket();
	m_pSocket->Create(0, SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_CONNECT | FD_CLOSE);
	pLogoinDlg = new CLogInDlg();
	CString m_strthetheUser;
	INT_PTR INTPTR_rsp = pLogoinDlg->DoModal();
	if (INTPTR_rsp == IDCANCEL)
	{
		AfxMessageBox(_T("��ӭ�������"));
		delete pLogoinDlg;
		m_pSocket->Close();
		return false;
	}
	else
	{
		m_strthetheUser = pLogoinDlg->m_strthetheUser;
		delete pLogoinDlg;
	}

	CClientDlg dlg;
	dlg.m_caption = m_strthetheUser;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô�����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

int CClientApp::Exit()
{
	if (m_pSocket)
	{
		delete m_pSocket;
		m_pSocket = NULL;
	}

	return CWinApp::ExitInstance();
}

CSessionSocket* CClientApp::GetMainSocket() const
{
	return m_pSocket;
}

CLogInDlg *CClientApp::TurnintoLog() const
{
	return pLogoinDlg;
}

void CClientApp::ListenToPort(int Port) {
	m_pListenSocket = new CListenSocket();
	if (!m_pListenSocket)
	{
		AfxMessageBox(_T("��̬�����������׽��ֳ���!"));
		return;
	}

	if (m_pListenSocket->Create(Port, SOCK_STREAM, FD_ACCEPT) == FALSE)
	{
		AfxMessageBox(_T("�����׽���ʧ��!"), MB_OK | MB_ICONEXCLAMATION);
		m_pListenSocket->Close();
		return;
	}

	// �����ɹ����ȴ���������
	if (m_pListenSocket->Listen() == FALSE)
	{
		if (m_pListenSocket->GetLastError() == WSAEWOULDBLOCK)
		{
			AfxMessageBox(_T("��������ʧ��!"));//��Ϊvs2005Ĭ��ʹ�õ���unicode�ַ����뼯����unicodeҪռ2byte,ͨ�����ַ�ֻռ1byte,���Ե����޷�ת��������Ҫ���� _T("") �� L"" ����ת����
			m_pListenSocket->Close();
			return;
		}
	}
}