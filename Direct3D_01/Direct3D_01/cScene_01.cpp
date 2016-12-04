#include "stdafx.h"
#include "cScene_01.h"


cScene_01::cScene_01()
{
}


cScene_01::~cScene_01()
{
}

void cScene_01::Init()
{
	//비행기 정점 버퍼 생성
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 3,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,
		&this->planeVB,
		NULL);

	LPMYVERTEX pVertice;
	this->planeVB->Lock(0, 0, (void**)&pVertice, 0);
	pVertice[0].pos = D3DXVECTOR3(0, 0, 1);
	pVertice[0].color = 0xFFFF0000;
	pVertice[1].pos = D3DXVECTOR3(1, 0, -1);
	pVertice[1].color = 0xFFFFFF00;
	pVertice[2].pos = D3DXVECTOR3(-1, 0, -1);
	pVertice[2].color = 0xFFFFFF00;
	this->planeVB->Unlock();

	//비행기 월드 행렬 초기화
	D3DXMatrixIdentity(&matPlane);		//행렬을 항등행렬로 만들어준다.

	//회전 초기화
	this->yawAngle = 0.0f;
	this->yawSpeed = 90.0 * ONE_RAD;	//초당 90 도 회전

	//위치 초기회
	this->pos.x = 0.0f;
	this->pos.y = 0.0f;
	this->pos.z = 0.0f;

	//이동속도
	this->moveSpeed = 3.0f;

	//자식 비행기 위치
	this->localPos = D3DXVECTOR3(3, 0, 0);
	D3DXMatrixIdentity(&matChildPlane);

	//뷰행렬을 준비
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 lookAtPos(0, 0, 0);		//0, 0, 0,  위치를 본다.
	D3DXVECTOR3 eyePos(0, 30, 0);		//눈의 위치는 0, 1, -10 이다.
	D3DXVECTOR3 up(0, 0, 1);				//상방기준은 월드 위다.

	D3DXMatrixLookAtLH(
		&matView,			//얻어올 뷰행렬 
		&eyePos,			//눈위치 벡터
		&lookAtPos,			//바라볼 위치
		&up					//상방 기준 ( 완전 정확하지 않아도된다 )
		);
	//뷰행렬 셋팅
	Device->SetTransform(D3DTS_VIEW, &matView);

	//투영행렬 준비
	D3DXMATRIXA16 matProjection;
	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		60.0f * ONE_RAD,		//화각 ( 60.0 도 )
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),		//화면 종횡비,
		0.01f,					//근거리 평면 거리 ( 이값이 0 이면 뎁스 꼬인다 0 으로 주지말아라 )
		100.0f);					//원거리 평명 거리
	//투영행렬 셋팅
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	Device->SetRenderState(
		D3DRS_LIGHTING,			//뭘셋팅할꺼니?
		FALSE					//셋팅값은 어떻게 되니?
		);
}

void cScene_01::Release()
{
	SAFE_RELEASE(this->planeVB);
}

void cScene_01::Update(float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_LEFT))
		this->yawAngle -= this->yawSpeed * timeDelta;
	if (KEY_MGR->IsStayDown(VK_RIGHT))
		this->yawAngle += this->yawSpeed * timeDelta;

	//YawAngle값만큼 회전행렬
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, this->yawAngle);

	//초당 이동량 
	float moveDelta = this->moveSpeed * timeDelta;		//이번 프레임에 움직일량

	//회전을 먹기전 사물의 진행방향
	D3DXVECTOR3 moveDir(0, 0, 1);

	//진행방향에 회전행렬을 먹인다.
	D3DXVec3TransformNormal(&moveDir, &moveDir, &matRot);


	//이동 벡터 이동량 * 진행방향
	D3DXVECTOR3 moveVector = moveDir * moveDelta;

	//위치값에 이동향을 더한다.
	//this->pos = this->pos + moveVector;
	this->pos += moveVector;

	//위치 행렬
	D3DXMATRIXA16 matPosition;
	D3DXMatrixTranslation(&matPosition,
		this->pos.x,
		this->pos.y,
		this->pos.z);
	this->matPlane = matRot * matPosition;


	//자식 배행기 행렬 준비
	D3DXMATRIXA16 matChildPos;
	D3DXMatrixTranslation(&matChildPos,
		this->localPos.x,
		this->localPos.y,
		this->localPos.z);

	//자식 행렬
	if (KEY_MGR->IsStayDown(VK_SPACE))
	{
		//부모및으로....
		this->matChildPlane = matChildPos * this->matPlane;
	}
	else
	{
		this->matChildPlane = matChildPos;
	}
}

void cScene_01::Render()
{
	//비행기 월드 행렬 셋팅
	Device->SetTransform(D3DTS_WORLD, &matPlane);

	//비행기 그려라
	Device->SetStreamSource(0, this->planeVB, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	//자식 비행기 월드 행렬 셋팅
	Device->SetTransform(D3DTS_WORLD, &matChildPlane);

	//비행기 그려라
	Device->SetStreamSource(0, this->planeVB, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	//행렬값 확인
	D3DXMATRIXA16 mat;
	//D3DXMatrixIdentity( &mat );
	D3DXMatrixTranslation(&mat, 3, 2, 7);


	/*
	//다음 과 같은 맴버 변수로 행렬 요소값에 접근가능...
	mat._11, mat._12, mat._13, mat._14
	mat._21, mat._22, mat._23, mat._24
	mat._31, mat._32, mat._33, mat._34
	mat._41, mat._42, mat._43, mat._44

	mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3]
	mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3]
	mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3]
	mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]
	*/

	/*char szTemp[128];
	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3]);
	DXFONT_MGR->PrintText(szTemp, 0, 0, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3]);
	DXFONT_MGR->PrintText(szTemp, 0, 20, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3]);
	DXFONT_MGR->PrintText(szTemp, 0, 40, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]);
	DXFONT_MGR->PrintText(szTemp, 0, 60, 0xffffffff);*/

	char szTemp[128];
	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", matChildPlane._11, matChildPlane._12, matChildPlane._13, matChildPlane._14);
	DXFONT_MGR->PrintText(szTemp, 0, 0, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", matChildPlane._21, matChildPlane._22, matChildPlane._23, matChildPlane._24);
	DXFONT_MGR->PrintText(szTemp, 0, 20, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", matChildPlane._31, matChildPlane._32, matChildPlane._33, matChildPlane._34);
	DXFONT_MGR->PrintText(szTemp, 0, 40, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", matChildPlane._41, matChildPlane._42, matChildPlane._43, matChildPlane._44);
	DXFONT_MGR->PrintText(szTemp, 0, 60, 0xffffffff);
}