#include "stdafx.h"
#include "cScene_04.h"


cScene_04::cScene_04()
{
}


cScene_04::~cScene_04()
{
}

void cScene_04::Init()
{
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 8,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,		//����� �޸� Ǯ ����
		&this->vb,
		NULL);

	//����� �޸�Ǯ
	//D3DPOOL_DEFAULT			//���������� �޸𸮴� �ý��ۻ�翡 ���� �˾Ƽ� �����̵Ǵµ� ������ ���� ī��޸𸮿� �����̵ȴ�.
	//D3DPOOL_MANAGED			//���������� �޸𸮴� �ý����� �޸𸮿����� �����̵ǰ� ����ī�� �޸𸮸� ����Ѵٸ� ����ī�� �޸𸮿��� �����̵ȴ�, �� ������ ���������� System �޸𸮿� �����̵� ���¿��� ����ī�� �޸𸮿� �װſ� �Ȱ��� �纻�� ������ ����ε�... �̴� Device �� ���� �Ǿ ������ ������ �ٽ� ������ �� �ִ� ������ �ִ�.

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//���� �迭�غ�
	MYVERTEX vertices[8];

	vertices[0].pos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	vertices[0].color = 0xffffff00;

	vertices[1].pos = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	vertices[1].color = 0xffff0000;

	vertices[2].pos = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	vertices[2].color = 0xff00ff00;

	vertices[3].pos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	vertices[3].color = 0xff00ffff;

	vertices[4].pos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	vertices[4].color = 0xff0000ff;

	vertices[5].pos = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	vertices[5].color = 0xffff00ff;

	vertices[6].pos = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	vertices[6].color = 0xffffffff;

	vertices[7].pos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	vertices[7].color = 0xff000000;

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
		sizeof(WORD) * 36,		//Cube �� 36 ���� �ε��� ������ �ʿ�,
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
	WORD indices[36] = {
		0, 4, 7, 0, 7, 3,
		3, 7, 6, 3, 6, 2,
		2, 6, 5, 2, 5, 1,
		1, 5, 4, 1, 4, 0,
		4, 5, 6, 4, 6, 7,
		1, 0, 3, 1, 3, 2
	};

	//�͵������� Lock
	void* pIndices = NULL;
	this->ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, indices, sizeof(WORD) * 36);

	this->ib->Unlock();

	Device->SetRenderState(
		D3DRS_LIGHTING,			//�������Ҳ���?
		FALSE					//���ð��� ��� �Ǵ�?
		);

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

void cScene_04::Release()
{
	//��������
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_04::Update(float timeDelta)
{
	//�ʴ� 90 �� ����
	this->yaw += (90.0f * ONE_RAD) * timeDelta;

	D3DXMATRIXA16 matRoatate;
	D3DXMatrixRotationY(&matRoatate, this->yaw);

	//���� ���
	this->matWorld = matRoatate;
}

void cScene_04::Render()
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

	//SetStreamSource ���õ� �������ۿ� �ε��� ���� �������� �׸���.
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,		//���� ���ؽ��迭 �ε��� 
		0,
		8,		//���� ���� 
		0,		//���� �ε����迭 �ε���
		12		//�׸��� ��ä�� �ﰢ���̴ϱ� 12 ��
		);
}
