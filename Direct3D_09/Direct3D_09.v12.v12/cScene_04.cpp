#include "stdafx.h"
#include "cScene_04.h"


cScene_04::cScene_04()
{
}


cScene_04::~cScene_04()
{
}


HRESULT cScene_04::Init()
{
	//
	// ���޽� ����
	//
	if (FAILED(D3DXCreateSphere(Device, 3.0f, 50, 50, &pMesh, NULL)))
		return E_FAIL;

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
		"BlinnPhong.fx",			//�ҷ��� ���̴� �ڵ� �����̸�
		NULL, 						//���̴��� �������Ҷ� �߰��� ����� #define ���� ( �ϴ� NULL )
		NULL, 						//���̴��� �������Ҷ� #include ���ù��� ó���Ҷ� ����� �������̽� �÷��� ( �ϴ� NULL )
		dwShaderFlag,				//���̴� ������ �÷���
		NULL, 						//���̴� �Ű������� ������ �޸�Ǯ ( �ϴ� NULL )
		&pEffect,					//�ε��� ���̴� Effect ������
		&pError 					//���̴��� �ε��ϰ� �������Ҷ� ������ ����� �ش� ���ۿ� �����޽����� ���� ( ���������� �ε��Ǹ� NULL �� �����ȴ� )
		);

	//�ε��� ������ �ִٸ�....
	if (pError != NULL)
	{
		//Error ������ ���������Ϳ� �������뿡 ���� ���ڿ��� �����ȴ�.
		LOG_MGR->AddLog((char*)pError->GetBufferPointer());

		//Error ���� Release
		SAFE_RELEASE(pError);
		return E_FAIL;
	}



	return S_OK;
}

void cScene_04::Release()
{
	SAFE_RELEASE(pMesh);

	//Effect ����
	SAFE_RELEASE(this->pEffect);
}

void cScene_04::Update(float timeDelta)
{
	lightTrans.DefaultControl2(timeDelta);


	//ī�޶� Default ��Ʈ��
	mainCamera.DefaultControl(timeDelta);

	//ī�޶� ������Ʈ
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_04::Render()
{
	//Effect �� ���� �������� �����Ѵ�.
	D3DXMATRIXA16 matWorld = meshTrans.GetFinalMatrix();
	//���� ��� ����
	this->pEffect->SetMatrix("matWorld", &matWorld);

	//Device �κ��� ���õ� View ��İ� Projection ����� ��´�.
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;
	Device->GetTransform(D3DTS_VIEW, &matView);
	Device->GetTransform(D3DTS_PROJECTION, &matProjection);


	//�� ��� ����
	this->pEffect->SetMatrix("matView", &matView);

	//���� ��� ����
	this->pEffect->SetMatrix("matProjection", &matProjection);

	//�÷� ���� ���� ( ���� ����  ���õǴ� ���ʹ� ��� Vector4 �� �뵿�ܰ�ȴ� )
	D3DXVECTOR4 vMaterialDiffuseColor(1, 0, 0, 1);
	this->pEffect->SetVector("vMaterialDiffuseColor", &vMaterialDiffuseColor);
	D3DXVECTOR4 vMaterialSpecularColor(1, 1, 1, 1);
	this->pEffect->SetVector("vMaterialSpecularColor", &vMaterialSpecularColor);

	//�Ŀ�
	this->pEffect->SetFloat("fPower", 30.0f);





	//������ ����
	D3DXVECTOR4 lightDir(this->lightTrans.GetForward(), 1);
	this->pEffect->SetVector("vLightDir", &lightDir);

	//���� ��ġ
	D3DXVECTOR4 eyePos(this->mainCamera.GetWorldPosition(), 1);
	this->pEffect->SetVector("vEyePos", &eyePos);


	//Technique ����
	this->pEffect->SetTechnique("MyShader");


	//���̴��� Cube ����
	UINT iPassNum = 0;		//�н���

	//Effect �� ������ ���� ( �̶� Pass ���� UINT ������ �޾ƿ´� )
	this->pEffect->Begin(
		&iPassNum,		//�ش� Effect �� Pass ���� �޴´�.
		0				//�÷��� �ϴ� NULL
		);

	//Pass ����� for �� �����糤��.
	for (UINT i = 0; i < iPassNum; i++){

		this->pEffect->BeginPass(i);		//i ��° Pass ����

		//����
		pMesh->DrawSubset(0);

		this->pEffect->EndPass();			//Pass ��

	}

	this->pEffect->End();						//Effect �� ������ ��



	this->lightTrans.RenderGimozo(Device);
}