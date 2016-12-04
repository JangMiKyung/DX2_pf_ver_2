#pragma once

#include "cScene.h"

class cSetBoundObject;
class cBaseObject;
class cLight;

class cScene_01_BoundLoad : public cScene
{
	cBaseObject*				houseObject;
	std::vector<cLight*>		lights;
	std::vector<cBaseObject*>	boundObjects;

	cTransform*					pTrans;

public:
	cScene_01_BoundLoad();
	~cScene_01_BoundLoad();

	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();

	void LoadBound();
};

