#include "stdafx.h"
#include "cScene_03.h"


cScene_03::cScene_03()
{
}


cScene_03::~cScene_03()
{
}

HRESULT cScene_03::Init()
{
	//
	// 구메쉬 생성
	//
	if (FAILED(D3DXCreateSphere(Device, 3.0f, 50, 50, &pMesh, NULL)))
		return E_FAIL;

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
		"Phong.fx",		//불러올 셰이더 코드 파일이름
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

void cScene_03::Release()
{
	SAFE_RELEASE(pMesh);

	//Effect 해재
	SAFE_RELEASE(this->pEffect);
}

void cScene_03::Update(float timeDelta)
{
	lightTrans.DefaultControl2(timeDelta);


	//카메라 Default 컨트롤
	mainCamera.DefaultControl(timeDelta);

	//카메라 업데이트
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_03::Render()
{
	//Effect 에 전역 변수값을 셋팅한다.
	D3DXMATRIXA16 matWorld = meshTrans.GetFinalMatrix();
	//월드 행렬 셋팅
	this->pEffect->SetMatrix("matWorld", &matWorld);

	//Device 로부터 셋팅된 View 행렬과 Projection 행렬을 얻는다.
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;
	Device->GetTransform(D3DTS_VIEW, &matView);
	Device->GetTransform(D3DTS_PROJECTION, &matProjection);


	//뷰 행렬 셋팅
	this->pEffect->SetMatrix("matView", &matView);

	//투영 행렬 셋팅
	this->pEffect->SetMatrix("matProjection", &matProjection);

	//컬러 벡터 셋팅 ( 주의 사항  셋팅되는 벡터는 모두 Vector4 로 대동단결된다 )
	D3DXVECTOR4 vMaterialDiffuseColor(1, 0, 0, 1);
	this->pEffect->SetVector("vMaterialDiffuseColor", &vMaterialDiffuseColor);
	D3DXVECTOR4 vMaterialSpecularColor(1, 1, 1, 1);
	this->pEffect->SetVector("vMaterialSpecularColor", &vMaterialSpecularColor);

	//파워
	this->pEffect->SetFloat("fPower", 5.0f);





	//광원의 방향
	D3DXVECTOR4 lightDir(this->lightTrans.GetForward(), 1);
	this->pEffect->SetVector("vLightDir", &lightDir);

	//눈의 위치
	D3DXVECTOR4 eyePos(this->mainCamera.GetWorldPosition(), 1);
	this->pEffect->SetVector("vEyePos", &eyePos);


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
		pMesh->DrawSubset(0);

		this->pEffect->EndPass();			//Pass 끝

	}

	this->pEffect->End();						//Effect 로 랜더링 끝



	this->lightTrans.RenderGimozo(Device);
}