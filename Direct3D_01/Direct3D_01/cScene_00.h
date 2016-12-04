#pragma once

#include "cScene.h"

class cScene_00 : public cScene
{
private:
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//정점의 위치
		DWORD		color;			//정점의 컬러

		//현제 정점의 정보를 나타내는 플래그 상수값 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ 정점의 위치정보 | D3DFVF_DIFFUSE 정점의 컬러정보
	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9 planeVB; //비행기 정점 버퍼

	float yawSpeed;
	float yawAngle;
	float moveSpeed;
	D3DXVECTOR3 pos;
	D3DXMATRIXA16 matPlane;

	D3DXVECTOR3 pos2;
	D3DXMATRIXA16 matPlane2;


public:
	cScene_00();
	virtual ~cScene_00();

	virtual void Init();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();
};

