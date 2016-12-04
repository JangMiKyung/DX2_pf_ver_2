#include "stdafx.h"
#include "cScene_04.h"


cScene_04::cScene_04()
{
}


cScene_04::~cScene_04()
{
}

void cScene_04::Init()
{
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 8,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,		//저장될 메모리 풀 설정
		&this->vb,
		NULL);

	//저장될 메모리풀
	//D3DPOOL_DEFAULT			//정점버퍼의 메모리는 시스템사양에 맞춰 알아서 저장이되는데 보통은 비디오 카드메모리에 저장이된다.
	//D3DPOOL_MANAGED			//정점버퍼의 메모리는 시스템의 메모리영역에 저장이되고 비디오카드 메모리를 사용한다면 비디오카드 메모리에도 저장이된다, 즉 버퍼의 원본내용이 System 메모리에 저장이된 상태에서 비디오카드 메모리에 그거와 똑같은 사본을 만들어쓰는 방식인데... 이는 Device 가 해제 되어도 버퍼의 내용을 다시 복구할 수 있는 여지가 있다.

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//정점 배열준비
	MYVERTEX vertices[8];

	vertices[0].pos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	vertices[0].color = 0xffffff00;

	vertices[1].pos = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	vertices[1].color = 0xffff0000;

	vertices[2].pos = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	vertices[2].color = 0xff00ff00;

	vertices[3].pos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	vertices[3].color = 0xff00ffff;

	vertices[4].pos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	vertices[4].color = 0xff0000ff;

	vertices[5].pos = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	vertices[5].color = 0xffff00ff;

	vertices[6].pos = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	vertices[6].color = 0xffffffff;

	vertices[7].pos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	vertices[7].color = 0xff000000;

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
	WORD indices[36] = {
		0, 4, 7, 0, 7, 3,
		3, 7, 6, 3, 6, 2,
		2, 6, 5, 2, 5, 1,
		1, 5, 4, 1, 4, 0,
		4, 5, 6, 4, 6, 7,
		1, 0, 3, 1, 3, 2
	};

	//익덱스벙보 Lock
	void* pIndices = NULL;
	this->ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, indices, sizeof(WORD) * 36);

	this->ib->Unlock();

	Device->SetRenderState(
		D3DRS_LIGHTING,			//뭘셋팅할꺼니?
		FALSE					//셋팅값은 어떻게 되니?
		);

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void cScene_04::Release()
{
	//버퍼해제
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_04::Update(float timeDelta)
{
	//초당 90 도 증가
	this->yaw += (90.0f * ONE_RAD) * timeDelta;

	D3DXMATRIXA16 matRoatate;
	D3DXMatrixRotationY(&matRoatate, this->yaw);

	//월드 행렬
	this->matWorld = matRoatate;
}

void cScene_04::Render()
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

	//SetStreamSource 셋팅된 정점버퍼와 인덱스 버의 내용으로 그린다.
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,		//시작 버텍스배열 인덱스 
		0,
		8,		//정점 갯수 
		0,		//시작 인덱스배열 인덱스
		12		//그리는 개채수 삼각형이니깐 12 개
		);
}
