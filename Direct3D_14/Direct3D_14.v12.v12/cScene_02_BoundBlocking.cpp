#include "stdafx.h"
#include "cScene_02_BoundBlocking.h"


cScene_02_BoundBlocking::cScene_02_BoundBlocking()
{
}


cScene_02_BoundBlocking::~cScene_02_BoundBlocking()
{
}

HRESULT cScene_02_BoundBlocking::Scene_Init()
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);
	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	objects.SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X", &matCorrection));
	objects.SetActive(true);

	object2.SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X", &matCorrection));
	object2.SetActive(true);

	object2.Transform.SetWorldPosition(D3DXVECTOR3(20, -4, 0));
	object2.Transform.SetScale(D3DXVECTOR3(2, 10, 10));

	object3.SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X", &matCorrection));
	object3.SetActive(true);

	object3.Transform.SetWorldPosition(D3DXVECTOR3(-20, -4, 0));
	object3.Transform.SetScale(D3DXVECTOR3(2, 10, 10));





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

void cScene_02_BoundBlocking::Scene_Release()
{
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);
}

void cScene_02_BoundBlocking::Scene_Update(float timeDelta)
{
	objects.Transform.DefaultControl2(timeDelta);

	this->bHit = PHYSICS_MGR->IsBlocking(&this->objects, &this->object2, 0.5f);
	this->bHit = PHYSICS_MGR->IsBlocking(&this->objects, &this->object3, 0.0f);
}

void cScene_02_BoundBlocking::Scene_Render1()
{
	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//셰이더에 라이팅 셋팅
	cXMesh_Static::sStaticMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Static::sStaticMeshEffect->SetInt("LightNum", this->lights.size());

	cXMesh_Static::SetCamera(this->mainCamera);

	objects.Render();
	object2.Render();
	object3.Render();
}