#pragma once

class cTransform;

class cBoundSphere
{
public:
	D3DXVECTOR3 localCenter;
	float radius;
	D3DXVECTOR3 halfSize;

public:
	cBoundSphere();
	~cBoundSphere();

	//월드 단위의 센터와 반지름을 얻는다.
	void GetWorldCenterRadius(const cTransform* cTrans, D3DXVECTOR3* center, float* radius);

	//월드 단위로 그려진다.
	virtual void RenderGizmo(const cTransform* trans);

	virtual void SetBound(const D3DXVECTOR3* pCenter, const D3DXVECTOR3* pHalfSize);
};

