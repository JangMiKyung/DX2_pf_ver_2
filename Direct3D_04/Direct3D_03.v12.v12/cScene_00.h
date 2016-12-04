#pragma once

#include "cScene.h"

class cScene_00 : public cScene
{
public:
	cScene_00();
	~cScene_00();

	virtual void Init();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();
};

