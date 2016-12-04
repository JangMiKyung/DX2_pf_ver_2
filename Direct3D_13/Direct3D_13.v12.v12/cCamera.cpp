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
