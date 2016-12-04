#include "StdAfx.h"
#include "cCamera.h"


cCamera::cCamera(void)
{
	//�⺻ȭ�� 60 ��
	this->fov = 60.0f * ONE_RAD;

	//�⺻ Near
	this->camNear = 0.1f;

	//�⺻ Far
	this->camFar = 1000.0f;

}


cCamera::~cCamera(void)
{
}




//������İ� View ����� ������Ʈ
void cCamera::UpdateMatrix()
{
	//ȭ���� ���� Projection ��� ������Ʈ
	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		this->fov,
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),
		this->camNear,
		this->camFar);

	//����� ī�޶� ������ġ�� ���� ������̴�.
	D3DXMatrixInverse(&matView, NULL, &matFinal);


	this->matViewProjection = this->matView * this->matProjection;


}

//View ��İ� ���� ����� ����
void cCamera::UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice)
{
	//��� ������Ʈ ���ְ� 
	this->UpdateMatrix();

	//����
	pDevice->SetTransform(D3DTS_VIEW, &this->matView);
	pDevice->SetTransform(D3DTS_PROJECTION, &this->matProjection);


}


//ȭ���� ��ġ�� ������ ī�޶��� ���� ���̸� ��´�
void cCamera::ComputeRay(LPRay pOutRay, const D3DXVECTOR2* screenPos)
{
	//Device �� ����Ʈ�� ��´�.
	D3DVIEWPORT9 viewPort;
	Device->GetViewport(&viewPort);

	//��ũ���� ��ġ ������ ���� 
	float factorX = screenPos->x / viewPort.Width;
	float factorY = (1.0f - (screenPos->y / viewPort.Height));

	//0 ~ 1 => -1 ~ 1
	factorX = factorX * 2.0f - 1.0f;
	factorY = factorY * 2.0f - 1.0f;

	//��������� ��´�.
	D3DXMATRIXA16 matProj = this->GetProjectionMatrix();

	//

	//������ ��ġ�� ȭ�� �����Ϸ��� ������.
	D3DXVECTOR3 direction(
		factorX / matProj._11,
		factorY / matProj._22,
		1.0f);

	//ī�޶� ���� ���
	D3DXMATRIXA16 matCamWorld = GetFinalMatrix();

	//������ ������ �������� ���� ���͸� ī�޶� ���� ��Ʈ����
	//���� ������ ���´�.
	D3DXVec3TransformNormal(
		&direction, &direction, &matCamWorld);

	//���⺤�� ����
	D3DXVec3Normalize(&direction, &direction);

	//������ ����
	pOutRay->direction = direction;

	//������ �������� ī�޶� ��ġ�� �ȴ�.
	pOutRay->origin = this->GetWorldPosition();

}