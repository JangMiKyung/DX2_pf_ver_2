#include "stdafx.h"
#include "cTransform.h"


cTransform::cTransform()
{
	this->bAutoUpdate = true;

	//리셋 정보
	this->Reset();

	if (this->bAutoUpdate)
		this->UpdateTransform();
}


cTransform::~cTransform()
{
}

//transform 정보를 초기화
void cTransform::Reset(int resetFlag /*= -1*/)
{
	if (resetFlag & RESET_POSITION)
	{
		//위치는 0, 0, 0
		this->position.x = 0;
		this->position.y = 0;
		this->position.z = 0;
	}

	if (resetFlag & RESET_ROTATION)
	{
		//회전 방향은
		/*
		this->axis[0] = D3DXVECTOR3(1, 0, 0);
		this->axis[1] = D3DXVECTOR3(0, 1, 0);
		this->axis[2] = D3DXVECTOR3(0, 0, 1);*/

		this->right		= D3DXVECTOR3(1, 0, 0);
		this->up		= D3DXVECTOR3(0, 1, 0);
		this->forward	= D3DXVECTOR3(0, 0, 1);
	}

	if (resetFlag & RESET_SCALE)
	{
		this->scale = D3DXVECTOR3(1, 1, 1);
	}

	this->UpdateTransform();
}

//위치를 월드 좌표계로 셋팅
void cTransform::SetWorldPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::SetWorldPosition(D3DXVECTOR3 pos)
{
	this->position.x = pos.x;
	this->position.y = pos.y;
	this->position.z = pos.z;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//자신의 축 기준으로 이동 시킴
void cTransform::MovePositionSelf(float dx, float dy, float dz)
{
	//이동 벡터
	D3DXVECTOR3 move =
		this->right * dx +
		this->up * dy +
		this->forward * dz;

	//위치값 갱신

	this->position += move;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::MovePositionSelf(D3DXVECTOR3 delta)
{
	//이동 벡터
	D3DXVECTOR3 move =
		this->right * delta.x +
		this->up * delta.y +
		this->forward * delta.z;

	this->position += move;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//월드 기준으로 이동시킨다.
void cTransform::MovePositionWorld(float dx, float dy, float dz)
{
	this->position.x += dx;
	this->position.y += dy;
	this->position.z += dz;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::MovePositionWorld(D3DXVECTOR3 delta)
{
	this->position += delta;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}


//스케일 셋팅
void cTransform::SetScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::SetScale(D3DXVECTOR3 deltaScale)
{
	this->scale = deltaScale;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//스케일링
void cTransform::Scaling(float dx, float dy, float dz)
{
	this->scale.x += dx;
	this->scale.y += dy;
	this->scale.z += dz;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::Scaling(D3DXVECTOR3 deltaScale)
{
	this->scale += deltaScale;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//월드 기준으로 회전
void cTransform::RotateWorld(float angleX, float angleY, float angleZ)
{
	//각 축에 대한 회전 행렬
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationX(&matRotateX, angleX);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationY(&matRotateY, angleY);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, angleZ);

	//최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::RotateWorld(D3DXVECTOR3 angle)
{
	//각 축에 대한 회전 행렬
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationX(&matRotateX, angle.x);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationY(&matRotateY, angle.y);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, angle.z);

	//최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//자신의 축기준으로 회전
void cTransform::RotateSelf(float angleX, float angleY, float angleZ)
{
	//각 축에 대한 회전 행렬
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationAxis(&matRotateX, &this->right, angleX);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationAxis(&matRotateY, &this->up, angleY);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationAxis(&matRotateZ, &this->forward, angleZ);


	//최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::RotateSelf(D3DXVECTOR3 angle)
{
	//각 축에 대한 회전 행렬
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationAxis(&matRotateX, &this->right, angle.x);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationAxis(&matRotateY, &this->up, angle.y);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationAxis(&matRotateZ, &this->forward, angle.z);


	//최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//사원수를 이용한 특정 회전값으로 회전량을 가져라
void cTransform::SetRotate(float eAngleX, float eAngleY, float eAngleZ)
{
	//사원수 준비
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, eAngleZ);

	//사원수에 의한 행렬준비
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);//사원수에 의한 회전값으로 회전행렬이 만들어진다

	//축 리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	//최종 행렬 대로 회전
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//특정 방향으로 바라보게 회전해라
void cTransform::LookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up /*= D3DXVECTOR3(0, 1, 0)*/)
{

}

//특정 방향을 바라보는데 angle 각만큼 회전 해라
void cTransform::LookDirection(D3DXVECTOR3 dir, float angle)
{

}

//특정 위치를 바라보게 회전해라
void cTransform::LookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up /*= D3DXVECTOR3(0, 1, 0)*/)
{

}

//특정 위치를 바라보는데 angle 값만큼 회전 해라
void cTransform::LookPosition(D3DXVECTOR3 dir, float angle)
{

}

//
//update 들
//

//Transform 을 업데이트 한다
void cTransform::UpdateTransform()
{
	//자신의 정보로 Final 행렬을 갱신한다
	D3DXMatrixIdentity(&this->matFinal);

	//스케일 먹은 축정보
	D3DXVECTOR3 scaleRight		= this->right * this->scale.x;
	D3DXVECTOR3 scaleUp			= this->up * this->scale.y;
	D3DXVECTOR3 scaleForward	= this->forward * this->scale.z;

	memcpy(&this->matFinal._11, &scaleRight,	sizeof(D3DXVECTOR3));
	memcpy(&this->matFinal._21, &scaleUp,		sizeof(D3DXVECTOR3));
	memcpy(&this->matFinal._31, &scaleForward,	sizeof(D3DXVECTOR3));
	memcpy(&this->matFinal._41, &this->position, sizeof(D3DXVECTOR3));
}

//Device 에 자신의 Transform 을 월드로 셋팅
void cTransform::SetDeviceWorld(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &matFinal);
}

//Device 에 자신의 Transform 을 뷰로 셋팅
void cTransform::SetDeviceView(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIXA16 matView;
	D3DXMatrixInverse(&matView, NULL, &matFinal);

	pDevice->SetTransform(D3DTS_VIEW, &matView);
}

//
//Get 들
//

D3DXMATRIXA16 cTransform::GetFinalMatrix()
{
	return this->matFinal;
}

//Transform 에 대한 기즈모를 그린다
void cTransform::RenderGizmo(bool applyScale /*= false*/)
{
	GIZMO_MGR->Line(this->position, this->position + this->forward * ((applyScale) ? this->scale.z : 1.0f), 0xff0000ff);
	GIZMO_MGR->Line(this->position, this->position + this->up * ((applyScale) ? this->scale.y : 1.0f), 0xff00ff00);
	GIZMO_MGR->Line(this->position, this->position + this->right * ((applyScale) ? this->scale.x : 1.0f), 0xffff0000);
}