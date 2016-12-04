#include "stdafx.h"
#include "cScene_Test.h"
#include "cLight.h"
#include "cLight_Direction.h"
#include "cLight_Point.h"
#include "cCamera.h"
#include "cBaseObject.h"
#include "cXMesh_Static.h"
#include "cXMesh_Skinned.h"
#include "cSkinnedAnimation.h"
#include "Unit.h"
#include "State.h"
#include "MoveMap.h"
#include "cNode.h"
cScene_Test::cScene_Test()
{
}


cScene_Test::~cScene_Test()
{

}

HRESULT cScene_Test::Scene_Init()
{
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matTrans;
	D3DXMatrixScaling(&matScale, 0.1f, 0.1f, 0.1f);
	D3DXMatrixTranslation(&matTrans, 5, 0, 0);
	matWorld = matScale*matTrans;
	this->house = new cBaseObject();
	this->house->SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/Hospital 3D Model Pack/cot/baby_cot.X", matWorld));
	this->house->SetActive(true);
	//holl/holl.x
	//room_1/room_1.x
	//room_2/room_2.x
	//passage/passage.x

	//라이트 푸쉬


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
	pLight3->pTransform->SetWorldPosition(3, 3, 0);
	pLight3->Intensity = 0.5f;


	this->lights.push_back(pLight2);
	this->lights.push_back(pLight3);

	Pig = new PigEnemy("../../Resources/Meshes/Pig/20161125_Pig.X", { 0,0,0 });
	Pig->Init();
	Miles = new Player("../../Resources/Meshes/Miles2/Miles.X", { 0,0,0 });

	NodeList = new MoveMap();
	NodeList->init();

	Miles->CameraAttachToUnit(vCamera[PLAYER]);
	CurDest = NodeList->Head;

	return S_OK;
}

void cScene_Test::Scene_Release()
{
	SAFE_DELETE(this->house);
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);

}

void cScene_Test::Scene_Update(float timeDelta)
{
	this->house->Update(timeDelta);
	this->Miles->Update(timeDelta);

	//플레이어 카메라는 계속 업데이트 해줘야 한다.
	this->vCamera[PLAYER]->SetWorldPosition(Miles->getHeadPos());

	this->Pig->Update(timeDelta);
	this->Pig->LerpMoveControll(timeDelta, NodeList->GetCurNode(), NodeList->NextNode());

	if (!Pig->GetState()->GetisActive())
	{
		if(Pig->getCurAniName() == "RUN" )
		Pig->StateChange("WALK", 0.3f);
		else if (Pig->getCurAniName() == "WALK")
			Pig->StateChange("RUN", 0.3f);

		NodeList->setCurNodeNext();
		Pig->GetState()->setActive(true);
	}	
}

void cScene_Test::Scene_Render1()
{
	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i <1; i++)
	{
		matLights[i] = this->lights[i]->GetLightMatrix();
	//	this->lights[i]->RenderGizmo();
	}

	//셰이더에 라이팅 셋팅
	//cXMesh_Static::sStaticMeshEffect->SetTechnique("ReceiveShadow");
	cXMesh_Static::sStaticMeshEffect->SetMatrixArray("matLights", matLights, 1);
	cXMesh_Static::sStaticMeshEffect->SetInt("LightNum", this->lights.size());
	cXMesh_Static::SetCamera(this->pMainCamera);

	cXMesh_Skinned::sSkinnedMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Skinned::sSkinnedMeshEffect->SetInt("LightNum", this->lights.size());
	cXMesh_Skinned::SetCamera(this->pMainCamera);

    Pig->Render();
	Miles->Render();
	NodeList->renderNodeRoute();
	this->house->Render();

//	this->air->Render();

}
