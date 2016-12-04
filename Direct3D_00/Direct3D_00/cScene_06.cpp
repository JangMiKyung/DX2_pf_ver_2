#include "stdafx.h"
#include "cScene_06.h"


cScene_06::cScene_06()
{
}


cScene_06::~cScene_06()
{
}

void cScene_06::Init()
{
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

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//정점 배열준비
	MYVERTEX vertices[8];

	vertices[0].pos = D3DXVECTOR3(-2.3f, -2.3f, -2.3f);
	vertices[0].color = 0xffffff00;

	vertices[1].pos = D3DXVECTOR3(-2.3f, -2.3f, 2.3f);
	vertices[1].color = 0xffffff00;

	vertices[2].pos = D3DXVECTOR3(2.3f, -2.3f, 2.3f);
	vertices[2].color = 0xffffff00;

	vertices[3].pos = D3DXVECTOR3(2.3f, -2.3f, -2.3f);
	vertices[3].color = 0xffffff00;

	vertices[4].pos = D3DXVECTOR3(-2.3f, 2.3f, -2.3f);
	vertices[4].color = 0xffffff00;

	vertices[5].pos = D3DXVECTOR3(-2.3f, 2.3f, 2.3f);
	vertices[5].color = 0xffffff00;

	vertices[6].pos = D3DXVECTOR3(2.3f, 2.3f, 2.3f);
	vertices[6].color = 0xffffff00;

	vertices[7].pos = D3DXVECTOR3(2.3f, 2.3f, -2.3f);
	vertices[7].color = 0xffffff00;

	//버퍼 Lock
	void* pVertices = NULL;
	this->vb->Lock(0, 0, &pVertices, 0);

	//얻어온 포인터에 정점정보 메모리 복사 
	memcpy(pVertices, vertices, sizeof(MYVERTEX) * 8);

	this->vb->Unlock();

	//
	// 인덱스 버퍼 생성
	//

	Device->CreateIndexBuffer(
		sizeof(WORD) * 24,		//Cube 는 36 개의 인덱스 정보가 필요,
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
	WORD indices[24] = {
		0, 4, 4, 7, 7, 3, 3, 0,
		1, 5, 5, 6, 6, 2, 2, 1,
		0, 1, 4, 5, 7, 6, 3, 2,
	};

	//익덱스정보 Lock
	void* pIndices = NULL;
	this->ib->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, indices, sizeof(WORD) * 24);
	this->ib->Unlock();

	Device->SetRenderState(
		D3DRS_LIGHTING,			//뭘셋팅할꺼니?
		FALSE					//셋팅값은 어떻게 되니?
		);

	yaw = 0.0f;

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void cScene_06::Release()
{
	//버퍼해제
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_06::Update(float timeDelta)
{
	//초당 90 도 증가
	this->yaw += (90.0f * ONE_RAD) * timeDelta;

	D3DXMATRIXA16 matRoatate;

	D3DXMatrixRotationY(&matRoatate, this->yaw);

	//월드 행렬
	this->matWorld = matRoatate;
}

void cScene_06::Render()
{
	//뷰행렬을 준비
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 lookAtPos(0, 0, 0);		//0, 0, 0,  위치를 본다.
	D3DXVECTOR3 eyePos(0, 5, -10);		//눈의 위치는 0, 1, -10 이다.
	D3DXVECTOR3 up(0, 1, 0);				//상방기준은 월드 위다.

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


	//특정물제를 그리기전에 물체에 대해당되는 월드 행렬을 셋팅하고 그리자
	Device->SetTransform(D3DTS_WORLD, &this->matWorld);

	//스트림소스 셋팅
	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));

	//인덱스 버퍼 셋팅
	Device->SetIndices(this->ib);

	Device->SetFVF(MYVERTEX::FVF);

	Device->DrawIndexedPrimitive(
		D3DPT_LINELIST,// D3DPT_LINELIST 선을그리겟다.
		0,		//시작 버텍스배열 인덱스 
		0,
		8,		//정점 갯수 
		0,		//시작 인덱스배열 인덱스
		12		//그리는 개채수 라인이니깐 12 개
		);
}