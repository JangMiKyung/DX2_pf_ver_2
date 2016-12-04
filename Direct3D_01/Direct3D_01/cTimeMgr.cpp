#include "StdAfx.h"
#include "cTimeMgr.h"
#include <stdio.h>

//timeGetTime 을 이용하기위한 헤더와 라이브러리
#include <MMSystem.h>
#pragma comment( lib, "winmm.lib" )		//winmm.lib 추가

cTimeMgr::cTimeMgr( void )
{
}

cTimeMgr::~cTimeMgr( void )
{
}

HRESULT cTimeMgr::Init( void )
{
	//고성능 타이머 지원 여부
	//QueryPerformanceFrequency 함수로 초당 카운트 가능한 값을 얻는다.
	//고성능 타이머 지원 된다면 TRUE 안된다면 FALSE
	if ( QueryPerformanceFrequency( (LARGE_INTEGER*) &m_nPreriodTime ) )
	{
		//고성능 타이머를 지원한다면...
		m_bHardWare = true;

		//1 카운트당 초가 얼마나 되느냐?
		m_TimeScaleSec = 1.0 / m_nPreriodTime;

		//초기화 시점의 시간을 LastTime 으로...
		QueryPerformanceCounter( (LARGE_INTEGER*) &m_nLastTime );
	}

	else
	{
		//고성능 타이머를 지원하지 않는다면...
		m_bHardWare = false;

		//1000 밀리세컨드 단위의 시간값을 얻는다.
		//timeGetTime 이 GetTickCount 보다 좀더 정밀하다.
		m_nLastTime = timeGetTime();

		//1 카운트당 초가 얼마나 되느냐?
		m_TimeScaleSec = 1.0 / 1000.0;

	}

	m_TotalSec = 0.0f;
	m_FrameCountSec = 0.0f;
	m_FramePerSec = 0;
	m_FrameCount = 0;

	return S_OK;
}

void cTimeMgr::Release( void )
{
}

//매업데이트 물려준다.
void cTimeMgr::UpdateTime( float frameLock )
{
	//현제 시간을 얻는다.
	if ( m_bHardWare )
		QueryPerformanceCounter( (LARGE_INTEGER*) &m_nCurTime );
	else
		m_nCurTime = timeGetTime();

	//지난 시간과의 격차를 계산
	m_nDeltaTime = m_nCurTime - m_nLastTime;


	//한프레임 간격의 지난 초시간 계산
	m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;

	//프레임 락이 존해한다면...( 프레임 스키핑 )
	if ( frameLock > 0.0f ) {

		//고여있을 시간.
		float fLockTime = ( 1.0f / frameLock );

		while ( m_FrameDeltaSec < fLockTime )
		{
			//현제 시간을 얻는다.
			if ( m_bHardWare )
				QueryPerformanceCounter( (LARGE_INTEGER*) &m_nCurTime );
			else
				m_nCurTime = timeGetTime();

			//지난 시간과의 격를 계산
			m_nDeltaTime = m_nCurTime - m_nLastTime;

			//한프레임 간격의 지난 초시간 계산
			m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;
		}

	}

	//지난 시간 갱신
	m_nLastTime = m_nCurTime;

	//총지난 시간 누적
	m_TotalSec += m_FrameDeltaSec;

	//프레임 초 누적
	m_FrameCountSec += m_FrameDeltaSec;

	//1초가 지났다면....
	if ( m_FrameCountSec >= 1.0 ) {
		m_FramePerSec = m_FrameCount;
		m_FrameCount = 0;
		m_FrameCountSec -= 1.0;
	}
	m_FrameCount++;


}

//타임정보를 출력한다
void cTimeMgr::DrawTimeInfo( HDC hdc )
{
	char szTemp[128];
	wsprintf( szTemp, "FPS : %d", m_FramePerSec );
	DrawShadowText( hdc, 0, 0, szTemp, 0x0000ffff );

	sprintf_s( szTemp, "FrameDeltaSec : %.2f", m_FrameDeltaSec );
	DrawShadowText( hdc, 0, 20, szTemp, 0x0000ffff );

	sprintf_s( szTemp, "TotalDeltaSec : %.2f", m_TotalSec );
	DrawShadowText( hdc, 0, 40, szTemp, 0x0000ffff );
}

//그림자 Text
void cTimeMgr::DrawShadowText( HDC hdc, int x, int y, char* szText, COLORREF textColor )
{
	//Text 배경을 투명으로 
	SetBkMode( hdc, TRANSPARENT );

	SetTextColor( hdc, 0x00000000 );
	TextOut( hdc, x + 1, y + 1, szText, strlen( szText ) );

	SetTextColor( hdc, textColor );
	TextOut( hdc, x, y, szText, strlen( szText ) );

	SetTextColor( hdc, 0x00000000 );

	//Text 불투명으로
	SetBkMode( hdc, OPAQUE );
}