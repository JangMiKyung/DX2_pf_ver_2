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
	// Effect 관련 변수
	//
	LPD3DXEFFECT					pEffect;			//로딩된 셰이더

public:
	cScene_03();
	~cScene_03();

	virtual HRESULT Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();
};

