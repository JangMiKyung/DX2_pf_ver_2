#pragma once

#include "cScene.h"
class cScene_02 : public cScene
{
private:
	cCamera				mainCamera;

	D3DLIGHT9			light;				//���� ����
	cTransform			lightTrans;			//���� Transform


	LPD3DXMESH			pMeshCube;			//ť�� �޽�
	cTransform			transCube;			//ť�� Transform
	D3DMATERIAL9		cubeMaterial;		//ť�� ����

	LPD3DXMESH			pMeshSphere;		//�� �޽�
	cTransform			transSphere;		//�� Transform
	D3DMATERIAL9		SphereMaterial;		//�� ����

	LPD3DXMESH			pMeshTeapot;		//������ �޽�
	cTransform			transTeapot;		//������ Transform
	D3DMATERIAL9		teapotMaterial;		//������ ����

	LPD3DXMESH			pMeshTorus;			//���� �޽�
	cTransform			transTorus;			//���� Transform
	D3DMATERIAL9		torusMaterial;		//���� ����

public:
	cScene_02();
	~cScene_02();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();
};

