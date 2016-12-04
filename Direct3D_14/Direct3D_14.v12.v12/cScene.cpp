#include "stdafx.h"
#include "cScene.h"


cScene::cScene()
	: evironmentTexture(NULL), evironmemtSphereMesh(NULL)
{
	//ȯ�� Effect ���� ���´�.
	this->evironmentEffect =
		RESOURCE_FX->GetResource("../../Resources/Shaders/EvironmentCUBE.fx");
}


cScene::~cScene()
{
	SAFE_RELEASE(evironmentTexture);
	SAFE_RELEASE(evironmemtSphereMesh);
}

HRESULT cScene::Init()
{
	//���� �ʱ�ȭ �̷������.
	if (FAILED(this->Scene_Init()))
		return E_FAIL;

	return S_OK;
}

void cScene::Release()
{
	//���� ������ �Ͼ��.
	this->Scene_Release();
}

void cScene::Update(float timeDelta)
{
	mainCamera.DefaultControl(timeDelta);
	mainCamera.UpdateCamToDevice(Device);

	//���� ������Ʈ�� �Ͼ��.
	this->Scene_Update(timeDelta);
}

void cScene::Render()
{
	//ȯ�� ����
	this->RenderEnvironment();

	//�����ȴ�.
	this->Scene_Render0();
	this->Scene_Render1();
	this->Scene_Render2();

	//���ö���Ʈ ����
	this->Scene_RenderSprite();
}

void cScene::SetEnvironment(std::string cubeFilePath)
{
	//������ ȯ����� ���õǾ������� ����..
	SAFE_RELEASE(evironmentTexture);

	//ȯ��� �ε�
	D3DXCreateCubeTextureFromFile(
		Device,
		cubeFilePath.c_str(),
		&evironmentTexture);

	//ȯ�汸 �ε��Ǿ����� �ʴٸ�....
	if (this->evironmemtSphereMesh == NULL)
	{
		D3DXCreateSphere(Device,
			3.0f,
			20,
			20,
			&evironmemtSphereMesh, NULL);
	}
}

//ȯ�汸 ����
void cScene::RenderEnvironment()
{
	//ȯ�� �� �ε��Ȱ� ���ٸ� �����ض�.....
	if (this->evironmentTexture == NULL)
		return;

	//ȯ�� Effect ����
	this->evironmentEffect->SetTexture("MyCube_Tex", this->evironmentTexture);

	//WVP ��Ʈ����
	D3DXMATRIXA16 matWorld = this->mainCamera.GetFinalMatrix();
	D3DXMATRIXA16 matViewProj = this->mainCamera.GetViewProjectionMatrix();
	D3DXMATRIXA16 matWVP = matWorld * matViewProj;

	this->evironmentEffect->SetMatrix("matWVP", &matWVP);
	this->evironmentEffect->SetMatrix("matWorld", &matWorld);


	//�׷���...
	UINT numPass;
	this->evironmentEffect->Begin(&numPass, 0);		//���̴��� �׸��ٴ� ���� �˸��� pass ���� ��´�.

	for (UINT i = 0; i < numPass; i++)	//���� ����� ����.
	{
		this->evironmentEffect->BeginPass(i);			//i��° ��������

		this->evironmemtSphereMesh->DrawSubset(0);

		this->evironmentEffect->EndPass();				//i��° ������
	}
	this->evironmentEffect->End();						//���̴��� �׸��� ��


}