#pragma once

#include "cScene.h"

class cSetBoundObject;
class cBaseObject;
class cLight;

class cScene_00_BoundSet : public cScene
{
	cBaseObject*			houseObject;
	std::vector<cLight*>	lights;

	std::vector<cSetBoundObject*>	boundObjects;
	cSetBoundObject*				selectBound;

public:
	cScene_00_BoundSet();
	~cScene_00_BoundSet();

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();


	void SaveBoundsInfo();
};

