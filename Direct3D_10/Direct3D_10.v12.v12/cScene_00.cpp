#include "stdafx.h"
#include "cScene_00.h"


cScene_00::cScene_00()
{
}


cScene_00::~cScene_00()
{
}

HRESULT cScene_00::Init()
{
	//���� ����
	CreateQuad();

	//Texture �ε� 
	//�⺻������ �Ӹ�ü���� ���������.
	D3DXCreateTextureFromFile(
		Device,					//����̽�
		"bby.bmp",			//��������� ���ϸ�
		&this->m_pTex			//���� IDirect3DTexture9 ����������
		);

	//
	////�Ӹ�ü�ξ��� �ε�
	//D3DXCreateTextureFromFileEx(
	//	Device, 
	//	"bby.bmp",
	//	512,
	//	512, 
	//	0,
	//	0, 
	//	D3DFMT_UNKNOWN,
	//	D3DPOOL_MANAGED,
	//	D3DX_FILTER_NONE,
	//	D3DX_FILTER_NONE,
	//	0,
	//	0,
	//	0,
	//	&m_pTex );
	
	//������ �÷� ������ ����ϱ� ���ؼ��� Device �� �⺻ ������ 
	//����� �����ؾ� �Ѵ�
	//��а� �������� ��������� ������ �������� ����.
	Device->SetRenderState(
		D3DRS_LIGHTING,			//�������Ҳ���?
		FALSE					//���ð��� ��� �Ǵ�?
		);


	//���� ������ 200
	m_quadTrans.SetScale(200, 200, 200);

	//���κ���
	m_quadTrans.RotateLocal(90 * ONE_RAD, 0, 0);


	return S_OK;
}

void cScene_00::Release()
{
	SAFE_RELEASE(this->m_vb);
	SAFE_RELEASE(this->m_ib);

	//����
	SAFE_RELEASE(this->m_pTex);
}

void cScene_00::Update(float timeDelta)
{
	m_mainCamera.DefaultControl(timeDelta);
	m_quadTrans.DefaultControl2(timeDelta);

	m_mainCamera.UpdateCamToDevice(Device);



	//���͸�� ����
	//Device->SetTextureStageState( ����������ȣ, ��ſ����� ���͸� �����Ҳ���, ���Ϳɼ� )

	//��ſ����� ���͸� �����Ҳ���
	//D3DSAMP_MINFILTER	���� Sample �Ȱź��� �۰� �����°�쿡���� ���͸� ���
	//D3DSAMP_MAGFILTER	���� Sample �Ȱź��� ũ�� �����°�쿡���� ���͸� ���
	//D3DSAMP_MIPFILTER �Ӹ�ü���� ��迡�� ��� ���͸��Ұ���

	//���Ϳɼ�
	//D3DTEXF_POINT				���Ǽ��� ����
	//D3DTEXF_LINEAR			�ֺ��ȼ��� �������� ����
	//D3DTEXF_ANISOTROPIC		�̹漺�ʴ����� ���� ���� ���� ������ ���Ҽ� �ִ�.






	if (KEY_MGR->IsOnceDown('1')){
		//Texture �� ���͸��
		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}

	else if (KEY_MGR->IsOnceDown('2')){

		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);

		//�̹漺 ���͸��� ������ �����Ҽ� �ִ�.
		//D3DTEXF_ANISOTROPIC �ܰ踦 ����
		//Device->SetSamplerState( 0, D3DSAMP_MAXANISOTROPY, 16 );  //16 �ܰ� ������ �ȼ�����

		//�׷���ī�帶�� �ִ� �����ɼ� �ִ� D3DTEXF_ANISOTROPIC �ܰ� �� �ٸ���.
		//�׷��� D3DCAP9 �� ���� Ȯ���Ҽ� �ִ�
		Device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, MaxAnisotropic);

	}


	else if (KEY_MGR->IsOnceDown('0')){
		//Texture �� ���͸��
		Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	}
}

void cScene_00::Render()
{
	m_quadTrans.SetDeviceWorld(Device);


	//�׸��� ���� �ε��� Texture �� ��������.
	Device->SetTexture(
		0,			//���õ� Texture Stage �� �ϴ� 0
		m_pTex		//�ε��� Texture ������
		);

	//�׸���
	Device->SetStreamSource(0, this->m_vb, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->SetIndices(this->m_ib);
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0, 0, 4, 0, 2);

	//���õ� Texture �� ���̻� �׸��� ���� �ʴٸ� ���� �ؾ��Ѵ�.
	Device->SetTexture(0, NULL);		//������ ��� NULL �� ������ ���� �ȴ�.
}

void cScene_00::CreateQuad()
{
	//���� ���� ����
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 4,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,
		&this->m_vb,
		NULL);

	// 1----2
	// |   /|
	// |  / |
	// | /  |
	// |/   |
	// 0----3

	LPMYVERTEX pVertices = NULL;
	this->m_vb->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].pos = D3DXVECTOR3(-3.5f, -3.5f, 0.0f);
	pVertices[1].pos = D3DXVECTOR3(-3.5f, 3.5f, 0.0f);
	pVertices[2].pos = D3DXVECTOR3(3.5f, 3.5f, 0.0f);
	pVertices[3].pos = D3DXVECTOR3(3.5f, -3.5f, 0.0f);

	/*
	pVertices[0].color = 0xffff0000;
	pVertices[1].color = 0xffffff00;
	pVertices[2].color = 0xffff0000;
	pVertices[3].color = 0xffffff00;
	*/

	// 1----2
	// |   /|
	// |  / |
	// | /  |
	// |/   |
	// 0----3

	//������ Texture UV ��ǥ�� �־��ش�.
	pVertices[0].uv = D3DXVECTOR2(0, 100);
	pVertices[1].uv = D3DXVECTOR2(0, 0);
	pVertices[2].uv = D3DXVECTOR2(100, 0);
	pVertices[3].uv = D3DXVECTOR2(100, 100);


	this->m_vb->Unlock();


	//
	// �ε��� ����
	//
	Device->CreateIndexBuffer(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&this->m_ib,
		NULL);

	WORD idx[] = { 0, 1, 2, 0, 2, 3 };

	void* pIndices = NULL;

	this->m_ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, idx, sizeof(WORD) * 6);

	this->m_ib->Unlock();
}