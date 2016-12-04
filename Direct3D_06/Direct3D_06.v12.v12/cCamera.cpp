#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
{
	//기본 화각 60
	this->fov = 60.f * ONE_RAD;

	//기본 near
	this->camNear = 0.1f;

	this->camFar = 1000.f;
}


cCamera::~cCamera()
{
}

//디폴트 컨트롤
void cCamera::DefaultControl(float timeDelta)
{
	//디폴트 컨트롤을 위한 카메라 Angle 값
	static float nowAngleH = 0.0f;
	static float nowAngleV = 0.0f;
	static float maxAngleV = 80.5f;
	static float minAngleV = -85.0f;
	static float sensitivityH = 1.0f;
	static float sensitivityV = 1.0f;
	static D3DXVECTOR3 nowVelocity(0, 0, 0);

	static float accelate = 30.f; //초당 이동 증가값
	static float nowSpeed = 3.0f;
	static float maxSpeed = 10.f;

	//최초 누를 때는 마우스 위치를 가운데로 놓고 시작
	if (KEY_MGR->IsOnceDown(VK_RBUTTON))
	{
		//화면의 중심 위치
		int screenCenterX = WINSIZE_X / 2;
		int screenCenterY = WINSIZE_Y / 2;

		//다시 마우스 위치를 센터로
		SetMousePos(screenCenterX, screenCenterY);
	}
	//우클릭을 할때만 Default 컨트롤 을 한다
	else if (KEY_MGR->IsStayDown(VK_RBUTTON))
	{
		//이동처리

		//입력 방향벡터
		D3DXVECTOR3 inputVector(0.f, 0.f, 0.f);
		if (KEY_MGR->IsStayDown('W')) {
			inputVector.z = 1.f;
		}
		else if (KEY_MGR->IsStayDown('S'))	{
			inputVector.z = -1.f;
		}

		if (KEY_MGR->IsStayDown('A')){
			inputVector.x = -1.f;
		}
		else if (KEY_MGR->IsStayDown('D'))	{
			inputVector.x = 1.f;
		}

		if (KEY_MGR->IsStayDown('R')){
			inputVector.y = 1.f;
		}
		else if (KEY_MGR->IsStayDown('F'))	{
			inputVector.y = -1.f;
		}

		//제로 벡터가 아닐 때
		if ( VECTORZERO(inputVector) == false )
		{
			//정규화
			D3DXVec3Normalize(&inputVector, &inputVector);
		}


		/*
		//
		//가속 개념 있는 이동
		//


		//타겟 벡터
		D3DXVECTOR3 target = inputVector * maxSpeed;
		D3DXVec3TransformNormal(&target, &target, &this->matFinal);

		//타겟 벡터와 현재 Veclocity 크기차
		float targetLength = D3DXVec3Length(&target);
		float nowVelocityLength = D3DXVec3Length(&nowVelocity);
		float dist = abs(targetLength - nowVelocityLength);

		//타겟과 차이가 존재한다면...
		if (FLOATZERO(dist) == false)
		{
			//변화 허용량
			float delta = accelate * timeDelta;

			//보간 값
			float t = Clamp01(delta / dist);

			//이번프레임에 타겟까지 변화 하는데 충분하다
			if (FLOATEQUAL(t, 1.0f))
			{
				nowVelocity = target;
			}
			else
			{
				nowVelocity = VecLerp(nowVelocity, target, t);
			}
		}

		//이동 벡터는
		D3DXVECTOR3 moveDeltaVector = nowVelocity * timeDelta;
		//이동해라
		this->MovePositionWorld(moveDeltaVector);
		*/

		//
		// 가속개념 없는 이동
		//

		//이동 해라
		//타겟벡터 
		D3DXVECTOR3 target = inputVector * maxSpeed;
		this->MovePositionSelf(target * timeDelta);

		//
		// 회전 처리
		// 

		//화면의 중심위치
		int screenCenterX = WINSIZE_X / 2;
		int screenCenterY = WINSIZE_Y / 2;

		//현재 마우스 위치
		POINT mousePos = GetMousePos();

		//이동량 ( 중앙에서 멀어진 량 )
		float deltaX = mousePos.x - screenCenterX;
		float deltaY = mousePos.y - screenCenterY;

		//앵글 추가
		nowAngleH += deltaX * sensitivityH;
		nowAngleV += deltaY * sensitivityV;

		//앵글값을 min max 범위 안으로
		nowAngleV = Clamp(nowAngleV, minAngleV, maxAngleV);

		//다시 마우스 위치를 센터로...
		SetMousePos(screenCenterX, screenCenterY);

		this->SetRotate(nowAngleV * ONE_RAD, nowAngleH * ONE_RAD, 0.0f);
	}

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

	//뷰행렬 카메라 월드위치에 대한 역행렬
	D3DXMatrixInverse(&matView, NULL, &matFinal);

	this->matViewProjection = this->matView * this->matProjection;
}

//View 행렬과 투영 행렬을 셋팅
void cCamera::UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice)
{
	//행렬 업데이트 해주자
	this->UpdateMatrix();

	//셋팅
	pDevice->SetTransform(D3DTS_VIEW, &this->matView);
	pDevice->SetTransform(D3DTS_PROJECTION, &this->matProjection);
}
