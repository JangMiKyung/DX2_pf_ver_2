#pragma once

#include "cScene.h"

class cScene_01 : public cScene
{
private:
	LPD3DXFONT pDxFont; //Direct3D ÆùÆ® °´Ã¼

public:
	cScene_01();
	~cScene_01();

	virtual void Init();
	virtual void Release();
	virtual void Update(float timeDelta);
	virtual void Render();
};

