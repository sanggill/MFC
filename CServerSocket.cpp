// CServerSocket.cpp: 구현 파일
//

#include "pch.h"
#include "socServer.h"
#include "CServerSocket.h"


// CServerSocket

CServerSocket::CServerSocket()
{
}

CServerSocket::~CServerSocket()
{
}
void CServerSocket::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}
void CServerSocket::OnAccept(int nErrorCode)
{
	CClientSocket* pClient = new CClientSocket;	//Client 소켓 포인터 추가

	if (Accept(*pClient))	//Listen에서 클라이언트 접속을 확인하면
	{
		pClient->SetWnd(m_hWnd);	//Client소켓에 메인핸들 연결
	}
	else		//클라이언트 접속 문제시
	{
		delete pClient;
		AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
	}
	SendMessage(m_hWnd, WM_ACCEPT_SOCKET, 0, (LPARAM)pClient);
	CSocket::OnAccept(nErrorCode);
}


// CServerSocket 멤버 함수
