#pragma once

#include "cScene.h"

class cScene_05 : public cScene
{
	//정점하나에 대한 정보를 정의하는 구조체
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//정점의 위치
		DWORD		color;			//정점의 컬러

		//현제 정점의 정보를 나타내는 플래그 상수값 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ 정점의 위치정보 | D3DFVF_DIFFUSE 정점의 컬러정보
	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9			vb;			//정점을 보관할 정점 버퍼
	LPDIRECT3DINDEXBUFFER9			ib;			//정점 인덱스정보를 보관할 인덱스 버퍼

	D3DXMATRIXA16					matWorld;	//월드 행렬
	float							yaw;

public:
	cScene_05();
	~cScene_05();

	void Init();
	void Release();
	void Update(float timeDelta);
	void Render();
};

