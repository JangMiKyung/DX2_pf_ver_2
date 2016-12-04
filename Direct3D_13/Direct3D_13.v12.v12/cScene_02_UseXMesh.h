#pragma once

#include "cScene.h"
#include "cXMesh_Static.h"

class cScene_02_UseXMesh : public cScene
{
private:
	cXMesh_Static*			pMesh;
	cTransform				trans;
	std::vector<cLight*>	lights;

public:
	cScene_02_UseXMesh();
	~cScene_02_UseXMesh();

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timeDelta);

	virtual void Scene_Render1();
};

