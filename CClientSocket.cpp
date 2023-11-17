// CClientSocket.cpp: 구현 파일
//

#include "pch.h"
#include "socServer.h"
#include "CClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}
void CClientSocket::SetWnd(HWND hWnd)
{
	m_hWnd = hWnd;
}
void CClientSocket::OnReceive(int nErrorCode)
{
    CString strTmp = _T(""), strIPAddress = _T("");
    UINT uPortNumber = 0;
    TCHAR strBuffer[1024];
    ZeroMemory(strBuffer, sizeof(strBuffer));

    GetPeerName(strIPAddress, uPortNumber);

    // Receive 함수의 반환값 확인
    int nBytesReceived = Receive(strBuffer, sizeof(strBuffer));
    if (nBytesReceived > 0) { // 전달된 데이터(문자열)가 있을 경우
        // 받은 데이터의 끝에 Null 종료 문자 추가
        strBuffer[nBytesReceived / sizeof(TCHAR)] = _T('\0');

        strTmp.Format(_T("[%s : %d]: %s"), strIPAddress, uPortNumber, strBuffer);

        // WM_CLIENT_MSG_RECV 메시지를 통해 데이터 전달
        if (m_hWnd && ::IsWindow(m_hWnd)) {
            ::SendMessage(m_hWnd, WM_CLIENT_MSG_RECV, 0, (LPARAM)((LPCTSTR)strTmp));
        }
    }

    CSocket::OnReceive(nErrorCode);
}

void CClientSocket::OnClose(int nErrorCode)
{
    if (m_hWnd && ::IsWindow(m_hWnd)) {
        ::SendMessage(m_hWnd, WM_CLIENT_CLOSE, 0, (LPARAM)this);
    }

	CSocket::OnClose(nErrorCode);
}

// CClientSocket 멤버 함수
