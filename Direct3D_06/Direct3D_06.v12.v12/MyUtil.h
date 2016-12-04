#pragma once

#define EPSILON 0.00001f

//a 와 b가 같냐
#define FLOATEQUAL(a, b) ( abs((a) - (b)) < EPSILON )

//거의 0 이니?
#define FLOATZERO(a) ( abs((a)) < EPSILON )

//제로 벡터니?
#define VECTORZERO(v)  (FLOATZERO((v).x) && FLOATZERO((v).y) && FLOATZERO((v).z))

namespace MyUtil 
{
	extern LPDIRECT3DDEVICE9 Device;			//전역 Device 요주의 인물 ( 진짜중요함 예로 다함 예없음 안됨, 그의 파워는 HDC 와는 비교도 안됨 )

	HRESULT CreateDevice();						//디바이스 생성함수
	void ReleaseDevice();						//디바이스 해제함수

	POINT GetMousePos();
	void SetMousePos(int x, int y);

	float Clamp(float value, float min, float max); //value 값을 min max 사이로 맞춰줌
	float Clamp01(float value);				//value 값을 0~1 사이로 맞춰줌.
	float ClampMinusOnePlusOne(float value);//value 값을 -1 ~ 1 사이로 맞춰줌.

	//float 선형 보간
	float Lerp(float from, float to, float t);

	//벡터 선형 보간
	D3DXVECTOR3 VecLerp(const D3DXVECTOR3& from, const D3DXVECTOR3& to, float t);
}