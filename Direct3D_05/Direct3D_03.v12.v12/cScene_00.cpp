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
	//큐브 생성
	this->CreateCube();

	//회전값 초기화
	this->xAngle = 0.0f;
	this->yAngle = 0.0f;
	this->zAngle = 0.0f;

	//큐브 월드 행렬 초기화
	D3DXMatrixIdentity(&matWorld);

	//뷰행렬 준비
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 eyePos(1, 3.f, -5.f);
	D3DXVECTOR3 lookAt(0.f, 0.f, 0.f);
	D3DXVECTOR3 up(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&matView, &eyePos, &lookAt, &up);

	//뷰행렬 셋팅
	Device->SetTransform(D3DTS_VIEW, &matView);

	//투영행렬 준비
	D3DXMATRIXA16 matProjection;

	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		60.f * ONE_RAD,
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),
		0.01f,
		100.f);

	//투영 행렬 셋팅
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void cScene_00::Release()
{
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_00::Update(float timeDelta)
{
	if (KEY_MGR->IsOnceDown('A'))
		this->yAngle -= 10.0f;
	else if (KEY_MGR->IsOnceDown('D'))
		this->yAngle += 10.0f;


	if (KEY_MGR->IsOnceDown('W'))
		this->xAngle -= 10.0f;
	else if (KEY_MGR->IsOnceDown('S'))
		this->xAngle += 10.0f;


	if (KEY_MGR->IsOnceDown('Q'))
		this->zAngle += 10.0f;
	else if (KEY_MGR->IsOnceDown('E'))
		this->zAngle -= 10.0f;

	//Y 축에대한 회전행렬
	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationY(&matRotateY, (this->yAngle * ONE_RAD));

	//X 축에대한 회전행렬
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationX(&matRotateX, (this->xAngle * ONE_RAD));

	//Z 축에대한 회전행렬
	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, (this->zAngle * ONE_RAD));

	//월드 행렬
	//회량대로 곱해버리는 오일러 회전 방식
	this->matWorld = matRotateX * matRotateY * matRotateZ;

	//문제 있음 짐벌락
	//두번째 곱해지는 값이 90 도 단위로 회전을 하는 경우
	//첫번째 축과 마지막 회전축이 동일해져 첫번째 혹은 세번째 곱한 회전량 정보가 사라진다...
}

void cScene_00::Render()
{
	//큐브 월드 행렬 셋팅
	Device->SetTransform(D3DTS_WORLD, &matWorld);

	//스트림소스 셋팅
	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));

	//인덱스 버퍼 셋팅
	Device->SetIndices(this->ib);

	Device->SetFVF(MYVERTEX::FVF);

	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,		//시작 버텍스배열 인덱스 
		0,
		24,		//정점 갯수 
		0,		//시작 인덱스배열 인덱스
		12		//그리는 개채수 삼각형이니깐 12 개
		);

	//회전 값 출력
	std::string str = "";
	str.resize(128);
	
	sprintf(&str[0], "XAngle : %.2f", this->xAngle);
	DXFONT_MGR->PrintText(str, 0, 100, 0xffff8080);

	sprintf(&str[0], "YAngle : %.2f", this->yAngle);
	DXFONT_MGR->PrintText(str, 0, 120, 0xff80ff80);

	sprintf(&str[0], "ZAngle : %.2f", this->zAngle);
	DXFONT_MGR->PrintText(str, 0, 140, 0xff8080ff);

	GIZMO_MGR->Circle(D3DXVECTOR3(0, 0, 0), 3.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0xffff0000);
	GIZMO_MGR->Circle(D3DXVECTOR3(0, 0, 0), 3.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0xff00ff00);

	GIZMO_MGR->Circle(D3DXVECTOR3(0, 0, 0), 3.0f, D3DXVECTOR3(
		sin(this->yAngle *ONE_RAD),
		0.0f,
		cos(this->yAngle * ONE_RAD)),
		0xff0000ff);

}

void cScene_00::CreateCube()
{
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

	//쿼드 플랜 정점
	MYVERTEX quad[4];
	quad[0].pos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	quad[1].pos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	quad[2].pos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	quad[3].pos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);


	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, -90.0f * ONE_RAD);


	//정점 배열준비
	MYVERTEX vertices[24];

	//앞면 
	memcpy(vertices, quad, sizeof(MYVERTEX) * 4);
	vertices[0].color = 0xff000080;
	vertices[1].color = 0xff000080;
	vertices[2].color = 0xff000080;
	vertices[3].color = 0xff000080;


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//우측면 복사
	memcpy(vertices + 4, quad, sizeof(MYVERTEX) * 4);
	vertices[4].color = 0xffff0000;
	vertices[5].color = 0xffff0000;
	vertices[6].color = 0xffff0000;
	vertices[7].color = 0xffff0000;


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//뒷면 복사
	memcpy(vertices + 8, quad, sizeof(MYVERTEX) * 4);
	vertices[8].color = 0xff0000ff;
	vertices[9].color = 0xff0000ff;
	vertices[10].color = 0xff0000ff;
	vertices[11].color = 0xff0000ff;


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//좌측면 복사
	memcpy(vertices + 12, quad, sizeof(MYVERTEX) * 4);
	vertices[12].color = 0xff800000;
	vertices[13].color = 0xff800000;
	vertices[14].color = 0xff800000;
	vertices[15].color = 0xff800000;



	D3DXMatrixRotationZ(&matRot, -90.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//윗면 복사
	memcpy(vertices + 16, quad, sizeof(MYVERTEX) * 4);
	vertices[16].color = 0xff00ff00;
	vertices[17].color = 0xff00ff00;
	vertices[18].color = 0xff00ff00;
	vertices[19].color = 0xff00ff00;


	D3DXMatrixRotationZ(&matRot, -180.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//아랫면 복사
	memcpy(vertices + 20, quad, sizeof(MYVERTEX) * 4);
	vertices[20].color = 0xff008000;
	vertices[21].color = 0xff008000;
	vertices[22].color = 0xff008000;
	vertices[23].color = 0xff008000;


	//버퍼 Lock
	void* pVertices = NULL;
	this->vb->Lock(0, 0, &pVertices, 0);

	//얻어온 포인터에 정점정보 메모리 복사 
	memcpy(pVertices, vertices, sizeof(MYVERTEX) * 24);

	this->vb->Unlock();

	Device->CreateIndexBuffer(
		sizeof(WORD) * 36,		//Cube 는 36 개의 인덱스 정보가 필요,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,				//WORD 형인덱스는 2byte 이니 16 비트인덱스이다, DWORD 로 하면 4byte 니 32 비트 인덱스가 되어야 한다.
		D3DPOOL_DEFAULT,
		&this->ib,					//얻어올 인덱스버퍼 포인터
		NULL);

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//인덱스 정보
	WORD indices[36];

	for (int i = 0; i < 6; i++)
	{
		//정점 스타트 인덱스
		WORD vertexIndex = i * 4;

		//인덱스 배열 스타트 인덱스
		WORD indicexIndex = i * 6;

		indices[indicexIndex + 0] = vertexIndex + 0;
		indices[indicexIndex + 1] = vertexIndex + 1;
		indices[indicexIndex + 2] = vertexIndex + 2;
		indices[indicexIndex + 3] = vertexIndex + 0;
		indices[indicexIndex + 4] = vertexIndex + 2;
		indices[indicexIndex + 5] = vertexIndex + 3;
	}

	//익덱스벙보 Lock
	void* pIndices = NULL;
	this->ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, indices, sizeof(WORD) * 36);

	this->ib->Unlock();
}