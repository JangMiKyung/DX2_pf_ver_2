#pragma once
#include "cScene.h"
class cScene_03 : public cScene
{
private:
	cCamera			mainCamera;

	D3DLIGHT9		spotlight;				//광원 정보
	cTransform		spotlightTrans;			//광원 Transform

	D3DLIGHT9		pointlight;				//광원 정보
	cTransform		pointlightTrans;		//광원 Transform


	LPD3DXMESH		pMesh;				//큐브 메쉬
	cTransform		transMesh;			//큐브 Transform
	D3DMATERIAL9	material;			//큐브 재질

public:
	cScene_03();
	~cScene_03();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();
};

