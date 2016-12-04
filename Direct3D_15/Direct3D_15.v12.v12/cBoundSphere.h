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

	//���� ������ ���Ϳ� �������� ��´�.
	void GetWorldCenterRadius(const cTransform* cTrans, D3DXVECTOR3* center, float* radius);

	//���� ������ �׷�����.
	virtual void RenderGizmo(const cTransform* trans);

	virtual void SetBound(const D3DXVECTOR3* pCenter, const D3DXVECTOR3* pHalfSize);
};

