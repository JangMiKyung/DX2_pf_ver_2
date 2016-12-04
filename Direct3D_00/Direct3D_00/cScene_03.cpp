#include "stdafx.h"
#include "cScene_03.h"


cScene_03::cScene_03()
{
}


cScene_03::~cScene_03()
{
}


void cScene_03::Init()
{
	// 0------1
	// |	  |
	// |	  |
	// |	  |
	// 3------2

	vertex[0].pos = D3DXVECTOR3(-0.5f,  0.5f, 0.0f);
	vertex[1].pos = D3DXVECTOR3( 0.5f,  0.5f, 0.0f);
	vertex[2].pos = D3DXVECTOR3( 0.5f, -0.5f, 0.0f);
	vertex[3].pos = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);

	vertex[0].color = 0xffffff00;
	vertex[1].color = 0xffffff00;
	vertex[2].color = 0xfff00f00;
	vertex[3].color = 0xfff00f00;

	// 0------1
	// |	  |
	// |	  |
	// |	  |
	// 3------2

	//�ε��� (�ﰢ�� �׸� �������)
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;

	index[3] = 0;
	index[4] = 2;
	index[5] = 3;

	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	yaw = 0.f;

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void cScene_03::Release()
{

}

void cScene_03::Update(float timeDelta)
{
	//�ʴ� 90�� ȸ��
	this->yaw += (360.f * ONE_RAD) * timeDelta;

	D3DXMATRIXA16 matRotate;
	//3DXMatrixRotationAxis(��� ���� ���, ��������� �����ų�, ����);
	D3DXMatrixRotationAxis(&matRotate, &D3DXVECTOR3(0, 1, 0), yaw);

	//���� ���
	matWorld = matRotate;
}

void cScene_03::Render()
{
	//����� �غ�
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 lookAt(0, 0, 0);
	D3DXVECTOR3 eyePos(0, 1, -10);
	D3DXVECTOR3 up(0, 1, 0);

	//D3DXMatrixLookAtLH(������ ���� ���, ���� ��ġ, �ٶ� ��, ��);
	D3DXMatrixLookAtLH(&matView, &eyePos, &lookAt, &up);

	//�׸��� �༮�ѵ� ������� ������ �˷�����
	Device->SetTransform(D3DTS_VIEW, &matView);

	//������� �غ�
	D3DXMATRIXA16 matProjection;

	D3DXMatrixPerspectiveFovLH(
		&matProjection,		//��� ���� �������
		60.f * ONE_RAD,		//ȭ�� (�þ�)
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),//��Ⱦ��
		0.001f,				//�ּ� �þ� �Ÿ�
		1000.f);			//�ִ� �þ� �Ÿ�

	//�׸��� �༮�ѵ� ������� ������ �˷�����
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	//���� ��� ����
	Device->SetTransform(D3DTS_WORLD, &matWorld);

	Device->SetFVF(MYVERTEX::FVF);

	Device->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,	//�׸��� ���� �ﰢ���̴�
		0,					//�ּ� �ε��� ������
		4,					//���� ����
		2,					//�ﰢ�� ����
		this->index,		//DWORD �ε��� �迭
		D3DFMT_INDEX32,		//�ε��� ���� 32��Ʈ (���� WORD ��, D3DFMT_INDEX16)
		this->vertex,		//���� �迭
		sizeof(MYVERTEX));	//���� ����ü ũ��
}