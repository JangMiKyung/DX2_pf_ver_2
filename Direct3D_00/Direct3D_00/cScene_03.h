#pragma once

#include "cScene.h"

class cScene_03 : public cScene
{
private:

	//정점하나에 대한 정보를 정의하는 구조체
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//정점의 위치
		DWORD		color;			//정점의 컬러

		//현제 정점의 정보를 나타내는 플래그 상수값 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ 정점의 위치정보 | D3DFVF_DIFFUSE 정점의 컬러정보
	}MYVERTEX, *LPMYVERTEX;


	MYVERTEX		vertex[4];
	DWORD			index[6];
	float yaw;
	D3DXMATRIXA16	matWorld;

public:
	cScene_03();
	~cScene_03();

	void Init();
	void Release();
	void Update(float timeDelta);
	void Render();
};

