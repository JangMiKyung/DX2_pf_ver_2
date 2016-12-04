#include "stdafx.h"
#include "cScene_02.h"


cScene_02::cScene_02()
{
}


cScene_02::~cScene_02()
{
}

void cScene_02::Init()
{
	//
	// 큐브 생성
	//

	//큐브 메쉬
	D3DXCreateBox(Device, 1.0f, 1.0f, 1.0f, &pMeshCube, NULL);

	//큐브 위치
	transCube.SetWorldPosition(D3DXVECTOR3(-2.0f, 2.0f, 0.0f));

	//큐브 매터리얼									
	this->cubeMaterial.Diffuse = D3DXCOLOR(0xffffff00);
	this->cubeMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->cubeMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->cubeMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//셋팅된 Amient 를 정직하게 받겟다...
	this->cubeMaterial.Power = 10.0f;

	//
	// 주전자 생성
	//

	//주전자 메쉬
	D3DXCreateTeapot(Device, &pMeshTeapot, NULL);

	//주전자 위치
	transTeapot.SetWorldPosition(D3DXVECTOR3(2.0f, 2.0f, 0.0f));

	//주전자 매터리얼									
	this->teapotMaterial.Diffuse = D3DXCOLOR(0xffff0000);
	this->teapotMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->teapotMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->teapotMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//셋팅된 Amient 를 정직하게 받겟다...
	this->teapotMaterial.Power = 10.0f;

	//
	// 구 생성
	//

	//구 메쉬
	D3DXCreateSphere(Device,
		1.0f,		//구의 반지름
		60,			//세로 디테일
		60,			//가로 디테일
		&pMeshSphere,
		NULL);

	//구 위치
	transSphere.SetWorldPosition(D3DXVECTOR3(-2.0f, -2.0f, 0.0f));

	//구 매터리얼									
	this->SphereMaterial.Diffuse = D3DXCOLOR(0xff00ff00);
	this->SphereMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->SphereMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->SphereMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//셋팅된 Amient 를 정직하게 받겟다...
	this->SphereMaterial.Power = 10.0f;

	//
	// 도낫 생성
	//

	//도낫 메쉬
	D3DXCreateTorus(Device, 0.5f, 1.0f, 30, 30, &pMeshTorus, NULL);

	//도낫 위치
	transTorus.SetWorldPosition(D3DXVECTOR3(2.0f, -2.0f, 0.0f));

	//도낫 매터리얼									
	this->torusMaterial.Diffuse = D3DXCOLOR(0xff0000ff);
	this->torusMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->torusMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->torusMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//셋팅된 Amient 를 정직하게 받겟다...
	this->torusMaterial.Power = 10.0f;

	//라이팅
	ZeroMemory(&this->light, sizeof(D3DLIGHT9));
	this->light.Type = D3DLIGHT_DIRECTIONAL;						//방향성 광원 타입
	this->light.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);		//방향성 광원 방향
	this->light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//광원 Diffuse 컬러
	this->light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//광원 Specular 컬러

	//라이팅 셋팅
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);		//광원 사용하겠음....

	Device->SetLight(0, &this->light);			//0 번 광원 정보 셋팅
	Device->LightEnable(0, TRUE);					//0 번 광원 활성화....

	//Ambient 셑팅
	Device->SetRenderState(D3DRS_AMBIENT, 0xff000000);

	//스펙큘러 ( 정반사 활성화 )
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
}

void cScene_02::Release()
{
	//메쉬 해제
	SAFE_RELEASE(pMeshCube);
	SAFE_RELEASE(pMeshSphere);
	SAFE_RELEASE(pMeshTeapot);
	SAFE_RELEASE(pMeshTorus);
}

void cScene_02::Update(float timeDelta)
{
	float deltaMove = 3.0f * timeDelta;
	float deltaAngle = 90.0f * ONE_RAD * timeDelta;

	if (KEY_MGR->IsStayDown(VK_RBUTTON) == false)
	{
		if (KEY_MGR->IsStayDown('A'))
			this->lightTrans.MovePositionSelf(-deltaMove, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown('D'))
			this->lightTrans.MovePositionSelf(deltaMove, 0.0f, 0.0f);

		if (KEY_MGR->IsStayDown('W'))
			this->lightTrans.MovePositionSelf(0.0f, 0.0f, deltaMove);
		else if (KEY_MGR->IsStayDown('S'))
			this->lightTrans.MovePositionSelf(0.0f, 0.0f, -deltaMove);

		if (KEY_MGR->IsStayDown(VK_LEFT))
			this->lightTrans.RotateSelf(0.0f, -deltaAngle, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_RIGHT))
			this->lightTrans.RotateSelf(0.0f, deltaAngle, 0.0f);

		if (KEY_MGR->IsStayDown(VK_UP))
			this->lightTrans.RotateSelf(-deltaAngle, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_DOWN))
			this->lightTrans.RotateSelf(deltaAngle, 0.0f, 0.0f);
	}


	if (KEY_MGR->IsStayDown(VK_SPACE))
		RS(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		RS(D3DRS_FILLMODE, D3DFILL_SOLID);

	//라이팅 방향 갱신
	this->light.Direction = this->lightTrans.GetForward();
	//갱신된 라이팅 다시 셋팅
	Device->SetLight(0, &this->light);			//0 번 광원 정보 셋팅


	//카메라 Default 컨트롤
	mainCamera.DefaultControl(timeDelta);

	//카메라 업데이트
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_02::Render()
{
	//큐브 랜더
	Device->SetMaterial(&cubeMaterial);	//재질셋팅 ( 중요 ************* 한번랜더링명령이 실행될때 셋팅할수 있는 메터리얼수는 1 개이다 )
	transCube.SetDeviceWorld(Device);
	pMeshCube->DrawSubset(0);

	//주전자 랜더
	Device->SetMaterial(&teapotMaterial);	//재질셋팅 ( 중요 ************* 한번랜더링명령이 실행될때 셋팅할수 있는 메터리얼수는 1 개이다 )
	transTeapot.SetDeviceWorld(Device);
	pMeshTeapot->DrawSubset(0);

	//구 랜더
	Device->SetMaterial(&SphereMaterial);	//재질셋팅 ( 중요 ************* 한번랜더링명령이 실행될때 셋팅할수 있는 메터리얼수는 1 개이다 )
	transSphere.SetDeviceWorld(Device);
	pMeshSphere->DrawSubset(0);

	//도낫 랜더
	Device->SetMaterial(&torusMaterial);	//재질셋팅 ( 중요 ************* 한번랜더링명령이 실행될때 셋팅할수 있는 메터리얼수는 1 개이다 )
	transTorus.SetDeviceWorld(Device);
	pMeshTorus->DrawSubset(0);

	lightTrans.RenderGimozo();
}