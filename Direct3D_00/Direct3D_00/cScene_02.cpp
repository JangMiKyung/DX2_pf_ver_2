#include "stdafx.h"
#include "cScene_02.h"


cScene_02::cScene_02()
{
}


cScene_02::~cScene_02()
{
}

void cScene_02::Init()
{
	//���� ���� �����
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 3,	//������� ���� ������ ����Ʈ ũ��
		D3DUSAGE_WRITEONLY,		//��������� ���� ������ �Ӽ� �÷��� �ϴ� D3DUSAGE_WRITEONLY �� ������ ����, ���� �Ŀ� ���� ���ۿ� ���� ������ ���� �Ͼ�ٸ� D3DUSAGE_DYNAMIC ���� �ٲ����
		MYVERTEX::FVF,			//���� ���� ������ FVF ����
		D3DPOOL_DEFAULT,		//�޸�Ǯ �������
		&this->vb,				//������ ���� ���� ������
		NULL);					//�׳� ��~

	LPMYVERTEX pVertices = NULL; //Lock �ϰ� ���� ������ LPMYVERTEX �����ͷ� �ޱ� ����

	this->vb->Lock(
		0,		//���� �޸���ġ���� offsetbyute 0�̸� ó������
		0,		//Lock ��ũ��, ��ü ���Ҳ��� 0
		(void**)&pVertices, //Lock�ϰ� ���� ������
		0		//Lock �÷��� �ϴ� 0
		);

	//�����͸� ���ؼ� ���� ����~
	pVertices[0].pos = D3DXVECTOR3(0.f, 1.f, 0.f);
	pVertices[0].color = 0xff0000ff;
	pVertices[1].pos = D3DXVECTOR3(1.f, 0.f, 0.f);
	pVertices[1].color = 0xff00ff00;
	pVertices[2].pos = D3DXVECTOR3(-1.f, 0.f, 0.f);
	pVertices[2].color = 0xffff0000;

	//�� ������ ������
	this->vb->Unlock();

	Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	yaw = 0.f;
	//Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); //�ð���� �ø�
	//Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);//�ݽð���� �ø�(����Ʈ)
	//Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //�ø� ����(��� �� �׸���)
}

void cScene_02::Release()
{
	//���� ���� ����
	SAFE_RELEASE(this->vb);
}

void cScene_02::Update(float timeDelta)
{
	//�ʴ� 90�� ȸ��
	this->yaw += (90.f * ONE_RAD) * timeDelta;

	//y�࿡ ���� ȸ�� ���
	D3DXMATRIXA16 matRotate;
	
	//D3DXMatrixRotationY(&matRotate, this->yaw); //y�� ���� ȸ�� ���
	D3DXMatrixIdentity(&matRotate);

	//���� ���
	matWorld = matRotate;
}

void cScene_02::Render()
{
	//���� ���� �غ�
	D3DXMATRIXA16 matProjection;
	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		60.f * ONE_RAD,
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),
		0.01f,					//0 ���� �ȵ�
		1000.f);

	//���� ����
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	//�����
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 lookAtPos(0, 0, 0);
	D3DXVECTOR3 eyePos(0, 1, -10);
	D3DXVECTOR3 up(0, 1, 0);

	D3DXMatrixLookAtLH(
		&matView,	//���� �� ���
		&eyePos,	//���� ��ġ
		&lookAtPos,	//��� �ٶ󺼰ų�
		&up);		//���� ����(�׷��� ��Ȯ���� �ʾƵ� ��.

	//����� ����
	Device->SetTransform(D3DTS_VIEW, &matView);

	Device->SetTransform(D3DTS_WORLD, &matWorld);

	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));

	Device->SetFVF(MYVERTEX::FVF);
	//Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE); //�̷��� �ص� ��.

	Device->DrawPrimitive(
		D3DPT_TRIANGLELIST, //�ﰢ������ �׸��ڴ�
		0,					//0��°����
		1);					//�ϳ� �׸���
}