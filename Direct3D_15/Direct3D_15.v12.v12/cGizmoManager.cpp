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

	//circle �� �׸��� ���� ���� ��ġ ����
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

//���� �׸��� �Լ�
void cGizmoManager::Line(const D3DXVECTOR3& startPos, const D3DXVECTOR3& endPos, DWORD color /*= 0xff808080*/) const
{
	//������ ���� ����� �������
	D3DXMATRIXA16 matPrevWorld;
	m_pDevice->GetTransform(D3DTS_WORLD, &matPrevWorld);

	//���� ����� �ʱ�ȭ
	D3DXMATRIXA16 matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	m_pDevice->SetTransform(D3DTS_WORLD, &matIdentity);

	//���� ������ ��带 �������
	DWORD prevLight = 0;
	m_pDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);

	//������ ����
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�ӽ� ���� �迭 2��
	GIZMOVERTEX vertices[2];
	vertices[0].pos = startPos;
	vertices[0].color = color;
	vertices[1].pos = endPos;
	vertices[1].color = color;

	//�׸���
	m_pDevice->SetFVF(GIZMOVERTEX::FVF);
	m_pDevice->DrawPrimitiveUP(
		D3DPT_LINELIST, 1,
		vertices,
		sizeof(GIZMOVERTEX)
		);

	//�ٽ� �������´�
	m_pDevice->SetTransform(D3DTS_WORLD, &matPrevWorld);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, prevLight);

}

//���� �׸��带 �׸��� �Լ�
void cGizmoManager::WorldGrid(float cellSize, int cellNum) const
{
	//���� ���� ����
	float axisLineLength = cellSize * cellNum;

	//X �� ����
	D3DXVECTOR3 xStart(-axisLineLength, 0, 0);
	D3DXVECTOR3 xEnd(axisLineLength, 0, 0);

	D3DXVECTOR3 yStart(0, -axisLineLength, 0);
	D3DXVECTOR3 yEnd(0, axisLineLength, 0);

	D3DXVECTOR3 zStart(0, 0, -axisLineLength);
	D3DXVECTOR3 zEnd(0, 0, axisLineLength);

	//�� �׸���
	Line(xStart, xEnd, 0xffff0000);
	Line(yStart, yEnd, 0xff00ff00);
	Line(zStart, zEnd, 0xff0000ff);

	//��ȭ��ǥ
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

	//z��ȯ
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matTrans);
	}

	//�׸���
	Line(arrowVertices[0], arrowVertices[1], 0xff0000ff);
	Line(arrowVertices[2], arrowVertices[3], 0xff0000ff);

	//x ��
	D3DXMatrixTranslation(&matTrans, xEnd.x, xEnd.y, xEnd.z);
	D3DXMatrixRotationY(&matRotation, 90 * ONE_RAD);
	matFinal = matRotation * matTrans;

	//x ��ȯ
	for (int i = 0; i < 4; i++)
	{
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);
	}

	//�׸���
	Line(arrowVertices[0], arrowVertices[1], 0xffff0000);
	Line(arrowVertices[2], arrowVertices[3], 0xffff0000);

	//y��
	D3DXMatrixTranslation(&matTrans, yEnd.x, yEnd.y, yEnd.z);
	D3DXMatrixRotationX(&matRotation, -90.f * ONE_RAD);
	matFinal = matRotation * matTrans;

	//y ��ȯ
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&arrowVertices[i], &arrowPos[i], &matFinal);

	//�׸���
	Line(arrowVertices[0], arrowVertices[1], 0xff00ff00);
	Line(arrowVertices[2], arrowVertices[3], 0xff00ff00);

	//���� �׸��带 �׸���
	for (int i = -cellNum; i <= cellNum; i++)
	{
		//0�� �ѱ���
		if (i == 0) continue;

		D3DXVECTOR3 posStartX(i * cellSize, 0, -axisLineLength);
		D3DXVECTOR3 posEndX(i * cellSize, 0, axisLineLength);

		Line(posStartX, posEndX);

		D3DXVECTOR3 posStartZ(-axisLineLength, 0, i * cellSize);
		D3DXVECTOR3 posEndZ(axisLineLength, 0, i * cellSize);

		Line(posStartZ, posEndZ);
	}
}

//���� �׸��� �Լ�
void cGizmoManager::Circle(const D3DXVECTOR3& centerPos, float radius, const D3DXVECTOR3& axis /*= D3DXVECTOR3(0, 0, 1)*/, DWORD color /*= 0xff808080*/) const
{
	//������ ���� ����� �������.
	D3DXMATRIXA16 matPrevWorld;
	m_pDevice->GetTransform(D3DTS_WORLD, &matPrevWorld);

	//���� ������ ��带 ���
	DWORD prevLight = 0;
	m_pDevice->GetRenderState(D3DRS_LIGHTING, &prevLight);

	//�������� ����.
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�ӽ� ���� �迭 
	GIZMOVERTEX vertices[CIRCLEGIZMO_SEGMENTS + 1];
	for (int i = 0; i < CIRCLEGIZMO_SEGMENTS + 1; i++)
	{
		vertices[i].pos = m_pCirclePos[i];
		vertices[i].color = color;
	}

	//Circle �� ���� ��� �غ�

	//ȸ���غ�
	D3DXVECTOR3 forward = axis;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up(0, 1, 0);
	if (forward == up)
		up = D3DXVECTOR3(0, 0, -1);
	else if (forward == D3DXVECTOR3(0, -1, 0))
		up = D3DXVECTOR3(0, 0, 1);

	D3DXVec3Cross(&right, &up, &forward);
	D3DXVec3Cross(&up, &forward, &right);

	//�� ����ȭ
	D3DXVec3Normalize(&forward, &forward);
	D3DXVec3Normalize(&right, &right);
	D3DXVec3Normalize(&up, &up);

	//�� ������
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

	//�׸���.
	m_pDevice->SetFVF(GIZMOVERTEX::FVF);
	m_pDevice->DrawPrimitiveUP(
		D3DPT_LINESTRIP,			//D3DPT_LINESTRIP �� �ϸ� ������ ���� �Ѻױ׸���� Line �� �׷� �糤��.
		CIRCLEGIZMO_SEGMENTS,
		vertices,
		sizeof(GIZMOVERTEX));

	//�ٽ� ���� ���´�.
	m_pDevice->SetTransform(D3DTS_WORLD, &matPrevWorld);

	//������ ��� ���� ���´�
	m_pDevice->SetRenderState(D3DRS_LIGHTING, prevLight);
}

//���̾� ���� �׸��� �Լ�
void cGizmoManager::WireSphere(const D3DXVECTOR3& centerPos, float radius, DWORD color /*= 0xff808080*/) const
{
	this->Circle(centerPos, radius, D3DXVECTOR3(1, 0, 0), color);
	this->Circle(centerPos, radius, D3DXVECTOR3(0, 1, 0), color);
	this->Circle(centerPos, radius, D3DXVECTOR3(0, 0, 1), color);
}

//���� AABB BOX �׸��� �Լ�
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