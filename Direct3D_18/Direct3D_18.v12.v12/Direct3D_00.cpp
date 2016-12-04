// APIExam_005.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "cMainGame.h"

//
// 전역 변수
//
HWND		g_hWnd;		//윈도우 핸들 ( 자신의 프로그램에서 돌고있는 윈도우 번호 )
HINSTANCE	g_hInst;	//프로그램 인스턴스 핸들 ( OS 가 부여한 프로그램 번호 )

						//메인 게임 객체
cMainGame	g_MainGame;


// 콜백 프로시져 함수 프로토 타입 
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );


//
// 윈도우 메인 함수
//
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdParam, int nCmdShow )
{
	//OS 가알려준 프로그램 인스턴스 번호를 전역변수로 기억한다.
	g_hInst = hInstance;

	//윈도우 클레스 정보 생성
	//내가 이러한 윈도를 만들겠다 라는 정보
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.cbWndExtra = 0;			//윈도우에서 사용하는 여분의 메모리설정( 그냥 0 이다  신경쓰지말자 )
	WndClass.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );		//윈도우 배경색상
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//윈도우의 커서모양 결정
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//윈도우아이콘모양 결정
	WndClass.hInstance = hInstance;				//프로그램인스턴스핸들 
	WndClass.lpfnWndProc = (WNDPROC) WndProc;			//윈도우 프로시져 함수 포인터
	WndClass.lpszMenuName = NULL;						//메뉴이름 없으면 NULL
	WndClass.lpszClassName = WIN_CLASS;				//지금 작성하고 있는 윈도우 클레스의 이름
	WndClass.style = CS_HREDRAW | CS_VREDRAW;	//윈도우 그리기 방식 설정 ( 사이즈가 변경될때 화면갱신 CS_HREDRAW | CS_VREDRAW )

												//위에서 작성한 윈도우 클레스정보 등록
	RegisterClass( &WndClass );

	//윈도우 생성
	//생성된 윈도우 핸들을 전역변수 g_hWnd 가 받는다.
	g_hWnd = CreateWindow(
		WIN_CLASS,				//생성되는 윈도우의 클래스이름
		WIN_TITLE,				//윈도우 타이틀바에 출력되는 이름
		WIN_STYLE,				//윈도우 스타일
		WINPOS_X,				//윈도우 시작 위치 X 
		WINPOS_Y,				//윈도우 시작 위치 Y
		WINSIZE_X,				//윈도우 가로 크기 ( 작업영역의 크기가 아님 )
		WINSIZE_Y,				//윈도우 세로 크기 ( 작업영역의 크기가 아님 )
		GetDesktopWindow(),		//부모 윈도우 핸들 ( 프로그램에서 최상위 윈도우면 NULL 또는 GetDesktopWindow() )
		NULL,					//메뉴 ID ( 자신의 컨트롤 객체의 윈도우인경우 컨트롤 ID 가 된	
		hInstance,				//이 윈도우가 물릴 프로그램 인스턴스 핸들
		NULL					//추가 정보 NULL ( 신경끄자 )
	);

	//WINSIZE_X 와 WINSIZE_Y 의 작업영역의 크기를 
	//가지는 윈도우를 만든다.

	//만들려는 작업영영의 크기
	RECT rcClient = {0, 0, WINSIZE_X, WINSIZE_Y};

	//나 : rcClient 크기로 작업영역을 만들고 싶다.
	//WINAPI : 음 그렇다면 윈도우 형태를 알여줘라, 또한 메뉴도 있는지 알여줘라 그렇다면 내가 
	// 정확한 장업영역의 크기를 rcClient 알려주마. AdjustWindowRect 함수를 써라.
	AdjustWindowRect( &rcClient, WIN_STYLE, FALSE );

	//나 : 정확한 Client 의 영역을 알았으니 WindowSize를 변경해야지....
	//WINAPI : 그렇다면... SetWindowPos 함수를 사용하렴....
	SetWindowPos( g_hWnd,
		NULL,
		0, 0,		//위치는 0, 0, 으로...
		rcClient.right - rcClient.left,	//가로 크기
		rcClient.bottom - rcClient.top,	//세로 크기
		SWP_NOZORDER | SWP_NOMOVE		//나 : API 야 크기만 변할꺼니깐 깊이 순서랑,  위치 는 무시해라...
	);


	//메시지 구조체
	MSG Message;
	ZeroMemory( &Message, sizeof( MSG ) );

	//게임 초기화 성공했다면.....
	if ( SUCCEEDED( g_MainGame.Init() ) )
	{

		//위에서 만든 윈도우를 화면에 띄운다.
		ShowWindow( g_hWnd, nCmdShow );

		//
		// 메시지 루프
		//
		/*
		while( GetMessage( &Message, NULL, NULL, NULL ) )
		{
		TranslateMessage( &Message ); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
		DispatchMessage( &Message );  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
		}
		*/


		//메시큐에서 얻어온 메시지가 종료메시지가 아니면 계속 루프
		while ( Message.message != WM_QUIT ) {

			//메시지큐의 메시지를본다 
			//하지만 마지막의 PM_REMOVE 옵션으로인해 본메시지는
			//지운다.
			//만약 Message 가 없다면 false 를 리턴한다.
			if ( PeekMessage( &Message, NULL, NULL, NULL, PM_REMOVE ) ) {
				TranslateMessage( &Message ); //눌린 키보드 의 문자를 번역하여 WM_CHAR 메시지를 발생시킨다.
				DispatchMessage( &Message );  //받아온 메시지 정보로 윈도우 프로시져 함수를 실행시킨다.
			}

			//업데이트 
			g_MainGame.Update();

			//드로우
			g_MainGame.Draw();

		}

	}

	//해제
	g_MainGame.Release();

	//등록된 윈도우 클레스 해제 ( 해줘도 그만 안해줘도 그만.. )
	UnregisterClass( WIN_CLASS, hInstance );

	//윈도우 프로그램 종료	
	return (int) Message.wParam;
}

//
// 윈도우 프로시져 함수 ( 메시지 큐에서 받아온 메시지를 처리한다 )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	return g_MainGame.MainProc( hWnd, iMessage, wParam, lParam );
}