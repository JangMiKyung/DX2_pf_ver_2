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
	//위치값
	D3DXMATRIXA16 matWorld = pBoundTrans->GetFinalMatrix();

	//스케일값
	D3DXVECTOR3 scale = pBoundTrans->GetScale();

	//바운드 스피어 의 정보를 얻는다.
	D3DXVECTOR3 center;
	float radius;
	D3DXVECTOR3 halfSize;

	//변환된 위치
	D3DXVec3TransformCoord(&center, &pBound->localCenter, &matWorld);

	//반지름...
	halfSize.x = pBound->halfSize.x * scale.x;
	halfSize.y = pBound->halfSize.y * scale.y;
	halfSize.z = pBound->halfSize.z * scale.z;
	radius = D3DXVec3Length(&halfSize);

	//레이의 오리진에서 부터 구의 센터까지의 방향벡터
	D3DXVECTOR3 dirToCenter = center - Ray->origin;

	//길이의 제곱
	float lengthSq = D3DXVec3LengthSq(&dirToCenter);

	//반지름의 제곱
	float r2 = radius * radius;

	//만약 광선이 구안에 있다면...
	if (r2 > lengthSq)
	{
		//광선이 안에서 나가는 것은 체크 안된다.
		return false;
	}

	//여기까지오면 오리진은 구 밖에 있다는 얘기이고
	//구센터까지의 방향벡터와 레이의 방향벡터가 직각을 포함한
	//둔각이라면 죽었다깨어나도 충돌될 일이 없다.
	float dot = D3DXVec3Dot(&dirToCenter, &Ray->direction);

	if (dot <= 0.f)
	{
		return false;
	}

	//피타고라스의 정리를 하기 위해 직각 삼각형 공식유도
	//d2 = x2 + y2
	//d = sqrt(x2 + y2);

	float x2 = dot * dot;
	float d2 = lengthSq;

	//d2 - x2 = y2;
	float y2 = d2 - x2;

	//광선이 원 밖을 벗어났다.
	if (y2 > r2)
	{
		return false;
	}

	//여기까지 왔다면 일단은 히트다 히트
	//만약 얻어가야할 HitPoint 가 있다면..
	if (pHitPos != NULL)
	{
		//d를 radius 제곱
		d2 = r2;
		//d2 = y2 + x2
		//float x2 = d2 - y2;
		float x = sqrt(d2 - y2);

		*pHitPos = Ray->origin + (Ray->direction * (dot - x));

		//Hit 된 위치의 노말을 얻겠다면...
		if (pHitNormal)
		{
			*pHitNormal = *pHitNormal - center;
			D3DXVec3Normalize(pHitNormal, pHitNormal);
		}
	}

	return true;

}

//레이가 바운드에 충돌했는지 확인
bool cPhysicManager::IsRayHitBound(
	LPRay pRay,				   //레이
	cBoundBox* pBound,		   //바운드
	cTransform* pBoundTrans,   //바운드의 Transform
	D3DXVECTOR3* pHitPos,	   //Hit 위치 ( NULL 이면 대입 안됨 )
	D3DXVECTOR3* pHitNormal	   //Hit 의 노말 ( NULL 이면 대입 안됨 )
	)
{
	//애시당초 구랑 충돌하지 않았으면 사각형과 죽었다깨어나도 충돌 안됨.
	if (this->IsRayHitBound(pRay,
		(cBoundSphere*)pBound,
		pBoundTrans,
		NULL, NULL) == false)
	{
		return false;
	}

	//여기까지 왔으면 사각형과 충돌검사를 하자

	//레이를 로컬로 끌어댕긴다.
	//bound 의 역행렬
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

	//정점 8 개 뺀다...

	//   5-------6
	//  /|      /|
	// 1-------2 |
	// | 4-----|-7
	// |/      |/
	// 0-------3 

	//로컬 8개의 정점을 구한다
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

	//뒷면
	D3DXPLANE planeBack;
	this->CreatePlane(&planeBack, Vertices + 0, Vertices + 1, Vertices + 2);
	//뒷면이 레이랑 총돌했니?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeBack, false))
	{
		//히트 지점이 이 안에 있니?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.y <= hit.y && hit.y <= max.y)
		{
			//hit 지점월드로 땡겨서 리턴
			if (pHitPos != NULL)
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			if (pHitNormal != NULL)
				*pHitNormal = -pBoundTrans->GetForward();
		}

		return true;
	}

	//앞면
	D3DXPLANE planeFront;
	this->CreatePlane(&planeFront, Vertices + 6, Vertices + 5, Vertices + 4);
	//앞면이 레이랑 충돌했나?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeFront, false))
	{
		//히트지점이 이 안에 있니?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.y <= hit.y && hit.y <= max.y){

			//hit 지점 월드로 땡겨서 리턴
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}
			if (pHitNormal != NULL){
				*pHitNormal = pBoundTrans->GetForward();
			}

			return true;
		}
	}

	//하면
	D3DXPLANE planeBottom;
	this->CreatePlane(&planeBottom, Vertices + 3, Vertices + 4, Vertices + 0);
	//하면이 레이랑 충돌했니?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeBottom, false))
	{
		//히트지점이 이 안에 있니?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.z <= hit.z && hit.z <= max.z){

			//hit 지점월드로 땡겨서 리턴
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}

			if (pHitNormal != NULL){
				*pHitNormal = -pBoundTrans->GetUp();
			}

			return true;
		}
	}

	//상면
	D3DXPLANE planeTop;
	this->CreatePlane(&planeTop, Vertices + 1, Vertices + 5, Vertices + 2);
	//상면이 레이랑 충돌했니?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeTop, false))
	{
		//히트지점이 이 안에 있니?
		if (min.x <= hit.x && hit.x <= max.x &&
			min.z <= hit.z && hit.z <= max.z){

			//hit 지점월드로 땡겨서 리턴
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}

			if (pHitNormal != NULL){
				*pHitNormal = pBoundTrans->GetUp();
			}

			return true;
		}
	}

	//좌면
	D3DXPLANE planeLeft;
	this->CreatePlane(&planeLeft, Vertices + 0, Vertices + 5, Vertices + 1);
	//좌면이 레이랑 충돌했니?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeLeft, false))
	{
		//히트지점이 이 안에 있니?
		if (min.y <= hit.y && hit.y <= max.y &&
			min.z <= hit.z && hit.z <= max.z){

			//hit 지점월드로 땡겨서 리턴
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}

			if (pHitNormal != NULL){
				*pHitNormal = -pBoundTrans->GetRight();
			}

			return true;
		}
	}

	//우면
	D3DXPLANE planeRight;
	this->CreatePlane(&planeRight, Vertices + 2, Vertices + 6, Vertices + 3);
	//우면이 레이랑 충돌했니?
	if (this->IntersectRayToPlane(&hit, &newRay, &planeRight, false))
	{
		//히트지점이 이 안에 있니?
		if (min.y <= hit.y && hit.y <= max.y &&
			min.z <= hit.z && hit.z <= max.z){

			//hit 지점월드로 땡겨서 리턴
			if (pHitPos != NULL){
				D3DXVec3TransformCoord(pHitPos, &hit, &matWorld);
			}

			if (pHitNormal != NULL){
				*pHitNormal = pBoundTrans->GetRight();
			}

			return true;
		}
	}

	//여기까지 왔으면 충돌 실패
	return false;

}

//레이가 오브젝트와 충돌했는지....
bool cPhysicManager::IsRayHitStaticMeshObject(
	LPRay pRay,				//레이
	cBaseObject* pObject,	//Base Object
	D3DXVECTOR3* pHitPos,     //Hit 위치 ( NULL 이면 대입 안됨 )
	D3DXVECTOR3* pHitNormal	  //Hit 의 노말 ( NULL 이면 대입 안됨 )
	)
{
	//레이를 로컬로 끌어땡긴다

	//bound 의 역행렬
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

	//메쉬 충돌 검출함수

	cXMesh_Static* pStaticMesh = dynamic_cast<cXMesh_Static*>(pObject->pMesh);

	if (pStaticMesh != NULL)
	{
		//서브셋대로 돈다
		for (int i = 0; i < pStaticMesh->dwMaterialsNum; i++)
		{
			BOOL bHit = false;
			DWORD faceIndex = 0;
			float dist = 0.0f;
			DWORD hitCount = 0;

			D3DXIntersectSubset(
				pStaticMesh->pMesh,		//xMesh
				i,						//서브셋넘
				&origin,				//레이위치
				&direction,				//레이 방향
				&bHit,					//충돌 여부 얻기
				&faceIndex,				//충돌면 인덱스 
				NULL,
				NULL,
				&dist,					//충돌거리
				NULL,					//다중충돌시 다중히트정보 얻을 버퍼, 
				&hitCount);				//다중충돌시 히트 갯수 


			if (bHit){

				//hit 지점월드로 땡겨서 리턴
				if (pHitPos != NULL){
					D3DXVec3TransformCoord(pHitPos,
						&(origin + direction * dist), &matWorld);
				}

				//Hit 노말 구한다.
				if (pHitNormal != NULL){

					//충돌면 인덱스에 따른 정점 3개를 구한다.
					DWORD i0 = (faceIndex * 3);
					DWORD i1 = (faceIndex * 3 + 1);
					DWORD i2 = (faceIndex * 3 + 2);

					//정점인덱스를 얻는다.
					D3DXVECTOR3 v0 = pStaticMesh->Vertices[pStaticMesh->Indices[i0]];
					D3DXVECTOR3 v1 = pStaticMesh->Vertices[pStaticMesh->Indices[i1]];
					D3DXVECTOR3 v2 = pStaticMesh->Vertices[pStaticMesh->Indices[i2]];

					//정점 2개의 노말을 구한다.
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

//2개의 바운드 대한 출동 정보를 얻는다.
bool cPhysicManager::IsOverlap(cTransform* pTransA, cBoundSphere* pBoundA, cTransform* pTransB, cBoundSphere* pBoundB)
{
	//월드 구정보를 얻는다.
	D3DXVECTOR3 centerA;
	float radiusA;
	D3DXVECTOR3 centerB;
	float radiusB;

	pBoundA->GetWorldCenterRadius(pTransA, &centerA, &radiusA);
	pBoundB->GetWorldCenterRadius(pTransB, &centerB, &radiusB);

	D3DXVECTOR3 dirTo = centerB - centerA;

	//거리의 제곱을얻는다 ( sqrt 연산 제외한다 )
	float distPow2 = D3DXVec3LengthSq(&dirTo);

	//반지름 합제곱
	float sumRadiusPow2 = radiusA + radiusB;
	sumRadiusPow2 *= sumRadiusPow2;


	if (distPow2 > sumRadiusPow2)
		return false;


	return true;
}


bool cPhysicManager::IsOverlap(cTransform* pTransA, cBoundBox* pBoundA, cTransform* pTransB, cBoundBox* pBoundB)
{
	//월드 구정보를 얻는다.
	D3DXVECTOR3 centerA;
	float radiusA;
	D3DXVECTOR3 centerB;
	float radiusB;

	pBoundA->GetWorldCenterRadius(pTransA, &centerA, &radiusA);
	pBoundB->GetWorldCenterRadius(pTransB, &centerB, &radiusB);

	D3DXVECTOR3 dirTo = centerB - centerA;

	//거리의 제곱을얻는다 ( sqrt 연산 제외한다 )
	float distPow2 = D3DXVec3LengthSq(&dirTo);

	//반지름 합제곱
	float sumRadiusPow2 = radiusA + radiusB;
	sumRadiusPow2 *= sumRadiusPow2;

	//외접 구끼리 충돌 안했으면 죽었다깨어나도 상자끼리는 충돌하지 않는다...
	if (distPow2 > sumRadiusPow2)
		return false;

	//여기까지 온다면 외접구끼리는 충돌했다는 예기


	//토나오는 OBB 박스 충돌체크.....

	//배열순서
	//X = 0, Y = 1, Z = 2;
	//OBB 충돌에 필요한 충돌 구조체
	struct OBB{
		D3DXVECTOR3 center;		//중심점
		D3DXVECTOR3 axis[3];	//축방향	
		float halfLength[3];	//각축에 대한 절반 크기
	};

	//
	// A 바운딩에 대한 충돌 구조체
	//
	OBB obbA;

	//각 축 방향
	obbA.axis[0] = pTransA->GetRight();
	obbA.axis[1] = pTransA->GetUp();
	obbA.axis[2] = pTransA->GetForward();

	//센터
	obbA.center = centerA;

	//하프사이즈
	D3DXVECTOR3 scaleA = pTransA->GetScale();
	obbA.halfLength[0] = pBoundA->halfSize.x * scaleA.x;
	obbA.halfLength[1] = pBoundA->halfSize.y * scaleA.y;
	obbA.halfLength[2] = pBoundA->halfSize.z * scaleA.z;

	//
	// B 바운딩에 대한 충돌 구조체
	//
	OBB obbB;

	//각 축 방향
	obbB.axis[0] = pTransB->GetRight();
	obbB.axis[1] = pTransB->GetUp();
	obbB.axis[2] = pTransB->GetForward();

	//센터
	obbB.center = centerB;

	//하프사이즈
	D3DXVECTOR3 scaleB = pTransB->GetScale();
	obbB.halfLength[0] = pBoundB->halfSize.x * scaleB.x;
	obbB.halfLength[1] = pBoundB->halfSize.y * scaleB.y;
	obbB.halfLength[2] = pBoundB->halfSize.z * scaleB.z;

	//
	// OBB 충돌
	//
	float cos[3][3];		//각축 차에대한 대한 코사인 값  [A축][B축]  ( [0][1] => 이인덱스는 A의 X 축과 B의 Y 축의 각차에 대한 cos 값이다 )
	float absCos[3][3];		//각축 차에대한 대한 코사인 절대값  [A축][B축]  ( [0][1] => 이인덱스는 A의 X 축과 B의 Y 축의 각차에 대한 cos 절대 값이다 )
	float dist[3];			//A 바운드 각축으로 A 중심점에서 B 의 중심점벡터를 투영한 투영길이

	const float cutOff = 0.99999f;		//수직 판단을 하기위한 컷오프 값 ( 어느 한 축의 cos 결과 값이 이보다 크다면 두 충돌체는 한축이 평행하다는 예기 )
	bool existParallelPair = false;		//한 축이라도 평행하나?

	//A 에서 B 의 방향벡터
	D3DXVECTOR3 D = obbB.center - obbA.center;

	float r, r0, r1;			//r0 과 r1 의 합이 r 보다 작으면 충돌 실패 

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			cos[a][b] = D3DXVec3Dot(&obbA.axis[a], &obbB.axis[b]);
			absCos[a][b] = abs(cos[a][b]);

			//한축이 서로 교차 되는 지확인
			if (absCos[a][b] > cutOff) existParallelPair = true;
		}

		//센터끼리의 방향벡터를 A 바운드 Axis 의 투영한 거리
		dist[a] = D3DXVec3Dot(&obbA.axis[a], &D);
	}


	//
	// A 바운드 박스에 X 축을 기준으로 한 연산
	//

	//r 은 dist[0] 의 절대 값이 된다.
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
	// A 바운드 박스에 Y 축을 기준으로 한 연산
	//

	//r 은 dist[1] 의 절대 값이 된다.
	r = abs(dist[1]);

	//r0 
	r0 = obbA.halfLength[1];

	//r1 
	r1 = absCos[1][0] * obbB.halfLength[0] +
		absCos[1][1] * obbB.halfLength[1] +
		absCos[1][2] * obbB.halfLength[2];

	if (r > r0 + r1) return false;

	//
	// A 바운드 박스에 Z 축을 기준으로 한 연산
	//

	//r 은 dist[2] 의 절대 값이 된다.
	r = abs(dist[2]);

	//r0 
	r0 = obbA.halfLength[2];

	//r1 
	r1 = absCos[2][0] * obbB.halfLength[0] +
		absCos[2][1] * obbB.halfLength[1] +
		absCos[2][2] * obbB.halfLength[2];

	if (r > r0 + r1) return false;

	//
	// B 바운드 박스에 X 축을 기준으로 한 연산
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
	// B 바운드 박스에 Y 축을 기준으로 한 연산
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
	// B 바운드 박스에 Z 축을 기준으로 한 연산
	//
	r = abs(D3DXVec3Dot(&obbB.axis[2], &D));

	//r0 
	r0 = absCos[0][2] * obbA.halfLength[0] +
		absCos[1][2] * obbA.halfLength[1] +
		absCos[2][2] * obbA.halfLength[2];

	//r1 
	r1 = obbB.halfLength[2];
	if (r > r0 + r1) return false;

	//여기까왔는데 실패되지 않았다.. 그러면 existParallelPair true 이면
	//한축이 평행하다는 예기인데 이러면 분리축 9번만 검색하면된다....
	if (existParallelPair) return true;

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////

	//A의 X 축 B 의 X 축에 대한 외적의 충돌 체크
	r = abs(dist[2] * cos[1][0] - dist[1] * cos[2][0]);
	r0 = obbA.halfLength[1] * absCos[2][0] + obbA.halfLength[2] * absCos[1][0];
	r1 = obbB.halfLength[1] * absCos[0][2] + obbB.halfLength[2] * absCos[0][1];
	if (r > r0 + r1)
		return false;

	//A의 X 축 B 의 Y 축에 대한 외적의 충돌 체크
	r = abs(dist[2] * cos[1][1] - dist[1] * cos[2][1]);
	r0 = obbA.halfLength[1] * absCos[2][1] + obbA.halfLength[2] * absCos[1][1];
	r1 = obbB.halfLength[0] * absCos[0][2] + obbB.halfLength[2] * absCos[0][0];
	if (r > r0 + r1)
		return false;

	//A의 X 축 B 의 Z 축에 대한 외적의 충돌 체크
	r = abs(dist[2] * cos[1][2] - dist[1] * cos[2][2]);
	r0 = obbA.halfLength[1] * absCos[2][2] + obbA.halfLength[2] * absCos[1][2];
	r1 = obbB.halfLength[0] * absCos[0][1] + obbB.halfLength[1] * absCos[0][0];
	if (r > r0 + r1)
		return false;

	/////////////////////////////////////////////////////////////////

	//A의 Y 축 B 의 X 축에 대한 외적의 충돌 체크
	r = abs(dist[0] * cos[2][0] - dist[2] * cos[0][0]);
	r0 = obbA.halfLength[0] * absCos[2][0] + obbA.halfLength[2] * absCos[0][0];
	r1 = obbB.halfLength[1] * absCos[1][2] + obbB.halfLength[2] * absCos[1][1];
	if (r > r0 + r1)
		return false;

	//A의 Y 축 B 의 Y 축에 대한 외적의 충돌 체크
	r = abs(dist[0] * cos[2][1] - dist[2] * cos[0][1]);
	r0 = obbA.halfLength[0] * absCos[2][1] + obbA.halfLength[2] * absCos[0][1];
	r1 = obbB.halfLength[0] * absCos[1][2] + obbB.halfLength[2] * absCos[1][0];
	if (r > r0 + r1)
		return false;

	//A의 Y 축 B 의 Z 축에 대한 외적의 충돌 체크
	r = abs(dist[0] * cos[2][2] - dist[2] * cos[0][2]);
	r0 = obbA.halfLength[0] * absCos[2][2] + obbA.halfLength[2] * absCos[0][2];
	r1 = obbB.halfLength[0] * absCos[1][1] + obbB.halfLength[1] * absCos[1][0];
	if (r > r0 + r1)
		return false;


	/////////////////////////////////////////////////////////////////	 

	//A의 Z 축 B 의 X 축에 대한 외적의 충돌 체크
	r = abs(dist[1] * cos[0][0] - dist[0] * cos[1][0]);
	r0 = obbA.halfLength[0] * absCos[1][0] + obbA.halfLength[1] * absCos[0][0];
	r1 = obbB.halfLength[1] * absCos[2][2] + obbB.halfLength[2] * absCos[2][1];
	if (r > r0 + r1)
		return false;

	//A의 Z 축 B 의 Y 축에 대한 외적의 충돌 체크
	r = abs(dist[1] * cos[0][1] - dist[0] * cos[1][1]);
	r0 = obbA.halfLength[0] * absCos[1][1] + obbA.halfLength[1] * absCos[0][1];
	r1 = obbB.halfLength[0] * absCos[2][2] + obbB.halfLength[2] * absCos[2][0];
	if (r > r0 + r1)
		return false;

	//A의 Z 축 B 의 Z 축에 대한 외적의 충돌 체크
	r = abs(dist[1] * cos[0][2] - dist[0] * cos[1][2]);
	r0 = obbA.halfLength[0] * absCos[1][2] + obbA.halfLength[1] * absCos[0][2];
	r1 = obbB.halfLength[0] * absCos[2][1] + obbB.halfLength[1] * absCos[2][0];
	if (r > r0 + r1)
		return false;

	//여기까지 클리어 했다면  당신은 충돌 용자... ( PS 충돌 됐다는 예기다 )
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

//2개의 바운드 스피어에대한 출동하고 겹치지 않게 한다.
//moveFactor 가 0 일수록 겹칩에 대해 B 가 움직인다.
//moveFactor 가 0.5 이면 겹칩에 대해 A 와 B 가 똑같은량으로 움직인다.
//moveFactor 가 1 일수록 겹칩에 대해 A 가 움직인다.
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
	//둘이 충돌되지 않았으면 할필요없다
	if (IsOverlap(pTransA, pBoundA, pTransB, pBoundB) == false)
		return false;

	moveFactor = Clamp01(moveFactor);



	//둘이 부디쳤스니 밀어내야한다...

	//A의 Min Max
	D3DXVECTOR3 minA = pBoundA->localMinPos;
	D3DXVECTOR3 maxA = pBoundA->localMaxPos;

	//B의 Min Max
	D3DXVECTOR3 minB = pBoundB->localMinPos;
	D3DXVECTOR3 maxB = pBoundB->localMaxPos;

	//B 의 로컬 사각 8 점

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


	//A 의 월드 역행렬
	D3DXMATRIXA16 matWorldAInv;
	D3DXMatrixInverse(&matWorldAInv, NULL, &pTransA->GetFinalMatrix());

	//B 의 월드 행렬
	D3DXMATRIXA16 matWorldB = pTransB->GetFinalMatrix();

	//B 월드 만큼 가고 A 의 역으로 다시 움직인 행렬
	D3DXMATRIXA16 mat = matWorldB * matWorldAInv;

	//B pos 에 적용
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	//이이후 Pos 들은 A 대한 B 의 상대적인 위치값들이된다.


	//최종적으로 적용된 B pos 를 가지고 min max 를 갱신 하자
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

	//사각 형 구조체
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

	//겹칩량의 사각형
	fRect rcInter;
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);


	//각축의 겹칩량을 구하고 그중 가장 작은 축으로 B 이동시킨다.
	float interX = rcInter.right - rcInter.left;
	float interY = rcInter.top - rcInter.bottom;
	float interZ = rcInter.front - rcInter.back;
	float minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	//미는 방향
	D3DXVECTOR3 moveDirA(0, 0, 0);

	//밀량
	float moveLengthA = minInter;

	//X 축의 겹칩량이 제일 작다면..
	if (minInter == interX)
	{
		//A 의 왼쪽으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.left, rcA.left))
			moveDirA = -pTransA->GetRight();

		//A 의 오른쪽으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.right, rcA.right))
			moveDirA = pTransA->GetRight();
	}
	//Y 축의 겹칩량이 제일 작다면..
	else if (minInter == interY)
	{
		//A 의 위으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.top, rcA.top))
			moveDirA = pTransA->GetUp();

		//A 의 아래으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.bottom, rcA.bottom))
			moveDirA = -pTransA->GetUp();

	}

	//Z 축의 겹침량이 제일 작다면..
	else if (minInter == interZ)
	{
		//A 의 정면으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.front, rcA.front))
			moveDirA = pTransA->GetForward();

		//A 의 뒤으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.back, rcA.back))
			moveDirA = -pTransA->GetForward();
	}

	//여기까지온다면 밀량과 미는 방향이 계산된다.
	//moveDirA
	//moveLengthA
	//위의 두값은 A 를 가만히 두고 B 를 A 역으로 계산한 값이된다.

	//
	// 아래의 연산은 B 를 가만히 두고 A 를 B 역으로 계산한 값이된다.
	//
	D3DXVECTOR3 moveDirB(0, 0, 0);
	float moveLengthB = 0.0f;

	//A의 Min Max
	minA = pBoundA->localMinPos;
	maxA = pBoundA->localMaxPos;

	//B의 Min Max
	minB = pBoundB->localMinPos;
	maxB = pBoundB->localMaxPos;

	//B 의 로컬 사각 8 점

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

	//B 의 월드 역행렬
	D3DXMATRIXA16 matWorldBInv;
	D3DXMatrixInverse(&matWorldBInv, NULL, &pTransB->GetFinalMatrix());

	//A 의 월드 행렬
	D3DXMATRIXA16 matWorldA = pTransA->GetFinalMatrix();

	//A 월드 만큼 가고 B 의 역으로 다시 움직인 행렬
	mat = matWorldA * matWorldBInv;

	//A pos 에 적용
	for (int i = 0; i < 8; i++)
		D3DXVec3TransformCoord(&pos[i], &pos[i], &mat);

	//최종적으로 적용된 A pos 를 가지고 min max 를 갱신 하자
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

	//겹칩량의 사각형
	rcInter.left = max(rcA.left, rcB.left);
	rcInter.right = min(rcA.right, rcB.right);
	rcInter.bottom = max(rcA.bottom, rcB.bottom);
	rcInter.top = min(rcA.top, rcB.top);
	rcInter.back = max(rcA.back, rcB.back);
	rcInter.front = min(rcA.front, rcB.front);

	//각축의 겹칩량을 구하고 그중 가장 작은 축으로 B 이동시킨다.
	interX = rcInter.right - rcInter.left;
	interY = rcInter.top - rcInter.bottom;
	interZ = rcInter.front - rcInter.back;
	minInter = (interX < interY) ? ((interX < interZ) ? interX : interZ) : ((interY < interZ) ? interY : interZ);

	moveLengthB = minInter;

	//X 축의 겹칩량이 제일 작다면..
	if (minInter == interX)
	{
		//B 의 왼쪽으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.left, rcB.left))
			moveDirB = -pTransB->GetRight();

		//B 의 오른쪽으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.right, rcB.right))
			moveDirB = pTransB->GetRight();
	}
	//Y 축의 겹칩량이 제일 작다면..
	else if (minInter == interY)
	{
		//B 의 위으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.top, rcB.top))
			moveDirB = pTransB->GetUp();

		//B 의 아래으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.bottom, rcB.bottom))
			moveDirB = -pTransB->GetUp();

	}

	//Z 축의 겹침량이 제일 작다면..
	else if (minInter == interZ)
	{
		//B 의 정면으로 밀어야 한다면....
		if (FLOATEQUAL(rcInter.front, rcB.front))
			moveDirB = pTransB->GetForward();

		//B 의 뒤으로 밀어야 한다면....
		else if (FLOATEQUAL(rcInter.back, rcB.back))
			moveDirB = -pTransB->GetForward();
	}


	//여기까지온다면 아래의 4 개의 변수가 계산된 것이다....
	//D3DXVECTOR3 moveDirA( 0, 0, 0 );
	//float moveLengthA = minInter;
	//D3DXVECTOR3 moveDirB( 0, 0, 0 );
	//float moveLengthB = 0.0f;


	//밀량이 작은쪽으로...
	if (moveLengthB > moveLengthA)
	{
		//A 와 B 의 스케일 적용
		//여긴 A 를 가만히 두고 B 를 건들인거다
		//따라서 A 의 스케일이 2 였고 B 의 스케일 이 1 이였다고 가정하였을때
		//A 의 역행렬로 계산되어 A 의 스케일이 1 B 의 스케일이 0.5 로 계산된 길이이다
		//그러므로 각 축의 길이는
		//Ascale 값을 곱하여 계산하는 것이 올바르다.
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
		//A 와 B 의 스케일 적용
		//여긴 B 를 가만히 두고 A 를 건들인거다 /
		//따라서 A 의 스케일이 2 였고 B 의 스케일 이 1 이였다고 가정하였을때
		//B 의 역행렬로 계산되어 A 의 스케일이 2 B 의 스케일이 1 로 계산된 길이이다
		//그러므로 각 축의 길이는 BScale 값을 곱하여 계산하는 것이 올바르다.;

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


//점 3개로 무한 평면을 만든다.
void cPhysicManager::CreatePlane(LPD3DXPLANE	pOutPlane, const D3DXVECTOR3* p0, const D3DXVECTOR3* p1, const D3DXVECTOR3* p2)
{
	//평면의 노말 
	D3DXVECTOR3 normal;
	D3DXVECTOR3 edge1 = *p1 - *p0;
	D3DXVECTOR3 edge2 = *p2 - *p0;
	D3DXVec3Cross(&normal, &edge1, &edge2);
	D3DXVec3Normalize(&normal, &normal);

	//평면의 노말 방향으로 원점까지의 최단 거리
	float dist = -D3DXVec3Dot(&normal, p0);

	//평면값 쓰자
	pOutPlane->a = normal.x;
	pOutPlane->b = normal.y;
	pOutPlane->c = normal.z;
	pOutPlane->d = dist;

	//D3DXPlaneFromPoints(
}

//임의의 한점에서 평면까지의 최단거리
float cPhysicManager::PlaneDot(const LPD3DXPLANE pPlane, const D3DXVECTOR3* point)
{
	//평면의 노말
	D3DXVECTOR3 normal(pPlane->a, pPlane->b, pPlane->c);

	/*
	//평면 상의 임의의 한점???
	D3DXVECTOR3 p = normal * -pPlane->d;

	//평면상의 임의의 한점으로 부터 매개변수로 받은 점까지의 방향벡터
	D3DXVECTOR3 dir = *point - p;

	//내적
	float distance = D3DXVec3Dot( &normal, &dir );
	*/

	float distance = D3DXVec3Dot(&normal, point) + pPlane->d;


	return distance;
}

//반직선과 평면의 충돌 위치
bool cPhysicManager::IntersectRayToPlane(D3DXVECTOR3* pOut, const LPRay pRay, const LPD3DXPLANE pPlane, bool bCheck2Side /*=false*/)
{
	//노말 벡터
	D3DXVECTOR3 normal(pPlane->a, pPlane->b, pPlane->c);

	float dot2 = D3DXVec3Dot(&normal, &pRay->direction);//광선의 바향과 평면의 법선 방햐으이 각차의 cos값

	//그리드를 0,0,0 에서 보면 안 보이는 것과 같은 이치다.
	//반직선과 평면의 각도가 90 라는 얘기는 평행하다는 얘기.
	if (FLOATEQUAL(dot2, 0.f))
	{
		return false;
	}

	//반직선의 시작점에서 평면까지의 최단거리
	float dist = D3DXVec3Dot(&normal, &pRay->origin) + pPlane->d;

	//양면 체크를 안 한다면
	if (bCheck2Side == false)
	{
		//반직선의 시작 위치가 뒤에 있어도 실패
		if (dist < 0.f)
			return false;
	}

	//반직선방향으로 충돌지점까지의 거리
	float t = dist / -dot2;

	//레이 방향이 반대로 되어 있는 경우
	if (t < 0.f)
		return false;

	//충돌 위치
	*pOut = pRay->origin + (pRay->direction * t);

	return true;
}
