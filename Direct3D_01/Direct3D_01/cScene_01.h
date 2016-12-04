#pragma once

#include "cScene.h"

class cScene_01 : public cScene
{
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//정점의 위치
		DWORD		color;			//정점의 컬러

		//현제 정점의 정보를 나타내는 플래그 상수값 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ 정점의 위치정보 | D3DFVF_DIFFUSE 정점의 컬러정보
	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9				planeVB;			//비행기 정점 버퍼]

	float								yawSpeed;			//초당 수평 회전 속도
	float								yawAngle;			//수평 각
	float								moveSpeed;			//초당 이동량
	D3DXVECTOR3							pos;				//비행기 위치
	D3DXMATRIXA16						matPlane;			//비행기 행렬


	//자식 비행기 정보
	D3DXVECTOR3							localPos;			//부모기준으로 어디에 있니?
	D3DXMATRIXA16						matChildPlane;		//자식비행기 행렬

public:
	cScene_01();
	~cScene_01();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

};

