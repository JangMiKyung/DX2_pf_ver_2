#include "stdafx.h"
#include "cPhysicManager.h"
#include "cTransform.h"

cPhysicManager::cPhysicManager()
{
}


cPhysicManager::~cPhysicManager()
{
}

bool cPhysicManager::IsRayHitBound(
	LPRay Ray,
	cBoundSphere* pBound,
	cTransform* pBoundTrans,
	D3DXVECTOR3* pHitPos,
	D3DXVECTOR3* pHitNormal)
{
	//��ġ��
	D3DXMATRIXA16 matWorld = pBoundTrans->GetFinalMatrix();

	//�����ϰ�
	D3DXVECTOR3 scale = pBoundTrans->GetScale();

	//�ٿ�� ���Ǿ� �� ������ ��´�.
	D3DXVECTOR3 center;
	float radius;
	D3DXVECTOR3 halfSize;

	//��ȯ�� ��ġ
	D3DXVec3TransformCoord(&center, &pBound->localCenter, &matWorld);

	//������...
	halfSize.x = pBound->halfSize.x * scale.x;
	halfSize.y = pBound->halfSize.y * scale.y;
	halfSize.z = pBound->halfSize.z * scale.z;
	radius = D3DXVec3Length(&halfSize);

	//������ ���������� ���� ���� ���ͱ����� ���⺤��
	D3DXVECTOR3 dirToCenter = center - Ray->origin;

	//������ ����
	float lengthSq = D3DXVec3LengthSq(&dirToCenter);

	//�������� ����
	float r2 = radius * radius;

	//���� ������ ���ȿ� �ִٸ�...
	if (r2 > lengthSq)
	{
		//������ �ȿ��� ������ ���� üũ �ȵȴ�.
		return false;
	}

	//����������� �������� �� �ۿ� �ִٴ� ����̰�
	//�����ͱ����� ���⺤�Ϳ� ������ ���⺤�Ͱ� ������ ������
	//�а��̶�� �׾��ٱ���� �浹�� ���� ����.
	float dot = D3DXVec3Dot(&dirToCenter, &Ray->direction);

	if (dot <= 0.f)
	{
		return false;
	}

	//��Ÿ����� ������ �ϱ� ���� ���� �ﰢ�� ��������
	//d2 = x2 + y2
	//d = sqrt(x2 + y2);

	float x2 = dot * dot;
	float d2 = lengthSq;

	//d2 - x2 = y2;
	float y2 = d2 - x2;

	//������ �� ���� �����.
	if (y2 > r2)
	{
		return false;
	}

	//������� �Դٸ� �ϴ��� ��Ʈ�� ��Ʈ
	//���� ������ HitPoint �� �ִٸ�..
	if (pHitPos != NULL)
	{
		//d�� radius ����
		d2 = r2;
		//d2 = y2 + x2
		//float x2 = d2 - y2;
		float x = sqrt(d2 - y2);

		*pHitPos = Ray->origin + (Ray->direction * (dot - x));

		//Hit �� ��ġ�� �븻�� ��ڴٸ�...
		if (pHitNormal)
		{
			*pHitNormal = *pHitNormal - center;
			D3DXVec3Normalize(pHitNormal, pHitNormal);
		}
	}

	return true;

}

//���̰� �ٿ�忡 �浹�ߴ��� Ȯ��
bool cPhysicManager::IsRayHitBound(
	LPRay pRay,				   //����
	cBoundBox* pBound,		   //�ٿ��
	cTransform* pBoundTrans,   //�ٿ���� Transform
	D3DXVECTOR3* pHitPos,	   //Hit ��ġ ( NULL �̸� ���� �ȵ� )
	D3DXVECTOR3* pHitNormal	   //Hit �� �븻 ( NULL �̸� ���� �ȵ� )
	)
{
	//�ֽô��� ���� �浹���� �ʾ����� �簢���� �׾��ٱ���� �浹 �ȵ�.
	if (this->IsRayHitBound(pRay,
		(cBoundSphere*)pBound,
		pBoundTrans,
		NULL, NULL) == false)
	{
		return false;
	}

	//������� ������ �簢���� �浹�˻縦 ����

	//���̸� ���÷� �������.
	//bound �� �����
	D3DXMATRIXA16 matWorld = pBoundTrans->GetFinalMatrix();
	D3DXMATRIXA16 matInvMatrix;
	D3DXMatrixInverse(&matInvMatrix, NULL, &matWorld);

	//NewRayInfo
	D3DXVECTOR3 origin;
	D3DXVec3TransformCoord(&origin, &pRay->origin, &matInvMatrix);
	D3DXVECTOR3 direction;
	D3DXVec3TransformNormal(&direction, &pRay->direction, &matInvMatrix);

	Ray newRay;
	newRay.origin = origin;
	newRay.direction = direction;

	//���� 8 �� ����...

	//   5-------6
	//  /|      /|
	// 1-------2 |
	// | 4-----|-7
	// |/      |/
	// 0-------3 

	//���� 8���� ������ ���Ѵ�
	D3DXVECTOR3 Vertices[8];
	Vertices[0] = D3DXVECTOR3(pBound->localMinPos.x, pBound->localMinPos.y, pBound->localMinPos.z);
	Vertices[1] = D3DXVECTOR3(pBound->localMinPos.x, pBound->localMaxPos.y, pBound->localMinPos.z);
	Vertices[2] = D3DXVECTOR3(pBound->localMaxPos.x, pBound->localMaxPos.y, pBound->localMinPos.z);
	Vertices[3] = D3DXVECTOR3(pBound->localMaxPos.x, pBound->localMinPos.y, pBound->localMinPos.z);
	Vertices[4] = D3DXVECTOR3(pBound->localMinPos.x, pBound->localMinPos.y, pBound->localMaxPos.z);
	Vertices[5] = D3DXVECTOR3(pBound->localMinPos.x, pBound->localMaxPos.y, pBound->localMaxPos.z);
	Vertices[6] = D3DXVECTOR3(pBound->localMaxPos.x, pBound->localMaxPos.y, pBound->localMaxPos.z);
	Vertices[7] = D3DXVECTOR3(pBound->localMaxPos.x, pBound->localMinPos.y, pBound->localMaxPos.z);

	D3DXVECTOR3 hit;
	D3DXVECTOR3 min = pBound->localMinPos;
	D3DXVECTOR3 max = pBound->localMaxPos;

	//�޸�
	D3DXPLANE planeBack;
	this->CreatePlane(&planeBack, Vertices + 0, Vertices + 1, Vertices + 2);
	//�޸��� ���̶� �ѵ��ߴ�?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeBack, false))
	{
		//��Ʈ ������ �� �ȿ� �ִ�?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.y <= hit.y && hit.y <= max.y)
		{
			//hit ��������� ���ܼ� ����
			if (pHitPos != NULL)
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			if (pHitNormal != NULL)
				*pHitNormal = -pBoundTrans->GetForward();
		}

		return true;
	}

	//�ո�
	D3DXPLANE planeFront;
	this->CreatePlane(&planeFront, Vertices + 6, Vertices + 5, Vertices + 4);
	//�ո��� ���̶� �浹�߳�?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeFront, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.y <= hit.y && hit.y <= max.y){

			//hit ���� ����� ���ܼ� ����
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}
			if (pHitNormal != NULL){
				*pHitNormal = pBoundTrans->GetForward();
			}

			return true;
		}
	}

	//�ϸ�
	D3DXPLANE planeBottom;
	this->CreatePlane(&planeBottom, Vertices + 3, Vertices + 4, Vertices + 0);
	//�ϸ��� ���̶� �浹�ߴ�?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeBottom, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.z <= hit.z && hit.z <= max.z){

			//hit ��������� ���ܼ� ����
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}

			if (pHitNormal != NULL){
				*pHitNormal = -pBoundTrans->GetUp();
			}

			return true;
		}
	}

	//���
	D3DXPLANE planeTop;
	this->CreatePlane(&planeTop, Vertices + 1, Vertices + 5, Vertices + 2);
	//����� ���̶� �浹�ߴ�?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeTop, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.z <= hit.z && hit.z <= max.z){

			//hit ��������� ���ܼ� ����
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}

			if (pHitNormal != NULL){
				*pHitNormal = pBoundTrans->GetUp();
			}

			return true;
		}
	}

	//�¸�
	D3DXPLANE planeLeft;
	this->CreatePlane(&planeLeft, Vertices + 0, Vertices + 5, Vertices + 1);
	//�¸��� ���̶� �浹�ߴ�?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeLeft, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.y <= hit.y && hit.y <= max.y &&
			min.z <= hit.z && hit.z <= max.z){

			//hit ��������� ���ܼ� ����
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}

			if (pHitNormal != NULL){
				*pHitNormal = -pBoundTrans->GetRight();
			}

			return true;
		}
	}

	//���
	D3DXPLANE planeRight;
	this->CreatePlane(&planeRight, Vertices + 2, Vertices + 6, Vertices + 3);
	//����� ���̶� �浹�ߴ�?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeRight, false))
	{
		//��Ʈ������ �� �ȿ� �ִ�?
		if (min.y <= hit.y && hit.y <= max.y &&
			min.z <= hit.z && hit.z <= max.z){

			//hit ��������� ���ܼ� ����
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}

			if (pHitNormal != NULL){
				*pHitNormal = pBoundTrans->GetRight();
			}

			return true;
		}
	}

	//������� ������ �浹 ����
	return false;

}

//���̰� ������Ʈ�� �浹�ߴ���....
bool cPhysicManager::IsRayHitStaticMeshObject(
	LPRay pRay,				//����
	cBaseObject* pObject,	//Base Object
	D3DXVECTOR3* pHitPos,     //Hit ��ġ ( NULL �̸� ���� �ȵ� )
	D3DXVECTOR3* pHitNormal	  //Hit �� �븻 ( NULL �̸� ���� �ȵ� )
	)
{
	//���̸� ���÷� ������

	//bound �� �����
	D3DXMATRIXA16 matWorld = pObject->pTransform->GetFinalMatrix();
	D3DXMATRIXA16 matInvMatrix;
	D3DXMatrixInverse(&matInvMatrix, NULL, &matWorld);

	//NewRayInfo
	D3DXVECTOR3 origin;
	D3DXVec3TransformCoord(&origin, &pRay->origin, &matInvMatrix);
	D3DXVECTOR3 direction;
	D3DXVec3TransformNormal(&direction, &pRay->direction, &matInvMatrix);

	Ray newRay;
	newRay.origin = origin;
	newRay.direction = direction;

	//�޽� �浹 �����Լ�

	cXMesh_Static* pStaticMesh = dynamic_cast<cXMesh_Static*>(pObject->pMesh);

	if (pStaticMesh != NULL)
	{
		//����´�� ����
		for (int i = 0; i < pStaticMesh->dwMaterialsNum; i++)
		{
			BOOL bHit = false;
			DWORD faceIndex = 0;
			float dist = 0.0f;
			DWORD hitCount = 0;

			D3DXIntersectSubset(
				pStaticMesh->pMesh,		//xMesh
				i,						//����³�
				&origin,				//������ġ
				&direction,				//���� ����
				&bHit,					//�浹 ���� ���
				&faceIndex,				//�浹�� �ε��� 
				NULL,
				NULL,
				&dist,					//�浹�Ÿ�
				NULL,					//�����浹�� ������Ʈ���� ���� ����, 
				&hitCount);				//�����浹�� ��Ʈ ���� 


			if (bHit){

				//hit ��������� ���ܼ� ����
				if (pHitPos != NULL){
					D3DXVec3TransformCoord(pHitPos,
						&(origin + direction * dist), &matWorld);
				}

				//Hit �븻 ���Ѵ�.
				if (pHitNormal != NULL){

					//�浹�� �ε����� ���� ���� 3���� ���Ѵ�.
					DWORD i0 = (faceIndex * 3);
					DWORD i1 = (faceIndex * 3 + 1);
					DWORD i2 = (faceIndex * 3 + 2);

					//�����ε����� ��´�.
					D3DXVECTOR3 v0 = pStaticMesh->Vertices[pStaticMesh->Indices[i0]];
					D3DXVECTOR3 v1 = pStaticMesh->Vertices[pStaticMesh->Indices[i1]];
					D3DXVECTOR3 v2 = pStaticMesh->Vertices[pStaticMesh->Indices[i2]];

					//���� 2���� �븻�� ���Ѵ�.
					D3DXVECTOR3 edge1 = v1 - v0;
					D3DXVECTOR3 edge2 = v2 - v0;

					D3DXVec3Cross(pHitNormal, &edge1, &edge2);
					D3DXVec3Normalize(pHitNormal, pHitNormal);

					D3DXVec3TransformNormal(pHitNormal, pHitNormal, &matWorld);

				}
				return true;
			}
		}
	}

	return false;
}

bool cPhysicManager::IsOverlap(cBaseObject* pObjA, cBaseObject* pObjB)
{
	return this->IsOverlap(
		pObjA->pTransform, &pObjA->BoundBox,
		pObjB->pTransform, &pObjB->BoundBox);
}

//2���� �ٿ�� ���� �⵿ ������ ��´�.
bool cPhysicManager::IsOverlap(cTransform* pTransA, cBoundSphere* pBoundA, cTransform* pTransB, cBoundSphere* pBoundB)
{
	//���� �������� ��´�.
	D3DXVECTOR3 centerA;
	float radiusA;
	D3DXVECTOR3 centerB;
	float radiusB;

	pBoundA->GetWorldCenterRadius(pTransA, &centerA, &radiusA);
	pBoundB->GetWorldCenterRadius(pTransB, &centerB, &radiusB);

	D3DXVECTOR3 dirTo = centerB - centerA;

	//�Ÿ��� ��������´� ( sqrt ���� �����Ѵ� )
	float distPow2 = D3DXVec3LengthSq(&dirTo);

	//������ ������
	float sumRadiusPow2 = radiusA + radiusB;
	sumRadiusPow2 *= sumRadiusPow2;


	if (distPow2 > sumRadiusPow2)
		return false;


	return true;
}


bool cPhysicManager::IsOverlap(cTransform* pTransA, cBoundBox* pBoundA, cTransform* pTransB, cBoundBox* pBoundB)
{
	//���� �������� ��´�.
	D3DXVECTOR3 centerA;
	float radiusA;
	D3DXVECTOR3 centerB;
	float radiusB;

	pBoundA->GetWorldCenterRadius(pTransA, &centerA, &radiusA);
	pBoundB->GetWorldCenterRadius(pTransB, &centerB, &radiusB);

	D3DXVECTOR3 dirTo = centerB - centerA;

	//�Ÿ��� ��������´� ( sqrt ���� �����Ѵ� )
	float distPow2 = D3DXVec3LengthSq(&dirTo);

	//������ ������
	float sumRadiusPow2 = radiusA + radiusB;
	sumRadiusPow2 *= sumRadiusPow2;

	//���� ������ �浹 �������� �׾��ٱ���� ���ڳ����� �浹���� �ʴ´�...
	if (distPow2 > sumRadiusPow2)
		return false;

	//������� �´ٸ� ������������ �浹�ߴٴ� ����


	//�䳪���� OBB �ڽ� �浹üũ.....

	//�迭����
	//X = 0, Y = 1, Z = 2;
	//OBB �浹�� �ʿ��� �浹 ����ü
	struct OBB{
		D3DXVECTOR3 center;		//�߽���
		D3DXVECTOR3 axis[3];	//�����	
		float halfLength[3];	//���࿡ ���� ���� ũ��
	};

	//
	// A �ٿ���� ���� �浹 ����ü
	//
	OBB obbA;

	//�� �� ����
	obbA.axis[0] = pTransA->GetRight();
	obbA.axis[1] = pTransA->GetUp();
	obbA.axis[2] = pTransA->GetForward();

	//����
	obbA.center = centerA;

	//����������
	D3DXVECTOR3 scaleA = pTransA->GetScale();
	obbA.halfLength[0] = pBoundA->halfSize.x * scaleA.x;
	obbA.halfLength[1] = pBoundA->halfSize.y * scaleA.y;
	obbA.halfLength[2] = pBoundA->halfSize.z * scaleA.z;

	//
	// B �ٿ���� ���� �浹 ����ü
	//
	OBB obbB;

	//�� �� ����
	obbB.axis[0] = pTransB->GetRight();
	obbB.axis[1] = pTransB->GetUp();
	obbB.axis[2] = pTransB->GetForward();

	//����
	obbB.center = centerB;

	//����������
	D3DXVECTOR3 scaleB = pTransB->GetScale();
	obbB.halfLength[0] = pBoundB->halfSize.x * scaleB.x;
	obbB.halfLength[1] = pBoundB->halfSize.y * scaleB.y;
	obbB.halfLength[2] = pBoundB->halfSize.z * scaleB.z;

	//
	// OBB �浹
	//
	float cos[3][3];		//���� �������� ���� �ڻ��� ��  [A��][B��]  ( [0][1] => ���ε����� A�� X ��� B�� Y ���� ������ ���� cos ���̴� )
	float absCos[3][3];		//���� �������� ���� �ڻ��� ���밪  [A��][B��]  ( [0][1] => ���ε����� A�� X ��� B�� Y ���� ������ ���� cos ���� ���̴� )
	float dist[3];			//A �ٿ�� �������� A �߽������� B �� �߽������͸� ������ ��������

	const float cutOff = 0.99999f;		//���� �Ǵ��� �ϱ����� �ƿ��� �� ( ��� �� ���� cos ��� ���� �̺��� ũ�ٸ� �� �浹ü�� ������ �����ϴٴ� ���� )
	bool existParallelPair = false;		//�� ���̶� �����ϳ�?

	//A ���� B �� ���⺤��
	D3DXVECTOR3 D = obbB.center - obbA.center;

	float r, r0, r1;			//r0 �� r1 �� ���� r ���� ������ �浹 ���� 

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			cos[a][b] = D3DXVec3Dot(&obbA.axis[a], &obbB.axis[b]);
			absCos[a][b] = abs(cos[a][b]);

			//������ ���� ���� �Ǵ� ��Ȯ��
			if (absCos[a][b] > cutOff) existParallelPair = true;
		}

		//���ͳ����� ���⺤�͸� A �ٿ�� Axis �� ������ �Ÿ�
		dist[a] = D3DXVec3Dot(&obbA.axis[a], &D);
	}


	//
	// A �ٿ�� �ڽ��� X ���� �������� �� ����
	//

	//r �� dist[0] �� ���� ���� �ȴ�.
	r = abs(dist[0]);

	//r0 
	r0 = obbA.halfLength[0];

	//r1
	/*
	r1 = abs( D3DXVec3Dot( &obbA.axis[0], &( obbB.axis[0] * obbB.halfLength[0] ) ) ) +
	abs( D3DXVec3Dot( &obbA.axis[0], &( obbB.axis[1] * obbB.halfLength[1] ) ) ) +
	abs( D3DXVec3Dot( &obbA.axis[0], &( obbB.axis[2] * obbB.halfLength[2] ) ) );
	*/
	/*
	r1 = abs( D3DXVec3Dot( &obbA.axis[0], &obbB.axis[0] ) ) * obbB.halfLength[0] +
	abs( D3DXVec3Dot( &obbA.axis[0], &obbB.axis[1] ) ) * obbB.halfLength[1] +
	abs( D3DXVec3Dot( &obbA.axis[0], &obbB.axis[2] ) ) * obbB.halfLength[2];
	*/

	r1 = absCos[0][0] * obbB.halfLength[0] +
		absCos[0][1] * obbB.halfLength[1] +
		absCos[0][2] * obbB.halfLength[2];

	if (r > r0 + r1) return false;

	//
	// A �ٿ�� �ڽ��� Y ���� �������� �� ����
	//

	//r �� dist[1] �� ���� ���� �ȴ�.
	r = abs(dist[1]);

	//r0 
	r0 = obbA.halfLength[1];

	//r1 
	r1 = absCos[1][0] * obbB.halfLength[0] +
		absCos[1][1] * obbB.halfLength[1] +
		absCos[1][2] * obbB.halfLength[2];

	if (r > r0 + r1) return false;

	//
	// A �ٿ�� �ڽ��� Z ���� �������� �� ����
	//

	//r �� dist[2] �� ���� ���� �ȴ�.
	r = abs(dist[2]);

	//r0 
	r0 = obbA.halfLength[2];

	//r1 
	r1 = absCos[2][0] * obbB.halfLength[0] +
		absCos[2][1] * obbB.halfLength[1] +
		absCos[2][2] * obbB.halfLength[2];

	if (r > r0 + r1) return false;

	//
	// B �ٿ�� �ڽ��� X ���� �������� �� ����
	//
	r = abs(D3DXVec3Dot(&obbB.axis[0], &D));

	//r0 
	r0 = absCos[0][0] * obbA.halfLength[0] +
		absCos[1][0] * obbA.halfLength[1] +
		absCos[2][0] * obbA.halfLength[2];

	//r1 
	r1 = obbB.halfLength[0];

	if (r > r0 + r1) return false;

	//
	// B �ٿ�� �ڽ��� Y ���� �������� �� ����
	//
	r = abs(D3DXVec3Dot(&obbB.axis[1], &D));

	//r0 
	r0 = absCos[0][1] * obbA.halfLength[0] +
		absCos[1][1] * obbA.halfLength[1] +
		absCos[2][1] * obbA.halfLength[2];

	//r1 
	r1 = obbB.halfLength[1];
	if (r > r0 + r1) return false;

	//
	// B �ٿ�� �ڽ��� Z ���� �������� �� ����
	//
	r = abs(D3DXVec3Dot(&obbB.axis[2], &D));

	//r0 
	r0 = absCos[0][2] * obbA.halfLength[0] +
		absCos[1][2] * obbA.halfLength[1] +
		absCos[2][2] * obbA.halfLength[2];

	//r1 
	r1 = obbB.halfLength[2];
	if (r > r0 + r1) return false;

	//�����Դµ� ���е��� �ʾҴ�.. �׷��� existParallelPair true �̸�
	//������ �����ϴٴ� �����ε� �̷��� �и��� 9���� �˻��ϸ�ȴ�....
	if (existParallelPair) return true;

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////

	//A�� X �� B �� X �࿡ ���� ������ �浹 üũ
	r = abs(dist[2] * cos[1][0] - dist[1] * cos[2][0]);
	r0 = obbA.halfLength[1] * absCos[2][0] + obbA.halfLength[2] * absCos[1][0];
	r1 = obbB.halfLength[1] * absCos[0][2] + obbB.halfLength[2] * absCos[0][1];
	if (r > r0 + r1)
		return false;

	//A�� X �� B �� Y �࿡ ���� ������ �浹 üũ
	r = abs(dist[2] * cos[1][1] - dist[1] * cos[2][1]);
	r0 = obbA.halfLength[1] * absCos[2][1] + obbA.halfLength[2] * absCos[1][1];
	r1 = obbB.halfLength[0] * absCos[0][2] + obbB.halfLength[2] * absCos[0][0];
	if (r > r0 + r1)
		return false;

	//A�� X �� B �� Z �࿡ ���� ������ �浹 üũ
	r = abs(dist[2] * cos[1][2] - dist[1] * cos[2][2]);
	r0 = obbA.halfLength[1] * absCos[2][2] + obbA.halfLength[2] * absCos[1][2];
	r1 = obbB.halfLength[0] * absCos[0][1] + obbB.halfLength[1] * absCos[0][0];
	if (r > r0 + r1)
		return false;

	/////////////////////////////////////////////////////////////////

	//A�� Y �� B �� X �࿡ ���� ������ �浹 üũ
	r = abs(dist[0] * cos[2][0] - dist[2] * cos[0][0]);
	r0 = obbA.halfLength[0] * absCos[2][0] + obbA.halfLength[2] * absCos[0][0];
	r1 = obbB.halfLength[1] * absCos[1][2] + obbB.halfLength[2] * absCos[1][1];
	if (r > r0 + r1)
		return false;

	//A�� Y �� B �� Y �࿡ ���� ������ �浹 üũ
	r = abs(dist[0] * cos[2][1] - dist[2] * cos[0][1]);
	r0 = obbA.halfLength[0] * absCos[2][1] + obbA.halfLength[2] * absCos[0][1];
	r1 = obbB.halfLength[0] * absCos[1][2] + obbB.halfLength[2] * absCos[1][0];
	if (r > r0 + r1)
		return false;

	//A�� Y �� B �� Z �࿡ ���� ������ �浹 üũ
	r = abs(dist[0] * cos[2][2] - dist[2] * cos[0][2]);
	r0 = obbA.halfLength[0] * absCos[2][2] + obbA.halfLength[2] * absCos[0][2];
	r1 = obbB.halfLength[0] * absCos[1][1] + obbB.halfLength[1] * absCos[1][0];
	if (r > r0 + r1)
		return false;


	/////////////////////////////////////////////////////////////////	 

	//A�� Z �� B �� X �࿡ ���� ������ �浹 üũ
	r = abs(dist[1] * cos[0][0] - dist[0] * cos[1][0]);
	r0 = obbA.halfLength[0] * absCos[1][0] + obbA.halfLength[1] * absCos[0][0];
	r1 = obbB.halfLength[1] * absCos[2][2] + obbB.halfLength[2] * absCos[2][1];
	if (r > r0 + r1)
		return false;

	//A�� Z �� B �� Y �࿡ ���� ������ �浹 üũ
	r = abs(dist[1] * cos[0][1] - dist[0] * cos[1][1]);
	r0 = obbA.halfLength[0] * absCos[1][1] + obbA.halfLength[1] * absCos[0][1];
	r1 = obbB.halfLength[0] * absCos[2][2] + obbB.halfLength[2] * absCos[2][0];
	if (r > r0 + r1)
		return false;

	//A�� Z �� B �� Z �࿡ ���� ������ �浹 üũ
	r = abs(dist[1] * cos[0][2] - dist[0] * cos[1][2]);
	r0 = obbA.halfLength[0] * absCos[1][2] + obbA.halfLength[1] * absCos[0][2];
	r1 = obbB.halfLength[0] * absCos[2][1] + obbB.halfLength[1] * absCos[2][0];
	if (r > r0 + r1)
		return false;

	//������� Ŭ���� �ߴٸ�  ����� �浹 ����... ( PS �浹 �ƴٴ� ����� )
	return true;
}

bool cPhysicManager::IsOverlap(cTransform* pTransA, cBoundSphere* pBoundA, cTransform* pTransB, cBoundBox* pBoundB)
{
	return true;
}
bool cPhysicManager::IsOverlap(cTransform* pTransA, cBoundBox* pBoundA, cTransform* pTransB, cBoundSphere* pBoundB)
{
	return true;
}


bool cPhysicManager::IsBlocking(cBaseObject* pObjA, cBaseObject* pObjB, float moveFactor)
{
	return this->IsBlocking(
		pObjA->pTransform, &pObjA->BoundBox,
		pObjB->pTransform, &pObjB->BoundBox, moveFactor);
}

//2���� �ٿ�� ���Ǿ���� �⵿�ϰ� ��ġ�� �ʰ� �Ѵ�.
//moveFactor �� 0 �ϼ��� ��Ĩ�� ���� B �� �����δ�.
//moveFactor �� 0.5 �̸� ��Ĩ�� ���� A �� B �� �Ȱ��������� �����δ�.
//moveFactor �� 1 �ϼ��� ��Ĩ�� ���� A �� �����δ�.
bool cPhysicManager::IsBlocking(
	cTransform* pTransA, cBoundSphere* pBoundA,
	cTransform* pTransB, cBoundSphere* pBoundB,
	float moveFactor)

{
	return true;
}

bool cPhysicManager::IsBlocking(
	cTransform* pTransA, cBoundBox* pBoundA,
	cTransform* pTransB, cBoundBox* pBoundB,
	float moveFactor)
{
	//���� �浹���� �ʾ����� ���ʿ����
	if (IsOverlap(pTransA, pBoundA, pTransB, pBoundB) == false)
		return false;

	moveFactor = Clamp01(moveFactor);



	//���� �ε��ƽ��� �о���Ѵ�...

	//A�� Min Max
	D3DXVECTOR3 minA = pBoundA->localMinPos;
	D3DXVECTOR3 maxA = pBoundA->localMaxPos;

	//B�� Min Max
	D3DXVECTOR3 minB = pBoundB->localMinPos;
	D3DXVECTOR3 maxB = pBoundB->localMaxPos;

	//B �� ���� �簢 8 ��

	//       5-------6 Max
	//      /|      /|
	//     1-------2 |
	//     | 4-----|-7
	//     |/      |/
	// Min 0-------3

	D3DXVECTOR3 pos[8];
	pos[0] = D3DXVECTOR3(minB.x, minB.y, minB.z);
	pos[1] = D3DXVECTOR3(minB.x, maxB.y, minB.z);
	pos[2] = D3DXVECTOR3(maxB.x, maxB.y, minB.z);
	pos[3] = D3DXVECTOR3(maxB.x, minB.y, minB.z);
	pos[4] = D3DXVECTOR3(minB.x, minB.y, maxB.z);
	pos[5] = D3DXVECTOR3(minB.x, maxB.y, maxB.z);
	pos[6] = D3DXVECTOR3(maxB.x, maxB.y, maxB.z);
	pos[7] = D3DXVECTOR3(maxB.x, minB.y, maxB.z);


	//A �� ���� �����
	D3DXMATRIXA16 matWorldAInv;
	D3DXMatrixInverse(&matWorldAInv, NULL, &pTransA->GetFinalMatrix());

	//B �� ���� ���
	D3DXMATRIXA16 matWorldB = pTransB->GetFinalMatrix();

	//B ���� ��ŭ ���� A �� ������ �ٽ� ������ ���
	D3DXMATRIXA16 mat = matWorldB * matWorldAInv;

	//B pos �� ����
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	//������ Pos ���� A ���� B �� ������� ��ġ�����̵ȴ�.


	//���������� ����� B pos �� ������ min max �� ���� ����
	minB = pos[0];
	maxB = pos[0];
	for (int i = 1; i < 8; i++){
		if (pos[i].x < minB.x) minB.x = pos[i].x;
		if (pos[i].y < minB.y) minB.y = pos[i].y;
		if (pos[i].z < minB.z) minB.z = pos[i].z;
		if (pos[i].x > maxB.x) maxB.x = pos[i].x;
		if (pos[i].y > maxB.y) maxB.y = pos[i].y;
		if (pos[i].z > maxB.z) maxB.z = pos[i].z;
	}

	//�簢 �� ����ü
	struct fRect{
		float left;
		float right;
		float bottom;
		float top;
		float back;
		float front;
	};

	fRect rcA = { minA.x, maxA.x, minA.y, maxA.y, minA.z, maxA.z };
	fRect rcB = { minB.x, maxB.x, minB.y, maxB.y, minB.z, maxB.z };

	//��Ĩ���� �簢��
	fRect rcInter;
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);


	//������ ��Ĩ���� ���ϰ� ���� ���� ���� ������ B �̵���Ų��.
	float interX = rcInter.right - rcInter.left;
	float interY = rcInter.top - rcInter.bottom;
	float interZ = rcInter.front - rcInter.back;
	float minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	//�̴� ����
	D3DXVECTOR3 moveDirA(0, 0, 0);

	//�з�
	float moveLengthA = minInter;

	//X ���� ��Ĩ���� ���� �۴ٸ�..
	if (minInter == interX)
	{
		//A �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.left, rcA.left))
			moveDirA = -pTransA->GetRight();

		//A �� ���������� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.right, rcA.right))
			moveDirA = pTransA->GetRight();
	}
	//Y ���� ��Ĩ���� ���� �۴ٸ�..
	else if (minInter == interY)
	{
		//A �� ������ �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.top, rcA.top))
			moveDirA = pTransA->GetUp();

		//A �� �Ʒ����� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.bottom, rcA.bottom))
			moveDirA = -pTransA->GetUp();

	}

	//Z ���� ��ħ���� ���� �۴ٸ�..
	else if (minInter == interZ)
	{
		//A �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.front, rcA.front))
			moveDirA = pTransA->GetForward();

		//A �� ������ �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.back, rcA.back))
			moveDirA = -pTransA->GetForward();
	}

	//��������´ٸ� �з��� �̴� ������ ���ȴ�.
	//moveDirA
	//moveLengthA
	//���� �ΰ��� A �� ������ �ΰ� B �� A ������ ����� ���̵ȴ�.

	//
	// �Ʒ��� ������ B �� ������ �ΰ� A �� B ������ ����� ���̵ȴ�.
	//
	D3DXVECTOR3 moveDirB(0, 0, 0);
	float moveLengthB = 0.0f;

	//A�� Min Max
	minA = pBoundA->localMinPos;
	maxA = pBoundA->localMaxPos;

	//B�� Min Max
	minB = pBoundB->localMinPos;
	maxB = pBoundB->localMaxPos;

	//B �� ���� �簢 8 ��

	//       5-------6 Max
	//      /|      /|
	//     1-------2 |
	//     | 4-----|-7
	//     |/      |/
	// Min 0-------3

	pos[0] = D3DXVECTOR3(minA.x, minA.y, minA.z);
	pos[1] = D3DXVECTOR3(minA.x, maxA.y, minA.z);
	pos[2] = D3DXVECTOR3(maxA.x, maxA.y, minA.z);
	pos[3] = D3DXVECTOR3(maxA.x, minA.y, minA.z);
	pos[4] = D3DXVECTOR3(minA.x, minA.y, maxA.z);
	pos[5] = D3DXVECTOR3(minA.x, maxA.y, maxA.z);
	pos[6] = D3DXVECTOR3(maxA.x, maxA.y, maxA.z);
	pos[7] = D3DXVECTOR3(maxA.x, minA.y, maxA.z);

	//B �� ���� �����
	D3DXMATRIXA16 matWorldBInv;
	D3DXMatrixInverse(&matWorldBInv, NULL, &pTransB->GetFinalMatrix());

	//A �� ���� ���
	D3DXMATRIXA16 matWorldA = pTransA->GetFinalMatrix();

	//A ���� ��ŭ ���� B �� ������ �ٽ� ������ ���
	mat = matWorldA * matWorldBInv;

	//A pos �� ����
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	//���������� ����� A pos �� ������ min max �� ���� ����
	minA = pos[0];
	maxA = pos[0];
	for (int i = 1; i < 8; i++){
		if (pos[i].x < minA.x) minA.x = pos[i].x;
		if (pos[i].y < minA.y) minA.y = pos[i].y;
		if (pos[i].z < minA.z) minA.z = pos[i].z;
		if (pos[i].x > maxA.x) maxA.x = pos[i].x;
		if (pos[i].y > maxA.y) maxA.y = pos[i].y;
		if (pos[i].z > maxA.z) maxA.z = pos[i].z;
	}
	//rcA = { minA.x, maxA.x, minA.y, maxA.y, minA.z, maxA.z };
	//rcB = { minB.x, maxB.x, minB.y, maxB.y, minB.z, maxB.z };
	rcA.left = minA.x;		rcA.right = maxA.x;			rcA.bottom = minA.y;			rcA.top = maxA.y;			rcA.back = minA.z;			rcA.front = maxA.z;
	rcB.left = minB.x;		rcB.right = maxB.x;			rcB.bottom = minB.y;			rcB.top = maxB.y;			rcB.back = minB.z;			rcB.front = maxB.z;

	//��Ĩ���� �簢��
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);

	//������ ��Ĩ���� ���ϰ� ���� ���� ���� ������ B �̵���Ų��.
	interX = rcInter.right - rcInter.left;
	interY = rcInter.top - rcInter.bottom;
	interZ = rcInter.front - rcInter.back;
	minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	moveLengthB = minInter;

	//X ���� ��Ĩ���� ���� �۴ٸ�..
	if (minInter == interX)
	{
		//B �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.left, rcB.left))
			moveDirB = -pTransB->GetRight();

		//B �� ���������� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.right, rcB.right))
			moveDirB = pTransB->GetRight();
	}
	//Y ���� ��Ĩ���� ���� �۴ٸ�..
	else if (minInter == interY)
	{
		//B �� ������ �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.top, rcB.top))
			moveDirB = pTransB->GetUp();

		//B �� �Ʒ����� �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.bottom, rcB.bottom))
			moveDirB = -pTransB->GetUp();

	}

	//Z ���� ��ħ���� ���� �۴ٸ�..
	else if (minInter == interZ)
	{
		//B �� �������� �о�� �Ѵٸ�....
		if (FLOATEQUAL(rcInter.front, rcB.front))
			moveDirB = pTransB->GetForward();

		//B �� ������ �о�� �Ѵٸ�....
		else if (FLOATEQUAL(rcInter.back, rcB.back))
			moveDirB = -pTransB->GetForward();
	}


	//��������´ٸ� �Ʒ��� 4 ���� ������ ���� ���̴�....
	//D3DXVECTOR3 moveDirA( 0, 0, 0 );
	//float moveLengthA = minInter;
	//D3DXVECTOR3 moveDirB( 0, 0, 0 );
	//float moveLengthB = 0.0f;


	//�з��� ����������...
	if (moveLengthB > moveLengthA)
	{
		//A �� B �� ������ ����
		//���� A �� ������ �ΰ� B �� �ǵ��ΰŴ�
		//���� A �� �������� 2 ���� B �� ������ �� 1 �̿��ٰ� �����Ͽ�����
		//A �� ����ķ� ���Ǿ� A �� �������� 1 B �� �������� 0.5 �� ���� �����̴�
		//�׷��Ƿ� �� ���� ���̴�
		//Ascale ���� ���Ͽ� ����ϴ� ���� �ùٸ���.
		D3DXVECTOR3 scaleA = pTransA->GetScale();
		D3DXVECTOR3 scaleB = pTransB->GetScale();

		pTransB->MovePositionWorld(
			moveDirA.x * moveLengthA * (1.0 - moveFactor) * scaleA.x,
			moveDirA.y * moveLengthA * (1.0 - moveFactor) * scaleA.y,
			moveDirA.z * moveLengthA * (1.0 - moveFactor) * scaleA.z);


		pTransA->MovePositionWorld(
			-moveDirA.x * moveLengthA * moveFactor * scaleA.x,
			-moveDirA.y * moveLengthA * moveFactor * scaleA.y,
			-moveDirA.z * moveLengthA * moveFactor * scaleA.z);


	}


	else
	{
		//A �� B �� ������ ����
		//���� B �� ������ �ΰ� A �� �ǵ��ΰŴ� /
		//���� A �� �������� 2 ���� B �� ������ �� 1 �̿��ٰ� �����Ͽ�����
		//B �� ����ķ� ���Ǿ� A �� �������� 2 B �� �������� 1 �� ���� �����̴�
		//�׷��Ƿ� �� ���� ���̴� BScale ���� ���Ͽ� ����ϴ� ���� �ùٸ���.;

		D3DXVECTOR3 scaleA = pTransA->GetScale();
		D3DXVECTOR3 scaleB = pTransB->GetScale();

		pTransA->MovePositionWorld(
			moveDirB.x * moveLengthB * moveFactor  * scaleB.x,
			moveDirB.y * moveLengthB * moveFactor  * scaleB.y,
			moveDirB.z * moveLengthB * moveFactor  * scaleB.z);

		pTransB->MovePositionWorld(
			-moveDirB.x * moveLengthB * (1.0 - moveFactor) *scaleB.x,
			-moveDirB.y * moveLengthB * (1.0 - moveFactor) *scaleB.y,
			-moveDirB.z * moveLengthB * (1.0 - moveFactor) *scaleB.z);
	}


	return true;



}


//�� 3���� ���� ����� �����.
void cPhysicManager::CreatePlane(LPD3DXPLANE	pOutPlane, const D3DXVECTOR3* p0, const D3DXVECTOR3* p1, const D3DXVECTOR3* p2)
{
	//����� �븻 
	D3DXVECTOR3 normal;
	D3DXVECTOR3 edge1 = *p1 - *p0;
	D3DXVECTOR3 edge2 = *p2 - *p0;
	D3DXVec3Cross(&normal, &edge1, &edge2);
	D3DXVec3Normalize(&normal, &normal);

	//����� �븻 �������� ���������� �ִ� �Ÿ�
	float dist = -D3DXVec3Dot(&normal, p0);

	//��鰪 ����
	pOutPlane->a = normal.x;
	pOutPlane->b = normal.y;
	pOutPlane->c = normal.z;
	pOutPlane->d = dist;

	//D3DXPlaneFromPoints(
}

//������ �������� �������� �ִܰŸ�
float cPhysicManager::PlaneDot(const LPD3DXPLANE pPlane, const D3DXVECTOR3* point)
{
	//����� �븻
	D3DXVECTOR3 normal(pPlane->a, pPlane->b, pPlane->c);

	/*
	//��� ���� ������ ����???
	D3DXVECTOR3 p = normal * -pPlane->d;

	//������ ������ �������� ���� �Ű������� ���� �������� ���⺤��
	D3DXVECTOR3 dir = *point - p;

	//����
	float distance = D3DXVec3Dot( &normal, &dir );
	*/

	float distance = D3DXVec3Dot(&normal, point) + pPlane->d;


	return distance;
}

//�������� ����� �浹 ��ġ
bool cPhysicManager::IntersectRayToPlane(D3DXVECTOR3* pOut, const LPRay pRay, const LPD3DXPLANE pPlane, bool bCheck2Side /*=false*/)
{
	//�븻 ����
	D3DXVECTOR3 normal(pPlane->a, pPlane->b, pPlane->c);

	float dot2 = D3DXVec3Dot(&normal, &pRay->direction);//������ ����� ����� ���� �������� ������ cos��

	//�׸��带 0,0,0 ���� ���� �� ���̴� �Ͱ� ���� ��ġ��.
	//�������� ����� ������ 90 ��� ���� �����ϴٴ� ���.
	if (FLOATEQUAL(dot2, 0.f))
	{
		return false;
	}

	//�������� ���������� �������� �ִܰŸ�
	float dist = D3DXVec3Dot(&normal, &pRay->origin) + pPlane->d;

	//��� üũ�� �� �Ѵٸ�
	if (bCheck2Side == false)
	{
		//�������� ���� ��ġ�� �ڿ� �־ ����
		if (dist < 0.f)
			return false;
	}

	//�������������� �浹���������� �Ÿ�
	float t = dist / -dot2;

	//���� ������ �ݴ�� �Ǿ� �ִ� ���
	if (t < 0.f)
		return false;

	//�浹 ��ġ
	*pOut = pRay->origin + (pRay->direction * t);

	return true;
}
