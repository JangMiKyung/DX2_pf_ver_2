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
	
	//큐브 생성
	this->CreateCube();

	//
	// 셰이더 로딩
	//

	// 셰이더 로딩시 컴파일 에러가 나면 에러의 내용을 받아올 Error 버퍼
	LPD3DXBUFFER pError = NULL;

	DWORD	dwShaderFlag = 0;		//셰이더 플레그 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//셰이더를 디버그모드로 컴파일하겠다 ( 디버그모드로 해야 잘못된 컴파일 오류가 날때 Error 버퍼에 오류정보가 들어간다 ) 
#endif

	//fxFile 로 부터 Effect 객체 로딩
	D3DXCreateEffectFromFile(
		Device,						//디바이스
		"HSLS_BaseColor.fx",		//불러올 셰이더 코드 파일이름
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

	//셰이더 로딩이 잘되었다...
	//핸들을 받아온다.
	hTechnique = pEffect->GetTechniqueByName("MyShader");				//테크닉 핸들

	//전역변수 핸들
	hMatWorld = pEffect->GetParameterByName(NULL, "matWorld");
	hMatView = pEffect->GetParameterByName(NULL, "matView");
	hMatProjection = pEffect->GetParameterByName(NULL, "matProjection");
	hVColor = pEffect->GetParameterByName(NULL, "vColor");

	return S_OK;
}

void cScene_00::Release()
{
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);

	//Effect 해재
	SAFE_RELEASE(this->pEffect);

	//핸들은 할필요 없다
}

void cScene_00::Update(float timeDelta)
{
	cubeTrans.DefaultControl2(timeDelta);




	//카메라 Default 컨트롤
	mainCamera.DefaultControl(timeDelta);

	//카메라 업데이트
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_00::Render()
{
	//Effect 에 전역 변수값을 셋팅한다.
	D3DXMATRIXA16 matWorld = cubeTrans.GetFinalMatrix();

	//월드 행렬 셋팅
	this->pEffect->SetMatrix(hMatWorld, &matWorld);

	//Device 로부터 셋팅된 View 행렬과 Projection 행렬을 얻는다.
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;
	Device->GetTransform(D3DTS_VIEW, &matView);
	Device->GetTransform(D3DTS_PROJECTION, &matProjection);

	//뷰 행렬 셋팅
	this->pEffect->SetMatrix("matView", &matView);

	//투영 행렬 셋팅
	this->pEffect->SetMatrix(hMatProjection, &matProjection);

	//컬러 벡터 셋팅 ( 주의 사항  셋팅되는 벡터는 모두 Vector4 로 대동단결된다 )
	D3DXVECTOR4 color(0, 1, 0, 1);//RGBA
	this->pEffect->SetVector(hVColor, &color);

	//Technique 셋팅
	this->pEffect->SetTechnique(this->hTechnique);

	//셰이더로 Cube 랜더
	UINT iPassNum = 0;		//패스수

	//Effect 로 랜더링 시작 ( 이때 Pass 수를 UINT 형으로 받아온다 )
	this->pEffect->Begin(
		&iPassNum,		//해당 Effect 에 Pass 수를 받는다.
		0				//플레그 일단 NULL
		);

	//Pass 수대로 for 문 돌아재낀다.
	for (UINT i = 0; i < iPassNum; i++)
	{
		this->pEffect->BeginPass(i);		//i 번째 Pass 시작

		//랜더
		RenderCube();

		this->pEffect->EndPass();			//Pass 끝
	}

	this->pEffect->End();					//Effect 로 랜더링 끝
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

	//저장될 메모리풀
	//D3DPOOL_DEFAULT			//정점버퍼의 메모리는 시스템사양에 맞춰 알아서 저장이되는데 보통은 비디오 카드메모리에 저장이된다.
	//D3DPOOL_MANAGED			//정점버퍼의 메모리는 시스템의 메모리영역에 저장이되고 비디오카드 메모리를 사용한다면 비디오카드 메모리에도 저장이된다, 즉 버퍼의 원본내용이 System 메모리에 저장이된 상태에서 비디오카드 메모리에 그거와 똑같은 사본을 만들어쓰는 방식인데... 이는 Device 가 해제 되어도 버퍼의 내용을 다시 복구할 수 있는 여지가 있다.


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
	vertices[0].normal = D3DXVECTOR3(0, 0, -1);
	vertices[1].normal = D3DXVECTOR3(0, 0, -1);
	vertices[2].normal = D3DXVECTOR3(0, 0, -1);
	vertices[3].normal = D3DXVECTOR3(0, 0, -1);


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//우측면 복사
	memcpy(vertices + 4, quad, sizeof(MYVERTEX) * 4);
	vertices[4].normal = D3DXVECTOR3(1, 0, 0);
	vertices[5].normal = D3DXVECTOR3(1, 0, 0);
	vertices[6].normal = D3DXVECTOR3(1, 0, 0);
	vertices[7].normal = D3DXVECTOR3(1, 0, 0);


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//뒷면 복사
	memcpy(vertices + 8, quad, sizeof(MYVERTEX) * 4);
	vertices[8].normal = D3DXVECTOR3(0, 0, 1);
	vertices[9].normal = D3DXVECTOR3(0, 0, 1);
	vertices[10].normal = D3DXVECTOR3(0, 0, 1);
	vertices[11].normal = D3DXVECTOR3(0, 0, 1);


	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//좌측면 복사
	memcpy(vertices + 12, quad, sizeof(MYVERTEX) * 4);
	vertices[12].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[13].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[14].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[15].normal = D3DXVECTOR3(-1, 0, 0);



	D3DXMatrixRotationZ(&matRot, -90.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//윗면 복사
	memcpy(vertices + 16, quad, sizeof(MYVERTEX) * 4);
	vertices[16].normal = D3DXVECTOR3(0, 1, 0);
	vertices[17].normal = D3DXVECTOR3(0, 1, 0);
	vertices[18].normal = D3DXVECTOR3(0, 1, 0);
	vertices[19].normal = D3DXVECTOR3(0, 1, 0);


	D3DXMatrixRotationZ(&matRot, -180.0f * ONE_RAD);

	//돌려
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//아랫면 복사
	memcpy(vertices + 20, quad, sizeof(MYVERTEX) * 4);
	vertices[20].normal = D3DXVECTOR3(0, -1, 0);
	vertices[21].normal = D3DXVECTOR3(0, -1, 0);
	vertices[22].normal = D3DXVECTOR3(0, -1, 0);
	vertices[23].normal = D3DXVECTOR3(0, -1, 0);


	//버퍼 Lock
	void* pVertices = NULL;
	this->vb->Lock(0, 0, &pVertices, 0);

	//얻어온 포인터에 정점정보 메모리 복사 
	memcpy(pVertices, vertices, sizeof(MYVERTEX) * 24);

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

void cScene_00::RenderCube()
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