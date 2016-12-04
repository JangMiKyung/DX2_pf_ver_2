#pragma once
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
	void GetWorldCenterRadius(const cTransform& cTrans, D3DXVECTOR3* center, float* radius);

	//���� ������ �׷�����.
	virtual void RenderGizmo(const cTransform& trans);
};

