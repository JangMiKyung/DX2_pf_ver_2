#include "stdafx.h"
#include "cScene_00.h"


cScene_00::cScene_00()
{
}


cScene_00::~cScene_00()
{
}

void cScene_00::Init()
{
	//����� ���� ���� ����
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 3,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,
		&planeVB,
		NULL);

	LPMYVERTEX pVertice = NULL;
	planeVB->Lock(0, 0, (void**)&pVertice, 0);

	pVertice[0].pos = D3DXVECTOR3(0.f, 0.f, 1.f);
	pVertice[1].pos = D3DXVECTOR3(1.f, 0.f, -1.f);
	pVertice[2].pos = D3DXVECTOR3(-1.f, 0.f, -1.f);
	pVertice[0].color = 0xff00ff00;
	pVertice[1].color = 0xff00ffff;
	pVertice[2].color = 0xff00ffff;

	planeVB->Unlock();

	//����� ���� ��� �ʱ�ȭ
	D3DXMatrixIdentity(&matPlane);


	//ȸ�� �ʱ�ȭ
	yawAngle = 0.f;
	yawSpeed = 90.f * ONE_RAD;

	//��ġ �ʱ�ȭ
	pos.x = 0.f;
	pos.y = 0.f;
	pos.z = 0.f;

	//�̵��ӵ�
	moveSpeed = 3.f;

	//�ι�° �����
	pos2.x = 0.f;
	pos2.y = 0.f;
	pos2.z = -5.f;

	D3DXMatrixIdentity(&matPlane2);

	//����� �غ�
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 eyePos(0.f, 30.f, 0.f);
	D3DXVECTOR3 lookAt(0.f, 0.f, 0.f);
	D3DXVECTOR3 up(0.f, 0.f, 1.f);

	D3DXMatrixLookAtLH(
		&matView,
		&eyePos,
		&lookAt,
		&up);

	//����� ����
	Device->SetTransform(D3DTS_VIEW, &matView);

	//���� �غ�
	D3DXMATRIXA16 matProjection;

	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		60.f * ONE_RAD,
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),
		0.01f,
		100.f);

	//���� ����
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	//�� ����
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

}

void cScene_00::Release()
{
	SAFE_RELEASE(planeVB);
}

void cScene_00::Update(float timeDelta)
{
	//ȸ�� ��Ʈ��
	if (KEY_MGR->IsStayDown(VK_LEFT))
		this->yawAngle -= this->yawSpeed * timeDelta;
	else if (KEY_MGR->IsStayDown(VK_RIGHT))
		this->yawAngle += yawSpeed * timeDelta;

	//YawAngle����ŭ ȸ�����
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, this->yawAngle);


	//�ʴ� �̵��� 
	float moveDelta = this->moveSpeed * timeDelta;

	//ȸ���� �Ա��� �繰�� �������
	D3DXVECTOR3 moveDir(0, 0, 1);

	//������⿡ ȸ������� ���δ�.
	D3DXVec3TransformNormal(&moveDir, &moveDir, &matRot);

	//�̵� ���� �̵��� * �������
	D3DXVECTOR3 moveVector = moveDir * moveDelta;

	//��ġ���� �̵����� ���Ѵ�.
	this->pos += moveVector;

	//��ġ ���
	D3DXMATRIXA16 matPosition;
	D3DXMatrixTranslation(&matPosition,
		this->pos.x,
		this->pos.y,
		this->pos.z);

	this->matPlane = matRot * matPosition;

	//��ġ���
	D3DXMATRIXA16 matPosition2;
	D3DXMatrixTranslation(&matPosition2,
		this->pos2.x,
		this->pos2.y,
		this->pos2.z);

	//�ι�° ����⿡���� ȸ�� ���
	D3DXMATRIXA16 matRot2;

	//�ٶ���� �� Ÿ���� ���⺤��
	D3DXVECTOR3 dirToTarget = this->pos - this->pos2;

	//����ȭ
	D3DXVec3Normalize(&dirToTarget, &dirToTarget);

	D3DXVECTOR3 dir2(0, 0, 1);

	//����
	float yawAngle2 = acos(D3DXVec3Dot(&dir2, &dirToTarget));

	//����� Ÿ�ٹ��� ����
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, &dir2, &dirToTarget);

	//���� ����� ����ȭ ���� �ʴ´�
	//�׷��� ��������� ����ȭ ����.
	D3DXVec3Normalize(&cross, &cross);

	//�׿�������� ������ �� ȸ�����
	D3DXMatrixRotationAxis(&matRot2, &cross, yawAngle2);

	this->matPlane2 = matRot2 * matPosition2;
}

void cScene_00::Render()
{
	//����� ���� ��� ����
	Device->SetTransform(D3DTS_WORLD, &matPlane);

	//����� �׷���
	Device->SetStreamSource(0, this->planeVB, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);



	//�ι�° ����� ���� ��� ����
	Device->SetTransform(D3DTS_WORLD, &matPlane2);

	//����� �׷���
	Device->SetStreamSource(0, this->planeVB, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
}