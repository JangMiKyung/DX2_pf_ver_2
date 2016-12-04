#include "stdafx.h"
#include "cScene_06.h"


cScene_06::cScene_06()
{
}


cScene_06::~cScene_06()
{
}

void cScene_06::Init()
{
	//
	// ���� ���� ����
	//
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 8,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,		//����� �޸� Ǯ ����
		&this->vb,
		NULL);

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//���� �迭�غ�
	MYVERTEX vertices[8];

	vertices[0].pos = D3DXVECTOR3(-2.3f, -2.3f, -2.3f);
	vertices[0].color = 0xffffff00;

	vertices[1].pos = D3DXVECTOR3(-2.3f, -2.3f, 2.3f);
	vertices[1].color = 0xffffff00;

	vertices[2].pos = D3DXVECTOR3(2.3f, -2.3f, 2.3f);
	vertices[2].color = 0xffffff00;

	vertices[3].pos = D3DXVECTOR3(2.3f, -2.3f, -2.3f);
	vertices[3].color = 0xffffff00;

	vertices[4].pos = D3DXVECTOR3(-2.3f, 2.3f, -2.3f);
	vertices[4].color = 0xffffff00;

	vertices[5].pos = D3DXVECTOR3(-2.3f, 2.3f, 2.3f);
	vertices[5].color = 0xffffff00;

	vertices[6].pos = D3DXVECTOR3(2.3f, 2.3f, 2.3f);
	vertices[6].color = 0xffffff00;

	vertices[7].pos = D3DXVECTOR3(2.3f, 2.3f, -2.3f);
	vertices[7].color = 0xffffff00;

	//���� Lock
	void* pVertices = NULL;
	this->vb->Lock(0, 0, &pVertices, 0);

	//���� �����Ϳ� �������� �޸� ���� 
	memcpy(pVertices, vertices, sizeof(MYVERTEX) * 8);

	this->vb->Unlock();

	//
	// �ε��� ���� ����
	//

	Device->CreateIndexBuffer(
		sizeof(WORD) * 24,		//Cube �� 36 ���� �ε��� ������ �ʿ�,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,				//WORD ���ε����� 2byte �̴� 16 ��Ʈ�ε����̴�, DWORD �� �ϸ� 4byte �� 32 ��Ʈ �ε����� �Ǿ�� �Ѵ�.
		D3DPOOL_DEFAULT,
		&this->ib,					//���� �ε������� ������
		NULL);

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//�ε��� ����
	WORD indices[24] = {
		0, 4, 4, 7, 7, 3, 3, 0,
		1, 5, 5, 6, 6, 2, 2, 1,
		0, 1, 4, 5, 7, 6, 3, 2,
	};

	//�͵������� Lock
	void* pIndices = NULL;
	this->ib->Lock(0, 0, &pIndices, 0);
	memcpy(pIndices, indices, sizeof(WORD) * 24);
	this->ib->Unlock();

	Device->SetRenderState(
		D3DRS_LIGHTING,			//�������Ҳ���?
		FALSE					//���ð��� ��� �Ǵ�?
		);

	yaw = 0.0f;

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void cScene_06::Release()
{
	//��������
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_06::Update(float timeDelta)
{
	//�ʴ� 90 �� ����
	this->yaw += (90.0f * ONE_RAD) * timeDelta;

	D3DXMATRIXA16 matRoatate;

	D3DXMatrixRotationY(&matRoatate, this->yaw);

	//���� ���
	this->matWorld = matRoatate;
}

void cScene_06::Render()
{
	//������� �غ�
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 lookAtPos(0, 0, 0);		//0, 0, 0,  ��ġ�� ����.
	D3DXVECTOR3 eyePos(0, 5, -10);		//���� ��ġ�� 0, 1, -10 �̴�.
	D3DXVECTOR3 up(0, 1, 0);				//�������� ���� ����.

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


	//Ư�������� �׸������� ��ü�� ���ش�Ǵ� ���� ����� �����ϰ� �׸���
	Device->SetTransform(D3DTS_WORLD, &this->matWorld);

	//��Ʈ���ҽ� ����
	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));

	//�ε��� ���� ����
	Device->SetIndices(this->ib);

	Device->SetFVF(MYVERTEX::FVF);

	Device->DrawIndexedPrimitive(
		D3DPT_LINELIST,// D3DPT_LINELIST �����׸��ٴ�.
		0,		//���� ���ؽ��迭 �ε��� 
		0,
		8,		//���� ���� 
		0,		//���� �ε����迭 �ε���
		12		//�׸��� ��ä�� �����̴ϱ� 12 ��
		);
}