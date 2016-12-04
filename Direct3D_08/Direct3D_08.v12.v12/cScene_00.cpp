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
	//									   0xAARRGGBB
	this->cubeMaterial.Diffuse = D3DXCOLOR(0xffffff00);
	this->cubeMaterial.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	this->cubeMaterial.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
	this->cubeMaterial.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);//���õ� Amient �� �����ϰ� �ްٴ�...
	this->cubeMaterial.Power = 10.0f;


	//������
	ZeroMemory(&this->light, sizeof(D3DLIGHT9));
	this->light.Type = D3DLIGHT_DIRECTIONAL;						//���⼺ ���� Ÿ��
	this->light.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);		//���⼺ ���� ����
	this->light.Diffuse = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);		//���� Diffuse �÷�
	this->light.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���� Specular �÷�

	//������ ����
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);		//���� ����ϰ���....

	Device->SetLight(0, &this->light);			//0 �� ���� ���� ����
	Device->LightEnable(0, TRUE);				//0 �� ���� Ȱ��ȭ....

	//Ambient �V��
	Device->SetRenderState(D3DRS_AMBIENT, 0xff000000);

	//����ŧ�� ( ���ݻ� Ȱ��ȭ )
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

}

void cScene_00::Release()
{
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_00::Update(float timeDelta)
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
	this->light.Direction = this->lightTrans.GetForward(); //���ŵ� ������ �ٽ� ����
	Device->SetLight(0, &this->light);	//0 �� ���� ���� ����


	//ī�޶� Default ��Ʈ��
	mainCamera.DefaultControl(timeDelta);

	//ī�޶� ������Ʈ
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_00::Render()
{
	//ť���� �������� ( �߿� ************* �ѹ������������ ����ɶ� �����Ҽ� �ִ� ���͸������ 1 ���̴� )
	Device->SetMaterial(&cubeMaterial);

	this->cubeTrans.SetDeviceWorld(Device);
	this->RenderCube();

	lightTrans.RenderGimozo();
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
	vertices[0].normal = D3DXVECTOR3(0, 0, -1);
	vertices[1].normal = D3DXVECTOR3(0, 0, -1);
	vertices[2].normal = D3DXVECTOR3(0, 0, -1);
	vertices[3].normal = D3DXVECTOR3(0, 0, -1);


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//������ ����
	memcpy(vertices + 4, quad, sizeof(MYVERTEX) * 4);
	vertices[4].normal = D3DXVECTOR3(1, 0, 0);
	vertices[5].normal = D3DXVECTOR3(1, 0, 0);
	vertices[6].normal = D3DXVECTOR3(1, 0, 0);
	vertices[7].normal = D3DXVECTOR3(1, 0, 0);


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//�޸� ����
	memcpy(vertices + 8, quad, sizeof(MYVERTEX) * 4);
	vertices[8].normal = D3DXVECTOR3(0, 0, 1);
	vertices[9].normal = D3DXVECTOR3(0, 0, 1);
	vertices[10].normal = D3DXVECTOR3(0, 0, 1);
	vertices[11].normal = D3DXVECTOR3(0, 0, 1);


	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//������ ����
	memcpy(vertices + 12, quad, sizeof(MYVERTEX) * 4);
	vertices[12].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[13].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[14].normal = D3DXVECTOR3(-1, 0, 0);
	vertices[15].normal = D3DXVECTOR3(-1, 0, 0);



	D3DXMatrixRotationZ(&matRot, -90.0f * ONE_RAD);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//���� ����
	memcpy(vertices + 16, quad, sizeof(MYVERTEX) * 4);
	vertices[16].normal = D3DXVECTOR3(0, 1, 0);
	vertices[17].normal = D3DXVECTOR3(0, 1, 0);
	vertices[18].normal = D3DXVECTOR3(0, 1, 0);
	vertices[19].normal = D3DXVECTOR3(0, 1, 0);


	D3DXMatrixRotationZ(&matRot, -180.0f * ONE_RAD);

	//����
	for (int i = 0; i < 4; i++)
		D3DXVec3TransformCoord(&quad[i].pos, &quad[i].pos, &matRot);

	//�Ʒ��� ����
	memcpy(vertices + 20, quad, sizeof(MYVERTEX) * 4);
	vertices[20].normal = D3DXVECTOR3(0, -1, 0);
	vertices[21].normal = D3DXVECTOR3(0, -1, 0);
	vertices[22].normal = D3DXVECTOR3(0, -1, 0);
	vertices[23].normal = D3DXVECTOR3(0, -1, 0);


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

void cScene_00::RenderCube()
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