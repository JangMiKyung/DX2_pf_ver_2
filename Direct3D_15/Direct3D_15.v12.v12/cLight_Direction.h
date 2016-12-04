#pragma once
#include "cLight.h"
class cLight_Direction : public cLight
{
public:
	cLight_Direction();
	~cLight_Direction();

	virtual D3DXMATRIXA16 GetLightMatrix() override;
	virtual void RenderGizmo() override;
};

