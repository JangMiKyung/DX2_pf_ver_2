#pragma once

#define EPSILON 0.00001f

//a 와 b가 같냐
#define FLOATEQUAL(a, b) ( abs((a) - (b)) < EPSILON )

//거의 0 이니?
#define FLOATZERO(a) ( abs((a)) < EPSILON )

//제로 벡터니?
#define VECTORZERO(v)  (FLOATZERO((v).x) && FLOATZERO((v).y) && FLOATZERO((v).z))

#define RS(a, b)		Device->SetRenderState((a), (b))

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
	//( t 는 0 ~ 1 사이의 값을 받는데...)
	//( t 가 0 일수록 from 가까운 값이 리턴되고 )
	//( t 가 1 일수록 to 가까운 값이 리턴된다 )
	float Lerp(float from, float to, float t);


	//float 선형 보간 반대
	//value 값이 head 와 rear 사이에 어느정도 위치에 있는지
	//0~1 사이의 값으로 리턴한다.
	float InverseLerp(float head, float rear, float value);

	//벡터 선형 보간
	D3DXVECTOR3 VecLerp(const D3DXVECTOR3& from, const D3DXVECTOR3& to, float t);


	//Random
	void SetRandomSeed();
	float RandomFloatRange(float min, float max);
	int RandomIntRange(int min, int max);



	//정점의 노말을 계산해주는 함수
	void ComputeNormal(
		D3DXVECTOR3* pNorams,			//결과로 받을 노말백터 배열 포인터 ( 노말벡터의 배열의 수는 정점 배열의 수와 같다 )
		const D3DXVECTOR3* pVertices,	//계산에 사용된 정점위치 배열 포인터
		int verticesNum,				//정점 갯수
		const DWORD* pIndices,			//인덱스 배열포인터 ( 일단 인덱스는 DWORD 로 대동단결 )
		int indicesNum					//인덱스 배열포인터 갯수
		);
}