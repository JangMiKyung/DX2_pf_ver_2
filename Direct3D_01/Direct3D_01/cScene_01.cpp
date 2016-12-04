#include "stdafx.h"
#include "cScene_01.h"


cScene_01::cScene_01()
{
}


cScene_01::~cScene_01()
{
}

void cScene_01::Init()
{
	//����� ���� ���� ����
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 3,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,
		&this->planeVB,
		NULL);

	LPMYVERTEX pVertice;
	this->planeVB->Lock(0, 0, (void**)&pVertice, 0);
	pVertice[0].pos = D3DXVECTOR3(0, 0, 1);
	pVertice[0].color = 0xFFFF0000;
	pVertice[1].pos = D3DXVECTOR3(1, 0, -1);
	pVertice[1].color = 0xFFFFFF00;
	pVertice[2].pos = D3DXVECTOR3(-1, 0, -1);
	pVertice[2].color = 0xFFFFFF00;
	this->planeVB->Unlock();

	//����� ���� ��� �ʱ�ȭ
	D3DXMatrixIdentity(&matPlane);		//����� �׵���ķ� ������ش�.

	//ȸ�� �ʱ�ȭ
	this->yawAngle = 0.0f;
	this->yawSpeed = 90.0 * ONE_RAD;	//�ʴ� 90 �� ȸ��

	//��ġ �ʱ�ȸ
	this->pos.x = 0.0f;
	this->pos.y = 0.0f;
	this->pos.z = 0.0f;

	//�̵��ӵ�
	this->moveSpeed = 3.0f;

	//�ڽ� ����� ��ġ
	this->localPos = D3DXVECTOR3(3, 0, 0);
	D3DXMatrixIdentity(&matChildPlane);

	//������� �غ�
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 lookAtPos(0, 0, 0);		//0, 0, 0,  ��ġ�� ����.
	D3DXVECTOR3 eyePos(0, 30, 0);		//���� ��ġ�� 0, 1, -10 �̴�.
	D3DXVECTOR3 up(0, 0, 1);				//�������� ���� ����.

	D3DXMatrixLookAtLH(
		&matView,			//���� ����� 
		&eyePos,			//����ġ ����
		&lookAtPos,			//�ٶ� ��ġ
		&up					//��� ���� ( ���� ��Ȯ���� �ʾƵ��ȴ� )
		);
	//����� ����
	Device->SetTransform(D3DTS_VIEW, &matView);

	//������� �غ�
	D3DXMATRIXA16 matProjection;
	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		60.0f * ONE_RAD,		//ȭ�� ( 60.0 �� )
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),		//ȭ�� ��Ⱦ��,
		0.01f,					//�ٰŸ� ��� �Ÿ� ( �̰��� 0 �̸� ���� ���δ� 0 ���� �������ƶ� )
		100.0f);					//���Ÿ� ��� �Ÿ�
	//������� ����
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	Device->SetRenderState(
		D3DRS_LIGHTING,			//�������Ҳ���?
		FALSE					//���ð��� ��� �Ǵ�?
		);
}

void cScene_01::Release()
{
	SAFE_RELEASE(this->planeVB);
}

void cScene_01::Update(float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_LEFT))
		this->yawAngle -= this->yawSpeed * timeDelta;
	if (KEY_MGR->IsStayDown(VK_RIGHT))
		this->yawAngle += this->yawSpeed * timeDelta;

	//YawAngle����ŭ ȸ�����
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, this->yawAngle);

	//�ʴ� �̵��� 
	float moveDelta = this->moveSpeed * timeDelta;		//�̹� �����ӿ� �����Ϸ�

	//ȸ���� �Ա��� �繰�� �������
	D3DXVECTOR3 moveDir(0, 0, 1);

	//������⿡ ȸ������� ���δ�.
	D3DXVec3TransformNormal(&moveDir, &moveDir, &matRot);


	//�̵� ���� �̵��� * �������
	D3DXVECTOR3 moveVector = moveDir * moveDelta;

	//��ġ���� �̵����� ���Ѵ�.
	//this->pos = this->pos + moveVector;
	this->pos += moveVector;

	//��ġ ���
	D3DXMATRIXA16 matPosition;
	D3DXMatrixTranslation(&matPosition,
		this->pos.x,
		this->pos.y,
		this->pos.z);
	this->matPlane = matRot * matPosition;


	//�ڽ� ����� ��� �غ�
	D3DXMATRIXA16 matChildPos;
	D3DXMatrixTranslation(&matChildPos,
		this->localPos.x,
		this->localPos.y,
		this->localPos.z);

	//�ڽ� ���
	if (KEY_MGR->IsStayDown(VK_SPACE))
	{
		//�θ������....
		this->matChildPlane = matChildPos * this->matPlane;
	}
	else
	{
		this->matChildPlane = matChildPos;
	}
}

void cScene_01::Render()
{
	//����� ���� ��� ����
	Device->SetTransform(D3DTS_WORLD, &matPlane);

	//����� �׷���
	Device->SetStreamSource(0, this->planeVB, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	//�ڽ� ����� ���� ��� ����
	Device->SetTransform(D3DTS_WORLD, &matChildPlane);

	//����� �׷���
	Device->SetStreamSource(0, this->planeVB, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	//��İ� Ȯ��
	D3DXMATRIXA16 mat;
	//D3DXMatrixIdentity( &mat );
	D3DXMatrixTranslation(&mat, 3, 2, 7);


	/*
	//���� �� ���� �ɹ� ������ ��� ��Ұ��� ���ٰ���...
	mat._11, mat._12, mat._13, mat._14
	mat._21, mat._22, mat._23, mat._24
	mat._31, mat._32, mat._33, mat._34
	mat._41, mat._42, mat._43, mat._44

	mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3]
	mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3]
	mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3]
	mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]
	*/

	/*char szTemp[128];
	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3]);
	DXFONT_MGR->PrintText(szTemp, 0, 0, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3]);
	DXFONT_MGR->PrintText(szTemp, 0, 20, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3]);
	DXFONT_MGR->PrintText(szTemp, 0, 40, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]);
	DXFONT_MGR->PrintText(szTemp, 0, 60, 0xffffffff);*/

	char szTemp[128];
	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", matChildPlane._11, matChildPlane._12, matChildPlane._13, matChildPlane._14);
	DXFONT_MGR->PrintText(szTemp, 0, 0, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", matChildPlane._21, matChildPlane._22, matChildPlane._23, matChildPlane._24);
	DXFONT_MGR->PrintText(szTemp, 0, 20, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", matChildPlane._31, matChildPlane._32, matChildPlane._33, matChildPlane._34);
	DXFONT_MGR->PrintText(szTemp, 0, 40, 0xffffffff);

	sprintf_s(szTemp, "%.2f, %.2f, %.2f, %.2f", matChildPlane._41, matChildPlane._42, matChildPlane._43, matChildPlane._44);
	DXFONT_MGR->PrintText(szTemp, 0, 60, 0xffffffff);
}