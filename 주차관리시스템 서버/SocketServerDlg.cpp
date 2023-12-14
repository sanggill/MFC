
// SocketServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"
#include <windows.h>
#include <iostream>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSocketServerDlg 대화 상자



CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_PROGRESS1, m_fileProgress);
}

BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSocketServerDlg::OnBnClickedButton1)
	
END_MESSAGE_MAP()


// CSocketServerDlg 메시지 처리기

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_ListenSocket.Create(3000, SOCK_STREAM)) {
		if (!m_ListenSocket.Listen()) {
			AfxMessageBox(_T("ERROR:Listen() return False"));
		}
	}
	else {
		AfxMessageBox(_T("ERROR:Failed to create server socket!"));
	}

	m_ConnectSocket.Create();
	if (m_ConnectSocket.Connect(_T("10.10.21.125"), 25000) == FALSE) {
		AfxMessageBox(_T("ERROR : Failed to connect Server"));
		PostQuitMessage(0);
		return FALSE;
	}

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	data_base();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSocketServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketServerDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	CString text;
	// 방법2. 객체생성해서 쓸때
	CEdit* test = (CEdit*)GetDlgItem(IDC_INPUT);
	test->GetWindowTextW(text);

	str.Format(L"관리자: %s", text);

	m_List.AddString(str);
	m_List.SetCurSel(m_List.GetCount() - 1);

	POSITION pos;
	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL) {
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			int checkLenOfData = pClient->Send((LPVOID)(LPCTSTR)str, str.GetLength() * 2);
			if (checkLenOfData != lstrlen(str) * 2) {
				AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다!"));
			}
		}
	}
	test->SetWindowTextW(_T(""));
}

void CSocketServerDlg::OnDestroy()
{
	CDialog::OnDestroy();
	CString str;

	POSITION pos;

	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;


	while (pos != NULL) {
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) {
			str.Format(L"접속해제: Client (%d)", (int)m_ListenSocket.m_ptrClientSocketList.Find(pClient));
			m_List.AddString(str);
			m_List.SetCurSel(m_List.GetCount() - 1);
			
			pClient->ShutDown();
			pClient->Close();

			delete pClient;
		}
	}
	m_ListenSocket.ShutDown();
	m_ListenSocket.Close();
}




////////////////////////////////////////////
// 파이썬으로 이미지 파일 전송 
////////////////////////////////////////////

void CSocketServerDlg::py_data(CString Filesize_Token, CString Filename_Token)
{
	int error;
	int len = sizeof(error);
	m_ConnectSocket.GetSockOpt(SO_ERROR, &error, &len);

	if (error == 0) {
		m_List.AddString(_T("파이썬 소켓은 연결되어 있습니다."));
	}
	else {
		m_List.AddString(_T("파이썬 소켓은 연결되어 있지 않습니다."));
	}
	CString saveFolderPath = _T("C:\\Users\\lms111\\Desktop\\SaveFile\\");
	CString saveFilePath = saveFolderPath + Filename_Token;
	CString spname;
	spname.Format(_T("sendimg/!@#/%s/!@#/%s/!@#/"), Filename_Token, Filesize_Token);
	CFile openFile;
     
	if (openFile.Open(saveFilePath, CFile::modeRead)) {
		const int chunkSize = 4096;  // 조각의 크기 설정
		char chunkBuffer[chunkSize];  // 조각을 저장할 버퍼
		ULONGLONG openFileSize = openFile.GetLength();
		// 전체 파일 데이터를 읽음
		char* openFileBuffer = new char[openFileSize];
		openFile.Read(openFileBuffer, (UINT)openFileSize);

		// 파일 이름과 파일 크기를 먼저 전송
		CStringA utf8Spname(spname); // UTF-8로 변환
		m_ConnectSocket.Send(utf8Spname.GetBuffer(), utf8Spname.GetLength());
		Sleep(100);
		// UTF-8로 변환된 파일 데이터를 조각으로 나누어 전송
		ULONGLONG totalSent = 0;
		while (totalSent < openFileSize) {
			int bytesToSend = min(chunkSize, (int)(openFileSize - totalSent));

			// UTF-8로 변환된 파일 조각을 생성
			CStringA utf8Chunk(openFileBuffer + totalSent, bytesToSend);

			// 조각을 서버로 전송
			m_ConnectSocket.Send(utf8Chunk.GetBuffer(), bytesToSend);

			// 전송한 바이트 수를 누적
			totalSent += bytesToSend;
			CString progressInfo;
			if (totalSent == bytesToSend) {
				progressInfo.Format(_T("PY_THON Sent %llu bytes out of %llu bytes"), totalSent, openFileSize);
				m_List.AddString(progressInfo);
			}
		}

		openFile.Close();
		delete[] openFileBuffer;
	}
	else {
		CString progressInfo;
		progressInfo.Format(_T("파일 보내는중에 오류"));
		m_List.AddString(progressInfo);
	} 
	
}


///////////////////////////////////////////////
//아두이노에 사이렌 요청 
//////////////////////////////////////////////

int CSocketServerDlg::arduino(int number)
{
	HANDLE hSerial;
	hSerial = CreateFile(L"\\\\.\\COM4", GENERIC_WRITE | GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			m_List.AddString(_T("Serial port doesn't exist!"));
		}
		else 
		{
			m_List.AddString(_T("Error opening serial port!"));
		}
		return 1;
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if (!GetCommState(hSerial, &dcbSerialParams))
	{
		m_List.AddString(_T("Error getting serial port state!"));
		CloseHandle(hSerial);
		return 1;
	}

	dcbSerialParams.BaudRate = CBR_9600; // 통신 속도
	dcbSerialParams.ByteSize = 8;       // 데이터 비트
	dcbSerialParams.StopBits = ONESTOPBIT; // 스탑 비트
	dcbSerialParams.Parity = NOPARITY;   // 패리티

	if (!SetCommState(hSerial, &dcbSerialParams)) 
	{
		m_List.AddString(_T("Error setting serial port state!"));
		CloseHandle(hSerial);
		return 1;
	}
	if (number== 1)
	{
		const char* data = "1";
		DWORD bytes_written;
		m_List.AddString(_T("들어옴"));
		WriteFile(hSerial, data, strlen(data), &bytes_written, NULL);
		
	}
	else if (number == 2) 
	{
		const char* data = "2";
		DWORD bytes_written;
		WriteFile(hSerial, data, strlen(data), &bytes_written, NULL);
	}
	CloseHandle(hSerial);
	
}


//////////////////////////////////////////////
// 저장된 차량 외 다른 차량 시 클라이언트로 차량번호 전송
///////////////////////////////////////////////

void CSocketServerDlg::client_msg(CString msg)
{
	CString delimiter = _T("/!@#/");
	CStringArray tokens; // 토큰을 저장할 배열

	// Tokenize 함수를 사용하여 데이터를 분할하고 tokens 배열에 저장
	int nTokenPos = 0;
	CString token = msg.Tokenize(delimiter, nTokenPos);
	while (!token.IsEmpty()) {
		tokens.Add(token);
		token = msg.Tokenize(delimiter, nTokenPos);
	}
	CString carnum;
	if (tokens.GetSize() < 2 || tokens[1].IsEmpty()) 
	{
		carnum = "번호없음";
	}
	else {
		carnum = tokens[1];
	}


	CRecordset rs(&db);
	CString strSQL;
	strSQL.Format(_T("SELECT time FROM registerNum WHERE carChk = '%s'"), carnum);

	rs.Open(CRecordset::forwardOnly, strSQL);

	CString strName;
	// 결과 처리
	while (!rs.IsEOF()) {
		rs.GetFieldValue(_T("time"), strName);
		rs.MoveNext();
	}


    m_List.AddString(_T("클라이언트 보낼 값: ") + strName);
	POSITION pos;
	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL) {
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);
		if (pClient != NULL) 
		{
			CString time_num = carnum +"/!@#/"+strName;
			int checkLenOfData = pClient->Send((LPVOID)(LPCTSTR)time_num, time_num.GetLength() * 2);
			m_List.AddString(_T("클라 최종값:" + time_num));
			if (checkLenOfData != lstrlen(time_num) * 2) {
				AfxMessageBox(_T("일부 데이터가 정상적으로 전송되지 못했습니다!"));
			}
		}
	}

}
void CSocketServerDlg::data_base()
{
	CString strConnection = _T("DRIVER={MySQL ODBC 8.2 Unicode Driver};SERVER=10.10.21.125;DATABASE=carNumberCheck;UID=username;PWD=1234");

	m_List.AddString(_T(" 데이터베이스 연결 중."));
	// 연결 성공 시 작업 수행
	TRY
	{
		db.Open(NULL, FALSE, FALSE, strConnection);
	m_List.AddString(_T(" 데이터베이스 연결 완료."));
	// 연결 성공 시 작업 수행
	}
		CATCH(CDBException, e)
	{
		// 연결 실패 시 예외 처리
		AfxMessageBox(_T("Database Error: ") + e->m_strError);
		m_List.AddString(_T(" 데이터베이스 연결 안됨."));
	}
	END_CATCH

}
