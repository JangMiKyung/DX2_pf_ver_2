#include "stdafx.h"
#include "cBoundSphere.h"


cBoundSphere::cBoundSphere()
{
}


cBoundSphere::~cBoundSphere()
{
}

//���� ������ ���Ϳ� �������� ��´�.
void cBoundSphere::GetWorldCenterRadius(const cTransform& cTrans, D3DXVECTOR3* center, float* radius)
{
	D3DXMATRIXA16 matFinal = cTrans.GetFinalMatrix();

	//���� ����
	D3DXVec3TransformCoord(center, &this->localCenter, &matFinal);

	D3DXVECTOR3 scale = cTrans.GetScale();

	//���� ����
	D3DXVECTOR3 halfSize;
	halfSize.x = this->halfSize.x * scale.x;
	halfSize.y = this->halfSize.y * scale.y;
	halfSize.z = this->halfSize.z * scale.z;

	*radius = D3DXVec3Length(&halfSize);
}

//���� ������ �׷�����.
void cBoundSphere::RenderGizmo(const cTransform& trans)
{
	D3DXVECTOR3 worldCenter;
	float radius;

	this->GetWorldCenterRadius(trans, &worldCenter, &radius);
	GIZMO_MGR->WireSphere(worldCenter, radius, 0xffffff00);
}