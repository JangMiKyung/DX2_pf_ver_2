#pragma once


//로그 출력 타입
#define LOG_NONE		0x00000000
#define LOG_CONSOLE		0x00000001
#define LOG_FILE		0x00000002
#define LOG_WINDOW		0x00000004
#define LOG_ALL			( LOG_CONSOLE | LOG_FILE | LOG_WINDOW )

#define LOG_WINDOW_TITLE	"Log..."		//로그 창 윈도우 타이틀
#define LOG_WINDOW_CX		250				//로그 창 윈도우 가로 크기
#define LOG_WINDOW_CY		600				//로그 창 윈도우 세로 크기


//
// 로그 매니져
//
#include "cSingletonBase.h"

class cLogMgr : public cSingletonBase<cLogMgr>
{
private:
	unsigned int		m_nTarget;
	char				m_szFileName[MAX_PATH];		//로그 내용 저장할 파일 이름
	HWND				m_hWndLog;					//로그창 윈도우 핸들
	HWND				m_hWndListBox;				//로그창 리스트 박스

protected:
	cLogMgr( void );

public:
	~cLogMgr( void );


public:

	//BaseSingleton 친구 등록
	friend cLogMgr* cSingletonBase<cLogMgr>::GetInstance( void );

	//로그창 설정
	HRESULT			Init( unsigned int nTarget, HWND hParentWnd, const char* pszFileName = NULL );

	//로그 찍기
	VOID			AddLog( char* strmsg, ... );

	//파일로 로그 저장
	VOID			LogFile( char* strmsg, ... );

	//해제
	VOID			Release( VOID );


private:
	//로그 윈도우생성
	VOID			CreateLogWindow( HWND hWnd );

	//로그 윈도우 메세지 프로시져
	static LRESULT CALLBACK WndLogProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
};


#define LOG_MGR cLogMgr::GetInstance()
