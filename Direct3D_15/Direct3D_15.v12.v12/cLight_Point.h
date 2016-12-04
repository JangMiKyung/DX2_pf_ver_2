#pragma once
#include "cLight.h"
class cLight_Point : public cLight
{
public:
	float				minRange;			//���� �ּҰŸ� 
	float				maxRange;			//���� �ִ�Ÿ�
	float				distancePow;		//�Ÿ� ������

public:
	cLight_Point();
	~cLight_Point();

	virtual D3DXMATRIXA16 GetLightMatrix()override;

	virtual void RenderGizmo()override;
};

