#pragma once

#include "cScene.h"
#include "cPlayerPlane.h"
#include "cEnemyPlane.h"

class cScene_02_BoundBlocking : public cScene
{
private:
	cBaseObject				objects;
	cBaseObject				object2;
	cBaseObject				object3;
	std::vector<cLight*>	lights;
	bool					bHit;

public:
	cScene_02_BoundBlocking();
	~cScene_02_BoundBlocking();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
};

