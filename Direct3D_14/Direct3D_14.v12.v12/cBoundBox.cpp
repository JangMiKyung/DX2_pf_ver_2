#include "stdafx.h"
#include "cBoundBox.h"


cBoundBox::cBoundBox()
{
}


cBoundBox::~cBoundBox()
{
}


void cBoundBox::GetWorldBox(const cTransform& cTrans, D3DXVECTOR3* outBoxPos)
{
	//   5-------6
	//  /|      /|
	// 1-------2 |
	// | 4-----|-7
	// |/      |/
	// 0-------3 

	//���� 8 ���� ������ ���Ѵ�
	D3DXVECTOR3 Vertices[8];
	Vertices[0] = D3DXVECTOR3(this->localMinPos.x, this->localMinPos.y, this->localMinPos.z);
	Vertices[1] = D3DXVECTOR3(this->localMinPos.x, this->localMaxPos.y, this->localMinPos.z);
	Vertices[2] = D3DXVECTOR3(this->localMaxPos.x, this->localMaxPos.y, this->localMinPos.z);
	Vertices[3] = D3DXVECTOR3(this->localMaxPos.x, this->localMinPos.y, this->localMinPos.z);
	Vertices[4] = D3DXVECTOR3(this->localMinPos.x, this->localMinPos.y, this->localMaxPos.z);
	Vertices[5] = D3DXVECTOR3(this->localMinPos.x, this->localMaxPos.y, this->localMaxPos.z);
	Vertices[6] = D3DXVECTOR3(this->localMaxPos.x, this->localMaxPos.y, this->localMaxPos.z);
	Vertices[7] = D3DXVECTOR3(this->localMaxPos.x, this->localMinPos.y, this->localMaxPos.z);

	D3DXMATRIXA16 matFinal = cTrans.GetFinalMatrix();
	for (int i = 0; i < 8; i++)
	{
		D3DXVec3TransformCoord(outBoxPos + i, Vertices + i, &matFinal);
	}

}

void cBoundBox::GetWorldAABBMinMax(const cTransform& cTrans, D3DXVECTOR3* min, D3DXVECTOR3* max)
{
	//��ȯ�� �ڽ��� ���� ��ġ 8���� ��´�.
	D3DXVECTOR3 worldPos[8];
	GetWorldBox(cTrans, worldPos);

	//���� 8���� �ִ� �ּҸ� ������
	*min = worldPos[0];
	*max = worldPos[0];

	for (int i = 1; i < 8; i++){

		if (min->x > worldPos[i].x) {
			min->x = worldPos[i].x;
		}
		else if (max->x < worldPos[i].x){
			max->x = worldPos[i].x;
		}

		if (min->y > worldPos[i].y){
			min->y = worldPos[i].y;
		}
		else if (max->y < worldPos[i].y){
			max->y = worldPos[i].y;
		}

		if (min->z > worldPos[i].z){
			min->z = worldPos[i].z;
		}
		else if (max->z < worldPos[i].z){
			max->z = worldPos[i].z;
		}
	}
}

//���� ������ �׷��ش�.
void cBoundBox::RenderGizmo(const cTransform& trans)
{
	//��ȯ�� �ڽ��� ������ġ 8�� ��´�.
	D3DXVECTOR3 worldPos[8];
	GetWorldBox(trans, worldPos);

	//   5-------6
	//  /|      /|
	// 1-------2 |
	// | 4-----|-7
	// |/      |/
	// 0-------3 
	//OBB �ڽ� �׸���
	GIZMO_MGR->Line(worldPos[0], worldPos[1], 0xff00ff00);
	GIZMO_MGR->Line(worldPos[1], worldPos[2], 0xff00ff00);
	GIZMO_MGR->Line(worldPos[2], worldPos[3], 0xff00ff00);
	GIZMO_MGR->Line(worldPos[3], worldPos[0], 0xff00ff00);

	GIZMO_MGR->Line(worldPos[4], worldPos[5], 0xff00ff00);
	GIZMO_MGR->Line(worldPos[5], worldPos[6], 0xff00ff00);
	GIZMO_MGR->Line(worldPos[6], worldPos[7], 0xff00ff00);
	GIZMO_MGR->Line(worldPos[7], worldPos[4], 0xff00ff00);

	GIZMO_MGR->Line(worldPos[0], worldPos[4], 0xff00ff00);
	GIZMO_MGR->Line(worldPos[1], worldPos[5], 0xff00ff00);
	GIZMO_MGR->Line(worldPos[2], worldPos[6], 0xff00ff00);
	GIZMO_MGR->Line(worldPos[3], worldPos[7], 0xff00ff00);

	D3DXVECTOR3 min;
	D3DXVECTOR3 max;

	//���� 8 ���� �ִ� �ּҸ� ������.
	min = worldPos[0];
	max = worldPos[0];

	for (int i = 1; i < 8; i++){

		if (min.x > worldPos[i].x){
			min.x = worldPos[i].x;
		}
		else if (max.x < worldPos[i].x){
			max.x = worldPos[i].x;
		}

		if (min.y > worldPos[i].y){
			min.y = worldPos[i].y;
		}
		else if (max.y < worldPos[i].y){
			max.y = worldPos[i].y;
		}

		if (min.z > worldPos[i].z){
			min.z = worldPos[i].z;
		}
		else if (max.z < worldPos[i].z){
			max.z = worldPos[i].z;
		}
	}

	//AABB �׸���.
	GIZMO_MGR->AABBBox(min, max, 0xff008800);
}