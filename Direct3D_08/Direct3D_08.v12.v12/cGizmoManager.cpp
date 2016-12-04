#include "stdafx.h"
#include "cGizmoManager.h"


cGizmoManager::cGizmoManager()
{
}


cGizmoManager::~cGizmoManager()
{
}

HRESULT cGizmoManager::Init(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	float intervalAngle = (D3DX_PI * 2.0f) / CIRCLEGIZMO_SEGMENTS;

	//circle 을 그리기 위한 정점 위치 셋팅
	for (int i = 0; i <= CIRCLEGIZMO_SEGMENTS; i++)
	{
		float angle = i * intervalAngle;
		m_pCirclePos[i] = D3DXVECTOR3(cos(angle), sin(angle), 0.0f);
	}

	return S_OK;
}

void cGizmoManager::Release()
{

}

//선을 그리는 함수
void cGizmoManager::Line(const D3DXVECTOR3& startPos, const D3DXVECTOR3& endPos, DWORD color /*= 0xff808080*/) const
{
	//이전의 월드 행렬을 기억하자
	D3DXMATRIXA16 matPrevWorld;
	m_pDevice->GetTransform(D3DTS_WORLD, &matPrevWorld);

	//월드 행렬을 초기화
	D3DXMATRIXA16 matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	m_pDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	//이전 라이팅 모드를 기억하자
	DWORD prevLight = 0;
	m_pDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);

	//라이팅 끄자
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//임시 정점 배열 2개
	GIZMOVERTEX vertices[2];
	vertices[0].pos = startPos;
	vertices[0].color = color;
	vertices[1].pos = endPos;
	vertices[1].color = color;

	//그리자
	m_pDevice->SetFVF(GIZMOVERTEX::FVF);
	m_pDevice->DrawPrimitiveUP(
		D3DPT_LINELIST, 1,
		vertices,
		sizeof(GIZMOVERTEX)
		);

	//다시 돌려놓는다
	m_pDevice->SetTransform(D3DTS_WORLD, &matPrevWorld);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, prevLight);

}

//월드 그리드를 그리는 함수
void cGizmoManager::WorldGrid(float cellSize, int cellNum) const
{
	//축의 절반 길이
	float axisLineLength = cellSize * cellNum;

	//X 축 라인
	D3DXVECTOR3 xStart(-axisLineLength, 0, 0);
	D3DXVECTOR3 xEnd(axisLineLength, 0, 0);

	D3DXVECTOR3 yStart(0, -axisLineLength, 0);
	D3DXVECTOR3 yEnd(0, axisLineLength, 0);

	D3DXVECTOR3 zStart(0, 0, -axisLineLength);
	D3DXVECTOR3 zEnd(0, 0, axisLineLength);

	//축 그린다
	Line(xStart, xEnd, 0xffff0000);
	Line(yStart, yEnd, 0xff00ff00);
	Line(zStart, zEnd, 0xff0000ff);

	//축화살표
	D3DXVECTOR3 arrowPos[4];
	arrowPos[0] = D3DXVECTOR3(-1.f, 0.f, -1.f);
	arrowPos[1] = D3DXVECTOR3(0.f, 0.f, 0.f);
	arrowPos[2] = D3DXVECTOR3(0.f, 0.f, 0.f);
	arrowPos[3] = D3DXVECTOR3(1.f, 0.f, -1.f);

	D3DXVECTOR3 arrowVertices[4];

	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, zEnd.x, zEnd.y, zEnd.z);
	D3DXMATRIXA16 matRotation;
	D3DXMATRIXA16 matFinal = matTrans;

	//z변환
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matTrans);
	}

	//그린다
	Line(arrowVertices[0], arrowVertices[1], 0xff0000ff);
	Line(arrowVertices[2], arrowVertices[3], 0xff0000ff);

	//x 축
	D3DXMatrixTranslation(&matTrans, xEnd.x, xEnd.y, xEnd.z);
	D3DXMatrixRotationY(&matRotation, 90 * ONE_RAD);
	matFinal = matRotation * matTrans;

	//x 변환
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);
	}

	//그린다
	Line(arrowVertices[0], arrowVertices[1], 0xffff0000);
	Line(arrowVertices[2], arrowVertices[3], 0xffff0000);

	//y축
	D3DXMatrixTranslation(&matTrans, yEnd.x, yEnd.y, yEnd.z);
	D3DXMatrixRotationX(&matRotation, -90.f * ONE_RAD);
	matFinal = matRotation * matTrans;

	//y 변환
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);

	//그린다
	Line(arrowVertices[0], arrowVertices[1], 0xff00ff00);
	Line(arrowVertices[2], arrowVertices[3], 0xff00ff00);

	//월드 그리드를 그린다
	for (int i = -cellNum; i <= cellNum; i++)
	{
		//0은 넘기자
		if (i == 0) continue;

		D3DXVECTOR3 posStartX(i * cellSize, 0, -axisLineLength);
		D3DXVECTOR3 posEndX(i * cellSize, 0, axisLineLength);

		Line(posStartX, posEndX);

		D3DXVECTOR3 posStartZ(-axisLineLength, 0, i * cellSize);
		D3DXVECTOR3 posEndZ(axisLineLength, 0, i * cellSize);

		Line(posStartZ, posEndZ);
	}
}

//원을 그리는 함수
void cGizmoManager::Circle(const D3DXVECTOR3& centerPos, float radius, const D3DXVECTOR3& axis /*= D3DXVECTOR3(0, 0, 1)*/, DWORD color /*= 0xff808080*/) const
{
	//이전의 월드 행렬을 기억하자.
	D3DXMATRIXA16 matPrevWorld;
	m_pDevice->GetTransform(D3DTS_WORLD, &matPrevWorld);

	//이전 라이팅 모드를 기억
	DWORD prevLight = 0;
	m_pDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);

	//라이팅을 끈다.
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//임시 정점 배열 
	GIZMOVERTEX vertices[CIRCLEGIZMO_SEGMENTS + 1];
	for (int i = 0; i < CIRCLEGIZMO_SEGMENTS + 1; i++)
	{
		vertices[i].pos = m_pCirclePos[i];
		vertices[i].color = color;
	}

	//Circle 에 대한 행렬 준비

	//회전준비
	D3DXVECTOR3 forward = axis;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up(0, 1, 0);
	if (forward == up)
		up = D3DXVECTOR3(0, 0, -1);
	else if (forward == D3DXVECTOR3(0, -1, 0))
		up = D3DXVECTOR3(0, 0, 1);

	D3DXVec3Cross(&right, &up, &forward);
	D3DXVec3Cross(&up, &forward, &right);

	//축 정규화
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&up, &up);

	//축 스케일
	forward = forward * radius;
	right = right * radius;
	up = up * radius;

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);

	memcpy(&matWorld._11, right, sizeof(D3DXVECTOR3));
	memcpy(&matWorld._21, up, sizeof(D3DXVECTOR3));
	memcpy(&matWorld._31, forward, sizeof(D3DXVECTOR3));
	memcpy(&matWorld._41, centerPos, sizeof(D3DXVECTOR3));

	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	//그린다.
	m_pDevice->SetFVF(GIZMOVERTEX::FVF);
	m_pDevice->DrawPrimitiveUP(
		D3DPT_LINESTRIP,			//D3DPT_LINESTRIP 을 하면 정점에 대해 한붓그리기로 Line 을 그려 재낀다.
		CIRCLEGIZMO_SEGMENTS,
		vertices,
		sizeof(GIZMOVERTEX));

	//다시 돌려 놓는다.
	m_pDevice->SetTransform(D3DTS_WORLD, &matPrevWorld);

	//라이팅 모드 돌려 놓는다
	m_pDevice->SetRenderState(D3DRS_LIGHTING, prevLight);
}

//와이어 구를 그리는 함수
void cGizmoManager::WireSphere(const D3DXVECTOR3& centerPos, float radius, DWORD color /*= 0xff808080*/) const
{
	this->Circle(centerPos, radius, D3DXVECTOR3(1, 0, 0), color);
	this->Circle(centerPos, radius, D3DXVECTOR3(0, 1, 0), color);
	this->Circle(centerPos, radius, D3DXVECTOR3(0, 0, 1), color);
}

//월드 AABB BOX 그리는 함수
void cGizmoManager::AABBBox(const D3DXVECTOR3& minPos, const D3DXVECTOR3& maxPos, DWORD color /*= 0xff808080*/) const
{
	this->Line(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), color);
	this->Line(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), color);
	this->Line(D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), color);
	this->Line(D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), D3DXVECTOR3(minPos.x, minPos.y, minPos.z), color);

	this->Line(D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), color);
	this->Line(D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	this->Line(D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
	this->Line(D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), color);

	this->Line(D3DXVECTOR3(minPos.x, minPos.y, minPos.z), D3DXVECTOR3(minPos.x, minPos.y, maxPos.z), color);
	this->Line(D3DXVECTOR3(maxPos.x, minPos.y, minPos.z), D3DXVECTOR3(maxPos.x, minPos.y, maxPos.z), color);
	this->Line(D3DXVECTOR3(maxPos.x, maxPos.y, minPos.z), D3DXVECTOR3(maxPos.x, maxPos.y, maxPos.z), color);
	this->Line(D3DXVECTOR3(minPos.x, maxPos.y, minPos.z), D3DXVECTOR3(minPos.x, maxPos.y, maxPos.z), color);
}