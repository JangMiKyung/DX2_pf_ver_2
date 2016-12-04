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
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 4,
		D3DUSAGE_WRITEONLY,
		MYVERTEX::FVF,
		D3DPOOL_DEFAULT,
		&m_vb,
		NULL);

	LPMYVERTEX pVertices = NULL;

	m_vb->Lock(0, 0, (void**)&pVertices, 0);

	pVertices[0].pos = D3DXVECTOR3(-3.5f, -3.5f, 0.f);
	pVertices[1].pos = D3DXVECTOR3(-3.5f,  3.5f, 0.f);
	pVertices[2].pos = D3DXVECTOR3( 3.5f,  3.5f, 0.f);
	pVertices[3].pos = D3DXVECTOR3( 3.5f, -3.5f, 0.f);

	pVertices[0].uv = D3DXVECTOR2(4, 0);
	pVertices[1].uv = D3DXVECTOR2(0, 0);
	pVertices[2].uv = D3DXVECTOR2(0, 4);
	pVertices[3].uv = D3DXVECTOR2(4, 4);

	m_vb->Unlock();

	Device->CreateIndexBuffer(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&m_ib,
		NULL);

	WORD idx[] = { 0, 1, 2, 0, 2, 3 };

	void* pIndices = NULL;

	m_ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, idx, sizeof(WORD) * 6);

	m_ib->Unlock();

	//texture �ε�
	D3DXCreateTextureFromFile(
		Device,
		"Sample_0.jpg",
		&m_tex);


	//����� �غ�
	D3DXMATRIXA16 matView;
	D3DXVECTOR3 eyePos(0.f, 0.f, -10.f);
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


	//TextTure Warp ��� ����

	//�ݺ������� ���´� 1 �Ѿ�� UV ��ǥ�� 1�� ���� �ٽ� 0���� ����(����Ʈ)
	/*Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);*/

	//�ݺ������� ���� ���´� 1 �Ѿ�� UV ��ǥ�� 1���� 0���� ������ 0 ���Ϸ� ������ �ٽ� 1�� ���Ͽ� ����
	/*Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);*/

	//�Ӹ������� ����� 1�Ѿ�� UV ��ǥ�� �α׸� 1�� �����Ѵ�
	/*Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);*/

	//�Ӹ������� ����� 1�Ѿ�� UV ��ǥ ���õ� ���� �÷��� ĥ�Ѵ�
	/*Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);*/

	//���ε��� ���� �� �ִ�~
	Device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
	Device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

}

void cScene_00::Release()
{
	SAFE_RELEASE(m_vb);
	SAFE_RELEASE(m_ib);
	SAFE_RELEASE(m_tex);
}

void cScene_00::Update(float timeDelta)
{

}

void cScene_00::Render()
{
	D3DXMATRIXA16 matQuad;
	D3DXMatrixIdentity(&matQuad);

	//����
	Device->SetTransform(D3DTS_WORLD, &matQuad);

	//�׸��� ���� �ε��� �ؽ��ĸ� ����
	Device->SetTexture(
		0,
		m_tex);

	//�׸���
	Device->SetStreamSource(0, m_vb, 0, sizeof(MYVERTEX));
	Device->SetFVF(MYVERTEX::FVF);
	Device->SetIndices(m_ib);
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		4,
		0,
		2);

	//���õ� texture�� ���̻� �׸��� ���� �ʴٸ� ���� �ؾ��Ѵ�.
	Device->SetTexture(0, NULL);//������ ��� NULL�� ������ �����ȴ�.
}