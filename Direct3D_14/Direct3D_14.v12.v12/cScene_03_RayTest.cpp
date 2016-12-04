#include "stdafx.h"
#include "cScene_03_RayTest.h"


cScene_03_RayTest::cScene_03_RayTest()
{
}


cScene_03_RayTest::~cScene_03_RayTest()
{
}

HRESULT cScene_03_RayTest::Scene_Init()
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);
	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	objects.SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Fighter/Fighter.X", &matCorrection));
	objects.SetActive(true);

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

void cScene_03_RayTest::Scene_Release()
{
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);
}

void cScene_03_RayTest::Scene_Update(float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_LCONTROL))
		RayTrans.DefaultControl2(timeDelta);

	else
		objects.Transform.DefaultControl2(timeDelta);


	Ray r;
	r.direction = this->RayTrans.GetForward();
	r.origin = this->RayTrans.GetWorldPosition();

	
	this->bHit = PHYSICS_MGR->IsRayHitBound(
		&r,
		(cBoundBox*)&this->objects.BoundBox,
		&this->objects.Transform,
		&this->hitPos,
		&this->hitNormal);
	

	if (KEY_MGR->IsStayDown(VK_LBUTTON))
	{
		POINT ptMouse = GetMousePos();

		D3DXVECTOR2 screenPos(ptMouse.x, ptMouse.y);

		this->mainCamera.ComputeRay(&this->camRay, &screenPos);

		this->bHit = PHYSICS_MGR->IsRayHitStaticMeshObject(
			&this->camRay,
			&this->objects,
			&this->hitPos,
			&this->hitNormal);
	}
}

void cScene_03_RayTest::Scene_Render1()
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

	RayTrans.RenderGimozo();
	GIZMO_MGR->Line(RayTrans.GetWorldPosition(), RayTrans.GetWorldPosition() + RayTrans.GetForward() * 100000.0f, 0xff00ffff);
	GIZMO_MGR->Line(objects.Transform.GetWorldPosition(), objects.Transform.GetWorldPosition() + objects.Transform.GetForward()*1000.0f, 0xffff0000);

	if (this->bHit)
	{
		GIZMO_MGR->WireSphere(this->hitPos, 0.5f, 0xffff0000);
		GIZMO_MGR->Line(this->hitPos, this->hitPos + this->hitNormal, 0xffff0000);
	}

	//CamRay 그린다.
	GIZMO_MGR->Line(this->camRay.origin, this->camRay.origin + this->camRay.direction * 100, 0xffffff00);
}