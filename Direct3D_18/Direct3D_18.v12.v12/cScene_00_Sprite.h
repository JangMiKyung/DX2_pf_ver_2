#pragma once

#include "cScene.h"

class cXMesh_Skinned;
class cSkinnedAnimation;
class cBoundBox;
class cLight;

class cScene_00_Sprite : public cScene
{
private:

	cSkinnedAnimation*		pSkinned1;
	cSkinnedAnimation*		pSkinned2;
	cTransform*				pSkinnedTrans;
	cTransform*				pSkinnedTrans2;

	cTransform*				pBoxTrans1;
	cBoundBox*				pBox1;
	cTransform*				pBoxTrans2;


	cTransform*				pApplyTrans;

	std::vector<cLight*>	lights;

	LPDIRECT3DTEXTURE9		pTex;

public:
	cScene_00_Sprite();
	~cScene_00_Sprite();


	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();


	virtual void Scene_RenderSprite();
};

