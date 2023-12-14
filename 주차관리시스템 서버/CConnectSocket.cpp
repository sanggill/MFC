// CConnectSocket.cpp: 구현 파일
//

#include "pch.h"
#include "SocketServer.h"
#include "CConnectSocket.h"
#include "SocketServerDlg.h"
#include<string>
// CConnectSocket

CConnectSocket::CConnectSocket()
{
}
CConnectSocket::~CConnectSocket()
{
}
std::wstring UTF8toUTF16(const std::string& utf8Str) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
    std::wstring utf16Str(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &utf16Str[0], len);
    return utf16Str;
}


/////////////////////////////////////////////
//         파이썬 데이터 받기              //
////////////////////////////////////////////

void CConnectSocket::OnReceive(int nErrorCode)
{
    CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd();
    char szBuffer[1024]; // UTF-8 문자열을 저장할 버퍼
    ::ZeroMemory(szBuffer, sizeof(szBuffer));

    // 데이터 수신
    if (Receive(szBuffer, sizeof(szBuffer)) > 0) 
    {
        // UTF-8 문자열을 UTF-16으로 변환
        wchar_t unicodeBuffer[1024];
        MultiByteToWideChar(CP_UTF8, 0, szBuffer, -1, unicodeBuffer, 1024);

        // 변환된 유니코드 문자열을 CString에 저장
        CString receivedData(unicodeBuffer);

        // 분할 기준 문자열 설정
        CString delimiter = _T("/!@#/");
        CStringArray tokens; // 토큰을 저장할 배열

        // Tokenize 함수를 사용하여 데이터를 분할하고 tokens 배열에 저장
        int nTokenPos = 0;
        CString token = receivedData.Tokenize(delimiter, nTokenPos);
        while (!token.IsEmpty()) {
            tokens.Add(token);
            token = receivedData.Tokenize(delimiter, nTokenPos);
        }

        // 분할된 토큰 처리
        for (int i = 0; i < tokens.GetSize(); ++i) {
            pMain->m_List.AddString(_T("받은값: ") + tokens.GetAt(i));
        }


        if (tokens[0] == "warning")
        {
            int num = 1;
            pMain->arduino(num); //아두이노함수 이동
            pMain->client_msg(receivedData); // 클라이언트 함수 이동
          
        }
        else
        {
            int num = 2;
            pMain->arduino(num); //아두이노함수 이동
        }
    }
      
    // 기본 수신 처리 호출
    CSocket::OnReceive(nErrorCode);
}


// CConnectSocket 멤버 함수
