#pragma once

#include "cSingletonBase.h"

#define CIRCLEGIZMO_SEGMENTS 36

class cGizmoManager : public cSingletonBase<cGizmoManager>
{
private:
	typedef struct tagGIZMOVERTEX
	{
		D3DXVECTOR3 pos;
		DWORD color;
		enum{FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE};
	}GIZMOVERTEX, *LPGIZMOVERTEX;

private:
	LPDIRECT3DDEVICE9 m_pDevice;		//기존에 있는 디바이스
	D3DXVECTOR3 m_pCirclePos[CIRCLEGIZMO_SEGMENTS + 1];//circle의 정점 위치


public:
	cGizmoManager();
	~cGizmoManager();

	HRESULT Init(LPDIRECT3DDEVICE9 pDevice);
	void Release();

	//선을 그리는 함수
	void Line(const D3DXVECTOR3& startPos, const D3DXVECTOR3& endPos, DWORD color = 0xff808080) const;

	//월드 그리드를 그리는 함수
	void WorldGrid(float cellSize, int cellNum) const;

	//원을 그리는 함수
	void Circle(const D3DXVECTOR3& centerPos, float radius, const D3DXVECTOR3& axis = D3DXVECTOR3(0, 0, 1), DWORD color = 0xff808080) const;

	//와이어 구를 그리는 함수
	void WireSphere(const D3DXVECTOR3& centerPos, float radius, DWORD color = 0xff808080) const;

	//월드 AABB BOX 그리는 함수
	void AABBBox(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos, DWORD color = 0xff808080) const;
};

#define GIZMO_MGR cGizmoManager::GetInstance()