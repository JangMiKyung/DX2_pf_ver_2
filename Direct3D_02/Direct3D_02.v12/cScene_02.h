#pragma once

#include "cScene.h"

class cScene_02 : public cScene
{
public:
	cScene_02();
	~cScene_02();

	void Init();
	void Release();
	void Update(float timeDelta);
	void Render();
};

