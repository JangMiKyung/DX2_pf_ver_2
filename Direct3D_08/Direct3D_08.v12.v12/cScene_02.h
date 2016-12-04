#pragma once

#include "cScene.h"
class cScene_02 : public cScene
{
private:
	cCamera				mainCamera;

	D3DLIGHT9			light;				//광원 정보
	cTransform			lightTrans;			//광원 Transform


	LPD3DXMESH			pMeshCube;			//큐브 메쉬
	cTransform			transCube;			//큐브 Transform
	D3DMATERIAL9		cubeMaterial;		//큐브 재질

	LPD3DXMESH			pMeshSphere;		//구 메쉬
	cTransform			transSphere;		//구 Transform
	D3DMATERIAL9		SphereMaterial;		//구 재질

	LPD3DXMESH			pMeshTeapot;		//주전자 메쉬
	cTransform			transTeapot;		//주전자 Transform
	D3DMATERIAL9		teapotMaterial;		//주전자 재질

	LPD3DXMESH			pMeshTorus;			//도넛 메쉬
	cTransform			transTorus;			//도넛 Transform
	D3DMATERIAL9		torusMaterial;		//도넛 재질

public:
	cScene_02();
	~cScene_02();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();
};

