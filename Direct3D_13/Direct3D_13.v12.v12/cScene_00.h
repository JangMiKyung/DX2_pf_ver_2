#pragma once
#include "cScene.h"

class cTerrain;

class cScene_00 : public cScene
{
private:
	LPD3DXMESH							pMesh;
	cTransform							meshTrans;
	cTransform							meshLocal;
	DWORD								dwMaterialsNum;		//로딩된 매쉬의 재질 갯수
	std::vector<LPDIRECT3DTEXTURE9>		vecTextures;			//로딩된 메쉬에서 사용하는 Texture
	std::vector<D3DMATERIAL9>			vecMaterials;			//로딩된 메쉬에서 사용하는 매터리얼...


public:
	cScene_00(void);
	~cScene_00(void);

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();

private:


};

