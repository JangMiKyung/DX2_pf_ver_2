#include "stdafx.h"
#include "cCamera.h"


cCamera::cCamera()
{
	//�⺻ ȭ�� 60
	this->fov = 60.f * ONE_RAD;

	//�⺻ near
	this->camNear = 0.1f;

	this->camFar = 1000.f;
}


cCamera::~cCamera()
{
}

//����Ʈ ��Ʈ��
void cCamera::DefaultControl(float timeDelta)
{
	//����Ʈ ��Ʈ���� ���� ī�޶� Angle ��
	static float nowAngleH = 0.0f;
	static float nowAngleV = 0.0f;
	static float maxAngleV = 80.5f;
	static float minAngleV = -85.0f;
	static float sensitivityH = 1.0f;
	static float sensitivityV = 1.0f;
	static D3DXVECTOR3 nowVelocity(0, 0, 0);

	static float accelate = 30.f; //�ʴ� �̵� ������
	static float nowSpeed = 3.0f;
	static float maxSpeed = 10.f;

	//���� ���� ���� ���콺 ��ġ�� ����� ���� ����
	if (KEY_MGR->IsOnceDown(VK_RBUTTON))
	{
		//ȭ���� �߽� ��ġ
		int screenCenterX = WINSIZE_X / 2;
		int screenCenterY = WINSIZE_Y / 2;

		//�ٽ� ���콺 ��ġ�� ���ͷ�
		SetMousePos(screenCenterX, screenCenterY);
	}
	//��Ŭ���� �Ҷ��� Default ��Ʈ�� �� �Ѵ�
	else if (KEY_MGR->IsStayDown(VK_RBUTTON))
	{
		//�̵�ó��

		//�Է� ���⺤��
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

		//���� ���Ͱ� �ƴ� ��
		if ( VECTORZERO(inputVector) == false )
		{
			//����ȭ
			D3DXVec3Normalize(&inputVector, &inputVector);
		}


		/*
		//
		//���� ���� �ִ� �̵�
		//


		//Ÿ�� ����
		D3DXVECTOR3 target = inputVector * maxSpeed;
		D3DXVec3TransformNormal(&target, &target, &this->matFinal);

		//Ÿ�� ���Ϳ� ���� Veclocity ũ����
		float targetLength = D3DXVec3Length(&target);
		float nowVelocityLength = D3DXVec3Length(&nowVelocity);
		float dist = abs(targetLength - nowVelocityLength);

		//Ÿ�ٰ� ���̰� �����Ѵٸ�...
		if (FLOATZERO(dist) == false)
		{
			//��ȭ ��뷮
			float delta = accelate * timeDelta;

			//���� ��
			float t = Clamp01(delta / dist);

			//�̹������ӿ� Ÿ�ٱ��� ��ȭ �ϴµ� ����ϴ�
			if (FLOATEQUAL(t, 1.0f))
			{
				nowVelocity = target;
			}
			else
			{
				nowVelocity = VecLerp(nowVelocity, target, t);
			}
		}

		//�̵� ���ʹ�
		D3DXVECTOR3 moveDeltaVector = nowVelocity * timeDelta;
		//�̵��ض�
		this->MovePositionWorld(moveDeltaVector);
		*/

		//
		// ���Ӱ��� ���� �̵�
		//

		//�̵� �ض�
		//Ÿ�ٺ��� 
		D3DXVECTOR3 target = inputVector * maxSpeed;
		this->MovePositionSelf(target * timeDelta);

		//
		// ȸ�� ó��
		// 

		//ȭ���� �߽���ġ
		int screenCenterX = WINSIZE_X / 2;
		int screenCenterY = WINSIZE_Y / 2;

		//���� ���콺 ��ġ
		POINT mousePos = GetMousePos();

		//�̵��� ( �߾ӿ��� �־��� �� )
		float deltaX = mousePos.x - screenCenterX;
		float deltaY = mousePos.y - screenCenterY;

		//�ޱ� �߰�
		nowAngleH += deltaX * sensitivityH;
		nowAngleV += deltaY * sensitivityV;

		//�ޱ۰��� min max ���� ������
		nowAngleV = Clamp(nowAngleV, minAngleV, maxAngleV);

		//�ٽ� ���콺 ��ġ�� ���ͷ�...
		SetMousePos(screenCenterX, screenCenterY);

		this->SetRotate(nowAngleV * ONE_RAD, nowAngleH * ONE_RAD, 0.0f);
	}

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

	//����� ī�޶� ������ġ�� ���� �����
	D3DXMatrixInverse(&matView, NULL, &matFinal);

	this->matViewProjection = this->matView * this->matProjection;
}

//View ��İ� ���� ����� ����
void cCamera::UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice)
{
	//��� ������Ʈ ������
	this->UpdateMatrix();

	//����
	pDevice->SetTransform(D3DTS_VIEW, &this->matView);
	pDevice->SetTransform(D3DTS_PROJECTION, &this->matProjection);
}
