#include "stdafx.h"
#include "cTransform.h"


cTransform::cTransform()
{
	this->bAutoUpdate = true;

	//���� ����
	this->Reset();

	if (this->bAutoUpdate)
		this->UpdateTransform();
}


cTransform::~cTransform()
{
}

//transform ������ �ʱ�ȭ
void cTransform::Reset(int resetFlag /*= -1*/)
{
	if (resetFlag & RESET_POSITION)
	{
		//��ġ�� 0, 0, 0
		this->position.x = 0;
		this->position.y = 0;
		this->position.z = 0;
	}

	if (resetFlag & RESET_ROTATION)
	{
		//ȸ�� ������
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

//��ġ�� ���� ��ǥ��� ����
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

//�ڽ��� �� �������� �̵� ��Ŵ
void cTransform::MovePositionSelf(float dx, float dy, float dz)
{
	//�̵� ����
	D3DXVECTOR3 move =
		this->right * dx +
		this->up * dy +
		this->forward * dz;

	//��ġ�� ����

	this->position += move;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::MovePositionSelf(D3DXVECTOR3 delta)
{
	//�̵� ����
	D3DXVECTOR3 move =
		this->right * delta.x +
		this->up * delta.y +
		this->forward * delta.z;

	this->position += move;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//���� �������� �̵���Ų��.
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


//������ ����
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

//�����ϸ�
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

//���� �������� ȸ��
void cTransform::RotateWorld(float angleX, float angleY, float angleZ)
{
	//�� �࿡ ���� ȸ�� ���
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationX(&matRotateX, angleX);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationY(&matRotateY, angleY);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, angleZ);

	//���� ȸ�� ���
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::RotateWorld(D3DXVECTOR3 angle)
{
	//�� �࿡ ���� ȸ�� ���
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationX(&matRotateX, angle.x);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationY(&matRotateY, angle.y);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, angle.z);

	//���� ȸ�� ���
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//�ڽ��� ��������� ȸ��
void cTransform::RotateSelf(float angleX, float angleY, float angleZ)
{
	//�� �࿡ ���� ȸ�� ���
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationAxis(&matRotateX, &this->right, angleX);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationAxis(&matRotateY, &this->up, angleY);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationAxis(&matRotateZ, &this->forward, angleZ);


	//���� ȸ�� ���
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::RotateSelf(D3DXVECTOR3 angle)
{
	//�� �࿡ ���� ȸ�� ���
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationAxis(&matRotateX, &this->right, angle.x);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationAxis(&matRotateY, &this->up, angle.y);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationAxis(&matRotateZ, &this->forward, angle.z);


	//���� ȸ�� ���
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//���� ȸ�� ��� ��� ȸ�� ��Ų��.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//������� �̿��� Ư�� ȸ�������� ȸ������ ������
void cTransform::SetRotate(float eAngleX, float eAngleY, float eAngleZ)
{
	//����� �غ�
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, eAngleZ);

	//������� ���� ����غ�
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);//������� ���� ȸ�������� ȸ������� ���������

	//�� ����
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	//���� ��� ��� ȸ��
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//Ư�� �������� �ٶ󺸰� ȸ���ض�
void cTransform::LookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up /*= D3DXVECTOR3(0, 1, 0)*/)
{

}

//Ư�� ������ �ٶ󺸴µ� angle ����ŭ ȸ�� �ض�
void cTransform::LookDirection(D3DXVECTOR3 dir, float angle)
{

}

//Ư�� ��ġ�� �ٶ󺸰� ȸ���ض�
void cTransform::LookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up /*= D3DXVECTOR3(0, 1, 0)*/)
{

}

//Ư�� ��ġ�� �ٶ󺸴µ� angle ����ŭ ȸ�� �ض�
void cTransform::LookPosition(D3DXVECTOR3 dir, float angle)
{

}

//
//update ��
//

//Transform �� ������Ʈ �Ѵ�
void cTransform::UpdateTransform()
{
	//�ڽ��� ������ Final ����� �����Ѵ�
	D3DXMatrixIdentity(&this->matFinal);

	//������ ���� ������
	D3DXVECTOR3 scaleRight		= this->right * this->scale.x;
	D3DXVECTOR3 scaleUp			= this->up * this->scale.y;
	D3DXVECTOR3 scaleForward	= this->forward * this->scale.z;

	memcpy(&this->matFinal._11, &scaleRight,	sizeof(D3DXVECTOR3));
	memcpy(&this->matFinal._21, &scaleUp,		sizeof(D3DXVECTOR3));
	memcpy(&this->matFinal._31, &scaleForward,	sizeof(D3DXVECTOR3));
	memcpy(&this->matFinal._41, &this->position, sizeof(D3DXVECTOR3));
}

//Device �� �ڽ��� Transform �� ����� ����
void cTransform::SetDeviceWorld(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &matFinal);
}

//Device �� �ڽ��� Transform �� ��� ����
void cTransform::SetDeviceView(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIXA16 matView;
	D3DXMatrixInverse(&matView, NULL, &matFinal);

	pDevice->SetTransform(D3DTS_VIEW, &matView);
}

//
//Get ��
//

D3DXMATRIXA16 cTransform::GetFinalMatrix()
{
	return this->matFinal;
}

//Transform �� ���� ����� �׸���
void cTransform::RenderGizmo(bool applyScale /*= false*/)
{
	GIZMO_MGR->Line(this->position, this->position + this->forward * ((applyScale) ? this->scale.z : 1.0f), 0xff0000ff);
	GIZMO_MGR->Line(this->position, this->position + this->up * ((applyScale) ? this->scale.y : 1.0f), 0xff00ff00);
	GIZMO_MGR->Line(this->position, this->position + this->right * ((applyScale) ? this->scale.x : 1.0f), 0xffff0000);
}