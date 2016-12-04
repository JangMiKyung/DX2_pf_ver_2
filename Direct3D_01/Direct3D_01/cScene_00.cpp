#include "stdafx.h"
#include "cScene_00.h"


cScene_00::cScene_00()
{
}


cScene_00::~cScene_00()
{
}

void cScene_00::Init()
{
	//비행기 정점 버퍼 생성
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 3,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,
		&planeVB,
		NULL);

	LPMYVERTEX pVertice = NULL;
	planeVB->Lock(0, 0, (void**)&pVertice, 0);

	pVertice[0].pos = D3DXVECTOR3(0.f, 0.f, 1.f);
	pVertice[1].pos = D3DXVECTOR3(1.f, 0.f, -1.f);
	pVertice[2].pos = D3DXVECTOR3(-1.f, 0.f, -1.f);
	pVertice[0].color = 0xff00ff00;
	pVertice[1].color = 0xff00ffff;
	pVertice[2].color = 0xff00ffff;

	planeVB->Unlock();

	//비행기 월드 행렬 초기화
	D3DXMatrixIdentity(&matPlane);


	//회전 초기화
	yawAngle = 0.f;
	yawSpeed = 90.f * ONE_RAD;

	//위치 초기화
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;

	//이동속도
	moveSpeed = 3.f;

	//두번째 비행기
	pos2.x = 0.f;
	pos2.y = 0.f;
	pos2.z = -5.f;

	D3DXMatrixIdentity(&matPlane2);

	//뷰행렬 준비
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 eyePos(0.f, 30.f, 0.f);
	D3DXVECTOR3 lookAt(0.f, 0.f, 0.f);
	D3DXVECTOR3 up(0.f, 0.f, 1.f);

	D3DXMatrixLookAtLH(
		&matView,
		&eyePos,
		&lookAt,
		&up);

	//뷰행렬 셋팅
	Device->SetTransform(D3DTS_VIEW, &matView);

	//투영 준비
	D3DXMATRIXA16 matProjection;

	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		60.f * ONE_RAD,
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),
		0.01f,
		100.f);

	//투영 셋팅
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	//빛 끄자
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

}

void cScene_00::Release()
{
	SAFE_RELEASE(planeVB);
}

void cScene_00::Update(float timeDelta)
{
	//회전 컨트롤
	if (KEY_MGR->IsStayDown(VK_LEFT))
		this->yawAngle -= this->yawSpeed * timeDelta;
	else if (KEY_MGR->IsStayDown(VK_RIGHT))
		this->yawAngle += yawSpeed * timeDelta;

	//YawAngle값만큼 회전행렬
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, this->yawAngle);


	//초당 이동량 
	float moveDelta = this->moveSpeed * timeDelta;

	//회전을 먹기전 사물의 진행방향
	D3DXVECTOR3 moveDir(0, 0, 1);

	//진행방향에 회전행렬을 먹인다.
	D3DXVec3TransformNormal(&moveDir, &moveDir, &matRot);

	//이동 벡터 이동량 * 진행방향
	D3DXVECTOR3 moveVector = moveDir * moveDelta;

	//위치값에 이동향을 더한다.
	this->pos += moveVector;

	//위치 행렬
	D3DXMATRIXA16 matPosition;
	D3DXMatrixTranslation(&matPosition,
		this->pos.x,
		this->pos.y,
		this->pos.z);

	this->matPlane = matRot * matPosition;

	//위치행렬
	D3DXMATRIXA16 matPosition2;
	D3DXMatrixTranslation(&matPosition2,
		this->pos2.x,
		this->pos2.y,
		this->pos2.z);

	//두번째 비행기에대한 회전 행렬
	D3DXMATRIXA16 matRot2;

	//바라봐야 할 타겟의 방향벡터
	D3DXVECTOR3 dirToTarget = this->pos - this->pos2;

	//정규화
	D3DXVec3Normalize(&dirToTarget, &dirToTarget);

	D3DXVECTOR3 dir2(0, 0, 1);

	//각차
	float yawAngle2 = acos(D3DXVec3Dot(&dir2, &dirToTarget));

	//정면과 타겟방향 외적
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &dir2, &dirToTarget);

	//외적 결과는 정규화 되지 않는다
	//그래서 외적결과를 정규화 하자.
	D3DXVec3Normalize(&cross, &cross);

	//그외적결과를 축으로 한 회전행렬
	D3DXMatrixRotationAxis(&matRot2, &cross, yawAngle2);

	this->matPlane2 = matRot2 * matPosition2;
}

void cScene_00::Render()
{
	//비행기 월드 행렬 셋팅
	Device->SetTransform(D3DTS_WORLD, &matPlane);

	//비행기 그려라
	Device->SetStreamSource(0, this->planeVB, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);



	//두번째 비행기 월드 행렬 셋팅
	Device->SetTransform(D3DTS_WORLD, &matPlane2);

	//비행기 그려라
	Device->SetStreamSource(0, this->planeVB, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}