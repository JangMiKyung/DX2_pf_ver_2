#pragma once

#include "cSingletonBase.h"
#include "cBoundBox.h"
#include "cBaseObject.h"

class cPhysicManager : public cSingletonBase<cPhysicManager>
{
public:
	cPhysicManager();
	~cPhysicManager();

	//���̰� �ٿ�忡 �浹�ߴ��� Ȯ��
	bool IsRayHitBound(
		LPRay Ray,				//����
		cBoundSphere* pBound,	//�ٿ��
		cTransform* pBoundTrans,  //�ٿ���� Transform
		D3DXVECTOR3* pHitPos,     //Hit ��ġ ( NULL �̸� ���� �ȵ� )
		D3DXVECTOR3* pHitNormal	  //Hit �� �븻 ( NULL �̸� ���� �ȵ� )
		);

	//���̰� �ٿ�忡 �浹�ߴ��� Ȯ��
	bool IsRayHitBound(
		LPRay Ray,				//����
		cBoundBox* pBound,		//�ٿ��
		cTransform* pBoundTrans,//�ٿ���� Transform
		D3DXVECTOR3* pHitPos,   //Hit ��ġ ( NULL �̸� ���� �ȵ� )
		D3DXVECTOR3* pHitNormal	//Hit �� �븻 ( NULL �̸� ���� �ȵ� )
		);

	//���̰� ������Ʈ�� �浹�ߴ���....
	bool IsRayHitStaticMeshObject(
		LPRay Ray,				//����
		cBaseObject* pObject,	//Base Object
		D3DXVECTOR3* pHitPos,     //Hit ��ġ ( NULL �̸� ���� �ȵ� )
		D3DXVECTOR3* pHitNormal	  //Hit �� �븻 ( NULL �̸� ���� �ȵ� )
		);

	bool IsOverlap(cBaseObject* pObjA, cBaseObject* pObjB);

	//2���� �ٿ�� ���� �⵿ ������ ��´�.
	bool IsOverlap(cTransform* pTransA, cBoundSphere* pBoundA, cTransform* pTransB, cBoundSphere* pBoundB);
	bool IsOverlap(cTransform* pTransA, cBoundBox* pBoundA, cTransform* pTransB, cBoundBox* pBoundB);

	bool IsOverlap(cTransform* pTransA, cBoundSphere* pBoundA, cTransform* pTransB, cBoundBox* pBoundB);
	bool IsOverlap(cTransform* pTransA, cBoundBox* pBoundA, cTransform* pTransB, cBoundSphere* pBoundB);

	bool IsBlocking(cBaseObject* pObjA, cBaseObject* pObjB, float moveFactor = 0.0f);

	//2���� �ٿ�� ���Ǿ���� �⵿�ϰ� ��ġ�� �ʰ� �Ѵ�.
	//moveFactor �� 0 �ϼ��� ��Ĩ�� ���� B �� �����δ�.
	//moveFactor �� 0.5 �̸� ��Ĩ�� ���� A �� B �� �Ȱ��������� �����δ�.
	//moveFactor �� 1 �ϼ��� ��Ĩ�� ���� A �� �����δ�.
	bool IsBlocking(
		cTransform* pTransA, cBoundSphere* pBoundA,
		cTransform* pTransB, cBoundSphere* pBoundB,
		float moveFactor = 0.0f);


	bool IsBlocking(
		cTransform* pTransA, cBoundBox* pBoundA,
		cTransform* pTransB, cBoundBox* pBoundB,
		float moveFactor = 0.0f);

	//�� 3���� ���� ����� �����.
	void CreatePlane(LPD3DXPLANE	pOutPlane, const D3DXVECTOR3* p0, const D3DXVECTOR3* p1, const D3DXVECTOR3* p2);

	//������ �������� �������� �ִܰŸ�
	float PlaneDot(const LPD3DXPLANE pPlane, const D3DXVECTOR3* point);

	//�������� ����� �浹 ��ġ
	bool IntersectRayToPlane(D3DXVECTOR3* pOut, const LPRay pRay, const LPD3DXPLANE pPlane, bool bCheck2Side = false);



};

#define PHYSICS_MGR cPhysicManager::GetInstance()