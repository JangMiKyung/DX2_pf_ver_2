#include "stdafx.h"
#include "cScene_00.h"


cScene_00::cScene_00()
{
}


cScene_00::~cScene_00()
{
}

HRESULT cScene_00::Init()
{
	//쿼드 생성
	CreateQuad();

	//Texture 로딩 
	//기본적으로 밉맵체인이 만들어진다.
	D3DXCreateTextureFromFile(
		Device,					//디바이스
		"bby.bmp",			//경로포함한 파일명
		&this->m_pTex			//얻어올 IDirect3DTexture9 더블포인터
		);

	//
	////밉맵체인없이 로딩
	//D3DXCreateTextureFromFileEx(
	//	Device, 
	//	"bby.bmp",
	//	512,
	//	512, 
	//	0,
	//	0, 
	//	D3DFMT_UNKNOWN,
	//	D3DPOOL_MANAGED,
	//	D3DX_FILTER_NONE,
	//	D3DX_FILTER_NONE,
	//	0,
	//	0,
	//	0,
	//	&m_pTex );
	
	//정점의 컬러 정보를 출력하기 위해서는 Device 의 기본 라이팅 
	//기능을 해재해야 한다
	//당분간 라이팅을 사용할일이 없으니 라이팅을 끄자.
	Device->SetRenderState(
		D3DRS_LIGHTING,			//뭘셋팅할꺼니?
		FALSE					//셋팅값은 어떻게 되니?
		);


	//쿼드 스케일 200
	m_quadTrans.SetScale(200, 200, 200);

	//위로보게
	m_quadTrans.RotateLocal(90 * ONE_RAD, 0, 0);


	return S_OK;
}

void cScene_00::Release()
{
	SAFE_RELEASE(this->m_vb);
	SAFE_RELEASE(this->m_ib);

	//해제
	SAFE_RELEASE(this->m_pTex);
}

void cScene_00::Update(float timeDelta)
{
	m_mainCamera.DefaultControl(timeDelta);
	m_quadTrans.DefaultControl2(timeDelta);

	m_mainCamera.UpdateCamToDevice(Device);



	//필터모드 셋팅
	//Device->SetTextureStageState( 스테이지번호, 어떤거에대한 필터를 셋팅할꺼니, 필터옵션 )

	//어떤거에대한 필터를 셋팅할꺼니
	//D3DSAMP_MINFILTER	기존 Sample 된거보다 작게 나오는경우에대한 필터링 방법
	//D3DSAMP_MAGFILTER	기존 Sample 된거보다 크게 나오는경우에대한 필터링 방법
	//D3DSAMP_MIPFILTER 밉맵체인의 경계에서 어떻게 필터링할건지

	//필터옵션
	//D3DTEXF_POINT				한피셀만 본다
	//D3DTEXF_LINEAR			주변픽셀을 선형으로 보간
	//D3DTEXF_ANISOTROPIC		이방성필더링을 통한 보간 예는 레벨을 정할수 있다.






	if (KEY_MGR->IsOnceDown('1')){
		//Texture 의 필터모드
		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}

	else if (KEY_MGR->IsOnceDown('2')){

		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

		//이방성 필터링은 레벨을 조정할수 있다.
		//D3DTEXF_ANISOTROPIC 단계를 설정
		//Device->SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, 16 );  //16 단계 까지의 픽셀보간

		//그래픽카드마다 최대 지원될수 있는 D3DTEXF_ANISOTROPIC 단계 는 다르다.
		//그래서 D3DCAP9 을 통해 확인할수 있다
		Device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, MaxAnisotropic);

	}


	else if (KEY_MGR->IsOnceDown('0')){
		//Texture 의 필터모드
		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	}
}

void cScene_00::Render()
{
	m_quadTrans.SetDeviceWorld(Device);


	//그리기 전에 로딩된 Texture 를 셋팅하자.
	Device->SetTexture(
		0,			//셋팅될 Texture Stage 는 일단 0
		m_pTex		//로딩된 Texture 포인터
		);

	//그리자
	Device->SetStreamSource(0, this->m_vb, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->SetIndices(this->m_ib);
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 0, 4, 0, 2);

	//셋팅된 Texture 를 더이상 그리고 싶지 않다면 해제 해야한다.
	Device->SetTexture(0, NULL);		//포인터 대신 NULL 을 넣으면 해제 된다.
}

void cScene_00::CreateQuad()
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
	pVertices[0].uv = D3DXVECTOR2(0, 100);
	pVertices[1].uv = D3DXVECTOR2(0, 0);
	pVertices[2].uv = D3DXVECTOR2(100, 0);
	pVertices[3].uv = D3DXVECTOR2(100, 100);


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