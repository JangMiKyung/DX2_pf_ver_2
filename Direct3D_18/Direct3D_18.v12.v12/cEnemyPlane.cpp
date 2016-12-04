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
	//위치 랜덤
	this->pTransform->SetWorldPosition(RandomFloatRange(-15.0f, 15.0f), 0, 50);
	//속도 랜덤
	this->speed = RandomFloatRange(1.0f, 5.0f);

	//뒤를 봐
	this->pTransform->LookDirection(D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 1, 0));
}

void cEnemyPlane::BaseObjectUpdate(float timeDelta)
{
	//0 미만으로 떨어지면...
	if (this->pTransform->GetWorldPosition().z < 0.0f){
		this->nextGenTime = RandomFloatRange(5, 10);
		this->SetActive(false);
	}
	else{
		//주구장창 앞으로
		this->pTransform->MovePositionSelf(0, 0, this->speed * timeDelta);
	}
}

void cEnemyPlane::BaseObjectNoActiveUpdate(float timeDelta)
{
	this->nextGenTime -= timeDelta;

	if (this->nextGenTime <= 0.0f)
		this->SetActive(true);
}