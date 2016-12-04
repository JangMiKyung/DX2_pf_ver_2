#include "stdafx.h"
#include "cScene_01.h"


cScene_01::cScene_01()
{
}


cScene_01::~cScene_01()
{
}

void cScene_01::Init()
{
	//폰트 객체 만든다
	D3DXCreateFont(
		Device,				//디바이스
		30.f,				//폰트의 높이
		0.f,				//폰트의 폭(이 값이 0이면 앞에서 설정한 폰트 높이에 비례)
		FW_BOLD,			//폰트의 두께 설정(FW_BOLD 폰트의 BOLD 스타일 크기를 갖는다)
		1,					//밉맵(그냥 1)
		FALSE,				//이텔릭 여부
		DEFAULT_CHARSET,	//캐릭터 set( 그냥 DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//외각 처리방식 ( 그냥 OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//퀄리티 ( 그냥 DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// 글씨체 패밀리셋 설정
		"궁서체",			//사용될 폰트 이름 
		&pDxFont			//생성된 폰트객체 받을 더블 포인터
		);

}

void cScene_01::Release()
{
	SAFE_RELEASE(pDxFont);
}

void cScene_01::Update(float timeDelta)
{

}

void cScene_01::Render()
{
	//출력 영역 (윈도우 좌표계)
	RECT rcText2 = { 101, 101, 120, 120 };

	pDxFont->DrawText(
		NULL,				//스프라이트에 그리는 경우 스프라이트 객체
		"정신차리자",			//들어갈 문자열
		-1,					//출력 문자열 갯수 -1이면 다 찍어라
		&rcText2,			//출력 영역
		DT_LEFT | DT_NOCLIP,//왼쪽정렬, DT_NOCLIP하면 영역을 무시하고 다 출력
		0xff000000			//문자열 색상
		);

	RECT rcText1 = { WINSIZE_X / 2, 0, WINSIZE_X, 50 };

	pDxFont->DrawText(
		NULL,
		"모기가 너무 많다",
		-1,
		&rcText1,
		DT_RIGHT | DT_NOCLIP,
		0xffffff00
		);

	RECT rcText3 = { 50, 0, WINSIZE_X - 50, WINSIZE_Y };

	pDxFont->DrawText(
		NULL,
		"쉬는 시간을 갖자",
		-1,
		&rcText3,
		DT_CENTER | DT_NOCLIP | DT_VCENTER,
		0xffffffff
		);


}