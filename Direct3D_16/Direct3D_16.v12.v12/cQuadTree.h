#pragma once

#include "cTerrain.h"

class cQuadTree
{
private:
	enum CORNER { CORNER_LT = 0, CORNER_RT, CORNER_LB, CORNER_RB };		//�� �ڳ� �� �ε��� ���
	cTerrain::LPTERRAINVERTEX		m_pTerrainVertices;					//�ڽ��� �����ִ� �ͷ����� ���� ����

	DWORD		m_Corners[4];			//�ڽ��� ���� Ʈ���� ���ڳ� ���� �ε���
	DWORD		m_Center;				//�ڽ��� ���� Ʈ���� �߾� ���� �ε���	( �̰� -1 �̶�� �ڽľ��� )
	cQuadTree*	m_pChilds[4];			//�ڽ��� �ڽ� ���� Ʈ��	( �ڽ��� ������ ����� �� NULL )


	D3DXVECTOR3 m_CenterPos;			//�ڽ��� ����Ʈ�� �߽� ��ġ
	float		m_Radius;				//�ڽ��� ����Ʈ�� ���� ������

public:
	cQuadTree();
	~cQuadTree();

	//���� Ʈ�� �ʱ�ȭ
	HRESULT Init(
		cTerrain::LPTERRAINVERTEX pVertices,		//Terrain ���� �迭
		DWORD nVerNumEdge							//QuadTree �� ���� �Ѹ��� ����
		);

	//�ڽ� Ʈ�� �����.
	void CreateChildTree();

	//���� �浹������ ��´�.
	void GetRayHits(vector<D3DXVECTOR3>* pHits, const LPRay pRay);
};

