#include "stdafx.h"
#include "cScene.h"
#include "cCamera.h"
#include <algorithm>
cScene::cScene()
	: evironmentTexture(NULL), evironmemtSphereMesh(NULL)
{
	//환경 Effect 물어 놓는다.
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
	//씬의 초기화 이루어진다.
	if (FAILED(this->Scene_Init()))
		return E_FAIL;

	return S_OK;
}

void cScene::Release()
{
	//씬의 해제가 일어난다.
	this->Scene_Release();
}

void cScene::Update(float timeDelta)
{
	pMainCamera->CameraUpdate(timeDelta);
	pMainCamera->UpdateCamToDevice(Device);
	this->ChangeCameraMod();
	//씬의 업데이트가 일어난다.
	this->Scene_Update(timeDelta);
}

void cScene::Render()
{
	//환경 랜더
	this->RenderEnvironment();

	//랜더된다.
	this->Scene_Render0();
	this->Scene_Render1();
	this->Scene_Render2();

	//스플라이트 랜더
	SPRITE_MGR->BeginSpriteRender();
	this->Scene_RenderSprite();
	SPRITE_MGR->EndSpriteRender();
}

void cScene::SetEnvironment(std::string cubeFilePath)
{
	//기존의 환경맵이 셋팅되어있을지 몰라..
	SAFE_RELEASE(evironmentTexture);

	//환경맵 로딩
	D3DXCreateCubeTextureFromFile(
		Device,
		cubeFilePath.c_str(),
		&evironmentTexture);

	//환경구 로딩되어있지 않다면....
	if (this->evironmemtSphereMesh == NULL)
	{
		D3DXCreateSphere(Device,
			3.0f,
			20,
			20,
			&evironmemtSphereMesh, NULL);
	}
}

//환경구 랜더
void cScene::RenderEnvironment()
{
	//환경 맵 로딩된게 없다면 리턴해라.....
	if (this->evironmentTexture == NULL)
		return;

	//환경 Effect 셋팅
	this->evironmentEffect->SetTexture("MyCube_Tex", this->evironmentTexture);

	//WVP 매트릭스
	D3DXMATRIXA16 matWorld = this->pMainCamera->GetFinalMatrix();
	D3DXMATRIXA16 matViewProj = this->pMainCamera->GetViewProjectionMatrix();
	D3DXMATRIXA16 matWVP = matWorld * matViewProj;

	this->evironmentEffect->SetMatrix("matWVP", &matWVP);
	this->evironmentEffect->SetMatrix("matWorld", &matWorld);


	//그려라...
	UINT numPass;
	this->evironmentEffect->Begin(&numPass, 0);		//셰이더로 그린다는 것을 알리고 pass 수를 얻는다.

	for (UINT i = 0; i < numPass; i++)	//공정 수대로 돈다.
	{
		this->evironmentEffect->BeginPass(i);			//i번째 공정시작

		this->evironmemtSphereMesh->DrawSubset(0);

		this->evironmentEffect->EndPass();				//i번째 공정끝
	}
	this->evironmentEffect->End();						//셰이더로 그리기 끝


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
