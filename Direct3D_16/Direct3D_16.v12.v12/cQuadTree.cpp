#include "stdafx.h"
#include "cQuadTree.h"


cQuadTree::cQuadTree()
{
	//차일드 포인터 NULL 처리
	ZeroMemory(m_pChilds, sizeof(cQuadTree*) * 4);

	//센터인덱스 -1 로 초기화
	m_Center = -1;
}


cQuadTree::~cQuadTree()
{
	//자식 안전 지우기
	SAFE_DELETE(m_pChilds[0]);
	SAFE_DELETE(m_pChilds[1]);
	SAFE_DELETE(m_pChilds[2]);
	SAFE_DELETE(m_pChilds[3]);
}

//쿼드 트리 초기화
HRESULT cQuadTree::Init(
	cTerrain::LPTERRAINVERTEX pVertices,			//Terrain 정점 배열
	DWORD nVerNumEdge)								//QuadTree 의 정점 한면의 갯수
{
	//Terrain 정점 데이터 물린다
	m_pTerrainVertices = pVertices;

	//각 코너 인덱스 계산한다.
	m_Corners[CORNER_LT] = 0;
	m_Corners[CORNER_RT] = nVerNumEdge - 1;
	m_Corners[CORNER_LB] = (nVerNumEdge - 1) * nVerNumEdge;
	m_Corners[CORNER_RB] = nVerNumEdge * nVerNumEdge - 1;

	//자식 트리 만든다.
	CreateChildTree();

	return S_OK;

}

//자식 트리 만든다.
void cQuadTree::CreateChildTree()
{
	//중심위치
	//각코너의 포지션 센터 값
	m_CenterPos = (m_pTerrainVertices[m_Corners[0]].pos +
		m_pTerrainVertices[m_Corners[1]].pos +
		m_pTerrainVertices[m_Corners[2]].pos +
		m_pTerrainVertices[m_Corners[3]].pos) * 0.25f;

	//아무점 하나의 거리가 반지름이다.
	D3DXVECTOR3 dir = m_pTerrainVertices[m_Corners[0]].pos - m_CenterPos;
	float prevLengthSq = D3DXVec3LengthSq(&dir);
	m_Radius = sqrt(prevLengthSq);

	//자식이 있다....
	if ((m_Corners[CORNER_RT] - m_Corners[CORNER_LT]) > 1)
	{
		//센터 인덱스 계산
		m_Center = (m_Corners[0] + m_Corners[1] + m_Corners[2] + m_Corners[3]) / 4;

		DWORD topCenter = (m_Corners[CORNER_RT] + m_Corners[CORNER_LT]) / 2;		//상단 중앙
		DWORD leftCenter = (m_Corners[CORNER_LT] + m_Corners[CORNER_LB]) / 2;		//좌 중앙
		DWORD rightCenter = (m_Corners[CORNER_RT] + m_Corners[CORNER_RB]) / 2;	//우 중앙
		DWORD bottomCenter = (m_Corners[CORNER_RB] + m_Corners[CORNER_LB]) / 2;	//우 중앙


		//좌상단 자식
		m_pChilds[CORNER_LT] = new cQuadTree;
		m_pChilds[CORNER_LT]->m_Corners[CORNER_LT] = m_Corners[CORNER_LT];
		m_pChilds[CORNER_LT]->m_Corners[CORNER_RT] = topCenter;
		m_pChilds[CORNER_LT]->m_Corners[CORNER_LB] = leftCenter;
		m_pChilds[CORNER_LT]->m_Corners[CORNER_RB] = m_Center;
		m_pChilds[CORNER_LT]->m_pTerrainVertices = m_pTerrainVertices;
		m_pChilds[CORNER_LT]->CreateChildTree();


		//우상단 자식
		m_pChilds[CORNER_RT] = new cQuadTree;
		m_pChilds[CORNER_RT]->m_Corners[CORNER_LT] = topCenter;
		m_pChilds[CORNER_RT]->m_Corners[CORNER_RT] = m_Corners[CORNER_RT];
		m_pChilds[CORNER_RT]->m_Corners[CORNER_LB] = m_Center;
		m_pChilds[CORNER_RT]->m_Corners[CORNER_RB] = rightCenter;
		m_pChilds[CORNER_RT]->m_pTerrainVertices = m_pTerrainVertices;
		m_pChilds[CORNER_RT]->CreateChildTree();


		//좌하단 자식
		m_pChilds[CORNER_LB] = new cQuadTree;
		m_pChilds[CORNER_LB]->m_Corners[CORNER_LT] = leftCenter;
		m_pChilds[CORNER_LB]->m_Corners[CORNER_RT] = m_Center;
		m_pChilds[CORNER_LB]->m_Corners[CORNER_LB] = m_Corners[CORNER_LB];
		m_pChilds[CORNER_LB]->m_Corners[CORNER_RB] = bottomCenter;
		m_pChilds[CORNER_LB]->m_pTerrainVertices = m_pTerrainVertices;
		m_pChilds[CORNER_LB]->CreateChildTree();

		//우하단 자식
		m_pChilds[CORNER_RB] = new cQuadTree;
		m_pChilds[CORNER_RB]->m_Corners[CORNER_LT] = m_Center;
		m_pChilds[CORNER_RB]->m_Corners[CORNER_RT] = rightCenter;
		m_pChilds[CORNER_RB]->m_Corners[CORNER_LB] = bottomCenter;
		m_pChilds[CORNER_RB]->m_Corners[CORNER_RB] = m_Corners[CORNER_RB];
		m_pChilds[CORNER_RB]->m_pTerrainVertices = m_pTerrainVertices;
		m_pChilds[CORNER_RB]->CreateChildTree();

	}

}

//레이 충돌지점을 얻는다.
void cQuadTree::GetRayHits(vector<D3DXVECTOR3>* pHits, const LPRay pRay)
{
	//나의 바운드와 충돌했니?
	if (PHYSICS_MGR->IsRayHitSphere(pRay, &m_CenterPos, this->m_Radius, NULL, NULL))
	{
		//자식이잇니?
		if ((m_Corners[CORNER_RT] - m_Corners[CORNER_LT]) > 1)
		{
			//자식 재귀
			for (int i = 0; i < 4; i++)
			{
				m_pChilds[i]->GetRayHits(pHits, pRay);
			}
		}

		//자식이 없는 맨마지막 노드니?
		else
		{
			//Tri 2개 얻는다.

			// lt-----rt
			//  |    /|
			//  |   / |
			//  |  /  |
			//  | /   |
			//  |/    |
			// lb-----rb


			//나의 폴리곤과 Ray 의 Hit 지점을 얻자
			float dist = 0.0f;



			D3DXVECTOR3 lt = this->m_pTerrainVertices[m_Corners[CORNER_LT]].pos;
			D3DXVECTOR3 rt = this->m_pTerrainVertices[m_Corners[CORNER_RT]].pos;
			D3DXVECTOR3 lb = this->m_pTerrainVertices[m_Corners[CORNER_LB]].pos;
			D3DXVECTOR3 rb = this->m_pTerrainVertices[m_Corners[CORNER_RB]].pos;

			//좌상단 폴리곤 체크
			if (D3DXIntersectTri(
				&lt, &rt, &lb,			//폴리곤 정점 3 위치 ( 두루기 순서 주의 )
				&pRay->origin,		//레이 위치
				&pRay->direction,		//레이 방향
				NULL, NULL,
				&dist			//히트가 되었다면 Origin 에서 부터 hit 위치까지의 거리가 나온다.
				))
			{
				//히트지점
				D3DXVECTOR3 hitPos = pRay->origin + pRay->direction * dist;

				//푸쉬
				pHits->push_back(hitPos);

				return;
			}



			//우하단 폴리곤 체크
			if (D3DXIntersectTri(
				&lb, &rt, &rb,			//폴리곤 정점 3 위치
				&pRay->origin,		//레이 위치
				&pRay->direction,		//레이 방향
				NULL, NULL,
				&dist			//히트가 되었다면 Origin 에서 부터 hit 위치까지의 거리가 나온다.
				))
			{
				//히트지점
				D3DXVECTOR3 hitPos = pRay->origin + pRay->direction * dist;

				//푸쉬
				pHits->push_back(hitPos);

				return;
			}
		}
	}
}