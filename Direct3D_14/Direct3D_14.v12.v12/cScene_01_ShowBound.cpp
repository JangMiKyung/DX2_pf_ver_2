#include "stdafx.h"
#include "cScene_01_ShowBound.h"


cScene_01_ShowBound::cScene_01_ShowBound()
{
}


cScene_01_ShowBound::~cScene_01_ShowBound()
{
}

HRESULT cScene_01_ShowBound::Scene_Init()
{
	//원래 있던 매쉬에서 다음과 같은 행렬로 보정시켜 
	//모든 정보를 틀어버린다.
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);
	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	objects.SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X", &matCorrection));
	objects.SetActive(true);

	object2.SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X", &matCorrection));
	object2.SetActive(true);

	object2.Transform.SetWorldPosition(D3DXVECTOR3(10, 10, -2));
	object2.Transform.LookPosition(D3DXVECTOR3(0, 0, 0));

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

void cScene_01_ShowBound::Scene_Release()
{
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);
}

void cScene_01_ShowBound::Scene_Update(float timeDelta)
{
	objects.Transform.DefaultControl2(timeDelta);

	this->bHit = PHYSICS_MGR->IsOverlap(&this->objects, &this->object2);
}

void cScene_01_ShowBound::Scene_Render1()
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

	if (this->bHit)
		DXFONT_MGR->PrintText("으악 충돌", 100, 100, 0xffff0000);
	else
		DXFONT_MGR->PrintText("안 충돌", 100, 100, 0xff00ff00);
}