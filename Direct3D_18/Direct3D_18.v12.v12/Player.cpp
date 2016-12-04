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
#include "Command.h"

Player::Player(std::string filePath, D3DXVECTOR3 pos)
{
	m_FilePath = filePath;
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.04f, 0.04f, 0.04f);

	cXMesh_Skinned* pSkinned1 = RESOURCE_SKINNEDXMESH->
		GetResource(filePath,
			&matCorrection);


	//위에서 로딩괸 SkinnedMesh 인스턴스를 만든다.
	this->pSkinnedAni = new cSkinnedAnimation();
	this->pSkinnedAni->Init(pSkinned1);

	this->pSkinnedTrans = new cTransform();
	pSkinnedTrans->SetWorldPosition(pos);


	//대가리 Transform 동적할당
	this->pHeadTrans = new cTransform();
	this->pSkinnedAni->AddBoneTransform("Hero-Camera", pHeadTrans);

	this->input = new InputHandler;
 
	InitAnimation();
	InitLight();

	CollisionSphere = new cBoundSphere;
	CollisionSphere->SetBound(&pSkinnedTrans->GetLocalPosition(), &D3DXVECTOR3(1, 1, 1));
	DetectSphere = new cBoundBox;



}

void Player::Update(float timeDelta)
{
	pSkinnedAni->Update(timeDelta);
	//this->pSkinned->AddBoneTransform("Hero-Camera", pHeadTrans);
	headPos = pHeadTrans->GetWorldPosition();

	PlayerControll(timeDelta);
}

void Player::Release()
{
	SAFE_DELETE(this->pSkinnedTrans);
	this->pSkinnedAni->Release();
	SAFE_DELETE(this->pSkinnedAni);
}

void Player::PlayerControll(float timeDelta)
{
    Command* command = input->HandleInput();
	command->execute(*this, timeDelta);
}


bool Player::checkEnterWalking()
{
	if (KEY_MGR->IsStayDown('A') || KEY_MGR->IsStayDown('D') || KEY_MGR->IsStayDown('W') || KEY_MGR->IsStayDown('S'))
	{
		return true;
	}

	return false;
}

void Player::InitAnimation()
{
	State* state1;
	state1 = new State(0, 0.02, pSkinnedTrans, "STAND_IDLE");
	m_MState.insert( make_pair("STAND_IDLE", state1) );

	State* state2;
	state2 = new State(8, 0.02, pSkinnedTrans, "RUN_FORWARD");
	m_MState.insert( make_pair("RUN_FORWARD", state2) );

	State* state3;
	state3 = new State(5, 0.02, pSkinnedTrans, "WALK_FORWARD");
	m_MState.insert( make_pair("WALK_FORWARD", state3) );

	State* state4;
	state4 = new State(0, 0.02, pSkinnedTrans, "CROUCH_IDLE");
	m_MState.insert(make_pair("CROUCH_IDLE", state4) );

	StatePlayChange("STAND_IDLE", 0.3f);
}

void Player::InitLight()
{
	//라이트 푸쉬
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 0.1f;

	cLight_Point* pLight2 = new cLight_Point();
	pLight2->Color = D3DXCOLOR(1, 1, 1, 0);
	pLight2->minRange = 5.0f;
	pLight2->maxRange = 99.0f;
	pLight2->distancePow = 50.0f;

	this->lights.push_back(pLight1);
	this->lights.push_back(pLight2);

}

void Player::SitDown()
{

	if(!pSkinnedAni->isPlay() && m_Animation_Name == "STAND_TO_CROUCH")
		StatePlayChange("CROUCH_IDLE", 0.3f);
	else
	{
		StateOneShotChange("STAND_TO_CROUCH", 0.3f);
	}
}
