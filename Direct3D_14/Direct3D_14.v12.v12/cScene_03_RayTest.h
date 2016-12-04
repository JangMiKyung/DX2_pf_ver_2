#pragma once

#include "cScene.h"
#include "cPlayerPlane.h"
#include "cEnemyPlane.h"

class cScene_03_RayTest : public cScene
{
private:
	cBaseObject				objects;
	cTransform				RayTrans;
	std::vector<cLight*>	lights;
	bool					bHit;
	D3DXVECTOR3				hitPos;
	D3DXVECTOR3				hitNormal;

	Ray						camRay;

public:
	cScene_03_RayTest();
	~cScene_03_RayTest();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
};

