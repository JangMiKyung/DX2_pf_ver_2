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
	//큐브 생성
	this->CreateCubeHardEdge();
	//this->CreateCubeSoftEdge();

	this->cubeMaterial.Diffuse = D3DXCOLOR(0xffffff00);
	this->cubeMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->cubeMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->cubeMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//셋팅된 Amient 를 정직하게 받겟다...
	this->cubeMaterial.Power = 10.0f;

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

void cScene_01::Release()
{
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_01::Update(float timeDelta)
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

	//라이팅 방향 갱신
	this->light.Direction = this->lightTrans.GetForward();
	//갱신된 라이팅 다시 셋팅
	Device->SetLight(0, &this->light);			//0 번 광원 정보 셋팅


	//카메라 Default 컨트롤
	mainCamera.DefaultControl(timeDelta);

	//카메라 업데이트
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_01::Render()
{
	//큐브의 재질셋팅 ( 중요 ************* 한번랜더링명령이 실행될때 셋팅할수 있는 메터리얼수는 1 개이다 )
	Device->SetMaterial(&cubeMaterial);

	this->cubeTrans.SetDeviceWorld(Device);
	this->RenderCube();

	lightTrans.RenderGimozo();
}


void cScene_01::CreateCubeHardEdge()
{
	//쿼드 플랜 정점 위치
	D3DXVECTOR3 quad[4];
	quad[0] = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	quad[1] = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	quad[2] = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	quad[3] = D3DXVECTOR3(0.5f, -0.5f, -0.5f);


	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, -90.0f * ONE_RAD);

	//정점 배열준비
	D3DXVECTOR3 vertices[24];
	//앞면 
	memcpy(vertices, quad, sizeof(D3DXVECTOR3) * 4);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//우측면 복사
	memcpy(vertices + 4, quad, sizeof(D3DXVECTOR3) * 4);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//뒷면 복사
	memcpy(vertices + 8, quad, sizeof(D3DXVECTOR3) * 4);


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//좌측면 복사
	memcpy(vertices + 12, quad, sizeof(D3DXVECTOR3) * 4);


	D3DXMatrixRotationZ(&matRot, -90.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//윗면 복사
	memcpy(vertices + 16, quad, sizeof(D3DXVECTOR3) * 4);


	D3DXMatrixRotationZ(&matRot, -180.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//아랫면 복사
	memcpy(vertices + 20, quad, sizeof(D3DXVECTOR3) * 4);


	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//인덱스 정보
	DWORD indices[36];

	for (int i = 0; i < 6; i++)
	{
		//정점 스타트 인덱스
		DWORD vertexIndex = i * 4;

		//인덱스 배열 스타트 인덱스
		DWORD indicexIndex = i * 6;

		indices[indicexIndex + 0] = vertexIndex + 0;
		indices[indicexIndex + 1] = vertexIndex + 1;
		indices[indicexIndex + 2] = vertexIndex + 2;
		indices[indicexIndex + 3] = vertexIndex + 0;
		indices[indicexIndex + 4] = vertexIndex + 2;
		indices[indicexIndex + 5] = vertexIndex + 3;
	}


	//노말계산 
	D3DXVECTOR3 Normal[24];
	ComputeNormal(Normal, vertices, 24, indices, 36);

	//
	// 정점 버퍼 생성
	//
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 24,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,		//저장될 메모리 풀 설정
		&this->vb,
		NULL);

	LPMYVERTEX pVertices = NULL;
	this->vb->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 24; i++){
		pVertices[i].pos = vertices[i];
		pVertices[i].normal = Normal[i];
	}

	this->vb->Unlock();

	//
	// 인덱스 버퍼 생성
	//
	Device->CreateIndexBuffer(
		sizeof(DWORD) * 36,		//Cube 는 36 개의 인덱스 정보가 필요,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,				//WORD 형인덱스는 2byte 이니 16 비트인덱스이다, DWORD 로 하면 4byte 니 32 비트 인덱스가 되어야 한다.
		D3DPOOL_DEFAULT,
		&this->ib,					//얻어올 인덱스버퍼 포인터
		NULL);

	DWORD* pIndices = NULL;
	this->ib->Lock(0, 0, (void**)&pIndices, 0);

	memcpy(pIndices, indices, sizeof(DWORD) * 36);

	this->ib->Unlock();
}

void cScene_01::CreateCubeSoftEdge()
{
	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3


	//정점 위치 정보 구성
	D3DXVECTOR3 pos[8];
	pos[0] = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	pos[1] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	pos[2] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	pos[3] = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	pos[4] = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	pos[5] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	pos[6] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	pos[7] = D3DXVECTOR3(1.0f, 1.0f, -1.0f);

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//정점 인덱스 
	DWORD index[36] = {
		0, 4, 7, 0, 7, 3,
		3, 7, 6, 3, 6, 2,
		2, 6, 5, 2, 5, 1,
		1, 5, 4, 1, 4, 0,
		4, 5, 6, 4, 6, 7,
		2, 1, 0, 2, 0, 3
	};


	//노말 계산
	D3DXVECTOR3 Normal[8];
	ComputeNormal(Normal, pos, 8, index, 36);





	//
	// 정점 버퍼 생성
	//
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 8,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,		//저장될 메모리 풀 설정
		&this->vb,
		NULL);

	LPMYVERTEX pVertices = NULL;
	this->vb->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 8; i++){
		pVertices[i].pos = pos[i];
		pVertices[i].normal = Normal[i];
	}

	this->vb->Unlock();

	//
	// 인덱스 버퍼 생성
	//
	Device->CreateIndexBuffer(
		sizeof(DWORD) * 36,		//Cube 는 36 개의 인덱스 정보가 필요,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,				//WORD 형인덱스는 2byte 이니 16 비트인덱스이다, DWORD 로 하면 4byte 니 32 비트 인덱스가 되어야 한다.
		D3DPOOL_DEFAULT,
		&this->ib,					//얻어올 인덱스버퍼 포인터
		NULL);

	DWORD* pIndices = NULL;
	this->ib->Lock(0, 0, (void**)&pIndices, 0);

	memcpy(pIndices, index, sizeof(DWORD) * 36);

	this->ib->Unlock();
}


void cScene_01::RenderCube()
{
	//스트림소스 셋팅
	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));

	//인덱스 버퍼 셋팅
	Device->SetIndices(this->ib);

	//지금그리는 정점에 대한 정점 정보 FVF 플레그 값을 넣어준다. 
	//Device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	Device->SetFVF(MYVERTEX::FVF);

	//SetStreamSource 셋팅된 정점버퍼와 인덱스 버의 내용으로 그린다.
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,		//시작 버텍스배열 인덱스 
		0,
		24,		//정점 갯수 
		0,		//시작 인덱스배열 인덱스
		12		//그리는 개채수 삼각형이니깐 12 개
		);
}