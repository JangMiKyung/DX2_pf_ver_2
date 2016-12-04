#include "stdafx.h"
#include "cScene_03.h"


cScene_03::cScene_03()
{
}


cScene_03::~cScene_03()
{
}

void cScene_03::Init()
{
	//구 메쉬
	D3DXCreateSphere(Device,
		5.0f,		//구의 반지름
		100,			//세로 디테일
		100,			//가로 디테일
		&pMesh,
		NULL);

	//구 위치
	transMesh.SetWorldPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//구 매터리얼									
	material.Diffuse = D3DXCOLOR(0xffffffff);
	material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//셋팅된 Amient 를 정직하게 받겟다...
	material.Power = 10.0f;

	//
	// Spot 라이팅
	//
	ZeroMemory(&this->spotlight, sizeof(D3DLIGHT9));
	this->spotlight.Type = D3DLIGHT_SPOT;								//스팟 광원 타입
	this->spotlight.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);		//스팟 광원 방향
	this->spotlight.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//스팟 광원 위치
	this->spotlight.Diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);		//광원 Diffuse 컬러
	this->spotlight.Specular = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);		//광원 Specular 컬러
	this->spotlight.Range = 10.0f;			//Spot 거리
	this->spotlight.Theta = ONE_RAD * 40.0f;//안쪽원뿔각 40도 
	this->spotlight.Phi = ONE_RAD * 45.0f;	//바깥쪽원 뿔각 45도
	this->spotlight.Falloff = 1.0f;			//안쪽원뿔에서 바깥쪽원뿔까지 감쇠되는 수치
	this->spotlight.Attenuation0 = 1.0f;
	this->spotlight.Attenuation1 = 0.5f;
	this->spotlight.Attenuation2 = 0.25f;

	//감쇠 공식
	//거리를 D ( 0 ~ 1 ) 라고 볼때
	// 1 / A0 + A1 * D + A2 * D^2 = 적용되는 광원량

	//
	// 포인트 라이팅
	//
	ZeroMemory(&this->pointlight, sizeof(D3DLIGHT9));
	this->pointlight.Type = D3DLIGHT_POINT;								//점 광원 타입
	//this->pointlight.Direction = D3DXVECTOR3( -1.0f, 0.0f, 0.0f );	//점 광원 방향 ( 방향은 중요하지 않다 )
	this->pointlight.Position = D3DXVECTOR3(0, 0, 0);					//점광원 포지션
	this->pointlight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);		//광원 Diffuse 컬러
	this->pointlight.Specular = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);	//광원 Specular 컬러
	this->pointlight.Range = 5.0f;										//점광원 빛의 도달거리
	this->pointlight.Attenuation0 = 0.0f;
	this->pointlight.Attenuation1 = 0.0f;
	this->pointlight.Attenuation2 = 0.5f;

	//라이팅 셋팅
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);		//광원 사용하겠음....

	Device->SetLight(0, &this->spotlight);			//0 번 광원 정보 셋팅
	Device->LightEnable(0, TRUE);					//0 번 광원 활성화....

	Device->SetLight(1, &this->pointlight);			//1 번 광원 정보 셋팅
	Device->LightEnable(1, FALSE);					//1 번 광원 활성화....


	//Ambient 셑팅
	Device->SetRenderState(D3DRS_AMBIENT, 0xff000000);

	//스펙큘러 ( 정반사 활성화 )
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
}

void cScene_03::Release()
{
	//메쉬 해제
	SAFE_RELEASE(pMesh);
}

void cScene_03::Update(float timeDelta)
{
	float deltaMove = 3.0f * timeDelta;
	float deltaAngle = 90.0f * ONE_RAD * timeDelta;

	if (KEY_MGR->IsStayDown(VK_RBUTTON) == false)
	{
		if (KEY_MGR->IsStayDown('A'))
			this->spotlightTrans.MovePositionSelf(-deltaMove, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown('D'))
			this->spotlightTrans.MovePositionSelf(deltaMove, 0.0f, 0.0f);

		if (KEY_MGR->IsStayDown('W'))
			this->spotlightTrans.MovePositionSelf(0.0f, 0.0f, deltaMove);
		else if (KEY_MGR->IsStayDown('S'))
			this->spotlightTrans.MovePositionSelf(0.0f, 0.0f, -deltaMove);

		if (KEY_MGR->IsStayDown(VK_LEFT))
			this->spotlightTrans.RotateSelf(0.0f, -deltaAngle, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_RIGHT))
			this->spotlightTrans.RotateSelf(0.0f, deltaAngle, 0.0f);

		if (KEY_MGR->IsStayDown(VK_UP))
			this->spotlightTrans.RotateSelf(-deltaAngle, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_DOWN))
			this->spotlightTrans.RotateSelf(deltaAngle, 0.0f, 0.0f);
	}

	if (KEY_MGR->IsStayDown(VK_SPACE))
		RS(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		RS(D3DRS_FILLMODE, D3DFILL_SOLID);

	//라이팅 위치 갱신
	this->spotlight.Position = this->spotlightTrans.GetWorldPosition();
	this->spotlight.Direction = this->spotlightTrans.GetForward();

	//갱신된 라이팅 다시 셋팅
	Device->SetLight(0, &this->spotlight);			//1 번 광원 정보 셋팅

	//카메라 Default 컨트롤
	mainCamera.DefaultControl(timeDelta);

	//카메라 업데이트
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_03::Render()
{
	//메쉬랜더
	Device->SetMaterial(&material);
	transMesh.SetDeviceWorld(Device);
	pMesh->DrawSubset(0);

	//라이트 Transform
	this->spotlightTrans.RenderGimozo();
	GIZMO_MGR->WireSphere(
		this->spotlightTrans.GetWorldPosition(),
		this->spotlight.Range,
		0xffffff00);
}