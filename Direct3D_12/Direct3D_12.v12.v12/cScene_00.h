#pragma once
#include "cScene.h"

class cTerrain;

class cScene_00 : public cScene
{
private:
	LPD3DXMESH			pMesh;
	cTransform			meshTrans;


public:
	cScene_00(void);
	~cScene_00(void);

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();

private:


};

