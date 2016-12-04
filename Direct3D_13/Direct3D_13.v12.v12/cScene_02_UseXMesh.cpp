#include "stdafx.h"
#include "cScene_02_UseXMesh.h"


cScene_02_UseXMesh::cScene_02_UseXMesh()
{
}


cScene_02_UseXMesh::~cScene_02_UseXMesh()
{
}

HRESULT cScene_02_UseXMesh::Scene_Init()
{
	//원래 있던 매쉬에서 다음과 같은 행렬로 보정시켜 
	//모든 정보를 틀어버린다.
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.1f, 0.1f, 0.1f);

	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);

	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	this->pMesh = RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/volcanoRock/volcanoRock_00.X", &matCorrection);

	//라이트 푸쉬
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 1.0f;

	cLight_Point* pLight2 = new cLight_Point();
	pLight2->Color = D3DXCOLOR(1, 1, 1, 0);
	pLight2->minRange = 5.0f;
	pLight2->maxRange = 10.0f;
	pLight2->distancePow = 10.0f;

	cLight_Point* pLight3 = new cLight_Point();
	pLight3->Color = D3DXCOLOR(1, 1, 1, 0);
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

void cScene_02_UseXMesh::Scene_Release()
{
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);
}

void cScene_02_UseXMesh::Scene_Update(float timeDelta)
{
	if( KEY_MGR->IsStayDown( VK_SHIFT ) )
		this->lights[2]->Transform.DefaultControl2( timeDelta );
	else
		this->trans.DefaultControl2( timeDelta );

	this->mainCamera.DefaultControl(timeDelta);
}

void cScene_02_UseXMesh::Scene_Render1()
{
	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//셰이더에 라이팅 셋팅
	cXMesh_Static::sStaticMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Static::sStaticMeshEffect->SetInt("LightNum", this->lights.size());


	cXMesh_Static::SetCamera(this->mainCamera);
	this->pMesh->Render(this->trans);

	this->lights[1]->RenderGizmo();
}