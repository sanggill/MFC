#include "pch.h"
#include "ListenSocket.h"
#include "ClientSocket.h"
#include "SocketServerDlg.h"
#include "SocketServer.h"

CListenSocket::CListenSocket()
{
}

CListenSocket::~CListenSocket()
{
}

// CListenSocket 멤버 함수
void CListenSocket::OnAccept(int nErrorCode)
{
	CClientSocket* pClient = new CClientSocket;
	CString str;

	if (Accept(*pClient)) { // 클라이언트 접속 요청이 오면 서버-클라이언트를 연결시켜준다
		pClient->SetListenSocket(this);
		m_ptrClientSocketList.AddTail(pClient); // 리스트에 클라이언트 소켓 저장

		CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd(); // CSocketServerDlg의 핸들을 가져옴
		str.Format(L"접속: Client (%d)", (int)m_ptrClientSocketList.Find(pClient)); // 클라이언트 번호(POSITION(주소) 값)
		pMain->m_List.AddString(str);
		pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);
	}
	else {
		delete pClient;
		AfxMessageBox(_T("ERROR : Failed can't accept new Client!"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

void CListenSocket::CloseClientSocket(CSocket* pClient)
{
	POSITION pos;
	CString str;
	CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd();
	pos = m_ptrClientSocketList.Find(pClient);

	if (pos != NULL) {
		if (pClient != NULL) {
			str.Format(L"접속해제: Client (%d)", (int)m_ptrClientSocketList.Find(pClient));
			pMain->m_List.AddString(str);
			pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);
			pClient->ShutDown();
			pClient->Close();
		}
		m_ptrClientSocketList.RemoveAt(pos);
		delete pClient;
	}
}

void CListenSocket::SendAllMessage(const TCHAR* pszMessage)
{
	POSITION pos;
	pos = m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL) {
		pClient = (CClientSocket*)m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			int checkLenOfData = pClient->Send(pszMessage, lstrlen(pszMessage) * 2);
			if (checkLenOfData != lstrlen(pszMessage) * 2) {
				AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다!"));
			}
		}
	}
}