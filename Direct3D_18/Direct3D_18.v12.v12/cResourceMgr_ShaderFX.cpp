#include "stdafx.h"
#include "cResourceMgr_ShaderFX.h"


cResourceMgr_ShaderFX::cResourceMgr_ShaderFX()
{
}


cResourceMgr_ShaderFX::~cResourceMgr_ShaderFX()
{
}

LPD3DXEFFECT cResourceMgr_ShaderFX::LoadResource(std::string filePath, void* pParam /*= NULL*/)
{
	LPD3DXEFFECT pEffect = NULL;

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
		filePath.c_str(),				//불러올 셰이더 코드 파일이름
		NULL, 						//셰이더를 컴파일할때 추가로 사용할 #define 정의 ( 일단 NULL )
		NULL, 						//셰이더를 컴파일할때 #include 지시문을 처리할때 사용할 인터페이스 플레그 ( 일단 NULL )
		dwShaderFlag,				//셰이더 컴파일 플레그
		NULL, 						//셰이더 매개변수를 공유할 메모리풀 ( 일단 NULL )
		&pEffect,					//로딩될 셰이더 Effect 포인터
		&pError 					//셰이더를 로딩하고 컴파일할때 문제가 생기면 해당 버퍼에 에러메시지가 들어간다 ( 성공적으로 로딩되면 NULL 이 참조된다 )
		);

	//로딩에 문제가 있다면....
	if ( pError != NULL && pEffect == NULL )
	{
		//Error 버퍼의 버퍼포인터에 에러내용에 관한 문자열이 참조된다.
		LOG_MGR->AddLog((char*)pError->GetBufferPointer());

		//Error 버퍼 Release
		SAFE_RELEASE(pError);
		return NULL;
	}


	return pEffect;
}

void cResourceMgr_ShaderFX::ReleaseResource(LPD3DXEFFECT data)
{
	SAFE_RELEASE(data);
}