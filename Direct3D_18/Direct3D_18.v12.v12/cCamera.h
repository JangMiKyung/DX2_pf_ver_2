#pragma once

#include "cTransform.h"
#include <functional>

class cCamera : public cTransform
{
protected:
	float fov;			//ī�޶� ȭ��
	float camNear;		//ī�޶� Near
	float camFar;		//ī�޶� Far


	D3DXMATRIXA16		matView;		//�����
	D3DXMATRIXA16		matProjection;	//�������
	D3DXMATRIXA16		matViewProjection;	//�� * �������
	cTransform* Body;



public:
	cCamera(void);
	~cCamera(void);

	//������İ� View ����� ������Ʈ
	void UpdateMatrix();
	//View ��İ� ���� ����� ����
	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void CameraUpdate(float timeDelta) = 0;

	//ī�޶� ���� ����� ��´�.
	D3DXMATRIXA16 GetViewMatrix()const {
		return this->matView;
	}

	D3DXMATRIXA16 GetProjectionMatrix()const {
		return this->matProjection;
	}

	D3DXMATRIXA16 GetViewProjectionMatrix()const {
		return this->matViewProjection;
	}

	//ȭ�� ����
	void SetFov(float fov){
		this->fov = fov;
	}

	float GetFov(){
		return this->fov;
	}

	//ȭ���� ��ġ�� ������ ī�޶��� ���� ���̸� ��´�
	void ComputeRay(LPRay pOutRay, const D3DXVECTOR2* screenPos);

	//���� ��ġ��  ȭ���� ��ġ�� ��´�.
	bool GetWorldPosToScreenPos(D3DXVECTOR2* pScreenPos, const D3DXVECTOR3* pWorldPos);
};


class PlayerCamera : public cCamera
{
public:
	PlayerCamera() : cCamera()
	{
		//�⺻ȭ�� 60 ��
		this->fov = 60.0f * ONE_RAD;

		//�⺻ Near
		this->camNear = 0.1f;

		//�⺻ Far
		this->camFar = 1000.0f;
	}

	void CameraUpdate(float timeDelta)
	{
		this->DefaultControl3(timeDelta);
	}
};

class FreeCamera : public cCamera
{
public:
	FreeCamera() : cCamera()
	{
		//�⺻ȭ�� 60 ��
		this->fov = 60.0f * ONE_RAD;

		//�⺻ Near
		this->camNear = 0.1f;

		//�⺻ Far
		this->camFar = 1000.0f;
	}
	void CameraUpdate(float timeDelta)
	{
		this->DefaultControl(timeDelta);
	}
};
