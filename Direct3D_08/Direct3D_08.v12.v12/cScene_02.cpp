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
	//
	// ť�� ����
	//

	//ť�� �޽�
	D3DXCreateBox(Device, 1.0f, 1.0f, 1.0f, &pMeshCube, NULL);

	//ť�� ��ġ
	transCube.SetWorldPosition(D3DXVECTOR3(-2.0f, 2.0f, 0.0f));

	//ť�� ���͸���									
	this->cubeMaterial.Diffuse = D3DXCOLOR(0xffffff00);
	this->cubeMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->cubeMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->cubeMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���õ� Amient �� �����ϰ� �ްٴ�...
	this->cubeMaterial.Power = 10.0f;

	//
	// ������ ����
	//

	//������ �޽�
	D3DXCreateTeapot(Device, &pMeshTeapot, NULL);

	//������ ��ġ
	transTeapot.SetWorldPosition(D3DXVECTOR3(2.0f, 2.0f, 0.0f));

	//������ ���͸���									
	this->teapotMaterial.Diffuse = D3DXCOLOR(0xffff0000);
	this->teapotMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->teapotMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->teapotMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���õ� Amient �� �����ϰ� �ްٴ�...
	this->teapotMaterial.Power = 10.0f;

	//
	// �� ����
	//

	//�� �޽�
	D3DXCreateSphere(Device,
		1.0f,		//���� ������
		60,			//���� ������
		60,			//���� ������
		&pMeshSphere,
		NULL);

	//�� ��ġ
	transSphere.SetWorldPosition(D3DXVECTOR3(-2.0f, -2.0f, 0.0f));

	//�� ���͸���									
	this->SphereMaterial.Diffuse = D3DXCOLOR(0xff00ff00);
	this->SphereMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->SphereMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->SphereMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���õ� Amient �� �����ϰ� �ްٴ�...
	this->SphereMaterial.Power = 10.0f;

	//
	// ���� ����
	//

	//���� �޽�
	D3DXCreateTorus(Device, 0.5f, 1.0f, 30, 30, &pMeshTorus, NULL);

	//���� ��ġ
	transTorus.SetWorldPosition(D3DXVECTOR3(2.0f, -2.0f, 0.0f));

	//���� ���͸���									
	this->torusMaterial.Diffuse = D3DXCOLOR(0xff0000ff);
	this->torusMaterial.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	this->torusMaterial.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	this->torusMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���õ� Amient �� �����ϰ� �ްٴ�...
	this->torusMaterial.Power = 10.0f;

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

void cScene_02::Release()
{
	//�޽� ����
	SAFE_RELEASE(pMeshCube);
	SAFE_RELEASE(pMeshSphere);
	SAFE_RELEASE(pMeshTeapot);
	SAFE_RELEASE(pMeshTorus);
}

void cScene_02::Update(float timeDelta)
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


	if (KEY_MGR->IsStayDown(VK_SPACE))
		RS(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		RS(D3DRS_FILLMODE, D3DFILL_SOLID);

	//������ ���� ����
	this->light.Direction = this->lightTrans.GetForward();
	//���ŵ� ������ �ٽ� ����
	Device->SetLight(0, &this->light);			//0 �� ���� ���� ����


	//ī�޶� Default ��Ʈ��
	mainCamera.DefaultControl(timeDelta);

	//ī�޶� ������Ʈ
	mainCamera.UpdateCamToDevice(Device);
}

void cScene_02::Render()
{
	//ť�� ����
	Device->SetMaterial(&cubeMaterial);	//�������� ( �߿� ************* �ѹ������������ ����ɶ� �����Ҽ� �ִ� ���͸������ 1 ���̴� )
	transCube.SetDeviceWorld(Device);
	pMeshCube->DrawSubset(0);

	//������ ����
	Device->SetMaterial(&teapotMaterial);	//�������� ( �߿� ************* �ѹ������������ ����ɶ� �����Ҽ� �ִ� ���͸������ 1 ���̴� )
	transTeapot.SetDeviceWorld(Device);
	pMeshTeapot->DrawSubset(0);

	//�� ����
	Device->SetMaterial(&SphereMaterial);	//�������� ( �߿� ************* �ѹ������������ ����ɶ� �����Ҽ� �ִ� ���͸������ 1 ���̴� )
	transSphere.SetDeviceWorld(Device);
	pMeshSphere->DrawSubset(0);

	//���� ����
	Device->SetMaterial(&torusMaterial);	//�������� ( �߿� ************* �ѹ������������ ����ɶ� �����Ҽ� �ִ� ���͸������ 1 ���̴� )
	transTorus.SetDeviceWorld(Device);
	pMeshTorus->DrawSubset(0);

	lightTrans.RenderGimozo();
}