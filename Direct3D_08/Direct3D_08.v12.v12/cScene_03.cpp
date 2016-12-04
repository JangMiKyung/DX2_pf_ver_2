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
	//�� �޽�
	D3DXCreateSphere(Device,
		5.0f,		//���� ������
		100,			//���� ������
		100,			//���� ������
		&pMesh,
		NULL);

	//�� ��ġ
	transMesh.SetWorldPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�� ���͸���									
	material.Diffuse = D3DXCOLOR(0xffffffff);
	material.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���õ� Amient �� �����ϰ� �ްٴ�...
	material.Power = 10.0f;

	//
	// Spot ������
	//
	ZeroMemory(&this->spotlight, sizeof(D3DLIGHT9));
	this->spotlight.Type = D3DLIGHT_SPOT;								//���� ���� Ÿ��
	this->spotlight.Direction = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);		//���� ���� ����
	this->spotlight.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���� ���� ��ġ
	this->spotlight.Diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);		//���� Diffuse �÷�
	this->spotlight.Specular = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.0f);		//���� Specular �÷�
	this->spotlight.Range = 10.0f;			//Spot �Ÿ�
	this->spotlight.Theta = ONE_RAD * 40.0f;//���ʿ��԰� 40�� 
	this->spotlight.Phi = ONE_RAD * 45.0f;	//�ٱ��ʿ� �԰� 45��
	this->spotlight.Falloff = 1.0f;			//���ʿ��Կ��� �ٱ��ʿ��Ա��� ����Ǵ� ��ġ
	this->spotlight.Attenuation0 = 1.0f;
	this->spotlight.Attenuation1 = 0.5f;
	this->spotlight.Attenuation2 = 0.25f;

	//���� ����
	//�Ÿ��� D ( 0 ~ 1 ) ��� ����
	// 1 / A0 + A1 * D + A2 * D^2 = ����Ǵ� ������

	//
	// ����Ʈ ������
	//
	ZeroMemory(&this->pointlight, sizeof(D3DLIGHT9));
	this->pointlight.Type = D3DLIGHT_POINT;								//�� ���� Ÿ��
	//this->pointlight.Direction = D3DXVECTOR3( -1.0f, 0.0f, 0.0f );	//�� ���� ���� ( ������ �߿����� �ʴ� )
	this->pointlight.Position = D3DXVECTOR3(0, 0, 0);					//������ ������
	this->pointlight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);		//���� Diffuse �÷�
	this->pointlight.Specular = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f);	//���� Specular �÷�
	this->pointlight.Range = 5.0f;										//������ ���� ���ްŸ�
	this->pointlight.Attenuation0 = 0.0f;
	this->pointlight.Attenuation1 = 0.0f;
	this->pointlight.Attenuation2 = 0.5f;

	//������ ����
	Device->SetRenderState(D3DRS_LIGHTING, TRUE);		//���� ����ϰ���....

	Device->SetLight(0, &this->spotlight);			//0 �� ���� ���� ����
	Device->LightEnable(0, TRUE);					//0 �� ���� Ȱ��ȭ....

	Device->SetLight(1, &this->pointlight);			//1 �� ���� ���� ����
	Device->LightEnable(1, FALSE);					//1 �� ���� Ȱ��ȭ....


	//Ambient �V��
	Device->SetRenderState(D3DRS_AMBIENT, 0xff000000);

	//����ŧ�� ( ���ݻ� Ȱ��ȭ )
	Device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
}

void cScene_03::Release()
{
	//�޽� ����
	SAFE_RELEASE(pMesh);
}

void cScene_03::Update(float timeDelta)
{
	float deltaMove = 3.0f * timeDelta;
	float deltaAngle = 90.0f * ONE_RAD * timeDelta;

	if (KEY_MGR->IsStayDown(VK_RBUTTON) == false)
	{
		if (KEY_MGR->IsStayDown('A'))
			this->spotlightTrans.MovePositionSelf(-deltaMove, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown('D'))
			this->spotlightTrans.MovePositionSelf(deltaMove, 0.0f, 0.0f);

		if (KEY_MGR->IsStayDown('W'))
			this->spotlightTrans.MovePositionSelf(0.0f, 0.0f, deltaMove);
		else if (KEY_MGR->IsStayDown('S'))
			this->spotlightTrans.MovePositionSelf(0.0f, 0.0f, -deltaMove);

		if (KEY_MGR->IsStayDown(VK_LEFT))
			this->spotlightTrans.RotateSelf(0.0f, -deltaAngle, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_RIGHT))
			this->spotlightTrans.RotateSelf(0.0f, deltaAngle, 0.0f);

		if (KEY_MGR->IsStayDown(VK_UP))
			this->spotlightTrans.RotateSelf(-deltaAngle, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_DOWN))
			this->spotlightTrans.RotateSelf(deltaAngle, 0.0f, 0.0f);
	}

	if (KEY_MGR->IsStayDown(VK_SPACE))
		RS(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		RS(D3DRS_FILLMODE, D3DFILL_SOLID);

	//������ ��ġ ����
	this->spotlight.Position = this->spotlightTrans.GetWorldPosition();
	this->spotlight.Direction = this->spotlightTrans.GetForward();

	//���ŵ� ������ �ٽ� ����
	Device->SetLight(0, &this->spotlight);			//1 �� ���� ���� ����

	//ī�޶� Default ��Ʈ��
	mainCamera.DefaultControl(timeDelta);

	//ī�޶� ������Ʈ
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_03::Render()
{
	//�޽�����
	Device->SetMaterial(&material);
	transMesh.SetDeviceWorld(Device);
	pMesh->DrawSubset(0);

	//����Ʈ Transform
	this->spotlightTrans.RenderGimozo();
	GIZMO_MGR->WireSphere(
		this->spotlightTrans.GetWorldPosition(),
		this->spotlight.Range,
		0xffffff00);
}