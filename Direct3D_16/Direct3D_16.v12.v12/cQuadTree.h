#pragma once

#include "cTerrain.h"

class cQuadTree
{
private:
	enum CORNER { CORNER_LT = 0, CORNER_RT, CORNER_LB, CORNER_RB };		//각 코너 별 인덱스 상수
	cTerrain::LPTERRAINVERTEX		m_pTerrainVertices;					//자신이 물고있는 터레인의 정점 정보

	DWORD		m_Corners[4];			//자신의 쿼드 트리의 각코너 정점 인덱스
	DWORD		m_Center;				//자신의 쿼드 트리의 중앙 정점 인덱스	( 이게 -1 이라면 자식없다 )
	cQuadTree*	m_pChilds[4];			//자신의 자식 쿼드 트리	( 자신이 마지막 노드라면 다 NULL )


	D3DXVECTOR3 m_CenterPos;			//자신의 쿼드트리 중심 위치
	float		m_Radius;				//자신의 쿼드트리 영역 반지름

public:
	cQuadTree();
	~cQuadTree();

	//쿼드 트리 초기화
	HRESULT Init(
		cTerrain::LPTERRAINVERTEX pVertices,		//Terrain 정점 배열
		DWORD nVerNumEdge							//QuadTree 의 정점 한면의 갯수
		);

	//자식 트리 만든다.
	void CreateChildTree();

	//레이 충돌지점을 얻는다.
	void GetRayHits(vector<D3DXVECTOR3>* pHits, const LPRay pRay);
};

