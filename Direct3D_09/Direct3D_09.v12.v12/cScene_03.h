#pragma once
#include "cScene.h"

class cScene_03 : public cScene
{
private:

	LPD3DXMESH						pMesh;

	cTransform						meshTrans;
	cCamera							mainCamera;
	cTransform						lightTrans;

	//
	// Effect ���� ����
	//
	LPD3DXEFFECT					pEffect;			//�ε��� ���̴�

public:
	cScene_03();
	~cScene_03();

	virtual HRESULT Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();
};

