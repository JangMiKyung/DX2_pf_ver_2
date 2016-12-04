#pragma once
#include "cScene.h"
class cScene_00 : public cScene
{
private:
	//정점하나에 대한 정보를 정의하는 구조체
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//정점의 위치
		D3DXVECTOR3 normal;			//정점의 노말벡터 ( 정점 표면의 바라보는 방향을 얻는다 )
		//현제 정점의 정보를 나타내는 플래그 상수값 
		enum { FVF = D3DFVF_XYZ | D3DFVF_NORMAL };	//D3DFVF_XYZ 정점의 위치정보 | D3DFVF_NORMAL 정점의 노말벡터

	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9			vb;				//정점을 보관할 정점 버퍼
	LPDIRECT3DINDEXBUFFER9			ib;				//정점 인덱스정보를 보관할 인덱스 버퍼

	cTransform						cubeTrans;
	cCamera							mainCamera;

	//
	// Effect 관련 변수
	//
	LPD3DXEFFECT					pEffect;			//로딩된 셰이더
	D3DXHANDLE						hTechnique;			//테크닉 핸들
	D3DXHANDLE						hMatWorld;			//월드 행렬 핸들
	D3DXHANDLE						hMatView;			//뷰행렬 핸들
	D3DXHANDLE						hMatProjection;		//투영 행렬 핸들
	D3DXHANDLE						hVColor;			//컬러 정보 핸들

public:
	cScene_00();
	~cScene_00();

	virtual HRESULT Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateCube();

	void RenderCube();
};

