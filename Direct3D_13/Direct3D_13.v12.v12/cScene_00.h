#pragma once
#include "cScene.h"

class cTerrain;

class cScene_00 : public cScene
{
private:
	LPD3DXMESH							pMesh;
	cTransform							meshTrans;
	cTransform							meshLocal;
	DWORD								dwMaterialsNum;		//�ε��� �Ž��� ���� ����
	std::vector<LPDIRECT3DTEXTURE9>		vecTextures;			//�ε��� �޽����� ����ϴ� Texture
	std::vector<D3DMATERIAL9>			vecMaterials;			//�ε��� �޽����� ����ϴ� ���͸���...


public:
	cScene_00(void);
	~cScene_00(void);

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();

private:


};

