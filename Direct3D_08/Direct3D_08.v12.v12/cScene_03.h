#pragma once
#include "cScene.h"
class cScene_03 : public cScene
{
private:
	cCamera			mainCamera;

	D3DLIGHT9		spotlight;				//���� ����
	cTransform		spotlightTrans;			//���� Transform

	D3DLIGHT9		pointlight;				//���� ����
	cTransform		pointlightTrans;		//���� Transform


	LPD3DXMESH		pMesh;				//ť�� �޽�
	cTransform		transMesh;			//ť�� Transform
	D3DMATERIAL9	material;			//ť�� ����

public:
	cScene_03();
	~cScene_03();

	virtual void Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();
};

