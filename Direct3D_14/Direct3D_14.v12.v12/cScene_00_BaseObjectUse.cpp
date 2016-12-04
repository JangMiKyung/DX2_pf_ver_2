#include "stdafx.h"
#include "cScene_00_BaseObjectUse.h"


cScene_00_BaseObjectUse::cScene_00_BaseObjectUse()
{
}


cScene_00_BaseObjectUse::~cScene_00_BaseObjectUse()
{
}

HRESULT cScene_00_BaseObjectUse::Scene_Init()
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);
	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	plane.SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X", &matCorrection));

	//plane 에 카메라 붙여라...
	plane.Transform.AddChild(&this->mainCamera);
	this->mainCamera.SetLocalPosition(0, 5, -10);
	plane.SetActive(true);

	for (int i = 0; i < 10; i++){
		this->enemyPlanes.push_back(new cEnemyPlane());
		this->enemyPlanes[i]->SetActive(true);
		this->enemyPlanes[i]->SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X"));
	}

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
	pLight3->Color = D3DXCOLOR(0, 1, 0, 0);
	pLight3->minRange = 5.0f;
	pLight3->maxRange = 10.0f;
	pLight3->distancePow = 10.0f;
	pLight3->Transform.SetWorldPosition(3, 3, 0);
	pLight3->Intensity = 0.5f;

	this->lights.push_back(pLight1);
	this->lights.push_back(pLight2);
	this->lights.push_back(pLight3);


	return S_OK;
}

void cScene_00_BaseObjectUse::Scene_Release()
{
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);


	for (int i = 0; i < 10; i++)
		SAFE_DELETE(this->enemyPlanes[i]);
}

void cScene_00_BaseObjectUse::Scene_Update(float timeDelta)
{
	plane.Update(timeDelta);

	for (int i = 0; i < 10; i++)
		this->enemyPlanes[i]->Update(timeDelta);
}

void cScene_00_BaseObjectUse::Scene_Render1()
{
	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//셰이더에 라이팅 셋팅
	cXMesh_Static::sStaticMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Static::sStaticMeshEffect->SetInt("LightNum", this->lights.size());

	cXMesh_Static::SetCamera(this->mainCamera);

	plane.Render();

	//빨간색 Player 비행기 정면으로 빨간색라인
	D3DXVECTOR3 pos = plane.Transform.GetWorldPosition();
	D3DXVECTOR3 forward = plane.Transform.GetForward();
	GIZMO_MGR->Line(pos, pos + forward * 30, 0xffff0000);

	for (int i = 0; i < 10; i++)
		this->enemyPlanes[i]->Render();
}