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
	//ť�� ����
	this->CreateCube();

	//ȸ������ ����.
	this->forward = D3DXVECTOR3(0, 1, 0);
	this->up = D3DXVECTOR3(0, 0, -1);
	this->right = D3DXVECTOR3(1, 0, 0);

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

void cScene_02::Release()
{
	SAFE_RELEASE(this->vb);
	SAFE_RELEASE(this->ib);
}

void cScene_02::Update(float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_LSHIFT))
	{
		if (KEY_MGR->IsStayDown('A'))
			this->RotateSelfYaw(-90.0f * ONE_RAD * timeDelta);
		else if (KEY_MGR->IsStayDown('D'))
			this->RotateSelfYaw(90.0f * ONE_RAD * timeDelta);

		if (KEY_MGR->IsStayDown('W'))
			this->RotateSelfPitch(-90.0f * ONE_RAD * timeDelta);
		else if (KEY_MGR->IsStayDown('S'))
			this->RotateSelfPitch(90.0f * ONE_RAD * timeDelta);

		if (KEY_MGR->IsStayDown('Q'))
			this->RotateSelfRoll(90.0f * ONE_RAD * timeDelta);

		else if (KEY_MGR->IsStayDown('E'))
			this->RotateSelfRoll(-90.0f  * ONE_RAD * timeDelta);
	}
	else
	{
		if (KEY_MGR->IsStayDown('A'))
			this->RotateWorldYaw(-90.0f * ONE_RAD * timeDelta);
		else if (KEY_MGR->IsStayDown('D'))
			this->RotateWorldYaw(90.0f * ONE_RAD * timeDelta);

		if (KEY_MGR->IsStayDown('W'))
			this->RotateWorldPitch(-90.0f * ONE_RAD * timeDelta);
		else if (KEY_MGR->IsStayDown('S'))
			this->RotateWorldPitch(90.0f * ONE_RAD * timeDelta);

		if (KEY_MGR->IsStayDown('Q'))
			this->RotateWorldRoll(90.0f * ONE_RAD * timeDelta);

		else if (KEY_MGR->IsStayDown('E'))
			this->RotateWorldRoll(-90.0f  * ONE_RAD * timeDelta);
	}

	matWorld._11 = right.x;		
	matWorld._12 = right.y;		
	matWorld._13 = right.z;		
	matWorld._14 = 0;
	
	matWorld._21 = up.x;		
	matWorld._22 = up.y;		
	matWorld._23 = up.z;		
	matWorld._24 = 0;
	
	matWorld._31 = forward.x;	
	matWorld._32 = forward.y;	
	matWorld._33 = forward.z;	
	matWorld._34 = 0;
	
	matWorld._41 = 0;			
	matWorld._42 = 0;			
	matWorld._43 = 0;			
	matWorld._44 = 1;

}


void cScene_02::Render()
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
}

void cScene_02::RotateSelfYaw(float angle)
{
	//�ڽ��� up���� �������� �� ȸ��
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis(&matRot, &up, angle);

	//forward �� ȸ��
	D3DXVec3TransformNormal(&forward, &forward, &matRot);

	//right �� �������� ������
	D3DXVec3Cross(&right, &up, &forward);

	//���� ����� ����ȭ ���ִ� ����
	D3DXVec3Normalize(&right, &right);
}

void cScene_02::RotateSelfPitch(float angle)
{

}

void cScene_02::RotateSelfRoll(float angle)
{

}

void cScene_02::RotateWorldYaw(float angle)
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationY(&matRot, angle);

	D3DXVec3TransformNormal(&this->right, &this->right, &matRot);
	D3DXVec3TransformNormal(&this->up, &this->up, &matRot);
	D3DXVec3TransformNormal(&this->forward, &this->forward, &matRot);
}

void cScene_02::RotateWorldPitch(float angle)
{

}

void cScene_02::RotateWorldRoll(float angle)
{

}


void cScene_02::CreateCube()
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