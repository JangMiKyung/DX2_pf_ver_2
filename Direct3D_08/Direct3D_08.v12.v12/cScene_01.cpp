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
	//ť�� ����
	this->CreateCubeHardEdge();
	//this->CreateCubeSoftEdge();

	this->cubeMaterial.Diffuse = D3DXCOLOR(0xffffff00);
	this->cubeMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->cubeMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->cubeMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���õ� Amient �� �����ϰ� �ްٴ�...
	this->cubeMaterial.Power = 10.0f;

	//������
	ZeroMemory(&this->light, sizeof(D3DLIGHT9));
	this->light.Type = D3DLIGHT_DIRECTIONAL;						//���⼺ ���� Ÿ��
	this->light.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);		//���⼺ ���� ����
	this->light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���� Diffuse �÷�
	this->light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���� Specular �÷�

	//������ ����
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);		//���� ����ϰ���....

	Device->SetLight(0, &this->light);			//0 �� ���� ���� ����
	Device->LightEnable(0, TRUE);					//0 �� ���� Ȱ��ȭ....


	//Ambient �V��
	Device->SetRenderState(D3DRS_AMBIENT, 0xff000000);


	//����ŧ�� ( ���ݻ� Ȱ��ȭ )
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
}

void cScene_01::Release()
{
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_01::Update(float timeDelta)
{
	float deltaMove = 3.0f * timeDelta;
	float deltaAngle = 90.0f * ONE_RAD * timeDelta;

	if (KEY_MGR->IsStayDown(VK_RBUTTON) == false)
	{
		if (KEY_MGR->IsStayDown('A'))
			this->lightTrans.MovePositionSelf(-deltaMove, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown('D'))
			this->lightTrans.MovePositionSelf(deltaMove, 0.0f, 0.0f);

		if (KEY_MGR->IsStayDown('W'))
			this->lightTrans.MovePositionSelf(0.0f, 0.0f, deltaMove);
		else if (KEY_MGR->IsStayDown('S'))
			this->lightTrans.MovePositionSelf(0.0f, 0.0f, -deltaMove);

		if (KEY_MGR->IsStayDown(VK_LEFT))
			this->lightTrans.RotateSelf(0.0f, -deltaAngle, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_RIGHT))
			this->lightTrans.RotateSelf(0.0f, deltaAngle, 0.0f);

		if (KEY_MGR->IsStayDown(VK_UP))
			this->lightTrans.RotateSelf(-deltaAngle, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_DOWN))
			this->lightTrans.RotateSelf(deltaAngle, 0.0f, 0.0f);
	}

	//������ ���� ����
	this->light.Direction = this->lightTrans.GetForward();
	//���ŵ� ������ �ٽ� ����
	Device->SetLight(0, &this->light);			//0 �� ���� ���� ����


	//ī�޶� Default ��Ʈ��
	mainCamera.DefaultControl(timeDelta);

	//ī�޶� ������Ʈ
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_01::Render()
{
	//ť���� �������� ( �߿� ************* �ѹ������������ ����ɶ� �����Ҽ� �ִ� ���͸������ 1 ���̴� )
	Device->SetMaterial(&cubeMaterial);

	this->cubeTrans.SetDeviceWorld(Device);
	this->RenderCube();

	lightTrans.RenderGimozo();
}


void cScene_01::CreateCubeHardEdge()
{
	//���� �÷� ���� ��ġ
	D3DXVECTOR3 quad[4];
	quad[0] = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	quad[1] = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	quad[2] = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	quad[3] = D3DXVECTOR3(0.5f, -0.5f, -0.5f);


	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, -90.0f * ONE_RAD);

	//���� �迭�غ�
	D3DXVECTOR3 vertices[24];
	//�ո� 
	memcpy(vertices, quad, sizeof(D3DXVECTOR3) * 4);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//������ ����
	memcpy(vertices + 4, quad, sizeof(D3DXVECTOR3) * 4);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//�޸� ����
	memcpy(vertices + 8, quad, sizeof(D3DXVECTOR3) * 4);


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//������ ����
	memcpy(vertices + 12, quad, sizeof(D3DXVECTOR3) * 4);


	D3DXMatrixRotationZ(&matRot, -90.0f * ONE_RAD);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//���� ����
	memcpy(vertices + 16, quad, sizeof(D3DXVECTOR3) * 4);


	D3DXMatrixRotationZ(&matRot, -180.0f * ONE_RAD);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i], &quad[i], &matRot);

	//�Ʒ��� ����
	memcpy(vertices + 20, quad, sizeof(D3DXVECTOR3) * 4);


	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//�ε��� ����
	DWORD indices[36];

	for (int i = 0; i < 6; i++)
	{
		//���� ��ŸƮ �ε���
		DWORD vertexIndex = i * 4;

		//�ε��� �迭 ��ŸƮ �ε���
		DWORD indicexIndex = i * 6;

		indices[indicexIndex + 0] = vertexIndex + 0;
		indices[indicexIndex + 1] = vertexIndex + 1;
		indices[indicexIndex + 2] = vertexIndex + 2;
		indices[indicexIndex + 3] = vertexIndex + 0;
		indices[indicexIndex + 4] = vertexIndex + 2;
		indices[indicexIndex + 5] = vertexIndex + 3;
	}


	//�븻��� 
	D3DXVECTOR3 Normal[24];
	ComputeNormal(Normal, vertices, 24, indices, 36);

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

	LPMYVERTEX pVertices = NULL;
	this->vb->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 24; i++){
		pVertices[i].pos = vertices[i];
		pVertices[i].normal = Normal[i];
	}

	this->vb->Unlock();

	//
	// �ε��� ���� ����
	//
	Device->CreateIndexBuffer(
		sizeof(DWORD) * 36,		//Cube �� 36 ���� �ε��� ������ �ʿ�,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,				//WORD ���ε����� 2byte �̴� 16 ��Ʈ�ε����̴�, DWORD �� �ϸ� 4byte �� 32 ��Ʈ �ε����� �Ǿ�� �Ѵ�.
		D3DPOOL_DEFAULT,
		&this->ib,					//���� �ε������� ������
		NULL);

	DWORD* pIndices = NULL;
	this->ib->Lock(0, 0, (void**)&pIndices, 0);

	memcpy(pIndices, indices, sizeof(DWORD) * 36);

	this->ib->Unlock();
}

void cScene_01::CreateCubeSoftEdge()
{
	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3


	//���� ��ġ ���� ����
	D3DXVECTOR3 pos[8];
	pos[0] = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	pos[1] = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	pos[2] = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	pos[3] = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	pos[4] = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	pos[5] = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	pos[6] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	pos[7] = D3DXVECTOR3(1.0f, 1.0f, -1.0f);

	//		5--------6
	//     /|       /|
	//    4--------7 |
	//    | |      | | 
	//    | 1------| 2
	//    |/       |/
	//    0--------3

	//���� �ε��� 
	DWORD index[36] = {
		0, 4, 7, 0, 7, 3,
		3, 7, 6, 3, 6, 2,
		2, 6, 5, 2, 5, 1,
		1, 5, 4, 1, 4, 0,
		4, 5, 6, 4, 6, 7,
		2, 1, 0, 2, 0, 3
	};


	//�븻 ���
	D3DXVECTOR3 Normal[8];
	ComputeNormal(Normal, pos, 8, index, 36);





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

	LPMYVERTEX pVertices = NULL;
	this->vb->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 8; i++){
		pVertices[i].pos = pos[i];
		pVertices[i].normal = Normal[i];
	}

	this->vb->Unlock();

	//
	// �ε��� ���� ����
	//
	Device->CreateIndexBuffer(
		sizeof(DWORD) * 36,		//Cube �� 36 ���� �ε��� ������ �ʿ�,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,				//WORD ���ε����� 2byte �̴� 16 ��Ʈ�ε����̴�, DWORD �� �ϸ� 4byte �� 32 ��Ʈ �ε����� �Ǿ�� �Ѵ�.
		D3DPOOL_DEFAULT,
		&this->ib,					//���� �ε������� ������
		NULL);

	DWORD* pIndices = NULL;
	this->ib->Lock(0, 0, (void**)&pIndices, 0);

	memcpy(pIndices, index, sizeof(DWORD) * 36);

	this->ib->Unlock();
}


void cScene_01::RenderCube()
{
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
}