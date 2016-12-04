#include "stdafx.h"
#include "cScene_00_Sprite.h"
#include "cTerrain.h"
#include "cCamera.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"

cScene_00_Sprite::cScene_00_Sprite()
{
}


cScene_00_Sprite::~cScene_00_Sprite()
{
}

HRESULT cScene_00_Sprite::Scene_Init()
{
	D3DXMATRIXA16 matCorrection;
	D3DXMatrixScaling(&matCorrection, 0.1f, 0.1f, 0.1f);

	cXMesh_Skinned* pSkinned = RESOURCE_SKINNEDXMESH->GetResource("../../Resources/Meshes/NamJa/NamJa.X", &matCorrection);

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

	this->pTex = RESOURCE_TEXTURE->GetResource("../../Resources/Testures/Sample_0.JPG");


	return S_OK;
}

void cScene_00_Sprite::Scene_Release()
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

void cScene_00_Sprite::Scene_Update(float timeDelta)
{
	this->pSkinned1->Update(timeDelta);
	this->pSkinned2->Update(timeDelta);


}

void cScene_00_Sprite::Scene_Render1()
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

	this->pBox1->RenderGizmo(this->pBoxTrans1);
	this->pBox1->RenderGizmo(this->pBoxTrans2);
}


void cScene_00_Sprite::Scene_RenderSprite()
{
	RECT rc = { 0, 0, 740, 492 };
	RECT rc2 = { 0, 0, GetMousePos().x, GetMousePos().y };

	//걍 출력
	/*SPRITE_MGR->DrawTexture(
	this->pTex,
	&rc,
	GetMousePos().x, GetMousePos().y,
	0x80800000,
	&D3DXVECTOR3( 50, 50, 0 ) );*/
	
	//이미지 회전
	/*static float angle = 0.0f;
	angle += ONE_RAD * 90.0f * TIME_MGR->GetFrameDeltaSec();
	SPRITE_MGR->DrawTexture(
		this->pTex,
		&rc,
		GetMousePos().x, GetMousePos().y,
		1, 1,
		angle,
		0x80800000,
		&D3DXVECTOR3(0, 0, 0));*/

	//고무줄
	/*SPRITE_MGR->DrawArea(
		this->pTex,
		&rc, &rc2);*/

	//창
	/*RECT rcCente = { 10, 10, 90, 90 };
	SPRITE_MGR->DrawNinePatch(
		this->pTex,
		&rc, &rc2, &rcCente, 0xffffffff, false);*/

	//void DrawLoop(
	//	LPDIRECT3DTEXTURE9 pTex,
	//	float offsetX, float offsetY,		//SourOffset 
	//	LPRECT pSour,						//그릴 소스 영역
	//	LPRECT pDest,						//그릴 화면 영역
	//	DWORD color = 0xffffffff			//섞이는 색상 
	//	);

	float static ox = 0.0f;
	float static oy = 0.0f;
	ox -= TIME_MGR->GetFrameDeltaSec() * 100.0;
	oy -= TIME_MGR->GetFrameDeltaSec() * 100.0;
	SPRITE_MGR->DrawLoop(
		this->pTex,
		ox, oy,
		&rc, &rc2, 0xffffffff);
}