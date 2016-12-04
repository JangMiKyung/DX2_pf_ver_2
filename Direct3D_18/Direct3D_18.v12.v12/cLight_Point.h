#pragma once
#include "cLight.h"
class cLight_Point : public cLight
{
public:
	float				minRange;			//빛의 최소거리 
	float				maxRange;			//빛의 최대거리
	float				distancePow;		//거리 감쇠율

public:
	cLight_Point();
	~cLight_Point();

	virtual D3DXMATRIXA16 GetLightMatrix()override;

	virtual void RenderGizmo()override;
};

