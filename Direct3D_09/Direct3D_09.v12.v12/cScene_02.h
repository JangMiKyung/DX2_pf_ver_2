#pragma once

#include "cScene.h"

class cScene_02 : public cScene
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
	cScene_02();
	~cScene_02();

	virtual HRESULT Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();
};

