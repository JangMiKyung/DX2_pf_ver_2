#include "stdafx.h"
#include "cEnemyPlane.h"
#include "cTransform.h"

cEnemyPlane::cEnemyPlane()
{
}


cEnemyPlane::~cEnemyPlane()
{
}

void cEnemyPlane::BaseObjectEnable()
{
	//��ġ ����
	this->pTransform->SetWorldPosition(RandomFloatRange(-15.0f, 15.0f), 0, 50);
	//�ӵ� ����
	this->speed = RandomFloatRange(1.0f, 5.0f);

	//�ڸ� ��
	this->pTransform->LookDirection(D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 1, 0));
}

void cEnemyPlane::BaseObjectUpdate(float timeDelta)
{
	//0 �̸����� ��������...
	if (this->pTransform->GetWorldPosition().z < 0.0f){
		this->nextGenTime = RandomFloatRange(5, 10);
		this->SetActive(false);
	}
	else{
		//�ֱ���â ������
		this->pTransform->MovePositionSelf(0, 0, this->speed * timeDelta);
	}
}

void cEnemyPlane::BaseObjectNoActiveUpdate(float timeDelta)
{
	this->nextGenTime -= timeDelta;

	if (this->nextGenTime <= 0.0f)
		this->SetActive(true);
}