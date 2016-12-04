#pragma once

#include "cScene.h"

#include "cPlayerPlane.h"
#include "cEnemyPlane.h"

class cScene_01_ShowBound : public cScene
{
private:
	cBaseObject				objects;
	cBaseObject				object2;
	std::vector<cLight*>	lights;
	bool					bHit;

public:
	cScene_01_ShowBound();
	~cScene_01_ShowBound();

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();
};

