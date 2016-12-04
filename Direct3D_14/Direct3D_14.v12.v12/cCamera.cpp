#include "StdAfx.h"
#include "cCamera.h"


cCamera::cCamera(void)
{
	//기본화각 60 도
	this->fov = 60.0f * ONE_RAD;

	//기본 Near
	this->camNear = 0.1f;

	//기본 Far
	this->camFar = 1000.0f;

}


cCamera::~cCamera(void)
{
}




//투영행렬과 View 행렬을 업데이트
void cCamera::UpdateMatrix()
{
	//화각에 의한 Projection 행렬 업데이트
	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		this->fov,
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),
		this->camNear,
		this->camFar);

	//뷰행렬 카메라 월드위치에 대한 역행렬이다.
	D3DXMatrixInverse(&matView, NULL, &matFinal);


	this->matViewProjection = this->matView * this->matProjection;


}

//View 행렬과 투영 행렬을 셋팅
void cCamera::UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice)
{
	//행렬 업데이트 해주고 
	this->UpdateMatrix();

	//셋팅
	pDevice->SetTransform(D3DTS_VIEW, &this->matView);
	pDevice->SetTransform(D3DTS_PROJECTION, &this->matProjection);


}


//화면의 위치를 가지고 카메라의 투영 레이를 얻는다
void cCamera::ComputeRay(LPRay pOutRay, const D3DXVECTOR2* screenPos)
{
	//Device 의 뷰포트를 얻는다.
	D3DVIEWPORT9 viewPort;
	Device->GetViewport(&viewPort);

	//스크린의 위치 비율을 얻자 
	float factorX = screenPos->x / viewPort.Width;
	float factorY = (1.0f - (screenPos->y / viewPort.Height));

	//0 ~ 1 => -1 ~ 1
	factorX = factorX * 2.0f - 1.0f;
	factorY = factorY * 2.0f - 1.0f;

	//투영행렬을 얻는다.
	D3DXMATRIXA16 matProj = this->GetProjectionMatrix();

	//

	//동차로 위치에 화각 스케일량을 나눈다.
	D3DXVECTOR3 direction(
		factorX / matProj._11,
		factorY / matProj._22,
		1.0f);

	//카메라 월드 행렬
	D3DXMATRIXA16 matCamWorld = GetFinalMatrix();

	//레이의 방향을 동차에서 얻은 벡터를 카메라 월드 매트릭스
	//월드 방향이 나온다.
	D3DXVec3TransformNormal(
		&direction, &direction, &matCamWorld);

	//방향벡터 정규
	D3DXVec3Normalize(&direction, &direction);

	//레이의 방향
	pOutRay->direction = direction;

	//레이의 오리진은 카메라 위치가 된다.
	pOutRay->origin = this->GetWorldPosition();

}