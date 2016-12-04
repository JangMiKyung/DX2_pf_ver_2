#pragma once

#include "cScene.h"

//FVF??

//FVF는 flexible Vertex Format 의 약자로
//D3D 에서 정해져 있는 정점 포맷 중에서 사용자가 
//임의로 선별해 새로운 정점 포맷을 정하는 방법이다.

//1. 포지션 정점의 좌표 x,y,z (float)  : D3DFVF_XYZ
//2. RHW(float)						: D3DFVF_XYZRHW
//3. Blending Weight Data  결합 가중치 (float)	: D3DFVF_XYZB1 ~ D3DFVF_XYZB5
//4. Vertex normal 정점의 법선 벡터(x,y,z)(float) : D3DFVF_NORMAL
//5. Vertex Point Size 정점의 점 크기 (float)    : D3DFVF_PSIZE
//6. Diffuse Color (DWORD)                     : D3DFVF_DIFFUSE
//7. Specular Color (DWORD)                    : D3DFVF_SPECULAR
//8. Texture Coordinate Set 1 (float)          : D3DFVF_TEX0 - D3DFVF_TEX8

class cScene_00 : public cScene
{
private:
	//정점하나에 대한 정보를 정의하는 구조체
	typedef struct tagMYVERTEX {
		D3DXVECTOR3 pos;			//정점의 위치
		DWORD		color;			//정점의 컬러

									//현제 정점의 정보를 나타내는 플래그 상수값 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ 정점의 위치정보 | D3DFVF_DIFFUSE 정점의 컬러정보

													// FVF 의 선언 순서에 따라 구조체 자료형도 순서대로 해주어야 한다.
	}MYVERTEX, *LPMYVERTEX;

	MYVERTEX			vertices[3];			//정점 구조체 3개	


public:
	cScene_00();
	~cScene_00();

	virtual void Init();

	virtual void Release();

	virtual void Update( float timeDelta );

	virtual void Render();
};

