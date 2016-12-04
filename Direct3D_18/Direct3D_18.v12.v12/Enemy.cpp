#include "stdafx.h"
#include "Unit.h"
#include "cTransform.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"
#include "State.h"
#include "cCamera.h"
#include "cNode.h"
#include"cBoundBox.h"
#include "cBoundSphere.h"

PigEnemy::PigEnemy(std::string filePath, D3DXVECTOR3 pos)
{
	m_FilePath = filePath;
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.02f, 0.02f, 0.02f);

	cXMesh_Skinned* pSkinned = RESOURCE_SKINNEDXMESH->
		GetResource(filePath,
			&matCorrection);

	//위에서 로딩괸 SkinnedMesh 인스턴스를 만든다.
	this->pSkinnedAni = new cSkinnedAnimation();
	this->pSkinnedAni->Init(pSkinned);

	this->pSkinnedTrans = new cTransform();
	pSkinnedTrans->SetWorldPosition(pos);

	InitAnimation();
	InitLight();

	CollisionSphere = new cBoundSphere;
	CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(1,1,1));
	DetectSphere = new cBoundBox;

	m_pCurState->setActive(true);

};



void PigEnemy::Update(float timeDelta)
{
	this->pSkinnedAni->Update(timeDelta);


//	this->m_pCurState->Move(timeDelta, { -10,0,-10 }, { 70,0,70 });
}

void PigEnemy::Release()
{
	SAFE_DELETE(this->pSkinnedTrans);
	this->pSkinnedAni->Release();
	SAFE_DELETE(this->pSkinnedAni);
}



void PigEnemy::InitAnimation()
{
	// 이동속도 , 회전속도, Transform 넣을 것.
	State* state1;
	state1 = new State(0, 0.1, pSkinnedTrans, "IDLE");
	m_MState.insert(make_pair("IDLE", state1));

	State* state2;
	state2 = new State(0.04, 0.015, pSkinnedTrans, "WALK");
	m_MState.insert(make_pair("WALK", state2));

	State* state3;
	state3 = new State(0.08, 0.02, pSkinnedTrans, "RUN");
	m_MState.insert(make_pair("RUN", state3));

	State* state4;
	state4 = new State(0.4, 0.2, pSkinnedTrans, "ROTATE");
	m_MState.insert(make_pair("ROTATE", state4));

	StatePlayChange("WALK", 0.3f);
}

void PigEnemy::InitLight()
{
	//라이트 푸쉬
	//라이트 푸쉬
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 0.9f;

	cLight_Point* pLight3 = new cLight_Point();
	pLight3->Color = D3DXCOLOR(1, 1, 1, 0);
	pLight3->minRange = 5.0f;
	pLight3->maxRange = 222.0f;
	pLight3->distancePow = 10.0f;
	pLight3->pTransform->SetWorldPosition(11, 11, 0);
	pLight3->Intensity = 0.7f;

	this->lights.push_back(pLight1);
	this->lights.push_back(pLight3);
}


