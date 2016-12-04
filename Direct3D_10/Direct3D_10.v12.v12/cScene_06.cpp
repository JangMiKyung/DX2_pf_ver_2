#include "stdafx.h"
#include "cScene_06.h"


cScene_06::cScene_06()
{
}


cScene_06::~cScene_06()
{
}

HRESULT cScene_06::Init()
{
	//
	// ���� ����
	//
	CreateQuad();

	//
	// ���� ����θ� �����.
	//

	//������ ���¸� �˷��ִ� �迭
	D3DVERTEXELEMENT9 vertElement[6];			//�迭�� �������� ���� + 1

	//Position 
	vertElement[0].Stream = 0;							//Stream �� 0
	vertElement[0].Offset = 0;							//�޸��� ���� Byte ���� 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			//�ڷ��� Ÿ��
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		//���� Ÿ��
	vertElement[0].UsageIndex = 0;						//UsageIndex �ϴ� 0


	//UV
	vertElement[1].Stream = 0;					//Stream �� 0
	vertElement[1].Offset = 12;					//�޸��� ���� Byte ���� 0
	vertElement[1].Type = D3DDECLTYPE_FLOAT2;	//�ڷ��� Ÿ��
	vertElement[1].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[1].Usage = D3DDECLUSAGE_TEXCOORD;		//���� Ÿ��
	vertElement[1].UsageIndex = 0;						//UsageIndex �ϴ� 0

	//Normal
	vertElement[2].Stream = 0;					//Stream �� 0
	vertElement[2].Offset = 20;					//�޸��� ���� Byte ���� 0
	vertElement[2].Type = D3DDECLTYPE_FLOAT3;	//�ڷ��� Ÿ��
	vertElement[2].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[2].Usage = D3DDECLUSAGE_NORMAL;		//���� Ÿ��
	vertElement[2].UsageIndex = 0;						//UsageIndex �ϴ� 0

	//binormal
	vertElement[3].Stream = 0;					//Stream �� 0
	vertElement[3].Offset = 32;					//�޸��� ���� Byte ���� 0
	vertElement[3].Type = D3DDECLTYPE_FLOAT3;	//�ڷ��� Ÿ��
	vertElement[3].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[3].Usage = D3DDECLUSAGE_BINORMAL;		//���� Ÿ��
	vertElement[3].UsageIndex = 0;						//UsageIndex �ϴ� 0


	//Tangent
	vertElement[4].Stream = 0;					//Stream �� 0
	vertElement[4].Offset = 44;					//�޸��� ���� Byte ���� 0
	vertElement[4].Type = D3DDECLTYPE_FLOAT3;	//�ڷ��� Ÿ��
	vertElement[4].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[4].Usage = D3DDECLUSAGE_TANGENT;		//���� Ÿ��
	vertElement[4].UsageIndex = 0;						//UsageIndex �ϴ� 0


	//���̻� ����
	D3DVERTEXELEMENT9 end = D3DDECL_END(); //{0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
	vertElement[5] = end;


	//
	// LPDIRECT3DVERTEXDECLARATION9 ����
	//
	Device->CreateVertexDeclaration(
		vertElement,		//�տ��� ���� D3DVERTEXELEMENT9 �迭 ������
		&m_pVDecl			//���� LPDIRECT3DVERTEXDECLARATION9 ������
		);

	//
	// Texture �ε� 
	//
	//�⺻������ �Ӹ�ü���� ���������.
	D3DXCreateTextureFromFile(
		Device,							//����̽�
		"terrain4.png",					//��������� ���ϸ�
		&this->m_pDiffuseTex			//���� IDirect3DTexture9 ����������
		);

	D3DXCreateTextureFromFile(
		Device,							//����̽�
		"Stone-normal.jpg",					//��������� ���ϸ�
		&this->m_pNormalTex			//���� IDirect3DTexture9 ����������
		);

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
		"TextureNormal2.fx",			//�ҷ��� ���̴� �ڵ� �����̸�
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

void cScene_06::Release()
{
	SAFE_RELEASE(this->pEffect);

	SAFE_RELEASE(this->m_vb);
	SAFE_RELEASE(this->m_ib);

	//����
	SAFE_RELEASE(this->m_pDiffuseTex);

	//����
	SAFE_RELEASE(this->m_pVDecl);
}

void cScene_06::Update(float timeDelta)
{
	m_mainCamera.DefaultControl(timeDelta);
	if (KEY_MGR->IsStayDown(VK_LSHIFT))
		m_lightTrans.DefaultControl2(timeDelta);
	else
		m_quadTrans.DefaultControl2(timeDelta);

	m_mainCamera.UpdateCamToDevice(Device);
}

void cScene_06::Render()
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

	//���� ���
	this->pEffect->SetMatrix("matWorld", &matWorld);

	//���� ����
	D3DXVECTOR3 lightDir = m_lightTrans.GetForward();
	this->pEffect->SetVector("vLightDir", &D3DXVECTOR4(lightDir, 1));

	//�ü� ��ġ
	D3DXVECTOR3 eyePos = m_mainCamera.GetWorldPosition();
	this->pEffect->SetVector("vEyePos", &D3DXVECTOR4(eyePos, 1));

	//POwer
	this->pEffect->SetFloat("specPower", 10);


	//POwer
	this->pEffect->SetTexture("DiffuseTex", m_pDiffuseTex);
	this->pEffect->SetTexture("NormalTex", m_pNormalTex);


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
		//Device->SetFVF( MYVERTEX::FVF );

		//FVF ��ż���
		Device->SetVertexDeclaration(m_pVDecl);



		Device->SetIndices(this->m_ib);
		Device->DrawIndexedPrimitive(
			D3DPT_TRIANGLELIST,
			0, 0, 4, 0, 2);


		this->pEffect->EndPass();			//Pass ��

	}

	this->pEffect->End();						//Effect �� ������ ��



	this->m_lightTrans.RenderGimozo();
}

void cScene_06::CreateQuad()
{
	//���� ��ġ
	D3DXVECTOR3 vertexPos[] = {
		D3DXVECTOR3(-3.5f, -3.5f, 0.0f),
		D3DXVECTOR3(-3.5f, 3.5f, 0.0f),
		D3DXVECTOR3(3.5f, 3.5f, 0.0f),
		D3DXVECTOR3(3.5f, -3.5f, 0.0f)
	};

	//���� UV
	D3DXVECTOR2 vertexUV[] = {
		D3DXVECTOR2(0, 1),
		D3DXVECTOR2(0, 0),
		D3DXVECTOR2(1, 0),
		D3DXVECTOR2(1, 1)
	};

	//���� �ε���
	DWORD vertexIndices[] = { 0, 1, 2, 0, 2, 3 };

	//���� �븻
	D3DXVECTOR3 vertexNormal[4];
	ComputeNormal(vertexNormal, vertexPos, 4, vertexIndices, 6);

	//Tangent bi normal
	D3DXVECTOR3 vertexTangent[4];
	D3DXVECTOR3 vertexBinormal[4];
	ComputeTangentAngBinormal(
		vertexTangent,
		vertexBinormal,
		vertexPos,
		vertexNormal,
		vertexUV,
		vertexIndices,
		2,
		4);



	//���� ���� ����
	Device->CreateVertexBuffer(
		sizeof(MYVERTEX) * 4,
		D3DUSAGE_WRITEONLY,
		0,						//FVF �������� 0 ���... 
		D3DPOOL_DEFAULT,
		&this->m_vb,
		NULL);


	LPMYVERTEX pVertices = NULL;
	this->m_vb->Lock(0, 0, (void**)&pVertices, 0);

	for (int i = 0; i < 4; i++){
		pVertices[i].pos = vertexPos[i];
		pVertices[i].uv = vertexUV[i];
		pVertices[i].normal = vertexNormal[i];
		pVertices[i].tangent = vertexTangent[i];
		pVertices[i].binormal = vertexBinormal[i];
	}

	this->m_vb->Unlock();


	//
	// �ε��� ����
	//
	Device->CreateIndexBuffer(
		sizeof(DWORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&this->m_ib,
		NULL);

	void* pIndices = NULL;

	this->m_ib->Lock(0, 0, &pIndices, 0);

	memcpy(pIndices, vertexIndices, sizeof(DWORD) * 6);

	this->m_ib->Unlock();
}