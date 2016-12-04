#include "stdafx.h"
#include "cScene_01.h"


cScene_01::cScene_01()
{
}


cScene_01::~cScene_01()
{
}

HRESULT cScene_01::Init()
{
	//
	// ���� ����
	//
	CreateQuad();

	//
	// Texture �ε� 
	//
	//�⺻������ �Ӹ�ü���� ���������.
	D3DXCreateTextureFromFile(
		Device,					//����̽�
		"bby.bmp",			//��������� ���ϸ�
		&this->m_pTex			//���� IDirect3DTexture9 ����������
		);


	//Texture �� ���͸��
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//
	// ���̴� �ε�
	//

	// ���̴� �ε��� ������ ������ ���� ������ ������ �޾ƿ� Error ����
	LPD3DXBUFFER pError = NULL;

	DWORD				dwShaderFlag = 0;		//���̴� �÷��� 0 

#ifdef _DEBUG
	dwShaderFlag = dwShaderFlag | D3DXSHADER_DEBUG;		//���̴��� ����׸��� �������ϰڴ� ( ����׸��� �ؾ� �߸��� ������ ������ ���� Error ���ۿ� ���������� ���� ) 
#endif

	//fxFile �� ���� Effect ��ü �ε�
	D3DXCreateEffectFromFile(
		Device,						//����̽�
		"Texure.fx",				//�ҷ��� ���̴� �ڵ� �����̸�
		NULL, 						//���̴��� �������Ҷ� �߰��� ����� #define ���� ( �ϴ� NULL )
		NULL, 						//���̴��� �������Ҷ� #include ���ù��� ó���Ҷ� ����� �������̽� �÷��� ( �ϴ� NULL )
		dwShaderFlag,				//���̴� ������ �÷���
		NULL, 						//���̴� �Ű������� ������ �޸�Ǯ ( �ϴ� NULL )
		&pEffect,					//�ε��� ���̴� Effect ������
		&pError 					//���̴��� �ε��ϰ� �������Ҷ� ������ ����� �ش� ���ۿ� �����޽����� ���� ( ���������� �ε��Ǹ� NULL �� �����ȴ� )
		);

	//�ε��� ������ �ִٸ�....
	if (pError != NULL)
	{
		//Error ������ ���������Ϳ� �������뿡 ���� ���ڿ��� �����ȴ�.
		LOG_MGR->AddLog((char*)pError->GetBufferPointer());

		//Error ���� Release
		SAFE_RELEASE(pError);
		return E_FAIL;
	}


	return S_OK;
}

void cScene_01::Release()
{
	SAFE_RELEASE(this->pEffect);

	SAFE_RELEASE(this->m_vb);
	SAFE_RELEASE(this->m_ib);

	//����
	SAFE_RELEASE(this->m_pTex);
}

void cScene_01::Update(float timeDelta)
{
	m_mainCamera.DefaultControl(timeDelta);
	m_quadTrans.DefaultControl2(timeDelta);
	m_mainCamera.UpdateCamToDevice(Device);
}

void cScene_01::Render()
{
	//Effect �� ���� �������� �����Ѵ�.
	D3DXMATRIXA16 matWorld = m_quadTrans.GetFinalMatrix();

	//Device �κ��� ���õ� View ��İ� Projection ����� ��´�.
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;
	Device->GetTransform(D3DTS_VIEW, &matView);
	Device->GetTransform(D3DTS_PROJECTION, &matProjection);


	//�Ѻ�ȯ ���
	D3DXMATRIXA16 matWVP = matWorld * matView * matProjection;
	this->pEffect->SetMatrix("matWVP", &matWVP);


	//�÷� ���� ���� ( ���� ����  ���õǴ� ���ʹ� ��� Vector4 �� �뵿�ܰ�ȴ� )
	D3DXVECTOR4 color(1, 1, 0, 1);
	this->pEffect->SetVector("vColor", &color);


	//Texture ����
	this->pEffect->SetTexture("MyTex", m_pTex);

	//Ÿ�ϼ� ����
	this->pEffect->SetFloat("tilingX", 2);
	this->pEffect->SetFloat("tilingY", 2);



	//Technique ����
	this->pEffect->SetTechnique("MyShader");



	//���̴��� Cube ����
	UINT iPassNum = 0;		//�н���

	//Effect �� ������ ���� ( �̶� Pass ���� UINT ������ �޾ƿ´� )
	this->pEffect->Begin(
		&iPassNum,		//�ش� Effect �� Pass ���� �޴´�.
		0				//�÷��� �ϴ� NULL
		);

	//Pass ����� for �� �����糤��.
	for (UINT i = 0; i < iPassNum; i++){

		this->pEffect->BeginPass(i);		//i ��° Pass ����

		//����
		//�׸���
		Device->SetStreamSource(0, this->m_vb, 0, sizeof(MYVERTEX));
		Device->SetFVF(MYVERTEX::FVF);
		Device->SetIndices(this->m_ib);
		Device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0, 0, 4, 0, 2);


		this->pEffect->EndPass();			//Pass ��

	}

	this->pEffect->End();						//Effect �� ������ ��



}

void cScene_01::CreateQuad()
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
	pVertices[0].uv = D3DXVECTOR2(0, 1);
	pVertices[1].uv = D3DXVECTOR2(0, 0);
	pVertices[2].uv = D3DXVECTOR2(1, 0);
	pVertices[3].uv = D3DXVECTOR2(1, 1);


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