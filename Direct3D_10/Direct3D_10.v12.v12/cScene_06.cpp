#include "stdafx.h"
#include "cScene_06.h"


cScene_06::cScene_06()
{
}


cScene_06::~cScene_06()
{
}

HRESULT cScene_06::Init()
{
	//
	// 쿼드 생성
	//
	CreateQuad();

	//
	// 정점 선언부를 만든다.
	//

	//정점의 형태를 알려주는 배열
	D3DVERTEXELEMENT9 vertElement[6];			//배열을 정점정보 갯수 + 1

	//Position 
	vertElement[0].Stream = 0;							//Stream 은 0
	vertElement[0].Offset = 0;							//메모리의 시작 Byte 단위 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			//자료의 타입
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		//정보 타입
	vertElement[0].UsageIndex = 0;						//UsageIndex 일단 0


	//UV
	vertElement[1].Stream = 0;					//Stream 은 0
	vertElement[1].Offset = 12;					//메모리의 시작 Byte 단위 0
	vertElement[1].Type = D3DDECLTYPE_FLOAT2;	//자료의 타입
	vertElement[1].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[1].Usage = D3DDECLUSAGE_TEXCOORD;		//정보 타입
	vertElement[1].UsageIndex = 0;						//UsageIndex 일단 0

	//Normal
	vertElement[2].Stream = 0;					//Stream 은 0
	vertElement[2].Offset = 20;					//메모리의 시작 Byte 단위 0
	vertElement[2].Type = D3DDECLTYPE_FLOAT3;	//자료의 타입
	vertElement[2].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[2].Usage = D3DDECLUSAGE_NORMAL;		//정보 타입
	vertElement[2].UsageIndex = 0;						//UsageIndex 일단 0

	//binormal
	vertElement[3].Stream = 0;					//Stream 은 0
	vertElement[3].Offset = 32;					//메모리의 시작 Byte 단위 0
	vertElement[3].Type = D3DDECLTYPE_FLOAT3;	//자료의 타입
	vertElement[3].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[3].Usage = D3DDECLUSAGE_BINORMAL;		//정보 타입
	vertElement[3].UsageIndex = 0;						//UsageIndex 일단 0


	//Tangent
	vertElement[4].Stream = 0;					//Stream 은 0
	vertElement[4].Offset = 44;					//메모리의 시작 Byte 단위 0
	vertElement[4].Type = D3DDECLTYPE_FLOAT3;	//자료의 타입
	vertElement[4].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[4].Usage = D3DDECLUSAGE_TANGENT;		//정보 타입
	vertElement[4].UsageIndex = 0;						//UsageIndex 일단 0


	//더이상 없어
	D3DVERTEXELEMENT9 end = D3DDECL_END(); //{0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
	vertElement[5] = end;


	//
	// LPDIRECT3DVERTEXDECLARATION9 생성
	//
	Device->CreateVertexDeclaration(
		vertElement,		//앞에서 만든 D3DVERTEXELEMENT9 배열 포인터
		&m_pVDecl			//얻어올 LPDIRECT3DVERTEXDECLARATION9 포인터
		);

	//
	// Texture 로딩 
	//
	//기본적으로 밉맵체인이 만들어진다.
	D3DXCreateTextureFromFile(
		Device,							//디바이스
		"terrain4.png",					//경로포함한 파일명
		&this->m_pDiffuseTex			//얻어올 IDirect3DTexture9 더블포인터
		);

	D3DXCreateTextureFromFile(
		Device,							//디바이스
		"Stone-normal.jpg",					//경로포함한 파일명
		&this->m_pNormalTex			//얻어올 IDirect3DTexture9 더블포인터
		);

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
		"TextureNormal2.fx",			//불러올 셰이더 코드 파일이름
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

void cScene_06::Release()
{
	SAFE_RELEASE(this->pEffect);

	SAFE_RELEASE(this->m_vb);
	SAFE_RELEASE(this->m_ib);

	//해제
	SAFE_RELEASE(this->m_pDiffuseTex);

	//해제
	SAFE_RELEASE(this->m_pVDecl);
}

void cScene_06::Update(float timeDelta)
{
	m_mainCamera.DefaultControl(timeDelta);
	if (KEY_MGR->IsStayDown(VK_LSHIFT))
		m_lightTrans.DefaultControl2(timeDelta);
	else
		m_quadTrans.DefaultControl2(timeDelta);

	m_mainCamera.UpdateCamToDevice(Device);
}

void cScene_06::Render()
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

	//월드 행렬
	this->pEffect->SetMatrix("matWorld", &matWorld);

	//광원 방향
	D3DXVECTOR3 lightDir = m_lightTrans.GetForward();
	this->pEffect->SetVector("vLightDir", &D3DXVECTOR4(lightDir, 1));

	//시선 위치
	D3DXVECTOR3 eyePos = m_mainCamera.GetWorldPosition();
	this->pEffect->SetVector("vEyePos", &D3DXVECTOR4(eyePos, 1));

	//POwer
	this->pEffect->SetFloat("specPower", 10);


	//POwer
	this->pEffect->SetTexture("DiffuseTex", m_pDiffuseTex);
	this->pEffect->SetTexture("NormalTex", m_pNormalTex);


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
		//Device->SetFVF( MYVERTEX::FVF );

		//FVF 대신셋팅
		Device->SetVertexDeclaration(m_pVDecl);



		Device->SetIndices(this->m_ib);
		Device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0, 0, 4, 0, 2);


		this->pEffect->EndPass();			//Pass 끝

	}

	this->pEffect->End();						//Effect 로 랜더링 끝



	this->m_lightTrans.RenderGimozo();
}

void cScene_06::CreateQuad()
{
	//정점 위치
	D3DXVECTOR3 vertexPos[] = {
		D3DXVECTOR3(-3.5f, -3.5f, 0.0f),
		D3DXVECTOR3(-3.5f, 3.5f, 0.0f),
		D3DXVECTOR3(3.5f, 3.5f, 0.0f),
		D3DXVECTOR3(3.5f, -3.5f, 0.0f)
	};

	//정점 UV
	D3DXVECTOR2 vertexUV[] = {
		D3DXVECTOR2(0, 1),
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1)
	};

	//정점 인덱스
	DWORD vertexIndices[] = { 0, 1, 2, 0, 2, 3 };

	//정점 노말
	D3DXVECTOR3 vertexNormal[4];
	ComputeNormal(vertexNormal, vertexPos, 4, vertexIndices, 6);

	//Tangent bi normal
	D3DXVECTOR3 vertexTangent[4];
	D3DXVECTOR3 vertexBinormal[4];
	ComputeTangentAngBinormal(
		vertexTangent,
		vertexBinormal,
		vertexPos,
		vertexNormal,
		vertexUV,
		vertexIndices,
		2,
		4);



	//정점 버퍼 생성
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 4,
		D3DUSAGE_WRITEONLY,
		0,						//FVF 쓰지말자 0 써라... 
		D3DPOOL_DEFAULT,
		&this->m_vb,
		NULL);


	LPMYVERTEX pVertices = NULL;
	this->m_vb->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 4; i++){
		pVertices[i].pos = vertexPos[i];
		pVertices[i].uv = vertexUV[i];
		pVertices[i].normal = vertexNormal[i];
		pVertices[i].tangent = vertexTangent[i];
		pVertices[i].binormal = vertexBinormal[i];
	}

	this->m_vb->Unlock();


	//
	// 인덱스 버퍼
	//
	Device->CreateIndexBuffer(
		sizeof(DWORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&this->m_ib,
		NULL);

	void* pIndices = NULL;

	this->m_ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, vertexIndices, sizeof(DWORD) * 6);

	this->m_ib->Unlock();
}