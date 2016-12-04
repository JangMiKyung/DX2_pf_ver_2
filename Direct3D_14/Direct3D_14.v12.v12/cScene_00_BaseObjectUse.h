#pragma once

#include "cScene.h"
#include "cPlayerPlane.h"
#include "cEnemyPlane.h"

class cScene_00_BaseObjectUse : public cScene
{
private:
	cPlayerPlane				plane;
	std::vector<cLight*>		lights;
	std::vector<cEnemyPlane*>	enemyPlanes;

public:
	cScene_00_BaseObjectUse();
	~cScene_00_BaseObjectUse();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
};

