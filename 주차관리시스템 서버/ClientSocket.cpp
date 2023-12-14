#include "pch.h"
#include "ClientSocket.h"
#include "ListenSocket.h"
#include "SocketServerDlg.h"
#include "SocketServer.h"

#define BUF_SIZE 1000000

CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}

void CClientSocket::SetListenSocket(CAsyncSocket* pSocket)
{
	m_pListenSocket = pSocket;
}

void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
	pServerSocket->CloseClientSocket(this);
}


/////////////////////////////////////////
//     클라이언트한테 데이터 받기      //
////////////////////////////////////////

void CClientSocket::OnReceive(int nErrorCode)
{
    CSocketServerDlg* pMain = (CSocketServerDlg*)AfxGetMainWnd();
    
    CString strTime;
    // 현재 시간 정보를 가져오는 부분
    time_t t = time(NULL);
    struct tm tm;
    errno_t err_t = localtime_s(&tm, &t);
    if (err_t != 0) {
        pMain->m_List.AddString(L"Time ERROR!");
    }
    strTime.Format(L"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    CString strTmp = _T(""), strIPAddress = _T("");
    UINT uPortNumber = 0;
    TCHAR strBuffer[4096];
    ::ZeroMemory(strBuffer, sizeof(strBuffer));
    // ::은 범위지정 연산자  strBuffer의 배열을 0으로 지정

    GetPeerName(strIPAddress, uPortNumber);
    if (Receive(strBuffer, sizeof(strBuffer)) > 0) { // 전달된 데이터(문자열)가 있을 경우
        TRACE(_T("Received data: %s\n"), strBuffer);
        CStringArray tokens;
        ParseFileHeader(strBuffer, tokens, _T(','));
        if (tokens.GetSize() >= 2 && num != 0) {
            CString Type_Token = tokens.GetAt(0);

            if (Type_Token == "Type:Message") // 구분자를 설정함
            {
                CString Message_Token = tokens.GetAt(1);
                strTmp.Format(_T("[%s:%d]: %s"), strIPAddress, uPortNumber, Message_Token);
                pMain->m_List.AddString(strTmp);  // 메시지 리스트(메시지창?)에 입력받은 메시지 띄우기
                pMain->m_List.SetCurSel(pMain->m_List.GetCount() - 1);
                CListenSocket* pServerSocket = (CListenSocket*)m_pListenSocket;
                pServerSocket->SendAllMessage(Message_Token.GetBuffer()); // 다른 클라이언트들에게 메시지 전달
            }

           


            else if (Type_Token == "Type:File")
            {
                CString Filesize_Token = tokens.GetAt(1);
                CString Filename_Token = tokens.GetAt(2);


                strTmp.Format(_T("Received File: Size=%s, Name=%s\n"), Filesize_Token, Filename_Token);
                pMain->m_List.AddString(strTmp);

                ULONGLONG fileSize = _ttoi64(Filesize_Token);
                char* fileBuffer = new char[fileSize];

                UINT totalBytesRead = 0;
                while (totalBytesRead < fileSize)
                {
                    UINT bytesRead = Receive(fileBuffer + totalBytesRead, (UINT)(fileSize - totalBytesRead));
                    if (bytesRead <= 0)
                    {

                        TRACE(_T("Failed to receive file data\n"));
                        break;

                    }

                    totalBytesRead += bytesRead;

                }
                CString saveFolderPath = _T("C:\\Users\\lms111\\Desktop\\SaveFile\\");
                CString saveFilePath = saveFolderPath + Filename_Token;
                //    // 파일을 저장
                CFile file;
                if (file.Open(saveFilePath, CFile::modeCreate | CFile::modeWrite))
                {
                    file.Write(fileBuffer, (UINT)fileSize);
                    file.Close();
                    TRACE(_T("File saved to: %s\n"), saveFilePath);

                    // 따로 저장된 경로와 파일 이름을 표시
                    pMain->m_List.AddString(_T("다운로드 경로: ") + saveFolderPath);
                    pMain->m_List.AddString(_T("파일 이름: ") + Filename_Token);
                }
                pMain->m_fileProgress.SetPos(100);
                // 다운로드 완료 메시지를 로그에 기록
                pMain->m_List.AddString(strTime);
                pMain->m_List.AddString(L"Download complete");
                delete[] fileBuffer;
                pMain->py_data(Filesize_Token, Filename_Token);
            
            }
        }
        num++;
    }
    CSocket::OnReceive(nErrorCode);
}

void CClientSocket::ParseFileHeader(const CString& input, CStringArray& output, TCHAR delimiter)
{
    int start = 0;
    int end = 0;

    while ((end = input.Find(delimiter, start)) != -1)
    {
        CString token = input.Mid(start, end - start);
        output.Add(token);
        start = end + 1;
    }

    CString lastToken = input.Mid(start);
    output.Add(lastToken);
}