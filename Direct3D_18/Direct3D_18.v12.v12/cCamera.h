#pragma once

#include "cTransform.h"
#include <functional>

class cCamera : public cTransform
{
protected:
	float fov;			//카메라 화각
	float camNear;		//카메라 Near
	float camFar;		//카메라 Far


	D3DXMATRIXA16		matView;		//뷰행렬
	D3DXMATRIXA16		matProjection;	//투영행렬
	D3DXMATRIXA16		matViewProjection;	//뷰 * 투영행렬
	cTransform* Body;



public:
	cCamera(void);
	~cCamera(void);

	//투영행렬과 View 행렬을 업데이트
	void UpdateMatrix();
	//View 행렬과 투영 행렬을 셋팅
	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);
	virtual void CameraUpdate(float timeDelta) = 0;

	//카메라 관련 행렬을 얻는다.
	D3DXMATRIXA16 GetViewMatrix()const {
		return this->matView;
	}

	D3DXMATRIXA16 GetProjectionMatrix()const {
		return this->matProjection;
	}

	D3DXMATRIXA16 GetViewProjectionMatrix()const {
		return this->matViewProjection;
	}

	//화각 셋팅
	void SetFov(float fov){
		this->fov = fov;
	}

	float GetFov(){
		return this->fov;
	}

	//화면의 위치를 가지고 카메라의 투영 레이를 얻는다
	void ComputeRay(LPRay pOutRay, const D3DXVECTOR2* screenPos);

	//월드 위치로  화면의 위치를 얻는다.
	bool GetWorldPosToScreenPos(D3DXVECTOR2* pScreenPos, const D3DXVECTOR3* pWorldPos);
};


class PlayerCamera : public cCamera
{
public:
	PlayerCamera() : cCamera()
	{
		//기본화각 60 도
		this->fov = 60.0f * ONE_RAD;

		//기본 Near
		this->camNear = 0.1f;

		//기본 Far
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
		//기본화각 60 도
		this->fov = 60.0f * ONE_RAD;

		//기본 Near
		this->camNear = 0.1f;

		//기본 Far
		this->camFar = 1000.0f;
	}
	void CameraUpdate(float timeDelta)
	{
		this->DefaultControl(timeDelta);
	}
};
