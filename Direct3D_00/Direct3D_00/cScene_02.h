#pragma once

#include "cScene.h"

class cScene_02 : public cScene
{
private:
	typedef struct tagMYVERTEX
	{
		D3DXVECTOR3 pos;
		DWORD		color;

		enum{ FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	}MYVERTEX, *LPMYVERTEX;

	//정점 데이터를 보관할 버텍스버퍼 포인터
	LPDIRECT3DVERTEXBUFFER9 vb;
	float yaw; //y축 회전
	D3DXMATRIXA16 matWorld;//월드 행렬

public:
	cScene_02();
	~cScene_02();

	virtual void Init();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();
};

