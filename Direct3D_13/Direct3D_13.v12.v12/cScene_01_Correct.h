#pragma once
#include "cScene.h"
class cScene_01_Correct : public cScene
{
private:
	LPD3DXMESH							pMesh;
	cTransform							meshTrans;
	DWORD								dwMaterialsNum;		//�ε��� �Ž��� ���� ����
	std::vector<LPDIRECT3DTEXTURE9>		vecTextures;			//�ε��� �޽����� ����ϴ� Texture
	std::vector<D3DMATERIAL9>			vecMaterials;			//�ε��� �޽����� ����ϴ� ���͸���...

public:
	cScene_01_Correct();
	~cScene_01_Correct();

	virtual HRESULT Scene_Init();

	virtual void Scene_Release();

	virtual void Scene_Update(float timDelta);

	virtual void Scene_Render1();
};

