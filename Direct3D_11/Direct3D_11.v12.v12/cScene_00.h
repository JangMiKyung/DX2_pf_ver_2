#pragma once
#include "cScene.h"

class cTerrain;

class cScene_00 : public cScene
{
private:

	cCamera							m_mainCamera;
	cTerrain*						m_pTerrain;


public:
	cScene_00(void);
	~cScene_00(void);

	virtual HRESULT Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:


};

