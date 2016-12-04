#pragma once

#include "cScene.h"

class cLight;
class cXMesh_Static;
class Unit;
class MoveMap;
class cNode;

class cScene_Test : public cScene
{
private:

   
	MoveMap* NodeList;
	cNode* CurDest;
	Unit* Pig;
	Unit* Miles;
	std::vector<cLight*> lights;

	cSkinnedAnimation*		pSkinned1;
	cTransform*				pSkinnedTrans;

	cBaseObject* house;
	cBaseObject* air;
	D3DXMATRIXA16 matWorld;
public:
	cScene_Test();
	~cScene_Test();
	
	virtual HRESULT Scene_Init();
	virtual void Scene_Release();
	virtual void Scene_Update(float timeDelta);
	virtual void Scene_Render1();
};

