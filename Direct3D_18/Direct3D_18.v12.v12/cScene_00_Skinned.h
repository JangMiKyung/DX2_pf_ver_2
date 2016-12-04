#pragma once

#include "cScene.h"

class cXMesh_Skinned;
class cSkinnedAnimation;
class cBoundBox;
class cLight;
class cCamera;

class cScene_00_Skinned : public cScene
{
private:

	cSkinnedAnimation*		pSkinnedAni1;
	cSkinnedAnimation*		pSkinnedAni2;
	cTransform*				pSkinnedTrans;
	cTransform*				pSkinnedTrans2;


	cTransform*				pBoxTrans1;
	cBoundBox*				pBox1;
	cTransform*				pBoxTrans2;


	cTransform*				pApplyTrans;

	std::vector<cLight*>	lights;

	std::vector<cCamera*>	cameras;
	cCamera*				mainCamera;
	D3DXVECTOR3				headPos;

public:
	cScene_00_Skinned();
	~cScene_00_Skinned();

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();
};

