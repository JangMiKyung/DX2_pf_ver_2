#include "stdafx.h"
#include "cScene_01.h"


cScene_01::cScene_01()
{
}


cScene_01::~cScene_01()
{
}

HRESULT cScene_01::Init()
{
	//
	// 쿼드 생성
	//
	CreateQuad();

	//
	// Texture 로딩 
	//
	//기본적으로 밉맵체인이 만들어진다.
	D3DXCreateTextureFromFile(
		Device,					//디바이스
		"bby.bmp",			//경로포함한 파일명
		&this->m_pTex			//얻어올 IDirect3DTexture9 더블포인터
		);


	//Texture 의 필터모드
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	// 셰이더 로딩
	//

	// 셰이더 로딩시 컴파일 에러가 나면 에러의 내용을 받아올 Error 버퍼
	LPD3DXBUFFER pError = NULL;

	DWORD				dwShaderFlag = 0;		//셰이더 플레그 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif

	//fxFile 로 부터 Effect 객체 로딩
	D3DXCreateEffectFromFile(
		Device,						//디바이스
		"Texure.fx",				//불러올 셰이더 코드 파일이름
		NULL, 						//셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL, 						//셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,				//셰이더 컴파일 플레그
		NULL, 						//셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,					//로딩될 셰이더 Effect 포인터
		&pError 					//셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
		);

	//로딩에 문제가 있다면....
	if (pError != NULL)
	{
		//Error 버퍼의 버퍼포인터에 에러내용에 관한 문자열이 참조된다.
		LOG_MGR->AddLog((char*)pError->GetBufferPointer());

		//Error 버퍼 Release
		SAFE_RELEASE(pError);
		return E_FAIL;
	}


	return S_OK;
}

void cScene_01::Release()
{
	SAFE_RELEASE(this->pEffect);

	SAFE_RELEASE(this->m_vb);
	SAFE_RELEASE(this->m_ib);

	//해제
	SAFE_RELEASE(this->m_pTex);
}

void cScene_01::Update(float timeDelta)
{
	m_mainCamera.DefaultControl(timeDelta);
	m_quadTrans.DefaultControl2(timeDelta);
	m_mainCamera.UpdateCamToDevice(Device);
}

void cScene_01::Render()
{
	//Effect 에 전역 변수값을 셋팅한다.
	D3DXMATRIXA16 matWorld = m_quadTrans.GetFinalMatrix();

	//Device 로부터 셋팅된 View 행렬과 Projection 행렬을 얻는다.
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;
	Device->GetTransform(D3DTS_VIEW, &matView);
	Device->GetTransform(D3DTS_PROJECTION, &matProjection);


	//총변환 행렬
	D3DXMATRIXA16 matWVP = matWorld * matView * matProjection;
	this->pEffect->SetMatrix("matWVP", &matWVP);


	//컬러 벡터 셋팅 ( 주의 사항  셋팅되는 벡터는 모두 Vector4 로 대동단결된다 )
	D3DXVECTOR4 color(1, 1, 0, 1);
	this->pEffect->SetVector("vColor", &color);


	//Texture 셋팅
	this->pEffect->SetTexture("MyTex", m_pTex);

	//타일수 셋팅
	this->pEffect->SetFloat("tilingX", 2);
	this->pEffect->SetFloat("tilingY", 2);



	//Technique 셋팅
	this->pEffect->SetTechnique("MyShader");



	//셰이더로 Cube 랜더
	UINT iPassNum = 0;		//패스수

	//Effect 로 랜더링 시작 ( 이때 Pass 수를 UINT 형으로 받아온다 )
	this->pEffect->Begin(
		&iPassNum,		//해당 Effect 에 Pass 수를 받는다.
		0				//플레그 일단 NULL
		);

	//Pass 수대로 for 문 돌아재낀다.
	for (UINT i = 0; i < iPassNum; i++){

		this->pEffect->BeginPass(i);		//i 번째 Pass 시작

		//랜더
		//그리자
		Device->SetStreamSource(0, this->m_vb, 0, sizeof(MYVERTEX));
		Device->SetFVF(MYVERTEX::FVF);
		Device->SetIndices(this->m_ib);
		Device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0, 0, 4, 0, 2);


		this->pEffect->EndPass();			//Pass 끝

	}

	this->pEffect->End();						//Effect 로 랜더링 끝



}

void cScene_01::CreateQuad()
{
	//정점 버퍼 생성
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 4,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,
		&this->m_vb,
		NULL);

	// 1----2
	// |   /|
	// |  / |
	// | /  |
	// |/   |
	// 0----3

	LPMYVERTEX pVertices = NULL;
	this->m_vb->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].pos = D3DXVECTOR3(-3.5f, -3.5f, 0.0f);
	pVertices[1].pos = D3DXVECTOR3(-3.5f, 3.5f, 0.0f);
	pVertices[2].pos = D3DXVECTOR3(3.5f, 3.5f, 0.0f);
	pVertices[3].pos = D3DXVECTOR3(3.5f, -3.5f, 0.0f);

	/*
	pVertices[0].color = 0xffff0000;
	pVertices[1].color = 0xffffff00;
	pVertices[2].color = 0xffff0000;
	pVertices[3].color = 0xffffff00;
	*/

	// 1----2
	// |   /|
	// |  / |
	// | /  |
	// |/   |
	// 0----3

	//정점에 Texture UV 좌표를 넣어준다.
	pVertices[0].uv = D3DXVECTOR2(0, 1);
	pVertices[1].uv = D3DXVECTOR2(0, 0);
	pVertices[2].uv = D3DXVECTOR2(1, 0);
	pVertices[3].uv = D3DXVECTOR2(1, 1);


	this->m_vb->Unlock();


	//
	// 인덱스 버퍼
	//
	Device->CreateIndexBuffer(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&this->m_ib,
		NULL);

	WORD idx[] = { 0, 1, 2, 0, 2, 3 };

	void* pIndices = NULL;

	this->m_ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, idx, sizeof(WORD) * 6);

	this->m_ib->Unlock();
}