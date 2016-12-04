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
	// ���̴� �ε�
	//

	// ���̴� �ε��� ������ ������ ���� ������ ������ �޾ƿ� Error ����
	LPD3DXBUFFER pError = NULL;

	DWORD				dwShaderFlag = 0;		//���̴� �÷��� 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
#endif

	//fxFile �� ���� Effect ��ü �ε�
	D3DXCreateEffectFromFile(
		Device,						//����̽�
		filePath.c_str(),				//�ҷ��� ���̴� �ڵ� �����̸�
		NULL, 						//���̴��� �������Ҷ� �߰��� ����� #define ���� ( �ϴ� NULL )
		NULL, 						//���̴��� �������Ҷ� #include ���ù��� ó���Ҷ� ����� �������̽� �÷��� ( �ϴ� NULL )
		dwShaderFlag,				//���̴� ������ �÷���
		NULL, 						//���̴� �Ű������� ������ �޸�Ǯ ( �ϴ� NULL )
		&pEffect,					//�ε��� ���̴� Effect ������
		&pError 					//���̴��� �ε��ϰ� �������Ҷ� ������ ����� �ش� ���ۿ� �����޽����� ���� ( ���������� �ε��Ǹ� NULL �� �����ȴ� )
		);

	//�ε��� ������ �ִٸ�....
	if ( pError != NULL && pEffect == NULL )
	{
		//Error ������ ���������Ϳ� �������뿡 ���� ���ڿ��� �����ȴ�.
		LOG_MGR->AddLog((char*)pError->GetBufferPointer());

		//Error ���� Release
		SAFE_RELEASE(pError);
		return NULL;
	}


	return pEffect;
}

void cResourceMgr_ShaderFX::ReleaseResource(LPD3DXEFFECT data)
{
	SAFE_RELEASE(data);
}