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
	//ť�� ����
	this->CreateCube();

	//ȸ���� �ʱ�ȭ
	this->xAngle = 0.0f;
	this->yAngle = 0.0f;
	this->zAngle = 0.0f;

	//ť�� ���� ��� �ʱ�ȭ
	D3DXMatrixIdentity(&matWorld);

	//����� �غ�
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 eyePos(1, 3.f, -5.f);
	D3DXVECTOR3 lookAt(0.f, 0.f, 0.f);
	D3DXVECTOR3 up(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&matView, &eyePos, &lookAt, &up);

	//����� ����
	Device->SetTransform(D3DTS_VIEW, &matView);

	//������� �غ�
	D3DXMATRIXA16 matProjection;

	D3DXMatrixPerspectiveFovLH(
		&matProjection,
		60.f * ONE_RAD,
		static_cast<float>(WINSIZE_X) / static_cast<float>(WINSIZE_Y),
		0.01f,
		100.f);

	//���� ��� ����
	Device->SetTransform(D3DTS_PROJECTION, &matProjection);

	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
}

void cScene_00::Release()
{
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_00::Update(float timeDelta)
{
	if (KEY_MGR->IsOnceDown('A'))
		this->yAngle -= 10.0f;
	else if (KEY_MGR->IsOnceDown('D'))
		this->yAngle += 10.0f;


	if (KEY_MGR->IsOnceDown('W'))
		this->xAngle -= 10.0f;
	else if (KEY_MGR->IsOnceDown('S'))
		this->xAngle += 10.0f;


	if (KEY_MGR->IsOnceDown('Q'))
		this->zAngle += 10.0f;
	else if (KEY_MGR->IsOnceDown('E'))
		this->zAngle -= 10.0f;

	//Y �࿡���� ȸ�����
	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationY(&matRotateY, (this->yAngle * ONE_RAD));

	//X �࿡���� ȸ�����
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationX(&matRotateX, (this->xAngle * ONE_RAD));

	//Z �࿡���� ȸ�����
	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, (this->zAngle * ONE_RAD));

	//���� ���
	//ȸ����� ���ع����� ���Ϸ� ȸ�� ���
	this->matWorld = matRotateX * matRotateY * matRotateZ;

	//���� ���� ������
	//�ι�° �������� ���� 90 �� ������ ȸ���� �ϴ� ���
	//ù��° ��� ������ ȸ������ �������� ù��° Ȥ�� ����° ���� ȸ���� ������ �������...
}

void cScene_00::Render()
{
	//ť�� ���� ��� ����
	Device->SetTransform(D3DTS_WORLD, &matWorld);

	//��Ʈ���ҽ� ����
	Device->SetStreamSource(0, this->vb, 0, sizeof(MYVERTEX));

	//�ε��� ���� ����
	Device->SetIndices(this->ib);

	Device->SetFVF(MYVERTEX::FVF);

	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,		//���� ���ؽ��迭 �ε��� 
		0,
		24,		//���� ���� 
		0,		//���� �ε����迭 �ε���
		12		//�׸��� ��ä�� �ﰢ���̴ϱ� 12 ��
		);

	//ȸ�� �� ���
	std::string str = "";
	str.resize(128);
	
	sprintf(&str[0], "XAngle : %.2f", this->xAngle);
	DXFONT_MGR->PrintText(str, 0, 100, 0xffff8080);

	sprintf(&str[0], "YAngle : %.2f", this->yAngle);
	DXFONT_MGR->PrintText(str, 0, 120, 0xff80ff80);

	sprintf(&str[0], "ZAngle : %.2f", this->zAngle);
	DXFONT_MGR->PrintText(str, 0, 140, 0xff8080ff);

	GIZMO_MGR->Circle(D3DXVECTOR3(0, 0, 0), 3.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0xffff0000);
	GIZMO_MGR->Circle(D3DXVECTOR3(0, 0, 0), 3.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0xff00ff00);

	GIZMO_MGR->Circle(D3DXVECTOR3(0, 0, 0), 3.0f, D3DXVECTOR3(
		sin(this->yAngle *ONE_RAD),
		0.0f,
		cos(this->yAngle * ONE_RAD)),
		0xff0000ff);

}

void cScene_00::CreateCube()
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