#pragma once

namespace MyUtil 
{
	extern LPDIRECT3DDEVICE9 Device;			//전역 Device 요주의 인물 ( 진짜중요함 예로 다함 예없음 안됨, 그의 파워는 HDC 와는 비교도 안됨 )

	HRESULT CreateDevice();						//디바이스 생성함수
	void ReleaseDevice();						//디바이스 해제함수

}