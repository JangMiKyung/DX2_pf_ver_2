#include "StdAfx.h"
#include "cLogMgr.h"
#include <Windows.h>
#include <MMSystem.h>
#include <stdio.h>
#include <iostream>
#include <time.h>


cLogMgr::cLogMgr( void )
	: m_hWndLog( NULL ),
	m_hWndListBox( NULL ),
	m_nTarget( LOG_NONE )
{
	m_szFileName[0] = NULL;
}

cLogMgr::~cLogMgr( void )
{
	Release();
}


//로그창 설정
HRESULT	cLogMgr::Init( unsigned int nTarget, HWND hParentWnd, const char* pszFileName )
{
	m_nTarget = nTarget;


	//로그파일로 저장한다면...
	if ( m_nTarget & LOG_FILE )
		strcpy_s( m_szFileName, MAX_PATH, pszFileName );

	else
		m_szFileName[0] = NULL;


	//로그 윈도우라면...
	if ( m_nTarget & LOG_WINDOW )
		CreateLogWindow( hParentWnd );

	AddLog( "============== Begin Log ============== " );

	return S_OK;
}

//로그 찍기
VOID cLogMgr::AddLog( char* strmsg, ... )
{

	char szBuff[4096];
	char szDate[128];		//날짜
	char szTime[128];		//시간

	_strdate_s( szDate, 128 );
	_strtime_s( szTime, 128 );
	vsprintf_s( szBuff, strmsg, (char*) ( &strmsg + 1 ) );

	//Console에 출력할 경우
	if ( m_nTarget & LOG_CONSOLE )
	{
		printf( "(data[%s] time[%s] ) : %s\n", szDate, szTime, szBuff );
	}

	//FILE 에 출력 할경우
	if ( m_nTarget & LOG_FILE )
	{
		LogFile( "(data[%s] time[%s] ) : %s\n", szDate, szTime, szBuff );
	}

	//LOG 윈도우에 출력할 경우 ( 디버그 모드에서만 지원
#ifdef _DEBUG

	if ( m_nTarget & LOG_WINDOW )
	{
		//윈도우 리스트 박스에 로그 추가
		SendMessage( m_hWndListBox, LB_ADDSTRING, 0, (LPARAM) szBuff );

		//추가된 리스트 박스의 순번 얻기 (마지막을 선택하면 된다 )
		UINT32 n = SendMessage( m_hWndListBox, LB_GETCOUNT, 0, 0 ) - 1;

		//선택 커서 마지막으로
		SendMessage( m_hWndListBox, LB_SETCURSEL, (WPARAM) n, 0 );
	}

#endif //_DEBUG
}

//파일로 로그 저장
VOID cLogMgr::LogFile( char* strmsg, ... )
{
	//현시간을 얻어온다.
	SYSTEMTIME		st;
	GetLocalTime( &st );

	char szBuf[256];		//파일 이름

	if ( m_nTarget & LOG_FILE )
	{
		wsprintf( szBuf, "%s.txt", m_szFileName );
	}

	// c형 스타일의 파일입출력 
	FILE*		fp = NULL;
	//fp = fopen(szBuf,"a");		//기존에 파일이 있으면 추가로 붙여 넣는다.
	fopen_s( &fp, szBuf, "a" );

	// 에러 처리 
	if ( fp == NULL )
	{
		MessageBox( 0, "에러 파일로 저장 실패", 0, 0 );
		return;
	}

	else
	{
		char	buffer[4096];
		va_list list;					// 리스트 변수를 선언한다.

		va_start( list, strmsg );			// 포맷에 시작
		vsprintf_s( buffer, strmsg, list );	// 맞게 자르기 
		va_end( list );					// 리스트 끝 
		fprintf( fp, "%s\n", buffer );
	}

	//파일 닫는다.
	if ( fp )
	{
		fclose( fp );
		fp = NULL;
	}
}

//해재
VOID cLogMgr::Release( VOID )
{
	if ( m_hWndLog != NULL )
	{
		DestroyWindow( m_hWndLog );
	}
}


///////////////////////////////////////////////////////////


//로그 윈도우생성
VOID cLogMgr::CreateLogWindow( HWND hWnd )
{
	//
	// 로그 윈도우 생성
	//
	int			x, y, cx, cy;
	WNDCLASS	wc;
	RECT		rc;

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC) cLogMgr::WndLogProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle( NULL );
	wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "LogWin";

	RegisterClass( &wc );


	//부모 윈도우 오른쪽에 위치하게....
	RECT	rcWin;
	GetWindowRect( hWnd, &rcWin );

	cx = LOG_WINDOW_CX;
	cy = LOG_WINDOW_CY;
	x = rcWin.right;
	y = rcWin.top;

	HWND		hParenthWnd = NULL;
	HINSTANCE	hInst = NULL;

	hParenthWnd = hWnd;
	hInst = GetModuleHandle( NULL );

	m_hWndLog = CreateWindow(
		"LogWin",
		LOG_WINDOW_TITLE,
		WS_POPUP | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		x, y, cx, cy, hParenthWnd, NULL,
		hInst, NULL );


	//
	// 로그 윈도우에 리스트 박스 윈도우 붙여 넣는다.
	//
	GetClientRect( m_hWndLog, &rc );
	m_hWndListBox = CreateWindow( "listbox", "", WS_CHILD | WS_VSCROLL, 0, 0, rc.right, rc.bottom, m_hWndLog, NULL, GetModuleHandle( NULL ), NULL );
	ShowWindow( m_hWndLog, SW_SHOW );
	ShowWindow( m_hWndListBox, SW_SHOW );

}


//로그 윈도우 메세지 프로시져
LRESULT CALLBACK cLogMgr::WndLogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}