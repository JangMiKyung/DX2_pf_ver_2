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

	//* 메쉬 생성한다.
	cXMesh_Skinned* pSkinned = RESOURCE_SKINNEDXMESH->GetResource("../../Resources/Meshes/Miles/miles_test.X", &matCorrection);

	
	this->pSkinnedAni1 = new cSkinnedAnimation();

	this->pSkinnedAni1->Init(pSkinned);
	
	this->pSkinnedTrans = new cTransform();
	
	this->pSkinnedTrans->SetWorldPosition(0, 0, 0);

	/* 박스 */
	this->pBoxTrans1 = new cTransform();
	this->pBoxTrans2 = new cTransform();
	this->pBox1 = new cBoundBox();
	this->pBox1->SetBound(&D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(1, 1, 1));

	this->pSkinnedAni1->AddBoneTransform("Hero-Camera", pBoxTrans1);

	this->pApplyTrans = new cTransform();
	this->pSkinnedAni1->AddApplyTransform( "Hero-Root", this->pApplyTrans );


	//라이트///////////////////////////////////////////////////////////////////////////////
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 0.1f;

	cLight_Point* pLight2 = new cLight_Point();
	pLight2->Color = D3DXCOLOR(1, 1, 1, 0);
	pLight2->minRange = 5.0f;
	pLight2->maxRange = 10.0f;
	pLight2->distancePow = 10.0f;
	pLight2->pTransform->SetWorldPosition(pSkinnedTrans->GetWorldPosition());
	pLight2->Intensity = 0.5f;

	this->lights.push_back(pLight1);
	this->lights.push_back(pLight2);



	return S_OK;
}

void cScene_00_Skinned::Scene_Release()
{
	SAFE_DELETE(this->pSkinnedTrans2);
	SAFE_DELETE(this->pSkinnedTrans);

	this->pSkinnedAni1->Release();
	this->pSkinnedAni2->Release();

	SAFE_DELETE(this->pSkinnedAni1);
	SAFE_DELETE(this->pSkinnedAni2);
	SAFE_DELETE(this->pBoxTrans1);
	SAFE_DELETE(this->pBox1);
}

void cScene_00_Skinned::Scene_Update(float timDelta)
{
	this->pSkinnedAni1->Update(timDelta);

	lights[1]->pTransform->SetWorldPosition(pSkinnedTrans->GetWorldPosition());

	headPos = pBoxTrans1->GetWorldPosition();

	///headPos.y -= 0.4f;


	if (KEY_MGR->IsOnceDown('1'))
		this->pSkinnedAni1->Play("stand_idle ", 0.3f);

	if (KEY_MGR->IsOnceDown('2'))
		this->pSkinnedAni1->Play("run_forward", 0.3f);

	if (KEY_MGR->IsOnceDown('3'))
		this->pSkinnedAni1->Play("run", 0.3f);

	if (KEY_MGR->IsOnceDown(VK_SPACE))
		this->pSkinnedAni1->PlayOneShot("Atk01", 0.3, 0.3);

	if (KEY_MGR->IsOnceDown('5'))
		this->pSkinnedAni1->PlayOneShotAfterHold("Death", 0.3);


	//c를 누르면 메인 카메라가 비추도록
	if (KEY_MGR->IsOnceDown('c'))
		pMainCamera->SetWorldPosition(10.0f, 10.0f, -10.0f);
		
//		this->pSkinnedAni1->PlayOneShotAfterHold("Death", 0.3);


//	this->pApplyTrans->DefaultControl2( timDelta );


	static float speed = 3.0f;

	D3DXVECTOR3 temp = pSkinnedTrans->GetWorldPosition();
	if( KEY_MGR->IsStayDown( VK_UP ) ){
		pSkinnedTrans->SetWorldPosition(temp.x, temp.y, temp.z - timDelta * speed);
	}
	else if( KEY_MGR->IsStayDown( VK_DOWN ) ){	
		pSkinnedTrans->SetWorldPosition(temp.x, temp.y, temp.z + timDelta * speed);
	}

	

}

void cScene_00_Skinned::Scene_Render1()
{
	////적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//셰이더에 라이팅 셋팅
	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Skinned::sSkinnedMeshEffect->SetInt("LightNum", this->lights.size());
		
	//SkinnedEffect CameraMatrix 적용
	//D3DXMATRIXA16 matViewProjection = pCamera->GetViewProjectionMatrix();
	//m_pSkinnedEffect->SetMatrix( "matViewProjection", &matViewProjection );

	//* 카메라 설치
	//* cScene 에 있는 mainCamera를 카메라로 설치
	cXMesh_Skinned::SetCamera(this->pMainCamera);
	this->pMainCamera->SetWorldPosition(headPos);

	//* 달리는 skinnedTrans 의 로컬 포지션 얻어옴
	//D3DXVECTOR3 temp = pSkinnedTrans->GetLocalPosition();
	//this->pMainCamera->SetWorldPosition(D3DXVECTOR3(temp.x, temp.y + 10, temp.z));

	//* skinnedAni1 은 pSkinnedTrans에 그린다.
	this->pSkinnedAni1->Render(pSkinnedTrans);
	//* skinnedAni2 는 pSkinnedTrans2에 그린다.
	//this->pSkinnedAni2->Render(pSkinnedTrans2);

	//this->pSkinned1->RenderBoneName( this->pMainCamera, pSkinnedTrans );
	//this->pSkinned2->RenderBoneName( this->pMainCamera, pSkinnedTrans );

	this->pBox1->RenderGizmo(this->pBoxTrans1);
	this->pBox1->RenderGizmo(this->pBoxTrans2);

	//가지고 있는 Animation 을출력해보자..
	RESOURCE_SKINNEDXMESH->GetResource("../../Resources/Meshes/Miles/miles_test.X")->ShowAnimationName(0, 26);

}
