#include "StdAfx.h"
#include "cTimeMgr.h"
#include <stdio.h>

//timeGetTime �� �̿��ϱ����� ����� ���̺귯��
#include <MMSystem.h>
#pragma comment( lib, "winmm.lib" )		//winmm.lib �߰�

cTimeMgr::cTimeMgr( void )
{
}

cTimeMgr::~cTimeMgr( void )
{
}

HRESULT cTimeMgr::Init( void )
{
	//���� Ÿ�̸� ���� ����
	//QueryPerformanceFrequency �Լ��� �ʴ� ī��Ʈ ������ ���� ��´�.
	//���� Ÿ�̸� ���� �ȴٸ� TRUE �ȵȴٸ� FALSE
	if ( QueryPerformanceFrequency( (LARGE_INTEGER*) &m_nPreriodTime ) )
	{
		//���� Ÿ�̸Ӹ� �����Ѵٸ�...
		m_bHardWare = true;

		//1 ī��Ʈ�� �ʰ� �󸶳� �Ǵ���?
		m_TimeScaleSec = 1.0 / m_nPreriodTime;

		//�ʱ�ȭ ������ �ð��� LastTime ����...
		QueryPerformanceCounter( (LARGE_INTEGER*) &m_nLastTime );
	}

	else
	{
		//���� Ÿ�̸Ӹ� �������� �ʴ´ٸ�...
		m_bHardWare = false;

		//1000 �и������� ������ �ð����� ��´�.
		//timeGetTime �� GetTickCount ���� ���� �����ϴ�.
		m_nLastTime = timeGetTime();

		//1 ī��Ʈ�� �ʰ� �󸶳� �Ǵ���?
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

//�ž�����Ʈ �����ش�.
void cTimeMgr::UpdateTime( float frameLock )
{
	//���� �ð��� ��´�.
	if ( m_bHardWare )
		QueryPerformanceCounter( (LARGE_INTEGER*) &m_nCurTime );
	else
		m_nCurTime = timeGetTime();

	//���� �ð����� ������ ���
	m_nDeltaTime = m_nCurTime - m_nLastTime;


	//�������� ������ ���� �ʽð� ���
	m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;

	//������ ���� �����Ѵٸ�...( ������ ��Ű�� )
	if ( frameLock > 0.0f ) {

		//������ �ð�.
		float fLockTime = ( 1.0f / frameLock );

		while ( m_FrameDeltaSec < fLockTime )
		{
			//���� �ð��� ��´�.
			if ( m_bHardWare )
				QueryPerformanceCounter( (LARGE_INTEGER*) &m_nCurTime );
			else
				m_nCurTime = timeGetTime();

			//���� �ð����� �ݸ� ���
			m_nDeltaTime = m_nCurTime - m_nLastTime;

			//�������� ������ ���� �ʽð� ���
			m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;
		}

	}

	//���� �ð� ����
	m_nLastTime = m_nCurTime;

	//������ �ð� ����
	m_TotalSec += m_FrameDeltaSec;

	//������ �� ����
	m_FrameCountSec += m_FrameDeltaSec;

	//1�ʰ� �����ٸ�....
	if ( m_FrameCountSec >= 1.0 ) {
		m_FramePerSec = m_FrameCount;
		m_FrameCount = 0;
		m_FrameCountSec -= 1.0;
	}
	m_FrameCount++;


}

//Ÿ�������� ����Ѵ�
void cTimeMgr::DrawTimeInfo()
{
	char szTemp[128];
	wsprintf( szTemp, "FPS : %d", m_FramePerSec );
	DXFONT_MGR->PrintTextOutline(szTemp, 0, 0, 0xffffff00, 0xff000000);

	sprintf_s( szTemp, "FrameDeltaSec : %.2f", m_FrameDeltaSec );
	DXFONT_MGR->PrintTextOutline(szTemp, 0, 20, 0xffffff00, 0xff000000);

	sprintf_s( szTemp, "TotalDeltaSec : %.2f", m_TotalSec );
	DXFONT_MGR->PrintTextOutline(szTemp, 0, 40, 0xffffff00, 0xff000000);
}

//�׸��� Text
void cTimeMgr::DrawShadowText( HDC hdc, int x, int y, char* szText, COLORREF textColor )
{
	//Text ����� �������� 
	SetBkMode( hdc, TRANSPARENT );

	SetTextColor( hdc, 0x00000000 );
	TextOut( hdc, x + 1, y + 1, szText, strlen( szText ) );

	SetTextColor( hdc, textColor );
	TextOut( hdc, x, y, szText, strlen( szText ) );

	SetTextColor( hdc, 0x00000000 );

	//Text ����������
	SetBkMode( hdc, OPAQUE );
}