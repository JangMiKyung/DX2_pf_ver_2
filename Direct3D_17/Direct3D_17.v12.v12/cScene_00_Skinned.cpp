#include "stdafx.h"
#include "cScene_00_Skinned.h"
#include "cTerrain.h"
#include "cCamera.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"

cScene_00_Skinned::cScene_00_Skinned()
{
}


cScene_00_Skinned::~cScene_00_Skinned()
{
}

HRESULT cScene_00_Skinned::Scene_Init()
{
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.1f, 0.1f, 0.1f);

	cXMesh_Skinned* pSkinned = RESOURCE_SKINNEDXMESH->
		GetResource("../../Resources/Meshes/Witch/WitchApprentice.X",
		&matCorrection);

	//위에서 로딩괸 SkinnedMesh 인스턴스를 만든다.
	this->pSkinned1 = new cSkinnedAnimation();
	this->pSkinned2 = new cSkinnedAnimation();
	this->pSkinned1->Init(pSkinned);
	this->pSkinned2->Init(pSkinned);

	this->pSkinnedTrans = new cTransform();
	this->pSkinnedTrans2 = new cTransform();
	this->pSkinnedTrans2->SetWorldPosition(5, 0, 0);


	this->pBoxTrans1 = new cTransform();
	this->pBoxTrans2 = new cTransform();
	this->pBox1 = new cBoundBox();
	this->pBox1->SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 1, 1));

	this->pSkinned2->AddBoneTransform("Bip01-HeadNub", pBoxTrans1);
	this->pSkinned1->AddBoneTransform("Bip01-R-Hand", pBoxTrans2);

	this->pApplyTrans = new cTransform();
	this->pSkinned1->AddApplyTransform( "Bip01-Spine1", this->pApplyTrans );


	//라이트 푸쉬
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 0.1f;

	cLight_Point* pLight2 = new cLight_Point();
	pLight2->Color = D3DXCOLOR(1, 0, 0, 0);
	pLight2->minRange = 5.0f;
	pLight2->maxRange = 10.0f;
	pLight2->distancePow = 10.0f;

	cLight_Point* pLight3 = new cLight_Point();
	pLight3->Color = D3DXCOLOR(1, 1, 1, 0);
	pLight3->minRange = 5.0f;
	pLight3->maxRange = 10.0f;
	pLight3->distancePow = 10.0f;
	pLight3->pTransform->SetWorldPosition(3, 3, 0);
	pLight3->Intensity = 0.5f;

	this->lights.push_back(pLight1);
	this->lights.push_back(pLight2);
	this->lights.push_back(pLight3);

	return S_OK;
}

void cScene_00_Skinned::Scene_Release()
{
	SAFE_DELETE(this->pSkinnedTrans2);
	SAFE_DELETE(this->pSkinnedTrans);

	this->pSkinned1->Release();
	this->pSkinned2->Release();

	SAFE_DELETE(this->pSkinned1);
	SAFE_DELETE(this->pSkinned2);
	SAFE_DELETE(this->pBoxTrans1);
	SAFE_DELETE(this->pBox1);

}

void cScene_00_Skinned::Scene_Update(float timDelta)
{
	this->pSkinned1->Update(timDelta);
	this->pSkinned2->Update(timDelta);



	if (KEY_MGR->IsStayDown(VK_LCONTROL))
	{

		if (KEY_MGR->IsOnceDown('1'))
			this->pSkinned1->Play("Idle", 0.3f);

		if (KEY_MGR->IsOnceDown('2'))
			this->pSkinned1->Play("Walk", 0.3f);

		if (KEY_MGR->IsOnceDown('3'))
			this->pSkinned1->Play("run", 0.3f);

		if (KEY_MGR->IsOnceDown(VK_SPACE))
			this->pSkinned1->PlayOneShot("Atk01", 0.3, 0.3);

		if (KEY_MGR->IsOnceDown('5'))
			this->pSkinned1->PlayOneShotAfterHold("Death", 0.3);

	}

	else
	{

		if (KEY_MGR->IsOnceDown('1'))
			this->pSkinned2->Play("Idle", 0.3f);

		if (KEY_MGR->IsOnceDown('2'))
			this->pSkinned2->Play("Walk", 0.3f);

		if (KEY_MGR->IsOnceDown('3'))
			this->pSkinned2->Play("run", 0.3f);

		if (KEY_MGR->IsOnceDown(VK_SPACE))
			this->pSkinned2->PlayOneShot("Atk01", 0.3, 0.3);

		if (KEY_MGR->IsOnceDown('5'))
			this->pSkinned2->PlayOneShotAfterHold("Death", 0.3);

	}


	//this->pApplyTrans->DefaultControl2( timDelta );



	
	static float speed = 1.0f;

	if( KEY_MGR->IsStayDown( VK_UP ) ){
	speed += timDelta;
	if( speed >= 5.0f )
	speed = 5.0f;
	}

	else if( KEY_MGR->IsStayDown( VK_DOWN ) ){
	speed -= timDelta;
	if( speed <= -5.0f )
	speed = -5.0f;
	}

	//this->pSkinnedMesh->SetPlaySpeed( speed );
	

}

void cScene_00_Skinned::Scene_Render1()
{
	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//셰이더에 라이팅 셋팅
	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Skinned::sSkinnedMeshEffect->SetInt("LightNum", this->lights.size());



	//SkinnedEffect CameraMatrix 적용
	//D3DXMATRIXA16 matViewProjection = pCamera->GetViewProjectionMatrix();
	//m_pSkinnedEffect->SetMatrix( "matViewProjection", &matViewProjection );

	cXMesh_Skinned::SetCamera(this->pMainCamera);
	this->pSkinned1->Render(pSkinnedTrans);
	this->pSkinned2->Render(pSkinnedTrans2);

	//this->pSkinned1->RenderBoneName( this->pMainCamera, pSkinnedTrans );
	//this->pSkinned2->RenderBoneName( this->pMainCamera, pSkinnedTrans );


	//가지고 있는 Animation 을출력해보자..
//	RESOURCE_SKINNEDXMESH->GetResource("../../Resources/Meshes/Witch/WitchApprentice.X")->ShowAnimationName(0, 100);

}
