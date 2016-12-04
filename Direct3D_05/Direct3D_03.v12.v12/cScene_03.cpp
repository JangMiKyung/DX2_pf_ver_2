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
	//ť�� ����
	this->CreateCube();

	//ȸ������ ����.
	this->forward = D3DXVECTOR3(0, 0, 1);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->right = D3DXVECTOR3(1, 0, 0);
	this->pos = D3DXVECTOR3(0, 0, 0);

	//�ٶ󺸴� ��ġ
	this->tagetPos = D3DXVECTOR3(0, 0, 10);

	//������� �غ�
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 lookAtPos(0, 0, 0);		//0, 0, 0,  ��ġ�� ����.
	D3DXVECTOR3 eyePos(1, 3, -5);		//���� ��ġ�� 0, 1, -10 �̴�.
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


	//������ �÷� ������ ����ϱ� ���ؼ��� Device �� �⺻ ������ 
	//����� �����ؾ� �Ѵ�
	//��а� �������� ��������� ������ �������� ����.
	Device->SetRenderState(
		D3DRS_LIGHTING,			//�������Ҳ���?
		FALSE					//���ð��� ��� �Ǵ�?
		);
}

void cScene_03::Release()
{
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_03::Update(float timeDelta)
{
	if (KEY_MGR->IsStayDown('A'))
		this->pos += this->right * -5.0f * timeDelta;

	else if (KEY_MGR->IsStayDown('D'))
		this->pos += this->right *  5.0f * timeDelta;

	if (KEY_MGR->IsStayDown('W'))
		this->pos += this->forward *  5.0f * timeDelta;

	else if (KEY_MGR->IsStayDown('S'))
		this->pos += this->forward * -5.0f * timeDelta;



	if (KEY_MGR->IsStayDown(VK_LEFT))
		this->tagetPos.x += -5.0f * timeDelta;

	else if (KEY_MGR->IsStayDown(VK_RIGHT))
		this->tagetPos.x += 5.0f * timeDelta;

	if (KEY_MGR->IsStayDown(VK_UP))
		this->tagetPos.y += 5.0f * timeDelta;

	else if (KEY_MGR->IsStayDown(VK_DOWN))
		this->tagetPos.y += -5.0f * timeDelta;


	if (KEY_MGR->IsStayDown('T'))
		this->tagetPos.z += 5.0f * timeDelta;

	else if (KEY_MGR->IsStayDown('G'))
		this->tagetPos.z += -5.0f * timeDelta;

	//�׸� Target �� �ٶ� ����

	//Target ����
	D3DXVECTOR3 dirToTarget = this->tagetPos - this->pos;
	//����ȭ
	D3DXVec3Normalize(&dirToTarget, &dirToTarget);

	//forward �� ����
	this->forward = dirToTarget;

	//Up X Forward = Rigth
	D3DXVec3Cross(&this->right, &this->up, &this->forward);
	D3DXVec3Normalize(&this->right, &this->right);

	//Forward X Right = Up
	D3DXVec3Cross(&this->up, &this->forward, &this->right);
	D3DXVec3Normalize(&this->up, &this->up);

	D3DXVECTOR3 r = this->right * 3;
	D3DXVECTOR3 u = this->up * 0.5f;
	D3DXVECTOR3 f = this->forward;

	//���� ���
	this->matWorld._11 = r.x;				this->matWorld._12 = r.y;					this->matWorld._13 = r.z;					this->matWorld._14 = 0.0f;
	this->matWorld._21 = u.x;				this->matWorld._22 = u.y;					this->matWorld._23 = u.z;					this->matWorld._24 = 0.0f;
	this->matWorld._31 = f.x;				this->matWorld._32 = f.y;					this->matWorld._33 = f.z;					this->matWorld._34 = 0.0f;
	this->matWorld._41 = this->pos.x;		this->matWorld._42 = this->pos.y;			this->matWorld._43 = this->pos.z;			this->matWorld._44 = 1.0f;
}

void cScene_03::Render()
{
	//ť�� ���� ��� ����
	Device->SetTransform(D3DTS_WORLD, &matWorld);


	//��Ʈ���ҽ� ����
	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));

	//�ε��� ���� ����
	Device->SetIndices(this->ib);

	//���ݱ׸��� ������ ���� ���� ���� FVF �÷��� ���� �־��ش�. 
	//Device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	Device->SetFVF(MYVERTEX::FVF);

	//SetStreamSource ���õ� �������ۿ� �ε��� ���� �������� �׸���.
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,		//���� ���ؽ��迭 �ε��� 
		0,
		24,		//���� ���� 
		0,		//���� �ε����迭 �ε���
		12		//�׸��� ��ä�� �ﰢ���̴ϱ� 12 ��
		);


	//Ÿ����ġ�� ������
	GIZMO_MGR->WireSphere(this->tagetPos, 1.0f, 0xffff00ff);
}

void cScene_03::CreateCube()
{
	//
	// ���� ���� ����
	//
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 24,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,		//����� �޸� Ǯ ����
		&this->vb,
		NULL);

	//����� �޸�Ǯ
	//D3DPOOL_DEFAULT			//���������� �޸𸮴� �ý��ۻ�翡 ���� �˾Ƽ� �����̵Ǵµ� ������ ���� ī��޸𸮿� �����̵ȴ�.
	//D3DPOOL_MANAGED			//���������� �޸𸮴� �ý����� �޸𸮿����� �����̵ǰ� ����ī�� �޸𸮸� ����Ѵٸ� ����ī�� �޸𸮿��� �����̵ȴ�, �� ������ ���������� System �޸𸮿� �����̵� ���¿��� ����ī�� �޸𸮿� �װſ� �Ȱ��� �纻�� ������ ����ε�... �̴� Device �� ���� �Ǿ ������ ������ �ٽ� ������ �� �ִ� ������ �ִ�.


	//���� �÷� ����
	MYVERTEX quad[4];
	quad[0].pos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	quad[1].pos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	quad[2].pos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	quad[3].pos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);


	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, -90.0f * ONE_RAD);


	//���� �迭�غ�
	MYVERTEX vertices[24];

	//�ո� 
	memcpy(vertices, quad, sizeof(MYVERTEX) * 4);
	vertices[0].color = 0xff000080;
	vertices[1].color = 0xff000080;
	vertices[2].color = 0xff000080;
	vertices[3].color = 0xff000080;


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//������ ����
	memcpy(vertices + 4, quad, sizeof(MYVERTEX) * 4);
	vertices[4].color = 0xffff0000;
	vertices[5].color = 0xffff0000;
	vertices[6].color = 0xffff0000;
	vertices[7].color = 0xffff0000;


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//�޸� ����
	memcpy(vertices + 8, quad, sizeof(MYVERTEX) * 4);
	vertices[8].color = 0xff0000ff;
	vertices[9].color = 0xff0000ff;
	vertices[10].color = 0xff0000ff;
	vertices[11].color = 0xff0000ff;


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//������ ����
	memcpy(vertices + 12, quad, sizeof(MYVERTEX) * 4);
	vertices[12].color = 0xff800000;
	vertices[13].color = 0xff800000;
	vertices[14].color = 0xff800000;
	vertices[15].color = 0xff800000;



	D3DXMatrixRotationZ(&matRot, -90.0f * ONE_RAD);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//���� ����
	memcpy(vertices + 16, quad, sizeof(MYVERTEX) * 4);
	vertices[16].color = 0xff00ff00;
	vertices[17].color = 0xff00ff00;
	vertices[18].color = 0xff00ff00;
	vertices[19].color = 0xff00ff00;


	D3DXMatrixRotationZ(&matRot, -180.0f * ONE_RAD);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//�Ʒ��� ����
	memcpy(vertices + 20, quad, sizeof(MYVERTEX) * 4);
	vertices[20].color = 0xff008000;
	vertices[21].color = 0xff008000;
	vertices[22].color = 0xff008000;
	vertices[23].color = 0xff008000;


	//���� Lock
	void* pVertices = NULL;
	this->vb->Lock(0, 0, &pVertices, 0);

	//���� �����Ϳ� �������� �޸� ���� 
	memcpy(pVertices, vertices, sizeof(MYVERTEX) * 24);

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
	WORD indices[36];

	for (int i = 0; i < 6; i++)
	{
		//���� ��ŸƮ �ε���
		WORD vertexIndex = i * 4;

		//�ε��� �迭 ��ŸƮ �ε���
		WORD indicexIndex = i * 6;

		indices[indicexIndex + 0] = vertexIndex + 0;
		indices[indicexIndex + 1] = vertexIndex + 1;
		indices[indicexIndex + 2] = vertexIndex + 2;
		indices[indicexIndex + 3] = vertexIndex + 0;
		indices[indicexIndex + 4] = vertexIndex + 2;
		indices[indicexIndex + 5] = vertexIndex + 3;
	}

	//�͵������� Lock
	void* pIndices = NULL;
	this->ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, indices, sizeof(WORD) * 36);

	this->ib->Unlock();
}

void cScene_03::RotateSelfYaw(float angle)
{
	//�ڽ��� up ���� �������� �� ȸ�� ���
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &this->up, angle);

	//Front �� Right ȸ����Ų��.
	//D3DXVec3TransformNormal( &this->forward, &this->forward, &matRot );
	//D3DXVec3TransformNormal( &this->right, &this->right, &matRot );

	//Forwad �� ȸ��
	D3DXVec3TransformNormal(&this->forward, &this->forward, &matRot);

	//Right �� �������� ���Ѵ�.
	D3DXVec3Cross(&right, &this->up, &this->forward);
	//��������� ����ȭ ���ִ� ����
	D3DXVec3Normalize(&this->right, &this->right);
}

void cScene_03::RotateSelfPitch(float angle)
{
	//�ڽ��� right ���� �������� �� ȸ�� ���
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &this->right, angle);

	//Up �� Forward ȸ����Ų��.
	//D3DXVec3TransformNormal( &this->forward, &this->forward, &matRot );
	//D3DXVec3TransformNormal( &this->up, &this->up, &matRot );


	//Forwad �� ȸ��
	D3DXVec3TransformNormal(&this->forward, &this->forward, &matRot);

	//Up �� �������� ���Ѵ�.
	D3DXVec3Cross(&this->up, &this->forward, &this->right);
	//��������� ����ȭ ���ִ� ����
	D3DXVec3Normalize(&this->up, &this->up);
}

void cScene_03::RotateSelfRoll(float angle)
{
	//�ڽ��� forward ���� �������� �� ȸ�� ���
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &this->forward, angle);

	//Up �� Right ȸ����Ų��.
	//D3DXVec3TransformNormal( &this->right, &this->right, &matRot );
	//D3DXVec3TransformNormal( &this->up, &this->up, &matRot );

	//Up ȸ����Ų��.
	D3DXVec3TransformNormal(&this->up, &this->up, &matRot);
	//Right �� �������� ���Ѵ�.
	D3DXVec3Cross(&this->right, &this->up, &this->forward);
	//��������� ����ȭ ���ִ� ����
	D3DXVec3Normalize(&this->right, &this->right);
}

void cScene_03::RotateWorldYaw(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, angle);

	D3DXVec3TransformNormal(&this->right, &this->right, &matRot);
	D3DXVec3TransformNormal(&this->up, &this->up, &matRot);
	D3DXVec3TransformNormal(&this->forward, &this->forward, &matRot);
}

void cScene_03::RotateWorldPitch(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationX(&matRot, angle);

	D3DXVec3TransformNormal(&this->right, &this->right, &matRot);
	D3DXVec3TransformNormal(&this->up, &this->up, &matRot);
	D3DXVec3TransformNormal(&this->forward, &this->forward, &matRot);
}

void cScene_03::RotateWorldRoll(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationZ(&matRot, angle);

	D3DXVec3TransformNormal(&this->right, &this->right, &matRot);
	D3DXVec3TransformNormal(&this->up, &this->up, &matRot);
	D3DXVec3TransformNormal(&this->forward, &this->forward, &matRot);
}
