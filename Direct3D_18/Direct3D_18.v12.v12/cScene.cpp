#include "stdafx.h"
#include "cScene.h"
#include "cCamera.h"
#include <algorithm>
cScene::cScene()
	: evironmentTexture(NULL), evironmemtSphereMesh(NULL)
{
	//ȯ�� Effect ���� ���´�.
	this->evironmentEffect =
		RESOURCE_FX->GetResource("../../Resources/Shaders/EvironmentCUBE.fx");


	cCamera* DebuggingCamera = new FreeCamera;
	cCamera* MilesCamera = new PlayerCamera;

	vCamera.push_back(MilesCamera);
	vCamera.push_back(DebuggingCamera);

	viCamera = vCamera.begin();
	pMainCamera = vCamera[PLAYER];

}


cScene::~cScene()
{
	SAFE_RELEASE(evironmentTexture);
	SAFE_RELEASE(evironmemtSphereMesh);

	SAFE_DELETE(this->pMainCamera);
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
	pMainCamera->CameraUpdate(timeDelta);
	pMainCamera->UpdateCamToDevice(Device);
	this->ChangeCameraMod();
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
	SPRITE_MGR->BeginSpriteRender();
	this->Scene_RenderSprite();
	SPRITE_MGR->EndSpriteRender();
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
	D3DXMATRIXA16 matWorld = this->pMainCamera->GetFinalMatrix();
	D3DXMATRIXA16 matViewProj = this->pMainCamera->GetViewProjectionMatrix();
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

void cScene::ChangeCameraMod()
{
	if (KEY_MGR->IsOnceDown(VK_TAB)) {
		viCamera++;

		if (viCamera == vCamera.end()) {
			viCamera = vCamera.begin();
		}
		pMainCamera = (*viCamera);
	}
}

void cScene::ChangeCameraMod(int IDx)
{
	if (*(vCamera.begin() + IDx) != NULL) {
		pMainCamera = *(vCamera.begin() + IDx);
	}
}
