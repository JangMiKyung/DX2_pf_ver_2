#pragma once
#include "cScene.h"
class cScene_01 : public cScene
{
private:
	//정점하나에 대한 정보를 정의하는 구조체
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//정점의 위치
		DWORD		color;			//정점의 컬러

		//현제 정점의 정보를 나타내는 플래그 상수값 
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };	//D3DFVF_XYZ 정점의 위치정보 | D3DFVF_DIFFUSE 정점의 컬러정보
	}MYVERTEX, *LPMYVERTEX;


	LPDIRECT3DVERTEXBUFFER9			vb;				//정점을 보관할 정점 버퍼
	LPDIRECT3DINDEXBUFFER9			ib;				//정점 인덱스정보를 보관할 인덱스 버퍼

	cTransform						cubeTrans;		//큐브의 Transform
	cTransform						childCubeTrans;			//큐브의 자식 Transform
	cTransform						grandChildCubeTrans;	//큐브의 자식 Transform

	cCamera							mainCamera;

public:
	cScene_01();
	~cScene_01();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateCube();

	void RenderCube();
};

