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
}

void cScene_01::Release()
{
	//���� ���� ����
	SAFE_RELEASE(this->vb);
}

void cScene_01::Update(float timeDelta)
{

}

void cScene_01::Render()
{
	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));
	
	Device->SetFVF(MYVERTEX::FVF);
	//Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE); //�̷��� �ص� ��.

	Device->DrawPrimitive(
		D3DPT_TRIANGLELIST, //�ﰢ������ �׸��ڴ�
		0,					//0��°����
		1);					//�ϳ� �׸���
}