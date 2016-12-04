#pragma once
#include "cScene.h"
class cScene_04 : public cScene
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
	cScene_04();
	~cScene_04();

	virtual HRESULT Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();
};

