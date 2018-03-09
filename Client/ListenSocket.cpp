// ListenSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ListenSocket.h"
#include "ChatDlg.h"
#include "ClientDlg.h"


// CListenSocket

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}


// CListenSocket ��Ա����


void CListenSocket::OnAccept(int nErrorCode)
{
	CChatDlg *pNewDlg = new CChatDlg();
	CSessionSocket *pNewSocket = new CSessionSocket();
	if (Accept(*pNewSocket)) {
		pNewDlg->pthechatsoc = pNewSocket;
		pNewDlg->is_online = true;
		pNewDlg->Create(IDD_CHATDLG);
		AfxMessageBox(_T("��������Ϣ��"));
		pNewDlg->ShowWindow(SW_SHOW);
		pNewDlg->pthechatsoc->AsyncSelect(FD_READ | FD_CLOSE);
		((CClientDlg*)(AfxGetApp()->GetMainWnd()))->m_chatList->AddTail(pNewDlg);
	}
	else {
		delete pNewDlg;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}
