#include "stdafx.h"
#include "cTerrain.h"


cTerrain::cTerrain()
{
}


cTerrain::~cTerrain()
{
}

//�ʱ�ȭ
HRESULT cTerrain::Init(int vertexNum, float cellSize)
{
	//������ 
	this->nVerNumX = vertexNum;
	this->nVerNumZ = vertexNum;
	this->nTotalVerNum = this->nVerNumX * this->nVerNumZ;

	//����
	this->nCellNumX = this->nVerNumX - 1;
	this->nCellNumZ = this->nVerNumZ - 1;
	this->nTotalCellNum = this->nCellNumX * this->nCellNumZ;

	//��������
	this->fCellScale = cellSize;

	//������ ����
	this->nTotalTri = this->nTotalCellNum * 2;

	//�ͷ��� ������
	this->fTerrainSizeX = this->nCellNumX * cellSize;
	this->fTerrainSizeZ = this->nCellNumZ * cellSize;


	//
	// �ͷ��� ���� ����
	//
	//���� ����� ���� ��ġ ������ �����.
	vertexPoses = new D3DXVECTOR3[nTotalVerNum];

	//���� ������ �ϴ� 0. 0. 0  ��ġ��
	D3DXVECTOR3 startPos(0, 0, 0);

	for (int z = 0; z < this->nVerNumZ; z++)
	{
		for (int x = 0; x < this->nVerNumX; x++)
		{
			//�����ε���
			int idx = z * this->nVerNumX + x;

			//������ġ
			vertexPoses[idx].x = startPos.x + (x * fCellScale);
			vertexPoses[idx].z = startPos.z + (z * fCellScale);
			vertexPoses[idx].y = cos(x * 10 * ONE_RAD) * 3.0f;
		}
	}



	//
	// �����ε���
	//
	pIndices = new TERRAININDEX[this->nTotalTri];

	for (int z = 0; z < this->nCellNumZ; z++)
	{
		//�������
		int nowZ = z;
		int nextZ = z + 1;

		for (int x = 0; x < this->nCellNumX; x++)
		{
			//���� ��������
			int nowX = x;
			int nextX = x + 1;

			// lt-----rt
			//  |    /|
			//  |   / |
			//  |  /  |
			//  | /   |
			//  |/    |
			// lb-----rb

			//�𼭸� ���� �ε���
			DWORD lt = nextZ * this->nVerNumX + nowX;
			DWORD rt = nextZ * this->nVerNumX + nextX;
			DWORD lb = nowZ * this->nVerNumX + nowX;
			DWORD rb = nowZ * this->nVerNumX + nextX;


			//Tri �ε��� �迭 �ε���
			int idx = (z * this->nCellNumX + x) * 2;


			//Cell ������ 1
			//lb, lt, rt, 
			pIndices[idx]._0 = lb;
			pIndices[idx]._1 = lt;
			pIndices[idx]._2 = rt;

			//Cell ������ 1
			//lb, rt, rb, 
			pIndices[idx + 1]._0 = lb;
			pIndices[idx + 1]._1 = rt;
			pIndices[idx + 1]._2 = rb;
		}
	}

	//
	// ���� ����ü ����
	//
	/*

	//Position
	vertElement[0].Stream = 0;							//Stream �� 0
	vertElement[0].Offset = 0;							//�޸��� ���� Byte ���� 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			//�ڷ��� Ÿ��
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		//���� Ÿ��
	vertElement[0].UsageIndex = 0;						//UsageIndex �ϴ� 0
	*/

	D3DVERTEXELEMENT9 vertElement[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END()
	};

	Device->CreateVertexDeclaration(
		vertElement,			//�տ��� ���� D3DVERTEXELEMENT9 �迭 ������
		&this->terrainDecl			//���� LPDIRECT3DVERTEXDECLARATION9 ������
		);



	//
	// ���� ���۸� �����.
	//
	if (FAILED(Device->CreateVertexBuffer(
		sizeof(TERRAINVERTEX) * this->nTotalVerNum,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&this->terrainVb,
		0))) return E_FAIL;

	LPTERRAINVERTTEX pVertices = NULL;

	if (SUCCEEDED(this->terrainVb->Lock(0, 0, (void**)&pVertices, 0)))
	{
		for (int i = 0; i < this->nTotalVerNum; i++)
		{
			//���� ��ġ ����
			pVertices[i].pos = this->vertexPoses[i];
		}

		this->terrainVb->Unlock();
	}

	//
	// �ε��� ����
	//
	if (FAILED(Device->CreateIndexBuffer(
		sizeof(TERRAININDEX) * this->nTotalTri,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&this->terrainIb,
		NULL))) return E_FAIL;

	void* pI = NULL;

	if (SUCCEEDED(this->terrainIb->Lock(0, 0, &pI, 0)))
	{
		memcpy(pI, this->pIndices, sizeof(TERRAININDEX) * this->nTotalTri);

		this->terrainIb->Unlock();
	}


	return S_OK;
}

//����
void cTerrain::Release()
{
	SAFE_DELETE_ARR(this->vertexPoses);
	SAFE_DELETE_ARR(this->pIndices);

	SAFE_RELEASE(this->terrainIb);
	SAFE_RELEASE(this->terrainVb);
}

//����
void cTerrain::Render()
{
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//���� ��� �ϴ� �ʱ�ȭ
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	Device->SetTransform(D3DTS_WORLD, &matWorld);


	Device->SetStreamSource(0, this->terrainVb, 0, sizeof(TERRAINVERTEX));
	Device->SetIndices(this->terrainIb);
	Device->SetVertexDeclaration(this->terrainDecl);
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		this->nTotalVerNum,
		0,
		this->nTotalTri);

	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}