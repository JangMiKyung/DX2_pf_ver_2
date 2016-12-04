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


	LPDIRECT3DVERTEXBUFFER9			vb;			//정점을 보관할 정점 버퍼
	LPDIRECT3DINDEXBUFFER9			ib;			//정점 인덱스정보를 보관할 인덱스 버퍼
	D3DXMATRIXA16					matWorld;	//월드 행렬

	//사물의 축 ( 모든 축은 방향에 따라 무조건 직교한다 )
	D3DXVECTOR3						forward;	//사물의 정면 벡터
	D3DXVECTOR3						right;		//사물의 오른쪽 벡터
	D3DXVECTOR3						up;			//사물의 상방벡터
	D3DXVECTOR3						pos;		//사물의 위치


	//바라보는 위치
	D3DXVECTOR3						tagetPos;		//타겟 위치

public:
	cScene_03();
	~cScene_03();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateCube();

	void RotateSelfYaw(float angle);
	void RotateSelfPitch(float angle);
	void RotateSelfRoll(float angle);

	void RotateWorldYaw(float angle);
	void RotateWorldPitch(float angle);
	void RotateWorldRoll(float angle);
};

